#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <map>

#define DEBUG true


using namespace std;


vector<vector<string>> readInputFile(){
    ifstream file;
    file.open("input");
    vector<vector<string>> answers;
    vector<string> block;
    string line;
    while(!file.eof()){
        getline(file, line);
        if(line.length()!=0){
            block.push_back(line);
        }
        else{
            answers.push_back(block);
            block.clear();
        }
    }
    return answers;
}

bool containsChar(vector<char> chars, char c){
    return (find(chars.begin(), chars.end(), c)!=chars.end());
}

int countAnsweredQuestionUnqie(vector<vector<string>> answers){
    vector<char> uniqueChars;
    int cumulativeLength = 0;
    for(auto b : answers){
        for(auto l : b){
            for(char c : l){
                if(!containsChar(uniqueChars, c)){
                    uniqueChars.push_back(c);
                }
            }
        }
        cumulativeLength += uniqueChars.size();
        uniqueChars.clear();
    }
    return cumulativeLength;
}
int countEveryoneAnswered(vector<vector<string>> answers){
    map<char, int> cmap;
    string alph = "abcdefghijklmnopqrstuvwxyz";
    int outTotal = 0;
    for(auto b : answers){
        for(auto c : alph){
            cmap[c] = 0;
        }

        for(auto l : b){
            for(char c : l){
                cmap[c] += 1;
            }
        }
        int allAnswered = 0;
        for(auto c : alph){
            if(cmap[c] == b.size()){
                //if(DEBUG){cout << c << " " << cmap[c] << " " << b.size() << endl;}
                allAnswered += 1;
            }
            
        }
        //if(DEBUG){cout << allAnswered << endl};
        outTotal += allAnswered;
    }
    return outTotal;
}

int main(){
    vector<vector<string>> answers = readInputFile();
    // if(DEBUG){
    //     for(auto b : answers){
    //         for(auto l : b){
    //             cout << l << endl;   
    //         }
    //         cout << endl << endl;
    //     }
    // }
    cout << countAnsweredQuestionUnqie(answers) << endl;
    cout << "done" << endl;
    cout << "Part 2: " << countEveryoneAnswered(answers) << endl;
    return 0;
}