#pragma once
#include <functional>
#include <string>
#include <vector>
#include "Movie.hpp"

class MovieQuery {
public:
    MovieQuery(const std::vector<Movie>& movies);

    MovieQuery& whereGenre(const std::string& genre);
    MovieQuery& whereRatingAbove(float rating);
    MovieQuery& whereYearFrom(unsigned int year);
    MovieQuery& whereYearTo(unsigned int year);
    MovieQuery& whereStar(const std::string& star);
    MovieQuery& whereTitle(const std::string& title);

    std::vector<size_t> get() const;

private:
    const std::vector<Movie>& _movies;
    std::vector<std::function<bool(const Movie&)>> _filters;
};
