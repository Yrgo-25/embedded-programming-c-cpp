# Bilaga A

## Klasser i C++

### Bakgrund
För att hålla god struktur i större program finns ett ökat behov av att samla relaterad data och relaterat beteende i en och samma enhet i stället för att sprida ut allt över multipla separata variabler och funktioner. Som exempel kan det vara fördelaktigt att representera en GPIO‑pinne via en klass som lagrar pin-nummer, riktning och aktuellt tillstånd, samtidigt som klassen också innehåller funktioner för att skriva, läsa och toggla pinnen.

Inom programmering utgörs de vanligaste datastrukturerna för att lagra relaterad data som en enhet av så kallade **struktar** och **klasser**:
* **Struktar** (strukturer) används främst i C (begreppet klass existerar inte överhuvudtaget i C).
* **Klasser** används i många moderna programspråk, såsom C++, Java och Python.
* I C++ används både struktar och klasser.
* Klasser kan ses som en vidareutveckling av struktar, som är relativt simpla. Sett till uppbyggnad är dock dessa datastrukturer väldigt lika.

Språk som innehar stöd för klasser sägs inneha stöd för objektorientering, eller OOP *(Object Oriented Programming).* C sägs vara ett procedurellt språk, då det bygger på programmering via funktioner. Via struktar kan dock vissa objektorienterade principer efterliknas, vilket kommer demonstreras nedan.

En del objektorienterade språk, exempelvis Java och C#, är rent objektorienterade, vilket innebär att klasser måste användas. Andra programspråk, såsom C++ och Python, sägs utgöra multiparadigmspråk, då det finns möjlighet att programmera både objektorienterat (såsom i Java) samt procedurellt (såsom i C) eller en kombination av dem.

---

### Terminologi
Som skrevs tidigare används klasser i stället för struktar i de flesta konventionella programspråk som skapades efter C.

I C++ är dock struktar nästintill identiska med klasser; den enda formella skillnaden är att för klasser är medlemmar privata som standard, medan de är publika som standard i struktar. Struktar i C++ kan alltså användas på samma sätt som klasser.

För att minska förvirring i resten av detta avsnitt använder vi dock följande **konvention i detta kursmaterial**:
* **Strukt** syftar till enkla datastrukturer utan medlemsfunktioner, exempelvis:
    * Rena C-struktar (ibland kallade POD – *Plain Old Data*).
    * Struktar innehållande konstanter (`static constexpr`).
* **Klass** syftar till en mer avancerad variant av den datastruktur vi har sett hittills.
---

### Skillnader mellan klasser och struktar
Klasser kan ses som en vidareutveckling av struktar. Därmed är klasser och struktar lika sett till den grundläggande uppbyggnaden, men klasser erbjuder mycket mer funktionalitet samt flexibilitet. I detta avsnitt ska vi titta på den mest grundläggande funktionalitet som klasser erbjuder.

Några av de mest betydande skillnaderna mellan klasser och struktar är:
* **Klasser innehar stöd för inkapsling,** vilket innebär att man kan välja vad som är åtkomligt/synligt utanför klassen. Därmed kan hemlig data eller information som inte är relevant utanför klassen (såsom implementationsdetaljer) döljas.
* **Klasser kan innehålla funktioner,** vilket innebär att vi inte behöver implementera associerade funktioner med struktpekare, så som visades nedan.
* **Klasser erbjuder funktionalitet för arv,** vilket innebär att en ny klass kan erhålla innehållet från en basklass. Detta kan minska mängden kod som behövs avsevärt.
* **Klasser kan implementeras via så kallade klasstemplates** för att göra en eller flera datatyper valbara, likt de funktionstemplates vi såg tidigare. Ett exempel på ett klasstemplate i standardbiblioteket är *std::vector,* som medför att vi kan välja vad som ska lagras i en given vektor.

---

### Grundläggande struktur för en klass
Vi implementerar ett enkelt exempel på en GPIO‑drivrutin i C++ via en klass döpt `Gpio`. Klassen placeras i namnrymden `driver::gpio`, vilket är vanligt i större C++‑projekt för att organisera kod och undvika namnkrockar.

