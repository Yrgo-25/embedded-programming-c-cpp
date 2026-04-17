# **P02** – Driverbibliotek för ESP32‑S3

## Syfte
Ni ska i grupper om 3–4 bygga ett litet driverbibliotek i C++ för ESP32-S3.

Syftet är både att:
* Träna på arkitektur och design i C++ (interfaces, abstraction, dependency injection).
* Träna på att arbeta i små utvecklingsteam med Git (branches, pull requests, code reviews).

Projektet ska ha tydlig separering mellan:
* **Interfaces**: Abstraktioner som beskriver vad en driver kan göra.
* **Riktiga drivers** för ESP32-S3 (ESP-IDF).
* **Stubbar**: Test-/simuleringsklasser som efterliknar hårdvara.
* **Systemlogik**: En klass som arbetar mot interfaces och skapas via en factory (dependency injection).

Upplägget ska följa samma arkitekturprinciper som i kursrepot  
[test_automation](https://github.com/Yrgo-24/test-automation):
* Tydlig separering mellan drivers, stubbar och systemlogik.
* Interfaces mellan logik och hårdvara.
* Testbar design.

Repot kan användas som inspiration, men ni ska implementera er egen lösning.

**OBS!** I denna kurs skriver ni inte automatiserade tester. Däremot ska er design vara testbar; logiken ska kunna köras mot stubbar utan att känna till hårdvara. Dessa stubbar kommer sedan användas i kursen Mjuk- och hårdvarutestning.

---

## Filstruktur (rekommenderad)
Ni får gärna använda exakt denna struktur.

```
include/
    driver/
        adc/
            esp32s3.h
            interface.h
            stub.h
        factory/
            esp32s3.h
            interface.h
            stub.h
        gpio/
            esp32s3.h
            interface.h
            stub.h
        serial/
            esp32s3.h
            interface.h
            stub.h
        tempsensor/
            tmp36.h
            interface.h
            stub.h
        timer/
            esp32s3.h
            interface.h
            stub.h
    system/
        logic/
            logic.h
source/
    driver/
        adc/
            esp32s3.cpp
        factory/
            esp32s3.cpp
        gpio/
            esp32s3.cpp
        serial/
            esp32s3.cpp
        tempsensor/
            tmp36.cpp
        timer/
            esp32s3.cpp
    main.cpp
```

**Notering:** För enkelhets skull ska stubbarna implementeras helt i headerfilerna.

---

## Kravspecifikation

### Översikt
Ni ska implementera följande delar (nivå **G**):

1. **Interfaces**
   * `driver::adc::Interface` (ADC-driver)
   * `driver::factory::Interface` (Driver-factory)
   * `driver::gpio::Interface` (GPIO-driver)
   * `driver::serial::Interface` (Serial-driver/UART)
   * `driver::tempsensor::Interface` (Temperatursensor)
   * `driver::timer::Interface` (Timer-driver)

2. **Riktiga drivers (ESP32‑S3 / ESP‑IDF)**
   * `driver::adc::Esp32s3`
   * `driver::factory::Esp32s3`
   * `driver::gpio::Esp32s3`
   * `driver::serial::Esp32s3`
   * `driver::tempsensor::Tmp36`
   * `driver::timer::Esp32s3`

3. **Stubbar**
   * `driver::adc::Stub`
   * `driver::factory::Stub`
   * `driver::gpio::Stub`
   * `driver::serial::Stub`
   * `driver::tempsensor::Stub`
   * `driver::timer::Stub`

4. **Systemklass**
   * `system::logic::Logic`

---

## Funktionellt beteende (systemet)
Systemlogiken ska kunna köras i två lägen:
* **Hårdvaruläge:** Riktiga ESP32‑drivers.
* **Simulerat läge:** Stubbar.

Systemet ska uppfylla följande beteende (**G**):

### Beteende (G)
* Systemet styr **en LED** på en valfri GPIO.
* Systemet har ett **timerstyrt blinkläge**:
  * När blinkläge är aktivt togglar LED var `period_ms` (default t.ex. 500 ms).
  * När blinkläge är inaktivt är LED släckt.
* Systemet styrs via **Serial (UART)** med enkla kommandon.
* Systemet kan mäta temperatur via **TMP36**.

#### Kommando‑gränssnitt (G)
* `on`  → LED på (blink av)
* `off` → LED av (blink av)
* `blink on`  → aktivera blink
* `blink off` → avaktivera blink (LED av)
* `period 1000` → sätt blinkperiod i ms
* `status` → skriv ut aktuellt läge:
  * blinkläge (on/off)
  * period (ms)
  * aktuell temperatur (°C)
* `temp` → skriv ut aktuell temperatur (°C)

**Notering:** Serial‑drivern ska inte innehålla systemlogik. Tolkningslogik ligger i `system::logic::Logic`.

### Hårdvara (G – TMP36)
* TMP36 kopplas till **3.3V**, **GND** och en vald **ADC1‑pinne** (`Vout` → ADC).
* Dokumentera vald pin i README.
* `Vout` måste gå till en ADC1-pin som stödjer analog in (inte alla GPIO-pinnar gör det).

---

## Designkrav (viktiga)
* Systemlogiken får **inte** inkludera ESP‑IDF headers.
* Endast driver‑lagret får vara hårdvarunära.
* Systemlogik ska implementeras i en klass som tar in en factory via referens och skapar sina drivers via fabriken.
* Systemklassen ska äga sina drivers via `std::unique_ptr`.
* Factoryn räknas till driver-lagret och får därför inkludera ESP-IDF vid behov.
* Temperaturformler ska ligga i `driver::tempsensor::Tmp36` (inte i systemlogiken).
* `driver::tempsensor::Tmp36` ska **ta emot** en `driver::adc::Interface&` i sin konstruktor (dependency injection).
* ADC-instansen ska **ägas av systemlogiken** (via `std::unique_ptr`) och **skickas vidare** som referens när tempsensorn skapas.
* `main` ska vara "tunn": skapa objekt, koppla ihop och starta `run()`.

---

## Fabriker och smarta pekare
För att möjliggöra enkel växling mellan riktiga drivers och stubbar ska ni implementera **Abstract Factory‑mönstret**.

Ni ska:
* Skapa ett factory‑interface, t.ex. `driver::factory::Interface`.
* Låta fabriken skapa drivers via **`std::unique_ptr`**.
* Skapa två konkreta fabriker:
  * `driver::factory::Esp32s3`
  * `driver::factory::Stub`

Systemlogiken ska arbeta mot interfaces och ta emot en factory via referens i konstruktorn.  
Ett exempel på ett factory-interface visas i [bilaga A](./appendix/a_factory_example.md).

---

## Interfaces (minimumkrav)

### `driver::adc::Interface`
* Läsa råvärde (t.ex. 0…4095) från valfri pin.
* Läsa inspänning i Volt (V) från valfri pin.
* Status: Initierad.

### `driver::factory::Interface`
* Skapa instanser av diverse drivers via `std::unique_ptr`:
  * ADC
  * GPIO
  * Serial
  * Tempsensor
  * Timer

**Notering:** Factoryn skapar objekten, men ägandet flyttas till anroparen via `std::unique_ptr`.

**Notering (viktig):** `driver::tempsensor::Tmp36` använder ADC internt. Systemlogiken ska därför normalt prata med `driver::tempsensor::Interface` (inte direkt med ADC).

### `driver::gpio::Interface`
* Sätta/läsa nivå.
* Status: Initierad.

### `driver::serial::Interface`
* Skriv ut text.
* Läsa en rad.
* Status: Initierad.

### `driver::timer::Interface`
* Start/stop.
* Sätta timeout‑period.
* Indikera timeout.

### `driver::tempsensor::Interface`
* Läsa temperatur i grader Celsius.
* Status: Initierad.

---

## Fabriks-API för tempsensor
För att tydliggöra dependency injection ska factoryn kunna skapa TMP36 så här:

* `tempSensor(pin, adc)` tar emot **vilken pin** TMP36 är kopplad till, samt **vilken ADC-instans** som ska användas.

Exempel-signatur (i `driver::factory::Interface`):

```cpp
/**
 * @brief Create temperature sensor instance.
 * 
 * @param[in] pin Pin the temperature sensor instance is connected to.
 * @param[in] adc ADC to use to read the input voltage.
 * 
 * @return Pointer to the temperature sensor instance, or nullptr on failure.
 */
virtual std::unique_ptr<tempsensor::Interface> tempSensor(std::uint8_t pin, 
                                                          adc::Interface& adc) noexcept = 0;
```

---

## Stubbar
**Notering:** Stubbarna ska skrivas i ren C++ utan hårdvaruberoenden (t.ex. FreeRTOS eller ESP-IDF) så att systemlogiken kan köras och testas på en vanlig dator.

Stubbarna ska kunna:
* Hålla internt state.
* Simulera input:
  * Serial: mata in kommandon.
  * Timer: trigga timeout manuellt.
  * ADC: kunna sätta simulerad spänning/råvärde.
  * Temperatursensor: kunna sätta simulerad temperatur.
* Köra logiken utan hårdvara.

---

## Systemlogik
Implementera en klass (t.ex. `system::logic::Logic`) som tar in en driver‑factory som referens:

* `driver::factory::Interface& factory`

Krav:
* Startläge: blink **av**, LED **av**.
* Tolka serial‑kommandon.
* Timeout togglar LED när blink är aktivt.
* `temp` ska läsa temperatur via `driver::tempsensor::Interface` och skriva ut resultatet via serial.
* `run(stop)` kör huvudloopen.
* Klassen ska äga sina drivers via `std::unique_ptr`.
* Fabriken ägs inte av systemklassen. Den används endast för att skapa drivers vid konstruktion.

---

## Versionshantering (Git)
Projektet ska versionshanteras i ett Git-repo (t.ex. GitHub eller GitLab):
* Koden ska ligga i ett privat repo.
* Läraren ska bjudas in som collaborator.
* All utveckling ska ske via Git (inga zip-filer eller liknande).
* Alla studenter ska bidra med egna commits i repot.

---

## Gruppindelning och individuell bedömning
* Grupper om 3–4 studenter sätts samman av läraren.
* Projektet genomförs i grupp, men betyg sätts individuellt.

### Bidragsrapport (obligatorisk)
Varje student ska lämna in en egen bidragsrapport i samband med projektet.  
Syftet är att tydliggöra varje students individuella insats.

Bidragsrapporten ska innehålla:
* Kort beskrivning av egna bidrag (t.ex. drivers, systemlogik, designbeslut).
* Referenser till egna commits och/eller pull requests i Git-repot.

### Bedömning
* Projektets funktion och design bedöms på gruppnivå.
* Individuellt betyg baseras på:
  * Bidragsrapporten.
  * Aktivitet i Git (commits, reviews, etc.).
  * Studentens förmåga att förklara sin kod och design vid redovisning.
  * Detta gäller både för G- och VG-nivå.
  * Bedömning kan justeras individuellt upp eller ner baserat på faktisk insats.

**OBS!** En student som inte kan visa tillräckligt eget bidrag riskerar att inte bli godkänd, 
även om gruppens projekt i sig uppfyller kraven.

---

## Bedömningskriterier

### G (Godkänd)
* Interfaces för **ADC, Factory, GPIO, Serial, Timer och Temperatursensor**.
* En riktig **ESP32‑S3‑driver** per interface.
* En **stub** per interface.
* Systemlogik med factory‑injektion.
* En **abstract factory** som skapar drivers via `std::unique_ptr`.
* Två konkreta fabriker:
  * ESP32‑factory.
  * Stub‑factory.
* Systemet uppfyller beteendet ovan (inkl. `temp` via TMP36).

---

## VG (Väl Godkänd)
Allt för G, plus:

### Nya drivers + stubbar

1. **EEPROM**
   * Spara blinkläge.
   * Ladda inställningar vid uppstart.
   * Kommando: `save`.

2. **Watchdog**
   * Kickas i normal drift.
   * Vid fel (t.ex. ogiltig period) ska watchdog inte kickas → reset.

Ni ska implementera:
* `driver::eeprom::Interface`
* `driver::eeprom::Esp32s3`
* `driver::eeprom::Stub`
* `driver::watchdog::Interface`
* `driver::watchdog::Esp32s3`
* `driver::watchdog::Stub`

### Utökad temperaturfunktionalitet
* `temp on/off` → aktivera/inaktivera periodiska temperaturutskrifter (t.ex. 1 Hz).
* `temp limit 60` → sätt larmgräns i °C.
* Vid övertemperatur:
  * Serial‑varning.
  * LED i alarm‑mönster.

### Bidrag till kursens slutpoäng
* Betyget **G** ger 2 poäng till kurssammanställningen.
* Betyget **VG** ger 4 poäng till kurssammanställningen.

---

## Dokumentationskrav (VG)
* Doxygen på publika headers.
* Tydlig README.
* Testbar design.

---

## Redovisning
Projektet redovisas för lärare under lektionstid:
* Demo i hårdvara.

## Utvärdering
1. Varför använder vi interfaces mellan logik och drivers?
2. Vad är skillnaden mellan en stub och en riktig driver?
3. Varför injicerar vi drivers som referenser i konstruktorn?
4. Vilka delar ska vara hårdvaruberoende – och vilka ska vara hårdvaruoberoende?

---
