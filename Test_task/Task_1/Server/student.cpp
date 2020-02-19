#include "student.h"

Student::Student(const std::string& name, const std::string& surname, const std::string& birthdate)
    : name(name),
      surname(surname),
      birthdate(birthdate)
{}

bool operator==(const Student &lhs, const Student &rhs)
{
    return lhs.name == rhs.name && lhs.surname == rhs.surname && lhs.birthdate == rhs.birthdate;
}

void Student::set_name(std::string&& name)
{
    this->name = std::move(name);
}

void Student::set_surname(std::string&& surname)
{
    this->surname = std::move(surname);
}

void Student::set_birthdate(std::string&& birthdate)
{
    this->birthdate = std::move(birthdate);
}

void Student::clear()
{
    name.clear();
    surname.clear();
    birthdate.clear();
}

bool Student::operator<(const Student &rhs)
{
    if(surname != rhs.surname)
        return surname < rhs.surname;
    else
        return name < rhs.name;
}

std::ostream& operator<<(std::ostream &s, const Student &student)
{
    s << student.name << ' ' << student.surname << ' ' << student.birthdate << '\n';
    return s;
}

std::istream& operator>>(std::istream& s, Student& student)
{
    int i;
    s >> i >> student.name >> student.surname >> student.birthdate;
    return s;
}


