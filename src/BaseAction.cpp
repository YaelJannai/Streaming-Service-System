//
// Created by yael on 19/11/2019.
//
#include <iostream>
#include <utility>

#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"

using namespace std;

//Constructor
BaseAction::BaseAction() : errorMsg(""), status(PENDING) {}

//Copy Constructor
BaseAction::BaseAction(const BaseAction &other) : errorMsg(other.errorMsg), status(other.status) {}

//Destructor
BaseAction::~BaseAction() {}

/// This method return a string of the status.
/// \param status1 - an ActionStatus object to string.
/// \return a string of the status.
string BaseAction::toStringStatus(const ActionStatus status1) {
    if(status1 == 0)
    {
        return "PENDING";
    }
    else if(status1 == 1)
    {
        return "COMPLETED";
    }
    else
    {
        return "ERROR";
    }
}

// Getter of the status.
ActionStatus BaseAction::getStatus() const {
    return status;
}

/// This method change the status of the action to COMPLETE.
void BaseAction::complete() {
    status = COMPLETED;
}

/// This method change the status of the action to ERROR and set the error message.
/// \param errorMsg - the error message of the action.
void BaseAction::error(const std::string &errorMsg) {
    status = ERROR;
    this-> errorMsg = errorMsg;
    cout << "Error - " << this->getErrorMsg() << "\n";
}

//Getter of the ErrorMsg
std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}
//************************************CREATE_USER**********************************//

//Constructor
CreateUser::CreateUser(string userName, string recommend) : userName(std::move(userName)), recommend(std::move(recommend)) {}

//Copy Constructor
CreateUser::CreateUser(const CreateUser &other) : BaseAction(other), userName(""), recommend("")
{
    for (char i : other.getUserName()) {
        this->userName.push_back(i);
    }
    for (char i : other.getRecommend()) {
        this->recommend.push_back(i);
    }
}

///This method is in charge on creating a new user by the given user name and by the chosen recommendation algorithm given to the constructor.
/// \param sess - the session in which the user will be created.
void CreateUser::act(Session &sess)
{
    //Checking that the given userName is not already used by another user.
    if (sess.getUserMap().find(userName) == sess.getUserMap().end()){
        User* user;
        //We create a user by the type of recommendation algorithm that he chose.
        if(recommend == "len")
        {
            user = new LengthRecommenderUser(userName);
            sess.insertUserMap(userName, user);
            complete();
        }
        else if(recommend == "rer")
        {
            user = new RerunRecommenderUser(userName);
            sess.insertUserMap(userName, user);
            complete();
        }
        else if(recommend == "gen")
        {
            user = new GenreRecommenderUser(userName);
            sess.insertUserMap(userName, user);
            complete();
        }
        else
        {
            error("Recommendation algorithm is not valid");
        }
    }
    else
    {
        error("This user name already exists");
    }

    sess.insertActionLogs(this);
}

std::string CreateUser::toString() const {
    return "CreateUser " + toStringStatus(getStatus()) +" "+ getErrorMsg();
}

/// The Clone method clones the action by calling the copy-constructor of the new CreateUser object.
/// \return the new CreteUser action with the same values as this action.
CreateUser *CreateUser::clone()
{
    CreateUser *newCreate = new CreateUser(*this);
    return newCreate;
}

string CreateUser::getUserName() const {
    return userName;
}

string CreateUser::getRecommend() const {
    return recommend;
}
//************************************CHANGE_USER**********************************//

//Constructor
ChangeActiveUser::ChangeActiveUser(string userName) : userName(userName){}

//Copy Constructor
ChangeActiveUser::ChangeActiveUser(const ChangeActiveUser &other): BaseAction(other), userName("")
{
    for (char i : other.getUserName()) {
        this->userName.push_back(i);
    }
}

/// This method is in charge of changing the active user to be the user that its name is given to the constructor
/// \param sess - the session that the active user will be.
void ChangeActiveUser::act(Session &sess)
{
    //First, we find the user in the UserMap of the session, and then activating it.
    auto iter = sess.getUserMap().find(userName);
    if (iter != sess.getUserMap().end()){
        User *toActive(sess.getUserMap().at(userName));
        sess.setActiveUser(toActive);
        complete();
    }
    else {
        error("User name was not found in users list");
    }
    sess.insertActionLogs(this);
}

std::string ChangeActiveUser::toString() const {
    return "ChangeActiveUser "+ toStringStatus(getStatus()) +" "+ getErrorMsg();
}

/// The Clone method clones the action by calling the copy-constructor of the new ChangeActiveUser object.
/// \return the new ChangeActiveUser action with the same values as this action.
ChangeActiveUser *ChangeActiveUser::clone()
{
    ChangeActiveUser *newChange = new  ChangeActiveUser(*this);
    return newChange;
}

string ChangeActiveUser::getUserName() const {
    return userName;
}
//************************************DELETE_USER**********************************//

//Constructor
DeleteUser::DeleteUser(string userName) : userName(userName){}

