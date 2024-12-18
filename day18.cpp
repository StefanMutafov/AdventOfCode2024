#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Node;
class Edge;

std::vector<Node*> nodes;
std::vector<Edge*> edges;
bool contains(std::vector<Node*>& nodes, Node* node);
Node* extract_smallest(std::vector<Node*>& nodes);
std::vector<Node*>* neighbour_remaining_nodes(Node* node);
int distance(Node* node1, Node* node2);


class Node{
public:
    Node(int x, int y)
            : id(std::pair<int, int>(x, y)), previous(NULL), distance_from_start(INT_MAX) {
        nodes.push_back(this);
    }
    std::pair<int, int> id;
    int distance_from_start;
    Node *previous;


};

Node* get_node(int x, int y){
    Node * out;
    for(auto node : nodes){
        if(node->id.first == x && node->id.second == y){
            out = node;
        }
    }
    return out;
}



class Edge {
public:
    Edge(Node* node1, Node* node2, int distance)
            : node1(node1), node2(node2), distance(distance) {
        edges.push_back(this);
    }
    bool connects(Node* node1, Node* node2) {
        return (
                (node1 == this->node1 &&
                 node2 == this->node2) ||
                (node1 == this->node2 &&
                 node2 == this->node1));
    }

public:
    Node* node1;
    Node* node2;
    int distance;
};


Node* extract_smallest(std::vector<Node*>& nodes) {
    int size = nodes.size();
    if (size == 0) return NULL;
    int smallestPosition = 0;
    Node* smallest = nodes.at(0);
    for (int i = 1; i < size; ++i) {
        Node* current = nodes.at(i);
        if (current->distance_from_start <
            smallest->distance_from_start) {
            smallest = current;
            smallestPosition = i;
        }
    }
    nodes.erase(nodes.begin() + smallestPosition);
    return smallest;
}


std::vector<Node*>* neighbour_remaining_nodes(Node* node) {
    std::vector<Node*>* adjacentNodes = new std::vector<Node*>();
    const int size = edges.size();
    for (int i = 0; i < size; ++i) {
        Edge* edge = edges.at(i);
        Node* adjacent = NULL;
        if (edge->node1 == node) {
            adjacent = edge->node2;
        } else if (edge->node2 == node) {
            adjacent = edge->node1;
        }
        if (adjacent && contains(nodes, adjacent)) {
            adjacentNodes->push_back(adjacent);
        }
    }
    return adjacentNodes;
}


int distance(Node* node1, Node* node2) {
    const int size = edges.size();
    for (int i = 0; i < size; ++i) {
        Edge* edge = edges.at(i);
        if (edge->connects(node1, node2)) {
            return edge->distance;
        }
    }
    return -1;  // should never happen
}


bool contains(std::vector<Node*>& nodes, Node* node) {
    const int size = nodes.size();
    for (int i = 0; i < size; ++i) {
        if (node == nodes.at(i)) {
            return true;
        }
    }
    return false;
}



void read_file(const std::string &file_name, std::vector<std::vector<char>> &map, int line_num) {
    std::ifstream file("../input.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file.\n";
        return;
    }
    std::string line;
    for(int i = 0; std::getline(file, line) && i< line_num;++i ){
        std::istringstream stream(line);
        char ignore; // For skipping characters like '=', ',', etc.
        int x, y;
        stream >> x >> ignore >> y;
        map[y][x] = '#';
    }
file.close();
}

void create_graph(std::vector<std::vector<char>> map){

    for (Node* node : nodes) {
        delete node;
    }
    nodes.clear();

    for (Edge* edge : edges) {
        delete edge;
    }
    edges.clear(); // Clear the vector
    for(int row = 0; row < map.size(); ++row){
        for(int col = 0; col < map[0].size(); ++col){
            if(map[row][col] == '.'){
                Node *node = new Node(col, row);
                if(row == 0 && col == 0) node->distance_from_start=0;
                Node *n1 = get_node(col-1, row);
                Node *n2 = get_node(col, row-1);
                Node *n3 = get_node(col+1, row);
                Node *n4 = get_node(col, row+1);
                if(n1 != nullptr){
                    Edge* e = new Edge(node, n1,1);
                }
                if(n2 != nullptr){
                    Edge* e = new Edge(node, n2,1);
                }
                if(n3 != nullptr){
                    Edge* e = new Edge(node, n3,1);
                }
                if(n4 != nullptr){
                    Edge* e = new Edge(node, n4,1);
                }


            }

        }
    }

}

void simple_dijkstra(){
while(nodes.size() > 0){
    Node* smallest = extract_smallest(nodes);
    std::vector<Node*>* neighbours = neighbour_remaining_nodes(smallest);

    const int size = neighbours->size();
    for(int i = 0; i < size; ++i){
        Node* neighbour  = neighbours->at(i);
        int _distance = distance(smallest, neighbour) + smallest->distance_from_start;
        if(_distance < neighbour->distance_from_start){
            neighbour->distance_from_start = _distance;
            neighbour->previous = smallest;
        }
    }
delete neighbours;
}
}


bool find_exit(std::vector<std::vector<char>>& map, int x, int y, std::vector<std::vector<bool>>& visited) {
    // Base conditions: out of bounds or a wall
    if (x < 0 || y < 0 || x >= map[0].size() || y >= map.size() || map[y][x] == '#' || visited[y][x]) {
        return false;
    }

    // Mark this cell as visited
    visited[y][x] = true;

    // Check if we've reached the exit point
    if (x == map[0].size() - 1 && y == map.size() - 1) {
        return true;
    }

    // Recur in all 4 possible directions
    if (find_exit(map, x + 1, y, visited) ||  // Right
        find_exit(map, x - 1, y, visited) ||  // Left
        find_exit(map, x, y + 1, visited) ||  // Down
        find_exit(map, x, y - 1, visited)) {  // Up
        return true;
    }

    // If no path leads to the exit, return false
    return false;
}





int main() {
    const int rows = 71;
    const int cols = 71;
//    std::vector<std::vector<char>> map(rows, std::vector<char>(cols, '.'));
//    read_file("../input.txt",map, 1024);
//    create_graph(map);
//    Node* exit = get_node(map[0].size()-1, map.size()-1);
//    simple_dijkstra();
//    std::cout<<exit->distance_from_start<<std::endl;

//    for(int i=1024; i<3450;++i){
//        std::cout<<i<<std::endl;
//        std::vector<std::vector<char>> map(rows, std::vector<char>(cols, '.'));
//        read_file("../input.txt",map, i);
//        create_graph(map);
//        Node* exit = get_node(map[0].size()-1, map.size()-1);
//        simple_dijkstra();
//        if(exit->distance_from_start==INT_MAX){
//            std::cout<<"IT ISSSSSSSSSSS:"<<i<<std::endl;
//            break;
//        }
//    }

    for(int i = 1024; i<3450; ++i){
        std::cout<<i<<std::endl;
        std::vector<std::vector<char>> map(rows, std::vector<char>(cols, '.'));
        read_file("../input.txt",map, i);
        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
        if(!find_exit(map, 0,0, visited)){
            std::cout<<"IT ISSSSSSSSSSS:"<<i<<std::endl;
            break;
        }
    }

    return 0;
}
