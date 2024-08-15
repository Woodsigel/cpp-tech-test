#include <gmock/gmock.h>
#include <algorithm>

#include "graph.h"

using ::testing::Eq;


std::set<shared_vertex> makeVerticesFrom(std::set<VertexID> IDs) {
	std::set<shared_vertex> result;

	for (VertexID vertexID : IDs) {
		result.insert(std::make_shared<Vertex>(vertexID));
	}

	return result;
}

bool compareByID(const std::set<shared_vertex>& s1, const std::set<shared_vertex>& s2) {
	return std::is_permutation(s1.cbegin(), s1.cend(),
		              s2.cbegin(),
					  [](shared_vertex lhs, shared_vertex rhs)
					  { return lhs->getID() == rhs->getID();  });
}

shared_vertex item(const std::set<shared_vertex>& vertices) {
	assert(1 == vertices.size());

	return *vertices.begin();
}

TEST(VertexTest, vertexIDIsDeterminedDuringCreation) {
	Vertex v(0);

	ASSERT_THAT(v.getID(), Eq(0));
}

TEST(VertexTest, vertexIsUnDiscoveredAfterCreation) {
	Vertex v(1);

	ASSERT_THAT(v.isDiscovered(), Eq(false));
}

TEST(VertexTest, vertexIsDiscoveredAfterLabeling) {
	Vertex v(3);

	v.labelDiscovered();

	ASSERT_THAT(v.isDiscovered(), Eq(true));
}

TEST(VertexTest, vertexRelabeledAsUndiscoveredAfterReset) {
	Vertex v(4);
	v.labelDiscovered();
	ASSERT_THAT(v.isDiscovered(), Eq(true));

	v.reset();

	ASSERT_THAT(v.isDiscovered(), Eq(false));
}

TEST(VertexTest, parentOfVertexIsNullAfterReset) {
	Vertex v(5);
	auto parent = std::make_shared<Vertex>(6);
	v.setParent(parent);
	ASSERT_THAT(v.getParent(), Eq(parent));

	v.reset();

	ASSERT_THAT(v.getParent(), Eq(nullptr));
}

TEST(VertexTest, returnTrueIfIsAncestor) {
	auto v1 = std::make_shared<Vertex>(1);
	auto v2 = std::make_shared<Vertex>(2);
	auto v3 = std::make_shared<Vertex>(3);

	v2->setParent(v1);
	v3->setParent(v2);

	ASSERT_THAT(isAncestor(v1, v3), Eq(true));
}

TEST(VertexTest, returnFaseIfIsNotAncestor) {
	auto v1 = std::make_shared<Vertex>(1);
	auto v2 = std::make_shared<Vertex>(2);
	auto v3 = std::make_shared<Vertex>(3);

	v3->setParent(v2);
	v2->setParent(v1);

	auto anotherV1 = std::make_shared<Vertex>(1);
	ASSERT_THAT(anotherV1->getID(), Eq(v1->getID()));
	ASSERT_THAT(isAncestor(anotherV1, v3), Eq(false));
}

TEST(UndirectedGraphTest, makeVerticesFromEdges) {
	std::vector<Edge> edges = { {0, 1} };

	UndirectedGraph graph(edges);

	ASSERT_THAT(graph.getVertices().size(), Eq(2u));
}

TEST(UndirectedGraphTest, allVerticesAreUnDiscoveredAfterCreation) {
	std::vector<Edge> edges = { {0, 1}, {1, 2}, {2,3}, {3,4} };

	UndirectedGraph graph(edges);
	auto vertices = graph.getVertices();

	auto noneIsDiscovered = std::none_of(vertices.begin(), vertices.end(),
		                                 [](shared_vertex vertex) { return vertex->isDiscovered(); });
	ASSERT_TRUE(noneIsDiscovered);
}

TEST(UndirectedGraphTest, allVerticesAreUnDiscoveredAfterReset) {
	std::vector<Edge> edges = { {0, 1}, {1, 2}, {2,3}, {3,4} };
	UndirectedGraph graph(edges);
	auto v1 = graph.getVertexById(0);
	v1->labelDiscovered(); 
	auto vertices = graph.getVertices();
	auto atLeastOneIsDiscovered = std::any_of(vertices.begin(), vertices.end(),
		                                      [](shared_vertex vertex) { return vertex->isDiscovered(); });
	ASSERT_TRUE(atLeastOneIsDiscovered);

	graph.resetVertices();

	auto noneIsDiscovered = std::none_of(vertices.begin(), vertices.end(),
		                                 [](shared_vertex vertex) { return vertex->isDiscovered(); });
	ASSERT_TRUE(noneIsDiscovered);
}

