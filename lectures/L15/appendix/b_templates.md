# Bilaga B

## Templates, type traits och constraints
Templates gör det möjligt att skriva generisk kod som fungerar för flera typer: 
* Templates fungerar både för funktioner samt typer (klasser och struktar). 
* I denna bilaga demonstreras funktionstemplates.

### Funktionstemplates
Som exempel, nedan visas en funktion `add()`, som returnerar två tal av en godtycklig typ `T`:

```cpp
/**
 * @brief Add two numbers.
 * 
 * @tparam T The numeric type. Must be arithmetic.
 * 
 * @param[in] x The first number.
 * @param[in] y The second number.
 * 
 * @return The sum of the two numbers.
 */
template<typename T>
T add(const T x, const T y) noexcept { return x + y; }
```

När vi anropar:

```cpp
const auto sum1 = add<int>(1, 2);
const auto sum2 = add<double>(1.0, 2.0);
```

kommer kompilatorn att generera två separata funktioner:

```cpp
int add(const int x, const int y) noexcept { return x + y; }
double add(const double x, const double y) noexcept { return x + y; }
```

Notera att:
* Vi skriver `template <typename T>` direkt innan funktionshuvudet för att deklarerar att vi vill ha en valbar datatyp T, som i detta fall kallas T.
* Framför typen T skriver vi nyckelordet `typename` (eller `class`).
* Vi kan sätta ett defaultvärde på datatypen T direkt i template-deklarationen, exempelvis:

```cpp
template<typename T = int>
```

---

### Compile‑Time Polymorfism
Templates är ett exempel på så kallad **compile-time polymorfism**, dvs. typen bestäms vid kompilering (polymorfism = mångformighet).
Detta skiljer sig från interfaces, som använder **run-time polymorfism**, dvs. typen bestäms vid när programmet körs.

---

### Templates i inbyggda system
Templates kan vara mycket effektiva eftersom:
* Ingen run-time overhead uppstår.
* Kod kan optimeras aggressivt av kompilatorn.

Men det finns nackdelar:
* Flera instanser genereras om flera olika typer används (såsom funktionen `add()` ovan) → större binär.
* Ökad kompileringstid.
* Svårare felmeddelanden.

I inbyggda system är detta relevant eftersom:
* Flash-minne är begränsat.
* RAM är begränsat.
* Binärstorlek påverkar systemets egenskaper.

Templates bör därför användas med eftertanke.

---

### Type Traits
Type traits är hjälptyper i `<type_traits>` som låter oss undersöka egenskaper hos typer vid kompilering.

Några vanliga type traits presenteras nedan:
* `std::is_arithmetic`: Indikerar ifall en specifik datatyp `T` är aritmetisk/numerisk, både heltals- och flyttalsdatatyper.
* `std::is_integral`: Indikerar ifall en specifik datatyp `T` är av heltalstyp, antingen signerad eller osignerad.
* `std::is_unsigned`: Indikerar ifall en specifik datatyp `T` är av osignerad heltalstyp, såsom `std::uint8_t` och `std::uint32_t`.
* `std::is_signed`: Indikerar ifall en specifik datatyp `T` är av signerad heltalstyp, såsom `int` och `long`.
* `std::is_floating_point`: Indikerar ifall en specifik datatyp `T` är en flyttalsdatatyp. 

