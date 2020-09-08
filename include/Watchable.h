/**
 * h file.
 * Class watchable contain description of something a user can watch - movie & episode.
 * Each episode/movie has it's characteristics like name, length, etc.
 */

#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>

using namespace std;

class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    Watchable(const Watchable &other);
    Watchable& operator = (const Watchable &other);
    virtual ~Watchable();

    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual string getName() const = 0;
    virtual Watchable *clone() = 0;

    string toStringTags() const;
    long getID() const;
    int getLength() const;
    std::vector<std::string> getTags() const;
    void setLength(int length);

private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    Movie(const Movie &other);
    Movie& operator = (const Movie &other);

    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual string getName() const;
    virtual Movie *clone();

private:
    string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    Episode(const Episode &other);
    Episode&operator = (const Episode &other);

    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual string getName() const;
    virtual Episode *clone();

    int getSeason() const;
    int getEpisode() const;
    long getNextEpisodeId() const;
    void setNextEpisodeId(long num);

private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
