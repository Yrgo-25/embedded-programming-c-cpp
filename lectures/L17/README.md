# L17 - Flertrådning och synkroniseringsmekanismer, arbete med **P03**

## Dagordning
* Introduktion till flertrådning i C++ (`std::thread`).
* Data races och varför de är farliga.
* Kritiska sektioner med `std::mutex` och `std::lock_guard`.
* Skillnaden mellan `std::mutex` och `std::atomic`.
* Fortsatt arbete med **P03**.

## Mål med lektionen
* Förklara vad en tråd är och hur den skapas med `std::thread`.
* Förklara vad en data race är och varför det leder till *undefined behavior*.
* Använda `std::mutex` och `std::lock_guard` korrekt.
* Förklara när `std::atomic` räcker och när `std::mutex` krävs.

## Förutsättningar
* God kunskap i C++ (klasser, referenser, RAII).
* Grundläggande förståelse för programflöde och livscykel.

## Instruktioner

### Förberedelse
* Läs [bilaga B](./appendix/b_threads_mutex_atomic.md), som behandlar just flertrådning och synkroniseringsmekanismer i C++.

### Repetition
* Genomför följande [repetitionsuppgift](./appendix/a_repetition.md).

### Under lektionen
* Fundera på om flertrådning kan vara lämpligt i **P03**:
    * Identifiera delar i ***P03** där delad data förekommer.
    * Fundera på hur dessa skulle skyddas i en flertrådad miljö.
* För ytterligare information, se följande [video tutorial](https://www.youtube.com/watch?v=jaI_mufDQ2I), som behandlar mutex i C och C++. 

## Utvärdering
* Vad är skillnaden mellan `mutex` och `atomic`?
* Vad är ett data race?

## Nästa lektion
* Genomförande av övningstenta.
* Fortsatt arbete med **P03**.

---
