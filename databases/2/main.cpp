#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <set>

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

struct Student {
    std::string lastName;
    std::string firstName;
    int grade;
    int classroom;
    int bus;
};

struct Teacher {
    std::string lastName;
    std::string firstName;
    int classroom;
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
        students.push_back(student);
    }

    return students;
}

std::multimap<int, Teacher> loadTeachers(const std::string& filename) {
    std::multimap<int, Teacher> teachers;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
        return teachers;
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Teacher teacher;
        std::getline(ss, teacher.lastName, ',');
        std::getline(ss, teacher.firstName, ',');
        int classroom;
        ss >> classroom;
        teachers.insert({classroom, teacher});
    }

    return teachers;
}

void saveStudents(const std::vector<Student>& students, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл для записи: " << filename << std::endl;
        return;
    }
    for (const auto& student : students) {
        file << student.lastName << ", " << student.firstName << ", "
             << student.grade << ", " << student.classroom << ", " << student.bus << "\n";
    }
}

void saveTeachers(const std::multimap<int, Teacher>& teachers, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл для записи: " << filename << std::endl;
        return;
    }
    for (const auto& teacher : teachers) {
        file << teacher.second.lastName << ", " << teacher.second.firstName << ", " << teacher.first << "\n";
    }
}

void addStudent(std::vector<Student>& students) {
    Student newStudent;
    std::cout << "Enter last name: ";
    std::cin >> newStudent.lastName;
    newStudent.lastName = trim(newStudent.lastName);
    std::cout << "Enter first name: ";
    std::cin >> newStudent.firstName;
    newStudent.firstName = trim(newStudent.firstName);
    std::cout << "Enter grade: ";
    std::cin >> newStudent.grade;
    std::cout << "Enter classroom: ";
    std::cin >> newStudent.classroom;
    std::cout << "Enter bus number: ";
    std::cin >> newStudent.bus;
    students.push_back(newStudent);
    std::cin.ignore();
}

void deleteStudent(std::vector<Student>& students, const std::string& lastName, const std::string& firstName) {
    auto it = std::remove_if(students.begin(), students.end(), [&lastName, &firstName](const Student& student) {
        return trim(student.lastName) == trim(lastName) && trim(student.firstName) == trim(firstName);
    });
    if (it != students.end()) {
        students.erase(it, students.end());
        std::cout << "Student deleted.\n";
    } else {
        std::cout << "Student not found.\n";
    }
}

void editStudent(std::vector<Student>& students, const std::string& lastName) {
    auto it = std::find_if(students.begin(), students.end(), [&lastName](const Student& student) {
        return trim(student.lastName) == trim(lastName);
    });
    if (it != students.end()) {
        std::cout << "Editing student " << it->lastName << ", " << it->firstName << "\n";
        std::cout << "Enter new grade: ";
        std::cin >> it->grade;
        std::cout << "Enter new classroom: ";
        std::cin >> it->classroom;
        std::cout << "Enter new bus number: ";
        std::cin >> it->bus;
    } else {
        std::cout << "Student not found.\n";
    }
}

void addTeacher(std::multimap<int, Teacher>& teachers) {
    Teacher newTeacher;
    std::cout << "Enter last name: ";
    std::cin >> newTeacher.lastName;
    newTeacher.lastName = trim(newTeacher.lastName);
    std::cout << "Enter first name: ";
    std::cin >> newTeacher.firstName;
    newTeacher.firstName = trim(newTeacher.firstName);
    std::cout << "Enter classroom: ";
    std::cin >> newTeacher.classroom;
    teachers.insert({newTeacher.classroom, newTeacher});
}

void deleteTeacher(std::multimap<int, Teacher>& teachers, const std::string& lastName, const std::string& firstName) {
    auto it = std::find_if(teachers.begin(), teachers.end(), [&lastName, &firstName](const std::pair<int, Teacher>& teacher) {
        return trim(teacher.second.lastName) == trim(lastName) && trim(teacher.second.firstName) == trim(firstName);
    });
    if (it != teachers.end()) {
        teachers.erase(it);
        std::cout << "Teacher deleted.\n";
    } else {
        std::cout << "Teacher not found.\n";
    }
}