Fler type traits hittar du [här](https://en.cppreference.com/w/cpp/header/type_traits).

Varje type trait innehar en statisk bool-variabel `value`. Som exempel, för `std::is_integral<T>` gäller att:
* `value = true` om `T` är av heltaltstyp.
* `value = false` om `T` är av annan typ.

För att komma åt bool-variabeln `value` skriver man:
* `std::is_integral<int>::value` eller
* `std::is_integral_v<T>` (om C++17 eller en senare version av C++ används).

I nedanstående exempel används typen `std::is_integral<T>` för att kontrollera ifall en given typ `T` är av heltalstyp:

```cpp
#include <type_traits>

std::is_integral<int>::value     // True, då 'int' är av heltalstyp.
std::is_integral<double>::value  // False, då 'double' inte är av heltalstyp.
std::is_integral_v<std::uint8_t> // True, då 'std::uint8_t' är av heltalstyp.
```

---

### Compile-Time Constraints med `static_assert()`
Vi kan kombinera type traits med `static_assert()`, en statisk kontroll av ett påstående (en assertion), för att generera kompileringsfel om en icke lämplig typ används för en funktion. Vill vi också skriva ut ett felmeddelande tillsammans med kompileringsfelet kan vi lägga till detta direkt efter påståendet. 

Som exempel, funktionen `add()` ovan bör inte kunna användas på icke-numeriska typer:
* Vi kontrollerar typen `T` med `std::is_arithmetic<T>`, som blir `false` om typen inte är av numerisk typ.
* För att generera ett kompileringsfel om typen inte är numerisk använder vi `static_assert()`.
* I detta fall skrivs felmeddelandet "Cannot add values of non-arithmetic types!" ut om typen inte är numerisk.

```cpp
/**
 * @brief Add two numbers.
 * 
 * @tparam T The numeric type. Must be arithmetic.
 * 
 * @param[in] x The first number.
 * @param[in] y The second number.
 * 
 * @return The sum of the two numbers.
 */
template<typename T>
T add(const T x, const T y) noexcept 
{
    static_assert(std::is_arithmetic<T>::value, "Cannot add values of non-arithmetic types!");
    return x + y; 
}
```

Om man försöker använda funktionen med exempelvis `std::string` som typ genereras ett kompileringsfel:

```cpp
const auto sum = add<std::string>("C++", "programmer");
```

Detta är särskilt värdefullt i embedded-system där:
* Fel bör upptäckas så tidigt som möjligt.
* Run-time-fel kan vara svåra att felsöka.
* Säkerhet och robusthet är centrala.

---

### Skapa egna type traits
Ibland händer det att det inte finns ett type trait i standardbiblioteket som täcker allt man vill ha. Det går då utmärkt att skapa egna varianter. Att göra detta underlättar också förståelsen av type traits.

Ett type trait är inte något annat än ett structtemplate med en enda statisk medlem av datatypen `bool` döpt `value`. Värdet på `value` är satt till `false` som default. Bara för de typer vi vill sätter vi variabeln `value` till `true`.

Anta att vi vill skapa ett type trait för att kontrollera ifall en datatyp T är text, alltså av datatypen `const char*` eller `std::string`. Vi döper detta type trait till `is_string`. Ett struct-template för denna typ visas nedan:

```cpp
/**
 * @brief Type trait for checking if specified type is of string type.
 * 
 * @tparam T The type to check.
 */
template <typename T>
struct is_string
{
    // Set to true if the given type is of string type, false otherwise.
    static const bool value{false};
};
```

För en given datatyp T kommer variabeln `value` vara `false`. Vi ser sedan till att `value` är `true` då T = `const char*` samt T = `std::string` via nedanstående överlagringar:

```cpp
/**
 * @brief Specialization of the type trait `is_string` for the type `const char*`.
 */
template <>
struct is_string<const char*>
{
    // Indicate that `const char*` is considered a string type.
    static const bool value{true};
};

/**
 * @brief Specialization of the type trait `is_string` for the type `std::string`.
 */
template <>
struct is_string<std::string>
{
    // Indicate that `std::string` is considered a string type.
    static const bool value{true};
};
```

Vi testar att använda vårt nya type trait i nedanstående funktion `print` så att datatypen `T`, dvs. elementen i given vektor `data`, kan vara aritmetisk eller text. Notera i påståendet att det räcker nu att `T` är aritmetisk eller av strängtyp:

```cpp
/**
 * @brief Print content of the given vector.
 *
 * @tparam T The vector type. Must be arithmetic or of string type.
 * 
 * @param[in] data Vector holding the data to print.
 * @param[in] ostream Output stream to use (default = terminal print).
 */
template <typename T>
void print(const std::vector<T>& data, std::ostream& ostream = std::cout)
{
    // Generate a compilation error if the vector type isn't of arithmetic or string type.
    static_assert(std::is_arithmetic<T>::value || is_string<T>::value, 
        "Cannot print values other than numbers and text!");

    // Terminate the function if the vector is empty.
    if (data.empty()) { return; }
    
    // Print each number on a separate line.
    ostream << "--------------------------------------------------------------------------------\n";
    for (const auto& element : data)
    {
        ostream << element << "\n";
    }
    ostream << "--------------------------------------------------------------------------------\n\n";
}
```

---

### Templates vs Interfaces i P02
I tidigare delen av kursen har vi använt:
* Interfaces
* Abstract Factory
* `std::unique_ptr`

Detta är **run-time polymorfism**.

Templates däremot är **compile-time polymorfism**.

| Teknik | När bestäms typen? | Overhead | Flexibilitet |
|--------|-------------------|----------|--------------|
| Templates | Vid kompilering | Ingen run-time overhead | Mindre flexibel vid körning |
| Interfaces | Vid körning | Virtuella funktionsanrop | Mer flexibel |

Vi använder interfaces i **P02** eftersom:
* Vi vill kunna byta driver vid körning.
* Vi vill kunna använda stubbar.
* Vi vill injicera beroenden via factory.

Templates är lämpliga när:
* Typen inte behöver bytas dynamiskt.
* Prestanda är kritisk.
* Compile-time säkerhet är viktig.

---

### Sammanfattning
I detta avsnitt har vi sett hur:
* Templates skapar generisk kod vid kompilering.
* Flera kopior av samma funktion kan genereras.
* Type traits möjliggör compile-time constraints.
* `static_assert` skapar tydliga felmeddelanden.

Koppling till **P02**:
* Interfaces ger run-time polymorfism via virtuella metoder.
* Templates ger compile-time polymorfism utan virtuella anrop.
* Båda är viktiga verktyg i modern C++.
* I inbyggda system måste vi balansera flexibilitet, säkerhet och binärstorlek.

Templates är kraftfulla – men kan:
* Öka binärstorleken.
* Förlänga kompileringstiden.
* Ge svårlästa felmeddelanden.

---

