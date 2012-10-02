/*
 *******************************************************************************
 *  Main.cpp
 *
 *******************************************************************************
 *
 * Copyright:   Q2WEB GmbH
 *              quality to the web
 *
 *              Tel  : +49 (0) 211 / 159694-00	Kronprinzenstr. 82-84
 *              Fax  : +49 (0) 211 / 159694-09	40217 Duesseldorf
 *              eMail: info@q2web.de	http://www.q2web.de
 *
 *
 * Author:     hadoop
 *
 * Created:     Sep 26, 2012
 *
 * Copyright (c) 2008 Q2WEB GmbH.
 * All rights reserved. 
 *
 *******************************************************************************
 */
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string>
#include <stdlib.h>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <sstream>
#include <fstream>

#include "Point.hpp"
#include "EuclideanGeometry.hpp"
#include "LinearApproximationReferenceAlgorithm.hpp"

int getdir(std::string dir, std::vector<std::string> &files) {
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		std::cout << "Error(" << errno << ") opening " << dir << std::endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		if (!(strncmp(dirp->d_name, ".", 1) == 0)
				&& !(strncmp(dirp->d_name, "..", 2) == 0)) {
			files.push_back(std::string(dirp->d_name));
		}
	}
	closedir(dp);
	return 0;
}

std::vector<com::acme::Point*> buildPointFromFile(std::string file) {
	std::vector<com::acme::Point*> testVec;
	std::string separator(";");
	boost::filesystem::path inputFilename = file;
	inputFilename.make_preferred();
	std::ifstream input(inputFilename.string().c_str(), std::ios::binary);
	if (!input) {
		std::cout << "Can't open input file " << inputFilename << std::endl;
		//exit(1);
	} else {
		std::cout << "Importing " << inputFilename << std::endl;
		int t = 1;
		int r = 1;

		std::string fileRow;
		int firstRow = 0;
		while (getline(input, fileRow)) {
//			std::cout << "Row " << r << ".fileRow " << fileRow << std::endl;
			if (!fileRow.empty()) {
				if (firstRow != 0) {
					t = 0;
					int time = 0;
					std::vector<double> points;
					typedef boost::split_iterator<std::string::iterator> string_split_iterator;
					for (string_split_iterator It = boost::make_split_iterator(
							fileRow,
							first_finder(separator, boost::is_iequal()));
							It != string_split_iterator(); ++It) {
						std::string token = boost::copy_range<std::string>(*It);
						if (t == 0) {
							time = atoi(token.c_str());
						} else {
							points.push_back(atof(token.c_str()));
						}
						//std::cout << t << ". : token : " << token << std::endl;

						t++;
						if (t == 3) {
							break;
						}
					}
					com::acme::Point* p = new com::acme::Point(time,
							&points[0]);
					testVec.push_back(p);
//					std::cout << " : Point : " << p->getTime() << " : "
//							<< p->get(1) << " : testVec :" << testVec.size()
//							<< std::endl;
				}
				firstRow++;
			}
			r++;
		}
	}

	return testVec;
}
int main() {

	std::vector<com::acme::Point*> testVec;

	std::string folderPath("../highway");
	std::vector<std::string> files = std::vector<std::string>();

	getdir(folderPath, files);

	size_t dest_size ;
	size_t src_size;

	for (unsigned int i = 0; i < files.size(); i++) {
		std::cout << files[i] << std::endl;
		std::vector<com::acme::Point*>	tmpTestVec = buildPointFromFile(folderPath + "/" + files[i]);
		dest_size=tmpTestVec.size();
		src_size=testVec.size();
		// Expand the source vector.
		testVec.reserve(src_size + dest_size);
		testVec.insert(testVec.begin() + src_size, tmpTestVec.begin(), tmpTestVec.end());
	}

	std::cout << "Result : " << testVec.size() << std::endl;

//	for (std::vector<com::acme::Point*>::iterator it = testVec.begin();
//			it != testVec.end(); ++it) {
//		com::acme::Point* p = *it;
//		std::cout << "Result : " << p->getTime() << " : " << p->get(1)
//				<< std::endl;
//	}

//	double startArray[] = { 1, 3 };
//	com::acme::Point* start = new com::acme::Point(startArray);
//	double bArray[] = { 2, 4 };
//	com::acme::Point* b = new com::acme::Point(bArray);
//	double cArray[] = { 3, 2 };
//	com::acme::Point* c = new com::acme::Point(cArray);
//	double dArray[] = { 4, 4 };
//	com::acme::Point* d = new com::acme::Point(dArray);
//	double eArray[] = { 5, 2 };
//	com::acme::Point* e = new com::acme::Point(eArray);
//	double fArray[] = { 6, 4 };
//	com::acme::Point* f = new com::acme::Point(fArray);
//	double endArray[] = { 7, 3 };
//	com::acme::Point* end = new com::acme::Point(endArray);
//
//	testVec.push_back(start);
//	testVec.push_back(b);
//	testVec.push_back(c);
//	testVec.push_back(d);
//	testVec.push_back(e);
//	testVec.push_back(f);
//	testVec.push_back(end);

	com::acme::EuclideanGeometry* eg = new com::acme::EuclideanGeometry();
	com::acme::LinearApproximationReferenceAlgorithm* algo =
			new com::acme::LinearApproximationReferenceAlgorithm(eg);
	std::vector<com::acme::Point*> result = algo->run(testVec, 1);

	std::cout << " Result Vector Size : " << result.size() << std::endl;
	for (std::vector<com::acme::Point*>::iterator it = result.begin();
			it != result.end(); ++it) {
		com::acme::Point* p = *it;
		std::cout << p->get(0) << " : " << p->get(1) << std::endl;
	}

//	delete start;
//	delete b;
//	delete c;
//	delete d;
//	delete e;
//	delete f;
//	delete end;

	delete algo;
	delete eg;

//	std::cout << "Hallo REf" << std::endl;

	return 0;
}

