# L01 - Struktar i C (del I)

## Dagordning
* Kursöversikt: Vad vi ska bygga under kursen och varför (C → C++ → interfaces → factory → ESP32-drivrutiner).
* Repetition: Vad är en drivrutin? Vad är ett publikt API?
* Struktar i C: Syfte, medlemsdata och hur struktar används i embedded.
* Associerade funktioner till struktar (funktioner som arbetar på struktens data).

---

## Mål med lektionen
* Förstå varför vi grupperar data i struktar i embedded-system.
* Kunna implementera ett publikt API i en header-fil samt en privat implementation i en källkodsfil.
* Kunna skriva och använda associerade funktioner för en strukt.
* Kunna implementera en enkel GPIO-driver i C: initiera, skriva, läsa och eventuellt toggla.

---

## Förutsättningar
* Grundläggande C: pekare, funktioner, header/source, kompilering.
* Grundläggande förståelse för digital I/O (in/ut, pull-up/pull-down, nivåer).

---

## Instruktioner

### Förberedelse
* Läs igenom [bilaga A](./appendix/a_structs.md) om du inte är bekant med struktar i C.
* Se följande [video tutorial](https://youtu.be/T7o8Vj8ZAUc), som behandlar design av en GPIO-driver med struktar.

### Under lektionen
* Vi implementerar en enkel GPIO-driver i helklass..
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md):
    * Ni får tid under lektionen att genomföra övningarna.
    * Lösningarna diskuteras därefter gemensamt i klassen.
* Lektionsanteckningar finns [här](./notes/README.md).

---

## Utvärdering
* Varför används struktar med associerade funktioner i C?

---

## Nästa lektion
* Slutförande av GPIO-drivern i dess nuvarande skick.
* Inkapsling med opaque structs (fördeklaration i header, implementation i motsvarande källkodsfil).

---
