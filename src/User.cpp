/**
 * cpp file.
 * Class User is responsible to the user's characteristics. Save his name and the history vector.
 * Also contain the logics for each user type, depending on the recommendations algorithm.
 **/


#include "../include/User.h"
#include <iostream>
#include <algorithm>
#include "../include/Watchable.h"
#include "../include/Session.h"
using namespace std;

// Constructor
User::User(const std::string &name) : history(vector<Watchable*>()), name(name) {}

// Copy constructor
User::User(const User &other) : history(vector<Watchable*>()),  name(""){
    for (auto histWatch : other.get_history()) {
        this->insertHistory(histWatch);
    }
    for (char i : other.getName()) {
        name.push_back(i);
    }
}

// Copy assignment operator
User &User::operator=(const User &other) {
    if (this == &other) {
        return *this;
    }

    // Copy the history vector of other.
    this->history.clear();
    for (auto histWatch : other.get_history()) {
        this->insertHistory(histWatch);
    }

    name.clear();
    for (char i : other.getName()) {
        name.push_back(i);
    }
    return *this;
}

// Function that returns the user name, const
std::string User::getName() const {
    return name;
}

// Function that return the user history vector, const
std::vector<Watchable*> User::get_history() const {
    return history;
}

// Function that insert a value at the end of history vector
void User::insertHistory(Watchable *watch) {
    history.push_back(watch);
}

// Function that insert a value to a specific place in the history vector
void User::setHistoryAt(Watchable *watch, size_t i) {
    history[i] = watch;
}

// Destructor for user
User::~User() {
    history.clear();
}

//***************************************LENGTH*************************************//

// Constructor
LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name){}

// Copy constructor
LengthRecommenderUser::LengthRecommenderUser(const LengthRecommenderUser &other): User(other) {}

// Copy assignment operator
LengthRecommenderUser &LengthRecommenderUser::operator=(const LengthRecommenderUser &other) {
    if (this == &other) {
        return *this;
    }

    // Copy the history vector of other.
    this->history.clear();
    for (auto histWatch : other.get_history()) {
        this->insertHistory(histWatch);
    }

    getName().clear();
    for (char i : other.getName()) {
        getName().push_back(i);
    }
    return *this;
}

/// This function returns the recommendation for the next watchable to watch, based on the average length in the user watch history.
/// \param s a session that the user is active in
/// \return the next thing to watch
Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    if(history.empty())
    {
        return nullptr;
    }

    int sum(0);
    for (auto & i : history) {
       sum+= i->getLength();
    }

    // Calculates the average length in the user history vector.
    // Creates a new vector of pairs from content, pairs ow watchable + distance' between the watchable length and the average one.
    int average = sum/history.size();
    vector<pair<Watchable*, int>> distanceVec((vector<pair<Watchable*, int>>()));
    for (size_t j = 0; j < s.getContent().size(); ++j) {
        int dis(abs(average-s.getContent()[j]->getLength())) ;
        pair<Watchable*, int> p(s.getContent()[j], dis);
        distanceVec.push_back(p);
    }

    // Sort the new vector according to the 'distance' between the watchable length and the average one. Stable sorting, also by lexicographical order.
    sort(distanceVec.begin(), distanceVec.end(),[](const pair<Watchable*, int >& l, const pair<Watchable*, int >& r){
        if (l.second != r.second) {
            return l.second < r.second;
        }
        return l.first < r.first;
    });

    // Loops over the distanceVec, if a watchable fits the algorithm returns it, if the user didn't watch it already.
    for (auto & k : distanceVec) {
        for (size_t i = 0; i < history.size(); ++i) {
            vector<Watchable*>::iterator iter;
            iter = find(history.begin(), history.end(), k.first);
            if(iter == history.end())
            {
                return k.first;
            }
        }
    }
    // Returns null if didn't find a watchable to return.
    return nullptr;
}

/// This function clone this user  to a new user, with the name it receives
/// \param username for the new user
/// \return a LengthRecommenderUser with the same watch history and the given user name.
User *LengthRecommenderUser::clone(string username) {
    LengthRecommenderUser* newUser = new LengthRecommenderUser(username);
    newUser->history.clear();
    for (auto histWatch : history) {
        newUser->insertHistory(histWatch);
    }
    return newUser;
}

// To string method, return user type
string LengthRecommenderUser::toString() const {
    return "len";
}


//**********************************RERUN*****************************************//

//constructor
RerunRecommenderUser::RerunRecommenderUser(const std::string &name):User(name), cell(0) {}

