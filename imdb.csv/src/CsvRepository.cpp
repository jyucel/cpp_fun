#include "CsvRepository.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <optional>

CsvRepository::CsvRepository(const std::string& dataPath) : _dataPath(dataPath) {}



std::vector<std::string> parseCSVLine(const std::string& line) {
    
    std::vector<std::string> result;
    std::string token;
    bool inQuotes = false;

    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;       // växla in/ur citattecknat läge
        } else if (c == ',' && !inQuotes) {
            result.push_back(token);    // komma utanför citat = fältgräns
            token.clear();
        } else {
            token += c;                 // bygg upp fältet tecken för tecken
        }
    }
    result.push_back(token);           // lägg till sista fältet

    return result;
}

// Tar bort (), [], ", ' från en sträng
static std::string removeChars(const std::string& str) {
    std::string result = str;
    result.erase(std::remove_if(result.begin(), result.end(),
        [](char c){ return c == '(' || c == ')' || c == '"' ||
                           c == '[' || c == ']' || c == '\''; }), result.end());
    return result;
}

// Delar upp en kommaseparerad sträng i en vektor och trimmar mellanslag
static std::vector<std::string> splitByComma(const std::string& str) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ',')) {
        size_t start = token.find_first_not_of(' ');
        size_t end   = token.find_last_not_of(' ');
        if (start != std::string::npos)
            result.push_back(token.substr(start, end - start + 1));
    }
    return result;
}

// Parsar ett år-fält som "(2020)", "(2020– )", "(2020-2022)", "(– )" till två optional<unsigned int>
static std::pair<std::optional<unsigned int>, std::optional<unsigned int>> parseYear(const std::string& str) {
    std::string digits;
    std::vector<unsigned int> years;
    for (char c : str) {
        if (std::isdigit(c)) {
            digits += c;
        } else if (!digits.empty()) {
            if (digits.size() == 4)
                years.push_back(static_cast<unsigned int>(std::stoul(digits)));
            digits.clear();
        }
    }
    if (!digits.empty() && digits.size() == 4)
        years.push_back(static_cast<unsigned int>(std::stoul(digits)));
    std::optional<unsigned int> y1 = years.size() >= 1 ? std::optional<unsigned int>(years[0]) : std::nullopt;
    std::optional<unsigned int> y2 = years.size() >= 2 ? std::optional<unsigned int>(years[1]) : std::nullopt;
    return {y1, y2};
}

// Tar bort "min" från en sträng avd typen "125 min" och konverterar till unsigned int
static unsigned int parseDuration(const std::string& str) {
    std::string durStr = str;
    durStr.erase(std::remove_if(durStr.begin(), durStr.end(),
        [](char c){ return !std::isdigit(c); }), durStr.end());
    return durStr.empty() ? 0u : static_cast<unsigned int>(std::stoul(durStr));
}


std::vector<Movie> CsvRepository::loadMovies() {
    std::vector<Movie> movies;
    std::string title;
    std::optional<unsigned int> year1;
    std::optional<unsigned int> year2;
    std::string certificate;
    unsigned int duration;
    std::vector<std::string> genre; 
    float rating;
    std::string description;
    std::vector<std::string> stars;
    int votes;



    std::ifstream file(_dataPath + "IMDB.csv");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open IMDB.csv" << std::endl;
        return movies;
    }

    std::string line;
    std::getline(file, line); // header
    //std::cout << "Header: " << line << std::endl;

    while (std::getline(file, line)) {
        //std::cout << line << std::endl;
        //printparsedcsv(fields);

        auto fields = parseCSVLine(line);

        title = fields[0];                      //Title
        std::tie(year1, year2) = parseYear(fields[1]); // "(2020)", "(2020– )", "(2020-2022)" → optional<int>
        certificate = fields[2];
        duration = parseDuration(fields[3]);    // "58 min" → ta bort allt utom siffror → stoul
        genre = splitByComma(fields[4]);        // "Action, Comedy, Drama" → vector med "Action", "Comedy", "Drama"
        rating = fields[5].empty() ? 0.0f : std::stof(fields[5]); //ignorera tomma ratingfält, annars konvertera till float
        description = fields[6];
        stars = splitByComma(removeChars(fields[7])); // måste dela upp i vector
        votes  = fields[8].empty() ? 0    : std::stoi(removeChars(fields[8])); // måste ta bort kommatecken och citattecken
        // Skapa Movie-objekt och lägg till i vector
        movies.emplace_back(title, year1, year2, certificate, duration, genre, rating, description, stars, votes);
       // movies.back().printDetails(); // debug: visa detaljer för varje film som läses in
        //std::cout << "-----------------------------" << std::endl;
    }
    return movies;

}

void printparsedcsv(const std::vector<std::string>& fields) {
    std::string title = fields[0];
    std::string year = fields[1];
    std::string certificate = fields[2];
    std::string duration = fields[3];
    std::string genreStr = fields[4];
    std::string ratingStr = fields[5];
    std::string description = fields[6];
    std::string starsStr = fields[7];
    std::string votesStr = fields[8];
    std::cout<< "Parsed fields: \n" << "title: " << title << "\n " << "Year: "<< year << "\n " <<"Cerificate: " << certificate << "\n " << "Duration: " << duration
              << "\n" << "Genre: " << genreStr << "\n" << "Rating: " << ratingStr << "\n " << "Description: " << description
              << "\n" << "Stars: " << starsStr << "\n" << "Votes: " << votesStr << std::endl;
              std::cout<< "-----------------------------" << std::endl;
}
