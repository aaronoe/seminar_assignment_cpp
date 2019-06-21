//
// Created by aaron on 21.06.19.
//

#include "graph.h"

using namespace std;

long vertex::anyNextVertexId() {
    if (this->outgoing_edges.empty()) return -1;
    auto value = *this->outgoing_edges.begin();
    return value.second;
}

vertex::vertex(long vertex_id) {
    this->vertex_id = vertex_id;
}

void vertex::addIngoingEdge(long source, long target){
    this->incoming_edges.insert(make_pair(source, target));
}

void vertex::addOutgoingEdge(long source, long target) {
    this->outgoing_edges.insert(make_pair(source, target));
}

graph::graph(std::vector<models::student> students) {
    for (auto &student : students) {
        this->students.emplace_back(student.id);
    }
}

void graph::addEdge(long source, long target) {
    this->edges.insert(make_pair(source, target));
    this->students[source].addOutgoingEdge(source, target);
    this->students[target].addIngoingEdge(source, target);
}

long graph::vertex_count() {
    return this->students.size();
}

vertex graph::any_vertex() {
    return this->students[0];
}

vertex graph::get_vertex_by_id(long vertex_id) {
    return this->students[vertex_id];
}

void graph::remove(vertex student) {
    set<pair<long, long>> involved(student.incoming_edges.begin(), student.incoming_edges.end());

    auto out = student.outgoing_edges;
    std::copy(out.begin(), out.end(), std::inserter(involved, involved.end()));

    for (auto edge: involved) {
        this->students[edge.second].incoming_edges.erase(edge);
        this->students[edge.first].outgoing_edges.erase(edge);
        this->edges.erase(edge);
    }
}
