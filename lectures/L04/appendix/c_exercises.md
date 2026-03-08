# Bilaga C

## Övningar
Dessa övningar tränar koncept från [Bilaga B](./b_from_c_to_cpp.md).

---

## Övningsset 1 – Standardargument och funktioner

### Övning 1.1 – Debug-loggare
Skapa en namnrymd `debug` som innehåller en funktion:

```cpp
void log(const char* message, std::uint8_t level = 0U);
```

Uppgifter:
1. Implementera funktionen så att den skriver ut meddelandet med `std::printf` från `<cstdio>`.
2. Inkludera loggnivån i utskriften.
3. Markera funktionen med `noexcept`.

Exempel på användning:

```cpp
debug::log("System started");
debug::log("Sensor failure", 2U);
```

Exempel på utdata:

```text
System started, log level = 0
Sensor failure, log level = 2
```

---

### Övning 1.2 – Systemfördröjning
I samma program som i Övning 1.1, skapa en funktion som simulerar en enkel mjukvarufördröjning:

```cpp
void delay_ms(std::uint32_t ms = 1U);
```

Uppgifter:
1. Placera funktionen i namnrymden `system`.  
2. Markera funktionen med `noexcept`.  
3. Inuti funktionen, definiera en `constexpr`-konstant med namnet `maxCount` och tilldela den värdet `10000000UL`.  
4. Implementera en fördröjning med hjälp av nästlade loopar:
    * Den yttre loopen ska iterera `ms` gånger.
    * Den inre loopen ska iterera upp till `maxCount`.
    * För att minska risken att kompilatorn optimerar bort loopen, lägg till en volatil dummyvariabel `volatile std::uint32_t dummy` inuti funktionen och inkrementera den i den inre loopen.
5. Använd funktionen för att simulera en mjukvarufördröjning på `100 ms` mellan debug-utskrifterna i övning 1.1.

---

## Övningsset 2 – Struktdrivers

### Övning 2.1 – UART-stubbdriver
I ett nytt program, skapa en enkel UART-driver:

```cpp
namespace driver
{
struct Uart
{
    const std::uint32_t baudrate;
    bool initialized;

    void init() noexcept;
    void send(std::uint8_t byte) const noexcept;
};
} // namespace driver
```

Uppgifter:
1. Implementera de två metoderna:
   * `init()` ska sätta `initialized` till `true`.
   * `send()` ska skriva ut den givna byten med `std::printf` från `<cstdio>`:
     * Skriv endast ut byten om UART:en är initialiserad, dvs. om `initialized` är `true`.
     * Skriv ut byten som ett osignerat heltal med format-specifikatorn `%u`.

2. Skapa ett UART-objekt:

```cpp
driver::Uart uart{9600U, false};
```

3. Försök att skicka några bytes innan UART:en initialiseras.  
Säkerställ att inget skrivs ut i terminalen.

4. Anropa `init()` och skicka några bytes igen.  
Säkerställ att byten skrivs ut i terminalen.

---

### Övning 2.2 – Sensor-stubbdriver
I samma program som i Övning 2.1, skapa en enkel stubb för en digital sensor:

```cpp
namespace driver
{
struct Sensor
{
    std::int16_t value;
    bool enabled;

    void enable() noexcept;
    void disable() noexcept;
    std::int16_t read() const noexcept;
};
} // namespace driver
```

Uppgifter:
1. Implementera de tre metoderna:
    * `enable()` ska sätta `enabled` till `true`.
    * `disable()` ska sätta `enabled` till `false`.
    * `read()` ska returnera `value` om `enabled` är `true`, annars `0`.

2. Skapa en sensorinstans:
    * Ge instansen namnet `tempSensor`.
    * Sätt sensorvärdet till `25`.
    * Sätt sensorn till inaktiverad vid uppstart.

3. Skapa en UART-instans från Övning 2.1 och initialisera den så att temperaturen kan skrivas ut.

4. Läs värdet. Säkerställ att värdet är `0`, eftersom sensorn är inaktiverad.

5. Aktivera sensorn och läs värdet igen. Säkerställ att värdet är `25`.

Exempel på utdata:

```text
Temperature reading 1: 0
Temperature reading 2: 25
```

---

### Övning 2.3 – Mjukvarutimer
Skapa en enkel timer-driver:

