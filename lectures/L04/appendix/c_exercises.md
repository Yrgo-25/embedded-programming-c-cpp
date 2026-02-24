# Bilaga C

## Övningsuppgifter

## Uppgift 1
Skapa en fil döpt *main.cpp*. I denna fil, skapa en funktion döpt *getSmallestInt*, som returnerar det minsta talet i en vektor innehållande heltal av datatypen *int*.  

Som exempel:
* Om en vektor innehållande talen {3, 6, 4, 8, 6, 8, -2, -1} passeras ska heltalet -2 returneras.
* Om en vektor innehållande talen {1, 2, -1, -1, 0} passeras ska heltalet -1 returneras.
* Om en tom vektor passeras vid funktionsanropet ska heltalet 0 returneras.

Placera denna funktion i en namnrymd döpt *vector*.  

Testa att funktionen fungerar korrekt genom att skriva ut returvärdet vid anrop med följande vektorer:

```cpp
const std::vector<int> v1{3, 6, 4, 8, 6, 8, -2, -1}; 
const std::vector<int> v2{1, 2, -1, -1, 0};          
const std::vector<int> v3{};                        
```

Se lösningsförslag [här](#lösningsförslag-för-uppgift-1).

---

## Uppgift 2 
Använd funktionen *getSmallestInt* för att radera det minsta talet ur följande vektor:

```cpp
std::vector<int> numbers{1, 2, 5, 4, 8, 9, 0, -3, -5, -2};
```

Skriv ut vektorns innehåll och se till att talet -5 har raderats.

**Tips:** Läs [nedanstående segment](#ytterligare-vektormetoder) tips på metoder du kan använda för denna uppgift:
* Iterera genom vektorn med en C++-iterator och radera det minsta talet via vektor-metoden *erase*.  
* Iteration genom en vektor med C++-iteratorer kan du implementera via en for-loop tillsammans med vektor-metoder *begin* samt *end*.

Se lösningsförslag [här](#lösningsförslag-för-uppgift-2).

---

## Ytterligare vektormetoder

### Erhåll vektorns början och slut via metoder `begin` och `end`
Metoderna `begin` och `end` kan användas för att enkelt ta reda på början och slutet på en given vektor. Detta kan exempelvis användas för att iterera genom vektorn (via en for-sats) eller för att ta bort specifika element (se metoden `erase` nedan).

Som exempel, nedanstående for-sats kan användas för att iterera genom en vektor döpt *numbers*. Varje element skrivs ut följt av ett blanksteg. 

```cpp
#include <iostream>
#include <vector>

int main()
{
    // Create a vector holding three numbers.
    std::vector<int> numbers{1, 2, 3};
    
    // Print the numbers on a single line in the terminal.
    for (auto number{numbers.begin()}; number < numbers.end(); ++number)
    {
        std::cout << *number << " ";
    }
    // Terminate the program with success code 0.
    return 0;
}
```

Notera att:
* En dereferensoperator måste användas för att få tillgång till innehållet på adressen som iteratorn `number` pekar på, på samma sätt som när pekare används i C. `*number` kan därmed avläsas som innehållet på adressen som `number` pekar på.
* Vi kan med fördel använda nyckelordet `auto` för iteratorer. Om vi hade velat skriva ut typen hade det blivit mycket längre, då denna är `std::vector<int>::iterator` i detta fall. För mer komplicerade typer kan det bli väldigt mycket mer att skriva och svårare att läsa.

---

### Ta bort specifika element via metoden `erase`
Metoden `erase` fungerar utmärkt för att ta bort element på ett specifikt index. Detta index måste dock specificeras via en iterator. Som exempel, för att ta bort det tredje elementet, alltså elementet på index 2, i en vektor döpt *numbers*, kan vi därmed skriva

```cpp
numbers.erase(numbers.begin() + 2U);
```

Notera att vi specifikt skriver att vi ska radera elementet på startadressen (adressen för elementet på index 0) + 2U för att specificera index 2.

Med denna metod kan vi också ta bort multipla efterföljande element. Som exempel, för att ta bort elementet på index 2 - 5 i en vektor döpt *numbers* kan vi skriva

```cpp
numbers.erase(numbers.begin() + 2U, numbers.begin() + 5U);
```

---

## Lösningsförslag för uppgift 1

```cpp
#include <iostream>
#include <vector>

namespace vector
{
/**
 * @brief Get the smallest integer in given vector.
 * 
 * @param[in] numbers Vector holding numbers.
 * 
 * @return The smallest integer in the vector, or 0 if the vector is empty.
 */
int getSmallestInt(const std::vector<int>& numbers) noexcept
{
    // Return 0 if the vector is empty.
    if (numbers.empty()) { return 0; }

    // Consider the first value of the vector to be the smallest at start.
    auto smallest{numbers[0U]};
    
    // Compare each value in the vector, store the smallest one.
    for (const auto& number : numbers)
    {
        if (number < smallest) { smallest = number; }
    }
    // Return the smallest value found.
    return smallest;
}
} // namespace vector

/**
 * @brief Test the `vector::getSmallestInt` function.
 * 
 * @return 0 upon termination of the program.
 */
int main()
{
    // Create three vectors holding different amounts of integers for the test.
    const std::vector<int> v1{3, 6, 4, 8, 6, 8, -2, -1}; 
    const std::vector<int> v2{1, 2, -1, -1, 0};          
    const std::vector<int> v3{};   

    // Print the smallest value found in each vector.
    std::cout << "Smallest integer in v1: " << vector::getSmallestInt(v1) << "\n";
    std::cout << "Smallest integer in v2: " << vector::getSmallestInt(v2) << "\n";
    std::cout << "Smallest integer in v3: " << vector::getSmallestInt(v3) << "\n";
    return 0;
}
```

---

## Lösningsförslag för uppgift 2

```cpp
#include <iostream>
#include <vector>

namespace vector
{
/**
 * @brief Get the smallest integer in given vector.
 * 
 * @param[in] numbers Vector holding numbers.
 * 
 * @return The smallest integer in the vector, or 0 if the vector is empty.
 */
int getSmallestInt(const std::vector<int>& numbers) noexcept
{
    // Return 0 if the vector is empty.
    if (numbers.empty()) { return 0; }

    // Consider the first value of the vector to be the smallest at start.
    auto smallest{numbers[0U]};
    
    // Compare each value in the vector, store the smallest one.
    for (const auto& number : numbers)
    {
        if (number < smallest) { smallest = number; }
    }
    // Return the smallest value found.
    return smallest;
}

/**
 * @brief Print the content of given vector.
 * 
 * @param[in] numbers Vector holding numbers.
 * @param[in] ostream Reference to the output stream to use (default = terminal print).
 */
void print(const std::vector<int>& numbers, std::ostream& ostream = std::cout) noexcept
{
    // Print an opening separator line.
    ostream << "--------------------------------------------------------------------------------\n";
    
    // Print each number in given vector on a separate line.
    for (const auto& number : numbers) { ostream << number << "\n"; }
    
    // Print a trailing separator line.
    ostream << "--------------------------------------------------------------------------------\n\n";
}
} // namespace vector

/**
 * @brief Remove the smallest integer held by a specific vector.
 * 
 * @return 0 upon termination of the program.
 */
int main()
{
    // Create a vector holding integers.
    std::vector<int> numbers{1, 2, 5, 4, 8, 9, 0, -3, -5, -2};

    // Get the smallest number in the vector.
    const auto smallest{vector::getSmallestInt(numbers)};

    // Iterate through the vector and remove the smallest number.
    // Note: Removing an element from the vector invalidates the iterator, hence the loop will
    // terminate immediately after removing the first occurrence of the smallest number.
    for (auto number{numbers.begin()}; number < numbers.end(); ++number) 
    {
        if (smallest == *number) { numbers.erase(number); }
    }
    // Print the vector content and terminate the program.
    vector::print(numbers);
    return 0;
}
```

---