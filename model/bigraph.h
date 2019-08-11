/*
 * This algorithm has been adopted and slightly modified from:
 * https://www.geeksforgeeks.org/hopcroft-karp-algorithm-for-maximum-matching-set-2-implementation/
 */

#ifndef SEMINAR_ASSIGNMENT_BIGRAPH_H
#define SEMINAR_ASSIGNMENT_BIGRAPH_H
#include <vector>
#include <climits>

#define NIL 0
#define INF INT_MAX

class BipGraph
{
    // student_count and seminars_capacity are number of vertices on left
    // and right sides of Bipartite Graph
    int student_count, seminars_capacity;

    // adj[u] stores adjacents of left side
    // vertex 'u'. The value of u ranges from 1 to student_count.
    // 0 is used for dummy vertex
    std::vector<int> *adj;

    // These are basically pointers to arrays needed
    // for hopcroftKarp()
    int *students_matches, *seminars_matches, *dist;

public:
    BipGraph(int student_count, int total_capacity); // Constructor
    void addEdge(int u, int v); // To add edge

    // Returns true if there is an augmenting path
    bool bfs();

    // Adds augmenting path if there is one beginning
    // with u
    bool dfs(int source);

    // Returns size of maximum matcing
    std::vector<std::pair<long, long>> hopcroftKarp();
};

#endif //SEMINAR_ASSIGNMENT_BIGRAPH_H
