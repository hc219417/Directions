/**
 * @file: directions.cc
 * @author: Hannah Culver
 * @date: November 9, 2022
 * @brief: write a c++ program that creates a graph from the given file and determines
 *         the shortest route between two cities or all cities given a starting point
 * 
 * @objective: implement a graph and use it to find the shortest path between two nodes
 * @pat: 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
 */

#include "dijkstra.h"
using namespace std;

/**
 * displayUsage
 * @param none
 * @return display usage options
 */
void displayUsage();

int main(int argc, char const *argv[]){

    Dijkstra mygraph;

    if(argc < 1 || argc > 4){ //check for argument count, must be between 1 and 4
		cout << "Error: invalid command line argument" << endl;
		exit(0);
	}

    if(argc == 1){ //argument count equals 1
        if(std::string(argv[0]) != "./route"){
            cout << "Error: invalid command line argument" << endl;
		    exit(0);
        }
        displayUsage(); //display a usage message
    }

    if(argc == 2){ //argument count equals 2
        if(std::string(argv[0]) != "./route"){
            cout << "Error: invalid command line argument" << endl;
		    exit(0);
        }
        mygraph.show_vertices(std::string(argv[1])); //show all the available vertices
    }

    if(argc == 3){ //argument count equals 3
        if(std::string(argv[0]) != "./route"){
            cout << "Error: invalid command line argument" << endl;
		    exit(0);
        }
        mygraph.shortest_path_all(std::string(argv[1]), std::string(argv[2])); //display the shortest paths from source to all the destinations
    }

    else if(argc == 4){ //argument count equals 4
        if(std::string(argv[0]) != "./route"){
            cout << "Error: invalid command line argument" << endl;
		    exit(0);
        }
        mygraph.shortest_path_one(std::string(argv[1]), std::string(argv[2]), std::string(argv[3])); //display the route, distances, and total distance
    }

    return 0;
}

void displayUsage(){
    cout << "sample runs:" << endl;
    cout << "./route mygraph.txt Athens Cleveland (display the route, distances, and total distance)" << endl;
    cout << "./route mygraph.txt Athens London (display no route exists, destination unreachable)" << endl;
    cout << "./route mygraph.txt London Athens (display no route exists, source location does not exist)" << endl;
    cout << "./route mygraph.txt Athens (display the shortest paths from Athens to all the destinations)" << endl;
    cout << "./route mygraph.txt (show all the available vertices)" << endl;
    cout << "./route (display a usage message)" << endl;
}