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
	if (nullptr == decendant->getParent())
		return false;

	auto parent = decendant->getParent();
	while (nullptr != parent) {
		if (parent.get() == ancestor.get())
			return true;

		parent = parent->getParent();
	}

	return false;
}

void Vertex::setParent(std::shared_ptr<Vertex> _parent)
{
	assert(nullptr != _parent);
	assert(_parent.get() != this && _parent->getID() != this->getID());

	parent = _parent;
}

std::shared_ptr<Vertex> Vertex::getParent() const
{
	return parent;
}

bool Vertex::isParentOf(std::shared_ptr<Vertex> other)
{
	assert(nullptr != other);

	if (nullptr == other->parent)
		return false;

	return getID() == other->parent->getID();
}

void Vertex::reset()
{
	bDiscovered = false;
	parent = nullptr;
}


UndirectedGraph::UndirectedGraph(const std::vector<Edge>& edges) {
	for (auto&& edge : edges) {
		addAdjacentListFrom(edge);
	}
}


void UndirectedGraph::addAdjacentListFrom(const Edge& edge)
{
	auto source_vertex = makeVertex(edge.source);
	auto target_vertex = makeVertex(edge.target);

	if (source_vertex->getID() != target_vertex->getID()) {
		adjacent_lists[source_vertex].insert(target_vertex);
		adjacent_lists[target_vertex].insert(source_vertex);
		return;
	}

	if(!hasVertex(source_vertex))
		adjacent_lists[source_vertex] = std::set<shared_vertex>();
}

shared_vertex UndirectedGraph::makeVertex(VertexID id)
{
	auto it = std::find_if(adjacent_lists.begin(), 
		                   adjacent_lists.end(),
							[id](const std::pair< shared_vertex, std::set<shared_vertex>>& item)
							{
								return item.first->getID() == id;
							});
	bool isExisted = it != adjacent_lists.end();
	if(isExisted)
		return it->first;

	auto newVertex = std::make_shared<Vertex>(id);
	return newVertex;
}

std::set<shared_vertex> UndirectedGraph::getVertices() const
{

	std::set<shared_vertex> result;

	std::transform(adjacent_lists.begin(), 
		           adjacent_lists.end(),
				   std::inserter(result, result.begin()),
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

	recur_search(graph, source);
}

void DepthFirstVisitor::recur_search(UndirectedGraph& graph, shared_vertex currentVertex)
{
	currentVertex->labelDiscovered();

	for (shared_vertex neighbor : graph.adjacentVerticesOf(currentVertex)) {
		if (!neighbor->isDiscovered()) {
			neighbor->setParent(currentVertex);
			treeEdgeExaminer(currentVertex, neighbor);

			recur_search(graph, neighbor);
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

