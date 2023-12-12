/**
 * @file: dijkstra.h
 * @author: Hannah Culver
 * @date: November 9, 2022
 * @brief: header file for the dijkstra class
 */

#ifndef DIJKSTRA
#define DIJKSTRA

#include <stack>
#include <queue>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

struct Node{
    Node(){node = "", distance = 0, previous = "", visited = false, next = nullptr;} //zero-arg c-tor

    Node(string new_node, int new_distance, string new_previous, bool new_visited, Node *new_next){ //full service c-tor
        node = new_node;
        distance = new_distance;
        previous = new_previous;
        visited = new_visited;
        next = new_next;
    }

    string node;
    int distance;
    string previous;
    bool visited;
    Node *next;
};

struct Compare{ //compare distances (used to create min queue)
    bool operator() (Node *first, Node *second){
        return(first -> distance > second -> distance);
    }
};

class Dijkstra{
    public:
    Dijkstra(); //default constructor

    void build_graph(string input_filename); //build the graph given an input file (pretty much everything calls this function)

    void display_graph(string input_filename); //display the graph
    void show_vertices(string input_filename); //show the vertices

    void shortest_path(string input_filename, string source); //find the shortest path from a single starting point to all destinations

    void shortest_path_all(string input_filename, string source); //display the shortest path from source to all destinations
    void shortest_path_one(string input_filename, string source, string destination); //display the shortest path from source to a single destination

    int distance_between(string source, string destination); //find the distance between two nodes
    bool are_connected(string source, string destination); //determine whether two nodes are connected

    private:
    int vertices = 0; //number of vertices
    vector<Node*> nodes; //adjacency list
    vector<Node*> paths; //shortest paths
};

#endif

Dijkstra::Dijkstra(){ //default constructor (clear out both vectors)
    nodes.clear();
    paths.clear();
}

void Dijkstra::build_graph(string input_filename){
    ifstream inStream;
    inStream.open(input_filename); //open file
    if(inStream.fail()){
        cout << "Error: file not found" << endl;
        exit(0);
    }

    Node* cursor; //create cursor
    int count = 0; //create count

    stringstream ss; //create stringstream object
    string file_line; //line to be read from file

    string source, distance, destination;

    while(getline(inStream, file_line)){
        stringstream ss(file_line); //obtain line from the file
        ss >> source; //read in the first word as the name of the node
        nodes.push_back(new Node(source, 0, "", false, nullptr)); //load up the nodes vector
        paths.push_back(new Node(source, 10000, "n/a", false, nullptr)); //load up the paths vector
        cursor = nodes[count]; //set cursor equal to current index
        while(ss >> distance >> destination){ //read in the next two items as distance and destination
            cursor -> next = new Node(destination, stoi(distance), "", false, nullptr); //build the adjacency (linked) list
            cursor = cursor -> next; //update cursor
        }
        count++; //increment the count
    }

    if(nodes[count - 1] -> node == nodes[count - 2] -> node){
        nodes.pop_back();
        paths.pop_back();
        count--; //remove last entry if duplicate and decrement count
    }

    vertices = nodes.size(); //set number of vertices
    inStream.close(); //close file
}

void Dijkstra::display_graph(string input_filename){ //helper function (testing purposes)
    build_graph(input_filename);
    Node* cursor;
    for(int i = 0; i < vertices; i++){
        cursor = nodes[i];
        cout << cursor -> node;
        while(cursor -> next != nullptr){
            cout << " -> " << cursor -> next -> distance << " " << cursor -> next -> node;
            cursor = cursor -> next;
        }
        cout << endl;
    }
} //display the graph as an adjacency list (array of linked lists)

void Dijkstra::show_vertices(string input_filename){
    build_graph(input_filename);
    cout << "Vertices:" << endl;
    for(int i = 0; i < vertices; i++){
        cout << i + 1 << ". " << nodes[i] -> node << endl;
    }
}

int Dijkstra::distance_between(string source, string destination){
    int index = 0;
    for(int i = 0; i < vertices; i++){
        if(nodes[i] -> node == source){
            index = i;
            break;
        }
    }
    Node* cursor = nodes[index];
    if(cursor -> next != nullptr){
        cursor = cursor -> next;
        while(cursor != nullptr){
            if(cursor -> node == destination){
                return cursor -> distance;
            }
            cursor = cursor -> next;
        }
    }
    return -1; //should only happen if nodes are NOT connected
}              //i.e. no path exists from source to destination

bool Dijkstra::are_connected(string source, string destination){
    int index = 0;
    for(int i = 0; i < vertices; i++){
        if(nodes[i] -> node == source){
            index = i;
            break;
        }
    }
    Node* cursor = nodes[index];
    if(cursor -> next != nullptr){
        cursor = cursor -> next;
        while(cursor != nullptr){
            if(cursor -> node == destination){
                return true;
            }
            cursor = cursor -> next;
        }
    }
    return false; //for a directed graph this means there exists NO path from source to destination
}                 //though there could be one the other way around (i.e. from destination to source)

