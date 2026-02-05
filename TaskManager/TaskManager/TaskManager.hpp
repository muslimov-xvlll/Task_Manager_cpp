#pragma once
#include <string>
#include "Task.hpp"
#include <vector>
using namespace std;

//перечисление видов сортировки
enum class SortMode {
	ById,
	ByTitle,
	ByDeadLine,
	ByStatus
};

//перечисление видов фильтров
enum class FilterMode {
	All,
	OnlyToDo,
	OnlyInProgress,
	OnlyDone
};


class TaskManager {
private:
	vector<Task> tasks;
	int nextId = 1;
public:


	bool isValidDeadline(const std::string& date) const;

	void addTask(const string& title, const string& description, const string& deadline, Status status);
	void listTasks() const;
	bool removeTask(int id);
	void editTask(int id, const string& newTitle, const string& newDescription, const string& newDeadline);
	bool changeStatus(int id, Status newStatus);
	bool exists(int id) const;
	bool isEmpty() const;

	void saveToFile(const string& filename) const;
	void loadFromFile(const string& filename);

	void sortTasks(SortMode mode);

	void listTasksFiltered(FilterMode mode) const;

	void viewTask(int id) const;

	bool titleExists(const std::string& title) const;
};