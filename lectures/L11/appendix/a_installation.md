# Bilaga A

## Installation av ESP‑IDF i Windows

### Ladda ner installationsprogrammet
**1.** Gå till [Espressifs nedladdningssida](https://dl.espressif.com/dl/eim/).  
**2.** Ladda ner [eim-gui-windows-x64.exe](https://dl.espressif.com/github_assets/espressif/idf-im-ui/releases/download/v0.8.0/eim-gui-windows-x64.exe).

---

### Kör installationsprogrammet
**1.** Starta installationsfilen, välj `Start Installtion->Easy Installation`, vilket kommer installera ESP-IDF med standardinställningar.  
**2.** Välj default settings med installationsmapp: `C:\esp`.    
**3.** Installera både:
* ESP‑IDF.
* Verktyg (toolchain, Python, Git m.m.).

När installationen är klar skapas en genväg:

```
ESP‑IDF Command Prompt
```

---

### Starta ESP‑IDF‑miljön
**1.** Öppna: **ESP‑IDF Command Prompt**.
**2.** Ett terminalfönster öppnas med rätt miljövariabler.

Alla ESP32-specifika kommandon ska köras i detta fönster.

Om du inte hittar genvägen, skriv följande kommando i Powershell (här antas att version `ESP-IDF v6.0` är installerat):

```bash
. "C:\Espressif\tools\Microsoft.v6.0.PowerShell_profile.ps1"
```

Om du inte har rättigheter att köra skript, öppna Windows Powershell som administratör och mata
in följande kommando:

```bash
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

Klicka `Y` för `YES`.

---

### Skapa ett första projekt
Skriv följande i ESP‑IDF‑terminalen:

```bash
cd ~
idf.py create-project blink
cd blink
idf.py set-target esp32s3
```
---

### Bygg projektet

```bash
idf.py set-target esp32s3
idf.py build
```

Om allt fungerar ska projektet kompileras utan fel.

## Anslut och flasha kortet
**1.** Anslut Arduino Nano ESP32‑S3 via USB.  
**2.** Kontrollera COM‑port i Enhetshanteraren. Som exempel, om kortet visas som "USB Serial Device (COM4)" ska du använda `COM4`.  
**3.** Flasha projektet:  

```bash
idf.py -p COMx flash monitor
```

Byt ut `COMx` mot rätt port, exempelvis `COM3`.

Om allt fungerar ska du se utskrifter i terminalen.

Stäng terminalen:

```
Ctrl + ]
```

---