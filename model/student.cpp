#include <utility>

//
// Created by aaron on 02.06.19.
//

#include "student.h"

using namespace std;

models::student::student(const long &id, vector<long> priorities) : id(id), priorities(std::move(priorities)) {}
