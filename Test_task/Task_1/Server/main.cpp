#include "zmq.h"
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <thread>
#include "student.h"
#include "studenthasher.h"

void create_list_of_stundets(const char* file_1, const char* file_2, std::unordered_set<Student, StudentHasher>& students)
{
    std::fstream in;
    in.open(file_1, std::ios_base::in);
    if(!in.is_open()) {
        std::cout << file_1 << " was not opened\n";
        abort();
    }

    Student student;
    while(in >> student) {
        students.insert(std::move(student));
        student.clear();
    }
    in.close();

    in.open(file_2, std::ios_base::in);
    if(!in.is_open()) {
        std::cout << file_2 << " was not opened\n";
        abort();
    }
    while(in >> student) {
        students.insert(std::move(student));
        student.clear();
    }
    in.close();
}

size_t count_size_of_message(const std::unordered_set<Student, StudentHasher>& students)
{
    size_t size_of_message = 0;
    size_t count_of_zero_terminations = 0;
    for(const auto& i : students) {
        size_of_message += i.get_birthdate().size() + i.get_name().size() + i.get_surname().size();
        count_of_zero_terminations += 3;
    }
    size_of_message += count_of_zero_terminations;
    return size_of_message;
}

char* make_raw_data(const std::unordered_set<Student, StudentHasher>& students, size_t size_of_message)
{
    char* data = (char*) malloc(size_of_message);
    size_t offset = 0;
    for(const auto& i : students) {
        memcpy(data + offset, i.get_name().c_str(), i.get_name().size());
        offset += i.get_name().size();
        memset(data + offset, 0, sizeof(char));
        ++offset;

        memcpy(data + offset, i.get_surname().c_str(), i.get_surname().size());
        offset += i.get_surname().size();
        memset(data + offset, 0, sizeof(char));
        ++offset;

        memcpy(data + offset, i.get_birthdate().c_str(), i.get_birthdate().size());
        offset += i.get_birthdate().size();
        memset(data + offset, 0, sizeof(char));
        ++offset;
    }

    return data;
}

void start_server_thread() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Server thread: " << std::this_thread::get_id() << '\n';
    
	std::unordered_set<Student, StudentHasher> students;
    create_list_of_stundets("path_to_student_file_1.txt",
                            "path_to_student_file_2.txt",
                            students);
    size_t size_of_message = count_size_of_message(students);
    char* data = make_raw_data(students, size_of_message);
    students.clear();

    //------------------------------------------------------------------------
    void* context = zmq_ctx_new();
    void* publisher = zmq_socket(context, ZMQ_PUB);
    zmq_bind(publisher, "tcp://*:322");

    for(;;)
    {
        zmq_msg_t message;
        zmq_msg_init_size(&message, size_of_message);
        memcpy(zmq_msg_data(&message), data, size_of_message);
        zmq_msg_send(&message, publisher, 0);
        zmq_msg_close(&message);
    }

    free(data);
    zmq_close(publisher);
    zmq_ctx_destroy(context);
}

int main ()
{
    std::thread second_thread(start_server_thread);
    std::cout << "Gui thread: " << std::this_thread::get_id() << '\n';

    second_thread.join();
    return 0;
}
