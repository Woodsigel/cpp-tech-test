#include "graph.h"
#include "graph.h"
#include <cassert>

bool Vertex::operator==(const Vertex& other) const
{
	return (*this).getID() == other.getID();
}

bool Vertex::operator!=(const Vertex& other) const
{
	return !((*this) == other);
}

void Vertex::setParent(std::shared_ptr<Vertex> _parent)
{
	assert(_parent->getID() != vertexID);

	parent = _parent;
}

bool Vertex::isParentOf(std::shared_ptr<Vertex> other)
{
	assert(nullptr != other);

	if (nullptr == other->parent)
		return false;
	
	return getID() == other->parent->getID();
}


bool operator<(const Vertex& lhs, const Vertex& rhs)
{
	return lhs.getID() < rhs.getID();
}

UndirectedGraph::UndirectedGraph(const std::vector<Edge>& edges)
{
	adjacent_lists = edgesToAdjacentLists(edges);
}

std::map<VertexID, std::set<shared_vertex>> UndirectedGraph::edgesToAdjacentLists(const std::vector<Edge>& edges)
{
	std::map<VertexID, std::set<shared_vertex>> result;

	for (auto&& edge : edges) {
		auto source_vertex = makeVertex(edge.source);
		auto destination_vertex = makeVertex(edge.destination);

		result[edge.source].insert(destination_vertex);
		result[edge.destination].insert(source_vertex);
	}

	return result;
}

shared_vertex UndirectedGraph::makeVertex(VertexID vertexID)
{
	auto it = vertices.find(vertexID);
	if (it != vertices.end())
		return it->second;
	
	auto newVertex = std::make_shared<Vertex>(vertexID);
	vertices.insert({ vertexID, newVertex });

	return newVertex;
}


bool UndirectedGraph::hasVertex(VertexID vertexID) const
{
	return vertices.find(vertexID) != vertices.end();
}



shared_vertex UndirectedGraph::getVertex(VertexID vertexID) const
{
	assert(hasVertex(vertexID));

	return vertices.find(vertexID)->second;
}

 std::set<shared_vertex>& UndirectedGraph::adjacentVerticesOf(const shared_vertex vertex)
{
	assert(nullptr != vertex);
	assert(hasVertex(vertex->getID()));

	auto it = adjacent_lists.find(vertex->getID());
	
	return it->second;
}


