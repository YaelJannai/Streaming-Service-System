#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

using namespace std;

class User;
class Watchable;

class Session{
public:
    //Rule of five
    Session(const std::string &configFilePath);
    Session(const Session &other);
    Session& operator = (const Session &other);
    Session(Session &&other);
    Session& operator = (Session &&other);
    ~Session();

    void start();
    void chooseAction(string request, vector<string> actions);

    //Getters
    vector<BaseAction*> getActionLogs() const;
    vector<Watchable*> getContent() const;
    User* getActiveUser() const;
    unordered_map<std::string,User*> getUserMap() const;

    //Setters
    void setActiveUser(User* user);
    void insertActionLogs(BaseAction* action);
    void insertUserMap(string name, User* user);
    void deleteUserMap(string name);

    static bool isNumber(const string& s);

private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
};
#endif
