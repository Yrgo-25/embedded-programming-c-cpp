# Bilaga A

## Struktar i C

### Bakgrund
För att hålla god struktur i större program finns ett ökat behov av att lagra relaterad data som en enhet via enskilda datastrukturer i stället för multipla separata variabler. Som exempel kan det vara fördelaktigt att lagra persondata i form av namn, ålder, adress och dylikt för en person som en enhet i stället för separata variabler för varenda attribut. 

Inom programmering utgörs de vanligaste datastrukturerna för lagring av relaterad data som en enhet av så kallade *struktar* och *klasser:* 
* *Struktar* (strukturer) används främst i C (begreppet klass existerar inte överhuvudtaget i C).
* Klasser används nästan exklusivt i programspråk som skapades efter C, såsom C++, Java och Python. 
* Klasser kan ses som en vidareutveckling av struktar, som är relativt simpla. Sett till uppbyggnad är dock dessa datastrukturer väldigt lika.

Språk som innehar stöd för klasser sägs inneha stöd för objektorientering, eller OOP *(Object Oriented Programming.)* C sägs vara ett procedurellt språk, då det bygger på programmering via funktioner. Via struktar kan dock vissa objektorienterade principer efterliknas, vilket kommer demonstreras nedan.

En del objektorienterade språk, exempelvis Java och C# är rent objektorienterade, vilket innebär att klasser måste användas. Andra programspråk, såsom C++ och Python, sägs utgöra multiparadigmspråk, då det finns möjlighet att programmera båda objektorienterat (såsom i Java) samt procedurellt (såsom i C) eller en kombination av dem.

### Vad är en strukt?
Struktar i C fungerar likt arrayer, med skillnaden att datatyperna på medlemmarna i en strukt kan vara olika, medan samtliga element i en array måste vara samma. 

Därmed gäller att ett struktobjekt kan inneha en kombination av text, tal, boolvariabler med mera, exempelvis för att lagra en persons namn (text), ålder (heltal), om personen är singel (sant eller falskt) med mera.

Struktar implementeras via nyckelordet *struct.* Nedan visas en strukt döpt *person*, vars syfte är att lagra persondata:

```c
/**
 * @brief Structure representing personal data.
 */
struct person 
{
    const char* name;       /** The person's name. */
    unsigned age;           /** The person's age. */
    const char* address;    /** The person's address. */
    const char* occupation; /** The person's occupation. */
};
```

Via strukten *person* ovan kan vi lagra namn, ålder, address samt yrke för en given person. Som exempel, för en 42-årig advokat döpt Marie Nilsson som bor på Juristgatan 17 kan vi skapa ett objekt döpt *person1* så som visas nedan. Notera att initieringen är väldigt likt hur vi hade initierat en array i C:

```c
struct person person1 = {"Marie Nilsson", 42U, "Juristgatan 17", "Lawyer"};
```

Notera att vi passerar attributen, såsom "Marie Nilsson" för namnet och 42 för åldern, i samma ordning som de har deklarerats i strukten.

Till skillnad mot arrayer använder vi inte index för att nå innehållet i struktobjektet. I stället använder vi den så kallade punktoperatorn, som utgörs av en punkt mellan objektets namn och attributet vi söker. 

Därmed gäller att om vi vill få tillgång till namnet på *person1* skriver vi *person1.name*, då namnet är lagrat via attributet *name*. På samma sätt gäller att vi skriver *person1.age* för att få tillgång till personens ålder, *person1.address* för att få tillgång till persons adress och så vidare.

Nedan visas hur vi skriver ut persondatan för *person1* i terminalen via anrop av funktionen *print* från standardheadern *stdio.h*:

```c
    struct person person1 = {"Marie Nilsson", 42U, "Juristgatan 17", "Lawyer"};
    printf("Name:\t\t%s\n", person1.name);
    printf("Age:\t\t%u\n", person1.age);
    printf("Address:\t%s\n", person1.address);
    printf("Occupation:\t%s\n", person1.occupation);
```

Utskriften blir:

```
Name:           Marie Nilsson
Age:            42
Address:        Juristgatan 17
Occupation:     Lawyer
```

Kortfattat gällande ovanstående kodsnutt:
* **\t** medför ett tabtecken.
* **\n** medför ett nyradstecken.
* **%s** är formatspecificerare för text.
* **%u** utgör formatspecificerare för osignerade heltal.

Vi kan lägga till fler parametrar vid behov. Som exempel kan vi lägga till en boolvariabel *single*, som indikerar ifall personen i fråga är singel eller inte via booleska värden *true* och *false*. För att göra detta måste vi inkludera standardheadern *stdbool:*

```c
#include <stdbool.h>
```

Därefter kan vi lägga till parametern *single* så som visas nedan:

