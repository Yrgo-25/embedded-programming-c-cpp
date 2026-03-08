# Bilaga A

## Kompilering och körning av kod i Linux
Om du använder en Linux-miljö kan du använda *make* för att kompilera och köra din kod.  
Nedan följer en beskrivning av hur du installerar WSL för att kunna bygga och köra kod i en Linux-miljö på en Windows-dator.

---

## Installation
WSL *(Windows Subsystem for Linux)* gör det möjligt att köra en Linux-distribution i en terminalmiljö direkt i Windows, utan att använda en virtuell maskin.  
I denna kurs kommer vi använda Linux-distributionen **Ubuntu**.

WSL används i denna kurs för att:
* Bygga, lagra och köra kodexempel.
* Kompilera och köra C- och C++-program.
* Arbeta i en Linux-miljö med standardutvecklingsverktyg såsom `gcc`, `g++`, `make` och `cmake`.

---

### Installera WSL
1.  Öppna **Windows PowerShell** som administratör.
2.  Kör följande kommandon:

``` bash
wsl --install
wsl --set-default-version 2
```

Alternativt kan Ubuntu installeras direkt med:

``` bash
wsl --install -d Ubuntu
```

**OBS!** Om systemet begär en omstart, starta om datorn innan du fortsätter.

Verifiera installationen med:

``` bash
wsl --status
```

Kontrollera att **Default Version: 2** visas.

---

### Installera Ubuntu
Om Ubuntu inte installerades via PowerShell:
* 1. Öppna **Microsoft Store**. 
* 2. Sök efter `Ubuntu`. 
* 3. Välj den senaste LTS-versionen (för närvarande `Ubuntu 24.04 LTS`). 
* 4. Klicka på **Installera**.

---

### Starta Ubuntu
1.  Skriv `Ubuntu` i Windows sökfält.
2.  Starta distributionen.

Vid första uppstarten:
* Välj ett kort och tydligt användarnamn (till exempel ditt förnamn).
* Välj ett lösenord.
* Spara lösenordet på ett säkert ställe.

**OBS!** Lösenordet används endast i Linux-miljön.

---

### Installera grundläggande utvecklingsverktyg
När Ubuntu har startat, uppdatera systemet och installera nödvändiga verktyg:

``` bash
sudo apt -y update
sudo apt -y upgrade
sudo apt -y install build-essential cmake
```

Detta installerar:
* `gcc` och `g++`
* `make` och `cmake`
* Olika utvecklingsbibliotek

Verifiera installationen:

``` bash
gcc --version
g++ --version
```

---

## Skapa ett nytt program
För varje program, skapa en ny katalog, till exempel `example-dir`:

``` bash
mkdir example-dir
cd example-dir
```

I denna katalog skapar du en fil som heter `Makefile`:

``` bash
touch Makefile
```

---

## Enkel makefile
I denna makefile lägger du till följande innehåll:

``` bash
# Build and run the application as default.
default: build run

# Build the application.
build:
    @g++ main.cpp -o main -Wall -Werror -std=c++17

# Run the application.
run:
    @./main

# Clean the application.
clean:
    @rm -f main
```

**Notera**:
* Indenteringen under målen `build`, `run` och `clean` måste bestå av **tabbar**, inte mellanslag.
* Denna makefile bygger en körbar fil med namnet `main`.
* Angående kompilatorflaggorna:
    * `-Wall` aktiverar de flesta kompilatorvarningar.
    * `-Werror` omvandlar varningar till fel, vilket hjälper till att förhindra subtila buggar.
    * `-std=c++17` anger att C++-versionen är C++17.
* För att se kompilationskommandona som körs av make, ta bort prefixet `@`.

---

## Enkel makefile med parametrar
Det är också möjligt att använda parametrar för att göra makefilen lättare att underhålla, särskilt när antalet källfiler ökar:

``` bash
# Target application.
TARGET := main

# C++ compiler.
CXX_COMPILER := g++

# C++ compiler flags.
CXX_FLAGS := -Wall -Werror -std=c++17

# Source files.
SOURCE_FILES := main.cpp

# Build and run the application as default.
default: build run

# Build the application.
build:
    @$(CXX_COMPILER) $(SOURCE_FILES) -o $(TARGET) $(CXX_FLAGS)

# Run the application.
run:
    @./$(TARGET)

# Clean the application.
clean:
    @rm -f $(TARGET)
```

---

## Tips
För att kompileringen ska fungera korrekt:
* Placera alla header- och källkodsfiler (`.h`- och `.cpp`-filer) direkt i denna katalog.
* Se till att inkludera alla källkodsfiler (`.cpp`-filer) mellan `g++` och `-o` i ditt build-target.
* För enkla exempel kan alla källkodsfiler placeras i samma katalog.

### Struktur för embedded-projekt
I större embedded-projekt organiseras källfiler vanligtvis i separata kataloger såsom:

``` text
Makefile
include/
    driver/
        gpio.h
source/
    driver/
        gpio.cpp
    main.cpp
```

Makefilen kan då utökas för att stödja dessa kataloger:
* Katalogen `include` inkluderas via kompilatorflaggorna.
* Källkodsfilerna listas en per rad med radfortsättning (`\`).

``` bash
# Target application.
TARGET := main

# C++ compiler.
CXX_COMPILER := g++

# C++ compiler flags.
CXX_FLAGS := -Wall -Werror -std=c++17 -Iinclude

# Source files.
SOURCE_FILES := source/driver/gpio.cpp \
                source/main.cpp \

# Build and run the application as default.
default: build run

# Build the application.
build:
    @$(CXX_COMPILER) $(SOURCE_FILES) -o $(TARGET) $(CXX_FLAGS)

# Run the application.
run:
    @./$(TARGET)

# Clean the application.
clean:
    @rm -f $(TARGET)
```

---

## Kompilera ett testprogram
Du kan kompilera och köra ditt program med följande kommando:

``` bash
make
```

Du kan också bygga programmet utan att köra det:

``` bash
make build
```

Du kan köra programmet utan att först kompilera:

``` bash
make run
```

Du kan ta bort kompilerade filer:

``` bash
make clean
```

---

Skapa och kör följande testprogram i en fil som heter `main.cpp`:

``` cpp
#include <cstdio>

int main()
{
    std::printf("Hello to C++!\n");
    return 0;
}
```

Kör programmet med:

``` bash
make
```

Förväntad utdata:

``` text
Hello to C++!
```

---

## Anteckningar
* Vi börjar med att inkludera standardheadern `<cstdio>`, som är
    C++-versionen av C-headern `<stdio.h>` och innehåller funktionalitet
    för in- och utdataoperationer.
* Den mesta funktionaliteten i C++-standardbiblioteket finns i
    namnrymden `std` (*standard*).
* En namnrymd fungerar ungefär som en katalog i ett filsystem och
    hjälper till att förhindra namnkollisioner.
* När man använder något från standardbiblioteket prefixar man det
    därför med `std::`, till exempel:
    * `std::uint8_t`
    * `std::size_t`
    * `std::vector`
* `std::printf` är C++-versionen av C-funktionen `printf` och används
    för att skriva ut text i terminalen.

---