```cpp
namespace driver
{
struct Timer
{
    const std::uint32_t timeout_ms;
    std::uint32_t counter_ms;
    bool running;

    void start() noexcept;
    void stop() noexcept;
    void toggle() noexcept;
    void tick() noexcept;
    bool timeout() noexcept;
};
} // namespace driver
```

Uppgifter:
1. Implementera de fem metoderna:
    * `start()` ska sätta `running` till `true`.
    * `stop()` ska sätta `running` till `false`.
    * `toggle()` ska toggla värdet på `running`.
    * `tick()` ska öka `counter_ms` om `running` är `true`, annars göra ingenting.
    * `timeout()` ska returnera `true` om `counter_ms >= timeout_ms`, annars `false`:
        * `counter_ms` ska återställas till `0` om `counter_ms >= timeout_ms`.

2. Skapa en timerinstans:
    * Ge instansen namnet `timer`.
    * Sätt timeouten till `1000 ms`.
    * Initiera den interna räknaren till `0`.
    * Sätt timern till körande vid uppstart.

3. Skapa en loop som körs i 5000 iterationer:
    * Anropa `tick()` i varje iteration.
    * Anropa `timeout()` för att kontrollera om timern har löpt ut.
    * Skriv ut `Timeout!` med `std::printf` från `<cstdio>` varje gång timern genererar en timeout.

---

## Övningsset 3 – Referenser

### Övning 3.1 – Tilldela värde
I ett nytt program, implementera följande funktion i en anonym namnrymd:

```cpp
constexpr void assign(std::uint8_t& byte) noexcept;
```

Uppgifter:
1. Tilldela värdet `0xFFU` till `byte`.
2. Testa funktionen med:

```cpp
std::uint8_t num{};
assign(num);
std::printf("num = %u\n", static_cast<unsigned>(num));
```

Förväntad utdata:

```cpp
num = 255
```

---

### Övning 3.2 – Byt värden
Implementera följande funktion i samma anonyma namnrymd som i Övning 3.1:

```cpp
constexpr void swap(std::uint32_t& a, std::uint32_t& b) noexcept;
```

Uppgifter:
1. Byt värdena med hjälp av en temporär variabel `temp`.
2. Testa funktionen med två variabler.

Exempel på utdata:

```text
Before swap: a = 3, b = 10
After swap: a = 10, b = 3
```

---

## Övningsset 4 – Bitmanipulation med templates

### Övning 4.1 – Rensa bit
I en ny fil, skapa en funktionstemplate i en anonym namnrymd som rensar en bit i ett register:

```cpp
template<typename T>
constexpr void clear(T& reg, std::uint8_t bit) noexcept;
```

Uppgifter:
1. Använd `static_assert` i kombination med `std::is_integral<T>::value` från `<type_traits>` för att säkerställa att `T` är en heltalstyp.
2. Använd felmeddelandet `Cannot perform bit operation with non-integral type!` i `static_assert`.
3. Rensa den valda biten i registret.
4. Skriv ut resultatet i binär form med C++-funktionalitet för in- och utmatning:
    * Utströmmen `std::cout` från `<iostream>` för utskrift.
    * `std::bitset<N>` från `<bitset>` för att generera en bitset-representation.

Exempel på användning:

```cpp
std::uint8_t reg{0xFFU};
clear(reg, 2U);
std::cout << "Register content: " << std::bitset<8>(reg) << "\n";
```

Förväntad utdata:

```text
Register content: 11111011
```

---

### Övning 4.2 – Toggla en bit
Skapa en funktionstemplate för att toggla en eller flera bitar i ett register med hjälp av ett parameter pack i samma anonyma namnrymd som i Övning 4.1:

```cpp
template<typename T, typename... Bits>
void toggle(T& reg, const Bits&... bits) noexcept;
```

Uppgifter:
1. Använd ett parameter pack.
2. Säkerställ att typen är en heltalstyp som i Övning 4.1.
3. Iterera över bitarna (till exempel med `{bits...}`).
4. Toggla alla angivna bitar i registret.
5. Skriv ut resultatet i binär form med C++-funktionalitet för in- och utmatning:
    * Utströmmen `std::cout` från `<iostream>` för utskrift.
    * `std::bitset<N>` från `<bitset>` för att generera en bitset-representation.

Exempel på användning:

```cpp
std::uint8_t reg{0xFFU};
clear(reg, 2U);
toggle(reg, 0U, 2U, 4U, 6U);
std::cout << "Register content: " << std::bitset<8>(reg) << "\n";
```

Förväntad utdata:

```text
Register content: 10100101
```

---
