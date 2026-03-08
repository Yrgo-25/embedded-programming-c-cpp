# Bilaga B

## Från C till C++

### 1. Varför använda C++ i inbyggda system?
Inbyggda system använder i allt högre grad C++ eftersom det möjliggör:
* Bättre abstraktion (via klasser och interface).
* Starkare typsäkerhet genom striktare kontroller vid kompileringstid.
* Garantier vid kompileringstid (t.ex. `static_assert`, templates).
* Förbättrad modularitet och tydligare ansvarsfördelning.
* Återanvändbara och testbara drivrutiner.

Det är viktigt att notera att C++ ger dessa fördelar samtidigt som språket fortfarande tillåter hårdvarunära åtkomst och så kallade **zero-cost abstractions**.

Till skillnad från många C++-applikationer för desktopmiljöer använder inbyggda system ofta:
* Inga undantag (och markerar funktioner med `noexcept`).
* Undviker dynamisk minnesallokering (`malloc/free` eller `new/delete`), eller begränsar den till systeminitialisering.
* Föredrar deterministisk exekvering, vilket innebär att exekveringstiden för kritiska kodvägar måste vara förutsägbar och begränsad.
---

### 2. Viktiga skillnader mellan C och C++

| Funktionalitet            | C          | C++       |
|---------------------------|------------|-----------|
| Namnrymder                | Nej        | Ja        |
| Klasser                   | Nej        | Ja        |
| Templates                 | Nej        | Ja        |
| Referenser                | Nej        | Ja        |
| Polymorfism vid kompileringstid | Nej | Ja        |
| Standardcontainrar        | Begränsat  | Rik STL   |
| RAII                      | Nej        | Ja        |

**Notering**: RAII *(Resource Acquisition Is Initialization)* innebär att en resurs allokeras när ett objekt skapas och automatiskt frigörs när objektet går ur sitt scope.

---

### 3. Namnrymder
Namnrymder används för att undvika symbolkollisioner i stora mjukvarusystem.  
De kan jämföras med mappar på en dator:

* Precis som olika mappar kan innehålla filer med samma namn kan olika namnrymder innehålla funktioner, klasser, variabler och andra entiteter med samma namn.
* Till exempel kan två olika namnrymder båda innehålla en funktion som heter `init()` utan att orsaka en konflikt.

En namnrymd definieras med nyckelordet `namespace`, följt av ett namn och ett kodblock inom klamrar.

Nedan visas ett enkelt exempel på en namnrymd som heter `driver`:

``` cpp
namespace driver
{

} // namespace driver
```

Allt som deklareras i detta block blir en del av namnrymden `driver`.

Till exempel kan namnrymden innehålla en funktion som heter `init()`:

``` cpp
namespace driver
{
void init() 
{
    // Driver initialization code.
}
} // namespace driver
```

För att anropa denna funktion utanför namnrymden prefixar vi den med namnrymdens namn följt av scope resolution-operatorn `::`:

``` cpp
int main()
{
    driver::init();
    return 0;
}
```

**OBS!**:
* Det mesta av funktionaliteten i C++ standardbibliotek nås via namnrymden `std`.
* Därför är det vanligt att skriva `std::` före typer och funktioner från standardbiblioteket, till exempel `std::uint8_t`, `std::uint16_t`, `std::size_t` och `std::printf`.
* Funktionalitet från C-biblioteket finns också tillgänglig i C++, men inkluderas vanligtvis via C++-headers som `<cstdint>`, `<cstdio>` och `<cstring>`.
* I modern C++ föredras dessa headers generellt framför de äldre C-formerna `<stdint.h>`, `<stdio.h>` och `<string.h>`.

#### Anonyma namnrymder
I C++ kan en anonym namnrymd användas för att begränsa symboler (variabler, funktioner, typer och konstanter vid kompileringstid) till en enda **translation unit** (dvs. en enda källkodsfil).

