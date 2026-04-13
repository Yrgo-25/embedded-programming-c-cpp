# Kursinfo

## Lärare
Erik Pihl ([erik.axel.pihl@gmail.com](mailto:erik.axel.pihl@gmail.com))

---

## Kursplan – Hårdvarunära programmering C/C++

| Vecka | Datum | Föreläsning | Ämne |
|-------|-------|-------------|------|
| 13 | 25/3 | L01 | Struktar i C – GPIO-driver |
| 14 | 30/3 | L02 | Struktar i C – opaque structs |
| 14 | 1/4 | L03 | Struktar i C – callbacks för interrupts, introduktion till **P01** |
| 15 | 7/4 | L04 | Introduktion till C++, arbete med **P01** |
| 15 | 9/4 | L05 | Klasser i C++ (del I), arbete med **P01** |
| 15 | 10/4 | L06 | Klasser i C++ (del II), arbete med **P01** |
| 16 | 13/4 | L07 | Arv och interfaces i C++ (del I), arbete med **P01** |
| 16 | 15/4 | L08 | Arv och interfaces i C++ (del II), arbete med **P01** |
| 16 | 16/4 | L09 | Arv och interfaces i C++ (del III), arbete med **P01** |
| 16 | 17/4 | L10 | Arv och interfaces i C++ (del IV), arbete med **P01** |
| 17 | 20/4 | L11 | Konfiguration av ESP32 och utvecklingsmiljö |
| 17 | 21/4 | L12 | Introduktion till **P02** - Driverbibliotek för ESP32 |
| 17 | 22/4 | L13 | Factory-mönster, arbete med **P02** |
| 17 | 23/4 | L14 | Factory-mönster med smarta pekare, arbete med **P02** |
| 18 | 27/4 | L15 | Templates, type traits och constraints, arbete med **P02** |
| 18 | 29/4 | L16 | Klasstemplates, arbete med **P02** |
| 18 | 30/4 | L17 | Flertrådning och synkroniseringsmekanismer, arbete med **P02** |
| 18 | 30/4 | L18 | Övningstentamen, arbete med **P02** |
| 19 | 5/5 | L19 | Slutförande och redovisning av **P02** |
| 20 | 7/5 | L20 | Tentamen - C++-teori |

---

## Examination och betygsnivåer

### Upplägg
* Två projekt (**P01** - **P02**).
* En skriftlig tentamen.

### Poängfördelning
* **P01** ger upp till 2 poäng (**G** = 1p, **VG** = 2p).
* **P02** ger upp till 4 poäng (**G** = 2p, **VG** = 4p).
* Den skriftliga tentamen ger upp till 4 poäng (**G** = 2p, **VG** = 4p).

### Betygsnivåer
Totalt 10 poäng:
* 5 <= **G** < 7 poäng
* 7 <= **VG** <= 10 poäng

För godkänt ska samtliga examinerande moment vara godkända.

---

## Kursmaterial

### Litteratur
* Utgörs av textdokument samt video tutorials, som bifogad och/eller länkas till i samband med lektionerna de behandlas.

### Hårdvara
Följande hårdvara, som finns på skolan, kommer att användas under kursen:
* **Arduino Uno**: 
    * För **P01** - **P02** samt implementering av drivers under den första delen av kursen.
    * Använd era kort från tidigare kurser.
* **ESP32-S3**: 
    * För **P02** samt senare kurser. 
    * Finns tillgängliga på skolan.

### Mjukvara
* [Visual Studio Code](https://code.visualstudio.com/download):
    * Primär editor i denna och framtida kurser.
* [Microchip Studio](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio#Downloads):
    * För kompilering och flashning av **Arduino Uno**.
    * Används enbart för kompilering och flashning, i övrigt används Visual Studio Code.
* [Ubuntu](../lectures/L04/appendix/a_compilation.md):
    * För mellanstora kodexempel i terminalmiljö.
* [ESP‑IDF](../lectures/L11/appendix/a_installation.md):
    * För kompilering och flashning av **ESP32-S3**.
* [Online C compiler](https://www.onlinegdb.com/online_c_compiler):
    * För kompilering och körning av mindre program i terminalmiljö.
    * För att kompilera och köra C++-kod, byt språk till **C++17** i det övre högra hörnet. 

---
