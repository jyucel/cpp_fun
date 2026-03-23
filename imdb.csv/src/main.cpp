#include <iostream>
#include <algorithm>
#include "Movie.hpp"
#include "CsvRepository.hpp"
#include "MovieQuery.hpp"
#include "MenuUtils.hpp"
#include "MenuHandler.hpp"


int main() {
    //Lets start ... Ladda filmer från CSV och sortera på rating
    CsvRepository repository("data/");
    std::vector<Movie> movies = repository.loadMovies();

    std::sort(movies.begin(), movies.end(), [](const Movie& a, const Movie& b) {
        return a.getRating() > b.getRating(); // fallande ordning (högst rating först)
    });
    // Menyhantering sker nu via MenuHandler
    // --- IMDB-menyhantering ---
    MenuHandler menuHandler(movies, repository);
    menuHandler.run();

    return 0;
}




