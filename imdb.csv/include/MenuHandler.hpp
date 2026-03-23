
#pragma once
#include "Movie.hpp"
#include "CsvRepository.hpp"
#include <vector>

class MenuHandler {
private:
    bool _running;
    std::vector<Movie>& _movies;
    CsvRepository& _repo;

    int showMainMenu();
    void handleShowAll();
    void handleSearchByActor();
    void handleSearchByTitle();

public:
    MenuHandler(std::vector<Movie>& movies, CsvRepository& repo);
    void run();
};