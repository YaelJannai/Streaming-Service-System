//
// Created by yael on 18/11/2019.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../include/Session.h"
#include "../include/json.hpp"
#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Action.h"

using namespace std;

//Constructor
Session::Session(const std::string &configFilePath) : content(vector<Watchable*>()), actionsLog(vector<BaseAction*>()),userMap(unordered_map<string, User*>()),activeUser(nullptr)
{
    //Reading the Json file
    using json = nlohmann::json;
    std::ifstream i(configFilePath);
    json j;
    i >> j;

    //Running over all the movies in the Json file.
    auto movies = j["movies"];
    long id(1);
    for(auto & movie : movies){
        //Creating a new Watchable object from the Movie type, and pushing it to the end of the content vector.
        Movie* movie1 = new Movie(id, movie["name"], movie["length"], movie["tags"]);
        content.push_back(movie1);
        id++;
    }

    //Running over all the tv-series in the Json file.
    auto tv_series = j["tv_series"];
    for (auto & episode : tv_series){
        for (size_t k = 0; k < (episode["seasons"]).size() ; ++k) {
            int count = 1;
            while (count <= episode["seasons"][k]){
                //For every show we create the same amount of Episode objects as the number of episodes that the show contains, and pushing it to the content vector.
                Episode* episode1 = new Episode(id, episode["name"], episode["episode_length"], k+1, count, episode["tags"]);
                count++;
                content.push_back(episode1);
                id++;

                //If the episode that we are dealing with is not the last of its series, we set its next episode id to be the next id,
                //if it's the last it will be initialize -1 in its constructor.
                if (k != (episode["seasons"]).size()-1 || count <= (episode["seasons"][k]))
                {
                    episode1->setNextEpisodeId(id);
                }
            }
        }
    }

    //Creating a default user with the length recommendation algorithm and setting it to be the active user.
    User *user = new LengthRecommenderUser("default");
    insertUserMap("default", user);
    activeUser = user;
}

//Copy Constructor
Session::Session(const Session &other) : content(vector<Watchable*>()), actionsLog(vector<BaseAction*>()),userMap(unordered_map<string, User*>()),activeUser(nullptr)
{
    //Cloning all the watchable content that the other session has, be deep copy.
    for (size_t i = 0; i < other.getContent().size(); ++i) {
       Watchable *newWatch = other.getContent()[i]->clone();
       this->content.push_back(newWatch);
    }

    //Cloning all the actions in the action log of the other session, be deep copy.
    for (auto i : other.actionsLog) {
        BaseAction *newAction = i->clone();
        this->actionsLog.push_back(newAction);
    }

    //Cloning all the users  that the other session has and  remove all their watch history pointers to the new content vector, be deep copy.
    for (std::pair<std::string,User*> element : other.userMap){
        std::string first1 = element.first;
        User* second2 = element.second->clone(element.first);
        for (size_t i = 0; i < second2->get_history().size(); ++i) {
           second2->setHistoryAt(this->content[element.second->get_history()[i]->getID()-1],i);
        }
        std::pair<std::string,User*> p(first1,second2);
        this->userMap.insert(p);
    }

    //Setting the active user
    activeUser = this->userMap.at(other.getActiveUser()->getName());
}

//Copy Assignment Operator
Session& Session::operator=(const Session &other)
{
    if (this != &other)
    {
        //Deleting all the Watchable objects in content, and clearing it.
        for (auto & k : content) {
            delete(k);
            k = nullptr;
        }
        content.clear();

        //Deleting all the actions from the actions log, and clearing it.
        for (auto & j : actionsLog) {
            delete(j);
            j = nullptr;
        }
        actionsLog.clear();

        //Setting the active user to point to a null pointer.
        activeUser = nullptr;

        //Deleting all the users from the userMap and  clearing it.
        for (auto it = userMap.begin(); it != userMap.end(); ++it){
            delete(it->second);
            it->second = nullptr;
        }
        userMap.clear();

        //Cloning all the watchable content that the other session has, be deep copy.
        for (size_t i = 0; i < other.getContent().size(); ++i) {
            Watchable *newWatch = other.getContent()[i]->clone();
            this->content.push_back(newWatch);
        }

        //Cloning all the actions in the action log of the other session, be deep copy.
        for (auto i : other.actionsLog) {
            BaseAction *newAction = i->clone();
            this->actionsLog.push_back(newAction);
        }

        //Cloning all the users  that the other session has and remove all their watch history pointers to the new content vector, be deep copy.
        for (std::pair<std::string,User*> element : other.userMap){
            std::string first1 = element.first;
            User* second2 = element.second->clone(element.first);
            for (size_t i = 0; i < second2->get_history().size(); ++i) {
                second2->setHistoryAt(this->content[element.second->get_history()[i]->getID()-1],i);
            }
            std::pair<std::string,User*> p(first1,second2);
            this->userMap.insert(p);
        }

        //Setting the active user.
        activeUser = this->userMap.at(other.getActiveUser()->getName());
    }
    return *this;
}


//Move Constructor
Session::Session(Session &&other) : content(vector<Watchable*>()), actionsLog(vector<BaseAction*>()),userMap(unordered_map<string, User*>()),activeUser(other.getActiveUser()) {

    //Moving all my pointers to the other's pointers.
    content = other.getContent();
    actionsLog = other.getActionLogs();
    userMap = other.getUserMap();
    activeUser = other.getActiveUser();

    //Clearing all the other vector.
    other.content.clear();
    other.actionsLog.clear();
    other.userMap.clear();
    other.setActiveUser(nullptr);
}

