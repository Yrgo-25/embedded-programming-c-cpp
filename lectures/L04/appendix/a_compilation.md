# Bilaga A

## Kompilering samt exekvering av kod i Linux
Om du använder en Linux-miljö kan du använda *make* för att kompilera och köra koden.
Nedan beskrivs installation av WSL för att kompilera och bygga kod i en Linuxmiljö på en Windowsdator.

### Installation 
WSL *(Windows Subsystem for Linux)* möjliggör att man kan köra en
Linux-distribution i terminalmiljö direkt i Windows, utan att använda en
virtuell maskin.

WSL används i kursen för att:
* Bygga, lagra och köra kodexempel.
* Kompilera och exekvera C- och C++-program.
* Arbeta i en Linux-miljö med standardverktyg som `gcc`, `g++`, `make` och `cmake`.

#### Installation av WSL
1.  Öppna **Windows PowerShell som administratör**.
2.  Kör följande kommandon:

``` bash
wsl --install
wsl --set-default-version 2
```

Alternativt kan Ubuntu installeras direkt med:

``` bash
wsl --install -d Ubuntu
```

**OBS!** Om systemet ber om omstart, starta om datorn innan du fortsätter.

Kontrollera installationen med:

``` bash
wsl --status
```

Verifiera att **Default Version: 2** visas.

------------------------------------------------------------------------

#### Installera Ubuntu
Om Ubuntu inte installerades via kommandoraden:
1.  Öppna **Microsoft Store**.
2.  Sök efter `Ubuntu`.
3.  Välj den senaste LTS-versionen (för närvarande `Ubuntu 24.04 LTS`).
4.  Klicka på **Installera**.

#### Starta Ubuntu
1.  Skriv `Ubuntu` i Windows sökfält.
2.  Starta distributionen.

Vid första uppstarten:
* Ange ett kort och tydligt användarnamn (exempelvis ditt förnamn).
* Ange ett lösenord.
* Spara lösenordet på ett säkert sätt.

**OBS!** Lösenordet används endast i Linux-miljön.

#### Installera grundläggande utvecklingsverktyg
När Ubuntu startats, uppdatera systemet och installera nödvändiga verktyg:

``` bash
sudo apt -y update
sudo apt -y upgrade
sudo apt -y install build-essential cmake
```

Detta installerar:
* `gcc` och `g++`
* `make`
* Diverse standardbibliotek
* `cmake`

Verifiera installationen:

``` bash
gcc --version
g++ --version
```

---

### Skapa ett nytt program

För varje program, skapa en ny katalog, exempelvis *example-dir:*:

```bash
mkdir example-dir
cd example-dir
```

I denna katalog, skapa en fil döpt *makefile:*

```bash
touch makefile
```

---

### Enkel makefil

I denna makefil, lägg till följande innehåll:

```bash
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

**OBS!** Indenteringen under targets `build`, `run` och `clean` måste bestå av tabbar, inte spaces.

---

### Enkel makefil med parametrar

Det går också utmärkt att använda parametrar för att göra makefilen lättare att underhålla,
särskilt när antalet källkodsfiler ökar:

```bash
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

### Tips

För att kompileringen ska fungera:
* Placera samtliga header- och källkodsfiler (.h- samt .cpp-filer) direkt i denna katalog.
* Om projektet växer kan makefilen utökas för att stödja underkataloger.
* Se till att lägga till samtliga källkodsfiler (.cpp-filer) mellan `g++` samt `-o` under vårt build-target.

---

### Kompilering av testprogram
Tack vare denna makefil kan du sedan kompilera och köra ditt program via följande kommando (i denna katalog):

```bash
make
```

Du kan också enbart bygga ditt program utan att köra det efteråt via följande kommando:

```bash
make build
```

Du kan köra ditt program utan att kompilera innan via följande kommando:

```bash
make run
```

Du kan också ta bort kompilerade filer via följande kommando:

```bash
make clean
```

Kompilera och kör följande testprogram i en fil döpt `main.cpp`:

```cpp
#include <iostream>

int main()
{
    std::cout << "Hello to C++!\n";
    std::cout << "We're going to use this language for machine learning!\n";
    return 0;
}
```

Utskriften bör bli:  

```
Hello to C++!
We're going to use this language for machine learning!
```

Några saker att notera:
* Vi börjar med att inkludera standardheadern `iostream`, som innehåller funktioner och operatorer för utskrift.
* Allt som ingår i C++ standardbibliotek är implementerat i en så kallad namnrymd döpt `std`, vilket står för *standard* som i *standard library*. Denna namnrymd fungerar lite som en katalog/mapp och minskar bland annat risken för namnkrockar; det gör då inget att vi har multipla typer med samma namn, så länge de ligger i olika namnrymder, på samma sätt som att vi på vår dator kan ha filer med samma namn förutsatt att de ligger i olika kataloger. 
* När vi använder något från standardbiblioteket, vare sig det är en typ, funktioner eller annat måste vi därmed använda prefixet `std`, följt av separatorn `::`, exempelvis `std::vector`, `std::cout`, `std::list` med mera.
* `std::cout` är en operator för utskrift i terminalen *(cout = character out)*, som måste följas av `<<` innan vi lägger till vad vi vill skriva ut.
* `\n` är ett nyradstecken och medför att potentiell nästa utskrift genomförs på nästa rad.

---
