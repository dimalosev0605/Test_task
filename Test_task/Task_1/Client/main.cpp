#include "zmq.hpp"
#include <iostream>
#include "../Server/student.h"

void create_list_of_students_from_raw_data(const char* data, size_t length, std::vector<Student>& students)
{
    Student student;
    std::string temp;

    int c = 0;
    for(size_t i = 0; i < length; ++i) {
        if(data[i] != '\0')
            temp.push_back(data[i]);
        else {
            switch (c) {
            case 0: student.set_name(std::move(temp));
                break;
            case 1: student.set_surname(std::move(temp));
                break;
            case 2: {
                student.set_birthdate(std::move(temp));
                students.push_back(std::move(student));
                student.clear();
                c = -1;
                break;
            }
            }
            ++c;
            temp.clear();
        }
    }
}

int main ()
{
    void* context = zmq_ctx_new();
    void* subscriber = zmq_socket(context, ZMQ_SUB);
    zmq_connect(subscriber, "tcp://localhost:322");
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, 0, 0);

    zmq_msg_t reply;
    zmq_msg_init(&reply);
    zmq_msg_recv(&reply, subscriber, 0);
    size_t length = zmq_msg_size(&reply);
    char* data = (char*)malloc(length);
    memcpy(data, zmq_msg_data(&reply), length);
    zmq_msg_close(&reply);

    zmq_close(subscriber);
    zmq_ctx_destroy(context);

    std::vector<Student> students;
    create_list_of_students_from_raw_data(data, length, students);
    free(data);

    std::sort(students.begin(), students.end());
    std::cout << "List of students:\n";
    for(const auto& i : students) {
        std::cout << i;
    }

    return 0;
}
