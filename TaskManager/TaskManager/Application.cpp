#include "Application.hpp"
#include "UI.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

auto isBlank = [](const std::string& s) {
    return std::all_of(s.begin(), s.end(), [](unsigned char c) {
        return std::isspace(c);
    });
};


std::string trim(const std::string& s) {
	size_t start = s.find_first_not_of(" \t\n\r");
	if (start == std::string::npos)
		return ""; // ������ ������� ������ �� ��������

	size_t end = s.find_last_not_of(" \t\n\r");
	return s.substr(start, end - start + 1);
}

void Application::showMenu() const {

	uiHeader("TASK MANAGER");

	cout << "1 - �������� ������ \n"
		"2 - �������� ��� ������ \n"
		"3 - ������������� ������ \n"
		"4 - ������� ������ \n"
		"5 - �������� ������ ������\n"
		"6 - ���������� �����\n"
		"7 - ���������� �����\n"
		"8 � ����������� ������\n"
		"0 - �����\n";

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
			cout << "������� ID ������: ";
			cin >> id;
			cin.ignore();
			manager.viewTask(id);
			break;
		}

		case 0: 
			manager.saveToFile(storageFile);
			return;
		default:
			uiError("�������� ����\n");
		}
	}
}

void Application::handleAdd() {
	string title, desc, deadline;
	cin.ignore();

	while (true) {
		cout << "������� ��������: ";
		getline(cin, title);

		if (title.empty() || isBlank(title)) {
			uiError("�������� ������ �� ����� ���� ������ ��� �������� ������ �� ��������");
			continue;
		}

		title = trim(title);

		if (manager.titleExists(title)) {
			uiError("������ � ����� ��������� ��� ����������");
			continue;
		}

		break;
	}

	while (true) {
		cout << "������� ��������: ";
		getline(cin, desc);

		if (desc.empty() || isBlank(desc)) {
			uiError("�������� ������ �� ����� ���� ������ ��� �������� ������ �� ��������");
			continue;
		}

		break;
	}


	while (true) {
		cout << "������� ������� (��.��.����): ";
		getline(cin, deadline);

		if (!manager.isValidDeadline(deadline)) {
			uiError("������������ ���� ��� ���� � �������.\n");
			continue;
		}

		break;
	}



	manager.addTask(title, desc, deadline, Status::ToDo);
	manager.saveToFile(storageFile);
	uiSuccess("������ ������� ���������\n");
}

void Application::handleList() {
	if (manager.isEmpty()) {
		uiError("����� ���� ���.�������� ������!\n");
		return;
	}

	manager.listTasks();
}


void Application::handleEdit() {
	string newTitle, newDesc, newDeadline;
	int id;

	cout << "������� id ������ ������� ����� ��������: ";
	cin >> id;

	cin.ignore();

	// �������� ������������� ������ 
	 if (!manager.exists(id)) { 
		 uiError("������ � ����� ID �� �������\n");
		 return; 
	 }

	 while (true) {
		 cout << "������� ����� ��������: ";
		 getline(cin, newTitle);

		 if (newTitle.empty() || isBlank(newTitle)) {
			 uiError("�������� ������ �� ����� ���� ������ ��� �������� ������ �� ��������");
			 continue;
		 }

		 break;
	 }

	 while (true) {
		 cout << "������� ����� ��������: ";
		 getline(cin, newDesc);

		 if (newDesc.empty() || isBlank(newDesc)) {
			 uiError("�������� ������ �� ����� ���� ������ ��� �������� ������ �� ��������");
			 continue;
		 }

		 break;
	 }

	while (true) {
		cout << "������� ����� ������� (��.��.����): ";
		getline(cin, newDeadline);

		if (!manager.isValidDeadline(newDeadline)) {
			uiError("������������ ���� ��� ���� � �������.\n");
			continue;
		}

		break;
	}

	manager.editTask(id, newTitle, newDesc, newDeadline);
	manager.saveToFile(storageFile);
	uiSuccess("������ ������� ��������\n");
	
}

void Application::handleRemove() {
	int id;

	cout << "������� id ������ ������� ����� �������: ";
	cin >> id;

	if (manager.removeTask(id))
	{
		uiSuccess("������ ������� �������\n");
		manager.saveToFile(storageFile);
	}
	else {
		uiError("������ � ����� ID �� �������\n");
	}
}

void Application::handleChangeStatus() {
	int id;
	int newStatus;

	cout << "������� id ������ ������ ������� ����� ��������: ";
	cin >> id;

	// �������� ������������� ������ 
	if (!manager.exists(id)) {
		uiError("������ � ����� ID �� �������\n");
		return;
	}

	cout << "������� ����� �������, ������� ����� ��������� ������ ������: \n0 - ToDo \n1 - inProgress \n2 - Done\n";
	cin >> newStatus;

	Status s = static_cast<Status>(newStatus);
	manager.changeStatus(id, s);
	manager.saveToFile(storageFile);
	uiSuccess("������ ������� �������\n");
}

void Application::handleSort() {
	cout << "�������� ��� ����������:\n"
			"1. �� ID\n"
			"2. �� ��������\n"
			"3. �� ��������\n"
			"4. �� �������\n";

	int choice;
	cin >> choice;

	switch (choice) {
	case 1: manager.sortTasks(SortMode::ById); break;
	case 2: manager.sortTasks(SortMode::ByTitle); break;
	case 3: manager.sortTasks(SortMode::ByDeadLine); break;
	case 4: manager.sortTasks(SortMode::ByStatus); break;
	default:
		uiError("�������� �����\n");
		return;
	}

	uiSuccess("���������� ������� ����������\n");
}

void Application::handleFilter() {
	cout << "�������� ������:\n"
		"1. ��� ������\n"
		"2. ������ ToDo\n"
		"3. ������ InProgress\n"
		"4. ������ Done\n";

	int choice;
	cin >> choice;

	switch (choice) {
	case 1: manager.listTasksFiltered(FilterMode::All); break;
	case 2: manager.listTasksFiltered(FilterMode::OnlyToDo); break;
	case 3: manager.listTasksFiltered(FilterMode::OnlyInProgress); break;
	case 4: manager.listTasksFiltered(FilterMode::OnlyDone); break;
	default:
		uiError("�������� �����\n");
	}
}