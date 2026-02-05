#include "Task.hpp"
#include <sstream>
#include <iostream>
#include "Color.hpp"
#include <ctime>

string getCurrentDate() {
    time_t now = time(nullptr);
    tm local{};
    localtime_s(&local, &now);

    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d.%m.%Y", &local);

    return buffer;
}

Task::Task(int id, const string& title, const string& desc, const string& deadline, Status status)
    : id(id), title(title), description(desc), deadline(deadline), status(status)
{
    createdDate = getCurrentDate();
}

int Task::getId() const { return id; }
string Task::getTitle() const { return title; }
string Task::getDescription() const { return description; }
Status Task::getStatus() const { return status; }
string Task::getCreatedDate() const { return createdDate; }
string Task::getDeadline() const { return deadline; }

void Task::setStatus(Status s) { status = s; }
void Task::setTitle(const string& t) { title = t; }
void Task::setDescription(const string& d) { description = d; }
void Task::setDeadline(const string& dl) { deadline = dl; }

string Task::statusToString(Status s) const {
    switch (s) {
    case Status::ToDo: return "ToDo";
    case Status::InProgress: return "InProgress";
    case Status::Done: return "Done";
    }
    return "Unknown";
}

string Task::toString() const {
    std::stringstream ss;
    ss << "[" << id << "] "
        << "(" << statusToString(status) << ") "
        << title << " — дедлайн: " << deadline << "\n"
        << "Описание: " << description << "\n"
        << "Создано: " << createdDate;
    return ss.str();
}

void Task::print() const {
    // Выбор цвета по статусу
    switch (status) {
    case Status::ToDo:
        setColor(COLOR_TODO);
        break;
    case Status::InProgress:
        setColor(COLOR_INPROGRESS);
        break;
    case Status::Done:
        setColor(COLOR_DONE);
        break;
    }

    // Вывод задачи
    std::cout << toString() << "\n";

    // Сброс цвета
    setColor(COLOR_DEFAULT);
}


