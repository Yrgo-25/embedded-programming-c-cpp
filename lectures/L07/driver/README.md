# Demonstration av ett interface
Demonstration av ett interface `driver::led::Interface`, som används för att enkelt kunna använda
olika typer av lysdioder, exempelvis lysdioder från olika processorer, via underklasser.

## Filer
* [driver/led/interface.h](./include/driver/led/interface.h) utgör själva interfacet i form av basklassen `driver::led::Interface`.
* [driver/led/atmega328p.h](./include/driver/led/atmega328p.h) utgör en implementation av lysdioder för processorn `ATmega328P` via underklassen `driver::led::Atmega328p`. Motsvarande källkod finns [här](./source/led/atmega328p.cpp).
* [driver/led/esp32s3.h](./include/driver/led/esp32s3.h) utgör en implementation av lysdioder för processorn `ESP32-S3` via underklassen `driver::led::Esp32s3`. Motsvarande källkod finns [här](./source/led/esp32s3.cpp).
* I [main.cpp](./source/main.cpp) skapas lysdioder av vardera processor (via de tidigare nämnda underklasserna) och blinkar med olika frekvens. Detta åstakommes via en delad blink-funktion, som förväntar sig en referens till ett LED-interface (`driver::led::Interface`). Eftersom båda subklasser ärver `driver::led::Interface` fungerar det utmärkt.

## Kompilering samt körning av programmet
Kompilera och kör programmet via följande kommando (i denna katalog):

```bash
make
```

Du kan också enbart bygga programmet utan att köra det efteråt via följande kommando:

```bash
make build
```

Du kan köra programmet utan att kompilera innan via följande kommando:

```bash
make run
```

Du kan också ta bort kompilerade filer via följande kommando:

```bash
make clean
```

---
