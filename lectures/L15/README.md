# L15 – Templates, type traits och constraints, arbete med **P03**

## Dagordning
* Templates i inbyggda system – fördelar och nackdelar.
* Type traits och compile-time constraints.
* `static_assert()` och säkra API:er (fel vid kompilering i stället för vid körning).
* Koppling till **P03**: Compile-time vs run-time polymorfism.

## Mål med lektionen
* Förstå hur funktionstemplates fungerar i C++.
* Förstå hur templates genererar kod vid kompilering.
* Kunna använda type traits för att skapa compile-time constraints.
* Förstå varför compile-time-kontroller är särskilt viktiga i inbyggda system.
* Kunna resonera kring när templates är lämpliga – och när de bör undvikas.

## Förutsättningar
* Grundläggande kunskap om klasser och funktioner i C++.
* Förståelse för interfaces och polymorfism.
* Grundläggande förståelse för kompilering och länkning.

## Instruktioner

### Förberedelse
* Läs [bilaga B](./appendix/b_templates.md) för information om templates, type traits och constraints.
* För ytterligare information, se följande [video tutorial](https://www.youtube.com/watch?v=mbRiN_QmjUY), som behandlar enkla funktionstemplates utan type traits.

### Repetition
* Genomför repetitionsuppgiften i [bilaga A](./appendix/a_repetition.md).

### Under lektionen
* Följ genomgången av templates, type traits och constraints. Fundera på om detta är något ni kan använda i era drivers.
* Arbeta vidare med **P03**.

## Utvärdering
1. Vad är skillnaden mellan compile-time och run-time polymorfism?
2. Vad är fördelen med att få ett fel vid kompilering i stället för vid körning?
3. Varför kan templates öka binärstorleken?
4. När bör man använda templates i stället för interfaces?

## Nästa lektion
* Klasstemplates.
* Fortsatt arbete med **P03**.

---
