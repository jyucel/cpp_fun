#pragma once
#include <string>
#include <vector>
#include <optional>


//Ex på hur IMDB.csv filen ser ut ...
//  title,year,certificate,duration,genre (vector),rating,description,stars,votes
// The Crown,(2016– ),TV-MA,58 min,"Biography, Drama, History",8.7,Follows the political rivalries and romance of Queen Elizabeth II's reign and the events that shaped the second half of the twentieth century.,"['Claire Foy, ', 'Olivia Colman, ', 'Imelda Staunton, ', 'Matt Smith']","199,885"


class Movie {
private:
    std::string _title;
    std::optional<unsigned int> _year1;
    std::optional<unsigned int> _year2;
    std::string _certificate;
    unsigned int _duration;
    std::vector<std::string> _genre;
    float _rating;
    std::string _description;
    std::vector<std::string> _stars;
    int _votes;

public:
        Movie(const std::string& title, std::optional<unsigned int> year1, std::optional<unsigned int> year2, const std::string& certificate, unsigned int duration,
            const std::vector<std::string>& genre, float rating, const std::string& description,
            const std::vector<std::string>& stars, int votes);
   
    // Getters
    const std::string& getTitle() const;
    std::optional<unsigned int> getYear1() const;
    std::optional<unsigned int> getYear2() const;
    const std::string& getCertificate() const;
    unsigned int getDuration() const;
    const std::vector<std::string>& getGenre() const;
    float getRating() const;
    const std::string& getDescription() const;
    const std::vector<std::string>& getStars() const;
    int getVotes() const;
    void printDetails() const;
};
