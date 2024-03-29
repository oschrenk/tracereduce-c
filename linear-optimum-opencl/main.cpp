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
#include <iomanip>
#include <vector>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <fstream>
#include <fstream>
#include <unistd.h>
#include <pwd.h>

#include "Point.hpp"
#include "LinearOptimumOpenClAlgorithm.hpp"

int getdir(std::string dir, std::vector<std::string> &files);
std::vector<com::acme::Point*> buildTraceFromFile(std::string file);
void writeResultToAFile(std::string file, std::size_t pointsBefore,
		std::size_t pointsAfter, long int computingTime);
std::vector<com::acme::Point*> getTestPoints();
void clearTrace(std::vector<com::acme::Point*> trace);
int64_t timespecDiff(struct timespec *endTime, struct timespec *startTime);

int main(int argc, char**argv) {
	double epsilon = 1;
	std::stringstream epsilonStr;
	int c = 0;
	while ((c = getopt(argc, argv, "e:")) != -1) {
		switch (c) {
		case 'e':
			epsilonStr << optarg;
			epsilonStr >> epsilon;
			break;
		default:
			std::cout << "usage : ./douglaPeucker -e t[t epsilon] "
					<< std::endl;
			break;
		}
	}

	//struct timespec startTime, endTime;
	timeval startTime, endTime;

	std::vector<com::acme::Point*> trace;

	struct passwd *pw = getpwuid(getuid());
	std::string homedir(pw->pw_dir);
	std::string folderPath(homedir + "/data/highway");
	std::vector<std::string> files = std::vector<std::string>();
	getdir(folderPath, files);

	std::string file = homedir + "/data/douglasPeucker-opencl.cvs";
	std::ofstream output(file.c_str());
	if (!output) {
		std::cout << "Can  not open file : " << file << std::endl;
		return 0;
	}
	output << "before;after;time" << std::endl;
	output.close();

	output.open(file.c_str(), std::ios::app);
	if (!output) {
		std::cout << "Can not open file : " << file << std::endl;
		return 0;
	}
	long int diffMicro = 0;
	//uint64_t diffMicro = 0;
//	com::acme::DeviceHandler* cpuDh = new com::acme::DeviceHandler(
//			CL_DEVICE_TYPE_CPU);
	com::acme::LinearOptimumOpenClAlgorithm* dprAlgo =
			new com::acme::LinearOptimumOpenClAlgorithm();

	for (unsigned int i = 0; i < 1; i++) {
		std::cout << files[i] << std::endl;
		//trace = buildTraceFromFile(folderPath + "/" + files[i]);
		trace = getTestPoints();

		std::cout << " Computing with : " << trace.size() << " Points "
				<< " and epsilon : " << epsilon << std::endl;

		gettimeofday(&startTime, NULL);
		//clock_gettime(CLOCK_MONOTONIC, &startTime);

		std::vector<com::acme::Point*> result = dprAlgo->run(trace, epsilon);

		gettimeofday(&endTime, NULL);
		//clock_gettime(CLOCK_MONOTONIC, &endTime);

		diffMicro = (endTime.tv_sec * 1e6 + endTime.tv_usec)
				- (startTime.tv_sec * 1e6 + startTime.tv_usec);

		//diffMicro = timespecDiff(&endTime, &startTime);

		try {
			output << trace.size() << ";" << result.size() << ";" << diffMicro
					<< std::endl;
			output.flush();
		} catch (std::exception& e) {
			std::cout << "Could not write data " << std::endl;

		}
		//	writeResultToAFile(homedir + "/data/dp.cvs", trace.size(),
		//			result.size(), diffMIcro);

		// TODO if program too slow comment out for-loop
		clearTrace(trace);
		//delete dprAlgo;
		//delete eg;
	}
	output.close();
	return 0;
}
int64_t timespecDiff(struct timespec *endTime, struct timespec *startTime) {
	return ((endTime->tv_sec * 1000000) + endTime->tv_nsec / 1000)
			- ((startTime->tv_sec * 1000000) + startTime->tv_nsec / 1000);
}

void clearTrace(std::vector<com::acme::Point*> trace) {

	for (std::vector<com::acme::Point*>::iterator it = trace.begin();
			it != trace.end(); ++it) {
		com::acme::Point* p = *it;
		delete p;
	}
	trace.clear();
}

void writeResultToAFile(std::string file, std::size_t pointsBefore,
		std::size_t pointsAfter, long int computingTime) {
	std::cout << " Result Vector Size : " << pointsAfter << std::endl;
	std::ofstream output(file.c_str());
	if (!output) {
		std::cout << "Cannot open file : " << file << std::endl;
	} else {
		try {
			output << "before;after;time" << std::endl;
			output << pointsBefore << ";" << pointsAfter << ";" << computingTime
					<< std::endl;

			output.close();
		} catch (std::exception& e) {
			std::cout << "Could not write data " << std::endl;

		}

	}
}

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

std::vector<com::acme::Point*> buildTraceFromFile(std::string file) {
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
					//std::vector<double> points;
					double* points = new double[2];
					typedef boost::split_iterator<std::string::iterator> string_split_iterator;
					for (string_split_iterator It = boost::make_split_iterator(
							fileRow,
							first_finder(separator, boost::is_iequal()));
							It != string_split_iterator(); ++It) {
						std::string token = boost::copy_range<std::string>(*It);
						if (t == 0) {
							time = atoi(token.c_str());
						} else {
							//points[t - 1] = atof(token.c_str());
							points[t - 1] = boost::lexical_cast<double>(
									token.c_str());
							//points.push_back(atof(token.c_str()));
						}
//						std::cout << t << ". : token : " << token
//								<< " : CONVERT: "<<std::setprecision(15)<<points[t - 1]
//								<< std::endl;

						t++;
						if (t == 3) {
							break;
						}
					}
					com::acme::Point* p = new com::acme::Point(time, points);
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
std::vector<com::acme::Point*> getTestPoints() {
	std::vector<com::acme::Point*> testVec;
	double startArray[] = { 1, 3 };
	com::acme::Point* start = new com::acme::Point(startArray);
	double bArray[] = { 2, 4 };
	com::acme::Point* b = new com::acme::Point(bArray);
	double cArray[] = { 3, 2 };
	com::acme::Point* c = new com::acme::Point(cArray);
	double dArray[] = { 4, 4 };
	com::acme::Point* d = new com::acme::Point(dArray);
	double eArray[] = { 5, 2 };
	com::acme::Point* e = new com::acme::Point(eArray);
	double fArray[] = { 6, 4 };
	com::acme::Point* f = new com::acme::Point(fArray);
	double endArray[] = { 7, 3 };
	com::acme::Point* end = new com::acme::Point(endArray);

	testVec.push_back(start);
	testVec.push_back(b);
	testVec.push_back(c);
	testVec.push_back(d);
	testVec.push_back(e);
	testVec.push_back(f);
	testVec.push_back(end);
	return testVec;
}

