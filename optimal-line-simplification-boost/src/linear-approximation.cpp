#include <boost/config.hpp>
#include <iostream>
#include <fstream>

// boost utils
#include <boost/tuple/tuple.hpp>
#include <boost/foreach.hpp>

// boost geometry
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

// boost graph
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// http://geometrylibrary.geodan.nl/07__graph__route__example_8cpp-example.html

using namespace boost;

int main(int, char *[]) {
	// For now just use a sphere. Ellipsoids are in svn trunk but unsupported
	typedef boost::geometry::model::point<double, 2, boost::geometry::cs::spherical_equatorial<boost::geometry::degree> > spherical_point;
	// A linestring (named so by OGC) is a collection (default a vector) of points. 
	typedef boost::geometry::model::linestring<spherical_point> line_type;

	// Build the graph
	// listS, vecS, ... are (empty) default structs
	typedef adjacency_list<listS, vecS, directedS, no_property, property<edge_weight_t, int> > graphType;
	typedef graph_traits<graphType>::vertex_descriptor vertex_descriptor;
	typedef graph_traits<graphType>::edge_descriptor edge_descriptor;
	typedef std::pair<spherical_point, spherical_point> Edge;

	// Add example entries
	line_type line;
	//line += xy(1.1, 1.1), xy(2.5, 2.1), xy(3.1, 3.1), xy(4.9, 1.1), xy(3.1, 1.9);
	line.push_back(spherical_point(1.1, 1.1));
	line.push_back(spherical_point(2.5, 2.1));
	line.push_back(spherical_point(3.1, 3.1));
	line.push_back(spherical_point(4.9, 1.1));
	line.push_back(spherical_point(3.1, 1.9));

	line_type simplified;

	int numberOfNodes = line.size();
	char nodeNames[] = "ABCDE";

	Edge edges[] = { Edge(line.at(0), line.at(1)) };
	int edgeWeights[] = { 1 };
	int numberOfArcs = sizeof(edges) / sizeof(Edge);

	graphType g(edges, edges + numberOfArcs, edgeWeights, numberOfNodes);
	property_map<graphType, edge_weight_t>::type weightmap = get(edge_weight, g);

	std::vector<vertex_descriptor> p(num_vertices(g));
	std::vector<int> d(num_vertices(g));
	vertex_descriptor s = vertex(1, g);

	dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));


	std::cout << "distances and parents:" << std::endl;
	graph_traits<graphType>::vertex_iterator vi, vend;
	for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
		std::cout << "distance(" << nodeNames[*vi] << ") = " << d[*vi] << ", ";
		std::cout << "parent(" << nodeNames[*vi] << ") = " << nodeNames[p[*vi]]
				<< std::endl;
	}
	std::cout << std::endl;

	std::ofstream dot_file("figs/dijkstra-eg.dot");

	dot_file << "digraph D {\n" << "  rankdir=LR\n" << "  size=\"4,3\"\n"
			<< "  ratio=\"fill\"\n" << "  edge[style=\"bold\"]\n"
			<< "  node[shape=\"circle\"]\n";

	graph_traits<graphType>::edge_iterator ei, ei_end;
	for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
		graph_traits<graphType>::edge_descriptor e = *ei;
		graph_traits<graphType>::vertex_descriptor u = source(e, g), v = target(e,
				g);
		dot_file << nodeNames[u] << " -> " << nodeNames[v] << "[label=\""
				<< get(weightmap, e) << "\"";
		if (p[v] == u)
			dot_file << ", color=\"black\"";
		else
			dot_file << ", color=\"grey\"";
		dot_file << "]";
	}
	dot_file << "}";
	return EXIT_SUCCESS;
}
