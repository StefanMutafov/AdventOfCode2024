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

std::vector<int> files;
//std::vector<int> free_space;
std::queue<int> free_space;
int chtoi(char c){
    return c - '0';
}
void read_input(const std::string &file_name) {

    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Can't open file: " << file_name << std::endl;
        return;
    }

    std::string line;;
    char c;
    int id{0};
    for(int i = 0; file.get(c); ++i ){
        if(i % 2 == 0){
            for(int j = chtoi(c); j > 0; --j){
                files.push_back(id);
            }
            ++id;
        }else{
            for(int j = chtoi(c); j > 0; --j){
                free_space.push(files.size()+ free_space.size());
            }
        }
    }
    file.close();
}


int main() {
    read_input("../input.txt");
    while(!free_space.empty()){
        auto pos = files.begin()+free_space.front();
        if(pos > files.end()){break;}
        files.insert(pos,files.back());
        files.pop_back();
        free_space.pop();
    }
    long long sum{0};
    for(int i = files.size()-1; i>=0; --i ){
        sum+= files.back()*i;
        files.pop_back();
    }
    std::cout<<sum;
    return 0;
}
