
# Bilaga A

## Repetitionsuppgift
Skriv projektet med en [onlinekompilator](https://www.onlinegdb.com/online_c_compiler).

### Uppgiftsbeskrivning
Skriv en enkel strukt `button` med:
* Alias `button_t`.
* Två attribut:
    * `pin`: Lagrar pin-numret.
    * `state`: Tryckknappens insignal i form av ett bool-värde.
* Tre associerade funktioner:
    * `button_pin()`: Returnerar tryckknappens pin-nummer.
    * `button_read()`: Returnerar tryckknappens insignal.
    * `button_set_pressed(bool)`: Simulerar knapptryckning (`true` indikerar nedtryckning).

---

### Testprogram
Pin-nummer samt tryckknappens tillstånd kan skrivas ut med nedanstående funktion `button_print()`:

```c
void button_print(const button_t* self)
{
    if (NULL == self) { return; }
    printf("Button pin: %u\n", self->pin);
    printf("Button state: %s\n", self->state ? "pressed" : "released");
}
```

I funktionen `main`:
* Skapa en knapp ansluten till pin `13`. Sätt `state` till `false` vid start.
* Skriv ut knappinformation genom att anropa `button_print()`.
* Sätt tryckknappen till nedtryckt genom att anropa `button_set_pressed()`.
* Skriv sedan ut knappinformation igen genom att anropa `button_print()` än en gång.

---
