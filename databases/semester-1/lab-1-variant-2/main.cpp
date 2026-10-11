#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

struct Student {
    std::string lastName;
    std::string firstName;
    int grade;
    int classroom;
    int bus;
    std::string teacherLastName;
    std::string teacherFirstName;
};

std::vector<Student> loadStudents(const std::string& filename) {
    std::vector<Student> students;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
        return students;
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Student student;
        std::getline(ss, student.lastName, ',');
        std::getline(ss, student.firstName, ',');
        ss >> student.grade;
        ss.ignore();
        ss >> student.classroom;
        ss.ignore();
        ss >> student.bus;
        ss.ignore();
        std::getline(ss, student.teacherLastName, ',');
        std::getline(ss, student.teacherFirstName, ',');
        students.push_back(student);
    }

    return students;
}

void searchByStudent(const std::vector<Student>& students, const std::string& lastName) {
    auto start = std::chrono::high_resolution_clock::now();
    bool found = false;

    for (const auto& student : students) {
        if (student.lastName == lastName) {
            std::cout << student.lastName << ", " << student.firstName
                      << " - Grade: " << student.grade
                      << ", Classroom: " << student.classroom
                      << ", Teacher: " << student.teacherLastName << ", " << student.teacherFirstName
                      << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No students found with last name: " << lastName << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> searchTime = end - start;
    std::cout << "Search Time: " << searchTime.count() << " seconds" << std::endl;
}

void searchByStudentBus(const std::vector<Student>& students, const std::string& lastName) {
    auto start = std::chrono::high_resolution_clock::now();
    bool found = false;

    for (const auto& student : students) {
        if (student.lastName == lastName) {
            std::cout << student.lastName << ", " << student.firstName
                      << " - Bus: " << student.bus
                      << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No students found with last name: " << lastName << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> searchTime = end - start;
    std::cout << "Search Time: " << searchTime.count() << " seconds" << std::endl;
}

void searchByTeacher(const std::vector<Student>& students, const std::string& teacherLastName) {
    auto start = std::chrono::high_resolution_clock::now();
    bool found = false;

    for (const auto& student : students) {
        if (student.teacherLastName == teacherLastName) {
            std::cout << student.lastName << ", " << student.firstName
                      << " - Teacher: " << student.teacherLastName << ", " << student.teacherFirstName
                      << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No students found with teacher last name: " << teacherLastName << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> searchTime = end - start;
    std::cout << "Search Time: " << searchTime.count() << " seconds" << std::endl;
}

void searchByClassroom(const std::vector<Student>& students, int classroom) {
    auto start = std::chrono::high_resolution_clock::now();
    bool found = false;

    for (const auto& student : students) {
        if (student.classroom == classroom) {
            std::cout << student.lastName << ", " << student.firstName
                      << " - Classroom: " << student.classroom
                      << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No students found in classroom: " << classroom << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> searchTime = end - start;
    std::cout << "Search Time: " << searchTime.count() << " seconds" << std::endl;
}

void searchByBus(const std::vector<Student>& students, int busNumber) {
    auto start = std::chrono::high_resolution_clock::now();
    bool found = false;

    for (const auto& student : students) {
        if (student.bus == busNumber) {
            std::cout << student.lastName << ", " << student.firstName
                      << " - Bus: " << student.bus
                      << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No students found on bus number: " << busNumber << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> searchTime = end - start;
    std::cout << "Search Time: " << searchTime.count() << " seconds" << std::endl;
}

void commandLoop(const std::vector<Student>& students) {
    std::string command;
    std::cout << "Enter command: ";

    while (std::getline(std::cin, command)) {
        if (command == "Q" || command == "Quit") break;

        if (command.substr(0, 6) == "S Bus ") {
            std::string lastName = command.substr(6);
            searchByStudentBus(students, lastName);
        } else if (command[0] == 'S') {
            std::string lastName = command.substr(2);
            searchByStudent(students, lastName);
        } else if (command[0] == 'T') {
            searchByTeacher(students, command.substr(2));
        } else if (command[0] == 'C') {
            searchByClassroom(students, std::stoi(command.substr(2)));
        } else if (command[0] == 'B') {
            searchByBus(students, std::stoi(command.substr(2)));
        } else {
            std::cout << "Error! Invalid command!" << std::endl;
        }
        std::cout << "Enter command: ";
    }
}

int main() {
    auto students = loadStudents("students.txt");
    if (students.empty()) {
        std::cout << "No students loaded." << std::endl;
        return 1;
    }
    commandLoop(students);
    return 0;
}
