
# Bilaga C

## Övningsuppgifter

**1.** Betrakta nedanstående klass `driver::gpio::Led`:

```cpp
#include <cstdint>

namespace driver::gpio
{
class Led final
{
public:
    explicit Led(const std::uint8_t pin, const bool initialState = false) noexcept 
        : myPin{pin} 
        , myState{initialState}
    {}
    ~Led() noexcept = default;

    bool isOn() const noexcept { return myState; }
    void on() noexcept { myState = true; }
    void off() noexcept { myState = false; }
    void toggle() noexcept { myState = !myState; }

    Led()                      = delete;
    Led(const Led&)            = delete;
    Led(Led&&)                 = delete;
    Led& operator=(const Led&) = delete;
    Led& operator=(Led&&)      = delete;

private:
    const std::uint8_t myPin;
    bool myState;
};
} // namespace driver::gpio
```

Svara på följande:
* Vad har klassen för syfte?
* Vad kan man göra med en lysdiod via denna klass?
* Vilka medlemsvariabler finns i klassen?
* Varför ligger de under `private`?
* Vad händer när man skriver:

```cpp
Led led1{8U};
```

---

**2.** Skriv en enkel klass `driver::gpio::Button` med:
* En privat medlemsvariabel som lagrar pin-numret.
* En privat medlemsvariabel som lagrar tryckknappens insignal som ett bool-värde.
* En konstruktor som sätter pin-numret. Insignalen ska initialt vara false (ej tryckt).
* En metod som returnerar pin-numret.
* En metod som returnerar tryckknappens insignal.
* En metod som simulerar knapptryckning (exempelvis `setPressed(bool)`).

Radera kopierings- och förflyttningskonstruktorerna och motsvarande operatorer.  
Klassen ska inte heller kunna ärvas vidare.

---
