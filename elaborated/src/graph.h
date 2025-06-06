#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <vector>
#include <set>
#include <memory>
#include <map>
#include <functional>

class Vertex;

using VertexID = int;
using shared_vertex = std::shared_ptr<Vertex>;

bool isAncestor(const shared_vertex ancestor, const shared_vertex decendant);

struct Edge {
	VertexID source;
	VertexID target;
};

class Vertex {
public:
	Vertex(VertexID _id): id(_id) {}

	VertexID getID() const { return id; }

	bool isDiscovered() const { return bDiscovered; }
	void labelAsDiscovered() { bDiscovered = true; }

	void setParent(const shared_vertex parent);
	shared_vertex getParent() const;
	bool isParentOf(const shared_vertex other);

	void reset();

private:
	bool bDiscovered = false;
	VertexID id;

	shared_vertex parent = nullptr;
};

bool operator<(const Vertex& lhs, const Vertex& rhs);



class UndirectedGraph {
public:
	/*
     * Initialize an undirected graph from a set of edges
     *
     * @param edges the set of edges used to initialize the graph
     */
	UndirectedGraph(const std::vector<Edge>& edges);

	/*
	 * This function returns a set containing all the vertices that are currently
	 * part of the graph. 
	 *
	 * @return a set containing all the vertices in the graph
	 */
	std::set<shared_vertex> getVertices() const;

	/*
	 * Retrieve a vertex from the graph by its ID
	 *
	 * @param id the ID of the vertex to retrieve
	 * @return the vertex with the given ID, or a null pointer if the vertex is not
	 *         found in the graph
	 */
	shared_vertex getVertexById(VertexID id) const;


	/*
     * Check if the graph contains the specified vertex
     *
     * @param vertex the vertex to check
     * @return true if the vertex is present in the graph, false otherwise
     */
	bool hasVertex(shared_vertex vertex) const;

	/*
	 * This function returns the set of vertices that are directly connected to the
	 * provided vertex by an edge in the graph
	 *
	 * @param vertex the vertex for which to retrieve the adjacent vertices
	 * @return the set of vertices that are adjacent to the given vertex
	 */
	const std::set<shared_vertex>& adjacentVerticesOf(shared_vertex vertex) const;


	/*
     * This function resets the state of all vertices. Preparing the graph for a 
	 * new round of graph traversal algorithms.
	 */
	void resetVertices();

private:
	void insertAdjacencyListItem(const Edge& edge);
	shared_vertex makeVertex(VertexID id);

	std::map<shared_vertex, std::set<shared_vertex>> adjacencyList;
};

class DepthFirstVisitor {
public:
	using EdgeExaminer = std::function<void(const shared_vertex source, const shared_vertex target)>;

	DepthFirstVisitor();

	/*
	 * This function allows the caller to register a callback function that will be
	 * called whenever a tree edge is discovered during a depth-first search The provided
	 * `examiner` function will be invoked with the two vertices that are connected by the 
	 * discovered tree edge.
	 *
	 * @param examiner the callback function to be invoked when a tree edge is
	 *        discovered during a depth-first search
	 */
	void registerTreeEdgeExaminer(EdgeExaminer examiner);

	/*
     * This function allows the caller to register a callback function that will be
     * called whenever a back edge is discovered during a depth-first search The provided
     * `examiner` function will be invoked with the two vertices that are connected by the
     * discovered back edge.
     *
     * @param examiner the callback function to be invoked when a back edge is
     *        discovered during a depth-first search
     */
	void registerBackEdgeExaminer(EdgeExaminer examiner);

	/*
	 * Perform a depth-first search (DFS) on the graph starting from the given
	 * source vertex
	 *
	 * @param graph the graph on which to perform the depth-first search
	 * @param source the vertex from which to start the search, which must be
	 *        present in the provided graph
	 */
	void search(UndirectedGraph& graph, shared_vertex source);

private:
	void recurSearch(UndirectedGraph& graph, shared_vertex source);
	void examineBackEdgeIfFound(shared_vertex currentVertex, shared_vertex neighbor);

	EdgeExaminer treeEdgeExaminer;
	EdgeExaminer backEdgeExaminer;
};

#endif
