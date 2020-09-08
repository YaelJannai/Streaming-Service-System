/**
 * h file.
 * Class User is responsible to the user's characteristics. Save his name and the history vector.
 * Also contain the logics for each user type, depending on the recommendations algorithm.
 **/

#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;

using namespace std;

class User{
public:
    User(const std::string& name);
    User(const User &other);
    User& operator = (const User &other);
    virtual ~User();

    virtual Watchable* getRecommendation(Session& s) = 0;
    virtual User *clone(string username) =0;
    virtual string toString() const = 0;

    std::string getName() const;
    std::vector<Watchable*> get_history() const;

    void insertHistory(Watchable* watch);
    void setHistoryAt(Watchable* watch, size_t i);

protected:
    std::vector<Watchable*> history;

private:
     std::string name;

};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    LengthRecommenderUser(const LengthRecommenderUser &other);
    LengthRecommenderUser& operator = (const LengthRecommenderUser &other);

    virtual Watchable* getRecommendation(Session& s);
    virtual User *clone(string username);
    virtual string toString() const;

private:

};


class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    RerunRecommenderUser(const RerunRecommenderUser &other);
    RerunRecommenderUser& operator = (const RerunRecommenderUser &other);

    virtual Watchable* getRecommendation(Session& s);
    virtual User *clone(string username);
    virtual string toString() const;

private:
    long cell;
};



class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    GenreRecommenderUser(const GenreRecommenderUser &other);
    GenreRecommenderUser& operator = (const GenreRecommenderUser &other);

    virtual Watchable* getRecommendation(Session& s);
    virtual User *clone(string username);
    virtual string toString() const;

private:
};

#endif
