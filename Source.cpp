#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct tasks {
	string arr[3];
};

void AddTask(string line, fstream& file) {
	file.open("textFile.txt", fstream::app | fstream::in | fstream::out);

	int quatesIndex;
	int counter = 1;
	string newLine, temp, fileLine;
	struct tasks task;
	newLine = line.substr(1);

	for (int i = 0; i < 3; i++) {
		quatesIndex = newLine.find("\"");
		temp = newLine.substr(0, quatesIndex);

		if (i < 2) {
			newLine = newLine.substr(quatesIndex + 3);
		}

		task.arr[i] = temp;
	}

	while (getline(file, fileLine)) {
		counter++;
	}

	file.clear();
	file.seekg(0);

	file << counter << ". " << task.arr[0] << " on " << task.arr[1] << " at " << task.arr[2] << endl;
	cout << "Task " << task.arr[0] << " added successfully." << endl;

	file.close();
}

void listTasks(fstream& file) {
	file.open("textFile.txt");

	if (file.is_open()) {
		string fileLine;

		while (getline(file, fileLine)) {
			cout << fileLine << endl;
		}

	} else {
		cout << "Unable to open file" << endl;
	}

	file.close();
}

void removeTask(string taskIndex, fstream& file) {
	taskIndex = taskIndex.substr(0, 1);
	file.open("textFile.txt");

	ofstream temp;
	temp.open("temp.txt");

	if (file.is_open()) {
		string fileLine, number;
		int counter = 1;
		int pointIndex;
		bool isRemoved = false;

		while (getline(file, fileLine)) {
			pointIndex = fileLine.find(".");
			number = fileLine.substr(0, pointIndex);

			if (number != taskIndex) {
				fileLine = fileLine.substr(pointIndex);
				temp << counter << fileLine << endl;

				counter++;
			} else {
				fileLine = fileLine.substr(fileLine.find(" ") + 1);
				fileLine = fileLine.substr(0, fileLine.find(" on "));
				cout << "Task " << fileLine << " removed successfully." << endl;
				isRemoved = true;
			}
		}

		if (!isRemoved) {
			cout << "Invalid task index." << endl;
		}

		temp.close();
		file.close();
		remove("textFile.txt");
		rename("temp.txt", "textFile.txt");
	}
	else {
		cout << "Unable to open file" << endl;
	}
}

void findTask(string partialName, fstream& file) {
	file.open("textFile.txt");
	partialName = partialName.substr(1, partialName.length() - 3);
	int isFound = 0;
	string fileLine;

	while (getline(file, fileLine)) {
		isFound = fileLine.find(partialName);

		if (isFound != -1) {
			cout << fileLine << endl;
		}
	}

	file.close();
}

void help() {
	cout << "Simple Pesonal Task Management." << endl;
	cout << "Available commands:" << endl;
	cout << "add_task(<task name>, <date>, <time>)" << endl;
	cout << "list_tasks()" << endl;
	cout << "find_task(<partial name>)" << endl;
	cout << "remove_task(<task index>)" << endl;
	cout << "help()" << endl;
	cout << "exit()" << endl;
}

bool compareDates(const tasks& event1, const tasks& event2) {
	string date1 = event1.arr[1].substr(6, 4) + event1.arr[1].substr(3, 2) + event1.arr[1].substr(0, 2);
	string hours1 = event1.arr[2].substr(0, 2) + event1.arr[2].substr(3, 2);
	string date2 = event2.arr[1].substr(6, 4) + event2.arr[1].substr(3, 2) + event2.arr[1].substr(0, 2);
	string hours2 = event2.arr[2].substr(0, 2) + event2.arr[2].substr(3, 2);

	int iHours1 = stoi(hours1);
	int iHours2 = stoi(hours2);

	if (date1 != date2)
		return date1 < date2;

	return hours1 < hours2;
}

void sort(fstream& file) {
	file.open("textFile.txt");

	ofstream temp;
	temp.open("temp.txt");

	string fileLine;
	int counter = 1;

	vector<tasks> events;

	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			tasks task;
			task.arr[0] = line.substr(line.find_first_of('.') + 2);
			task.arr[1] = line.substr(line.find("on ") + 3, 10);
			task.arr[2] = line.substr(line.find("at ") + 3, 5); 

			events.push_back(task);
		}

		file.close();

		sort(events.begin(), events.end(), compareDates);

		if (temp.is_open()) {
			for (const tasks& event : events) {
				temp << counter << ". " << event.arr[0] << endl;
				counter++;
			}

			temp.close();
			remove("textFile.txt");
			rename("temp.txt", "textFile.txt");
		} else {
			cout << "Unable to open file" << endl;
		}
	} else {
		cout << "Unable to open file" << endl;
	}
}

int main() {
	struct tasks task;
	fstream file;

	string text, command, line;

	while (text != "exit()") {
		getline(std::cin, text);

		int bracketIndex = text.find("(");
		command = text.substr(0, bracketIndex);
		if (text.find("(") == -1) {
			cout << "Command \"" << text << "\" not found!" << endl;
			continue;
		}
		line = text.substr(bracketIndex + 1);

		if (command == "add_task") {
			AddTask(line, file);
		} else if (command == "list_tasks") {
			listTasks(file);
		} else if (command == "find_task") {
			findTask(line, file);
		} else if (command == "remove_task") {
			removeTask(line, file);
		} else if (command == "help"){
			help();
		} else {
			cout << "Command \"" << text << "\" not found!" << endl;
		}
		sort(file);
	}

	cout << "Bye!" << endl;

	system("PAUSE");
	return 0;
}