#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <iostream>
#include <memory>
#include <tuple>
#include <vector>
#include <list>
#include <deque>

#include <boost/graph/vector_as_graph.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/property_map/property_map.hpp>

using namespace std;
using namespace boost;

template <typename Graph, typename VertexNameMap, typename TransDelayMap>
void build_router_network(
		Graph& g
		, VertexNameMap& name_map
		, TransDelayMap& delay_map
		)/*{{{*/
{
	auto a = add_vertex(g); name_map[a] = 'a';
	auto b = add_vertex(g); name_map[b] = 'b';
	auto c = add_vertex(g); name_map[c] = 'c';
	auto d = add_vertex(g); name_map[d] = 'd';
	auto e = add_vertex(g); name_map[e] = 'e';

	typename graph_traits<Graph>::edge_descriptor ed;
	bool inserted;
	tie(ed, inserted) = add_edge(a, b, g); delay_map[ed] = 1.2;
	tie(ed, inserted) = add_edge(a, d, g); delay_map[ed] = 4.5;
	tie(ed, inserted) = add_edge(b, b, g); delay_map[ed] = 1.8;
	tie(ed, inserted) = add_edge(c, a, g); delay_map[ed] = 2.6;
	tie(ed, inserted) = add_edge(c, e, g); delay_map[ed] = 5.2;
	tie(ed, inserted) = add_edge(d, c, g); delay_map[ed] = 0.4;
	tie(ed, inserted) = add_edge(d, e, g); delay_map[ed] = 3.3;
}/*}}}*/

template <typename VertexDescriptor, typename VertexNameMap>
void print_vertex_name(VertexDescriptor v, VertexNameMap& name_map)
{ cout << name_map[v]; }

template <typename Graph, typename TransDelayMap, typename VertexNameMap>
void print_trans_delay(
		typename graph_traits<Graph>::edge_descriptor e
		, const Graph& g
		, TransDelayMap& delay_map
		, VertexNameMap& name_map
		)/*{{{*/
{
	cout << "trans-delay(";
	cout << name_map[source(e, g)] << ",";
	cout << name_map[target(e, g)] << ") = ";
	cout << delay_map[e];
}/*}}}*/

template <typename VertexNameMap>
class bfs_name_printer : public default_bfs_visitor/*{{{*/
{
public:
	bfs_name_printer(VertexNameMap& n_map) 
		: m_name_map_(n_map)
	{}

	template <typename Vertex, typename Graph>
	void discover_vertex(Vertex u, const Graph&) const
	{ cout << m_name_map_[u] << ' '; }

private:
	VertexNameMap& m_name_map_;
};/*}}}*/

template <typename Graph, typename VertexDescriptor, typename VertexNameMap>
void print_bfs_name(Graph& g, VertexDescriptor v, VertexNameMap& name_map)/*{{{*/
{
	bfs_name_printer<VertexNameMap> vis(name_map);
	cout << "BFS vertex discover order: ";
	breadth_first_search(g, v, visitor(vis));
	cout << endl;
}/*}}}*/

class BglTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(BglTest);
	CPPUNIT_TEST(printGraph);
	CPPUNIT_TEST(topologicalSort);
	CPPUNIT_TEST_SUITE_END();

public:
	void printGraph()/*{{{*/
	{
		typedef adjacency_list<vecS, vecS, bidirectionalS> Graph;
		Graph g;
		map<graph_traits<Graph>::vertex_descriptor, string> name_map;
		map<graph_traits<Graph>::edge_descriptor, double> delay_map;

		build_router_network(g, name_map, delay_map);
		cout << endl;
		for(auto p = vertices(g); p.first != p.second; ++p.first) {
			print_vertex_name(*p.first, name_map);
			cout << endl;
		}
		cout << endl;
		for(auto p = edges(g); p.first != p.second; ++p.first) {
			print_trans_delay(*p.first, g, delay_map, name_map);
			cout << endl;
		}

		print_bfs_name(g, *vertices(g).first, name_map);

	}/*}}}*/

	void topologicalSort()/*{{{*/
	{
		const char* tasks[] = {
			"pick up kids from school"
			, "buy groceries (and snacks)"
			, "get cache at ATM"
			, "drop off kids at soccer practice"
			, "cook dinner"
			, "pick up kids from soccer"
			, "eat dinner"
		};
		const int n_tasks = sizeof(tasks) / sizeof(tasks[0]);
#if 0
		vector<list<int>> g(n_tasks);

		g[0].push_back(3);
		g[1].push_back(3);
		g[1].push_back(4);
		g[2].push_back(1);
		g[3].push_back(5);
		g[4].push_back(6);
		g[5].push_back(6);
#else
		adjacency_list<listS, vecS, directedS> g(n_tasks);
		add_edge(0, 3, g);
		add_edge(1, 3, g);
		add_edge(1, 4, g);
		add_edge(2, 1, g);
		add_edge(3, 5, g);
		add_edge(4, 6, g);
		add_edge(5, 6, g);
#endif

		deque<int> topo_order;

		topological_sort(
				g
				, front_inserter(topo_order)
				, vertex_index_map(identity_property_map())
				);	

		int n = 1;
		cout << endl;
		for(auto n : topo_order) {
			cout << tasks[n] << endl;
		}
	}/*}}}*/
};

CPPUNIT_TEST_SUITE_REGISTRATION(BglTest);
