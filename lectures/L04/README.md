# L04 - Introduktion till C++

## Dagordning
* Övergång från C till modern C++ i inbyggda system:
  * Namnrymder.
  * `constexpr` och konstanter vid kompileringstid.
  * `noexcept` och undantagshantering i inbyggda system.
  * Defaultargument.
  * Moderna C++-struktar (medlemsfunktioner, konstruktor/destruktor och inkapsling).
  * Referenser.
  * Om tid finns: kort introduktion till funktionstemplates och parameterpack.
* Fortsatt arbete med **P01**.

---

## Mål med lektionen
* Bekanta sig med moderna C++-språkfunktioner som används i inbyggda system.
* Förstå hur C++ kan förbättra struktur och säkerhet i inbyggd mjukvara.
* Förstå hur moderna C++-struktar kan gruppera data och beteende i en och samma typ.
* Lära sig hur enkla drivrutiner kan implementeras med hjälp av struktar med medlemsfunktioner, konstruktorer/destruktorer och inkapsling.
* Förstå hur referenser förenklar funktionsanrop jämfört med pekare.

---

## Förkunskaper
* Kunskap om C-programmering (funktioner, pekare, struktar, etc.).
* Bekantskap med bitmanipulation och begrepp inom inbyggd programmering.

---

## Instruktioner

### Förberedelse
* Installera WSL och GCC-kompilatorn enligt beskrivningen i [Bilaga A](./appendix/a_compilation.md).
* Läs [Bilaga B](./appendix/b_from_c_to_cpp.md).

## Under lektionen
* Genomför övningarna i [Bilaga C](./appendix/c_exercises.md):
  * Ni får tid under lektionen att genomföra övningarna.
  * Lösningarna diskuteras därefter gemensamt i klassen.

---

## Utvärdering
* Vad är syftet med `constexpr`?
* Varför används `noexcept` ofta i inbyggda system?
* Hur skiljer sig en C++-strukt från en traditionell C-strukt?
* Varför är konstruktorer och destruktorer användbara i inbyggd C++?
* Vad är syftet med att göra medlemsvariabler privata?
* Vilka fördelar har referenser jämfört med pekare?

---

## Nästa lektion
* Klasser i C++:
  * Fördjupning i konstruktorer och destruktorer.
  * Nyckelorden `explicit`, `final`, `default` och `delete`.
  * Kopieringskonstruktorer.
  * Flyttkonstruktorer.
  * Tilldelningsoperatorer.
* Fortsatt arbete med **P01**.

---