TEST(UndirectedGraphTest, ignoreDuplicateEdge) {
	std::vector<Edge> edges = { {0, 1}, {0, 1} };

	UndirectedGraph graph(edges);

	ASSERT_THAT(graph.getVertices().size(), Eq(2u));
}

TEST(UndirectedGraphTest, queryExistedVertexById) {
	std::vector<Edge> edges = { {2, 3} };

	UndirectedGraph graph(edges);
	auto vertex = graph.getVertexById(2);

	ASSERT_THAT(vertex->getID(), Eq(2u));
	ASSERT_THAT(graph.hasVertex(vertex), true);
}

TEST(UndirectedGraphTest, vertexInsideGraphIsUnequalToTheOneOutsideGraph) {
	std::vector<Edge> edges = { {2, 3} };

	UndirectedGraph graph(edges);
	auto externalVertex = std::make_shared<Vertex>(2);

	ASSERT_THAT(externalVertex->getID(), Eq(2u));
	ASSERT_THAT(graph.hasVertex(externalVertex), Eq(false));
}

TEST(UndirectedGraphTest, returnNullWhenVertexIsNotFound) {
	std::vector<Edge> edges = { {2, 3} };

	UndirectedGraph graph(edges);
	auto vertex = graph.getVertexById(0);

	ASSERT_THAT(vertex, Eq(nullptr));
}

TEST(UndirectedGraphTest, makeAdjacentListsFromEdges) {
	std::vector<Edge> edges = { {4, 1}, {4, 2} };
	auto expect = makeVerticesFrom({ 1, 2 });

	UndirectedGraph graph(edges);
	auto vertex = graph.getVertexById(4);
	auto& actual = graph.adjacentVerticesOf(vertex);

	ASSERT_TRUE(compareByID(actual, expect));
}

TEST(UndirectedGraphTest, adjacentListsIsUndirected) {
	std::vector<Edge> edges = { {2, 5} };

	UndirectedGraph graph(edges);
	auto v1 = graph.getVertexById(2);
	auto v2 = graph.getVertexById(5);

	auto neighborOfV1 = graph.adjacentVerticesOf(v1);
	auto neighborOfV2 = graph.adjacentVerticesOf(v2);

	ASSERT_THAT(item(neighborOfV1)->getID(), Eq(v2->getID()));
	ASSERT_THAT(item(neighborOfV2)->getID(), Eq(v1->getID()));
}

TEST(UndirectedGraphTest, adjacentListsIgnoreSelfLoop) {
	std::vector<Edge> edges = { {10, 9} , {10, 10} };

	UndirectedGraph graph(edges);

	auto vertex = graph.getVertexById(10);
	auto neighbors = graph.adjacentVerticesOf(vertex);

	ASSERT_THAT(neighbors.size(), Eq(1));
	ASSERT_THAT(item(neighbors)->getID(), Eq(9));
}

TEST(UndirectedGraphTest, vertexInsideGraphIsUnique) {
	std::vector<Edge> edges = { {1, 2} };
	UndirectedGraph graph(edges);
	auto v1 = graph.getVertexById(1);
	auto v2 = graph.getVertexById(2);
	auto neighborOfV2 = item(graph.adjacentVerticesOf(v2));
	ASSERT_TRUE(neighborOfV2->getID() == v1->getID());
	ASSERT_THAT(v1->isDiscovered(), Eq(false));

	v1->labelDiscovered();

	ASSERT_THAT(neighborOfV2->isDiscovered(), Eq(true));
	ASSERT_THAT(v1->isDiscovered(), Eq(true));
}



class DepthFirstVisitorTest : public ::testing::Test {
public:

	DepthFirstVisitor depthFirstVisitor;

};


TEST_F(DepthFirstVisitorTest, noExaminerIsInvokedWhenGraphOnlyContainsOneVertex)
{
	std::vector<Edge> edges = { {1, 1} };
	UndirectedGraph graph(edges);
	ASSERT_THAT(graph.getVertices().size(), Eq(1));
	auto source = graph.getVertexById(1);

	bool onceInvoked = false;
	depthFirstVisitor.registerTreeEdgeExaminer([&onceInvoked](const shared_vertex _source, const shared_vertex _target) {
		onceInvoked = true;
		});
	depthFirstVisitor.search(graph, source);

	ASSERT_FALSE(onceInvoked);
}

TEST_F(DepthFirstVisitorTest, sourceIsParentOfTarget)
{
	std::vector<Edge> edges = { {1, 2} };
	UndirectedGraph graph(edges);
	auto source = graph.getVertexById(1);

	bool isParent = false;
	depthFirstVisitor.registerTreeEdgeExaminer([&isParent](const shared_vertex source, const shared_vertex target) {
		isParent = source->isParentOf(target);
		});
	depthFirstVisitor.search(graph, source);

	ASSERT_TRUE(isParent);
}