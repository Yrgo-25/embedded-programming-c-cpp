# L10 – Anteckningar

## Beskrivning
Implementation av ett seriellt interface samt en stubb-driver.

---

## Filer
* [driver/serial/interface.h](./include/driver/serial/interface.h):
    * Seriellt interface.
* [driver/serial/stub.h](./include/driver/serial/stub.h):
    * Deklaration av en seriell stubb-driver.
* [driver/serial/stub.cpp](./source/serial/stub.cpp):
    * Implementation av den seriella stubb-drivern.
* [main.cpp](./source/main.cpp):
    * Innehåller ett demonstrationsprogram där en seriell stubb-driver initieras och tilldelas simulerad indata. 
    * Drivern passeras till systemlogiken, där seriell data läses och skrivs ut byte för byte.

---
