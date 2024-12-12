#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <set>
#include <ratio>
#include <chrono>
#include <queue>


std::vector<std::vector<char>> read_input(const std::string &file_name) {
    std::vector<std::vector<char>> layout;
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Can't open file: " << file_name << std::endl;
        throw std::runtime_error("Could not open file " + file_name);
    }

    std::string line;
    while (std::getline(file, line)) {
        layout.emplace_back(line.begin(), line.end());

    }
    file.close();
    return layout;
}

void floodFill(const std::vector<std::vector<char>>& grid, std::vector<std::vector<bool>>& visited,
               int row, int col, char letter, int& count, int& differentNeighbours) {
    int rows = grid.size();
    int cols = grid[0].size();

    if (row < 0 or row >= rows or col < 0 or col >= cols or visited[row][col] or grid[row][col] != letter) {
        return;
    }

    visited[row][col] = true;
    count++;

    if (row == 0 or grid[row - 1][col] != letter) differentNeighbours++;       // Up
    if (row == rows - 1 or grid[row + 1][col] != letter) differentNeighbours++; // Down
    if (col ==  0 or grid[row][col - 1] != letter) differentNeighbours++;       // Left
    if (col == cols - 1 or grid[row][col + 1] != letter) differentNeighbours++; // Right


    floodFill(grid, visited, row - 1, col, letter, count, differentNeighbours); // Up
    floodFill(grid, visited, row + 1, col, letter, count, differentNeighbours); // Down
    floodFill(grid, visited, row, col - 1, letter, count, differentNeighbours); // Left
    floodFill(grid, visited, row, col + 1, letter, count, differentNeighbours); // Right
}


void floodFillCorners(const std::vector<std::vector<char>>& grid, std::vector<std::vector<bool>>& visited,
                      int row, int col, char letter, int& corners) {
    int rows = grid.size();
    int cols = grid[0].size();

    if (row < 0 or row >= rows or col < 0 or col >= cols or visited[row][col] or grid[row][col] != letter) {
        return;
    }

    visited[row][col] = true;
    
    if ((row > 0 && grid[row - 1][col] != letter) or row == 0) { // Outer edge
        if ((col > 0 && grid[row][col - 1] != letter) or col == 0) ++corners;
        if ((col < cols - 1 && grid[row][col + 1] != letter) or col == cols - 1) ++corners;
    }
    if ((row < rows - 1 && grid[row + 1][col] != letter) or row == rows - 1) { // Outer edge
        if ((col > 0 && grid[row][col - 1] != letter) or col == 0) ++corners;
        if ((col < cols - 1 && grid[row][col + 1] != letter) or col == cols - 1) ++corners;
    }
    if (row > 0 && grid[row - 1][col] == letter) { // Up or edge
        if (col > 0 && grid[row][col - 1] == letter && grid[row - 1][col - 1] != letter) ++corners; //Inner edge
        if (col < cols - 1 && grid[row][col + 1] == letter  && grid[row - 1][col + 1] != letter) ++corners;
    }
    if (row < rows - 1 && grid[row + 1][col] == letter) { // Down or edge
        if (col > 0 && grid[row][col - 1] == letter && grid[row + 1][col - 1] != letter) ++corners;//Inner edge
        if (col < cols - 1 && grid[row][col + 1] == letter && grid[row + 1][col + 1] != letter) ++corners;
    }
    
    floodFillCorners(grid, visited, row - 1, col, letter, corners); // Up
    floodFillCorners(grid, visited, row + 1, col, letter, corners); // Down
    floodFillCorners(grid, visited, row, col - 1, letter, corners); // Left
    floodFillCorners(grid, visited, row, col + 1, letter, corners); // Right
}

void caculate_fence(const std::vector<std::vector<char>>& grid){
    int rows = grid.size();
    int cols = grid[0].size();
    int total_cost = 0;
    int total_cost_disc = 0;
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<bool>> visited_corners(rows, std::vector<bool>(cols, false));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!visited[i][j]) {
                char letter = grid[i][j];
                int area = 0;
                int fences = 0;
                int corners = 0;
                floodFill(grid, visited, i, j, letter, area, fences);
                floodFillCorners(grid, visited_corners, i, j, letter, corners);
                total_cost += area*fences;
                total_cost_disc += area*corners;
            }
        }
    }
    std::cout << total_cost << std::endl;
    std::cout << total_cost_disc << std::endl;
}

int main() {
    std::vector<std::vector<char>> grid =  read_input("../input.txt");
    caculate_fence(grid);

    return 0;
}