//Move Assignment Operator
Session& Session::operator = (Session &&other)
{
    if (this != &other)
    {
        //Deleting all the Watchable objects in content, and clearing it.
        for (auto & k : content) {
            delete(k);
            k = nullptr;
        }
        content.clear();

        //Deleting all the actions from the actions log, and clearing it.
        for (auto & j : actionsLog) {
            delete(j);
            j = nullptr;
        }
        actionsLog.clear();

        //Setting the active user to point to a null pointer.
        activeUser = nullptr;

        //Deleting all the users from the userMap and  clearing it.
        for (auto & it : userMap){
            delete(it.second);
            it.second = nullptr;
        }
        userMap.clear();

        //Moving all my pointers to the other's pointers.
        content = other.getContent();
        actionsLog = other.getActionLogs();
        userMap = other.getUserMap();
        activeUser = other.getActiveUser();

        //Clearing all the other vector.
        other.content.clear();
        other.actionsLog.clear();
        other.userMap.clear();
        other.setActiveUser(nullptr);
    }
    return *this;
}

//Destructor
Session::~Session() {

    //Deleting all the Watchable objects in content, and clearing it.
    for (auto & k : content) {
        delete(k);
        k = nullptr;
    }
    content.clear();

    //Deleting all the actions from the actions log, and clearing it.
    for (auto & j : actionsLog) {
        delete(j);
        j = nullptr;
    }
    actionsLog.clear();

    //Setting the active user to point to a null pointer.
    activeUser = nullptr;

    //Deleting all the users from the userMap and  clearing it.
    for (auto & it : userMap){
        delete(it.second);
        it.second = nullptr;
    }
    userMap.clear();
}

/// This function is in charge to starting the program by asking the user for commands.
void Session::start() {

    //Vector of all the optional actions.
    vector<string> actions {"createuser", "changeuser", "deleteuser", "dupuser", "content", "watchhist", "watch", "log", "exit"};

    std::cout << "SPLFLIX is now on!" << std::endl;

    string request("");

    //A loop that ask the user to enter a command until he command "exit".
    //The loop call the chooseAction method with the string that was inserted.
    while (request != "exit"){
        cout << "\nEnter an action you want to do:\n";
        getline(cin, request);
        chooseAction(request, actions);
    }
}

/// This method is calling the right baseAction based of the request.
/// \param request - A string that contains the base action and its parameters divided by spaces.
/// \param actions - A vector that contains all the possible actions.
void Session::chooseAction(string request, vector< string> actions)
{

   string act(request.substr(0, request.find(' ')));
   request = request.substr(request.find(' ')+1);

   //We find the cell in the vector that contains the written action, and by switch we choose what to do for every cell (action).
    size_t i;
    for (i = 0 ;i < actions.size(); ++i) {
        if(actions[i] == act) {
            switch (i) {

                //Create user action:
                case 0: {
                    string userName(request.substr(0, request.find(' ')));
                    request = request.substr(request.find(' ') + 1);
                    CreateUser *cu = new CreateUser(userName, request);
                    cu->act(*this);
                }
                break;

                //Change active user action:
                case 1: {
                    ChangeActiveUser *cau = new ChangeActiveUser(request);
                    cau->act(*this);
                }
                break;

                //Delete user action:
                case 2: {
                    DeleteUser *du = new DeleteUser(request);
                    du->act(*this);
                }
                break;

                //Duplicate user action:
                case 3: {
                    string copyFrom(request.substr(0, request.find(' ')));
                    request = request.substr(request.find(' ') + 1);
                    DuplicateUser *dupu = new DuplicateUser(copyFrom, request);
                    dupu->act(*this);
                }
                break;

                //Print content list action:
                case 4: {
                    PrintContentList *list = new PrintContentList();
                    list->act(*this);
                }
                break;

                //Print watch history action:
                case 5: {
                    PrintWatchHistory *hist = new PrintWatchHistory();
                    hist->act(*this);
                }
                break;

                //Watch action:
                case 6: {
                    string num(request.substr(request.find(' ') + 1));
                    //Checking if the given string is made by a digits, and then transforming it to a long type variable.
                    if (isNumber(num)) {
                        long id(stol(num));
                        Watch *watchu = new Watch(id);
                        watchu->act(*this);
                    }
                }
                break;

                //Print actions log action:
                case 7: {
                    PrintActionsLog *log1 = new PrintActionsLog();
                    log1->act(*this);
                }
                break;

                //Exit action:
                case 8:
                {
                    Exit *exit1 = new Exit();
                    exit1->act(*this);
                }
                break;
            }
            break;
        }
    }

    //If none of the above cases fits, it means the user wrote an invalid action
    if(i == actions.size())
    {
        cout<< "You chose an invalid action \n";
    }
}


//Get ActionsLogs
vector<BaseAction*> Session::getActionLogs() const {
    return actionsLog;
}

//Get Content
vector<Watchable*> Session::getContent() const{
    return content;
}

//Get ActiveUser
User* Session::getActiveUser() const {
    return activeUser;
}

//Get UserMap
unordered_map<std::string,User*> Session::getUserMap() const {
    return userMap;
}

//Set ActiveUser
void Session::setActiveUser(User *user) {
    activeUser = user;
}

//Set ActionsLogs
void Session::insertActionLogs(BaseAction *action) {
    this->actionsLog.push_back(action);
}

//Set UserMap
void Session::insertUserMap(string name, User* user) {
    userMap.insert({name, user});

}

/// This method checks if a given String is made by numerical digits.
/// \param s - String to check
/// \return True - if it made by digits, False - else.
bool Session::isNumber(const string& s) {
    for (char i : s){
        if (!isdigit(i))
            return false;
    }
    return true;
}

/// This method deletes from the userMap base on a given name.
/// \param name - name of the user to delete.
void Session::deleteUserMap(string name) {
    userMap.erase(name);
}