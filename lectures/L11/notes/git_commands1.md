# Git-kommandon (del I)

## Initiera Git-repo med `main` som huvudbranch

```bash
git init --initial-branch=main
```

---

## Lägg till fil i Git
Vi lägger till filen `README.md`:

```bash
git add README.md
```

---

## Kolla status (tillagda filer osv.)

```bash
git status
```

---

## Commita fil
Vi commitar filen `README.md` med meddelandet `Add root README`:

```bash
git commit -m "Add root README"
```

Om commiten misslyckas (för att Git inte vet vem du är), registrera dina uppgifter:

```bash
git config --global user.name "Ditt Namn"
git config --global user.email "din.email@example.com"
```

Testa sedan att köra om commit-kommandot ovan.

---

## Kolla Git-historiken

```bash
git log
```

Avsluta med `Q`.

---

## Lägg till URL till remote-repo (GitHub)
I detta fall sätter vi vårt GitHub-repo till `origin`. Därefter pushar vi till detta via `origin`.

Vi använder en SSH-länk till repot för att slippa mata in lösenord eller access token vid varje push:

```bash
git remote add origin git@github.com:<ANVÄNDARE>/<REPO>.git
```

Exempel: `git@github.com:erikpihl/cpp_p02.git`

---

## Lägg till SSH-nyckel
Generera en SSH-nyckel:

```bash
ssh-keygen
```

Tryck ENTER → ENTER (standardinställningar, ingen passphrase).

Din publika nyckel sparas i en fil som slutar på .pub, t.ex.

```bash
~/.ssh/id_ed25519.pub
```

Skriv ut filens innehåll i terminalen och kopiera:

```bash
cat ~/.ssh/id_ed25519.pub
```

Lägg till denna nyckel i GitHub (Settings → SSH and GPG keys → New SSH key).

---

## Pusha nya commits till GitHub
**OBS!** `-u` betyder `upstream` och gör att din lokala branch kopplas till `origin/main`.

```bash
git push -u origin main
```

Nästa gång du pushar osv kommer koden automatiskt dirigeras till `origin/main`. Därmed räcker det med följande:

```bash
git push
```

---