Detta liknar användningen av nyckelordet `static` för funktioner eller variabler i C.

Anonyma namnrymder används vanligtvis i `.cpp`-filer för att dölja interna hjälpfunktioner eller variabler som inte ska vara synliga utanför filen.

Exempel:

``` cpp
namespace
{
// Content only visible within this source file.
void helper() {}
} // namespace
```

### Nästlade namnrymder
Namnrymder kan också nästlas, på samma sätt som mappar kan innehålla undermappar.

Anta till exempel följande katalogstruktur:

``` text
include/
    driver/
        gpio.h
        timer.h
source/
    driver/
        gpio.cpp
        timer.cpp
    main.cpp
```

En vanlig konvention är:
* Innehållet i `driver/gpio.h` och `driver/gpio.cpp` tillhör namnrymden `driver::gpio`.
* Innehållet i `driver/timer.h` och `driver/timer.cpp` tillhör namnrymden `driver::timer`.

### Syntax för nästlade namnrymder
Modern C++ tillåter att nästlade namnrymder deklareras i en kompakt form:

``` cpp
namespace driver::gpio
{
void init() 
{
    // GPIO initialization code.
}
} // namespace driver::gpio
```

Före C++17 skrevs samma namnrymd så här:

``` cpp
namespace driver
{
namespace gpio
{
void init() 
{
    // GPIO initialization code.
}
} // namespace gpio
} // namespace driver
```

### Använda funktioner från nästlade namnrymder
En funktion definierad i namnrymden `driver::gpio` kan anropas med sitt fullt kvalificerade namn:

``` cpp
int main()
{
    driver::gpio::init();
    return 0;
}
```

Det gör det tydligt vilken modul funktionen tillhör och förhindrar namnkonflikter i större projekt.

**Sammanfattat**: I inbyggda system används namnrymder ofta för att organisera drivrutiner, hårdvaruabstraktionslager (HAL) och kommunikationsprotokoll.

---

### 4. `constexpr`
Nyckelordet `constexpr` anger att ett värde eller ett uttryck (inklusive enkla funktioner) ska utvärderas vid kompileringstid i stället för vid körningstid. Detta gör att kompilatorn kan beräkna värden i förväg, vilket kan förbättra prestandan och garantera att vissa värden förblir
konstanta.

I inbyggda system är `constexpr` särskilt användbart för att definiera hårdvarurelaterade konstanter, såsom klockfrekvenser, registeroffsetar, bufferstorlekar och bitmasker.

Till exempel:

``` cpp
/** CPU frequency in Hz. */
constexpr std::uint32_t cpuFrequency{16000000U};

/** Baud rate in bps (bits per second). */
constexpr std::uint32_t baudrate{115200U};

/** Buffer length in bytes. */
constexpr std::size_t bufLen{20U};
```

**OBS**: Den universella initieraren `{}` används i stället för `=`
ovan:
* Detta är fördelaktigt eftersom `{}` kan användas för att initiera allt.
* Ett undantag är när nyckelordet `auto` används före C++17; då bör `auto` användas tillsammans med `=` för att undvika att så kallade initializer lists skapas.

Eftersom dessa värden är kända vid kompileringstid kan kompilatorn ersätta deras användning direkt med det beräknade värdet i den genererade maskinkoden.

Jämfört med makron (`#define`) har `constexpr` flera fördelar:
* Det är typsäkert.
* Det följer normala C++-regler för scope (funktioner, lokala block och namnrymder).
* Det kan användas i konstanta uttryck.

Till exempel kan `constexpr`-värden användas för:
* Arraystorlekar.
* Templateparametrar.
* Beräkningar vid kompileringstid.

I modern C++ föredras `constexpr` generellt framför `#define` när konstanter ska definieras.

`constexpr` kan även användas för funktioner när värdena är kända vid kompileringstid:

``` cpp
constexpr int add(const int x, const int y) { return x + y; }
```

