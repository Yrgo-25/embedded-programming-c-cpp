# Bilaga A

## Arv i C++
Arv är en grundläggande princip i C++ som möjliggör att man skapa nya klasser som bygger vidare på befintliga klasser. På detta sätt kan man "återanvända" hela eller delar av befintliga klasser och därigenom slippa skriva samma kod för flera olika klasser.

Rent terminologiskt gäller att:
* En klass som ärver en annan kallas en underklass (eller subklass).
* Den klass som ärvs från kallas en basklass (eller superklass).

Anta att en basklass döpt `driver::Gpio` har skapats för att enkelt kunna styra en GPIO-pin för en mikrokontroller:

```cpp
namespace driver
{
class Gpio final
{
public:
    explicit Gpio(std::uint8_t pin) noexcept;
    ~Gpio() noexcept;

    bool read() const noexcept;
    void write(bool enable) noexcept;

    Gpio()                       = delete;
    Gpio(const Gpio&)            = delete;
    Gpio(Gpio&&)                 = delete;
    Gpio& operator=(const Gpio&) = delete;
    Gpio& operator=(Gpio&&)      = delete;

protected:
    // Add GPIO-specific methods for initialization and such here.
    void init() noexcept;

    /** The pin the GPIO is connected to.*/
    std::uint8_t myPin;
};
} // namespace driver
```

Notera att nyckelordet `protected` används i stället för `private` i detta fall:
* Anledningen till detta är att vi vill låta eventuella underklasser kunna komma åt de delar av klassen som annars är privata (under förutsättning att så kallats publikt arv används). 
* Skillnaden mellan privata och skyddade medlemmar är alltså att underklasser kan komma åt skyddade medlemmar, men inte privata. Varken skyddade eller privata medlemmar är dock åtkomliga i övrigt.

### Typer av arv
Det finns tre typer av arv i C++:

* **Publikt arv** Den absolut vanligaste typen. Publika och skyddade medlemmar i basklassen behåller sina åtkomsträttigheter i underklassen. Används när underklassen är en specialisering av basklassen.
* **Privat arv:** Alla medlemmar i basklassen blir privata i underklassen. Används när basklassen endast ska användas
internt i underklassen.
* **Skyddat arv** Publika och skyddade medlemmar i basklassen blir skyddade i underklassen. Fungerar alltså som privat arv, med skillnaden att eventuella underklasser till aktuell underklass också får åtkomst till basklassen internt.

Nedan visas ett exempel på publikt arv, där en klass döpt `driver::Led` ärver GPIO-funktionaliten från klassen `driver::Gpio`:
* Metoderna `write()` och `read()` från basklassen `driver::Gpio` kan därmed användas av instanser av klassen `driver::Led`, alltså lysdioder. 
* En ny metod, `toggle()`, har lagts till för att toggla lysdioden:

```cpp
namespace driver
{
class Led final : public Gpio
{
public:
    explicit Led(std::uint8_t pin) noexcept;
    ~Led() noexcept;

    void toggle() noexcept;

    Led()                      = delete;
    Led(const Led&)            = delete;
    Led(Led&&)                 = delete;
    Led& operator=(const Led&) = delete;
    Led& operator=(Led&&)      = delete;
};
} // namespace driver
```

Notera att:
* Klassen `driver::Led` ärver klassen `driver::Gpio` publikt genom att vi skriver `: public Gpio` efter klassens namn. Eftersom båda klasser ligger i samma namnrymd behöver vi inte använda prefixet `driver` framför `Gpio`.
* Nyckelordet `final` har lagts till direkt efter klassens namn `driver::Led` så att denna klass inte kan
ärvas. Som tumregel är det en god idé att lägga till `final` om klassen inte avses kunna ärvas.
Ibland kan det dock vara önskvärt att kunna ärva implementationen för test och då får detta
nyckelord slopas.

Genom att ärva klassen `driver::Gpio` slapp vi lägga till en medlemsvariabel som håller lysdiodens pin-nummer samt rutiner för att skriva eller läsa lysdiodens utsignal. I stället kan vi enkelt skapa en lysdiod via klassen `driver::Led`s konstruktor och sedan använda metoderna `write()` och `read()` för att styra lysdioden:

```cpp
// Create an LED connected to pin 9.
driver::Led led1{9U};

// Enable the LED.
led1.write(true);
```

På samma sätt hade vi kunnat skapa en underklass döpt `driver::Button` för att kunna styra en knapp.
I detta fall har en metod lagts till för att aktivera/inaktivera avbrott vid nedtryckning
av tryckknappen. Motsvarande metod för att kolla om avbrott är aktiverat har också lagts till.
Eftersom knappen inte ska kunna styras ser vi till att metoden `write()` är privat;
vi kan inte radera ärvda metoder, men vi kan ändra synligheten.

```cpp
namespace driver
{
class Button final : public Gpio
{
public:
    explicit Button(std::uint8_t pin) noexcept;
    ~Button() noexcept;

    void enableInterrupt(bool enable) noexcept;
    bool isInterruptEnabled() const noexcept;

    Button()                         = delete;
    Button(const Button&)            = delete;
    Button(Button&&)                 = delete;
    Button& operator=(const Button&) = delete;
    Button& operator=(Button&&)      = delete;

private:
    void write(bool enable) noexcept;
};
} // namespace driver
```

Via denna klass kan vi sedan enkelt läsa lysdiodens utsignal, aktivera avbrott och så vidare:

```cpp
// Create a button connected to pin 13.
driver::Button button1{13U};

// Enable interrupt at pressdown.
button1.enableInterrupt(true);

// Toggle led1 if the button is pressed.
if (button1.read()) { led1.toggle(); }
```

---
