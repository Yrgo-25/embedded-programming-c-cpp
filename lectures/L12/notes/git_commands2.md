# Git-kommandon (del II)

## Ändra en commit
Anta att vi gjorde ett litet fel och inte vill skapa en ny commit för det.

Vi kan då göra en ändring på senaste commit via följande kommando:

```bash
git add <ÄNDRADE_FILER>
git commit --amend
```

---

## Pusha ändrat commit
I och med att den nya ändringen inte är ovanpå den gamla i tid måste vi använda flaggan `-f`
(force) för att pusha, annars kommer inte commiten pushas:

```bash
git push -f
```

---

## Synka lokal branch med remote-branch
Innan ni skapar en ny branch, se till att ni är up-to-date med main-branchen.

Först, se till att läsa in ändringar (utan att göra några ändringar).

```bash
git fetch origin main
```

Notera att FETCH_HEAD pekar längst upp på main-branchen i GitHub-repot. Vi synkar så att vi är på
exakt samma ställe lokalt:

```bash
git reset --hard FETCH_HEAD
```

* Vi resettar till `FETCH_HEAD`, vilket är på toppen av remote-repot, som vi hämtade ändringar från innan.
* `--hard` ser till att vi är exakt i synk med remote-repot; eventuella ändringar ni har gjort skrivs över.

---

## Skapa ny branch
Vi skapar en ny branch döpt `feature/timer`:

```bash
git checkout -b feature/timer
```

där
* `checkout` hoppar till `feature/timer`
* `-b` skapar branchen


---

## Ta bort branch
För att ta bort branchen, använd följande kommando:

```bash
git branch -D feature/timer
```

---

## Rebase mot main
Anta att du jobbar på `feature/timer` och någon annan har pushat till `main`.

Då vill vi lägga våra commits ovanpå senaste `main` istället för att skapa en merge commit, vilket
vi gör genom att läsa in senaste/nyaste `main` från remote-repot, för att sedan rebase på den:

```bash
git fetch origin main
git rebase origin/main
```

Detta gör att:
* Dina commits flyttas och läggs ovanpå senaste `main`.
* Historiken blir linjär och ren  .

---

## Hantera konflikter vid rebase
Om det uppstår konflikter på grund av att ni har arbetat i samma fil:
* Lös konflikterna i filerna:
    * Välj vilken version av koden som ska gälla.
    * Detta görs smidigast i Visual Studio Code med Git-extensionen installerad.
* Lägg till ändringarna:

```bash
git add <FIL>
```

* Fortsätt sedan rebase:

```bash
git rebase --continue
```

* Om du vill avbryta rebasen:

```bash
    git rebase --abort
```

---

## Pusha efter rebase
Eftersom historiken skrivits om måste du använda flaggan `f` vid push:

```bash
git push -f
```

---