---

### 5. `noexcept`
Nyckelordet `noexcept` anger att en funktion garanterat inte kastar undantag.

Som referens visas nedan ett exempel där ett undantag kastas. I detta exempel kastas ett undantag om värdet på `x` överstiger `100U`:

```cpp
#include <cstdint>
#include <stdexcept>

int main()
{
    constexpr std::size_t limit{100U};
    std::uint8_t x{};
    
    while (1)
    {
        if (x++ >= limit)
        {
            throw std::invalid_argument("This is an exception!");
        }
    }
    return 0;
}
```

Undantag (*exceptions*) är en mekanism som används i många C++-program för att signalera fel vid körningstid:
* När ett undantag kastas börjar programmet med **stack unwinding**, vilket innebär att anropsstacken gås igenom tills ett matchande `catch`-block hittas.
* Om en funktion som är markerad `noexcept` försöker kasta ett undantag kommer programmet att termineras omedelbart.

I många inbyggda system är undantag helt inaktiverade. Detta görs vanligtvis för att:
* Minska binärstorleken.
* Förenkla runtime-miljön.
* Säkerställa deterministiskt beteende.

I sådana system hanteras fel vanligtvis med C-liknande mekanismer såsom:
* Returkoder.
* Statusflaggor.
* Fel-callbacks.

Av denna anledning är det vanlig praxis i embedded C++ att markera funktioner som inte förväntas misslyckas med nyckelordet `noexcept`.

Detta har flera fördelar:
* Det dokumenterar att funktionen inte kan kasta undantag.
* Det möjliggör ytterligare kompilatoroptimeringar.
* Det undviker behovet av stackhantering relaterad till undantag.

Exempel:

```cpp
namespace driver::gpio
{
bool read() noexcept { return false; }
} // namespace driver::gpio
```

**Viktigt:**
* I inbyggda drivrutiner markeras funktioner som utför enkla hårdvaruoperationer (såsom att läsa eller skriva register) vanligtvis med `noexcept`, eftersom de inte förväntas kasta undantag.
* Funktioner som kan utföra dynamisk minnesallokering, till exempel att ändra storleken på en vektor, bör inte markeras med `noexcept`, eftersom minnesallokering kan misslyckas.

---

### 6. Defaultargument
C++ tillåter att funktioner anger standardvärden för parametrar:
* Om anroparen inte anger ett värde för en parameter används standardvärdet automatiskt.
* Detta kan förenkla API:er genom att låta vanliga fall använda färre argument.

Till exempel, betrakta funktionen `log()` nedan:

``` cpp
namespace debug
{
void log(const char* message, std::uint8_t level = 0U)
{
    // Log message with the specified level.
}
} // namespace debug
```

Parametern `level` har standardvärdet `0U`. Detta innebär att funktionen kan anropas på två olika sätt:
* Om loggnivån inte är viktig kan funktionen anropas enbart med meddelandet:

``` cpp
debug::log("System started");
```

I detta fall kommer värdet `0U` automatiskt att användas för `level`.

* Om en specifik loggnivå önskas kan anroparen ange denna explicit:

``` cpp
debug::log("Driver failure", 2U);
```

Standardargument används ofta i inbyggd mjukvara för funktioner som har ett rimligt standardbeteende, till exempel:
* Loggningssystem.
* Konfigurationsfunktioner för drivrutiner.
* Initialiseringsrutiner.

De gör att API:er kan förbli flexibla samtidigt som vanlig användning hålls enkel.

---

### 7. Struktar med medlemsfunktioner (metoder)
I C++ kan en strukt inte bara innehålla datamedlemmar utan även medlemsfunktioner, eller metoder som de vanligtvis kallas.  
Detta gör att relaterad data och operationer kan grupperas tillsammans i en och samma typ.

Betrakta följande `GPIO`-drivrutin, implementerad som en stubb (för simulering):

