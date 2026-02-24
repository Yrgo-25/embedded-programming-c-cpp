# L05 - Klasser (del I)

## Dagordning
* Grundläggande objektorientering i C++: klasser, inkapsling med mera.
* Fördjupning: 
    * Filorganisation för klasser - uppdelning i header- och sourcefiler.
    * Kopierings- och förflyttningssemantik samt nyckelord `noexcept` och `explicit`.

## Mål med lektionen
* Kunna skapa enkla klasser i C++.
* Känna till innebörden av begrepp såsom *konstruktor* och *inkapsling*.
* Kunna använda enumerationsklasser.

## Instruktioner

### Förberedelse
* Läs [bilaga A](./appendix/a_classes1.md) för en introduktion till klasser i C++.
* Läs gärna också [bilaga B](./appendix/b_classes2.md), som utgör en fördjupning gällande klasser.

### Repetition
* Betrakta nedanstående kodsnutt:
    * Förklara vad funktionen `assign()` gör. Vad innebär det att `seed` är markerad `static`?
    * Vad är värdet på `x` efter den första respektive den andra tilldelningen? Motivera ditt svar.
    * Skriv motsvarande kod i C++ med referenser i stället för pekare.

```c
#include <stdint.h>
#include <stdio.h>

static void assign(uint8_t* x) 
{
    static uint8_t seed = 255U;
    *x = ++seed;
}

int main(void)
{
    uint8_t x = 0;
    assign(&x);
    printf("x = %u after the first assignment!\n", x);
    assign(&x);
    printf("x = %u after the second assignment!\n", x);
    return 0;
}
```

### Under lektionen
* Två implementationer av klasser genomförs live under lektionen:
    * En enklare implementerad endast i en headerfil.
    * En mer utvecklad variant uppdelad i en header- och en källkodsfil.
* Genomför följande [övningsuppgifter](./appendix/c_exercises.md).

## Utvärdering
* Vad är skillnaden mellan en strukt och en klass i C++?
* Vad innebär inkapsling?
* Vad är en konstruktor och när anropas den?
* Vad är en destruktor och när anropas den?
* Vad betyder nyckelordet `explicit`?
* Vad används `noexcept` till?

---

## Nästa lektion
* Övning: GPIO-driver i C++.

---
