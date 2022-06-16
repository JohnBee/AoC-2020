#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>
#include <limits>

using namespace std;

typedef unsigned long long t_big;
template <typename t, int maxSize>
struct fixedQueue{
    deque<t> d;
    typename deque<t>::iterator invalidIterator;
    void push(t x){
        d.push_back(x);
        if(d.size() > maxSize){
            d.pop_front();
        }
    }
    bool containsSum(t s){
        for(typename deque<t>::iterator i = d.begin(); i != d.end()-1; i++){
            for(typename deque<t>::iterator j = i+1; j != d.end(); j++){
                if(*i + *j == s){
                    return true;
                }
            }
        }
        return false;
    }
    int size(){
        return d.size;
    }
};
t_big smallestAndBiggestSum(fixedQueue<t_big, 25> buffer){
        t_big smallest = std::numeric_limits<t_big>::max();
        t_big biggest = 0;
        for(typename deque<t_big>::iterator i = buffer.d.begin(); i != buffer.d.end(); i++){
            if(*i < smallest){
                smallest = *i;
            }
            if(*i > biggest){
                biggest = *i;
            }
        }
        return smallest+biggest;
    }
vector<t_big> loadInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        cout << "Can't open file: " << filename << endl;
        exit(0);
    }
    vector<t_big> out;
    string line;
    while(!file.eof()){
        getline(file, line);
        out.push_back(stoull(line));
    }
	file.close();
    return out;
}
pair<t_big, int> findInvalidNumber(vector<t_big> input, int preambleLength){
    fixedQueue<t_big, 25> buffer;
    // add the first 25 items
    int i;
    for(i = 0; i < preambleLength; i++){
        buffer.push(input.at(i));
    }
    // add and check the rest
    for(;i < input.size(); i++){
        t_big val = input[i];
        if(buffer.containsSum(val)){
            buffer.push(val);
        }
        else{
            return make_pair(val, i);
        }
    }
    return make_pair(0, 0);
}
pair<int, int> findBoundsForInvalid(vector<t_big> input, int index, t_big invalidNumber){
    int lowIndex;
    int highIndex;
    for(int i = 0; i < index; i++){
        for(int j = i+1; j < index; j++){
            // sum from i to j to see if it meets invalid
            t_big sum = 0;
            for(int k = i; k < j; k++){
                sum += input[k]; 
            }
            if(sum==invalidNumber){
                lowIndex = i;
                highIndex = j;
                return make_pair(lowIndex, highIndex);
            }
        }
    }
    return make_pair(0, 0);
}
t_big sumSmallestAndBiggest(vector<t_big> input, int lowIndex, int highIndex){
    t_big smallest = numeric_limits<t_big>::max();
    t_big biggest = 0;
    for(int i = lowIndex; i < highIndex; i++){
        t_big val = input.at(i);
        if(val < smallest){
            smallest = val;
        }
        if(val > biggest){
            biggest = val;
        }
    }
    return smallest+biggest;
}
t_big findInvalidBiggestSmallestSum(vector<t_big> input, int index, t_big invalidNumber){
    pair<int, int> indexes = findBoundsForInvalid(input, index, invalidNumber);
    if(indexes.first != 0 && indexes.second != 0){
        return sumSmallestAndBiggest(input, indexes.first, indexes.second);
    }  
    else {
        return 0;
    }
}
int main(int argc, char * argv[]){
    std::cout << "hell0 world2" << std::endl;
    if(argc < 2){
        cout << "no filename in commandline args." << endl;
        return 0;
    }
    vector<t_big> input = loadInput(argv[1]);
    pair<t_big, int> invalidNumber = findInvalidNumber(input, 25);
    cout << "Part 1: " << invalidNumber.first << endl;
    int index = invalidNumber.second;

    cout << "Part 2: " << findInvalidBiggestSmallestSum(input, index, invalidNumber.first) << endl;
    return 0;
}