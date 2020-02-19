#ifndef STUDENT_H
#define STUDENT_H

#include <string>

class Student
{
    std::string name;
    std::string surname;
    std::string birthdate;

public:
    Student() = default;
    Student(const std::string& name, const std::string& surname, const std::string& birthdate);

    std::string get_name() const { return name; }
    std::string get_surname() const { return surname; }
    std::string get_birthdate() const { return birthdate; }

    void set_name(std::string&& name);
    void set_surname(std::string&& surname);
    void set_birthdate(std::string&& birthdate);

    void clear();

    friend bool operator==(const Student& lhs, const Student& rhs);
    bool operator<(const Student& rhs);

    friend std::ostream& operator<<(std::ostream& s, const Student& student);
    friend std::istream& operator>>(std::istream& s, Student& student);
};

#endif // STUDENT_H
