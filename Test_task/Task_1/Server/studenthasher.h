#ifndef STUDENTHASHER_H
#define STUDENTHASHER_H

#include <student.h>

class StudentHasher
{
public:
    size_t operator()(const Student& student) const {
        return std::hash<std::string>()(student.get_name() + student.get_surname() + student.get_birthdate());
    }
};

#endif // STUDENTHASHER_H
