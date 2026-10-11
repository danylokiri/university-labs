#include "student.h"
#include "utils.h"
#include <iostream>
#include <string>

void commandLoop(const std::vector<Student>& students) {
    std::string command;
    std::cout << "Enter command: ";

    while (std::getline(std::cin, command)) {
        if (command == "Q" || command == "Quit") {
            break;
        } else if (command.substr(0, 1) == "S") {
            std::string lastName;
            bool isBusCommand = false;

            size_t busPos = command.find("Bus");
            if (busPos != std::string::npos) {
                lastName = command.substr(2, busPos - 3);
                isBusCommand = true;
            } else {
                lastName = command.substr(2);
            }

            if (isBusCommand) {
                searchByStudentBus(students, lastName);
            } else {
                searchByStudent(students, lastName);
            }
        } else if (command.substr(0, 1) == "T") {
            std::string lastName = command.substr(2);
            searchByTeacher(students, lastName);
        } else if (command.substr(0, 1) == "C") {
            int classroom = std::stoi(command.substr(2));
            searchByClassroom(students, classroom);
        } else if (command.substr(0, 1) == "B") {
            int busNumber = std::stoi(command.substr(2));
            searchByBus(students, busNumber);
        }
        else {
            std::cout << "Error! Invalid command!" << std::endl;
        }
        std::cout << "Enter command: ";
    }
}

int main() {
    std::vector<Student> students = loadStudents("students.txt");

    if (students.empty()) {
        std::cout << "No students loaded." << std::endl;
        return 1;
    }

    commandLoop(students);
    return 0;
}
