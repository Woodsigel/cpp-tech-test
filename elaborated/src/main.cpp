#include <iostream>
#include <vector>
#include <queue>
#include "graph.h"


using namespace std;

bool has_cycle(const vector<Edge>& edges) {
    DepthFirstVisitor depthFirst;
    bool foundBackEdge = false;
    depthFirst.registerBackEdgeExaminer([&foundBackEdge](const shared_vertex source,
                                      const shared_vertex target)
                                      { foundBackEdge = true; });

    UndirectedGraph graph(edges);
    auto source = graph.getVertexById(0);

    depthFirst.search(graph, source);

    return foundBackEdge;
}


void report_results(bool cycle_found) {
    if (cycle_found)
        cout << "Graph contains a cycle\n";
    else
        cout << "Graph does NOT contain any cycles\n";
}


void check_for_cycles(const vector<Edge>& edges) {
    bool cycle_found = has_cycle(edges);
    report_results(cycle_found);
}


int main(int argc, const char* argv[]) {
    const vector<Edge> edges_with_cycle = { {0, 1}, {0, 2}, {0, 3}, {1, 4}, {1, 5}, {4, 8}, {4, 9}, {3, 6}, {3, 7}, {6, 10}, {6, 11}, {5, 9} };
    const vector<Edge> edges_without_cycle = { {0, 1}, {0, 2}, {0, 3}, {1, 4}, {1, 5}, {4, 8}, {4, 9}, {3, 6}, {3, 7}, {6, 10}, {6, 11} };

    check_for_cycles(edges_with_cycle);
    check_for_cycles(edges_without_cycle);

    return 0;
}
