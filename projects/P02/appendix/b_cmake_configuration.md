# Bilaga B

## Konfiguration med CMake i ESP-IDF

### Katalogstruktur
Anta att följande katalogstruktur används:

```
p02/
├─ main/
│  ├─ CMakeLists.txt
│  ├─ include/
│  │  ├─ driver/
│  │  │  ├─ adc/
│  │  │  │  ├─ esp32s3.h
│  │  │  │  ├─ interface.h
│  │  │  │  └─ stub.h
│  │  │  ├─ factory/
│  │  │  ├─ gpio/
│  │  │  ├─ serial/
│  │  │  ├─ tempsensor/
│  │  │  └─ timer/
│  │  └─ system/
│  │     └─ logic/
│  │        └─ logic.h
│  ├─ source/
│  │  ├─ driver/
│  │  │  ├─ adc/
│  │  │  │  └─ esp32s3.cpp
│  │  │  ├─ factory/
│  │  │  │  └─ esp32s3.cpp
│  │  │  ├─ gpio/
│  │  │  │  └─ esp32s3.cpp
│  │  │  ├─ serial/
│  │  │  │  └─ esp32s3.cpp
│  │  │  ├─ tempsensor/
│  │  │  │  └─ tmp36.cpp
│  │  │  └─ timer/
│  │  │     └─ esp32s3.cpp
│  │  └─ main.cpp
```

---

### Innehåll i `CMakeLists.txt`
För ovanstående katalogstruktur kan `main/CMakeLists.txt` se ut enligt nedan:

```cmake
idf_component_register(
    SRCS
        "source/main.cpp"
        "source/driver/adc/esp32s3.cpp"
        "source/driver/factory/esp32s3.cpp"
        "source/driver/gpio/esp32s3.cpp"
        "source/driver/serial/esp32s3.cpp"
        "source/driver/tempsensor/tmp36.cpp"
        "source/driver/timer/esp32s3.cpp"
    INCLUDE_DIRS
        "include")

```

---

### Förklaring
* `SRCS` anger vilka källkodfiler (`.c` / `.cpp`) som ska kompileras.
* `INCLUDE_DIRS` anger var headerfiler (`.h`) finns.

Observera att:
* Alla .cpp-filer måste listas i `SRCS`, annars kompileras de inte.
* Katalogen `include/` fungerar som rot för `#include`.

Exempel:

```cpp
#include "driver/gpio/interface.h"
#include "system/logic/logic.h"
```

---
