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

std::unordered_map<char, std::vector<std::pair<int, int>>> anth_coord;
std::set<std::pair<int, int>> antinodes_p1;
std::set<std::pair<int, int>> antinodes_p2;
int size_x, size_y;

void read_input(const std::string &file_name) {

    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Can't open file: " << file_name << std::endl;
        return;
    }

    std::string line;
    int y{0};
    while (std::getline(file, line)) {
        int x{0};
        std::istringstream stream(line);
        char c;
        while (stream >> c) {
            if (c != '.') {
                anth_coord[c].emplace_back(x, y);
            }
            ++x;
        }
        ++y;

    }
    size_x = line.size() - 1;
    size_y = y - 1;


    file.close();
}

std::pair<int, int> calculate_offset(int x1, int y1, int x2, int y2) {
    std::pair<int, int> offset = {x2 - x1, y2 - y1};
    return offset;
}

bool is_within_bounds(int x, int y){
    return x>=0 && y >= 0 && x<=size_x && y<=size_y;
}

int main() {
    read_input("../input.txt");
    for (const auto &[key, coordinates]: anth_coord) {
        for (size_t i = 0; i < coordinates.size(); ++i) {
            for (size_t j = i + 1; j < coordinates.size(); ++j) {
                auto offset = calculate_offset(
                        coordinates[i].first, coordinates[i].second,
                        coordinates[j].first, coordinates[j].second);
                bool bounds1{true};
                bool bounds2{true};
                for (auto a{1}; bounds1 || bounds2; ++a) {
                    std::pair<int, int> antinode1 = {coordinates[i].first + offset.first * a,
                                                     coordinates[i].second + offset.second * a};
                    std::pair<int, int> antinode2 = {coordinates[j].first - offset.first * a,
                                                     coordinates[j].second - offset.second * a};

                    if (bounds1 = is_within_bounds(antinode1.first, antinode1.second)) {
                        antinodes_p2.insert(antinode1);
                        if(a==2){antinodes_p1.insert(antinode1);}
                    }
                    if (bounds2 = is_within_bounds(antinode2.first, antinode2.second)) {
                        antinodes_p2.insert(antinode2);
                        if(a==2){antinodes_p1.insert(antinode2);}
                    }
                }
            }
        }
    }
    std::cout<<"Part1:" << antinodes_p1.size() << std::endl;
    std::cout<<"Part1:" << antinodes_p2.size() << std::endl;
    return 0;
}
