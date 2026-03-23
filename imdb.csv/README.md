### Datakälla
Filmen IMDB.csv kommer från Kaggle-datasetet:
https://www.kaggle.com/datasets/payamamanat/imbd-dataset

# IMDB Movie Database (C++)


## Beskrivning
Detta projekt är en enkel IMDB-liknande filmdatabas i C++ där du kan:
- Ladda in filmer från en CSV-fil
- Söka på titel eller skådespelare
- Visa alla filmer
- Navigera via en interaktiv meny i terminalen

## Kom igång

### Bygg och kör
1. Klona/flytta projektet till din dator
2. Bygg med CMake:
	```sh
	mkdir build
	cd build
	cmake ..
	cmake --build .
	```
3. Kör programmet:
	```sh
	./lab.imdb.1
	```

### Struktur
- `src/` – Källkod (Movie.cpp, CsvRepository.cpp, MenuHandler.cpp, MenuUtils.cpp, m.fl.)
- `include/` – Headerfiler
- `data/IMDB.csv` – Filmdatabas (CSV-format)
- `CMakeLists.txt` – Byggfil

### Funktioner
- **Visa alla filmer** – Skriv ut alla filmer i databasen
- **Sök på skådespelare** – Hitta filmer där en viss skådespelare medverkar
- **Sök på titel** – Hitta filmer på titel eller del av titel
- **Interaktiv meny** – Navigera med siffror

### Exempel på CSV-rad
```
Cobra Kai,(2018– ),TV-14,30 min,"Action, Comedy, Drama",8.5,"Decades after their 1984 All Valley Karate Tournament bout, a middle-aged Daniel LaRusso and Johnny Lawrence find themselves martial-arts rivals again.","['Ralph Macchio, ', 'William Zabka, ', 'Courtney Henggeler, ', 'Xolo Maridueña']","177,031"
```

### Beroenden
- C++17
- CMake 3.16 eller senare

---

> Tips: Lägg till egna filmer i `data/IMDB.csv` för att testa!

