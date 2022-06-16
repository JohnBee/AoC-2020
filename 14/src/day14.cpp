#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <exception>
#include <regex>
#include <math.h>

using namespace std;

typedef map<unsigned long long, unsigned long long> t_mem;

vector<string> splitSpaces(string s){
    regex s_regex(" ");
    vector<string> foundStr(sregex_token_iterator(s.begin(), s.end(), s_regex, -1), sregex_token_iterator());
    return foundStr;
}

vector<vector<string>> loadInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        cout << "File not found: " << filename << endl;
        runtime_error("Cannot open file.");
        exit(1);
    }
    string line;
    vector<vector<string>> out;
    while(!file.eof()){
        getline(file, line);
        if(line.length() > 0){
            out.push_back(splitSpaces(line));
        }
    }
    return out;
}
unsigned long long getNumberInString(string s){
    regex s_regex("[0-9]+");
    vector<string> location(sregex_token_iterator(s.begin(), s.end(), s_regex, 0), sregex_token_iterator());
    return stol(location[0]);
}
vector<char> numToBitChars(unsigned long long num){
    vector<char> out;
    while(num > 0){
        if(num % 2 == 0){
            out.push_back('0');
        }
        else{
            out.push_back('1');
        }
        num = num / 2;
    }
    // apply padding
    while(out.size() < 36){
        out.push_back('0');
    }
    return out;
}
vector<char> applyMask(string mask, vector<char> numBits){
    // reverse mask
    for(int i = 0; i < mask.length() /2; i++){
        swap(mask[i], mask[mask.length() - i - 1]);
    }
    for(int i = 0; i < numBits.size(); i++){
        if(mask.at(i) !='X'){
            numBits[i] = mask.at(i);
        }
    }
    return numBits;
}
unsigned long long bitsCharToNum(vector<char> numBits){
    unsigned long long out = 0;
    for(int i = 0; i < numBits.size(); i++){
        if(numBits[i]=='1'){
            out += pow(2,i);
        }
    }
    return out;
}
t_mem runMemory(vector<vector<string>> input){
    // get the mask
    string mask = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    t_mem memory;
    for(auto s : input){
        if(s.at(0)=="mask"){
            mask = s.at(2);
        }
        else{
            //memory location;
            unsigned long long loc = getNumberInString(s.at(0));
            unsigned long long value = getNumberInString(s.at(2));

            // apply the mask;
            vector<char> vBits = numToBitChars(value);
            vBits = applyMask(mask, vBits);
            memory[loc] = bitsCharToNum(vBits);
        }
    }

    return memory;
}
vector<vector<char>> genBitCombinations(int length){
    vector<vector<char>> out;
    int totalCombs = pow(2, length);
    for(int i = 0; i < totalCombs; i++){
        vector<char> comb;
        int num = i;
        while(num > 0){
            if(num % 2 == 0){
                comb.push_back('0');
            }
            else{
                comb.push_back('1');
            }
            num = num / 2;
        }
        while(comb.size() < length){
            comb.push_back('0');
        }
        out.push_back(comb);
    }
    return out;
} 
vector<vector<char>> applyMaskPart2(string mask, vector<char> lBits){
    // reverse mask
    for(int i = 0; i < mask.length() /2; i++){
        swap(mask[i], mask[mask.length() - i - 1]);
    }
    // find all locations that have X in
    vector<int> floatLoc;
    for(int i = 0; i < mask.length(); i++){
        if(mask[i] == 'X'){
            floatLoc.push_back(i);
        }
    }
    // generate all possible masks
    vector<string> bitmasks;
    vector<vector<char>> bitLocCombs = genBitCombinations(floatLoc.size());
    for(auto blc : bitLocCombs){
        string bMask = mask;
        for(int i = 0; i < blc.size(); i++){
            bMask[floatLoc[i]] = blc[i];
        }
        bitmasks.push_back(bMask);
    }
    // for all masks apply to input
    vector<vector<char>> appliedMasks;
    for(string bitmask : bitmasks){
        vector<char> amask;
        for(int i = 0; i < bitmask.length(); i++){   
            if(bitmask[i]=='1'){
                amask.push_back('1');
            }
            else if(mask[i]=='X'){
                amask.push_back(bitmask[i]);
            }
            else{
                // 0 in mask
                amask.push_back(lBits[i]);
            }
        }
        appliedMasks.push_back(amask);
    }
    return appliedMasks;
}
t_mem runMemoryPart2(vector<vector<string>> input){
    string mask = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    t_mem memory;
    for(auto s : input){
        if(s.at(0)=="mask"){
            mask = s.at(2);
        }
        else{
            //memory location;
            unsigned long long loc = getNumberInString(s.at(0));
            unsigned long long value = getNumberInString(s.at(2));

            // apply the mask;
            vector<char> lBits = numToBitChars(loc);
            vector<vector<char>> lBitsFloating = applyMaskPart2(mask, lBits);
            for(auto lb : lBitsFloating){
                memory[bitsCharToNum(lb)] = value;
            }
            
        }
    }
    return memory;
}
unsigned long long sumMemory(t_mem memory){
    unsigned long long sum = 0;
    for(t_mem::iterator i = memory.begin(); i != memory.end(); i++){
        sum += i->second;
    }
    return sum;
}
int main(int argc, char * argv[]){
    cout << "Day 14" << endl;
    if(argc < 2){
        cout << "Not enough args, missing input filename." << endl;
        exit(1);
    }
    vector<vector<string>> input = loadInput(argv[1]);
    vector<char> x = numToBitChars(1023);
    t_mem memory = runMemory(input);
    cout << "Part 1: " << sumMemory(memory) << endl;
    t_mem memory2 = runMemoryPart2(input);
    cout << "Part 2: " << sumMemory(memory2) << endl;
    return 0;
}