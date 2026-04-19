# L16 - Klasstemplates, arbete med **P02**

## Dagordning
* Klasstemplates - implementation och analys av en vektorklass.
* Fortsatt arbete med **P02**.

---

## Mål med lektionen
* Förklara hur klasstemplates instansieras vid kompilering.
* Förstå hur olika template-instanser genererar separat kod.
* Förstå hur `static_assert()` kan användas för att begränsa klasstemplates vid kompilering.
* Resonera kring när templates är lämpliga i embedded-system.

---

## Förutsättningar
* Grundläggande kunskap om klasser och funktioner i C++.
* Grundläggande kunskap om templates (L15).
* Grundläggande förståelse för kompilering och länkning.

---

## Instruktioner

### Förberedelse
* Läs [bilaga A](appendix/a_class_templates.md) för information om klasstemplates, inklusive exempelimplementation av `container::Vector<T>`. Fundera på om detta är något ni kan använda i era drivers.
* För ytterligare information, se följande [video tutorial](https://www.youtube.com/watch?v=GZEdFPA9IRw), som behandlar enkla klasstemplates utan type traits.

### Under lektionen
* Följ genomgången av klasstemplates.
* Arbeta vidare med **P02**.
* Fundera på:
    * Kan någon driver göras generisk, dvs. kan någon klass implementeras som ett klasstemplate?
    * Finns parametrar som kan bli template-argument?

---

## Utvärdering
* Varför måste template-implementation ligga i headerfiler?
* Nämn en fördel och en nackdel med templates i embedded-system.

---

## Nästa lektion
* Flertrådning och synkroniseringsmekanismer.
* Fortsatt arbete med **P02**.

---
