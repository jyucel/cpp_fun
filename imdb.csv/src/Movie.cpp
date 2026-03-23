#include "Movie.hpp"
#include <iostream>
#include <vector>
#include <optional>

Movie::Movie(const std::string& title, std::optional<unsigned int> year1, std::optional<unsigned int> year2, const std::string& certificate, unsigned int duration,
             const std::vector<std::string>& genre, float rating, const std::string& description,
             const std::vector<std::string>& stars, int votes)
    : _title(title), _year1(year1), _year2(year2), _certificate(certificate), _duration(duration),
      _genre(genre), _rating(rating), _description(description), _stars(stars), _votes(votes) {}


      //Print details of the movie in a crappy format
    void Movie::printDetails() const {
        std::cout << "Title: " << Movie::getTitle() << std::endl;
        std::cout << "Year: " << (getYear1().has_value() ? std::to_string(getYear1().value()) : "?");
        if (getYear2().has_value()) std::cout << "-" << getYear2().value();
        std::cout << std::endl;
        std::cout << "Certificate: " << Movie::getCertificate() << std::endl;
        std::cout << "Duration: " << Movie::getDuration() << std::endl;
        std::cout << "Genre: ";
        const auto& genres = Movie::getGenre();
        for (size_t i = 0; i < genres.size(); ++i) {
            std::cout << genres[i];
            if (i + 1 < genres.size()) std::cout << ", ";
        }
        std::cout << std::endl;
        std::cout << "Rating: " << Movie::getRating() << std::endl;
        std::cout << "Description: " << Movie::getDescription() << std::endl;
        std::cout << "Stars: ";
        for (const auto& star : Movie::getStars()) {
            std::cout << star << ", ";
        }
        std::cout << std::endl;
        std::cout << "Votes: " << Movie::getVotes() << std::endl;
}




const std::string& Movie::getTitle() const {
    return _title;
}
std::optional<unsigned int> Movie::getYear1() const {
    return _year1;
}
std::optional<unsigned int> Movie::getYear2() const {
    return _year2;
}
const std::string& Movie::getCertificate() const {
    return _certificate;
}

unsigned int Movie::getDuration() const {
    return _duration;
}
//genre in csv file is a comma separated list "Action, Comedy, Drama"
const std::vector<std::string>& Movie::getGenre() const {
    return _genre;
}
float Movie::getRating() const {
    return _rating;      
}
const std::string& Movie::getDescription() const {
    return _description;
}
const std::vector<std::string>& Movie::getStars() const {
    return _stars;
}
int Movie::getVotes() const {
    return _votes;
}   
