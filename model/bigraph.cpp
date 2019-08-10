#include "bigraph.h"

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

/**
 * @return pairs of matched students & seminars
 */
vector<pair<long, long>> BipGraph::hopcroftKarp() {
    // pairU[u] stores pair of u in matching where u
    // is a vertex on left side of Bipartite Graph.
    // If u doesn't have any pair, then pairU[u] is NIL
    pairU = new int[m + 1];

    // pairV[v] stores pair of v in matching. If v
    // doesn't have any pair, then pairU[v] is NIL
    pairV = new int[n + 1];

    // dist[u] stores distance of left side vertices
    // dist[u] is one more than dist[u'] if u is next
    // to u'in augmenting path
    dist = new int[m + 1];

    // Initialize NIL as pair of all vertices
    for (int u = 0; u < m; u++) {
        pairU[u] = NIL;
    }
    for (int v = 0; v < n; v++) {
        pairV[v] = NIL;
    }

    int result = 0;
    while (bfs()) {
        for (int u = 1; u <= m; u++) {
            // If current vertex is free and there is
            // an augmenting path from current vertex
            if (pairU[u] == NIL && dfs(u)) {
                result++;
            }
        }
    }

    vector<pair<long, long>> assignments;
    for (int i = 0; i < m + 1; ++i) {
        if (pairU[i] != NIL) {
            assignments.emplace_back(i, pairU[i]);
        }
    }
    return assignments;
}

// Returns true if there is an augmenting path
bool BipGraph::bfs() {
    queue<int> queue;

    // First layer of vertices (set distance as 0)
    for (int u = 1; u <= m; u++) {
        // If this is a free vertex, add it to queue
        if (pairU[u] == NIL) {
            dist[u] = 0;
            queue.push(u);
        } else {
            dist[u] = INF;
        }
    }

    dist[NIL] = INF;

    // queue is going to contain vertices of left side only.
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
                // (v, pairV[V]) is not yet explored edge.
                if (dist[pairV[v]] == INF) {
                    // Consider the pair and add it to queue
                    dist[pairV[v]] = dist[vertex] + 1;
                    queue.push(pairV[v]);
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
            if (dist[pairV[v]] == dist[source] + 1) {
                if (dfs(pairV[v])) {
                    pairV[v] = source;
                    pairU[source] = v;
                    return true;
                }
            }
        }

        dist[source] = INF;
        return false;
    }
    return true;
}

BipGraph::BipGraph(int m, int n) {
    this->m = m;
    this->n = n;
    adj = new vector<int>[m + 1];
}

void BipGraph::addEdge(int u, int v) {
    adj[u].push_back(v); // Add u to v’s list.
}