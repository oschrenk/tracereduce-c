/*
 *******************************************************************************
 *  LinearApproximationReferenceAlgorithm.cpp
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
 * Created:     Sep 29, 2012
 *
 * Copyright (c) 2008 Q2WEB GmbH.
 * All rights reserved. 
 *
 *******************************************************************************
 */

#include "LinearApproximationReferenceAlgorithm.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>

namespace com {
namespace acme {
int LinearApproximationReferenceAlgorithm::DEFAULT_WEIGHT = 1;

LinearApproximationReferenceAlgorithm::LinearApproximationReferenceAlgorithm() {
	std::cout << "Const LinearApproximationReferenceAlgorithm" << std::endl;

}

LinearApproximationReferenceAlgorithm::~LinearApproximationReferenceAlgorithm() {
	std::cout << "Dest LinearApproximationReferenceAlgorithm" << std::endl;
}
LinearApproximationReferenceAlgorithm::LinearApproximationReferenceAlgorithm(
		Distance* distance) {
	this->distance = distance;
}

std::vector<Point*> LinearApproximationReferenceAlgorithm::run(
		std::vector<Point*> trace, double epsilon) {

	std::vector<Point*> polylineSimplification = getPolylineSimplification(
			trace, epsilon);

	std::vector<Point*> simplifiedTrace;

	std::vector<Point*>::iterator point;
	for (point = polylineSimplification.begin();
			point != polylineSimplification.end(); ++point) {
		simplifiedTrace.push_back(*point);
	}

	return simplifiedTrace;
}

std::vector<Point*> LinearApproximationReferenceAlgorithm::getPolylineSimplification(
		std::vector<Point*> trace, double epsilon) {

	// create the edges
	std::vector<Edge> edges = setupEdges(trace, epsilon);

	int num_nodes = trace.size();
	int num_arcs = edges.size();

	int weights[edges.size()];
	for (int i = 0; i < num_arcs; ++i) {
		weights[i] = 1;
	}

	// init dijkstra
	Edge* edge_array = &edges[0];
	graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
	property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
	std::vector<vertex_descriptor> p(num_vertices(g));
	std::vector<int> d(num_vertices(g));
	vertex_descriptor source = vertex(0, g);

	// run dijkstra
	dijkstra_shortest_paths(g, source,
			predecessor_map(&p[0]).distance_map(&d[0]));

	std::cout << "distances and parents:" << std::endl;
	graph_traits<graph_t>::vertex_iterator vi, vend;
	for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
		std::cout << "distance(" << *vi << ") = " << d[*vi] << ", ";
		std::cout << "parent(" << *vi << ") = " << p[*vi] << std::endl;
	}

	std::cout << "pa(" << p.at(p.size() - 1) << ")" << std::endl;

	std::vector<Point*> simplifiedTrace;
	int currentIndex = trace.size() - 1;
	simplifiedTrace.push_back(trace.at(currentIndex));
	while ((currentIndex = p.at(currentIndex)) != 0) {
		simplifiedTrace.push_back(trace.at(currentIndex));
	}
	simplifiedTrace.push_back(trace.at(0));

	//return dijkstra.getPath(vertices.at(vertices.size() - 1));
	std::reverse(simplifiedTrace.begin(), simplifiedTrace.end());

	return simplifiedTrace;
}

/**
 * This method creates the edge list for a given list of nodes. In general,
 * there exist edges between each two successive nodes. Additional edges are
 * created between two nodes 'from' and 'to', if all nodes \in ]from; to[
 * can be interpolated without an interpolation error exceeding epsilon
 *
 * @param nodes
 *            the nodes
 * @param epsilon
 *            the epsilon
 * @return the list
 */
std::vector<Edge> LinearApproximationReferenceAlgorithm::setupEdges(
		std::vector<Point*> points, double epsilon) {

	std::vector<Edge> edgeList;
	int numberOfNodes = points.size();

	// connect each two successive nodes by an edge:
	for (int from = 0; from < numberOfNodes - 1; from++) {
		edgeList.push_back(Edge(from, from + 1));
	}

	// 'lineStartIndex' and 'lineEndIndex' create virtual edge
	// calculate distance from intermediatePointIndex to this edge
	for (int lineStartIndex = 0; lineStartIndex < numberOfNodes - 2;
			lineStartIndex++) {
		for (int lineEndIndex = lineStartIndex + 2;
				lineEndIndex < numberOfNodes; lineEndIndex++) {
			double maxDelta = 0.0;
			for (int intermediatePointIndex = lineStartIndex + 1;
					intermediatePointIndex < lineEndIndex;
					intermediatePointIndex++) {
				double delta = distance->distance(
						*points.at(intermediatePointIndex),
						*points.at(lineStartIndex), *points.at(lineEndIndex));

				if (delta > maxDelta) {
					maxDelta = delta;
				}
			}
			if (maxDelta <= epsilon) {
				edgeList.push_back(Edge(lineStartIndex, lineEndIndex));
			}
		}
	}

	return edgeList;
}
}
} /* namespace com */
