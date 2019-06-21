//
// Created by aaron on 21.06.19.
//

#include "TopTradingCycle.h"
#include <vector>
#include "../model/student.h"
#include "../model/graph.h"
#include "set"

using namespace std;

vertex getVertexInCycle(graph market) {
    std::set<long> visited;

    auto vertex = market.any_vertex();

    while (visited.find(vertex.vertex_id) == visited.end()) {
        visited.insert(vertex.vertex_id);
        auto next = vertex.anyNextVertexId();
        vertex = market.get_vertex_by_id(next);
    }

    return vertex;
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
    for (int k = 0; k < students.size(); ++k) {
        auto student = students[k];
        if (student.priorities.empty()) continue;

        // student k is assigned to first choice -> remove incident edges from graph
        if (students[k].priorities[0] == initial_ownership[k]) {
            market.remove(market.get_vertex_by_id(k));
        }
    }

    vector<pair<long, long>> matching;

    while (market.vertex_count() > 0) {
        auto cycle = getVertexInCycle(market);
    }
}