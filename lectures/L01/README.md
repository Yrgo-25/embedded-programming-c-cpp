# L01 - Struktar i C (del I)

## Dagordning
* Kursöversikt: Vad vi ska bygga under kursen och varför (C → C++ → interfaces → factory → ESP32-drivrutiner).
* Repetition: Vad är en drivrutin? Vad är ett publikt API?
* Struktar i C: Syfte, medlemsdata och hur struktar används i embedded.
* Inkapsling i C med opaque structs (fördeklaration i header, implementation i motsvarande källkodsfil).
* Associerade funktioner till struktar.

## Mål med lektionen
* Förstå varför vi grupperar data i struktar i embedded-system.
* Kunna implementera ett publikt API i en header-fil och en privat implementation i en källkodsfil.
* Kunna använda opaque structs för att dölja implementationsdetaljer.
* Kunna skriva och använda associerade funktioner för en strukt.
* Kunna implementera en enkel GPIO-driver i C: skapa, skriva, läsa och eventuellt toggla.

## Förutsättningar
* Grundläggande C: pekare, funktioner, header/source, kompilering.
* Grundläggande förståelse för digital I/O (in/ut, pull-up/pull-down, nivåer).

## Instruktioner

### Förberedelse
* Läs igenom [bilaga A](./appendix/a_structs.md) om du inte är bekant med struktar i C.
* Se följande [video tutorial](https://youtu.be/T7o8Vj8ZAUc), som behandlar design av en GPIO-driver med struktar.

### Under lektionen
* En enkel GPIO-driver implementeras i helklass:
    * Först med implementation av en publik struktur med enstaka associerade funktioner.
    * Därefter en opaque struct med inkapslade attribut.
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md):
    * Ni får tid under lektionen att genomföra övningarna.
    * Lösningarna diskuteras därefter gemensamt i klassen.

## Utvärdering
* Varför används struktar med associerade funktioner i C?
* Vad är fördelen med att använda opaque structs jämförs med att definiera struktar i headerfilerna?

## Nästa lektion
* Mer arbete med struktar - Utökning av GPIO-drivern (callbacks för interrupts).

---
