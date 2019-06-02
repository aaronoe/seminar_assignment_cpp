#ifndef SEMINAR_ASSIGNMENT_SEMINAR_H
#define SEMINAR_ASSIGNMENT_SEMINAR_H

#include <vector>

namespace models {

    class seminar {
    public:
        long id;
        int capacity;

        seminar(const long &id, const int &capacity);
    };

}


#endif //SEMINAR_ASSIGNMENT_SEMINAR_H
