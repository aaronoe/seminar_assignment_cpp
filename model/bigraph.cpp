/*
 * This algorithm has been adopted and slightly modified from:
 * https://www.geeksforgeeks.org/hopcroft-karp-algorithm-for-maximum-matching-set-2-implementation/
 */

#include "bigraph.h"

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

/**
 * @return pairs of matched students & seminars
 */
vector<pair<long, long>> BipGraph::hopcroftKarp() {
    // students_matches[u] stores pair of u in matching where u
    // is a vertex on left side of Bipartite Graph.
    // If u doesn't have any pair, then students_matches[u] is NIL
    students_matches = new int[student_count + 1];

    // seminars_matches[v] stores pair of v in matching. If v
    // doesn't have any pair, then students_matches[v] is NIL
    seminars_matches = new int[seminars_capacity + 1];

    // dist[u] stores distance of left side vertices
    // dist[u] is one more than dist[u'] if u is next
    // to u'in augmenting path
    dist = new int[student_count + 1];

    // Initialize NIL as pair of all vertices
    for (int u = 0; u < student_count; u++) {
        students_matches[u] = NIL;
    }
    for (int v = 0; v < seminars_capacity; v++) {
        seminars_matches[v] = NIL;
    }

    int result = 0;
    while (bfs()) {
        for (int u = 1; u <= student_count; u++) {
            // If current vertex is free and there is
            // an augmenting path from current vertex
            if (students_matches[u] == NIL && dfs(u)) {
                result++;
            }
        }
    }

    vector<pair<long, long>> assignments;
    for (int i = 0; i < student_count + 1; ++i) {
        if (students_matches[i] != NIL) {
            assignments.emplace_back(i, students_matches[i]);
        }
    }
    return assignments;
}

// Returns true if there is an augmenting path
bool BipGraph::bfs() {
    queue<int> queue;

    // First layer of vertices (set distance as 0)
    for (int u = 1; u <= student_count; u++) {
        // If this is a free vertex, add it to queue
        if (students_matches[u] == NIL) {
            dist[u] = 0;
            queue.push(u);
        } else {
            dist[u] = INF;
        }
    }

    dist[NIL] = INF;

    // queue is going to contain student vertices only.
    while (!queue.empty()) {
        int vertex = queue.front();
        queue.pop();

        // If this node is not NIL and can provide a shorter path to NIL
        if (dist[vertex] < dist[NIL]) {
            // Get all adjacent vertices of the dequeued vertex
            vector<int>::iterator iterator;
            for (iterator = adj[vertex].begin(); iterator != adj[vertex].end(); ++iterator) {
                int v = *iterator;

                // If pair of vertex is not considered so far
                // (v, seminars_matches[V]) is not yet explored edge.
                if (dist[seminars_matches[v]] == INF) {
                    // Consider the pair and add it to queue
                    dist[seminars_matches[v]] = dist[vertex] + 1;
                    queue.push(seminars_matches[v]);
                }
            }
        }
    }

    // If we could come back to NIL using alternating path of distinct
    // vertices then there is an augmenting path
    return (dist[NIL] != INF);
}

// Returns true if there is an augmenting path beginning with free vertex u
bool BipGraph::dfs(int source) {
    if (source != NIL) {
        vector<int>::iterator i;
        for (i = adj[source].begin(); i != adj[source].end(); ++i) {
            int v = *i;

            // Follow the distances set by BFS
            if (dist[seminars_matches[v]] == dist[source] + 1) {
                if (dfs(seminars_matches[v])) {
                    seminars_matches[v] = source;
                    students_matches[source] = v;
                    return true;
                }
            }
        }

        dist[source] = INF;
        return false;
    }
    return true;
}

BipGraph::BipGraph(int student_count, int total_capacity) {
    this->student_count = student_count;
    this->seminars_capacity = total_capacity;
    adj = new vector<int>[student_count + 1];
}

void BipGraph::addEdge(int u, int v) {
    adj[u].push_back(v); // Add u to v’s list.
}