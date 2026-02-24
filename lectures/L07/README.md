# L07 - Arv och interfaces (del I)

## Dagordning
* Arv för att skapa klasser som bygger på befintliga klasser. 
* Interfaces (abstrakta klasser) för att definiera en gemensam uppsättning metoder som olika 
klasser kan implementera på olika sätt.

## Mål med lektionen
* Kunna skapa klasser som ärver andra klasser.
* Kunna skapa interfaces.
* Förstå hur flera olika implementationer kan användas via samma interface.

## Förutsättningar
* Grundläggande kännedom om C++ (klasser, constructors, const).

## Instruktioner

### Förberedelse
* Repetera innehållet i [L05](../L05/README.md).
* Läs om arv i [bilaga A](./appendix/a_inheritance.md).
* Läs om interfaces i [bilaga B](./appendix/b_interfaces.md).
* Se gärna ett exempel på implementering av interfaces i C:
    * [driver/gpio/interface.h](./c_interface/include/driver/gpio/interface.h):
        * Innehåller interfacet `gpio_interface_t`, en strukt med en pekare till ett vtable.
    * [driver/gpio/atmega328p.h](./c_interface/include/driver/gpio/atmega328p.h): 
        * Konstruktorfunktion som skapar ett GPIO-interface baserat på en ATmega328p-driver.
    * [driver/gpio/stub.h](./c_interface/include/driver/gpio/stub.h):
        * Konstruktorfunktion som skapar ett GPIO-interface baserat på en stubb-driver.
    * [main.c](./c_interface/main.c):
        * Toggling av en lysdiod ansluten till ATmega328p via en simulerad tryckknapp.

### Under lektionen
* Påbörja följande [övningsuppgifter](./appendix/c_exercises.md), som täcker interfaces för inbyggda system.

## Utvärdering
* Vad innebär arv i C++?
* Vad är en abstrakt klass?
* Varför kan det vara bra att använda ett interface för en GPIO?

## Nästa lektion
* Fortsatt arbete med övningsuppgifterna.
---