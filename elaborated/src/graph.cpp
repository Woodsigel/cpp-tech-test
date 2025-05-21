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
	assert(nullptr != ancestor && nullptr != decendant);

	auto vertex = decendant;
	while (nullptr != vertex) {

		if (ancestor->isParentOf(vertex))
			return true;

		vertex = vertex->getParent();
	}

	return false;
}

void Vertex::setParent(const std::shared_ptr<Vertex> _parent)
{
	assert(nullptr != _parent);
	assert(_parent.get() != this && _parent->getID() != this->getID());

	parent = _parent;
}

std::shared_ptr<Vertex> Vertex::getParent() const
{
	return parent;
}

bool Vertex::isParentOf(const std::shared_ptr<Vertex> other)
{
	assert(nullptr != other);

	if (nullptr == other->parent)
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
	auto source_vertex = makeVertex(edge.source);
	auto target_vertex = makeVertex(edge.target);

	if (source_vertex->getID() != target_vertex->getID()) {
		adjacent_lists[source_vertex].insert(target_vertex);
		adjacent_lists[target_vertex].insert(source_vertex);
	}
}

shared_vertex UndirectedGraph::makeVertex(VertexID id)
{
	auto vertex = getVertexById(id);

	if (vertex != nullptr)
		return vertex;

	auto newVertex = std::make_shared<Vertex>(id);
	adjacent_lists[newVertex] = std::set<shared_vertex>();

	return newVertex;
}

std::set<shared_vertex> UndirectedGraph::getVertices() const
{

	std::set<shared_vertex> result;

	std::transform(adjacent_lists.begin(), adjacent_lists.end(), std::inserter(result, result.begin()),
				   [](const std::pair< shared_vertex, std::set<shared_vertex>>& item)
				   {
			           return item.first;
				   }
	);

	return result;
}

bool UndirectedGraph::hasVertex(shared_vertex vertex) const
{
	assert(nullptr != vertex);

	auto it = adjacent_lists.find(vertex);
	return it != adjacent_lists.end();
}

shared_vertex UndirectedGraph::getVertexById(VertexID id) const
{
	auto it = std::find_if(adjacent_lists.cbegin(),
		                   adjacent_lists.cend(), 
		                  [id](const std::pair< shared_vertex, std::set<shared_vertex>>& item)
		                  { return item.first->getID() == id;  });

	auto found = it != adjacent_lists.end();

	return found ? it->first : nullptr;
}

const std::set<shared_vertex>& UndirectedGraph::adjacentVerticesOf(shared_vertex vertex) const {
	assert(hasVertex(vertex));

	auto it = adjacent_lists.find(vertex);
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
	currentVertex->labelDiscovered();

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

