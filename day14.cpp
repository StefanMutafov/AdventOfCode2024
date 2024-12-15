//PART1//
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <string>
//
//
//int main() {
//    std::ifstream file("../input.txt");
//    if (!file.is_open()) {
//        std::cerr << "Error: Unable to open file.\n";
//        return 1;
//    }
//    int rows = 103;
//    int cols = 101;
//    int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
//
//    std::string line;
//    while (std::getline(file, line)) {
//        std::istringstream stream(line);
//        char ignore; // For skipping characters like '=', ',', etc.
//        int px, py, vx, vy;
//
//        // Extract the numbers into the variables
//        stream >> ignore >> ignore >> px >> ignore >> py; // p=0,4
//        stream >> ignore >> ignore >> vx >> ignore >> vy; // v=3,-3
//            px = (px + (100 * vx)) % cols;
//            if (px < 0) {
//                px += cols;
//            }
//        py = (py+(100*vy))%rows;
//        if (py < 0) {
//            py += rows;
//        }
//        if (px != cols / 2 && py != rows / 2) { // Exclude players on quadrant borders
//            if (px > cols / 2) { // Right quadrants
//                if (py > rows / 2) { // Top quadrants
//                    q4++;
//                } else { // Bottom quadrants
//                    q2++;
//                }
//            } else if (px < cols / 2) { // Left quadrants
//                if (py > rows / 2) { // Top quadrants
//                    q3++;
//                } else { // Bottom quadrants
//                    q1++;
//                }
//            }
//        }
//
//
//
//        // Placeholder for your calculations
//       // std::cout << "Processing pairs (" << px << "," << py << ") and (" << vx << "," << vy << ")\n";
//        // Your calculations go here
//    }
//    int fact = q1*q2*q3*q4;
//    std::cout<<fact<<std::endl;
//
//    file.close();
//    return 0;
//}




///PART2

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

    int rows = 103;
    int cols = 101;
struct Robot {
    int px, py; // Position coordinates
    int vx, vy; // Speed (velocity) coordinates
};

std::vector<Robot> robots_init;



void read_input(const std::string &file_name) {
    std::ifstream file("../input.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file.\n";
        return ;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        char ignore; // For skipping characters like '=', ',', etc.
        int px, py, vx, vy;

        // Extract the numbers into the variables
        stream >> ignore >> ignore >> px >> ignore >> py; // p=0,4
        stream >> ignore >> ignore >> vx >> ignore >> vy; // v=3,-3

        // Create a Player struct and save to the vector
        Robot robot{};
        robot.px = px;
        robot.py = py;
        robot.vx = vx;
        robot.vy = vy;

        // Store the player data
        robots_init.push_back(robot);
    }


    file.close();
}

void generateGrid(const std::vector<Robot>& robots, std::vector<std::vector<char>>& grid) {
    // Initialize the grid with '.' (empty spaces)
    grid.resize(rows, std::vector<char>(cols, '.'));

    // Iterate over the players and place them on the grid
    for (const Robot& robot : robots) {
        int px = robot.px;
        int py = robot.py;

        // Check if the position is within the grid bounds
        if (px >= 0 && px < cols && py >= 0 && py < rows) {
            grid[py][px] = '@'; // Place player on the grid
        }
    }
}


void floodFill(const std::vector<std::vector<char>>& grid, std::vector<std::vector<bool>>& visited,
               int row, int col, char letter, int& count, bool& foundLargeCluster) {

    if (row < 0 || row >= rows || col < 0 || col >= cols || visited[row][col] || grid[row][col] != letter || foundLargeCluster) {
        return;
    }

    visited[row][col] = true;
    count++;

    // Check if current field is part of a large cluster
    if (count > 20) {
        foundLargeCluster = true;
    }

    // Recursively flood fill the four neighboring directions
    floodFill(grid, visited, row - 1, col, letter, count, foundLargeCluster); // Up
    floodFill(grid, visited, row + 1, col, letter, count, foundLargeCluster); // Down
    floodFill(grid, visited, row, col - 1, letter, count, foundLargeCluster); // Left
    floodFill(grid, visited, row, col + 1, letter, count, foundLargeCluster); // Right
}

void appendGridToFile(int seconds, const std::vector<std::vector<char>>& grid, const std::string& filename) {
    std::ofstream file(filename, std::ios::app); // Open the file in append mode

    if (!file.is_open()) {
        std::cerr << "Failed to open file for appending." << std::endl;
        return;
    }

    // Write the number of seconds to the file
    file << "Seconds: " << seconds << std::endl;

    // Write the grid to the file
    for (const auto& row : grid) {
        for (char cell : row) {
            file << cell;
        }
        file << std::endl; // Move to the next line after each row
    }

    // Close the file after appending
    file.close();
}

std::vector<Robot> updateRobotPositions(const std::vector<Robot>& robots, int seconds) {
    std::vector<Robot> updatedRobots = robots;  // Create a copy of the original players

    // Update positions for each player based on their velocity and time
    for (Robot& robot : updatedRobots) {
        robot.px = ( robot.px + (seconds *  robot.vx)) % cols;
        if (robot.px < 0) {
            robot.px += cols;
            }

        robot.py = (robot.py+(seconds*robot.vy))%rows;
        if ( robot.py < 0) {
            robot.py += rows;
        }


    }

    return updatedRobots;  // Return the updated player positions
}


int main() {
    read_input("../input.txt");
    for (int second = 8150; true; ++second) {
        std::vector<std::vector<char>> grid;
        std::cout<<"Trying second "<< second<<std::endl;
        auto new_pos = updateRobotPositions(robots_init,second);
        generateGrid(new_pos,grid);
        int count = 0;
        bool cluster = false;
        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
        //appendGridToFile(second,grid,"field1.txt");
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (grid[i][j] == '@' && !visited[i][j]) {
                    count = 0;
                    floodFill(grid, visited, i, j, '@', count, cluster);
                    if (cluster) {
                        std::cout << "Large cluster found at second " << second << " with more than 20 @ symbols!" << std::endl;
                        break;  // Exit the loop as we found a large cluster
                    }
                }
            }
            if (cluster) break;  // Exit the outer loop if a large cluster was found
        }
        if(cluster) {
            appendGridToFile(second,grid,"field1.txt");
        }

    }



    return 0;
}


