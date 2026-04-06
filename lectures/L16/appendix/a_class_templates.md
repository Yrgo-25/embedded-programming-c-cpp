# Bilaga B

## Klasstemplates - Implementering av en generisk container (`container::Vector`)
I denna bilaga demonstreras hur man skapar en klasstemplate, dvs. en generisk datatyp som kan lagra objekt av valfri typ.

Vi använder ett exempel som liknar `std::vector`, men skapar en egen minimal implementation:

``` cpp
container::Vector<T>
```

Syftet är att:
* Förstå hur klasstemplates fungerar. 
* Förstå hur minneshantering påverkas i embedded-system.
* Använda type traits och constraints i en verklig container.
* Se skillnaden mellan compile-time och run-time polymorfism i praktiken.

---

### Vad är ett klasstemplate?
En klasstemplate är en mall för att skapa en klass där typen inte är bestämd förrän vid kompilering. Som exempel, nedan visas ett klasstemplate för en enkel vektor, `container::Vector`:

``` cpp
namespace container
{
template<typename T>
class Vector
{
public:
    Vector() noexcept
        : myData{nullptr}
        , mySize{}
    {}
private:
    T* myData;
    std::size_t mySize;
};
} // namespace container
```

När vi sedan skriver:

``` cpp
container::Vector<int> a{};
container::Vector<double> b{};
```

kommer kompilatorn att generera två separata typer:

``` cpp
namespace container
{
// Vector specialization for int.
class Vector<int>
{
public:
    Vector() noexcept
        : myData{nullptr}
        , mySize{}
    {}
private:
    int* myData;
    std::size_t mySize;
};

// Vector specialization for double.
class Vector<double>
{
public:
    Vector() noexcept
        : myData{nullptr}
        , mySize{}
    {}
private:
    double* myData;
    std::size_t mySize;
};
} // namespace container
```

Detta är compile-time polymorfism.

---

### Exempelimplementation
Nedan visas ett exempel på en enkel implementation av en vektor i form av ett klasstemplate.  
Denna typ har grundläggande funktionalitet för att:
* Skapa vektorer, antingen tomma eller initierade med ett godtyckligt antal värden.
* Ändra storlek på vektorn via `resize()`.
* Lägga till ett värde i slutet av vektorn via `pushBack()`.
* Iterera över elementen via `begin()` och `end()`, vilket också möjliggör användning av range-baserade for-loopar (`for (const auto& num : vec)`).

För att hålla implementationen enkel har följande gjorts:
* Funktioner `std::realloc()` samt `std::free()` från `<cstdlib>` används för att förenkla omallokeringar. Detta medför dock att vektortypen endast kan inneha triviala typer, dvs. typer som saknar egen destruktor eller specialdefinierad kopierings-/flyttlogik.
* Copy- och flyttkonstruktorerna samt motsvarande operatorer har raderats.

``` cpp
#pragma once

#include <cstddef>
#include <cstdlib>
#include <type_traits>

namespace container
{
template<typename T>
class Vector
{
    // Generate a compiler error if the given type isn't trivially copyable or destructible, 
    // since std::realloc() and std::free() are used in this implementation.
    static_assert(std::is_trivially_copyable<T>::value && std::is_trivially_destructible<T>::value, 
        "Vector type T must be of trivial type!");
public:
    Vector() noexcept
        : myData{nullptr}
        , mySize{}
    {
        // Initialized empty vector.
    }

    template<typename... Args>
    Vector(const Args& ...args) noexcept
        : Vector()
    {
        // Try to resize the vector to fit the given number of arguments.
        // Copy the arguments to the vector on success.
        if (resize(sizeof...(args)))
        {
            std::size_t i{};
            ((myData[i++] = args), ...);
        }
    }

    ~Vector() noexcept 
    {
        // Release allocated resources before deletion. 
        clear(); 
    }

    T& operator[](const std::size_t index) noexcept 
    { 
        // Return a reference to the element at given index.
        return myData[index]; 
    }
    const T& operator[](const std::size_t index) const noexcept 
    {
        // Return a read-only reference to the element at given index. 
        return myData[index]; 
    }

    T* begin() noexcept 
    { 
        // Return a pointer to the beginning of the data field.
        return myData; 
    }
    const T* begin() const noexcept 
    {
        // Return a read-only pointer to the beginning of the data field. 
        return myData; 
    }
    
    T* end() noexcept { return nullptr != myData ? myData + mySize : nullptr; }
    const T* end() const noexcept { return nullptr != myData ? myData + mySize : nullptr; }

    const T* data() const noexcept 
    { 
        // Return a read-only pointer to the data field.
        return myData; 
    }

    std::size_t size() const noexcept 
    {
        // Return the size of the vector. 
        return mySize; 
    }

    bool empty() const noexcept 
    { 
        // Return true if the vector is empty.
        return 0U == mySize; 
    }

    void clear() noexcept
    {
        // Free allocated resources, then reset the vector parameters.
        std::free(myData);
        myData = nullptr;
        mySize = 0U;
    }

    bool resize(const std::size_t size) noexcept
    {
        // Clear the vector if the requested size is 0.
        if (0U == size)
        {
            clear();
            return true;
        }

        // Try to reallocate the data field to the requested size, return false on failure.
        T* data{static_cast<T*>(std::realloc(myData, sizeof(T) * size))};
        if (nullptr == data) { return false; }
        myData = data;

        // Initialize new elements (if any).
        // Update the vector size, then return true to indicate success.
        for (std::size_t i{mySize}; i < size; ++i) { myData[i] = {}; }
        mySize = size;
        return true;
    }

    bool pushBack(const T& element) noexcept
    {
        const std::size_t newSize{mySize + 1U};
        // Try to reallocate the data field to fit one more element, return false on failure.
        T* data{static_cast<T*>(std::realloc(myData, sizeof(T) * newSize))};
        if (nullptr == data) { return false; }

        // Push the new element to the back of the vector, then return true to indicate success.
        myData = data;
        myData[mySize++] = element;
        return true;
    }

    Vector(Vector&)                  = delete; // No copy constructor.
    Vector(Vector&&)                 = delete; // No move constructor.
    Vector& operator=(const Vector&) = delete; // No copy assignment.
    Vector& operator=(Vector&&)      = delete; // No move assignment.

private:        
    /** Pointer to dynamically allocated data field. */
    T* myData;

    /** Size of the data field in number of elements. */
    std::size_t mySize;
};
} // namespace container
```

