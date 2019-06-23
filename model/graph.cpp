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

graph::graph(std::vector<models::student> students) {
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

vertex graph::any_vertex() {
    auto first = this->students.begin();
    return (*first).second;
}

vertex graph::get_vertex_by_id(long vertex_id) {
    return this->students[vertex_id];
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

void graph::SCCUtil(int u, int disc[], int low[], stack<int> *st, bool stackMember[],
        const vector<long> &student_map, unordered_map<long, int> &reverse_map) {
    set<set<long>> result;
// A static variable is used for simplicity, we can avoid use
    // of static variable by passing a pointer.
    static int time = 0;

    // Initialize discovery time and low value
    disc[u] = low[u] = ++time;
    st->push(u);
    stackMember[u] = true;

    auto vertex = this->students[student_map[u]];
    for (auto &edge: vertex.outgoing_edges) {
        auto v = reverse_map[edge.second];

        // If v is not visited yet, then recur for it
        if (disc[v] == -1)
        {
            SCCUtil(v, disc, low, st, stackMember, student_map, reverse_map);

            // Check if the subtree rooted with 'v' has a
            // connection to one of the ancestors of 'u'
            // Case 1 (per above discussion on Disc and Low value)
            low[u]  = min(low[u], low[v]);
        }

            // Update low value of 'u' only of 'v' is still in stack
            // (i.e. it's a back edge, not cross edge).
            // Case 2 (per above discussion on Disc and Low value)
        else if (stackMember[v])
            low[u]  = min(low[u], disc[v]);
    }

    // head node found, pop the stack and print an SCC
    int w = 0;  // To store stack extracted vertices
    if (low[u] == disc[u])
    {
        while (st->top() != u)
        {
            w = (int) st->top();
            cerr << w << " ";
            stackMember[w] = false;
            st->pop();
        }
        w = (int) st->top();
        cerr << w << "\n";
        stackMember[w] = false;
        st->pop();
    }
}

void graph::SCC() {
    auto V = this->students.size();
    int *disc = new int[V];
    int *low = new int[V];
    bool *stackMember = new bool[V];
    auto *st = new stack<int>();

    // Initialize disc and low, and stackMember arrays
    for (int i = 0; i < V; i++)
    {
        disc[i] = NIL;
        low[i] = NIL;
        stackMember[i] = false;
    }

    vector<long> student_map(students.size());
    unordered_map<long, int> reverse_map;
    int i = 0;
    for (auto &[id, vertex]: this->students) {
        student_map[i] = id;
        reverse_map[id] = i;
        i++;
    }
    for (int j = 0; j < V; ++j) {
        if (disc[j] == NIL) {
            SCCUtil(j, disc, low, st, stackMember, student_map, reverse_map);
        }
    }
}