```cpp
#include <cstdint>

namespace driver
{
/**
 * @brief GPIO stub driver.
 */
struct Gpio
{
    /** Pin the GPIO is connected to. */
    const std::uint8_t pin;

    /** GPIO state. */
    bool state;

    /**
     * @brief Set GPIO state.
     * 
     * @param[in] state GPIO state (true = enabled, false = disable).
     */
    void write(const bool state) noexcept
    {
        // Note: The 'this' keyword is used here to refer to our member variable 'state',
        // since we have an input argument with the same name.
        this->state = state;
    }

    /**
     * @brief Get GPIO state.
     *
     * @return True if the GPIO is enabled, false otherwise.
     *
     * @note The 'const' keyword is used after the method  name to set the GPIO instance to
     *       read-only in the scope of this method.
     */
    bool read() const noexcept { return state; }
};
} // namespace driver
```

Strukten `driver::Gpio` innehåller:
* Två datamedlemmar (`pin` och `state`).
* Två metoder (`write` och `read`).

Dessa funktioner arbetar på den data som lagras i strukten.

Instanser av strukten kan skapas enligt nedan:

``` cpp
// Initialize LED connected to pin 9, state set to false.
driver::Gpio led{9U, false};

// Initialize button connected to pin 13, simulate pressdown at startup.
driver::Gpio button{13U, true};
```

Funktionerna kan sedan anropas med punktoperatorn, precis som datamedlemmarna:

``` cpp
// Enable the LED if the button is pressed.
const bool buttonPressed{button.read()};
led.write(buttonPressed);
```

I traditionell C skulle motsvarande funktionalitet vanligtvis implementeras med hjälp av en strukt i kombination med separata funktioner som tar en pekare till strukten, som visas nedan:

```c
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief GPIO stub driver.
 */
typedef struct
{
    /** Pin the GPIO is connected to. */
    const std::uint8_t pin;

    /** GPIO state. */
    bool state;
} gpio_t;

/**
 * @brief Set GPIO state.
 * 
 * @param[in, out] self Pointer to the GPIO.
 * @param[in] state GPIO state (true = enabled, false = disable).
 */
void gpio_write(gpio_t* self, const bool state)
{
    if (NULL == self) { return; }
    self->state = state;
}

/**
 * @brief Get GPIO state.
 *
 * @param[in, out] self Pointer to the GPIO.
 *
 * @return True if the GPIO is enabled, false otherwise.
 */
bool gpio_read(const gpio_t* self)
{
    return NULL != self ? self->state : false; 
}
```

Instanser av strukten kan sedan skapas och användas som visas nedan:

``` cpp
// Initialize LED connected to pin 9, state set to false.
gpio_t led    = {9U, false};

// Initialize button connected to pin 13, simulate pressdown at startup.
gpio_t button = {13U, true};

// Enable the LED if the button is pressed.
const bool button_pressed = gpio_read(&button);
gpio_write(&led, button_pressed);
```

### Jämförelse

#### C-style

``` c
gpio_write(&led, true);
```

#### C++ style

``` cpp
led.write(true);
```

C++-syntaxen förbättrar läsbarheten och håller relaterad funktionalitet samlad, vilket gör koden lättare att förstå och underhålla.

Senare i kursen kommer vi introducera klasser, som vidareutvecklar detta koncept genom att stödja funktioner såsom inkapsling, arv och polymorfism.

---

### 8. Referenser
Referenser ger ett alternativt sätt att skicka variabler till funktioner.

En referens fungerar som ett alias för en annan variabel. Detta innebär att operationer som utförs på referensen direkt påverkar den ursprungliga variabeln. Referenser beter sig på liknande sätt som pekare, men kan användas med vanlig variabelsyntax.

Referenser deklareras med symbolen `&`.

Betrakta följande exempel:

``` cpp
void toggle(bool& state)
{
    state = !state;
}
```

Parametern `state` är en referens till en `bool`.