Ovanstående implementation medför att klasstemplate `container::Vector` fungerar i många avseenden likt `std::vector`, inklusive initiering med en argumentlista:

```cpp
container::Vector<int> vec1{1, 3, 5, 7, 9};
container::Vector<double> vec2{0.5, 1.5, 2.5, 3.5, 4.5};
```

---

### Placering av implementationen i headerfiler
Eftersom `container::Vector` är en template påverkar det hur implementationen måste placeras i källkoden.

Kompilatorn måste se hela definitionen av en template när den instansierar den, t.ex. när vi skriver:

``` cpp
container::Vector<int>
```

Det innebär att både klassens deklaration och metodernas implementation måste vara synliga där templaten används.

Det finns två vanliga lösningar:

#### a) Allt i samma headerfil
Det vanligaste är att placera både deklaration och implementation i samma headerfil, som exempel:

```
include/container/vector.h
```

Innehåller:

``` cpp
template<typename T>
class Vector { ... };

template<typename T>
void Vector<T>::push_back(const T& value)
{
    ...
}
```

Detta fungerar eftersom kompilatorn ser all kod när templaten instansieras.

---

#### b) Header + implementation-header
I större projekt separerar man ofta deklaration och implementation i två filer, men inkluderar implementationen i slutet av headerfilen.

Struktur:

```
include/container/vector.h
include/container/impl/vector_impl.h
```

Och i slutet av `vector.h`:

``` cpp
#include "container/impl/vector_impl.h"
```

Detta ger samma effekt som alternativ 1, men ger bättre struktur och läsbarhet.

---

#### Varför fungerar inte en `.cpp`-fil?
Om implementationen läggs i en vanlig `.cpp`-fil kan inte kompilatorn se koden när templaten instansieras i andra filer. Detta leder ofta till länkfel som:

```
undefined reference to ...
```

Templates måste därför implementeras i headerfiler (direkt eller indirekt via include).

---

### Embedded-perspektiv
I embedded-system måste vi tänka på följande:

#### Heap-användning
* Dynamisk allokering kan vara förbjuden.
* Fragmentering kan uppstå.

#### Binärstorlek
* Varje instans av `Vector<T>` genererar ny kod.
* `Vector<int>` ≠ `Vector<double>`

#### Exception-hantering
* I många embedded-system är exceptions avstängda.
* Därför används `noexcept`.

---

### Sammanfattning
I denna bilaga har vi sett:
* Hur man skapar en klasstemplate.
* Hur man hanterar manuell minnesallokering.
* Hur copy/move-semantik kan hanteras eller begränsas.
* Varför template-implementation måste ligga i headerfiler.
* Hur templates påverkar embedded-system.

`container::Vector<T>` är ett tydligt exempel på:
* Compile-time polymorfism.
* Generisk programmering.
* Prestandaoptimerad C++.
* Full kontroll över minne och livscykel.

---
