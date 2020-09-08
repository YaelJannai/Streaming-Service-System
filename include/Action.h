#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

using namespace std;

class Session;
class User;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};

class BaseAction{
public:
    //Constructors and Destructor
	BaseAction();
	BaseAction(const BaseAction &other);
    virtual ~BaseAction();

    //Pure Virtual Methods
	virtual void act(Session& sess)=0;
	virtual std::string toString() const=0;
    virtual BaseAction *clone() = 0;

	static string toStringStatus(const ActionStatus status1);
    ActionStatus getStatus() const;

protected:
	void complete();
	void error(const std::string& errorMsg);
    std::string getErrorMsg() const;

private:
	std::string errorMsg;
	ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    //Constructors
    CreateUser(string userName, string recommend);
    CreateUser(const CreateUser &other);

    //Virtual Methods
    virtual CreateUser *clone();
	virtual void act(Session& sess);
	virtual std::string toString() const;

	string getUserName() const;
	string getRecommend() const;

private:
    string userName;
    string recommend;
};

class ChangeActiveUser : public BaseAction {
public:
    //Constructors
    ChangeActiveUser(string userName);
    ChangeActiveUser(const ChangeActiveUser &other);

    //Virtual Methods
    virtual ChangeActiveUser *clone();
	virtual void act(Session& sess);
	virtual std::string toString() const;

    string getUserName() const;

private:
    string userName;
};

class DeleteUser : public BaseAction {
public:
    //Constructors
    DeleteUser(string userName);
    DeleteUser(const DeleteUser &other);

    //Virtual Methods
    virtual DeleteUser *clone();
	virtual void act(Session & sess);
	virtual std::string toString() const;

private:
    string userName;
};

class DuplicateUser : public BaseAction {
public:
    //Constructors
    DuplicateUser(string copyUserName, string  newUserName);
    DuplicateUser(const DuplicateUser &other);

    //Virtual Methods
    virtual DuplicateUser *clone();
	virtual void act(Session & sess);
	virtual std::string toString() const;

	string getCopyUserName() const;
    string getNewUserName() const;

private:
    string copyUserName;
    string newUserName;
};

class PrintContentList : public BaseAction {
public:
    //Constructors
    PrintContentList();
    PrintContentList(const PrintContentList &other);

    //Virtual Methods
    virtual PrintContentList *clone();
	virtual void act (Session& sess);
	virtual std::string toString() const;
};

class PrintWatchHistory : public BaseAction {
public:
    //Constructors
    PrintWatchHistory();
    PrintWatchHistory(const PrintWatchHistory &other);

    //Virtual Methods
    virtual PrintWatchHistory *clone();
	virtual void act (Session& sess);
	virtual std::string toString() const;
};

class Watch : public BaseAction {
public:
    //Constructors
    Watch(long id);
    Watch(const Watch &other);

    //Virtual Methods
    virtual Watch *clone();
	virtual void act(Session& sess);
	virtual std::string toString() const;

    size_t getId() const;

private:
    size_t id;
};

class PrintActionsLog : public BaseAction {
public:
    //Constructors
    PrintActionsLog();
    PrintActionsLog(const PrintActionsLog &other);

    //Virtual Methods
    virtual PrintActionsLog *clone();
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class Exit : public BaseAction {
public:
    //Constructors
    Exit();
    Exit(const Exit &other);

    //Virtual Methods
    virtual Exit *clone();
	virtual void act(Session& sess);
	virtual std::string toString() const;
};
#endif