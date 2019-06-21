#ifndef SEMINAR_ASSIGNMENT_GRAPH_H
#define SEMINAR_ASSIGNMENT_GRAPH_H
#include <vector>
#include <set>
#include "student.h"

class vertex {
public:
    long vertex_id;
    std::set<std::pair<long, long>> outgoing_edges;
    std::set<std::pair<long, long>> incoming_edges;

    explicit vertex(long vertex_id);
    long anyNextVertexId();
    void addIngoingEdge(long source, long target);
    void addOutgoingEdge(long source, long target);
};

class graph {
    std::vector<vertex> students;
    std::set<std::pair<long, long>> edges;

public:
    explicit graph(std::vector<models::student> students);
    void addEdge(long source, long target);
    long vertex_count();
    vertex any_vertex();
    vertex get_vertex_by_id(long vertex_id);
    void remove(vertex student);
};

#endif //SEMINAR_ASSIGNMENT_GRAPH_H
