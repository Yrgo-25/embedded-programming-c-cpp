# Bilaga B

## Fördjupning gällande klasser

### Nyckelordet `explicit`
Betrakta den enkla klassen `Led` nedan:

```cpp
#include <cstdint>

class Led final
{
public:
    Led(const std::uint8_t pin) noexcept
        : myPin{pin}
        , myState{false}
    {}

    void setEnabled(const bool state) noexcept { myState = state; }
    bool isEnabled() const noexcept { return myState; }

private:
    const std::uint8_t myPin;
    bool myState;
};
```

Via ovanstående konstruktor kan vi skapa en `Led`-instans och passera denna som kopia (*pass-by-value*) till en funktion `ledPrint`, som visas nedan. Här sker utskrift med `std::printf()` från `<cstdio>`:

```cpp
void ledPrint(const Led led) noexcept
{
    const char* state{led.isEnabled() ? "on" : "off"};
    std::printf("The LED is %s!\n", state);
}

int main()
{
    Led led1{8U};
    led1.setEnabled(true);
    ledPrint(led1);
    return 0;
}
```

Utskriften ser ut såsom visas nedan:

```
The LED is on!
```

Vi kan också skapa en `Led`-instans via kopieringsinitiering, såsom visas nedan. Notera att vi inte har skapat en `Led`-instans i detta fall - i stället skickas ett pin-nummer (av misstag). Dessa argument konverteras implicit till en `Led`-instans:

```cpp
void ledPrint(const Led led) noexcept
{
    const char* state{led.isEnabled() ? "on" : "off"};
    std::printf("The LED is %s!\n", state);
}

int main()
{
    ledPrint(8U);
    return 0;
}
```

Utskriften ser ut såsom visas nedan:

```
The LED is off!
```

**Notering**: I detta exempel skickas `led` som en kopia (pass-by-value) för att tydligt demonstrera hur en implicit konvertering kan ske via konstruktorn. För större klasser används ofta const-referens i stället för att undvika onödiga kopior.

Det som sker i detta fall är att:
* Kompilatorn implicit konstruerar en `Led`-instans givet ett osignerat heltal i form av ett pin-nummer.
* Kompilatorn förväntade sig en `Led-instans`, men eftersom den fick ett osignerat heltal i stället, vilket matchar klassens konstruktor, anropade den implicit på denna och skapade på så sätt implicit en `Led-instans`.
* Detta kanske kan ses som effektivt eller snyggt, men det kan också medföra svårdetekterade buggar och rekommenderas därmed inte.
* Därmed är det bra att se till att implicit konvertering av `Led`-instanser inte är tillåtet.  Detta kan åstadkommas genom att lägga till nyckelordet `explicit` framför konstruktorn, såsom visas nedan:

```cpp
#include <cstdint>

class Led final
{
public:
    explicit Led(const std::uint8_t pin) noexcept
        : myPin{pin}
        , myState{false}
    {}

    void setEnabled(const bool state) noexcept { myState = state; }
    bool isEnabled() const noexcept { return myState; }

private:
    const std::uint8_t myPin;
    bool myState;
};
```

Efter att ha markerat konstruktorn `explicit` hade föregående kodexempel genererat ett kompileringsfel.

---

### Kopierings- och flyttoperationer
Utöver vanliga konstruktorer finns det i C++ också speciella medlemsfunktioner för kopiering och flyttning. Dessa är mycket viktiga i modern C++ och det är bra att känna till dem redan när man lär sig klasser.

De vanligaste är:
* **Kopieringskonstruktor** – Skapar ett nytt objekt som kopia av ett annat objekt.
* **Kopieringstilldelningsoperator** – Tilldelar ett redan existerande objekt från ett annat objekt.
* **Flyttkonstruktor** – Skapar ett nytt objekt genom att flytta resurser från ett annat objekt.
* **Flyttilldelningsoperator** – Tilldelar ett redan existerande objekt genom att flytta resurser från ett annat objekt.

I detta avsnitt fortsätter vi använda klassen `Gpio`. Detta gör att samma klass kan användas genom hela kapitlet samtidigt som vi visar syntaxen för kopierings‑ och flyttoperationer.

Följande implementationer visas främst för att illustrera syntaxen. För en klass som `Gpio` är det oftast bättre att radera dessa operationer helt.

Vi måste ha i åtanke att medlemsvariabler `myPin` och `myDirection` är deklarerade som `const`. Detta innebär att dessa värden bara kan sättas när objektet skapas och inte kan ändras senare. Detta påverkar hur kopierings‑ och flyttoperationer kan implementeras.

---

#### Kopieringskonstruktor
Kopieringskonstruktorn används för att skapa ett nytt objekt som kopia av ett annat objekt.

