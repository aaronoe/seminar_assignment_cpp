#ifndef SEMINAR_ASSIGNMENT_STUDENT_H
#define SEMINAR_ASSIGNMENT_STUDENT_H

#include <vector>
#include <string>

namespace models {

    class student {
    public:
        long id;
        std::vector<long> priorities;

        student(const long &id, std::vector<long> priorities);
    };

}

#endif //SEMINAR_ASSIGNMENT_STUDENT_H
