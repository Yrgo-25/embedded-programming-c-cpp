/**
 * @brief Implementation details of hardware timer driver.
 */
#include <stdint.h>
#include <stdlib.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include "driver/timer/atmega328p.h" 
#include "utils/bit_operations.h"

namespace driver 
{
namespace timer
{
/**
 * @brief Structure for implementation of timer hardware.
 */
struct Atmega328p::Hardware 
{
    /** Hardware counter. */
	volatile uint32_t counter;

    /** Pointer to mask register. */
	volatile uint8_t* maskReg;

    /** Mask bit for timer interrupt. */
	uint8_t maskBit;

    /** Timer index. */
	uint8_t index;  

    static Hardware* reserve() noexcept;
	static void release(Hardware* hw) noexcept;

private:
    static Hardware* init(const uint8_t timerIndex) noexcept;
}; 

namespace
{  
/**
 * @brief Structure containing indexes for available timer circuits.
 */
struct Index
{
	/** Index for Timer 0. */
	static constexpr uint8_t Timer0{0U};

	/** Index for Timer 1. */
	static constexpr uint8_t Timer1{1U};

	/** Index for Timer 2. */
	static constexpr uint8_t Timer2{2U};
};

/** Timer callback. */
typedef void (*Callback)();

/** The number of available timer circuits. */
constexpr uint8_t CircuitCount{3U};

/** Time between each timer interrupt in ms. */
constexpr double InterruptIntervalMs{0.128};

/** Array holding pointers to timers. */
Atmega328p* myTimers[CircuitCount]{};  

/** Callback registry. */
Callback myCallbackReg[CircuitCount]{};

// -----------------------------------------------------------------------------
constexpr uint32_t round(const double number) noexcept 
{
    return static_cast<uint32_t>(0.0 <= number ? number + 0.5 : number - 0.5);
}

// -----------------------------------------------------------------------------
constexpr bool isTimeoutValid(const uint32_t timeout_ms) noexcept
{
	return 0U < timeout_ms;
}

// -----------------------------------------------------------------------------
constexpr uint32_t maxCount(const uint32_t timeout_ms) noexcept
{
	return isTimeoutValid(timeout_ms) ? round(timeout_ms / InterruptIntervalMs) : 0U;
}

// -----------------------------------------------------------------------------
void invokeCallback(const uint8_t timerIndex) noexcept
{
	// Check the timer index, terminate the function if invalid.
	if (CircuitCount <= timerIndex) { return; }
    
	// Invoke callback.
	Atmega328p* timer{myTimers[timerIndex]};
    if (nullptr != timer) { timer->handleCallback(); }
}
} // namespace

// -----------------------------------------------------------------------------
Atmega328p::Atmega328p(const uint32_t timeout_ms, void (*callback)(), 
                       const bool startTimer) noexcept
    : myHw{isTimeoutValid(timeout_ms) ? Hardware::reserve() : nullptr}
	, myMaxCount{maxCount(timeout_ms)}
	, myRunning{false}
{
    if (nullptr == myHw) { return; }
	myTimers[myHw->index] = this;
	addCallback(callback);
	if (startTimer) { start(); }
}

// -----------------------------------------------------------------------------
Atmega328p::~Atmega328p() noexcept 
{ 
	// Skip cleanup if the timer is uninitialized.
	if (!isInitialized()) { return; }
	removeCallback();
	myTimers[myHw->index] = nullptr;
	Hardware::release(myHw); 
}

// -----------------------------------------------------------------------------
bool Atmega328p::isInitialized() const noexcept { return nullptr != myHw; }

// -----------------------------------------------------------------------------
bool Atmega328p::isRunning() const noexcept { return myRunning; }

// -----------------------------------------------------------------------------
bool Atmega328p::hasTimedOut() const noexcept
{
    return myRunning && (myHw->counter >= myMaxCount);
}

// -----------------------------------------------------------------------------
uint32_t Atmega328p::timeout_ms() const noexcept
{
	return round(myMaxCount * InterruptIntervalMs);
}

// -----------------------------------------------------------------------------
void Atmega328p::setTimeout_ms(const uint32_t timeout_ms) noexcept
{
	// Ignore invalid timeouts.
    if (0U == timeout_ms) { return; }
    myMaxCount = maxCount(timeout_ms);
}

// -----------------------------------------------------------------------------
void Atmega328p::start() noexcept
{ 
	if (0U == myMaxCount) { return; }
    sei();
	bit::set(*(myHw->maskReg), myHw->maskBit);
	myRunning = true;
}

// -----------------------------------------------------------------------------
void Atmega328p::stop() noexcept
{ 
    *(myHw->maskReg) = 0U;
	myRunning        = false; 
}

// -----------------------------------------------------------------------------
void Atmega328p::toggle() noexcept 
{ 
	if (myRunning) { stop(); }
	else { start(); }
}

// -----------------------------------------------------------------------------
void Atmega328p::restart() noexcept
{
    myHw->counter = 0U;
    start();
}

// -----------------------------------------------------------------------------
void Atmega328p::handleCallback() noexcept
{
	// Increment the timer, invoke callback on timeout.
	increment();

	if (hasTimedOut()) 
	{ 
		Callback callback{myCallbackReg[myHw->index]};
		if (nullptr != callback) { callback(); }
		clearTimedOut();
	}
}

// -----------------------------------------------------------------------------
void Atmega328p::addCallback(void (*callback)()) const noexcept
{ 
	myCallbackReg[myHw->index] = callback;
}

// -----------------------------------------------------------------------------
void Atmega328p::removeCallback() const noexcept 
{ 
	myCallbackReg[myHw->index] = nullptr;
}

// -----------------------------------------------------------------------------
bool Atmega328p::increment() noexcept
{
	if (!myRunning) { return false; }
	myHw->counter++; 
	return true;
}

// -----------------------------------------------------------------------------
void Atmega328p::clearTimedOut() noexcept { myHw->counter = 0U; }

// -----------------------------------------------------------------------------
Atmega328p::Hardware* Atmega328p::Hardware::reserve() noexcept
{
	// Reserve a timer circuit if any is available, otherwise return a nullptr.
    for (uint8_t i{}; i < CircuitCount; ++i)
	{
        if (nullptr == myTimers[i]) { return init(i); }
	}
	return nullptr;
}

// -----------------------------------------------------------------------------
void Atmega328p::Hardware::release(Atmega328p::Hardware* hw) noexcept
{
	// Reset the associated hardware timer.
    *(hw->maskReg) = 0U;

	switch (hw->index)
	{
		case Index::Timer0:
		{
			TCCR0B = 0U;
			break;
		}
		case Index::Timer1:
		{
			TCCR1B = 0U;
		    OCR1A = 0U;
			break;
		}
		case Index::Timer2:
		{
			TCCR2B = 0U;
			break;
		}
		default:
		{
			break;
		}
	}
	// Release allocated resources.
	free(hw);
}

// -----------------------------------------------------------------------------
Atmega328p::Hardware* Atmega328p::Hardware::init(const uint8_t timerIndex) noexcept
{
    constexpr uint16_t timer1MaxCount{256U};  
	constexpr uint8_t controlBits0{(1U << CS01)};
	constexpr uint8_t controlBits1{(1U << CS11) | (1U << WGM12)};
	constexpr uint8_t controlBits2{(1U << CS21)};

	// Allocate memory for the new timer hardware, return false is memory allocation failed.
    Hardware* hw{static_cast<Hardware*>(malloc(sizeof(Hardware)))};
	if (nullptr == hw) { return nullptr; }

	// Set the structure to refer to the corresponding timer circuit.
	switch (timerIndex)
	{
		case Index::Timer0:
        {
			hw->maskReg = &TIMSK0;
            hw->maskBit = TOIE0;
			TCCR0B      = controlBits0;
			break;
		}
		case Index::Timer1:
        {
			hw->maskReg = &TIMSK1;
            hw->maskBit = OCIE1A;
			TCCR1B      = controlBits1;
		    OCR1A       = timer1MaxCount;
			break;
		}
		case Index::Timer2:
		{
			hw->maskReg = &TIMSK2;
            hw->maskBit = TOIE2;
			TCCR2B      = controlBits2;
			break;
		}
		default:
		{
			free(hw);
			return nullptr;
		}
	}
	// Return the initialized circuit.
    hw->counter = 0U;
	hw->index   = timerIndex;
	return hw;
}

// -----------------------------------------------------------------------------
ISR (TIMER0_OVF_vect) { invokeCallback(Index::Timer0); }

// -----------------------------------------------------------------------------
ISR (TIMER1_COMPA_vect) { invokeCallback(Index::Timer1); }

// -----------------------------------------------------------------------------
ISR (TIMER2_OVF_vect) { invokeCallback(Index::Timer2); }

} // namespace timer
} // namespace driver
