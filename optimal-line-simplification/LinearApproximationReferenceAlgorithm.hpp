/*
 *******************************************************************************
 *  LinearApproximationReferenceAlgorithm.hpp
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
 * Author:     Oliver Schrenk <oliver.schrenk@q2web.de>
 *
 * Copyright (c) 2008 Q2WEB GmbH.
 * All rights reserved.
 *
 *******************************************************************************
 */

#ifndef LINEARAPPROXIMATIONREFERENCEALGORITHM_HPP_
#define LINEARAPPROXIMATIONREFERENCEALGORITHM_HPP_

#include <iostream>
#include <vector>
#include "Distance.hpp"
#include "Point.hpp"

#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

typedef adjacency_list<listS, vecS, directedS, no_property,
		property<edge_weight_t, int> > graph_t;
typedef graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
typedef graph_traits<graph_t>::edge_descriptor edge_descriptor;

namespace com {
namespace acme {
typedef std::pair<int, int> Edge;

class LinearApproximationReferenceAlgorithm {
public:
	LinearApproximationReferenceAlgorithm();
	LinearApproximationReferenceAlgorithm(Distance* distance);
	virtual ~LinearApproximationReferenceAlgorithm();

	std::vector<Point*> run(std::vector<Point*> trace, double epsilon);
	std::vector<Point*> getPolylineSimplification(std::vector<Point*> trace,
			double epsilon);
	std::vector<Edge> setupEdges(std::vector<Point*> nodes, double epsilon);

private:
	static int DEFAULT_WEIGHT;

	/** The geometry. */
	Distance* distance;
};
}
} /* namespace com */
#endif /* LINEARAPPROXIMATIONREFERENCEALGORITHM_HPP_ */
