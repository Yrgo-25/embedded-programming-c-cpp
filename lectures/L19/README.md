# L19 - Slutförande och redovisning av **P03**

## Dagordning
* Slutförande och redovisning av **P03**.
* Riktad repetition inför sluttentamen utifrån identifierade behov.

## Mål med lektionen
* Demonstrera ett fungerande system enligt kravspecifikationen.
* Förklara hur interfaces, abstract factory och dependency injection används.
* Motivera arkitekturval och diskutera testbar design.

## Förutsättningar
* God kunskap i C++ (klasser, referenser, RAII, smarta pekare).
* Förståelse för projektets arkitektur och kravspecifikation.

## Instruktioner

### Förberedelse
* Läs igenom [uppgiftsbeskrivningen](../../projects/P03/README.md) och säkerställ att samtliga krav är uppfyllda.
* Förbered en kort och strukturerad demonstration (2–3 minuter).

### Under lektionen
Säkerställ att:
* Systemet uppfyller alla krav i **P03**.
* Koden följer angiven filstruktur.
* Systemlogiken är separerad från hårdvaruberoende kod.
* Drivers skapas via factory och ägs via `std::unique_ptr`.

#### Redovisning (krav)
Redovisningen sker individuellt (ca 2–3 minuter per student).

Ni ska visa:
* Funktionalitet (LED, blinkläge, serial-kommandon, temperatur).
* Att systemlogiken arbetar mot interfaces.
* Att en factory används för att skapa drivers.
* Hur stubbar möjliggör testbarhet utan hårdvara.
* Hur dependency injection används (t.ex. ADC → TMP36).

## Utvärdering
* Vilken del av projektet var svårast?
* Vad har ni lärt er om arkitektur och testbar design?
* Vad känner ni er osäkra på inför sluttentamen?

## Nästa lektion
* Tentamen – C++-teori.

---