//copy constructor
RerunRecommenderUser::RerunRecommenderUser(const RerunRecommenderUser &other):User(other), cell(other.cell) {}

//copy assignment operator
RerunRecommenderUser &RerunRecommenderUser::operator=(const RerunRecommenderUser &other) {
    if (this == &other) {
        return *this;
    }

    // Copy the history vector of other.
    this->history.clear();
    for (auto histWatch : other.get_history()) {
        this->insertHistory(histWatch);
    }
    //CHECK IF THAT IS THE CORRECT WAY TO DO IT
    getName().clear();
    for (char i : other.getName()) {
        getName().push_back(i);
    }

    cell = other.cell;
    return *this;
}

/// This function returns the recommendation for the next watchable to watch, from the user watch history.
/// \param s a session that the user is active in
/// \return the next thing to watch
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    if (history.empty()){
        return nullptr;
    }
    long num = cell;
    cell++;
    //Calculates and returns the next watchable to watch.
    return history[num%(history.size())];
}

/// This function clone this user  to a new user, with the name it receives
/// \param username for the new user
/// \return a RerunRecommenderUser with the same watch history and the given user name.
User *RerunRecommenderUser::clone(string username) {
    RerunRecommenderUser* newUser = new RerunRecommenderUser(username);
    newUser->history.clear();
    for (auto histWatch : history) {
        newUser->insertHistory(histWatch);
    }
    return newUser;
}

// To string method, return user type
string RerunRecommenderUser::toString() const {
    return "rer";
}


//****************************************GENRE**********************************//

// Constructor
GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name){}

// Copy constructor
GenreRecommenderUser::GenreRecommenderUser(const GenreRecommenderUser &other):User(other) {}

// Copy assignment operator
GenreRecommenderUser &GenreRecommenderUser::operator=(const GenreRecommenderUser &other) {
    if (this == &other) {
        return *this;
    }

    // Copy the history vector of other.
    this->history.clear();
    for (auto histWatch : other.get_history()) {
        this->insertHistory(histWatch);
    }
    getName().clear();
    for (char i : other.getName()) {
        getName().push_back(i);
    }
    return *this;
}

/// This function returns the recommendation for the next watchable to watch, based on the popular tags from the user watch history.
/// \param s a session that the user is active in
/// \return the next thing to watch
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {

    if(history.empty())
    {
        return nullptr;
    }

    unordered_map<string,int> tagsMap((unordered_map<string,int>()));

    // Loops over the history vector, and on the watchable's tags, insert to a new vector the tags and the frequency of them.
    for (auto & i : history) {
        for (size_t j = 0; j < i->getTags().size(); ++j) {
           string tag (i->getTags()[j]);
            unordered_map<std::string,int>::iterator iter;
            iter = tagsMap.find(tag);
            if (iter != tagsMap.end()){
                iter->second++;
            }
            else {
                pair<string,int > p(tag, 1);
                tagsMap.insert(p);
            }
        }
    }

    // Sorts the tags by their frequency and then by a lexicographical order.
    vector<pair<string,int >> tagVec;
    copy(tagsMap.begin(), tagsMap.end(), back_inserter<vector<pair<string,int >>>(tagVec));
    sort(tagVec.begin(), tagVec.end(),[](const pair<string,int >& l, const pair<string,int >& r){
        if (l.second != r.second) {
            return l.second > r.second;
        }
        return l.first < r.first;
    });

    // Searches for a watchable in content that has the popular value and is not in the history vector.
    // If doesn't find any, search for the second popular, and so on. Is there isn't any, returns null.
    string popularTag;
    for (auto & k : tagVec) {
        popularTag=k.first;
        for (size_t i = 0; i < s.getContent().size(); ++i) {
            vector<Watchable*>::iterator it1;
            it1 = find(history.begin(), history.end(), s.getContent()[i]);
            if(it1 == history.end())
            {
                for (size_t j = 0; j < s.getContent()[i]->getTags().size(); ++j) {
                    if(popularTag == s.getContent()[i]->getTags()[j])
                    {
                        return s.getContent()[i];
                    }
                }
            }
        }

    }
    return nullptr;
}

/// This function clone this user  to a new user, with the name it receives
/// \param username for the new user
/// \return a GenreRecommenderUser with the same watch history and the given user name.
User *GenreRecommenderUser::clone(string username) {
    GenreRecommenderUser* newUser = new GenreRecommenderUser(username);
    newUser->history.clear();
    for (auto histWatch : history) {
        newUser->insertHistory(histWatch);
    }
    return newUser;
}

// To string method, return user type
string GenreRecommenderUser::toString() const {
    return "gen";
}





