# L14 - Factory-mönster med smarta pekare, arbete med **P03**

## Dagordning
* Problem med manuell minneshantering.
* Introduktion till smarta pekare (`std::unique_ptr`).
* Factory som returnerar `std::unique_ptr`.
* Påbörja implementation av factory i **P03**.
* Fortsatt arbete med projektet.

## Mål med lektionen
* Förstå varför smarta pekare används i stället för råa pekare.
* Förstå principen bakom `std::unique_ptr`.
* Kunna implementera en factory som returnerar `std::unique_ptr`.
* Kunna låta systemlogiken äga drivers via `std::unique_ptr`.

## Förutsättningar
* Genomgång av L13 (factory med råa pekare).
* Grundläggande kunskap om klasser och interfaces i C++ (L05–L08).

## Instruktioner

### Förberedelse
* Repetera L13.
* Läs igenom [bilaga B](./appendix/b_factory_smart_pointers.md) för information om factories 
med smarta pekare.
* Läs igenom factory-delen i [P03](../../projects/P03/README.md).

### Repetition
* Genomför följande [repetitionsuppgift](./appendix/a_repetition.md).

### Under lektionen
* Följ genomgången av smarta pekare och factory-mönstret.
* Påbörja implementation av factory i projektet.
* Koppla systemlogiken till factoryn.
* Ställ frågor vid behov.
* Arbeta vidare med **P03**.

## Delmål under projektarbetet
* `driver::factory::Interface` implementerad med `std::unique_ptr`.
* Stub-factory implementerad.
* Riktig factory påbörjad eller implementerad.
* Systemlogiken skapar drivers via factory.
* Systemet kompilerar i stub-läge.

### Ni ska kunna
* Förklara skillnaden mellan råa pekare och `std::unique_ptr`.
* Förklara varför `unique_ptr` passar bra i en factory.
* Implementera en factory som returnerar `std::unique_ptr`.
* Låta systemlogiken äga sina drivers via smarta pekare.

## Utvärdering
1. Vad är skillnaden mellan råa pekare och `std::unique_ptr`?
2. Vad händer när en `unique_ptr` går ur scope?
3. Varför är `unique_ptr` lämplig i factory-mönstret?
4. Vad händer om man försöker kopiera en `unique_ptr`?

## Nästa lektion
* Templates, type traits och constraints.
* Fortsatt arbete med **P03**.

---
