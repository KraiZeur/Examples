#include <iostream>

class Node {
private:
public:
};

class Path {
private:
	std::vector<sf::Vector2i> _waypoints;
public:
	void addPoint(sf::Vector2i point) {
		_waypoints.push_back(point);
	}
};

class AStar {
private:
public:
	Path* evaluate(sf::Vector2i start, sf::Vector2i end) {

		Path* path = new Path;

		// Push start node To Open

		while(!open.empty()) {
			// findBestNode From Open List
			// Node* node=open->pickBest();

			// Move best node to Close List
			// close->push_back(node);

			// if(node->is(goal)) end = true;
			// else

			// Foreach available movement from node
			// Check point isn't in open list


		}

		return path;

	}
};

int main(int argc, char const *argv[]) {

	AStar astar;

	Path* path = astar->evaluate({0,1}, {0,2});

	return 0;
}