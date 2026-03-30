# L02 - Struktar i C (del II)

## Dagordning
* Slutförande av GPIO-drivern i dess nuvarande skick.
* Inkapsling med opaque struct (fördeklaration i header, implementation i motsvarande källkodsfil).

---

## Mål med lektionen
* Kunna använda opaque structs för att dölja implementationsdetaljer i en drivrutin.

---

## Förutsättningar
* Genomgång av L01.

---

## Instruktioner

### Förberedelse
* Repetera innehållet i [L01](../L01/README.md).

### Repetition
* Genomför repetitionsuppgiften i [bilaga A](./appendix/a_repetition.md).

### Under lektionen
* Vi slutför först GPIO-drivern i dess nuvarande form.
* Vi modifierar sedan GPIO-drivern så att den använder en opaque struct där attributen är inkapslade.
* Lektionsanteckningar finns [här](./notes/README.md).

---

## Utvärdering
* Vad är fördelen med att använda opaque structs jämfört med att definiera struktar i headerfiler?
* Varför fungerar det att enbart fördeklara opaque structs i headerfiler när enbart struktpekare används?

---

## Nästa lektion
* Utökning av GPIO-drivern: Callbacks för interrupts.
* Introduktion till **P01**.

---
