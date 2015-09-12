#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>

#define pb push_back

using std::cout;
using std::cin;
using std::string;

class Vertex;
class Edge;
class Graph;


class Vertex {
	string name;
	std::set<Edge*> outbound;

	bool visited = false;
	double weight = 0.0;

 public:
	Vertex(string name) : name(name) {}

	string toString() const {
		return name;
	}

	void linkEdge(Edge* e) {
		outbound.insert(e);
	}

	bool isVisited() const {
		return visited;
	}

	void visit() {
		visited = true;
	}

	std::set<Edge*>& getOutbound() {
		return outbound;
	}

	double getWeight() const {
		return weight;
	}
};


class Edge {
	Vertex* start;
	Vertex* finish;

	bool visited = false;
	double weight = 0.0;

 public:
	Edge(Vertex* v1, Vertex* v2) {
		start = v1;
		finish = v2;
		v1->linkEdge(this);
	}

	bool isEqual(Edge* e) const {
		return e == this;  // works?
	}
	bool isEqual(Vertex* v1, Vertex* v2) const {
		return start == v1 && finish == v2;
	}
	bool isEqual(string v1_name, string v2_name) const {
		return start->toString() == v1_name &&
			   finish->toString() == v2_name;
	}

	void applyAdjList(std::map<string, std::set<Vertex*>>& adj_list) {
		adj_list[start->toString()].insert(finish);
	}
};


class Graph {
	std::set<Vertex*> vertices;
	std::set<Edge*> edges;
	std::map<string, std::set<Vertex*>> adj_list;

 public:
	Graph() {}

	Vertex* addVertex(Vertex* v) {
		std::set<Vertex*> adj_to;  // tmp
		adj_list[v->toString()] = adj_to;
		vertices.insert(v);
		return v;
	}
	Vertex* addVertex(string name) {
		Vertex* v = new Vertex(name);
		return addVertex(v);
	}

	Edge* addEdge(Edge* e) {
		e->applyAdjList(adj_list);
		edges.insert(e);
		return e;
	}
	Edge* addEdge(Vertex* v1, Vertex* v2) {
		Edge* e = new Edge(v1, v2);
		return addEdge(e);
	}
	Edge* addEdge(string v1_name, string v2_name) {
		Vertex* v1 = nullptr;
		Vertex* v2 = nullptr;

		for (Vertex* v : vertices) {
			string name = v->toString();
			if (name == v1_name) {
				v1 = v;
			}
			if (name == v2_name) {
				v2 = v;
			}
		}

		if (v1 == nullptr || v2 == nullptr) {
			throw "Vertex not found.";
		}

		return addEdge(v1, v2);
	}

	Vertex* getVertex(string v_name) {
		for (Vertex* v : vertices) {
			if (v->toString() == v_name) {
				return v;
			}
		}
		return nullptr;
	}

	Edge* getEdge(Vertex* v1, Vertex* v2) {
		for (Edge* e : edges) {
			if (e->isEqual(v1, v2)) {
				return e;
			}
		}
		return nullptr;
	}
	Edge* getEdge(string v1_name, string v2_name) {
		for (Edge* e : edges) {
			if (e->isEqual(v1_name, v2_name)) {
				return e;
			}
		}
		return nullptr;
	}

	bool containsVertex(Vertex* v) const {
		return vertices.find(v) != vertices.end();
	}
	bool containsVertex(string v_name) {
		// for (Vertex* v : vertices) {
		// 	if (v->toString() == v_name) {
		// 		return true;
		// 	}
		// }
		// return false;
		return getVertex(v_name) != nullptr;
	}

	bool containsEdge(Edge* e) const {
		return edges.find(e) != edges.end();
	}
	bool containsEdge(Vertex* v1, Vertex* v2) {
		// for (Edge* e : edges) {
		// 	if (e->isEqual(v1, v2)) {
		// 		return true;
		// 	}
		// }
		// return false;
		return getEdge(v1, v2) != nullptr;
	}
	bool containsEdge(string v1_name, string v2_name) {
		// for (Edge* e : edges) {
		// 	if (e->isEqual(v1_name, v2_name)) {
		// 		return true;
		// 	}
		// }
		// return false;
		return getEdge(v1_name, v2_name) != nullptr;
	}

	Vertex* DFS_explore(Vertex* v, string finish_name) {
		v->visit();
		if (v->toString() == finish_name) {
			return v;
		}

		for (Vertex* u : adj_list[v->toString()]) {
			if (!u->isVisited()) {
				return DFS_explore(u, finish_name);
			}
		}

		return nullptr;
	}

	Vertex* DFS(Vertex* start, string finish_name) {
		return DFS_explore(start, finish_name);
	}
};


std::ostream& operator<<(std::ostream& out, const Vertex& v) {
	out << "Vertex{name=" << v.toString();
	if (v.getWeight() != 0.0) {
		out << ", weight=" << v.getWeight();
	}
	out << ", visited=" << (v.isVisited() ? "T" : "F");
	out << "}";
	return out;
}


int main() {
	Graph g;

	Vertex* v1 = new Vertex("1");
	g.addVertex(v1);
	for (int i = 2; i <= 10; i++) {
		g.addVertex(std::to_string(i));
	}

	g.addEdge("1", "2");
	g.addEdge("2", "4");
	g.addEdge("4", "3");
	g.addEdge("3", "5");
	g.addEdge("3", "6");
	g.addEdge("6", "5");
	g.addEdge("5", "7");
	g.addEdge("2", "8");
	g.addEdge("2", "9");
	g.addEdge("3", "9");
	g.addEdge("4", "9");

	cout << sizeof(g) << std::endl;

	cout << (g.containsEdge("1", "2")?"dbg:true":"dbg:false") << std::endl;
	cout << (g.containsEdge("2", "4")?"dbg:true":"dbg:false") << std::endl;
	cout << (g.containsVertex("9")?"dbg:true":"dbg:false") << std::endl;
	cout << (g.containsVertex("42")?"dbg:true":"dbg:false") << std::endl;

	Vertex* v7 = g.DFS(v1, "7");

	cout << "Found v7: " << *v7 << std::endl;
}