#ifdef POLO

#include <boost/config.hpp>
#include <iostream>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

int main(int, char *[]) {
	typedef adjacency_list<listS, vecS, directedS, no_property,
	property<edge_weight_t, int> > graph_t;
	typedef graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
	typedef graph_traits<graph_t>::edge_descriptor edge_descriptor;
	typedef std::pair<int, int> Edge;

	const int num_nodes = 5;
	enum nodes {
		A, B, C, D, E
	};
	char name[] = "ABCDE";
	Edge edge_array[] = {Edge(A, C), Edge(B, B), Edge(B, D), Edge(B, E), Edge(
				C, B), Edge(C, D), Edge(D, E), Edge(E, A), Edge(E, B)};
	int weights[] = {1, 2, 1, 2, 7, 3, 1, 1, 1};
	int num_arcs = sizeof(edge_array) / sizeof(Edge);
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
	graph_t g(num_nodes);
	property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
	for (std::size_t j = 0; j < num_arcs; ++j) {
		edge_descriptor e; bool inserted;
		boost::tie(e, inserted) = add_edge(edge_array[j].first, edge_array[j].second, g);
		weightmap[e] = weights[j];
	}
#else
	graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
	property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
#endif
	std::vector<vertex_descriptor> p(num_vertices(g));
	std::vector<int> d(num_vertices(g));
	vertex_descriptor s = vertex(A, g);

#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
	// VC++ has trouble with the named parameters mechanism
	property_map<graph_t, vertex_index_t>::type indexmap = get(vertex_index, g);
	dijkstra_shortest_paths(g, s, &p[0], &d[0], weightmap, indexmap,
			std::less<int>(), closed_plus<int>(),
			(std::numeric_limits<int>::max)(), 0,
			default_dijkstra_visitor());
#else
	dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));
#endif

	std::cout << "distances and parents:" << std::endl;
	graph_traits<graph_t>::vertex_iterator vi, vend;
	for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
		std::cout << "distance(" << name[*vi] << ") = " << d[*vi] << ", ";
		std::cout << "parent(" << name[*vi] << ") = " << name[p[*vi]]
		<< std::endl;
	}
	std::cout << std::endl;

	std::ofstream dot_file("figs/dijkstra-eg.dot");

	dot_file << "digraph D {\n" << "  rankdir=LR\n" << "  size=\"4,3\"\n"
	<< "  ratio=\"fill\"\n" << "  edge[style=\"bold\"]\n"
	<< "  node[shape=\"circle\"]\n";

	graph_traits<graph_t>::edge_iterator ei, ei_end;
	for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
		graph_traits<graph_t>::edge_descriptor e = *ei;
		graph_traits<graph_t>::vertex_descriptor u = source(e, g), v = target(e,
				g);
		dot_file << name[u] << " -> " << name[v] << "[label=\""
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

#endif
