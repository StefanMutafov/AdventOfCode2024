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

bool sum_possible(const std::vector<long long> &numbers, long long curr_result, long long targer, int operators, int nums_calculated){
    if(nums_calculated == numbers.size()){
        return curr_result == targer;
    }

    if(curr_result > targer){
        return false;
    }

    long long c_num = numbers[nums_calculated];
    long long result1 = curr_result * c_num;
    long long result2 = curr_result + c_num;
    long long result3 = std::stoll(std::to_string(curr_result) + std::to_string(c_num));
    if(operators >= 1 && sum_possible(numbers, result1, targer, operators, nums_calculated + 1 )) return true;
    if(operators >= 2 && sum_possible(numbers, result2, targer, operators, nums_calculated + 1 )) return true;
    if(operators >= 3 && sum_possible(numbers, result3, targer, operators, nums_calculated + 1 )) return true;

    return false;
}

std::vector<long long > parseNumbers(const std::string &input) {
    std::vector<long long > numbers;
    std::istringstream stream(input);
    long long number;

    while (stream >> number) {
        numbers.push_back(number);
    }

    return numbers;
}

int main(){
    auto start = std::chrono::high_resolution_clock::now();
    long long  calibration1 = 0, calibration2 = 0;
    std::ifstream file("../input.txt");
    std::string line;

    while(std::getline(file, line)){
        auto del = line.find(':');
        long long  target = std::stoll(line.substr(0, del));
        std::vector<long long > numbers = parseNumbers(line.substr(del+1));

        if (sum_possible(numbers, numbers[0], target, 2, 1)) {
            calibration1 += target;
            calibration2 += target;
        }
            // Check if we can reach the target using N=3 operations
        else if (sum_possible(numbers, numbers[0], target, 3, 1)) {
            calibration2 += target;
        }
    }

    std::cout<<"fin calib1:"<<calibration1<<std::endl;
    std::cout<<"fin calib2:"<<calibration2<<std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " sec" << std::endl;

    return 0;
}
