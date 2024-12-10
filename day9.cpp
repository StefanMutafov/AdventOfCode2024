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

std::vector<std::pair<int, int>> files;
//std::vector<int> free_space;
std::vector<std::pair<int, int>> free_space;
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
            std::pair<int, int> file = {id, chtoi(c)};
            files.push_back(file);
            ++id;
        }else{
            std::pair<int, int> free = {files.size(), chtoi(c)};
            free_space.push_back(free);
//            for(int j = chtoi(c); j > 0; --j){
//                free_space.push(files.size()+ free_space.size());
//            }
        }
    }
    file.close();
}


int main() {
    read_input("../input.txt");
    auto inserted{0};
   for(int i = files.size() - 1; i >= 0; --i){
       for(auto j = 0; free_space[j].second < files[i].second; j++){
           if(free_space[j].second >= files[i].second){
               files.insert(files.begin()+free_space[j].first+inserted, files[i]);
                ++inserted;
                files.erase(files.end()-i);
           }
       }
   }
    return 0;
}