void Dijkstra::shortest_path(string input_filename, string source){
    build_graph(input_filename);

    priority_queue<Node*, vector<Node*>, Compare> q; //create priority queue object of Nodes using Compare structure

    bool found = false;
    for(int i = 0; i < vertices; i++){
        if(nodes[i] -> node == source){
            found = true;
        }
    }
    if(found == false){
        cout << "Error: no route exists, source location does not exist" << endl;
        exit(0);
    }

    //dijkstra's algorithm

    int index = 0;
    for(int i = 0; i < vertices; i++){
        if(paths[i] -> node == source){
            index = i;
            break;
        }
    } //find the index of the source node

    q.push(paths[index]); //push source node onto the queue
    paths[index] -> distance = 0; //set source node distance equal to 0

    while(!q.empty()){ //while the queue is not empty (i.e. not all the nodes have been visited)

        q.top() -> visited = true; //mark top node as visited

        for(int i = 0; i < vertices; i++){
            if(paths[i] -> visited == false){
                q.push(paths[i]);
            }
        } //load up the queue with all non-visited nodes

        Node* current = q.top(); //set aside top node by marking it as current (for distance and previous)
        q.pop(); //remove top node from the queue

        while(!q.empty()){ //while the queue is not empty
            if(are_connected(current -> node, q.top() -> node) && current -> distance + distance_between(current -> node, q.top() -> node) < q.top() -> distance){
                q.top() -> distance = current -> distance + distance_between(current -> node, q.top() -> node);
                q.top() -> previous = current -> node;
            } //if the current and top nodes are connected and the current distance plus the distance between them is less than the top node's current distance,
            q.pop(); //...set the top node's distance equal to the current distance plus the distance between them and update the top node's previous data field
        }

        for(int i = 0; i < vertices; i++){
            if(paths[i] -> visited == false){
                q.push(paths[i]); //load up the queue again
            }
        }
    }                                              
}

void Dijkstra::shortest_path_all(string input_filename, string source){
    shortest_path(input_filename, source);

    cout << "Shortest path from " << source << " to all destinations:" << endl;

    string destination; //current destination (soon to be iterated)
    string shortest_path; //shortest path string

    for(int i = 0; i < vertices; i++){ //go through all the destinations
        destination = nodes[i] -> node;
        if(paths[i] -> previous != "n/a"){ //no path exists between source and destination
            int dest_index = 0; //destination index
            for(int i = 0; i < vertices; i++){
                if(paths[i] -> node == destination){
                    dest_index = i;
                    break;
                }
            }

            string final_destination = paths[dest_index] -> node; //final destination
            int total_distance = paths[dest_index] -> distance; //total distance from source to destination
            string via = paths[dest_index] -> previous; //final destination node's previous (via) data field

            stack<Node*> s; //create a stack (LIFO data structure)
            s.push(paths[dest_index]); //push destination index onto stack

            while(via != source){ //while via does NOT equal source
                for(int i = 0; i < vertices; i++){
                    if(paths[i] -> node == s.top() -> previous){
                        dest_index = i;
                        break;
                    }
                } //update destination index
                via = paths[dest_index] -> previous; //update via
                s.push(paths[dest_index]); //push new destination index onto stack
            }

            cout << "\n" << source << " to " << final_destination << "..." << endl;
            while(!s.empty()){
                shortest_path += s.top() -> previous + " (" + to_string(distance_between(s.top() -> previous, s.top() -> node)) + ") -> ";
                s.pop(); //remove nodes from the stack one at a time (working backwards from destination back to source)
            }
            shortest_path += final_destination; //tack on final destination
            cout << left << setw(60) << shortest_path << "Total: " << total_distance << endl; //use setw() to line up the totals
            shortest_path = ""; //reset shortest path string
        }
    }
}

void Dijkstra::shortest_path_one(string input_filename, string source, string destination){
    shortest_path(input_filename, source);

    string shortest_path; //shortest path string

    if(source == destination){ //special case for if source equals destination...
        cout << "Shortest path from " << source << " to " << destination << ":" << endl;
        shortest_path += source + " (0) -> " + destination; //...since we're already there!
        cout << left << setw(60) << shortest_path << "Total: 0" << endl; //use setw() to line up the totals
        exit(0);
    }

    bool found = false;
    for(int i = 0; i < vertices; i++){
        if(nodes[i] -> node == destination){
            found = true;
        }
    }
    if(found == false){
        cout << "Error: no route exists, destination unreachable" << endl; //destination does not exist in the graph
        exit(0);
    }

    int dest_index = 0;
    for(int i = 0; i < vertices; i++){
        if(paths[i] -> node == destination){
            dest_index = i;
            break;
        }
    }

    if(paths[dest_index] -> previous == "n/a"){
        cout << "Error: no route exists, destination unreachable" << endl; //from source to destination, that is
        exit(0);
    }

    string final_destination = paths[dest_index] -> node; //final destination
    int total_distance = paths[dest_index] -> distance; //total distance from source to destination
    string via = paths[dest_index] -> previous; //final destination node's previous (via) data field

    stack<Node*> s; //create a stack (LIFO data structure)
    s.push(paths[dest_index]); //push destination index onto stack

    while(via != source){ //while via does NOT equal source
        for(int i = 0; i < vertices; i++){
            if(paths[i] -> node == s.top() -> previous){
                dest_index = i;
                break;
            }
        } //update destination index
        via = paths[dest_index] -> previous; //update via
        s.push(paths[dest_index]); //push new destination index onto stack
    }

    cout << "Shortest path from " << source << " to " << destination << ":" << endl;
    while(!s.empty()){
        shortest_path += s.top() -> previous + " (" + to_string(distance_between(s.top() -> previous, s.top() -> node)) + ") -> ";
        s.pop(); //remove nodes from the stack one at a time (working backwards from destination back to source)
    }
    shortest_path += final_destination; //tack on final destination
    cout << left << setw(60) << shortest_path << "Total: " << total_distance << endl; //use setw() to line up the totals
}