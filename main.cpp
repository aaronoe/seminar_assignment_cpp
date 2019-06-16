#include <random>

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "./model/student.h"
#include "./model/seminar.h"
#include "./model/bigraph.h"
#include "./model/hungarian.hpp"
#include "./algorithms/Hungarian.h"

using namespace std;
using namespace models;

/**
 * Run random serial dictatorship
 * @return Matching as vector of matched pairs
 */
vector<pair<long, long>> computeGreedy(vector<student> students, vector<seminar> seminars) {
    std::shuffle(students.begin(), students.end(), std::mt19937(std::random_device()()));
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

pair<vector<vector<int>>, vector<long>> getSeminarMapping(long total_count, vector<seminar> &available) {
    vector<vector<int>> seminar_mapping(static_cast<unsigned long>(total_count));
    vector<long> total_mapping;
    int counter = 0;

    for (seminar &seminar: available) {
        vector<int> ids(static_cast<unsigned long>(seminar.capacity));

        for (int i = 0; i < seminar.capacity; ++i) {
            ids[i] = counter++;
            total_mapping.push_back(seminar.id);
        }

        seminar_mapping[seminar.id] = ids;
    }

    return make_pair(seminar_mapping, total_mapping);
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
    int total_capacity_available = 0;
    for (auto &seminar : seminars) {
        int f_count = f_house_count[seminar.id];
        if (f_count != seminar.capacity) {
            available_seminars.push_back(seminar);
            total_capacity_available += seminar.capacity;
        }
    }

    // for every unassigned student, create a pair<long, long> of f-house-id, s-house-id
    // TODO- remember that the indices are not equivalent to student ids
    vector<pair<long, long>> priorities(students.size());

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

        if (s_house_id == -1) {
            // TODO: add l-houses
        }
        priorities[student.id] = make_pair(f_house_id, s_house_id);
    }

    if (!unassigned.empty() && total_capacity_available > 0) {
        // Expand list of seminars by capacity
        auto [mapping, total_mapping] = getSeminarMapping(seminars.size(), available_seminars);

        BipGraph graph(static_cast<int>(unassigned.size()), total_capacity_available);
        for (int i = 0; i < unassigned.size(); ++i) {
            auto student = unassigned[i];

            // TODO: the algorithm starts counting at 1
            auto prefs = priorities[student.id];
            for (int id: mapping[prefs.first]) {
                graph.addEdge(i + 1, id + 1);
            }
            for (int id: mapping[prefs.second]) {
                graph.addEdge(i + 1, id + 1);
            }
        }

        vector<pair<long, long>> matching = graph.hopcroftKarp();
        for (auto &match: matching) {
            auto first = match.first - 1;
            auto real_seminar = total_mapping[match.second - 1];
            cerr << "Mapped Value: " << match.second - 1 << " - Real: " << total_mapping[match.second - 1] << endl;

            assignments.emplace_back(first, real_seminar);
        }


        vector<long> capacity_list(seminars.size());
        // create new capacity list
        for (auto &[_, seminar_id]: assignments) {
            capacity_list[seminar_id] = capacity_list[seminar_id] + 1;
        }

        // check if students can be assigned to a better match
        for (auto &assignment : assignments) {
            auto [student_id, seminar_id] = assignment;

            auto student = students[student_id];
            auto seminar = seminars[seminar_id];

            for (auto pref: student.priorities) {
                if (pref == seminar_id) break;

                auto current_capacity = capacity_list[seminar_id];
                bool can_assign_more = current_capacity < seminar.capacity;
                if (can_assign_more) {
                    cerr << "Better match found" << endl;
                    capacity_list[seminar_id] = current_capacity + 1;
                    assignment = make_pair(student_id, pref);
                }
            }
        }
    }

    cerr << "Unmatched count: " << unassigned.size() << endl;

    return assignments;
}

vector<pair<long, long>> computeHungarianMatching(const vector<student> &students, vector<seminar> &seminars) {
    vector<pair<long, long>> assignments;

    auto [mapping, total_mapping] = getSeminarMapping(seminars.size(), seminars);

    auto row_length = total_mapping.size();
    auto col_length = students.size();

    vector<vector<int>> cost_matrix(col_length);

    for (int i = 0; i < students.size(); ++i) {
        auto &student = students[i];

        // set default cost to max seminar count
        vector<int> cost_row(row_length, INT32_MAX);

        for (int j = 0; j < student.priorities.size(); ++j) {
            auto pref = student.priorities[j];
            auto mapped_indices = mapping[pref];

            for (auto index: mapped_indices) {
                cost_row[index] = j;
            }
        }

        cost_matrix[i] = cost_row;
    }

    auto matching = Hungarian::Solve(cost_matrix, Hungarian::MODE_MINIMIZE_COST);

    if (!matching.success) return assignments;
    // extract result
    for (int k = 0; k < col_length; ++k) {
        auto row = matching.assignment[k];

        int matched_index = -1;
        for (int i = 0; i < row_length; ++i) {
            if (row[i] == 1) {
                matched_index = i;
                break;
            }
        }

        if (matched_index != -1) {
            auto mapped = total_mapping[matched_index];

            assignments.emplace_back(k, mapped);
        }
    }

    cerr << "Success: " << matching.success << endl;

    return assignments;
}


vector<pair<long, long>> computeOtherHungarianMatching(const vector<student> &students, vector<seminar> &seminars) {
    vector<pair<long, long>> assignments;

    auto [mapping, total_mapping] = getSeminarMapping(seminars.size(), seminars);

    auto row_length = total_mapping.size();
    auto col_length = students.size();

    vector<vector<double>> cost_matrix(col_length);

    for (int i = 0; i < students.size(); ++i) {
        auto &student = students[i];

        // set default cost to max seminar count
        vector<double> cost_row(row_length, INT32_MAX);

        for (int j = 0; j < student.priorities.size(); ++j) {
            auto pref = student.priorities[j];
            auto mapped_indices = mapping[pref];

            for (auto index: mapped_indices) {
                cost_row[index] = j;
            }
        }

        cost_matrix[i] = cost_row;
    }

    vector<int> assignment;
    HungarianAlgorithm algo;
    algo.Solve(cost_matrix, assignment);

    for (int k = 0; k < assignment.size(); ++k) {
        auto mapped = total_mapping[assignment[k]];
        assignments.emplace_back(k, mapped);
    }

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
        preference_list.resize(static_cast<unsigned long>(preference_length));
        int seminar_id;

        for (int i = 0; i < preference_length; ++i) {
             cin >> seminar_id;
             preference_list[i] = seminar_id;
        }

        students.emplace_back(student_id, preference_list);
    }

    return make_pair(students, seminars);
}

int main(int argc, char *argv[]) {
    string algorithm = "rsd";
    if (argc == 2) {
        algorithm = argv[1];
    }

    auto input = parseInput();

    vector<pair<long, long>> result;
    if (algorithm == "hungarian") {
        result = computeOtherHungarianMatching(input.first, input.second);
    } else if (algorithm == "popular") {
        result = popularCHA(input.first, input.second);
    } else if (algorithm == "rsd") {
        result = computeGreedy(input.first, input.second);
    }

    cout << result.size() << endl;
    for (auto matching : result) {
        cout << matching.first << " " << matching.second << endl;
    }

    return 1;
}
