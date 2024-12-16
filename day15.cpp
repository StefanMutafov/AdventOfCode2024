#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

typedef struct {
    int x;
    int y;
}Robot;

bool move_avaliable(int x, int y, int v_x, int v_y, std::vector<std::vector<char>> &field){
    if(field[y][x] == '.') return true;
    if(field[y][x] == '#') return false;
    return move_avaliable(x+v_x, y+v_y, v_x, v_y, field);
}
void move(Robot &robot, int v_x, int v_y, std::vector<std::vector<char>> &field){
    if(move_avaliable(robot.x, robot.y, v_x, v_y, field)){
        int current_x = robot.x + v_x;
        int current_y = robot.y + v_y;
        field[robot.y][robot.x] = '.';
        char buffer = '@';
        while(buffer != '.'){
           char temp = field[current_y][current_x];
           field[current_y][current_x] = buffer;
           buffer = temp;
            current_x += v_x;
            current_y += v_y;
        }
        robot.x += v_x;
        robot.y += v_y;

    }
}

void read_file(const std::string &file_name, std::vector<std::vector<char>> &field) {
    std::ifstream file("../input.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file.\n";
        return;
    }
    std::string line;
    Robot robot;
    bool map_finished{false};
    while (std::getline(file, line)) {
        if(line.empty() && !map_finished){
            std::getline(file, line);
            map_finished = true;
        }
        if(!map_finished) {
            std::vector<char> buffer;
            for (int i = 0; i < line.size(); ++i) {
                if (line[i] == '@') {
                    robot.x = i;
                    robot.y = field.size();
                }
                buffer.push_back(line[i]);
            }
            field.push_back(buffer);
        }else{
            for(auto c_move : line){
                switch (c_move) {
                    case '^':
                        move(robot, 0,-1,field);
                        break;
                    case 'v':
                        move(robot, 0,1,field);
                        break;
                    case '<':
                        move(robot, -1,0,field);
                        break;
                    case '>':
                        move(robot, 1,0,field);
                        break;
                    default:
                        break;
                }
            }

        }
}
file.close();
}


int main() {
    std::vector<std::vector<char>> field;
    read_file("../input.txt",field);
    long long sum{0};
    for (std::size_t i = 0; i < field.size(); ++i) {
        for (std::size_t j = 0; j < field[i].size(); ++j) {
            if(field[i][j] == 'O'){
                sum += j + i*100;
            }
        }
    }
    std::cout<<sum<<std::endl;
    return 0;
}
