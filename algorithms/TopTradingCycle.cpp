//
// Created by aaron on 21.06.19.
//

#include "TopTradingCycle.h"
#include <vector>
#include "../model/student.h"
#include "../model/graph.h"
#include "set"
#include <iostream>

using namespace std;

set<long> getVertexInCycle(graph market) {
    set<long> visited;

    auto start = market.any_vertex();

    while (visited.find(start.vertex_id) == visited.end()) {
        visited.insert(start.vertex_id);
        auto next = start.anyNextVertexId();
        if (next == -1) return set<long>();
        start = market.get_vertex_by_id(next);
    }

    return visited;
}


/*
std::vector<vertex> getCycle(graph &market) {
    std::unordered_set<vertex> visited;
    auto start_vertex = getVertexInCycle(market);

    while (visited.find(start_vertex) != visited.end()) {
        visited.insert(start_vertex);
        start_vertex = market.get_vertex_by_id(start_vertex.anyNextVertexId());
    }
}
 */

void getTopTradingCycle(vector<models::student> &students, long seminar_count, vector<long> initial_ownership) {
    graph market(students);
    vector<vector<long>> ownership_map(static_cast<unsigned long>(seminar_count));

    for (int i = 0; i < initial_ownership.size(); ++i) {
        auto seminar_id = initial_ownership[i];
        if (seminar_id == -1) continue;

        ownership_map[seminar_id].push_back(i);
    }

    for (int j = 0; j < students.size(); ++j) {
        auto student = students[j];
        if (student.priorities.empty()) continue;

        auto first_pref = student.priorities[0];
        auto ownerships = ownership_map[first_pref];

        for (auto &owner: ownerships) {
            market.addEdge(j, owner);
        }
    }

    // remove students already assigned to their first choice
    for (auto &[student_id, vertex]: market.nodes()) {
        auto student = students[student_id];
        if (student.priorities.empty()) continue;

        if (student.priorities[0] == initial_ownership[student_id]) {
            market.remove(vertex);
        }
    }

    vector<pair<long, long>> matching;
    vector<long> current_preferred_index(initial_ownership.size(), 0);

    while (market.vertex_count() > 0) {
        auto cycle = getVertexInCycle(market);
        // implement trade specified by cycle
        if (cycle.size() > 1) {
            cout << "implementing trade of size " << cycle.size() << endl;
        }
        if (cycle.size() > 1) {
            for (auto &id: cycle) {
                auto vertex = market.get_vertex_by_id(id);
                auto student = students[id];
                auto pref = student.priorities[current_preferred_index[id]];

                matching.emplace_back(id, pref);
                market.remove(vertex);
            }
        }

        // update prefs and remove vertices
        for (auto &[id, vertex]: market.nodes()) {
            if (vertex.outgoing_edges.empty()) {
                // the current seminar can't be swapped for, go to the next one
                auto student = students[id];
                current_preferred_index[id] += 1;

                // preference is same as existing seminar
                auto current_index = current_preferred_index[id];
                if (student.priorities.size() <= current_index || student.priorities[current_index] == initial_ownership[id]) {
                    market.remove(vertex);
                    cout << "removing student" << endl;
                    continue;
                }

                // otherwise set next seminar as preferred
                auto pref = student.priorities[current_index];
                auto ownerships = ownership_map[pref];

                for (auto &owner: ownerships) {
                    market.addEdge(id, owner);
                }
            }
        }
    }
}