Vi kommer använda oss av `camelCase` i stället för `snake_case`, vilket medför att:
* Vi använder oss av stora bokstäver i stället för understreck i funktions- och typnamn. Så i stället för ett funktionsnamn så som `print_numbers()` kommer vi döpa denna funktion `printNumbers()`.
* Samma mönster följs för typer så som struktar, klasser och enumerationer. En skillnad är dock att namnet på typer kommer börja med en stor bokstav, exempelvis `Gpio` i stället för `gpio`, `Direction` i stället för `direction` med mera.

---

#### 1. Lägg till inkluderingsdirektiv
Vi börjar med att inkludera standardheadern `<cstdint>`, så att vi har tillgång till datatyper såsom `std::uint8_t`:

```cpp
#include <cstdint>
```

---

#### 2. Skapa namnrymd och enumerationsklass
Därefter skapar vi namnrymden `driver::gpio` och enumerationsklassen `Direction` för att beskriva en GPIO‑pinnes riktning. Vi sätter enumerationens underliggande datatyp till `std::uint8_t`, vilket gör typen kompakt och säkerställer att inga negativa värden kan representeras (vilket kan förenkla validering):

```cpp
namespace driver::gpio
{
/**
 * @brief GPIO direction configuration.
 */
enum class Direction : std::uint8_t
{
    Input,       ///< Input without pull-up.
    InputPullup, ///< Input with pull-up enabled.
    Output,      ///< Output.
    Count,       ///< Number of supported directions.
};
} // namespace driver::gpio
```

Enumerationsklassen ovan används för att ange om en pinne ska vara:
* `Input`
* `InputPullup`
* `Output`

Vi kan också enkelt kontrollera om en given enumerator `direction` är giltig genom att säkerställa att denna har ett numeriskt värde som är mindre än `Direction::Count`:

```cpp
constexpr bool isDirectionValid(const Direction direction) noexcept
{
    return static_cast<std::uint8_t>(Direction::Count) > static_cast<std::uint8_t>(direction);
}
```

Den främsta anledningen till att vi väljer att implementera `Direction` som en enumerationsklass i stället för en traditionell enumeration är att vi slipper använda prefix för att undvika namnkrockar; i stället är det inbyggt i språket.

Notera ovan att enumeratorerna nu är döpta
* `Input`,
* `InputPullup` samt
* `Output` 

i stället för exempelvis
* `GPIO_DIRECTION_INPUT`, 
* `GPIO_DIRECTION_INPUT_PULLUP` samt 
* `GPIO_DIRECTION_OUTPUT`. 

Med enumerationsklassen ovan skriver vi i stället
* `Direction::Input`,
* `Direction::InputPullup` samt
* `Direction::Output`.

I allmänhet är det rekommenderat att använda enumerationsklasser i stället för traditionella enumerationer om man programmerar i modern C++, alltså C++11 och framåt.

---

#### 3. Definiera klassen `Gpio`
Därefter definierar vi klassen `Gpio` via nyckelordet `class`:

```cpp
namespace driver::gpio
{
/**
 * @brief GPIO driver.
 */
class Gpio final
{
public:

private:
};
} // namespace driver::gpio
```

Vi har lagt till två nyckelord i klassen, `public` och `private`:
* Allt som faller under nyckelordet `public` ligger i det publika segmentet:
    * Innehållet i detta segment är synligt och åtkomligt utanför klassen.
    * Allt som vi vill att "användaren" av klassen ska kunna se och använda ska deklareras här. 
    * Exempelvis kan detta röra sig om metoder för att skriva till pinnen, läsa dess tillstånd och toggla utgången.
* Allt som faller under nyckelordet `private` ligger i det privata segmentet:
    * Innehållet i detta segment är inte synligt eller åtkomligt utanför klassen:
    * Här placerar vi allt som vi vill vara icke åtkomligt för "användaren" av klassen.
    * Exempelvis kan detta röra sig om intern data eller implementationsdetaljer. 

