//
// Created by aaron on 21.06.19.
//

#include <list>
#include <iostream>
#include "graph.h"
#include "bigraph.h"

using namespace std;

long vertex::anyNextVertexId() {
    if (this->outgoing_edges.empty()) return -1;
    auto value = *this->outgoing_edges.begin();
    return value.second;
}

void vertex::addIngoingEdge(long source, long target){
    this->incoming_edges.insert(make_pair(source, target));
}

void vertex::addOutgoingEdge(long source, long target) {
    this->outgoing_edges.insert(make_pair(source, target));
}

graph::graph(const std::vector<models::student> &students) {
    for (auto &student : students) {
        this->students.emplace(make_pair(student.id, student.id));
    }
}

void graph::addEdge(long source, long target) {
    // check if vertices are in graph
    if (this->students.find(source) == this->students.end() || this->students.find(target) == this->students.end()) {
        return;
    }
    this->edges.insert(make_pair(source, target));
    this->students[source].addOutgoingEdge(source, target);
    this->students[target].addIngoingEdge(source, target);
}

long graph::vertex_count() {
    return this->students.size();
}

vertex graph::any_vertex() const {
    auto first = this->students.begin();
    return (*first).second;
}

vertex graph::get_vertex_by_id(long vertex_id) const {
    return students.at(vertex_id);
}

void graph::remove(long vertex_id) {
    auto student = get_vertex_by_id(vertex_id);
    set<pair<long, long>> involved(student.incoming_edges.begin(), student.incoming_edges.end());

    auto out = student.outgoing_edges;
    std::copy(out.begin(), out.end(), std::inserter(involved, involved.end()));

    for (auto edge: involved) {
        this->students[edge.second].incoming_edges.erase(edge);
        this->students[edge.first].outgoing_edges.erase(edge);
        this->edges.erase(edge);
    }
    this->students.erase(student.vertex_id);
}

unordered_map<long, vertex> &graph::nodes() {
    return this->students;
}