Detta innebär att funktionen modifierar den ursprungliga variabeln som skickades till den.

Exempel på användning:

``` cpp
bool state{false};

toggle(state);
```

Efter funktionsanropet kommer värdet på `state` att vara `true`.

I C skulle liknande beteende vanligtvis implementeras med hjälp av pekare:

``` c
void toggle(bool* state)
{
    *state = !(*state);
}
```

### Jämförelse

#### C-style (med pekare)

``` c
toggle(&state);
```

#### C++ style (med referens)

``` cpp
toggle(state);
```

Även om pekare fortfarande används i C++ föredras referenser ofta eftersom de:
* Inte kan vara `nullptr` (`NULL` i C).
* Inte kräver adressoperatorn (`&`) för att skicka adresser.
* Inte kräver dereferering (`*`) för att läsa värdet som en pekare pekar på.
* Ger tydligare syntax.

Som ett resultat används referenser ofta i modern C++ när en funktion behöver modifiera en befintlig variabel. En referens kan dock bara referera till en enda instans; pekare används fortfarande när vi behöver kunna ändra adressen vi pekar på, till exempel vid implementation av dynamiska arrayer (vektorer).

---

### 9. Nyckelordet `auto`
Nyckelordet `auto` gör det möjligt för kompilatorn att automatiskt härleda typen på en variabel från dess initialiseringsuttryck.

Som exempel:

``` cpp
auto number  = 5;   // Typen blir int.
auto counter = 10U; // Typen blir unsigned int
auto voltage = 3.3; // Typen blir double.
```

Detta kan förbättra läsbarheten och minska behovet av att upprepa långa typnamn.

Man måste dock vara försiktig när `auto` används tillsammans med klammerinitialisering `{}` i C++11/14. I vissa fall kan kompilatorn härleda en `std::initializer_list` i stället för den avsedda typen.

Som exempel:

``` cpp
auto a = 10; // Typen blir int.
auto b{10};  // Typen blir std::initializer_list<int> innan C++17.
```

Av denna anledning rekommenderades det generellt, när man använde `auto` före C++17, att använda tilldelningsoperatorn `=` i stället för klammerinitialisering `{}`.

I modern C++ (C++17 och senare) har beteendet förbättrats och klammerinitialisering fungerar mer intuitivt. En vanlig tumregel är dock fortfarande:
* När `auto` används, föredra `=` i stället för `{}` om du inte uttryckligen vill skapa en `std::initializer_list`.

I denna kurs kommer dock datatypen vanligtvis att anges explicit.  
Nyckelordet `auto` kommer främst att användas i situationer där typen annars skulle bli mycket lång, till exempel vid iteration över containrar:

```cpp
// Enable all LEDs in a list.
for (const auto& led : leds)
{
    led.on();
}
```

Utan `auto` kan typen bli mycket lång.

---

### 10. Funktionstemplates – ett smakprov
**OBS!** Här visar vi bara grundidén. Templates behandlas mer i detalj senare i kursen.

Templates gör det möjligt för funktioner och klasser att arbeta med flera olika datatyper utan att behöva duplicera kod.

I inbyggda system används templates ofta för att implementera generiska hjälpfunktioner, till exempel funktioner för bitmanipulation.

Betrakta följande exempel för att sätta en bit `bit` i ett givet register `reg`:
* Datatypen för registret som ska skrivas till anges som `T`. Typen bestäms vid kompileringstid när funktionen instansieras.
* Vi begränsar funktionen så att den endast fungerar för heltalstyper med hjälp av en `static_assert`. Om funktionen anropas med någon annan typ, till exempel ett flyttal, genereras ett kompilatorfel med felmeddelandet `Failed to set bit in register: T must be of integral type!`.
* Vi kontrollerar datatypen med strukten `std::is_integral<T>` från `<type_traits>`, specifikt via dess medlemsvariabel `value`:
  * Om `T` är en heltalstyp är `std::is_integral<T>::value` `true` och assertionen lyckas.
  * Om `T` inte är en heltalstyp är `std::is_integral<T>::value` `false` och assertionen misslyckas.
