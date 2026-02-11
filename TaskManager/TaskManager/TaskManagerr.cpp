#include "TaskManager.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "UI.hpp"


void parseDate(const std::string& date, int& day, int& month, int& year) {
	day = stoi(date.substr(0, 2));
	month = stoi(date.substr(3, 2));
	year = stoi(date.substr(6, 4));
}

int dateToInt(const std::string& date) {
	int day, month, year; 
	parseDate(date, day, month, year);

	return year * 10000 + month * 100 + day;
}


bool isValidDateFormat(const std::string& date) {
	if (date.size() != 10) return false;
	if (date[2] != '.' || date[5] != '.') return false;

	for (int i = 0; i < date.size(); i++) {
		if (i == 2 || i == 5) continue;
		if (!isdigit(date[i])) return false;
	}

	int day, month, year;
	parseDate(date, day, month, year);

	int visokos = 0;
	if (year % 400 == 0) ++visokos;
	else if (year % 100 == 0) visokos = 0;
	else if (year % 4 == 0) ++visokos;
	else visokos = 0;
	

	if (month < 1 || month > 12) return false;
	if (day < 1 || day > 31) return false;

	static int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (day > (visokos + daysInMonth[month - 1])) return false;

	return true;
}

void getToday(int& d, int& m, int& y) {
	time_t now = time(nullptr);
	tm local{};

	#ifdef _WIN32
	localtime_s(&local, &now);
	#else
	localtime_r(&now, &local);
	#endif


	d = local.tm_mday;
	m = local.tm_mon + 1;
	y = local.tm_year + 1900;
}

bool isDeadlineNotInPast(const std::string& date) {
	int day, month, year;
	parseDate(date, day, month, year);

	int td, tm, ty;
	getToday(td, tm, ty);

	if (year < ty) return false;
	if (year > ty) return true;

	if (month < tm) return false;
	if (month > tm) return true;

	return day >= td;
}

bool TaskManager::isValidDeadline(const string& date) const {
	return isValidDateFormat(date) && isDeadlineNotInPast(date);
}

bool TaskManager::titleExists(const std::string& title) const {
	for (const auto& task : tasks) {
		if (task.getTitle() == title)
			return true;
	}
	return false;
}

void TaskManager::addTask(const string& title, const string& description, const string& deadline, Status status) {
	tasks.emplace_back(nextId, title, description, deadline, status);
	nextId++;

}

string statusToString(Status s) {
	switch (s) {
	case Status::ToDo:        return "ToDo";
	case Status::InProgress:  return "InProgress";
	case Status::Done:        return "Done";
	}
	return "Unknown";
}

void TaskManager::listTasks() const {
	uiHeader("������ �����");

	uiTableRow("ID", "������", "��������", "�������", "�������");
	uiLine(70);

	for (const auto& t : tasks) {
		uiTableRow(
			std::to_string(t.getId()),
			statusToString(t.getStatus()),
			t.getTitle(),
			t.getDeadline(),
			t.getCreatedDate()
		);
	}

}

void TaskManager::viewTask(int id) const {
	for (const auto& task : tasks) {
		if (task.getId() == id) {

			uiHeader("�������� ������");

			uiTableRow("ID", "������", "��������", "�������", "�������");
			uiLine(60);

			uiTableRow(
				std::to_string(task.getId()),
				statusToString(task.getStatus()),
				task.getTitle(),
				task.getDeadline(),
				task.getCreatedDate()
			);

			uiLine(60);

			// �������� � ��������� ������
			uiSection("��������");
			std::cout << task.getDescription() << "\n";

			uiLine(60);
			return;
		}
	}

	uiError("������ � ����� ID �� �������");
}

bool TaskManager::removeTask(int id) {
	for (auto it = tasks.begin(); it != tasks.end(); ++it) {
		if (it->getId() == id) {
			tasks.erase(it);
			return true;
		}
	}
	return false;
}

