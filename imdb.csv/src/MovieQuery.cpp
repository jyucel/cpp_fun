#include "MovieQuery.hpp"
#include <algorithm>

MovieQuery::MovieQuery(const std::vector<Movie>& movies) : _movies(movies) {}

// Lägger till ett filter som kräver att filmen har en viss genre
MovieQuery& MovieQuery::whereGenre(const std::string& genre) {
    _filters.push_back([genre](const Movie& m) {
        const auto& genres = m.getGenre();
        return std::find(genres.begin(), genres.end(), genre) != genres.end();
    });
    return *this;
}

// Lägger till ett filter som kräver att filmen har rating strikt över angivet värde
MovieQuery& MovieQuery::whereRatingAbove(float rating) {
    _filters.push_back([rating](const Movie& m) {
        return m.getRating() > rating;
    });
    return *this;
}

// Lägger till ett filter som kräver att year1 >= year
MovieQuery& MovieQuery::whereYearFrom(unsigned int year) {
    _filters.push_back([year](const Movie& m) {
        return m.getYear1().has_value() && m.getYear1().value() >= year;
    });
    return *this;
}

// Lägger till ett filter som kräver att year1 <= year
MovieQuery& MovieQuery::whereYearTo(unsigned int year) {
    _filters.push_back([year](const Movie& m) {
        return m.getYear1().has_value() && m.getYear1().value() <= year;
    });
    return *this;
}

// Sök skådespelare som innehåller söksträngen (t.ex. "Skarsgård" matchar både "Bill Skarsgård" och "Alexander Skarsgård")
MovieQuery& MovieQuery::whereStar(const std::string& star) {
    _filters.push_back([star](const Movie& m) {
        const auto& stars = m.getStars();
        return std::any_of(stars.begin(), stars.end(), [&](const std::string& s) {
            return s.find(star) != std::string::npos;
        });
    });
    return *this;
}

// Lägger till ett filter som kräver att titeln innehåller söksträngen
MovieQuery& MovieQuery::whereTitle(const std::string& title) {
    _filters.push_back([title](const Movie& m) {
        return m.getTitle().find(title) != std::string::npos;
    });
    return *this;
}

// Returnerar index för alla filmer som matchar samtliga filter
std::vector<size_t> MovieQuery::get() const {
    std::vector<size_t> indices;
    for (size_t i = 0; i < _movies.size(); ++i) {
        bool match = true;
        for (const auto& f : _filters)
            if (!f(_movies[i])) { match = false; break; }
        if (match) indices.push_back(i);
    }
    return indices;
}