* Värdet `1` typomvandlas till typen `T` med hjälp av `static_cast`, vilket är ett säkrare alternativ till vanliga C-casts.

```cpp
#include <cstdint>
#include <type_traits>

/**
 * @brief Set bit in register.
 *
 * @tparam T The register type. Must be integral.
 *
 * @param[out] reg Register to write to.
 * @param[in] bit The bit to set.
 */
template<typename T>
constexpr void set(T& reg, const std::uint8_t bit) noexcept
{
    static_assert(std::is_integral<T>::value,
        "Failed to set bit in register: T must be of integral type!");

    reg |= (static_cast<T>(1) << bit);
}
```

Funktionen kan användas för att sätta bitar i register av godtycklig heltalstyp.  
I exemplet nedan sätts bitarna 1–2 i ett 8-bitarsregister genom att instansiera funktionen `set()` två gånger:

```cpp
std::uint8_t reg{};
set(reg, 1U);
set(reg, 2U);
```

#### Templateinstansiering och kodstorlek
Templates instansieras separat för varje datatyp som används i programmet.   
Till exempel, om funktionen `set()` används med både `std::uint8_t` och `std::uint32_t`:

```cpp
std::uint8_t reg1{};
std::uint32_t reg2{};

set(reg1, 1U);
set(reg2, 12U);
```

Då genererar kompilatorn två separata versioner av funktionen:

```cpp
set(std::uint8_t&, bit)
set(std::uint32_t&, bit)
```

**OBS!** Till skillnad från C kan funktioner i C++ ha samma namn så länge deras parameterlistor skiljer sig. Kompilatorn avgör då vid varje funktionsanrop vilken version som ska användas baserat på argumentens typer. Detta kallas funktionöverlagring (*function overloading*).

Varje version kompileras oberoende och inkluderas i den slutliga binären:
* Detta beteende är en av anledningarna till att templates betraktas som **zero-cost abstractions**; den genererade koden specialiseras för exakt den typ som används, vilket gör att kompilatorn kan optimera implementationen.
* I inbyggda system innebär detta dock också att överdriven användning av templates kan öka binärstorleken, eftersom flera versioner av samma funktion kan genereras.
* Av denna anledning bör templates användas med eftertanke i resursbegränsade system, särskilt när många olika datatyper är involverade.

#### Parameter packs
Vi kan även använda så kallade *parameter packs* för att sätta flera bitar samtidigt:

```cpp
#include <cstdint>
#include <type_traits>

template<typename T, typename... Bits>
constexpr void set(T& reg, const Bits&... bits) noexcept
{
    static_assert(std::is_integral<T>::value,
        "Failed to set bit in register: T must be of integral type!");

    for (const auto& bit : {bits...})
    {
        reg |= (static_cast<T>(1) << bit);
    }
}
```

I exemplet nedan sätts bitarna 1–5 i ett 8-bitarsregister genom att instansiera funktionen `set()` en gång:

```cpp
// Set bit 1-5 in a register.
std::uint8_t reg{};
set(reg, 1U, 2U, 3U, 4U, 5U);
```

---

### Summering
Denna bilaga introducerade följande moderna C++-funktioner som ofta används i inbyggd mjukvara:
* Namnrymder.
* `constexpr` och konstanter vid kompileringstid.
* `noexcept` och undantagshantering i inbyggda system.
* Standardargument.
* Struktar med medlemsfunktioner (metoder).
* Referenser.
* Funktionstemplates.

Dessa funktioner gör det möjligt för utvecklare att skriva säkrare, tydligare och mer underhållbar mjukvara för inbyggda system, samtidigt som full kontroll över hårdvaran och prestandan bibehålls.

---
