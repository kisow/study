#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <iostream>
#include <memory>
#include <tuple>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/vector_as_graph.hpp>
#include <boost/property_map/property_map.hpp>

using namespace std;
using namespace boost;

template <typename Graph, typename VertexNameMap, typename TransDelayMap>
void build_router_network(
		Graph& g
		, VertexNameMap& name_map
		, TransDelayMap& delay_map
		)
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
}

template <typename VertexDescriptor, typename VertexNameMap>
void print_vertex_name(VertexDescriptor v, VertexNameMap& name_map)
{
	cout << get(name_map, v);
}

class BglTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(BglTest);
	CPPUNIT_TEST(check);
	CPPUNIT_TEST_SUITE_END();

	typedef adjacency_list<vecS, vecS, bidirectionalS> Graph;

public:
	void check()/*{{{*/
	{
		Graph g;
		map<graph_traits<Graph>::vertex_descriptor, string> name_map;
		map<graph_traits<Graph>::edge_descriptor, double> delay_map;

		build_router_network(g, name_map, delay_map);
		for(auto p = vertices(g); p.first != p.second; ++p.first) {
			print_vertex_name(*p.first, name_map);
		}

	}/*}}}*/
};

CPPUNIT_TEST_SUITE_REGISTRATION(BglTest);