//Copy Constructor
DeleteUser::DeleteUser(const DeleteUser &other): BaseAction(other), userName("")
{
    for (char i : other.userName) {
        this->userName.push_back(i);
    }
}

/// This method is in charge of deleting a user by its given name to the constructor.
/// \param sess - the session that the user will be deleted from.
void DeleteUser::act(Session &sess)
{
    //First we find the user to delete.
    unordered_map<std::string,User*>::iterator iter;
    iter = sess.getUserMap().find(userName);
    if (iter != sess.getUserMap().end()){
        User *toDelete(sess.getUserMap().at(userName));

        //Checking if the chosen user is the active user, because we cannot delete the active user.
        if (userName == sess.getActiveUser()->getName())
        {
            error("Cannot delete active user");
        }
        else
        {
            delete(toDelete);
            sess.deleteUserMap(userName);
            complete();
        }
    }
    else {
        error("User name was not found in users list");
    }
    sess.insertActionLogs(this);
}

std::string DeleteUser::toString() const {
    return "DeleteUser " + toStringStatus(getStatus()) +" "+ getErrorMsg();
}

/// The Clone method clones the action by calling the copy-constructor of the new DeleteUser object.
/// \return the new DeleteUser action with the same values as this action.
DeleteUser *DeleteUser::clone()
{
    DeleteUser *newDelete = new DeleteUser(*this);
    return newDelete;
}
//************************************DUPLICATE_USER*******************************//

//Constructor
DuplicateUser::DuplicateUser(string copyUserName, string newUserName):copyUserName(std::move(copyUserName)), newUserName(std::move(newUserName)) {}

//Copy Constructor
DuplicateUser::DuplicateUser(const DuplicateUser &other) : BaseAction(other), copyUserName(""), newUserName("")
{
    for (char i : other.getCopyUserName()) {
        this->copyUserName.push_back(i);
    }
    for (char i : other.getNewUserName()) {
        this->newUserName.push_back(i);
    }
}

/// This method is in charge to create a new user with the same recommendation algorithm and watch history as an old chosen user,
/// the names of the old user and the new one are give in the constructor.
/// \param sess - the session that the new user will be created to and the old one is in it.
void DuplicateUser::act(Session &sess)
{

    //First we check if the old user is exists in the UserMap of the session, and that the name of the new one is not.
    auto it1(sess.getUserMap().find(copyUserName));
    auto it2(sess.getUserMap().find(newUserName));
    if (it1 != sess.getUserMap().end()){
        if(it2 == sess.getUserMap().end())
        {
            User* userToCopy(sess.getUserMap().at(copyUserName));
            //We call to the clone method of the user with the name of the new one.
            User* dupUser(userToCopy->clone(newUserName));
            sess.insertUserMap(newUserName, dupUser);
            complete();
        }
        else
        {
            error("The username you chose already exist");
        }
    }
    else {
        error("The user you want to copy does not exist");
    }

    sess.insertActionLogs(this);
}

std::string DuplicateUser::toString() const {
    return "DuplicateUser " + toStringStatus(getStatus()) +" "+ getErrorMsg();
}

/// The Clone method clones the action by calling the copy-constructor of the new DuplicateUser object.
/// \return the new DuplicateUser action with the same values as this action.
DuplicateUser *DuplicateUser::clone()
{
    DuplicateUser *newDup = new DuplicateUser(*this);
    return newDup;
}

string DuplicateUser::getCopyUserName() const {
    return copyUserName;
}

string DuplicateUser::getNewUserName() const {
    return newUserName;
}
//***********************************PRINT_CONTENT_LIST****************************//

//Constructor
PrintContentList::PrintContentList(){}

//Copy Constructor
PrintContentList::PrintContentList(const PrintContentList &other) : BaseAction(other){}

/// This method is in charge to print all the content of a given session.
/// \param sess - the session in which the content is in it.
void PrintContentList::act(Session &sess)
{
    vector <Watchable*> contentVec = sess.getContent();

    for (size_t i = 0; i < contentVec.size(); ++i) {
        cout<<to_string(i+1)<<". "<<contentVec[i]->toString()<<" "<<to_string(contentVec[i]->getLength())<<" minutes "<<contentVec[i]->toStringTags()<<"\n";
    }

    complete();
    sess.insertActionLogs(this);
}

std::string PrintContentList::toString() const {
   return "PrintContentList " + toStringStatus(getStatus()) +" "+ getErrorMsg();
}

/// The Clone method clones the action by calling the copy-constructor of the new PrintContentList object.
/// \return the new PrintContentList action with the same values as this action.
PrintContentList *PrintContentList::clone()
{
    PrintContentList *newContent = new PrintContentList(*this);
    return newContent;
}
//*************************PRINT_WATCH_HISTORY************************************//

//Constructor
PrintWatchHistory::PrintWatchHistory() {}

//Copy constructor
PrintWatchHistory::PrintWatchHistory(const PrintWatchHistory &other) :BaseAction(other) {}

