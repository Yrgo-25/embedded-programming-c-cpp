# L12 - Introduktion till driverbibliotek för ESP32

## Dagordning
* Introduktion till **P02**.
* Översikt av projektets arkitektur:
  * Interfaces.
  * Riktiga drivers.
  * Stubbar.
  * Systemlogik.
* Genomgång av filstruktur och krav.
* Påbörja implementation av första drivers och stubbar.

## Mål med lektionen
* Förstå syftet med ett driverbibliotek och varför vi separerar logik från hårdvara.
* Kunna beskriva skillnaden mellan:
  * Interface.
  * Riktig driver.
  * Stub.
  * Systemlogik.
* Kunna implementera:
  * Ett enkelt driver-interface.
  * En stub som simulerar hårdvara.
* Kunna kompilera systemlogik mot stubbar utan hårdvara.

## Förutsättningar
* Grundläggande kunskap om klasser och interfaces i C++ (L05–L08).
* ESP-IDF installerat och konfigurerat.

## Instruktioner

### Förberedelse
* Repetera innehållet i:
    * [L05](../L05/README.md) samt [L06](../L06/README.md) för information om klasser.
    * [L07](../L07/README.md) samt [L08](../L08/README.md) för information arv och interfaces.
* Läs igenom [uppgiftsbeskrivningen](../../projects/P02/README.md) innan lektionen.  
  Påbörja gärna arbetet om ni känner er redo.

### Under lektionen
* Projektet ska genomföras självständigt.
* Ställ frågor vid behov.
* Fokusera på att få arkitekturen rätt innan detaljer.

#### Delmål
* Projektets mappstruktur skapad enligt rekommendation.
* `driver::gpio::Interface` implementerad.
* `driver::gpio::Stub` implementerad.
* `driver::gpio::Esp32s3` påbörjad eller klar.
* `driver::timer::Interface` implementerad.
* `driver::timer::Stub` implementerad.

---

### Ni ska kunna
* Förklara varför systemlogik inte ska känna till hårdvarudetaljer.
* Skapa ett interface och två implementationer:
  * En riktig driver.
  * En stub.
* Köra systemlogik mot stubbar i stället för riktig hårdvara.
* Strukturera ett projekt enligt given arkitektur.

## Utvärdering
1. Vad är syftet med ett driver-interface?
2. Vad är skillnaden mellan:
   * En stub.
   * En riktig driver.
3. Varför ska systemlogiken inte inkludera ESP-IDF-headers?
4. Vilka fördelar får vi av att kunna köra logiken mot stubbar?

## Nästa lektion
* Factory-mönster.
* Dependency injection via factory.
* Fortsatt arbete med **P02**.

---
