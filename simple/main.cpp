//
//  main.cpp
//  CppGraphTest
//
//  Created by Dave Duprey on 25/11/2019.
//  Copyright © 2019 Dave Duprey. All rights reserved.
//

#include <iostream>
#include <vector>
#include <queue>
#include "graph.h"

using namespace std;

bool has_cycle(const vector<Edge>& edges) {
    if (edges.empty())
        return false;

    UndirectedGraph graph(edges);
    auto  startVertex = graph.getVertex(edges[0].source);
    std::queue<shared_vertex> vertices_queue;

    // Perform a breadth-first search to detect the presence of a circle.

    startVertex->labelDiscovered();
    vertices_queue.push(startVertex);

    while (!vertices_queue.empty()) {
        auto u = vertices_queue.front();
        vertices_queue.pop();

        for(auto& neighbor : graph.adjacentVerticesOf(u)) {
            if (neighbor->isParentOf(u))
                continue;
            
            if(neighbor->isDiscovered())
                return true;
        
            neighbor->labelDiscovered();
            neighbor->setParent(u);
            vertices_queue.push(neighbor);
        }

    }

    return false;
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
