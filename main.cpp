#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "./model/student.h"
#include "./model/seminar.h"

using namespace std;
using namespace models;

/**
 * Run random serial dictatorship
 * @return Matching as vector of matched pairs
 */
vector<pair<long, long>> computeGreedy(vector<student> students, vector<seminar> seminars) {
    std::random_shuffle(students.begin(), students.end());
    vector<int> assignment_count(seminars.size(), 0);
    vector<pair<long, long>> assignments;

    for (auto &student : students) {
        for (auto &priority: student.priorities) {
            int used = assignment_count[priority];
            int capacity = seminars[priority].capacity;

            if (used < capacity) {
                assignment_count[priority] = used + 1;
                assignments.emplace_back(student.id, priority);
                break;
            }
        }
    }

    return assignments;
}

vector<pair<long, long>> popularCHA(vector<student> students, vector<seminar> seminars) {
    vector<int> f_house_count(seminars.size(), 0);
    vector<pair<long, long>> assignments;

    for (auto &student : students) {
        auto first = student.priorities[0];

        f_house_count[first] = f_house_count[first] + 1;
    }

    vector<student> unassigned;
    // assign undersubscribed houses to applicants
    for (auto &student : students) {
        auto first = student.priorities[0];

        int f_count = f_house_count[first];
        if (f_count <= seminars[first].capacity) {
            assignments.emplace_back(student.id, first);
        } else {
            unassigned.push_back(student);
        }
    }

    vector<seminar> available_seminars;
    for (auto &seminar : seminars) {
        int f_count = f_house_count[seminar.id];
        if (f_count != seminar.capacity) {
            available_seminars.push_back(seminar);
        }
    }

    // for every unassigned student, create a pair<long, long> of f-house-id, s-house-id
    // TODO- remember that the indices are not equivalent to student ids
    vector<pair<long, long>> priorities;
    priorities.resize(unassigned.size());

    for (student &student : unassigned) {
        auto f_house_id = student.priorities[0];
        long s_house_id = -1;
        for (int i = 1; i < student.priorities.size(); ++i) {
            auto seminar = student.priorities[i];
            auto f_value = f_house_count[seminar];
            auto capacity = seminars[i].capacity;

            if (f_value < capacity) {
                s_house_id = seminar;
                break;
            }
        }

        // TODO: add l-houses
        priorities[student.id] = make_pair(f_house_id, s_house_id);
    }

    vector<pair<long, long>> matching; // todo: get hopcroft karp matching here
    assignments.insert(assignments.end(), matching.begin(), matching.end())

    cout << "Unmatched count: " << unassigned.size() << endl;

    return assignments;
}

int main() {
    seminar seminar0(0l, 0);
    seminar seminar1(1l, 1);
    seminar seminar2(2l, 2);

    student student0(0l, vector<long> { seminar0.id, seminar1.id });
    student student1(1l, vector<long> { seminar1.id, seminar0.id });
    student student2(2l, vector<long> { seminar2.id, seminar0.id });

    //auto result = computeGreedy(vector<student> { student0, student1, student2 }, vector<seminar> { seminar0, seminar1, seminar2 });
    auto result = popularCHA(vector<student> { student0, student1, student2 }, vector<seminar> { seminar0, seminar1, seminar2 });

    return 0;
}
