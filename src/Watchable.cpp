/**
 * h file.
 * Class watchable contain description of something a user can watch - movie & episode.
 * Each episode/movie has it's characteristics like name, length, etc.
 */

#include "../include/Watchable.h"
#include <iostream>
#include <utility>
#include "../include/Session.h"
#include "../include/User.h"
using namespace std;

// Constructor
Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), length(length), tags(vector<string>()){
    for (const auto & tag : tags) {
        this->tags.push_back(tag);
    }

}

// Copy constructor
Watchable::Watchable(const Watchable &other) : id(other.getID()), length(other.getLength()), tags(vector<string>()){
    for (const auto & tag : other.tags) {
        this->tags.push_back(tag);
    }
}

// Copy assignment operator
Watchable &Watchable::operator=(const Watchable &other) {
    if (this == &other) {
        return *this;
    }
    this->tags.clear();
    for (const auto & tag : other.tags) {
        this->tags.push_back(tag);
    }
    length = other.getLength();
    return *this;
}

// Destructor
 Watchable:: ~Watchable() {
    tags.clear();
}


// Returns the watchable's ID
long Watchable::getID() const {
    return id;
}

// Returns the watchable's length.
int Watchable::getLength() const {
    return length;
}

// Returns the watchable's tags vector.
std::vector<std::string> Watchable::getTags() const {
    return tags;
}

// To string method for the tags vector.
string Watchable::toStringTags() const {
    string ret = "[";
    for (size_t i = 0; i < getTags().size()-1; ++i) {
        ret += getTags()[i] + ", ";
    }
    ret += getTags()[getTags().size()-1] + "]";
    return ret;
}

// Set a length for a watchable.
void Watchable::setLength(int length) {
    this->length = length;
}
//************************MOVIE**********************************//

// Constructor
Movie::Movie(long id, const std::string& name, int length, const std::vector<std::string> &tags) : Watchable(id, length, tags), name("") {
    for (char i : name) {
        this->name.push_back(i);
    }
}

// Copy constructor
Movie::Movie(const Movie &other): Watchable(other.getID(),other.getLength(),other.getTags()), name("") {
    for (char i : other.getName()) {
        this->name.push_back(i);
    }
}

// Copy assignment operator
Movie &Movie::operator=(const Movie &other) {
    if (this == &other) {
        return *this;
    }
    getTags().clear();
    for (const auto & tag : other.getTags()) {
        this->getTags().push_back(tag);
    }
    setLength(other.getLength());
    name.clear();
    for (char i : other.getName()) {
        this->name.push_back(i);
    }
    return *this;
}

// To string for the movie's name.
string Movie::toString() const {
    return name;
}

// Getter for the next watchable to watch, according to the user recommendation algorithm.
Watchable *Movie::getNextWatchable(Session &sess) const {
    return sess.getActiveUser()->getRecommendation(sess);
}

// Returns the movie's name.
string Movie::getName() const {
    return name;
}

// Clones a movie, creates a new one with the same parameters.
Movie *Movie::clone() {
    Movie* newMovie = new Movie(*this);
    return newMovie;
}


//***********************EPISODE********************************//

// Constructor
Episode::Episode(long id, const std::string& seriesName, int length, int season, int episode, const std::vector<std::string> &tags) :
        Watchable(id, length, tags), seriesName(""), season(season), episode(episode), nextEpisodeId(-1) {
    for (char i : seriesName) {
        this->seriesName.push_back(i);
    }
}

// Copy constructor
Episode::Episode(const Episode &other):Watchable(other.getID(),other.getLength(),other.getTags()), seriesName(""), season(other.getSeason()), episode(other.getEpisode()), nextEpisodeId(other.getNextEpisodeId()) {
    for (char i : other.getName()) {
        this->seriesName.push_back(i);
    }
}

// Copy assignment operator
Episode &Episode::operator=(const Episode &other) {
    if (this == &other) {
        return *this;
    }
    getTags().clear();
    for (const auto & tag : other.getTags()) {
        this->getTags().push_back(tag);
    }
    setLength(other.getLength());
    seriesName.clear();
    for (char i : other.getName()) {
        this->seriesName.push_back(i);
    }
    return *this;
}

// To string for the episodes's name, season and number of episode
string Episode::toString() const {
    string ret = seriesName + " S" + to_string(season) +"E" + to_string(episode);
    return ret;
}

// Returns the next watchable content. the next episode, if exists, or by the recommendation algorithm.
Watchable* Episode::getNextWatchable(Session &sess) const {
    if (getNextEpisodeId() == -1)
    {
        return sess.getActiveUser()->getRecommendation(sess);
    }
    return (&sess)->getContent()[getID()];
}

// Returns the episode's name.
string Episode::getName() const {
    return seriesName;
}

// Clones an episode, creates a new one with the same parameters.
Episode *Episode::clone() {
    Episode* newEpisode = new Episode(*this);
    return newEpisode;
}

// Returns the season of the episode.
int Episode::getSeason() const {
    return season;
}

// Returns the episode number.
int Episode::getEpisode() const {
    return episode;
}

// Return the next episode to watch.
long Episode::getNextEpisodeId() const {
    return nextEpisodeId;
}

// Sets the next episode to watch.
void Episode::setNextEpisodeId(long num) {
        nextEpisodeId = num;
}





