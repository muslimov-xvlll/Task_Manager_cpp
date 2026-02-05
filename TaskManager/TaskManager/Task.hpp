#pragma once
#include <string>

using namespace std;

enum class Status { ToDo, InProgress, Done };

class Task {
private:
    int id;
    string title;
    string description;
    Status status;
    string createdDate;
    string deadline;

public:
    Task(int id, const string& title, const string& desc, const string& deadline, Status status);

    int getId() const;
    string getTitle() const;
    string getDescription() const;
    Status getStatus() const;
    string getCreatedDate() const;
    string getDeadline() const;

    string statusToString(Status s) const;

    void setStatus(Status s);
    void setTitle(const string& t);
    void setDescription(const string& d);
    void setDeadline(const string& dl);

    string toString() const;

    void print() const;
};
