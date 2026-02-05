#include "Application.hpp"
#include "UI.hpp"
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;



auto isBlank = [](const std::string& s) {
	return std::all_of(s.begin(), s.end(), isspace);
	};

std::string trim(const std::string& s) {
	size_t start = s.find_first_not_of(" \t\n\r");
	if (start == std::string::npos)
		return ""; // строка состоит только из пробелов

	size_t end = s.find_last_not_of(" \t\n\r");
	return s.substr(start, end - start + 1);
}

void Application::showMenu() const {

	uiHeader("TASK MANAGER");

	cout << "1 - Добавить задачу \n"
		"2 - Показать все задачи \n"
		"3 - Редактировать задачу \n"
		"4 - Удалить задачу \n"
		"5 - Изменить статус задачи\n"
		"6 - Сортировка задач\n"
		"7 - Фильтрация задач\n"
		"8 — Просмотреть задачу\n"
		"0 - Выход\n";

	uiLine();
}

void Application::run() {
	manager.loadFromFile(storageFile);
	int change;
	while (true) {
		showMenu();
		cin >> change;

		switch (change)
		{
		case 1: handleAdd(); break;
		case 2: handleList(); break;
		case 3: handleEdit(); break;
		case 4: handleRemove(); break;
		case 5: handleChangeStatus(); break;
		case 6: handleSort(); break;
		case 7: handleFilter(); break;
		case 8: {
			int id;
			cout << "Введите ID задачи: ";
			cin >> id;
			cin.ignore();
			manager.viewTask(id);
			break;
		}

		case 0: 
			manager.saveToFile(storageFile);
			return;
		default:
			uiError("Неверный ввод\n");
		}
	}
}

void Application::handleAdd() {
	string title, desc, deadline;
	cin.ignore();

	while (true) {
		cout << "Введите название: ";
		getline(cin, title);

		if (title.empty() || isBlank(title)) {
			uiError("Название задачи не может быть пустым или состоять только из пробелов");
			continue;
		}

		title = trim(title);

		if (manager.titleExists(title)) {
			uiError("Задача с таким названием уже существует");
			continue;
		}

		break;
	}

	while (true) {
		cout << "Введите описание: ";
		getline(cin, desc);

		if (desc.empty() || isBlank(desc)) {
			uiError("Описание задачи не может быть пустым или состоять только из пробелов");
			continue;
		}

		break;
	}


	while (true) {
		cout << "Введите дедлайн (дд.мм.гггг): ";
		getline(cin, deadline);

		if (!manager.isValidDeadline(deadline)) {
			uiError("Некорректная дата или дата в прошлом.\n");
			continue;
		}

		break;
	}



	manager.addTask(title, desc, deadline, Status::ToDo);
	manager.saveToFile(storageFile);
	uiSuccess("Задача успешно добавлена\n");
}

void Application::handleList() {
	if (manager.isEmpty()) {
		uiError("Задач пока нет.Добавьте первую!\n");
		return;
	}

	manager.listTasks();
}


void Application::handleEdit() {
	string newTitle, newDesc, newDeadline;
	int id;

	cout << "Введите id задачи которую нужно изменить: ";
	cin >> id;

	cin.ignore();

	// Проверка существования задачи 
	 if (!manager.exists(id)) { 
		 uiError("Задача с таким ID не найдена\n");
		 return; 
	 }

	 while (true) {
		 cout << "Введите новое название: ";
		 getline(cin, newTitle);

		 if (newTitle.empty() || isBlank(newTitle)) {
			 uiError("Название задачи не может быть пустым или состоять только из пробелов");
			 continue;
		 }

		 break;
	 }

	 while (true) {
		 cout << "Введите новое описание: ";
		 getline(cin, newDesc);

		 if (newDesc.empty() || isBlank(newDesc)) {
			 uiError("Описание задачи не может быть пустым или состоять только из пробелов");
			 continue;
		 }

		 break;
	 }

	while (true) {
		cout << "Введите новый дедлайн (дд.мм.гггг): ";
		getline(cin, newDeadline);

		if (!manager.isValidDeadline(newDeadline)) {
			uiError("Некорректная дата или дата в прошлом.\n");
			continue;
		}

		break;
	}

	manager.editTask(id, newTitle, newDesc, newDeadline);
	manager.saveToFile(storageFile);
	uiSuccess("Задача успешно изменена\n");
	
}

void Application::handleRemove() {
	int id;

	cout << "Введите id задачи которую нужно удалить: ";
	cin >> id;

	if (manager.removeTask(id))
	{
		uiSuccess("Задача успешно удалена\n");
		manager.saveToFile(storageFile);
	}
	else {
		uiError("Задача с таким ID не найдена\n");
	}
}

void Application::handleChangeStatus() {
	int id;
	int newStatus;

	cout << "Введите id задачи статус которой нужно изменить: ";
	cin >> id;

	// Проверка существования задачи 
	if (!manager.exists(id)) {
		uiError("Задача с таким ID не найдена\n");
		return;
	}

	cout << "Введите номер статуса, который нужно присвоить данной задаче: \n0 - ToDo \n1 - inProgress \n2 - Done\n";
	cin >> newStatus;

	Status s = static_cast<Status>(newStatus);
	manager.changeStatus(id, s);
	manager.saveToFile(storageFile);
	uiSuccess("Статус успешно изменен\n");
}

void Application::handleSort() {
	cout << "Выберите тип сортировки:\n"
			"1. По ID\n"
			"2. По названию\n"
			"3. По дедлайну\n"
			"4. По статусу\n";

	int choice;
	cin >> choice;

	switch (choice) {
	case 1: manager.sortTasks(SortMode::ById); break;
	case 2: manager.sortTasks(SortMode::ByTitle); break;
	case 3: manager.sortTasks(SortMode::ByDeadLine); break;
	case 4: manager.sortTasks(SortMode::ByStatus); break;
	default:
		uiError("Неверный выбор\n");
		return;
	}

	uiSuccess("Сортировка успешно выполненна\n");
}

void Application::handleFilter() {
	cout << "Выберите фильтр:\n"
		"1. Все задачи\n"
		"2. Только ToDo\n"
		"3. Только InProgress\n"
		"4. Только Done\n";

	int choice;
	cin >> choice;

	switch (choice) {
	case 1: manager.listTasksFiltered(FilterMode::All); break;
	case 2: manager.listTasksFiltered(FilterMode::OnlyToDo); break;
	case 3: manager.listTasksFiltered(FilterMode::OnlyInProgress); break;
	case 4: manager.listTasksFiltered(FilterMode::OnlyDone); break;
	default:
		uiError("Неверный выбор\n");
	}
}
