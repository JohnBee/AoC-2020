#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <deque>

using namespace std;


vector<int> loadInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("could not open file. ");
    }
    string line;
    vector<int> out = {0};
    while(!file.eof()){
        getline(file, line);
        if(line.length() > 0){
            out.push_back(stoi(line));

        }
    }
	file.close();
    sort(out.begin(), out.end());
    out.push_back(out.at(out.size()-1)+3);
    return out;
}
struct differences{
    int threeJolt = 0;
    int oneJolt = 0;
};
differences countJoltDifferences(vector<int> adapters){
    differences out;
    for(int i = 0; i < adapters.size()-1; i++){
        int diff = adapters.at(i+1)-adapters.at(i);
        if(diff == 1){
            out.oneJolt += 1;
        }
        else if(diff == 3){
            out.threeJolt += 1;
        }
        else{
            std::cout << "difference is not 3 or 1: " << adapters.at(i) << " " << adapters.at(i+1) << endl;
        }
    }
    return out;
}
struct run{
    int runStart = 0;
    int runEnd = 0;
    int size = 0;
    run(int start, int end){
        runStart = start;
        runEnd = end;
        size = end-start;
    }
};
bool runContainsOtherRun(vector<run> runs, run currentRun){
    for(run r : runs){
        if(currentRun.runStart >= r.runStart && currentRun.runEnd <= r.runEnd){
            return true;
        }
    }
    return false;
}
vector<run> findRuns(vector<int> adapters){
    // find runs of length 3 or 2 which increase in increments of 1.   
    vector<run> runs;
    // sliding window

    // find all 4 length runs
    // find all 3 length runs which aren't in the 4 length runs
    for(int i = 0; i < adapters.size()-4; i++){
        int start = i;
        int end = i+1;
        for(int k = i; k<i+3; k++){
            if(adapters.at(k+1)-adapters.at(k)==1){
                end = k+1;
            }
            else{
                break;
            }
        }
        if(end-start==3){
            run oneJoltRun(i, end);
            runs.push_back(oneJoltRun);
        }
    }
    // find all 3 length runs which aren't in the 4 length runs
    for(int i = 0; i < adapters.size()-3; i++){
        int start = i;
        int end = i+1;
        for(int k = i; k<i+2; k++){
            if(adapters.at(k+1)-adapters.at(k)==1 || adapters.at(k+1)-adapters.at(k)==2){
                end = k+1;
            }
            else{
                break;
            }
        }
        if(end-start==3 || end-start==2){
            run twoJoltRun(i, end);
            if(!runContainsOtherRun(runs, twoJoltRun)){
                runs.push_back(twoJoltRun);
            }
        }
    }
    return runs;
}
vector<vector<int>> findAllLongRun(vector<int> adapters, int start){
    vector<int> v_run;
    vector<vector<int>> out;
    for(int i = start; i < adapters.size()-1; i++){
        if(adapters.at(i+1)-adapters.at(i)==1 || adapters.at(i+1)-adapters.at(i)==2){
            v_run.push_back(adapters.at(i));
        }
        else {
            if(v_run.size() >= 2){
                v_run.push_back(adapters.at(i));
                out.push_back(v_run);
                v_run.clear();
            }
            else{
                v_run.clear();
            }
        }
    }
    return out;
}
void findAndAddGapSize(vector<int> v_run, int gapSize, map<int, vector<int>> & relMap){
    for(int i : v_run){
        if(find(v_run.begin(), v_run.end(), i+gapSize)!=v_run.end()){
            if(relMap.find(i)==relMap.end()){
                relMap[i] = {i+gapSize};
            }
            else{
                relMap[i].push_back(i+gapSize);
            }
        }
    }
    return;
}
int AllPathsBfs(int start, int end,  map<int, vector<int>> & relMap){
    int paths = 0;
    deque<int> toVisit = {start};
    while(!toVisit.empty()){
        int curr = toVisit.front();
        toVisit.pop_front();
        if(curr==end){
            paths+=1;
        }
        if(relMap.find(curr)!=relMap.end()){
            for(int next : relMap[curr]){
                toVisit.push_back(next);
            }
        }
    }
    return paths;
}
int countPermutationsOfRun(vector<int> v_run){
    // vector relationships for example, 0 to 1, 0 to 2, 0 to 3, 1 to 2
    map<int, vector<int>> relationshipMap;
    // find all +1 gaps and put them in to vector
    findAndAddGapSize(v_run, 1, relationshipMap);
    findAndAddGapSize(v_run, 2, relationshipMap);
    findAndAddGapSize(v_run, 3, relationshipMap);

    // start from 0 and DFS to count all paths from begining of map to the end.
    return AllPathsBfs(v_run.front(), v_run.back(), relationshipMap);

}
int main(int argc, char * argv[]){
    std::cout << "hello world" << endl;
    vector<int> adapters;
    if(argc < 2){
        throw runtime_error("No file arg.");
    }
    else{
        adapters = loadInput(argv[1]);
    }
    
    for(int i = 0; i < adapters.size(); i++){
        std::cout << i << ": " << adapters.at(i) << endl;
    }
    differences diff = countJoltDifferences(adapters);
    std::cout << "Part 1: " << diff.oneJolt * diff.threeJolt << endl;
    vector<vector<int>> longRun = findAllLongRun(adapters, 0);
    unsigned long long allPermsCount = 1;
    for(auto r : longRun){
        int perms = countPermutationsOfRun(r);
        allPermsCount *= perms;
    }
    std::cout << "Part 2: " << allPermsCount << endl;
    return 0;
}