/// This method is in charge of printing the watch history of the active user of a given session.
/// \param sess - the session in which the active user is in it.
void PrintWatchHistory::act(Session &sess)
{
    cout << "Watch history for " << sess.getActiveUser()->getName() << endl;
    vector <Watchable*> historyVec = sess.getActiveUser()->get_history();

    for (size_t i = 0; i < historyVec.size(); ++i) {
        cout<<to_string(i+1)<<". "<<historyVec[i]->toString()<<"\n";
    }
    complete();
    sess.insertActionLogs(this);
}

std::string PrintWatchHistory::toString() const {
    return "PrintWatchHistory " + toStringStatus(getStatus()) +" "+ getErrorMsg();
}

/// The Clone method clones the action by calling the copy-constructor of the new PrintWatchHistory object.
/// \return the new PrintWatchHistory action with the same values as this action.
PrintWatchHistory *PrintWatchHistory::clone() {
    PrintWatchHistory *newHist = new PrintWatchHistory(*this);
    return newHist;
}
//************************************WATCH***************************************//

//Constructor
Watch::Watch(long id) : id(id) {}

//Copy Constructor
Watch::Watch(const Watch &other) : BaseAction(other), id(other.getId()){}

/// This method is in charge of showing the user the movie/episode that he chose to watch by the id of the watchable that is given to the constructor.
/// \param sess - the session in which the content and the user are.
void Watch::act(Session &sess)
{
    if (id <= 0 || id > sess.getContent().size()){
        error("The id you gave is not valid");
        sess.insertActionLogs(this);
    }
    else {
        //Making the user watch what he chose, and adding it to his watch history.
        Watchable* watch = sess.getContent()[id-1];
        cout << "Watching " << sess.getContent()[id-1]->toString() << "\n";
        sess.getActiveUser()->insertHistory(watch);

        //Now we recommend the user to watch another watchable thing base on the algorithms.
        Watchable* nextWatch(watch->getNextWatchable(sess));
        if(nextWatch != nullptr)
        {
            cout << "We recommend watching " << nextWatch->toString() << " continue watching? [y/n]\n";

            //We check if the user chose to watch what we recommend to him.
            string answer;
            getline(cin, answer);
            //char ans = answer[0];

            if(answer == "y" || answer == "n")
            {
                complete();
                sess.insertActionLogs(this);
                //if he says 'yes' we call to another action of watch with the new id.
                if(answer == "y")
                {
                    Watch *nextWatchAction = new Watch(nextWatch->getID());
                    nextWatchAction->act(sess);
                }
            }
            else
            {
                error("Invalid character");
                sess.insertActionLogs(this);
            }
        }
    }
}

std::string Watch::toString() const {
    return "Watch " + toStringStatus(getStatus()) +" "+ getErrorMsg();
}

/// The Clone method clones the action by calling the copy-constructor of the new Watch object.
/// \return the new Watch action with the same values as this action.
Watch *Watch::clone()
{
    Watch *newWatch = new Watch(*this);
    return newWatch;
}

size_t Watch::getId() const {
    return id;
}
//**********************************PRINT_ACTIONS_LOG**********************************//

//Constructor
PrintActionsLog::PrintActionsLog() {}

//Copy Constructor
PrintActionsLog::PrintActionsLog(const PrintActionsLog &other) : BaseAction(other) {}

/// This methos is in charge of printing all the actions that were done in a given session with their status,
/// the printing will be done in a reverse order
/// \param sess - the session in which the actions occurred.
void PrintActionsLog::act(Session &sess)
{
    for (int i = sess.getActionLogs().size()-1; i >=0 ; --i) {
       cout << sess.getActionLogs()[i]->toString() << endl;
    }

    complete();
    sess.insertActionLogs(this);
}

string PrintActionsLog::toString() const {
    return  "PrintActionsLog " + toStringStatus(getStatus()) +" "+ getErrorMsg();
}

/// The Clone method clones the action by calling the copy-constructor of the new PrintActionsLog object.
/// \return the new PrintActionsLog action with the same values as this action.
PrintActionsLog *PrintActionsLog::clone() {
    PrintActionsLog *newLog = new PrintActionsLog(*this);
    return newLog;
}
//***************************************EXIT**************************************//

//Constructor
Exit::Exit() {}

//Copy Constructor
Exit::Exit(const Exit &other) : BaseAction(other) {}

/// this method is in charge to exit the given session, without deleting any of the parameters in it.
/// \param sess - the session the user is want to exit from
void Exit::act(Session &sess) {
    complete();
    sess.insertActionLogs(this);
}

std::string Exit::toString() const {
    return "Exit " + toStringStatus(getStatus()) +" "+ getErrorMsg();
}

/// The Clone method clones the action by calling the copy-constructor of the new Exit object.
/// \return the new Exit action with the same values as this action.
Exit *Exit::clone()
{
    Exit *newExit = new Exit(*this);
    return newExit;
}