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
    assignments.insert(assignments.end(), matching.begin(), matching.end());

    cout << "Unmatched count: " << unassigned.size() << endl;

    return assignments;
}

pair<vector<student>, vector<seminar>> parseInput() {
    int n, m;
    cin >> n >> m;

    // parse n seminars
    vector<seminar> seminars;
    long id;
    int capacity;
    for (int i = 0; i < n; ++i) {
        cin >> id >> capacity;
        seminars.emplace_back(id, capacity);
    }

    // parse m students
    vector<student> students;
    long student_id;
    int preference_length;
    for (int j = 0; j < m; ++j) {
        cin >> student_id >> preference_length;
        vector<long> preference_list;
        preference_list.resize(preference_length);
        int seminar_id;

        for (int i = 0; i < preference_length; ++i) {
             cin >> seminar_id;
             preference_list[i] = seminar_id;
        }

        students.emplace_back(student_id, preference_list);
    }

    return make_pair(students, seminars);
}

int main() {
    auto input = parseInput();
    auto result = computeGreedy(input.first, input.second);

    cout << result.size() << endl;
    for (auto matching : result) {
        cout << matching.first << " " << matching.second << endl;
    }

    return 1;
}
