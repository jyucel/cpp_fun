// IMDB-anpassad MenuHandler
#include "MenuHandler.hpp"
#include "MenuUtils.hpp"
#include "MovieQuery.hpp"
#include <iostream>

MenuHandler::MenuHandler(std::vector<Movie>& movies, CsvRepository& repo)
    : _running(true), _movies(movies), _repo(repo) {}

int MenuHandler::showMainMenu() {
    MenuUtils::drawMenu(
        "IMDB Movie Database",
        "Välj ett alternativ:",
        {
            "1. Visa alla filmer",
            "2. Sök på skådespelare",
            "3. Sök på titel",
            "4. Avsluta"
        }
    );
    return MenuUtils::readInt();
}

void MenuHandler::handleShowAll() {
    for (const auto& movie : _movies) {
        movie.printDetails();
        std::cout << "-----------------------------" << std::endl;
    }
    std::cout << "Tryck enter för att återgå till menyn...";
    std::cin.get();
}

void MenuHandler::handleSearchByActor() {
    std::cout << "Ange skådespelarnamn: ";
    std::string actor = MenuUtils::readLine();
    auto result = MovieQuery(_movies).whereStar(actor).get();
    if (result.empty()) {
        std::cout << "Inga filmer hittades med " << actor << std::endl;
    } else {
        for (size_t idx : result) {
            _movies[idx].printDetails();
            std::cout << "-----------------------------" << std::endl;
        }
    }
    std::cout << "Tryck enter för att återgå till menyn...";
    std::cin.get();
}

void MenuHandler::handleSearchByTitle() {
    std::cout << "Ange titel eller del av titel: ";
    std::string title = MenuUtils::readLine();
    auto result = MovieQuery(_movies).whereTitle(title).get();
    if (result.empty()) {
        std::cout << "Inga filmer hittades med \"" << title << "\"" << std::endl;
    } else {
        for (size_t idx : result) {
            _movies[idx].printDetails();
            std::cout << "-----------------------------" << std::endl;
        }
    }
    std::cout << "Tryck enter för att återgå till menyn...";
    std::cin.get();
}

void MenuHandler::run() {
    while (_running) {
        int val = showMainMenu();
        switch (val) {
            case 1: handleShowAll(); break;
            case 2: handleSearchByActor(); break;
            case 3: handleSearchByTitle(); break;
            case 4: _running = false; break;
            default: std::cout << "Ogiltigt val!\n";
        }
    }
}