#include "student.h"
#include <iostream>
#include <chrono>
#include <map>

void searchByStudent(const std::vector<Student>& students, const std::string& lastName) {
    auto start = std::chrono::high_resolution_clock::now();
    std::map<std::pair<std::string, std::string>, int> studentCount;

    for (int i = 0; i < students.size(); i++) {
        if (students[i].lastName == lastName) {
            studentCount[std::make_pair(students[i].lastName, students[i].firstName)]++;
        }
    }

    if (studentCount.size() == 0) {
        std::cout << "No students found with last name: " << lastName << std::endl;
    } else {
        for (auto it = studentCount.begin(); it != studentCount.end(); it++) {
            for (int j = 0; j < students.size(); j++) {
                if (students[j].lastName == it->first.first && students[j].firstName == it->first.second) {
                    std::cout << students[j].lastName << ", " << students[j].firstName
                              << " - Grade: " << students[j].grade
                              << ", Classroom: " << students[j].classroom
                              << ", Teacher: " << students[j].teacherLastName << ", " << students[j].teacherFirstName
                              << " - Found " << it->second << " time(s)" << std::endl;
                    break;
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> searchTime = end - start;
    std::cout << "Search Time: " << searchTime.count() << " seconds" << std::endl;
}

void searchByStudentBus(const std::vector<Student>& students, const std::string& lastName) {
    auto start = std::chrono::high_resolution_clock::now();
    std::map<std::pair<std::string, std::string>, int> studentCount;

    for (int i = 0; i < students.size(); i++) {
        if (students[i].lastName == lastName) {
            studentCount[std::make_pair(students[i].lastName, students[i].firstName)]++;
        }
    }

    if (studentCount.size() == 0) {
        std::cout << "No students found with last name: " << lastName << std::endl;
    } else {
        for (auto it = studentCount.begin(); it != studentCount.end(); it++) {
            for (int j = 0; j < students.size(); j++) {
                if (students[j].lastName == it->first.first && students[j].firstName == it->first.second) {
                    std::cout << students[j].lastName << ", " << students[j].firstName
                              << " - Bus: " << students[j].bus
                              << " - Found " << it->second << " time(s)" << std::endl;
                    break;
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> searchTime = end - start;
    std::cout << "Search Time: " << searchTime.count() << " seconds" << std::endl;
}

void searchByTeacher(const std::vector<Student>& students, const std::string& teacherLastName) {
    auto start = std::chrono::high_resolution_clock::now();
    std::map<std::pair<std::string, std::string>, int> studentCount;

    for (int i = 0; i < students.size(); i++) {
        if (students[i].teacherLastName == teacherLastName) {
            studentCount[std::make_pair(students[i].lastName, students[i].firstName)]++;
        }
    }

    if (studentCount.size() == 0) {
        std::cout << "No students found with teacher last name: " << teacherLastName << std::endl;
    } else {
        for (auto it = studentCount.begin(); it != studentCount.end(); it++) {
            for (int j = 0; j < students.size(); j++) {
                if (students[j].lastName == it->first.first && students[j].firstName == it->first.second) {
                    std::cout << students[j].lastName << ", " << students[j].firstName
                              << " - Found " << it->second << " time(s)" << std::endl;
                    break;
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> searchTime = end - start;
    std::cout << "Search Time: " << searchTime.count() << " seconds" << std::endl;
}

void searchByClassroom(const std::vector<Student>& students, int classroom) {
    auto start = std::chrono::high_resolution_clock::now();
    std::map<std::pair<std::string, std::string>, int> studentCount;

    for (int i = 0; i < students.size(); i++) {
        if (students[i].classroom == classroom) {
            studentCount[std::make_pair(students[i].lastName, students[i].firstName)]++;
        }
    }

    if (studentCount.size() == 0) {
        std::cout << "No students found in classroom: " << classroom << std::endl;
    } else {
        for (auto it = studentCount.begin(); it != studentCount.end(); it++) {
            for (int j = 0; j < students.size(); j++) {
                if (students[j].lastName == it->first.first && students[j].firstName == it->first.second) {
                    std::cout << students[j].lastName << ", " << students[j].firstName
                              << " - Found " << it->second << " time(s)" << std::endl;
                    break;
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> searchTime = end - start;
    std::cout << "Search Time: " << searchTime.count() << " seconds" << std::endl;
}

void searchByBus(const std::vector<Student>& students, int busNumber) {
    auto start = std::chrono::high_resolution_clock::now();
    std::map<std::pair<std::string, std::string>, int> studentCount;

    for (int i = 0; i < students.size(); i++) {
        if (students[i].bus == busNumber) {
            studentCount[std::make_pair(students[i].lastName, students[i].firstName)]++;
        }
    }

    if (studentCount.size() == 0) {
        std::cout << "No students found using bus number: " << busNumber << std::endl;
    } else {
        for (auto it = studentCount.begin(); it != studentCount.end(); it++) {
            for (int j = 0; j < students.size(); j++) {
                if (students[j].lastName == it->first.first && students[j].firstName == it->first.second) {
                    std::cout << students[j].lastName << ", " << students[j].firstName
                              << " - Grade: " << students[j].grade
                              << " - Found " << it->second << " time(s)" << std::endl;
                    break;
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> searchTime = end - start;
    std::cout << "Search Time: " << searchTime.count() << " seconds" << std::endl;
}
