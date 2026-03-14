# Bilaga C

## Övningar
Dessa övningar tränar koncepten från [Bilaga B](./b_from_c_to_cpp.md).

---

## Övningsset 1 – Standardargument och funktioner

### Övning 1.1 – Debug logger
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

## Övningsset 2 – Struktdriver

### Övning 2.1 – Mjukvarutimer
I denna övning ska du implementera en enkel mjukvarutimer-driver `driver::Timer` i en headerfil `driver/timer.h`.

Lägg till följande rader högst upp i filen:

```cpp
/**
 * @brief Timer driver implementation.
 */
#pragma once

#include <cstdint>
#include <cstdio>
```

Drivern ska simulera en timer som räknar millisekunder och genererar en timeout när ett konfigurerat timeout-värde nås.

Strukten ska:
* Använda privata medlemsvariabler med prefixet `my`.
* Ha en konstruktor.
* Ha en destruktor.

Använd `std::printf` från `<cstdio>` för terminalutskrifter.

---

### a) Privata medlemsvariabler
Lägg till tre privata medlemsvariabler:
* Den första medlemsvariabeln ska:
  * Lagra timeout i millisekunder.
  * Ha typen `const std::uint16_t`.
  * Heta `myTimeout_ms`.
* Den andra medlemsvariabeln ska:
  * Lagra den interna räknaren i millisekunder.
  * Ha typen `std::uint16_t`.
  * Heta `myCounter_ms`.
* Den tredje medlemsvariabeln ska:
  * Ange om timern körs.
  * Ha typen `bool`.
  * Heta `myRunning`.

---

### b) Konstruktor
Lägg till en konstruktor som:
* Tar två parametrar:
  * Timeout i millisekunder.
  * Startläget, som ska vara `false` som default.
* Initierar:
  * `myTimeout_ms` med det angivna timeout-värdet.
  * `myCounter_ms` med `0U`.
  * `myRunning` med `false`.
* Skriver ut att timern skapas.
* Anropar den publika metoden `start()` om startläget är `true`.

Exempelutskrift:

```text
Creating timer!
```

### c) Destruktor
Lägg till en destruktor som:
* Anropar den publika metoden `stop()`.
* Skriver ut att timern förstörs.

Exempelutskrift:

```text
Destroying timer!
```

### d) Publika metoder
Lägg till följande publika metoder:
* `timeout_ms()` ska:
    * Returnera `myTimeout_ms`.
* `isRunning()` ska:
    * Returnera `myRunning`.
* `start()` ska:
  * Sätta `myRunning` till `true`.
  * Skriva ut `Starting timer!`.
* `stop()` ska:
  * Sätta `myRunning` till `false`.
  * Skriva ut `Stopping timer!`.
* `toggle()` ska:
  * Toggla `myRunning`.
  * Skriva ut att timern togglades och om den nu körs eller är stoppad:
    * `Toggling timer: running!` när den är aktiv.
    * `Toggling timer: stopped!` när den är inaktiv.
* `tick()` ska:
  * Öka `myCounter_ms` om `myRunning` är `true` (det motsvarar att `1 ms` har passerat).
  * Annars ska inget göras.
* `hasTimedOut()` ska:
  * Returnera `true` om `myCounter_ms >= myTimeout_ms`, annars `false`.
  * Om `myCounter_ms >= myTimeout_ms`:
    * `myCounter_ms` ska återställas till `0U`.

### e) Skapa och använd en timerinstans
I `main()`:
* Skapa en timerinstans:
  * Ge instansen namnet `timer`.
  * Sätt timeout till `1000 ms`.
  * Sätt timern till körande vid uppstart.
* Skapa en loop som kör i `5000` iterationer:
  * Anropa `tick()` i varje iteration.
  * Anropa `hasTimedOut()` för att kontrollera om timern har löpt ut.
  * Skriv ut `Timeout after x ms!`, där `x` är det konfigurerade timeout-värdet, varje gång timern löper ut.

Eftersom timerns timeout är `1000 ms` och loopen kör i `5000` iterationer ska timern generera fem timeouter.

Exempelutskrift:

```text
Creating timer!
Starting timer!
Timeout after 1000 ms!
Timeout after 1000 ms!
Timeout after 1000 ms!
Timeout after 1000 ms!
Timeout after 1000 ms!
Stopping timer!
Destroying timer!
```

---

## Övningsset 3 – Referenser

### Övning 3.1 – Swappa värden
Implementera följande funktion i en anonym namnrymd:

```cpp
constexpr void swap(std::uint32_t& a, std::uint32_t& b) noexcept;
```

Uppgifter:
1. Swappa värdena med hjälp av en temporär variabel `temp`.
2. Testa funktionen med två variabler.

Exempel på utdata:

```text
Before swap: a = 3, b = 10
After swap: a = 10, b = 3
```

---

## Övningsset 4 – Bitmanipulation med templates

### Övning 4.1 – Rensa bit
Skapa ett funktionstemplate som nollställer en bit i ett register:

```cpp
template<typename T>
constexpr void clear(T& reg, std::uint8_t bit) noexcept;
```

Implementera detta funktionstemplate i en anonym namnrymd.

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
std::cout << "Register content: " << std::bitset<8U>(reg) << "\n";
```

Förväntad utdata:

```text
Register content: 11111011
```

---

### Övning 4.2 – Toggla en eller flera bitar
Skapa ett funktionstemplate för att toggla en eller flera bitar i ett register med hjälp av ett parameterpack:

```cpp
template<typename T, typename... Bits>
void toggle(T& reg, const Bits... bits) noexcept;
```

Implementera detta funktionstemplate i samma anonyma namnrymd som Övning 4.1.

Uppgifter:
1. Använd ett parameterpack.
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
std::cout << "Register content: " << std::bitset<8U>(reg) << "\n";
```

Förväntad utdata:

```text
Register content: 10100101
```

---