I vårt fall placerar vi samtliga medlemsvariabler i det privata segmentet. Vi kan också placera metoder som bara ska användas internt i klassen här. Att dölja information på detta sätt kallas **inkapsling**.

Normal praxis för en klass är följande:
* Medlemsvariabler hålls privata.
* Det publika segmentet ligger först i klassen, så att "användaren" vid behov enkelt kan kolla in vad för metoder och annat som hen har till sitt förfogande.
* Implementationsdetaljerna placeras nedanför i den privata delen av klassen och meningen är att användaren inte ens ska behöva (eller vilja) läsa ned hit.

Oftast dokumenteras enbart den publika delen av klassen; resten av klassen är implementationsdetaljer som enbart är av intresse för utvecklaren av klassen och dokumenteras därmed inte. Eftersom detta är ett typexempel dokumenteras dock all kod.

Vi har även lagt till nyckelordet `final` efter klassnamnet. Detta innebär att klassen inte får användas som basklass. Ingen annan klass får alltså ärva från `Gpio`. I embedded‑sammanhang är detta ofta ett rimligt designval för små, konkreta drivrutiner där man vill hålla designen enkel och tydlig.

---

#### 4. Lägg till medlemsvariabler
Under den privata delen lägger vi till medlemsvariablerna, alltså de variabler i klassen som lagrar pin-nummer, riktning och aktuellt tillstånd.
Notera att:
* Vi använder prefixet `my`, för att undvika namnkrockar med senare metoder vi kommer lägga till.
* Vi sätter medlemsvariabler som aldrig ska ändras efter initieringen till `const`.

```cpp
namespace driver::gpio
{
/**
 * @brief GPIO driver.
 */
class Gpio final
{
public:

private:
    /** GPIO pin number. */
    const std::uint8_t myPin;

    /** GPIO direction. */
    const Direction myDirection;

    /** GPIO state. */
    bool myState;
};
} // namespace driver::gpio
```

Medlemsvariablerna är privata, så att "användaren" inte kan läsa och skriva dem hur som helst. I stället tillhandahåller vi senare metoder såsom `write()`, `read()` och `toggle()` för att styra hur objektet används.

---

#### 5. Lägg till konstruktorer och destruktorer
Innan vi lägger till övriga metoder lägger vi till konstruktorer, vilket kan ses som initieringsrutiner som anropas automatiskt när ett objekt av klassen skapas.

Först och främst raderar vi den så kallade default‑konstruktorn, så att man inte kan skapa ett GPIO‑objekt utan att skicka med pin-nummer och riktning:

```cpp
public:
    Gpio() = delete; // No default constructor.
```

Man kallar denna konstruktor för default‑konstruktor, då den inte har några ingående argument. Därmed är det inte möjligt att skapa ett tomt objekt så som visas nedan:

```cpp
// Won't compile, since the default constructor is deleted.
driver::gpio::Gpio gpio{};
```

Normalt sett placeras raderade operatorer, såsom default-konstruktorn ovan, längst ned i det publika segmentet.

Vi kan också lägga till en destruktor, vilket kan ses som en funktion som kallas på automatiskt precis innan en instans av den givna klassen raderas.

Destruktorn ser ut som default‑konstruktorn, med skillnaden att negationstecknet `~` placeras framför klassens namn. Om vi inte är i behov av att göra något speciellt innan objektet raderas kan vi slopa destruktorn; kompilatorn kommer då skapa en default‑destruktor åt oss. Vill vi ändå skapa en destruktor för tydlighets skull kan vi explicit sätta destruktorn till default med nyckelordet `default`.

```cpp
/**
 * @brief Destructor.
 */
~Gpio() noexcept = default;
```

I modern C++ rekommenderas ofta nyckelordet `default`, vilket kommer användas i detta exempel. Dock hade det gått att implementera en default-destruktor såsom visas nedan:

```cpp
/**
 * @brief Destructor.
 */
~Gpio() noexcept {}
```