```c
/**
 * @brief Structure representing personal data.
 */
struct person 
{
    const char* name;       /** The person's name. */
    unsigned age;           /** The person's age. */
    const char* address;    /** The person's address. */
    const char* occupation; /** The person's occupation. */
    bool single;            /** Indicate whether the person is single. */
};
```

Vi kan även lägga till parametrar av egna typer också. Anta att vi vill ha en typ för personens kön, där giltiga värden är man, kvinna eller annat. 

I C hade vi kunnat göra detta med en enumeration döpt *gender,* där enumeratorerna föregås av prefixes GENDER för att undvika namnkrockar i större program. En sådan enumeration hade kunnat se ut så som visas nedan:

```c
/**
 * @brief Enumeration of genders.
 */
enum gender
{
    GENDER_MALE,   /** Male. */
    GENDER_FEMALE, /** Female. */
    GENDER_OTHER,  /** Other gender. */
};
```

Vi lägger till parametern *gender* för personens kön efter åldern, så att den mest basala datan (namn, ålder och kön) hamnar först:

```c
/**
 * @brief Structure representing personal data.
 */
struct person 
{
    const char* name;       /** The person's name. */
    unsigned age;           /** The person's age. */
    enum gender gender;     /** The person's gender. */
    const char* address;    /** The person's address. */
    const char* occupation; /** The person's occupation. */
    bool single;            /** Indicate whether the person is single. */
};
```

Anta att personen Marie Nilsson implementerad via objektet *person1* tidigare är kvinna och singel. I så fall hade associerat objekt *person1* kunnat implementeras så som visas nedan. Notera att vi passerar attributen i samma ordning som de har deklarerats i strukten:

```c
struct person person1 = {"Marie Nilsson", 42U, GENDER_FEMALE,
                         "Juristgatan 17", "Lawyer", true};
```

#### Exempelprogram
Nedan visas ett program, där persondatan lagrad via ovanstående objekt *person1* skrivs ut i terminalen. Vi implementerar en funktion döpt *gender_str* för att erhålla kön i textform.

```c
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Enumeration of genders.
 */
enum gender
{
    GENDER_MALE,   /** Male. */
    GENDER_FEMALE, /** Female. */
    GENDER_OTHER,  /** Other gender. */
};

/**
 * @brief Structure representing personal data.
 */
struct person 
{
    const char* name;       /** The person's name. */
    unsigned age;           /** The person's age. */
    enum gender gender;     /** The person's gender. */
    const char* address;    /** The person's address. */
    const char* occupation; /** The person's occupation. */
    bool single;            /** Indicate whether the person is single. */
};

/**
 * @brief Get the given gender as a string.
 * 
 * @param[in] gender The gender in question.
 * 
 * @return The given gender as a string.
 */
const char* gender_str(const enum gender gender)
{
    switch (gender)
    {
        case GENDER_MALE  : return "Male";
        case GENDER_FEMALE: return "Female";
        default           : return "Other";
    }
}

/**
 * @brief Print personal data in the terminal.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create a structure holding personal data.
    struct person person1 = {"Marie Nilsson", 42U, GENDER_FEMALE,
                             "Juristgatan 17", "Lawyer", true};

    // Print the personal data in the terminal, then terminate the program.
    printf("Name:\t\t%s\n", person1.name);
    printf("Age:\t\t%u\n", person1.age);
    printf("Gender:\t\t%s\n", gender_str(person1.gender));
    printf("Address:\t%s\n", person1.address);
    printf("Occupation:\t%s\n", person1.occupation);
    printf("Single:\t\t%s\n", person1.single ? "Yes" : "No");
    return 0;
}
```

Utskriften blir:

```
Name:           Marie Nilsson
Age:            42
Gender:         Female
Address:        Juristgatan 17
Occupation:     Lawyer
Single:         Yes
```



### Associerade funktioner till struktar
I C är det vanligt att skapa funktioner associerade med en strukt. Dessa funktioner skapas då för att användas med objekt av motsvarande strukt. 

Exempelvis kan vi skapa en funktion döpt *person_print* för att skriva ut persondata i terminalen. Detta kommer både göra programmet enklare att läsa, samtidigt som vi slipper skriva samma kod igen ifall vi vill skriva ut persondata för fler personer.

Vi lägger till prefixet *person* på alla funktioner associerade med strukten *person* av två anledningar:
* **1.** För att tydliggöra att funktionen är associerad med strukten *person.*
* **2.** För att undvika namnkrockar, vilket vi hade fått om en annan C-funktion heter *print.* Därmed ser vi till att samtliga funktioner har undika namn. Som exempel, skapar vi en till struct som heter exempelvis *device* kan vi döpa motsvarande utskriftsfunktion till *device_print.*

Funktionen *person_print* visas nedan:

