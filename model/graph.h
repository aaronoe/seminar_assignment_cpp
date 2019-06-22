#ifndef SEMINAR_ASSIGNMENT_GRAPH_H
#define SEMINAR_ASSIGNMENT_GRAPH_H
#include <vector>
#include <set>
#include <unordered_map>
#include "student.h"

using namespace std;
using namespace models;

class vertex {
public:
    long vertex_id;
    set<pair<long, long>> outgoing_edges;
    set<pair<long, long>> incoming_edges;

    vertex(const long student_id): vertex_id(student_id) {};
    vertex() {};
    long anyNextVertexId();
    void addIngoingEdge(long source, long target);
    void addOutgoingEdge(long source, long target);
};

class graph {
    unordered_map<long, vertex> students;
    set<pair<long, long>> edges;

public:
    explicit graph(vector<student> students);
    void addEdge(long source, long target);
    long vertex_count();
    vertex any_vertex();
    vertex get_vertex_by_id(long vertex_id);
    void remove(vertex student);
    unordered_map<long, vertex> nodes() const;
};

#endif //SEMINAR_ASSIGNMENT_GRAPH_H
