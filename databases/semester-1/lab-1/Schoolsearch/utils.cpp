#include "utils.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<Student> loadStudents(const std::string& filename) {
    std::vector<Student> students;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << filename << std::endl;
        return students;
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string lastName, firstName, teacherLastName, teacherFirstName;
        int grade, classroom, bus;

        std::getline(ss, lastName, ',');
        std::getline(ss, firstName, ',');
        ss >> grade;
        ss.ignore();
        ss >> classroom;
        ss.ignore();
        ss >> bus;
        ss.ignore();
        std::getline(ss, teacherLastName, ',');
        std::getline(ss, teacherFirstName, ',');

        students.push_back({lastName, firstName, grade, classroom, bus, teacherLastName, teacherFirstName});
    }

    return students;
}