```c
/**
 * @brief Print personal data in the terminal.
 * 
 * @param[in] self Reference to the person structure holding the data to print.
 */
void person_print(const struct person* self)
{
    // Terminate the function is the pointer is null.
    if (!self) { return; }

    // Print the attributes on separate lines.
    printf("--------------------------------------------------------------------------------\n");
    printf("Name:\t\t%s\n", self->name);
    printf("Age:\t\t%u\n", self->age);
    printf("Gender:\t\t%s\n", gender_str(self->gender));
    printf("Address:\t%s\n", self->address);
    printf("Occupation:\t%s\n", self->occupation);
    printf("Single:\t\t%s\n", self->single ? "Yes" : "No");
    printf("--------------------------------------------------------------------------------\n\n");
}
```

Notera att:
* Vi sätter ingående argument *self* till en pekare, då person-objekt är relativt stort. Därmed föredrar vi *pass-by-reference* (med pekare) i stället för *pass-by-value* (som hade medfört att vi skapat och sedan skickat en kopia av person-objektet vars innehåll vi vill skriva ut). 
* Eftersom vi bara vill läsa persondatan utan att modifiera något sätter vi person-objektet till en konstant via nyckelordet *const.*
* Vi döper ingående argument till *self* för att indikera att detta är själva objektet som den associerade funktionen är till för. Vanliga andra namn är *this* och *me,* men givetvis kan vi döpa detta argument till *person* om vi föredrar det.
* Eftersom ingående argument *self* är en pekare till person-objektet, inte själva objektet i sig, använder vi piloperatorn -> för att enkelt få tillgång till person-attributen. Som exempel, via piloperatorn kan vi få tillgång till personens namn genom att skriva *self->name* i stället för *(*self).name* för att först läsa innehållet som pekaren *self* pekar på *(.self)*, följt av att använda punktoperatorn för att få tillgång till attributen.

När vi anropar funktionen *person_print* måste vi skicka adressen till person-objektet vi vill skriva ut. Detta gör vi med den så kallade adressoperatorn &. Som exempel, för att kalla på *person_print* med adressen till objektet *person1* kan följande instruktion användas:

```c
person_print(&person1);
```

#### Exempelprogram
Vi lägger till funktionen *person_print* i ovanstående program. Vi lägger också till ytterligare en person, en 37-årig gift man, som heter "Sven Andersson", bor på "Kunskapsgatan 4" och jobbar som lärare. Vi skriver sedan ut persondatan för respektive person genom att anropa *person_print.* Programmet visas nedan:

```c
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Enumeration of genders.
 */
enum gender
{
    GENDER_MALE,   /** Male. */
    GENDER_FEMALE, /** Female. */
    GENDER_OTHER,  /** Other gender. */
};

/**
 * @brief Structure representing personal data.
 */
struct person 
{
    const char* name;       /** The person's name. */
    unsigned age;           /** The person's age. */
    enum gender gender;     /** The person's gender. */
    const char* address;    /** The person's address. */
    const char* occupation; /** The person's occupation. */
    bool single;            /** Indicate whether the person is single. */
};

/**
 * @brief Get the given gender as a string.
 * 
 * @param[in] gender The gender in question.
 * 
 * @return The given gender as a string.
 */
const char* gender_str(const enum gender gender)
{
    switch (gender)
    {
        case GENDER_MALE  : return "Male";
        case GENDER_FEMALE: return "Female";
        default           : return "Other";
    }
}

/**
 * @brief Print personal data in the terminal.
 * 
 * @param[in] self Reference to the person structure holding the data to print.
 */
void person_print(const struct person* self)
{
    // Terminate the function is the pointer is null.
    if (!self) { return; }

    // Print the attributes on separate lines.
    printf("--------------------------------------------------------------------------------\n");
    printf("Name:\t\t%s\n", self->name);
    printf("Age:\t\t%u\n", self->age);
    printf("Gender:\t\t%s\n", gender_str(self->gender));
    printf("Address:\t%s\n", self->address);
    printf("Occupation:\t%s\n", self->occupation);
    printf("Single:\t\t%s\n", self->single ? "Yes" : "No");
    printf("--------------------------------------------------------------------------------\n\n");
}

/**
 * @brief Print personal data in the terminal.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create structures holding personal data.
    const struct person person1 = {"Marie Nilsson", 42U, GENDER_FEMALE, "Juristgatan 17", 
                                   "Lawyer", true};
    const struct person person2 = {"Sven Andersson", 37U, GENDER_MALE, "Kunskapsgatan 4", 
                                   "Teacher", false};

    // Print the personal data by invoking `person_print`, then terminate the program.
    person_print(&person1);
    person_print(&person2);
    return 0;
}
```

Utskriften visas nedan:

```
--------------------------------------------------------------------------------
Name:           Marie Nilsson
Age:            42
Gender:         Female
Address:        Juristgatan 17
Occupation:     Lawyer
Single:         Yes
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
Name:           Sven Andersson
Age:            37
Gender:         Male
Address:        Kunskapsgatan 4
Occupation:     Teacher
Single:         No
--------------------------------------------------------------------------------
```

---