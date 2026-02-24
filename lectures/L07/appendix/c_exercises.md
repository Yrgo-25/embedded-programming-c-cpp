# Bilaga C

## Övningsuppgifter
Ni ska implementera ett interface för en GPIO-driver via en klass `driver::gpio::Interface`.

### Metoder
Interfacet ska inneha följande metoder:
* `~Interface()`: Virtuell destruktor, ska märkas som `= default`.
* `isInitialized()`: Indikerar ifall GPIO-instansen är initierad via ett bool-värde.
* `read()`: Indikerar GPIO-instansens tillstånd via ett bool-värde.
* `write(bool enable)`: Uppdaterar GPIO-instansens tillstånd **om det är en utport**, annars görs ingenting.
* `toggle()`: Togglar GPIO-instansens tillstånd **om det är en utport**, annars görs ingenting.

**Tips:** Låt `isInitialized()` och `read()` vara `const`.

Samtliga metoder (förutom destruktorn) ska deklareras som rent virtuella (`= 0`).

---

### Underklasser
Två GPIO-implementationer ska skapas:

#### **1.** `driver::gpio::Stub`
Stubklass för simulering av hårdvara:
* För att simulera exempelvis knapptryckningar bör metoden `write(bool enable)` fungera oavsett datariktning. I denna implementation ignoreras därmed datariktningen helt.
* För att simulera att GPIO-instansen inte är initierad ska en metod `setInitialized(bool initialized)` läggas till.
* Default-konstruktorn kan användas, då pin-nummer och datariktning är irrelevant för denna implementation.

#### **2** `driver::gpio::Atmega328p`
GPIO-driver för ATmega328p, mycket lik den klass som skapades i [L06](../../L06/README.md):
* Använd `driver::gpio::Atmega328p` från L06 som bas för denna nya driver.
* Ta också enumerationsklassen `driver::gpio::Direction` från L06.

---

### Filstruktur
Använd följande filstruktur:

```
include/
    driver/
        gpio/
            types.h
            atmega328p.h
            interface.h
            stub.h
source/
    arch/
        hw_platform.cpp
    driver/
        gpio/
            atmega328p.cpp
            stub.cpp
    main.cpp
```

---


### Testkod
I `main.cpp`, skapa två GPIO-instanser:
* `led (driver::gpio::Atmega328p)`: Lysdiod ansluten till pin 8 (utport) på ATmega328p.
* `button (driver::gpio::Stub)`: Simulerad tryckknapp. Varken pin-nummer eller datariktning behöver sättas.

Skapa också följande lokala variabler:
* En lokal variabel `bool buttonPrev`, som håller tryckknappens föregående tillstånd.
* En lokal variabel `std::uint16_t loopCount`, som räknar antalet varv i en loop (för att simulera knapptryckningar utan trådar).

Se till att programmet körs kontinuerligt via en while-loop:
* Varje varv, kontrollera tryckknappens nuvarande insignal.
* Om tryckknappen nu är nedtryckt (insignalen är hög), men inte var det förra varvet, ska lysdioden togglas.
* Tryckknappens föregående tillstånd ska uppdateras i slutet av varje varv i while-loopen (inför nästa jämförelse).
* Toggla tryckknappen för att simulera knapptryckningar var 60 000:e varv i while-loopen.

---

### Fix för användning av interfaces i AVR-miljön
När man använder interfaces och/eller statiska lokala variabler i en AVR-miljö (t.ex. ATmega328p) kan man få linker-fel, trots att koden kompilerar utan problem:

Detta beror på att den inbyggda C++-runtime-miljön på små mikrokontrollers är minimal och saknar vissa funktioner som normalt finns på större plattformar (t.ex. PC). Exempel på sådana symboler är:
* `__cxa_pure_virtual()`
* `__cxa_guard_acquire()`
* `__cxa_guard_release()`
* `operator delete(void*, unsigned int)`

Om dessa saknas i projektet uppstår linker-fel.

För att projektet ska länka korrekt måste följande stub-definitioner läggas till manuellt:

```cpp
void operator delete(void*, unsigned int) noexcept {}
extern "C" void __cxa_pure_virtual() {}
extern "C" int __cxa_guard_acquire (volatile void* g) { return !*(char*)g; }
extern "C" void __cxa_guard_release (volatile void* g) { *(char *)g = 1; }
extern "C" void __cxa_guard_abort (volatile void*) {}
```

**OBS:** Lägg inte till ovanstående i en headerfil! Det ska ligga i en .cpp-fil och endast finnas en gång i projektet.

---

#### Vad gör koden?
* `__cxa_pure_virtual` anropas om en rent virtuell metod skulle köras (programmeringsfel).
* `__cxa_guard_*` används internt av kompilatorn för att hantera initiering av statiska lokala variabler.
* `operator delete` krävs av vissa toolchains även om dynamisk allokering inte används.

Detta är en minimal implementation som räcker för embedded-projekt utan full C++ runtime.

---

#### Placering

Lägg hela kodsnutten i `source/arch/hw_platform.cpp`.

Viktigt:
* Koden ska ligga i en `.cpp`-fil (inte i en header).
* Den ska finnas exakt en gång i projektet.
* Om den saknas kommer projektet inte att länka.

---

