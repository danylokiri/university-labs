#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>

struct Student {
    std::string lastName;
    std::string firstName;
    int grade;
    int classroom;
    int bus;
    std::string teacherLastName;
    std::string teacherFirstName;
};

void searchByStudent(const std::vector<Student>& students, const std::string& lastName);
void searchByStudentBus(const std::vector<Student>& students, const std::string& lastName);
void searchByTeacher(const std::vector<Student>& students, const std::string& teacherLastName);
void searchByClassroom(const std::vector<Student>& students, int classroom);
void searchByBus(const std::vector<Student>& students, int busNumber);

#endif
