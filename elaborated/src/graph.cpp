#include "graph.h"
#include <cassert>
#include <algorithm>
#include <iterator>
#include <stack>

bool operator<(const Vertex& lhs, const Vertex& rhs)
{
	return lhs.getID() < rhs.getID();
}

bool isAncestor(const shared_vertex ancestor, const shared_vertex decendant)
{
	assert(ancestor != nullptr && decendant != nullptr);

	auto vertex = decendant;
	while (nullptr != vertex) {

		if (ancestor->isParentOf(vertex))
			return true;

		vertex = vertex->getParent();
	}

	return false;
}

void Vertex::setParent(const shared_vertex _parent)
{
	assert(_parent != nullptr);
	assert(_parent.get() != this);

	parent = _parent;
}

shared_vertex Vertex::getParent() const
{
	return parent;
}

bool Vertex::isParentOf(const shared_vertex other)
{
	assert(other != nullptr);

	if (other->parent == nullptr)
		return false;

	return this == other->parent.get();
}

void Vertex::reset()
{
	bDiscovered = false;
	parent = nullptr;
}


UndirectedGraph::UndirectedGraph(const std::vector<Edge>& edges) {
	for (auto&& edge : edges) 
		insertAdjacentListItem(edge);
}


void UndirectedGraph::insertAdjacentListItem(const Edge& edge)
{
	auto sourceVertex = makeVertex(edge.source);
	auto targetVertex = makeVertex(edge.target);

	if (sourceVertex->getID() != targetVertex->getID()) {
		adjacentLists[sourceVertex].insert(targetVertex);
		adjacentLists[targetVertex].insert(sourceVertex);
	}
}

shared_vertex UndirectedGraph::makeVertex(VertexID id)
{
	auto vertex = getVertexById(id);

	if (vertex != nullptr)
		return vertex;

	auto newVertex = std::make_shared<Vertex>(id);
	adjacentLists[newVertex] = std::set<shared_vertex>();

	return newVertex;
}

std::set<shared_vertex> UndirectedGraph::getVertices() const
{

	std::set<shared_vertex> result;

	std::transform(adjacentLists.begin(), adjacentLists.end(), std::inserter(result, result.begin()),
				   [](const std::pair< shared_vertex, std::set<shared_vertex>>& item)
				   {
			           return item.first;
				   }
	);

	return result;
}

bool UndirectedGraph::hasVertex(shared_vertex vertex) const
{
	assert(vertex != nullptr);

	auto it = adjacentLists.find(vertex);
	return it != adjacentLists.end();
}

shared_vertex UndirectedGraph::getVertexById(VertexID id) const
{
	auto it = std::find_if(adjacentLists.cbegin(),
		                   adjacentLists.cend(), 
		                  [id](const std::pair< shared_vertex, std::set<shared_vertex>>& item)
		                  { return item.first->getID() == id;  });

	auto found = it != adjacentLists.end();

	return found ? it->first : nullptr;
}

const std::set<shared_vertex>& UndirectedGraph::adjacentVerticesOf(shared_vertex vertex) const {
	assert(hasVertex(vertex));

	auto it = adjacentLists.find(vertex);
	return it->second;
}

void UndirectedGraph::resetVertices() {
	auto vertices = getVertices();

	for (auto&& v : vertices)
		v->reset();
}

DepthFirstVisitor::DepthFirstVisitor()
{
	treeEdgeExaminer = [](const shared_vertex source, const shared_vertex target) {};
	backEdgeExaminer = [](const shared_vertex source, const shared_vertex target) {};
}

void DepthFirstVisitor::registerTreeEdgeExaminer(EdgeExaminer examiner)
{
	treeEdgeExaminer = examiner;
}

void DepthFirstVisitor::registerBackEdgeExaminer(EdgeExaminer examiner)
{
	backEdgeExaminer = examiner;
}

void DepthFirstVisitor::search(UndirectedGraph& graph, shared_vertex source)
{
	assert(graph.hasVertex(source));
	graph.resetVertices();

	recurSearch(graph, source);
}

void DepthFirstVisitor::recurSearch(UndirectedGraph& graph, shared_vertex currentVertex)
{
	currentVertex->labelAsDiscovered();

	for (shared_vertex neighbor : graph.adjacentVerticesOf(currentVertex)) {
		if (!neighbor->isDiscovered()) {
			neighbor->setParent(currentVertex);
			treeEdgeExaminer(currentVertex, neighbor);

			recurSearch(graph, neighbor);
		}

		examineBackEdgeIfFound(currentVertex, neighbor);
	}
}

void DepthFirstVisitor::examineBackEdgeIfFound(shared_vertex currentVertex, shared_vertex neighbor)
{
	bool foundBackEdge = !neighbor->isParentOf(currentVertex) && isAncestor(neighbor, currentVertex);
	if(foundBackEdge)
		backEdgeExaminer(currentVertex, neighbor);
}