void editTeacher(std::multimap<int, Teacher>& teachers, const std::string& lastName, std::vector<Student>& students, const std::string& teachersFile) {
    auto it = std::find_if(teachers.begin(), teachers.end(), [&lastName](const std::pair<int, Teacher>& teacher) {
        return trim(teacher.second.lastName) == trim(lastName);
    });

    if (it != teachers.end()) {
        int oldClassroom = it->first;
        Teacher updatedTeacher = it->second;

        std::cout << "Editing teacher " << it->second.lastName << ", " << it->second.firstName << "\n";
        std::cout << "Enter new classroom: ";
        std::cin >> updatedTeacher.classroom;
        teachers.erase(it);
        teachers.insert({updatedTeacher.classroom, updatedTeacher});
        for (auto& student : students) {
            if (student.classroom == oldClassroom) {
                student.classroom = updatedTeacher.classroom;
            }
        }
        saveTeachers(teachers, teachersFile);
        std::cout << "Teacher and students updated.\n";
    } else {
        std::cout << "Teacher not found.\n";
    }
}


void searchByStudent(const std::vector<Student>& students, const std::string& lastName, const std::multimap<int, Teacher>& teachers) {
    bool found = false;
    for (const auto& student : students) {
        if (trim(student.lastName) == trim(lastName)) {
            auto range = teachers.equal_range(student.classroom);
            std::cout << student.lastName << ", " << student.firstName
                      << " - Grade: " << student.grade
                      << ", Classroom: " << student.classroom
                      << ", Bus: " << student.bus
                      << ", Teachers: ";
            for (auto it = range.first; it != range.second; ++it) {
                std::cout << it->second.lastName << " " << it->second.firstName << "; ";
            }
            std::cout << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No students found with last name: " << lastName << std::endl;
    }
}

void searchByBus(const std::vector<Student>& students, int busNumber) {
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
}

void searchByClassroom(const std::vector<Student>& students, int classroom) {
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
}

void searchTeacherByClassroom(const std::multimap<int, Teacher>& teachers, int classroom) {
    auto range = teachers.equal_range(classroom);
    if (range.first != range.second) {
        for (auto it = range.first; it != range.second; ++it) {
            std::cout << it->second.lastName << " " << it->second.firstName
                      << " - Classroom: " << classroom << std::endl;
        }
    } else {
        std::cout << "No teachers found for classroom: " << classroom << std::endl;
    }
}

void searchTeachersByGrade(const std::vector<Student>& students, const std::multimap<int, Teacher>& teachers, int grade) {
    std::set<std::string> displayedTeachers;
    for (const auto& student : students) {
        if (student.grade == grade) {
            auto range = teachers.equal_range(student.classroom);
            for (auto it = range.first; it != range.second; ++it) {
                std::string fullName = it->second.lastName + " " + it->second.firstName;
                if (displayedTeachers.find(fullName) == displayedTeachers.end()) {
                    std::cout << it->second.lastName << " " << it->second.firstName << std::endl;
                    displayedTeachers.insert(fullName);
                }
            }
        }
    }

    if (displayedTeachers.empty()) {
        std::cout << "No teachers found for grade: " << grade << std::endl;
    }
}

void searchByGrade(const std::vector<Student>& students, int grade) {
    bool found = false;
    for (const auto& student : students) {
        if (student.grade == grade) {
            std::cout << student.lastName << ", " << student.firstName
                      << " - Grade: " << student.grade
                      << ", Classroom: " << student.classroom
                      << ", Bus: " << student.bus
                      << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No students found for grade: " << grade << std::endl;
    }
}

void searchStudentsByTeacher(const std::vector<Student>& students, const std::multimap<int, Teacher>& teachers, const std::string& fullName) {
    bool found = false;

    for (const auto& teacher : teachers) {
        std::string teacherFullName = teacher.second.lastName + " " + teacher.second.firstName;

        if (teacherFullName == fullName) {
            for (const auto& student : students) {
                if (student.classroom == teacher.first) {
                    std::cout << student.lastName << ", " << student.firstName
                              << " - Grade: " << student.grade
                              << ", Classroom: " << student.classroom
                              << ", Bus: " << student.bus
                              << std::endl;
                    found = true;
                }
            }
        }
    }

    if (!found) {
        std::cout << "No students found for teacher: " << fullName << std::endl;
    }
}

void commandLoop(std::vector<Student>& students, std::multimap<int, Teacher>& teachers, const std::string& studentsFile, const std::string& teachersFile) {
    std::string command;
    std::cout << "Enter command: ";

    while (std::getline(std::cin, command)) {
        if (command == "Q" || command == "Quit") break;

        if (command[0] == 'S') {
            std::string lastName = command.substr(2);
            searchByStudent(students, lastName, teachers);
        } else if (command[0] == 'B') {
            int busNumber = std::stoi(command.substr(2));
            searchByBus(students, busNumber);
        } else if (command[0] == 'C' && command.back() == 'T') {
            int classroom = std::stoi(command.substr(2, command.size() - 3));
            searchTeacherByClassroom(teachers, classroom);
        } else if (command[0] == 'C') {
            int classroom = std::stoi(command.substr(2));
            searchByClassroom(students, classroom);
        } else if (command[0] == 'G' && command.back() == 'T') {
            int grade = std::stoi(command.substr(2, command.size() - 3));
            searchTeachersByGrade(students, teachers, grade);
        } else if (command[0] == 'G') {
            int grade = std::stoi(command.substr(2));
            searchByGrade(students, grade);
        } else if (command[0] == 'T') {
            std::string fullName = command.substr(2);
            searchStudentsByTeacher(students, teachers, fullName);
        } else if (command == "ADD S") {
            addStudent(students);
            saveStudents(students, studentsFile);
        } else if (command == "DEL S") {
            std::string lastName, firstName;
            std::cout << "Enter last name: ";
            std::cin >> lastName;
            std::cout << "Enter first name: ";
            std::cin >> firstName;
            deleteStudent(students, lastName, firstName);
            saveStudents(students, studentsFile);
            std::cin.ignore();
        } else if (command == "EDIT S") {
            std::string lastName;
            std::cout << "Enter last name: ";
            std::cin >> lastName;
            editStudent(students, lastName);
            saveStudents(students, studentsFile);
            std::cin.ignore();
        } else if (command == "ADD T") {
            addTeacher(teachers);
            saveTeachers(teachers, teachersFile);
            std::cin.ignore();
        } else if (command == "DEL T") {
            std::string lastName, firstName;
            std::cout << "Enter last name: ";
            std::cin >> lastName;
            std::cout << "Enter first name: ";
            std::cin >> firstName;
            deleteTeacher(teachers, lastName, firstName);
            saveTeachers(teachers, teachersFile);
            std::cin.ignore();
        } else if (command == "EDIT T") {
            std::string lastName;
            std::cout << "Enter last name: ";
            std::cin >> lastName;
            editTeacher(teachers, lastName, students, teachersFile);
            saveStudents(students, studentsFile);
            std::cin.ignore();
        } else {
            std::cout << "Error! Invalid command!" << std::endl;
        }
        std::cout << "Enter command: ";
    }
}

int main() {
    auto students = loadStudents("list.txt");
    auto teachers = loadTeachers("teachers.txt");

    if (students.empty() || teachers.empty()) {
        std::cout << "Error loading data." << std::endl;
        return 1;
    }

    commandLoop(students, teachers, "list.txt", "teachers.txt");
    std::cout << "bye!" << std::endl;
    return 0;
}
