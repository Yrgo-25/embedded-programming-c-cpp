# L13 - Factory-mönster, arbete med **P03**

## Dagordning
* Introduktion till factory-mönstret.
* Varför vi använder factory i **P03**.
* Enkel factory för drivers med "råa" pekare.
* Hur systemlogiken skapar drivers via en factory.

## Mål med lektionen
* Förstå vad factory-mönstret är och varför det används.
* Förstå hur factory möjliggör växling mellan:
  * Riktiga drivers.
  * Stubbar.
* Förstå hur systemlogiken kan skapa drivers via en factory.
* Kunna läsa och följa ett enkelt factory-exempel.

## Förutsättningar
* Grundläggande kunskap om klasser och interfaces i C++ (L05–L08).
* Förståelse för stubbar och systemlogik (L12).
* ESP-IDF installerat och konfigurerat.

## Instruktioner

### Förberedelse
* Läs igenom [bilaga A](./appendix/a_factory_raw_pointers.md) för information om factories.

### Under lektionen
* Lyssna på genomgången av factory-mönstret.
* Följ med i kodexemplet och ta anteckningar.
* Ställ frågor vid behov.
* Efter genomgången:
  * Fortsätt arbeta på **P03**.
  * Fokusera på drivers, stubbar och systemlogik.
  * Factory implementeras i nästa lektion.

## Delmål under projektarbetet
* Interfaces för relevanta drivers implementerade.
* Stubbar för relevanta drivers implementerade.
* Riktiga drivers påbörjade eller implementerade.
* Systemlogiken kompilerar mot stubbar.
* Grundläggande funktionalitet i systemet påbörjad.

## Utvärdering
1. Vad är syftet med factory-mönstret?
2. Varför ska systemlogiken inte skapa drivers direkt med `new`?
3. Vad menas med att systemlogiken bara känner till interfaces?
4. Vem ansvarar för att radera driver-objekten i rå-pekare-exemplet?

## Nästa lektion
* Factory-mönster med smarta pekare.
* Implementation av factory i **P03**.
* Arbete vidare med projektet.

---