```cpp
// Create led1.
Gpio led1{13U, Direction::Output, false};

// Create copy of led1.
Gpio led2{led1};
```

En explicit kopieringskonstruktor kan implementeras enligt följande:

```cpp
/**
 * @brief Copy constructor.
 *
 * @param[in] other GPIO instance to copy.
 */
Gpio(const Gpio& other) noexcept
    : myPin{other.myPin}
    , myDirection{other.myDirection}
    , myState{other.myState}
{}
```

Här initierar vi den nya instansens medlemsvariabler med värdena från objektet `other`.

---

#### Flyttkonstruktor
Flyttkonstruktorn används för att skapa ett nytt objekt genom att flytta resurser från ett annat objekt.

För en enkel klass som `Gpio`, där vi bara lagrar små inbyggda typer, finns det egentligen inga tunga resurser att flytta. Trots detta är det värdefullt att visa syntaxen.

```cpp
#include <utility>
// Create led1.
Gpio led1{13U, Direction::Output};

// Move led1 into led2.
Gpio led2{std::move(led1)};
```

**Notering**: För att anropa flyttkonstruktorn används funktionen `std::move()` från `<utility>`.

En explicit flyttkonstruktor kan implementeras enligt följande:

```cpp
/**
 * @brief Move constructor.
 *
 * @param[in, out] other GPIO instance to move from.
 */
Gpio(Gpio&& other) noexcept
    : myPin{other.myPin}
    , myDirection{other.myDirection}
    , myState{other.myState}
{
    // Clear resources allocated by `other`
    other.myState = false;
}
```

**Noteringar**: 
* `other` är en **rvalue‑referens**, vilket skrivs `Gpio&&`. Detta gör att objektets resurser kan flyttas i stället för att kopieras.
* Vi kan inte ändra `other.myPin` eller `other.myDirection`, eftersom dessa medlemsvariabler är deklarerade som `const`.
* För denna klass är flyttning i praktiken identisk med kopiering, eftersom inga resurser ägs dynamiskt.
* Om `other` innehöll andra resurser, såsom dynamiskt allokerat minne, hade detta raderats efter flytten.

---

#### Kopieringstilldelningsoperator
En kopieringstilldelningsoperator används när ett redan existerande objekt ska tilldelas från ett annat objekt.

```cpp
// Create led1 and led2.
Gpio led1{13U, Direction::Output};
Gpio led2{12U, Direction::Output};

// Make led2 a copy of led1.
led2 = led1;
```

För klassen `Gpio` är detta dock problematiskt. Vid tilldelning måste ett redan existerande objekt skrivas över, men `myPin` och `myDirection` är konstanta och får inte ändras efter initieringen.

En förenklad operator hade annars kunnat se ut så här:

```cpp
/**
 * @brief Copy assignment operator.
 *
 * @param[in] other GPIO instance to copy from.
 *
 * @return Reference to this instance.
 */
Gpio& operator=(const Gpio& other) noexcept
{
    if (this != &other)
    {
        myState = other.myState;
    }
    return *this;
}
```

Notera att vi **inte** kan skriva:

```cpp
myPin       = other.myPin;
myDirection = other.myDirection;
```

eftersom dessa är konstanta.

---

#### Flyttilldelningsoperator
På samma sätt finns en flyttilldelningsoperator, som används när ett objekt tilldelas från ett temporärt objekt eller via `std::move` från `<utility>`:

```cpp
#include <utility>

Gpio led1{13U, Direction::Output};
Gpio led2{12U, Direction::Output};

led2 = std::move(led1);
```

En förenklad implementation hade kunnat se ut så här:

```cpp
/**
 * @brief Move assignment operator.
 *
 * @param[in] other GPIO instance to move from.
 *
 * @return Reference to this instance.
 */
Gpio& operator=(Gpio&& other) noexcept
{
    if (this != &other)
    {
        myState       = other.myState;
        other.myState = false;
    }
    return *this;
}
```

Även här kan vi dock inte flytta `myPin` eller `myDirection`, eftersom dessa är konstanta.

---

### Rekommendation
I många verkliga embedded‑system väljer man att förbjuda kopiering och flyttning helt, eftersom ett objekt ofta representerar en unik hårdvaruresurs.

De raderade kopierings- och flyttoperationerna placeras normalt längst ned i det publika segmentet:

```cpp
Gpio(const Gpio&)            = delete; // No copy constructor.
Gpio(Gpio&&)                 = delete; // No move constructor.
Gpio& operator=(const Gpio&) = delete; // No copy assignment.
Gpio& operator=(Gpio&&)      = delete; // No move assignment.
```

På så sätt säkerställs att varje `Gpio`‑objekt representerar exakt en fysisk GPIO‑pinne och inte kan dupliceras av misstag.

---
