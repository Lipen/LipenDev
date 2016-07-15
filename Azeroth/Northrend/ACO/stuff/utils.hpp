#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>
#include <utility>
#include <random>


struct Node;
struct Edge;
struct CompleteGraph;
struct Ant;

typedef std::vector<int> VI;
typedef std::vector<double> VD;
typedef std::vector<Edge*> VE;
typedef std::vector<Node*> VN;
typedef std::vector<Ant*> VA;
typedef std::vector<std::pair<double, Node*>> VPDN;

std::default_random_engine RandomEngine;
std::uniform_real_distribution<double> random;  // 0 .. 1
std::uniform_real_distribution<double> uni_real_dist(1.0, 5.0);

#include "main.hpp"
#include "Node.hpp"
#include "Edge.hpp"
#include "CompleteGraph.hpp"
#include "Ant.hpp"


template<typename T>
std::ostream& operator<< (std::ostream &out, std::vector<T> v) {
	out << "[";

	for (size_t i = 0; i < v.size(); ++i) {
		out << v[i] << (( i < v.size()-1 ) ? ", " : "]");
	}

	return out;
}
template<typename T>
std::ostream& operator<< (std::ostream &out, std::vector<T*> v) {
	out << "[";

	for (size_t i = 0; i < v.size(); ++i) {
		out << *v[i] << (( i < v.size()-1 ) ? ", " : "]");
	}

	return out;
}
template<>
std::ostream& operator<< (std::ostream &out, VPDN v) {
	out << "[";

	for (size_t i = 0; i < v.size(); ++i) {
		out << "{" << v[i].first << ":" << *v[i].second << "}" << (( i < v.size()-1 ) ? ", " : "]");
	}

	return out;
}

template<typename T>
void constrain(T &x, const T &x_min, const T &x_max) {
	if (x < x_min) x = x_min;
	if (x > x_max) x = x_max;
}

#endif // UTILS_HPP
