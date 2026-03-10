# Bilaga B

## Extra övningsuppgifter
Dessa uppgifter är frivilliga och kan göras om du blir klar med de ordinarie övningarna.

Skapa följande katalogstruktur:

```text
Makefile
include/
    driver/
        serial/
            console.h
            interface.h
            stub.h
source/
    driver/
        serial/
            console.cpp
            stub.cpp
    main.cpp
```

---
# Övningsset 1 – Seriellt interface

## Övning 1.1 – Interface för seriell kommunikation
I denna övning ska du designa ett interface `driver::serial::Interface` som representerar en generisk seriell kommunikationsdriver.

### Uppgifter
Designa en klass med namnet `driver::serial::Interface` som representerar en generisk seriell driver.

Alla metoder utom destruktorn ska vara rent virtuella (`= 0`).

---

### a) Destruktor
Lägg till en destruktor som är:
* Virtuell.
* Märkt `noexcept`.
* Implementerad med `= default`.

---

### b) Initialiseringsstatus
Lägg till en rent virtuell metod `isInitialized()` som:
* Indikerar om drivern har initialiserats (`true/false`).
* Inte modifierar objektet (`const`).
* Inte kan kasta undantag.

---

### c) Sända data
Lägg till en rent virtuell metod `write()` som:
* Sänder en byte data.
* Tar en parameter av typen `std::uint8_t`.
* Inte returnerar något värde.
* Inte kan kasta undantag.

---

### d) Ta emot data
Lägg till en rent virtuell metod `read()` som:
* Försöker läsa en byte från den seriella drivern.
* Tar en referens till en variabel där byten ska lagras.
* Returnerar `true` om en byte mottogs, annars `false`.
* Inte kan kasta undantag.

---

# Övningsset 2 – Stub-implementation

## Övning 2.1 – Seriell stub
I denna övning ska du implementera en stub-driver `driver::serial::Stub` för det seriella interfacet.

En stub-driver simulerar hårdvarubeteende och är användbar för testning utan verklig hårdvara.

Klassen ska:
* Ärva från `driver::serial::Interface`.
* Vara märkt `final`.

---

### a) Medlemsvariabler
Lägg till tre privata medlemsvariabler:
* Den första medlemsvariabeln ska:
    * Indikera om drivern är initialiserad.
    * Ha typen `bool`.
    * Heta `myInitialized`.
* Den andra medlemsvariabeln ska:
    * Lagra den senast sända byten.
    * Ha typen `std::uint8_t`.
    * Heta `myLastByte`.
* Den tredje medlemsvariabeln ska:
    * Indikera om en byte finns tillgänglig att läsa.
    * Ha typen `bool`.
    * Heta `myHasData`.

### b) Konstruktor
Lägg till en standardkonstruktor som:
* Sätter drivern som initialiserad.
* Indikerar att ingen data finns tillgänglig att läsa.

### c) Implementera interface-metoder
Implementera alla metoder som krävs av interfacet:
* Metoden `isInitialized()` ska:
    * Returnera värdet som lagras i `myInitialized`.
* Metoden `write()` ska:
    * Inte göra någonting om `myInitialized` är `false`.
    * Annars:
        * Lagra den sända byten i `myLastByte`.
        * Sätta `myHasData` till `true`.
* Metoden `read()` ska:
    * Returnera `false` om `myInitialized` är `false` eller `myHasData` är `false`.
    * Annars:
        * Kopiera den lagrade byten (`myLastByte`) till utargumentet.
        * Sätta `myHasData` till `false`.
        * Returnera `true`.

### d) Metod för simulerad inkommande data
Lägg till en extra publik metod `inject()` som inte är en del av interfacet.

Denna metod ska:
* Simulera inkommande seriell data om `myInitialized` är `true`.
* Ta en byte som indata.
* Lagra byten så att den senare kan läsas.

---

# Övningsset 3 – Singleton-driver

## Övning 3.1 – Konsolbaserad seriell driver
Skapa en riktig driverimplementation `driver::serial::Console` som skriver sända bytes till systemets konsol.

Denna driver ska följa det så kallade singleton-mönstret, dvs. det finns endast en instans av klassen.

Anledningen är att systemet bara har en konsolutgångsenhet.

Denna klass ska:
* Ärva från `driver::serial::Interface`.
* Vara märkt `final`.

### a) Privat konstruktor
Lägg till en privat standardkonstruktor som förhindrar användare från att direkt skapa objekt av klassen.

### b) Publik destruktor
Lägg till en publik destruktor, märk den `noexcept` och `default`.

### c) Förhindra flera instanser
Säkerställ att endast en instans av klassen kan existera.

Ta bort följande funktioner (i den publika delen av klassen):
* Kopieringskonstruktor.
* Flyttkonstruktor.
* Kopieringstilldelningsoperator.
* Flyttilldelningsoperator.

### d) Singleton-åtkomstmetod
Lägg till en statisk publik metod `instance()` som returnerar en referens till den unika instansen av klassen:
* Metoden ska returnera konsoldrivern som en referens till `driver::serial::Interface`
(även om instansen själv är av typen `driver::serial::Console`).
* Metoden får inte kasta undantag.
* I denna metod:
    * Skapa en lokal statisk instans av drivern (`static driver::serial::Console`).
    * Initialisera instansen med standardkonstruktorn.

### e) Implementera interface-metoder
Implementera alla metoder som krävs av interfacet:
* Metoden `isInitialized()` ska:
    * Alltid returnera `true`, eftersom konsoldrivern alltid är tillgänglig.
* Metoden `write()` ska:
    * Skriva ut den givna byten till konsolen med `std::printf()` från `<cstdio>`.
* Metoden `read()` ska:
    * Alltid returnera `false`, eftersom konsolen inte stöder läsning.

---

# Övningsset 4 – Användning av interfacet
## Övning 4.1 – Skicka ett meddelande
**a)** Skapa en funktion `sendMessage()` som skickar ett meddelande via det seriella interfacet.

Funktionen ska:
* Ta en referens till `driver::serial::Interface`.
* Sända strängen `"Transmitting data with a serial driver!\n"`.
* Skicka meddelandet en byte i taget.

**b)** I `main.cpp`, testa stub-drivern:
* Skapa en instans av stub-drivern och skicka den till funktionen `sendMessage()`.
* Verifiera att den sända byten senare kan läsas med metoden `read()`.

**c)** I `main.cpp`, testa konsoldrivern:
* Hämta singleton-instansen av konsoldrivern.
* Skicka den till funktionen `sendMessage()`.
* Verifiera att programmet skriver ut följande:

```text
Transmitting data with a serial driver!
```

---

# Övningsset 5 – Reflektionsfrågor
**a)** Varför tar funktionen `sendMessage()` en referens till `driver::serial::Interface` istället
för en specifik driverklass?  
**b)** Vilka fördelar ger stub-drivern vid testning?  
**c)** Varför kan konsoldrivern rimligen implementeras som en singleton?  
**d)** Vilka nackdelar kan singleton-mönstret ha?  

---
