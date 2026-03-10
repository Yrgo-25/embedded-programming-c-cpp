# Implementering av interfaces i C

Implementering av interfaces samt konkreta typer (riktig driver samt stubb) i C via struktar och vtables.

Detta exempel riktar sig särskilt till C-programmerare och demonstrerar hur interfaces kan implementeras manuellt med struktar och vtables. Syftet är att ge en förståelse för hur 
C++-kompilatorn implementerar virtuella funktioner "under huven".

Implementationen är skriven för kompilering på en ATmega328P i Microchip Studio.

## Filer
* [driver/gpio/interface.h](./include/driver/gpio/interface.h): 
    * Innehåller interfacet `gpio_interface_t`, en strukt med en pekare till en vtable.
* [driver/gpio/atmega328p.h](./include/driver/gpio/atmega328p.h): 
    * Konstruktorfunktion som skapar ett GPIO-interface baserat på en ATmega328p-driver.
    * Motsvarande implementation finns [här](./source/driver/gpio/atmega328p.c).
* [driver/gpio/stub.h](./include/driver/gpio/stub.h): 
    * Konstruktorfunktion som skapar ett GPIO-interface baserat på en stubb-driver.
    * Motsvarande implementation finns [här](./source/driver/gpio/stub.c).
* [main.c](./main.c): 
    * Toggling av en lysdiod ansluten till ATmega328p via en simulerad tryckknapp.

---
