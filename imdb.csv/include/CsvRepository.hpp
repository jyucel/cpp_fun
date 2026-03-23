/* Läs in IMDB data från en CSV-fil */
#pragma once

#include "Movie.hpp"
#include <vector>
#include <string>   

class CsvRepository {
    private:
        std::string _dataPath; // Basväg för CSV-filer (t.ex. "data/")

    public:
        // Skapar ett repository med angiven sökväg till data-katalogen.
        // _dataPath används för att hitta rätt filer, t.ex. "data/movies.csv".
        CsvRepository(const std::string& dataPath);

        // Läs från CSV
        std::vector<Movie> loadMovies();
        
};