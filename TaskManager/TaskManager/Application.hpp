#pragma once
#include "TaskManager.hpp"
class Application {
private:
	TaskManager manager;

	void showMenu() const;
	void handleAdd();
	void handleList();
	void handleEdit();
	void handleRemove();
	void handleChangeStatus();
	void handleSort();
	void handleFilter();
	const std::string storageFile = "tasks.txt";


public:
	void run();
};