void TaskManager::editTask(int id, const std::string& newTitle, const std::string& newDescription, const std::string& newDeadline) {
	for (auto& t : tasks) {
		if (t.getId() == id) {
			t.setTitle(newTitle);
			t.setDescription(newDescription);
			t.setDeadline(newDeadline);
			return;
		}
	}
	return;
}

bool TaskManager::changeStatus(int id, Status newStatus) {
	for (auto& t : tasks) {
		if (t.getId() == id) {
			t.setStatus(newStatus);
			return true;
		}
	}
	return false;
}

bool TaskManager::exists(int id) const {
	for (const auto& task : tasks) {
		if (task.getId() == id)
			return true;
	}
	return false;
}


bool TaskManager::isEmpty() const {
	return tasks.empty();
}



Status stringToStatus(const std::string& s) {
	if (s == "ToDo") return Status::ToDo;
	if (s == "InProgress") return Status::InProgress;
	if (s == "Done") return Status::Done;
	return Status::ToDo; // fallback
}

void TaskManager::saveToFile(const std::string& filename) const {
	std::ofstream file(filename);
	if (!file.is_open()) return;

	for (const auto& task : tasks) {
		file << "ID: " << task.getId() << "\n";
		file << "Status: " << statusToString(task.getStatus()) << "\n";
		file << "Title: " << task.getTitle() << "\n";
		file << "Description: " << task.getDescription() << "\n";
		file << "Deadline: " << task.getDeadline() << "\n";
		file << "---\n";
	}
}


void TaskManager::loadFromFile(const std::string& filename) {
	ifstream file(filename);
	if (!file.is_open()) return;

	tasks.clear();

	string line;
	int id = 0;
	string status, title, desc, deadline;

	while (getline(file, line)) {
		if (line.rfind("ID:", 0) == 0) {
			id = stoi(line.substr(4));
		}
		else if (line.rfind("Status:", 0) == 0) {
			status = line.substr(8);
		}
		else if (line.rfind("Title:", 0) == 0) {
			title = line.substr(7);
		}
		else if (line.rfind("Description:", 0) == 0) {
			desc = line.substr(13);
		}
		else if (line.rfind("Deadline:", 0) == 0) {
			deadline = line.substr(10);
		}
		else if (line == "---") {
			tasks.emplace_back(
				id,
				title,
				desc,
				deadline,
				stringToStatus(status)
			);
		}
	}

	int maxId = 0;
	for (const auto& task : tasks) {
		if (task.getId() > maxId) {
			maxId = task.getId();
		}
	}

	nextId = maxId + 1;
}


void TaskManager::sortTasks(SortMode mode) {
	switch (mode) {
	case SortMode::ById:
		std::sort(tasks.begin(), tasks.end(),
			[](const Task& a, const Task& b) {
				return a.getId() < b.getId();
			});
		break;

	case SortMode::ByTitle:
		std::sort(tasks.begin(), tasks.end(),
			[](const Task& a, const Task& b) {
				return a.getTitle() < b.getTitle();
			});
		break;

	case SortMode::ByDeadLine:
		std::sort(tasks.begin(), tasks.end(),
			[](const Task& a, const Task& b) {
				return dateToInt(a.getDeadline()) < dateToInt(b.getDeadline());
			});
		break;

	case SortMode::ByStatus:
		std::sort(tasks.begin(), tasks.end(),
			[](const Task& a, const Task& b) {
				return static_cast<int>(a.getStatus()) < static_cast<int>(b.getStatus());
			});
		break;

	}
}

void TaskManager::listTasksFiltered(FilterMode mode) const {
	if (tasks.empty()) {
		std::cout << "����� ���� ���.\n";
		return;
	}

	for (const auto& task : tasks) {
		bool show = false;

		switch (mode) {
		case FilterMode::All:
			show = true;
			break;
		case FilterMode::OnlyToDo:
			show = (task.getStatus() == Status::ToDo);
			break;
		case FilterMode::OnlyInProgress:
			show = (task.getStatus() == Status::InProgress);
			break;
		case FilterMode::OnlyDone:
			show = (task.getStatus() == Status::Done);
			break;
		}

		if (show)
			task.print();
	}
}