Vi vill att användaren ska ange pin-nummer, riktning och eventuellt initialt tillstånd direkt när ett GPIO‑objekt skapas:
* Vi skapar då en konstruktor som tar pin-nummer, riktning samt ett initialt tillstånd som ingående argument. 
* Vi markerar konstruktorn med `explicit`, så att konstruktorn inte används för oavsiktliga implicita konverteringar. 
* Se [bilaga B](./b_classes2.md#nyckelordet-explicit) för ytterligare information om nyckelordet `explicit`.

```cpp
/**
 * @brief Create a new GPIO object.
 *
 * @param[in] pin GPIO pin number.
 * @param[in] direction GPIO direction.
 * @param[in] initialState Initial GPIO state (default = false).
 */
explicit Gpio(const std::uint8_t pin, const Direction direction, 
              const bool initialState = false) noexcept
{}
```

I konstruktorn ovan initierar vi medlemsvariablerna så att:
* `myPin` tilldelas pinnumret som lagras i det ingående argumentet `pin`.
* `myDirection` tilldelas värdet som lagras i det ingående argumentet `direction`.
* `myState` tilldelas värdet som lagras i det ingående argumentet `initialState.` Som default är `initialState` satt till `false`.

Detta åstadkommer vi genom att lägga till en initieringsdel mellan funktionshuvudet samt funktionskroppen (markerad via måsvingar {}). Notera att vi börjar initieringsdelen med ett kolon, initierar med måsvingar och separerar initieringen av varje medlemsvariabel med ett kommatecken.

```cpp
/**
 * @brief Create a new GPIO object.
 *
 * @param[in] pin GPIO pin number.
 * @param[in] direction GPIO direction.
 * @param[in] initialState Initial GPIO state (default = false).
 */
explicit Gpio(const std::uint8_t pin, const Direction direction, 
              const bool initialState = false) noexcept
    : myPin{pin}
    , myDirection{direction}
    , myState{initialState}
{}
```

Om vi behöver göra något ytterligare efter initieringen av medlemsvariablerna hade vi kunnat placera detta i konstruktorns funktionskropp. I detta fall vill vi dock enbart initiera medlemsvariablerna, så vi behöver inte lägga till något ytterligare.

Man kan nu använda konstruktorn som visas ovan för att initiera GPIO‑objekt. Nedan visas hur vi initierar objektet `led` för att representera en utgång på pinne 13 med initialt lågt tillstånd:

```cpp
driver::gpio::Gpio led{13U, driver::gpio::Direction::Output, false};
```

Eftersom default‑värdet för det ingående argumentet som lagrar initialt tillstånd är `false` kan vi också utelämna detta argument:

```cpp
driver::gpio::Gpio led{13U, driver::gpio::Direction::Output};
```

Vi kan också använda konstruktorn för att initiera ett objekt `button` för att representera en ingång med pull‑up på pinne 2:

```cpp
driver::gpio::Gpio button{2U, driver::gpio::Direction::InputPullup};
```

---

#### 6. Kopierings- och flyttoperationer
Utöver vanliga konstruktorer finns det i C++ också speciella medlemsfunktioner för kopiering och flyttning. Dessa är mycket viktiga i modern C++ och det är bra att känna till dem redan när man lär sig klasser.

De vanligaste operationerna visas nedan:

| Operation                          | Beskrivning                                                                             | Signatur för klassen `Gpio`          |
| ---------------------------------- | --------------------------------------------------------------------------------------- | ------------------------------------ |
| **Kopieringskonstruktor**          | Skapar ett nytt objekt som kopia av ett annat objekt.                                   | `Gpio(const Gpio& other)`            |
| **Flyttkonstruktor**               | Skapar ett nytt objekt genom att flytta resurser från ett annat objekt.                 | `Gpio(Gpio&& other)`                 |
| **Kopieringstilldelningsoperator** | Tilldelar ett redan existerande objekt från ett annat objekt.                           | `Gpio& operator=(const Gpio& other)` |
| **Flyttilldelningsoperator**       | Tilldelar ett redan existerande objekt genom att flytta resurser från ett annat objekt. | `Gpio& operator=(Gpio&& other)`      |

I många verkliga embedded‑system väljer man att förbjuda kopiering och flyttning helt, eftersom ett objekt ofta representerar en unik hårdvaruresurs. På så sätt säkerställs att varje `Gpio`‑objekt representerar exakt en fysisk GPIO‑pinne och inte kan dupliceras av misstag.

För klassen `Gpio` raderas kopiering och flyttning såsom visas nedan, normalt placerat längst ned i klassens publika segment.

```cpp
Gpio(const Gpio&)            = delete; // No copy constructor.
Gpio(Gpio&&)                 = delete; // No move constructor.
Gpio& operator=(const Gpio&) = delete; // No copy assignment.
Gpio& operator=(Gpio&&)      = delete; // No move assignment.
```

Se [bilaga B](./b_classes2.md#kopierings--och-flyttoperationer) för mer information om kopierings- och flyttoperationer.

---

#### 7. Lägg till publika metoder
För ett GPIO‑objekt är det mer naturligt att exponera funktionalitet i form av beteenden än klassiska get‑ och set‑metoder. Vi lägger därför till följande publika metoder:
* `write(bool state)` för att skriva ett nytt tillstånd till pinnen.
* `read()` för att läsa pinnens aktuella tillstånd.
* `toggle()` för att växla pinnens tillstånd.

Nedan visas metoden `write()`, som används för att skriva ett nytt tillstånd till GPIO-instansen:

```cpp
/**
 * @brief Write a new state to the GPIO pin.
 *
 * @param[in] state The state to write.
 */
void write(const bool state) noexcept
{
    if (myDirection == Direction::Output)
    {
        myState = state;
    }
}
```

I exemplet ovan tillåter vi enbart skrivning om pinnen är konfigurerad som utgång. Det är ett enkelt sätt att illustrera att klassen kan skydda sin interna data och styra hur objektet får användas.

Nedan visas metoden `read()`, som returnerar pinnens aktuella tillstånd:

```cpp
/**
 * @brief Read the current GPIO state.
 *
 * @return Current GPIO state.
 */
bool read() const noexcept { return myState; }
```

Notera att vi skriver nyckelordet `const` direkt efter funktionshuvudet, då vi ser till att klassens innehåll bara ska kunna läsas. Vi kan därmed inte av misstag råka ändra någon av klassmedlemmarna.

Vi hade kunnat använda metoden ovan för att läsa och skriva ut tillståndet på en LED som tidigare implementerades i mjukvaran via objektet `led`. Här används `std::printf()` från `<cstdio>` för att skriva ut tillståndet:

```cpp
int main()
{
    driver::gpio::Gpio led{13U, driver::gpio::Direction::Output};
    std::printf("State:\t%s\n", led.read() ? "High" : "Low");
    return 0;
}
```

Utskriften blir följande:

```
State:  Low
```

Vi lägger därefter till metoden `toggle()` för att kunna växla pinnens tillstånd:

```cpp
/**
 * @brief Toggle the GPIO state.
 */
void toggle() noexcept
{
    if (myDirection == Direction::Output)
    {
        myState = !myState;
    }
}
```

Om vi exempelvis kallar på `toggle()` ser vi till att medlemsvariabeln `myState` växlar mellan `false` och `true`, men enbart om pinnen är konfigurerad som utgång.

Vi kan först kalla på `write(true)` för att sätta LED‑pinnen hög, och därefter `toggle()` för att växla tillbaka till låg nivå. Vi skriver ut tillståndet både innan och efter anropen:

```cpp
int main()
{
    driver::gpio::Gpio led{13U, driver::gpio::Direction::Output};
    std::printf("Old state:\t%s\n", led.read() ? "High" : "Low");
    led.write(true);
    std::printf("After write:\t%s\n", led.read() ? "High" : "Low");
    led.toggle();
    std::printf("After toggle:\t%s\n", led.read() ? "High" : "Low");
    return 0;
}
```

Utskriften blir följande:

```
Old state:      Low
After write:    High
After toggle:   Low
```

Efter tillägg av metoderna ser klassen `Gpio` ut så här:

```cpp
namespace driver::gpio
{
/**
 * @brief GPIO driver.
 */
class Gpio final
{
public:
    /**
     * @brief Create a new GPIO object.
     *
     * @param[in] pin GPIO pin number.
     * @param[in] direction GPIO direction.
     * @param[in] initialState Initial GPIO state (default = false).
     */
    explicit Gpio(const std::uint8_t pin, const Direction direction, 
                  const bool initialState = false) noexcept
        : myPin{pin}
        , myDirection{direction}
        , myState{initialState}
    {}

    /**
     * @brief Destructor.
     */
    ~Gpio() noexcept = default;

    /**
     * @brief Write a new state to the GPIO pin.
     *
     * @param[in] state The state to write.
     */
    void write(const bool state) noexcept
    {
        if (myDirection == Direction::Output)
        {
            myState = state;
        }
    }

    /**
     * @brief Read the current GPIO state.
     *
     * @return Current GPIO state.
     */
    bool read() const noexcept { return myState; }

    /**
     * @brief Toggle the GPIO state.
     */
    void toggle() noexcept
    {
        if (myDirection == Direction::Output)
        {
            myState = !myState;
        }
    }

    Gpio()                       = delete; // No default constructor.
    Gpio(const Gpio&)            = delete; // No copy constructor.
    Gpio(Gpio&&)                 = delete; // No move constructor.
    Gpio& operator=(const Gpio&) = delete; // No copy assignment.
    Gpio& operator=(Gpio&&)      = delete; // No move assignment.

private:
    /** GPIO pin number. */
    const std::uint8_t myPin;

    /** GPIO direction. */
    const Direction myDirection;

    /** GPIO state. */
    bool myState;
};
} // namespace driver::gpio
```

---

### 8. Uppdelning av klass i header- och källkodsfil
Ofta används objekt av många olika klasser i ett program. Dessa klasser är dessutom ofta större än klassen vi har sett här. Det är därmed opraktiskt att implementera samtliga klasser i en enda fil. 

Oftast skapar man en headerfil dedikerad för en specifik klass, inklusive publika enumerationsklasser. Som exempel:
* Klassen `Gpio` samt enumerationsklassen `Direction` i namnrymden `driver::gpio` bör implementeras via headerfiler `gpio.h` samt `direction.h` i katalogen `driver/gpio`. Filsökvägarna blir då:
    * `driver/gpio/direction.h`
    * `driver/gpio/gpio.h`

Längst upp i respektive headerfil placerar vi direktivet `#pragma once`, som ser att vi inte råkar definiera multipla kopior av innehållet i denna headerfil om den inkluderas i flera olika filer, på samma sätt som header guards fungerar i C. Det förhindrar att samma headerfil inkluderas flera gånger i samma översättningsenhet (en källkodsfil tillsammans med alla inkluderade headerfiler):

```cpp
#pragma once
```

Vi inkluderar också `<cstdint>` för att få tillgång till datatyper såsom `std::uint8_t`:

```cpp
#include <cstdint>
```

**Filen `driver/gpio/direction.h`:**
Vi implementerar enumerationsklassen `Direction` i denna fil:

```cpp
/**
 * @brief GPIO direction configurations.
 */
#pragma once

#include <cstdint>

namespace driver::gpio
{
/**
 * @brief GPIO direction configuration.
 */
enum class Direction : std::uint8_t
{
    Input,       ///< Input without pull-up.
    InputPullup, ///< Input with pull-up enabled.
    Output,      ///< Output.
    Count,       ///< Number of supported directions.
};
} // namespace driver::gpio
```

**Filen `driver/gpio/gpio.h`:**
Vi implementerar klassen `Gpio` i denna fil. Eftersom enumerationsklassen `Direction` används direkt i denna fil inkluderas `driver/gpio/direction.h`:

```cpp
/**
 * @brief GPIO driver implementation.
 */
#pragma once

#include <cstdint>

#include "driver/gpio/direction.h"

namespace driver::gpio
{
/**
 * @brief GPIO driver.
 */
class Gpio final
{
public:
    /**
     * @brief Create a new GPIO object.
     *
     * @param[in] pin GPIO pin number.
     * @param[in] direction GPIO direction.
     * @param[in] initialState Initial GPIO state (default = false).
     */
    explicit Gpio(const std::uint8_t pin, const Direction direction, 
                  const bool initialState = false) noexcept
        : myPin{pin}
        , myDirection{direction}
        , myState{initialState}
    {}

    /**
     * @brief Destructor.
     */
    ~Gpio() noexcept = default;

    /**
     * @brief Write a new state to the GPIO pin.
     *
     * @param[in] state The state to write.
     */
    void write(const bool state) noexcept
    {
        if (myDirection == Direction::Output)
        {
            myState = state;
        }
    }

    /**
     * @brief Read the current GPIO state.
     *
     * @return Current GPIO state.
     */
    bool read() const noexcept { return myState; }

    /**
     * @brief Toggle the GPIO state.
     */
    void toggle() noexcept
    {
        if (myDirection == Direction::Output)
        {
            myState = !myState;
        }
    }

    Gpio()                       = delete; // No default constructor.
    Gpio(const Gpio&)            = delete; // No copy constructor.
    Gpio(Gpio&&)                 = delete; // No move constructor.
    Gpio& operator=(const Gpio&) = delete; // No copy assignment.
    Gpio& operator=(Gpio&&)      = delete; // No move assignment.

private:
    /** GPIO pin number. */
    const std::uint8_t myPin;

    /** GPIO direction. */
    const Direction myDirection;

    /** GPIO state. */
    bool myState;
};
} // namespace driver::gpio
```

**Metoddefinitioner i filen `driver/gpio/gpio.cpp`**:
Ofta placeras klassens metoddefinitioner i en separat källkodsfil för att hålla headerfilen mer läsbar. Som exempel, för att hålla filen `driver/gpio/gpio.h` läsbar kan vi med fördel placera metoddefinitionerna i en källkodsfil döpt `driver/gpio/gpio.cpp`. 

Längst upp i denna källkodsfil inkluderar vi följande headerfiler:
* `<cstdint>` för att få tillgång till datatyper såsom `std::uint8_t`.
* `driver/gpio/gpio.h` för att få tillgång till klassdefinitionen.
* `driver/gpio/direction.h` för att få tillgång till enumerationsklassen `Direction`.

**Notering**: Både `<cstdint>` samt `driver/gpio/direction.h` inkluderas indirekt via `driver/gpio/gpio.h`, men för att hålla god praxis bör vi inte förlita oss på indirekta beroenden.

```cpp
#include <cstdint>

#include "driver/gpio/direction.h"
#include "driver/gpio/gpio.h"
```
    
Därefter placerar vi metoddefinitionerna från klassen `Gpio` i denna fil.
Vi kopierar därmed samtliga metoder från klassen `Gpio` till denna fil. Värt att notera är att:
* För att kompilatorn ska förstå att respektive metod tillhör klassen `Gpio` måste vi använda prefixet `Gpio`. Så metoden `write()` måste definieras som `Gpio::write()` som exempel. 
* Defaultvärden på ingående argument ska endast skrivas i metoddeklarationen i headerfilen.
* Nyckelord framför metodernas namn, såsom `explicit`, ska inte skrivas med i metoddefinitionen.
* Konstruktorer, destruktorer med mera märkta `default` samt `delete` behöver inte placeras i källkodsfilen.

Vi börjar med metoden `write()`. I headerfilen `driver/gpio/gpio.h` låter vi metodhuvudet inklusive dokumentationen kvarstå:

```cpp
/**
 * @brief Write a new state to the GPIO pin.
 *
 * @param[in] state The state to write.
 */
void write(const bool state) noexcept;
```

I källkodsfilen `driver/gpio/gpio.cpp` placerar vi sedan metoddefinitionen:

```cpp
// -----------------------------------------------------------------------------
void Gpio::write(const bool state) noexcept
{
    if (myDirection == Direction::Output)
    {
        myState = state;
    }
}
```

Vi implementerar definitioner för övriga metoder i källkodsfilen `driver/gpio/gpio.cpp`. Vi placerar en rad med bindestreck mellan varje metod för att visuellt separera dessa nu när metoddokumentationen inte är med. 

**Filen `driver/gpio/gpio.cpp`**
Efter att ha lagt till samtliga metoddefinitioner ser filen `driver/gpio/gpio.cpp` ut så här:

```cpp
/**
 * @brief GPIO driver implementation details.
 */
#include <cstdint>

#include "driver/gpio/direction.h"
#include "driver/gpio/gpio.h"

namespace driver::gpio
{
// -----------------------------------------------------------------------------
Gpio::Gpio(const std::uint8_t pin, const Direction direction, const bool initialState) noexcept
    : myPin{pin}
    , myDirection{direction}
    , myState{initialState}
{}

// -----------------------------------------------------------------------------
void Gpio::write(const bool state) noexcept
{
    if (myDirection == Direction::Output)
    {
        myState = state;
    }
}

// -----------------------------------------------------------------------------
bool Gpio::read() const noexcept { return myState; }

// -----------------------------------------------------------------------------
void Gpio::toggle() noexcept
{
    if (myDirection == Direction::Output)
    {
        myState = !myState;
    }
}
} // namespace driver::gpio
```

**Filen `driver/gpio/gpio.h`**
Läsbarheten i headerfilen `driver/gpio/gpio.h` ökar nu, då vi har tagit bort en stor del av implementeringsdetaljerna i och med metoddefinitionerna:
* Vi måste dock fortfarande ha kvar metoddeklarationer samt definition av medlemsvariablerna innanför klassen.
* Vi tar bort nyckelordet `const` på parametrar passerade *by value* för att öka läsbarheten; att markera dessa `const` har bara effekt i funktionsdefinitionen.

```cpp
/**
 * @brief GPIO driver implementation.
 */
#pragma once

#include <cstdint>

#include "driver/gpio/direction.h"

namespace driver::gpio
{
/**
 * @brief GPIO driver.
 */
class Gpio final
{
public:
    /**
     * @brief Create a new GPIO object.
     *
     * @param[in] pin GPIO pin number.
     * @param[in] direction GPIO direction.
     * @param[in] initialState Initial GPIO state (default = false).
     */
    explicit Gpio(std::uint8_t pin, Direction direction, bool initialState = false) noexcept;

    /**
     * @brief Destructor.
     */
    ~Gpio() noexcept = default;

    /**
     * @brief Write a new state to the GPIO pin.
     *
     * @param[in] state The state to write.
     */
    void write(bool state) noexcept;

    /**
     * @brief Read the current GPIO state.
     *
     * @return Current GPIO state.
     */
    bool read() const noexcept;

    /**
     * @brief Toggle the GPIO state.
     */
    void toggle() noexcept;

    Gpio()                       = delete; // No default constructor.
    Gpio(const Gpio&)            = delete; // No copy constructor.
    Gpio(Gpio&&)                 = delete; // No move constructor.
    Gpio& operator=(const Gpio&) = delete; // No copy assignment.
    Gpio& operator=(Gpio&&)      = delete; // No move assignment.

private:
    /** GPIO pin number. */
    const std::uint8_t myPin;

    /** GPIO direction. */
    const Direction myDirection;

    /** GPIO state. */
    bool myState;
};
} // namespace driver::gpio
```

---

### Sammanfattning
Efter att ha gått igenom denna bilaga bör du som läsare ha tillräckligt med kunskap för att skapa en mindre klass.

Du bör kunna:
* Styra vad som är synligt respektive inte synligt utanför en klass via inkapsling.
* Skapa konstruktorer för att initiera objekt av klassen i fråga.
* Förstå vad `explicit` används till.
* Förstå vad `final` används till.
* Känna igen och skriva en destruktor.
* Känna igen kopieringskonstruktor, kopieringstilldelningsoperator, flyttkonstruktor och flyttilldelningsoperator.
* Använda `= default` och `= delete` för att styra vilka operationer som ska vara tillåtna.
* Lägga till publika metoder som styr hur ett objekt används.
* Skapa enumerationsklasser.
* Placera kod i en namnrymd för att strukturera projekt och undvika namnkrockar.

---
