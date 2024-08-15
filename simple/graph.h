#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <list>
#include <memory>
#include <map>
#include <vector>
#include <set>

using VertexID = int;

struct Edge {
    const int source;
    const int destination;
};

class Vertex {
    friend bool operator<(const Vertex& lhs, const Vertex& rhs);
public:
    explicit Vertex(VertexID _vertexID): vertexID(_vertexID) {}

    VertexID getID() const { return vertexID; }

    void labelDiscovered() { bVisited = true; }
    bool isDiscovered() const { return bVisited; }

    void setParent(std::shared_ptr<Vertex> parent);
    bool isParentOf(std::shared_ptr<Vertex> other);

    bool operator==(const Vertex& other) const;
    bool operator!=(const Vertex& other) const;

private:
    VertexID vertexID;
    bool bVisited = false;

    std::shared_ptr<Vertex> parent;
};

bool operator<(const Vertex& lhs, const Vertex& rhs);

using shared_vertex = std::shared_ptr<Vertex>;

class UndirectedGraph {
public:
    /*
     * Initialize an undirected graph from a set of edges
     * This constructor accepts a vector of edges, which is used to create and
     * initialize an undirected graph object.
     *
     * @param edges the set of edges used to initialize the graph
     */
    UndirectedGraph(const std::vector<Edge>& edges);

    /*
     * Check if the graph contains the specified vertex
     *
     * If the graph contains the vertex with the given ID, the function returns true.
     * Otherwise, it returns false.
     *
     * @param vertexID the ID of the vertex to check
     * @return true if the vertex is present in the graph, false otherwise
     */
    bool hasVertex(VertexID vertexID) const;

    /*
     * Retrieve a vertex from the graph
     *
     * This function returns the vertex object with the specified ID, assuming that
     * the vertex is already present in the graph. 
     *
     * @param vertexID the ID of the vertex to retrieve
     * @return the vertex with the given ID
     */
    shared_vertex getVertex(VertexID vertexID) const;

    /*
     * Retrieve the set of vertices adjacent to the given vertex
     *
     * This function returns the set of vertices that are directly connected to the
     * provided vertex by an edge in the graph
     *
     * @param vertex the vertex for which to retrieve the adjacent vertices
     * @return the set of vertices that are adjacent to the given vertex
     */
    std::set<shared_vertex>& adjacentVerticesOf(const shared_vertex vertex);

private:
    std::map<VertexID, std::set<shared_vertex>> edgesToAdjacentLists(const std::vector<Edge>& edges);
    shared_vertex makeVertex(VertexID vertexID);

    std::map<VertexID, shared_vertex> vertices;
    std::map<VertexID, std::set<shared_vertex>> adjacent_lists;
};

#endif
