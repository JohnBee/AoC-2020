#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
#include <exception>

using namespace std;

struct TimeTable{
    int leaveTime = 0;
    vector<int> busIds = {};
};

vector<string> loadInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        runtime_error("Cannot open file.");
        cout << "File not found: " << filename << endl;
        exit(0);
    }
    vector<string> out;
    string line;
    while(!file.eof()){
        getline(file, line);
        if(line.length() > 0){
            out.push_back(line);
        }
    }
    file.close();
    return out;
}

vector<int> getBusIds(string s){
    regex s_regex(",");
    vector<string> foundStr(sregex_token_iterator(s.begin(), s.end(), s_regex, -1), sregex_token_iterator());
    vector<int> out;
    for(auto str : foundStr){
        if(str!="x"){
            out.push_back(stoi(str));
        }
    }
    return out;
}

TimeTable buildInput(const vector<string> & input){
    TimeTable o;
    o.leaveTime = stoi(input[0]);
    o.busIds = getBusIds(input[1]);
    return o;
}
pair<bool, int> divisable(int time, vector<int> busIds){
    for(int b : busIds){
        if(time % b == 0){
            return make_pair(true, b);
        }
    }
    return make_pair(false, 0);
}
int getPart1(const TimeTable & ttable){
    int currentTime = ttable.leaveTime;
    pair<bool, int> divisableBusId = divisable(currentTime, ttable.busIds);
    while(!divisableBusId.first){
        currentTime++;
        divisableBusId = divisable(currentTime, ttable.busIds);
    }
    return (currentTime-ttable.leaveTime)*divisableBusId.second;
}
vector<int> getBusIdsWithGaps(string s){
    regex s_regex("[1-9]+|x");
    vector<string> foundStr(sregex_token_iterator(s.begin(), s.end(), s_regex, 0), sregex_token_iterator());
    vector<int> out;
    for(auto str : foundStr){
        if(str!="x"){
            out.push_back(stoi(str));
        }
        else{
            out.push_back(0);
        }
    }
    return out;
}
pair<int, int> largestNumberIndexPair(vector<int> busIds){
    int largestId = 0;
    int largestIdIndex = 0;
    for(int i = 0; i < busIds.size(); i++){
        if(busIds[i] > largestId){
            largestId = busIds[i];
            largestIdIndex = i;
        }
    }
    return make_pair(largestId, largestIdIndex);
}
bool matchesAllTime(long long time, int largestIndex, const vector<int> & busIds){
    for(int i = 0; i < busIds.size(); i++){
        if((time + (i - largestIndex)) % busIds[i] != 0){
            return false;
        }
    }
    return true;
}

long long gcd(long long p, long long q)
{
    if (q == 0){
        return p;
    }
    return gcd(q, p % q);
}
long long power(long long x, unsigned long long y, unsigned long long m)
{
    if (y == 0)
        return 1;
    int p = power(x, y / 2, m) % m;
    p = (p * p) % m;
 
    return (y % 2 == 0) ? p : (x * p) % m;
}
long long modInverse(long long a, long long m)
{
    int g = gcd(a, m);
    if (g != 1)
        exit(0);
    else
    {
        return power(a, m - 2, m);
    }
}
// chinese remainder theorem

// The time is a number where:
// time x = 0 (mod index 0)
// time x = 1 (mod index 1)
// time x = 2 (mod index 2)
long long crm(vector<int> values){
    vector<int> b; //create list of remainders, which is the index of the busId
    long long N = 1; // N is the product of all numbers not
    vector<long long> mods; 
    vector<long long> ni;
    for(int i = 0; i < values.size(); i++){
        if(values[i]!=0){
            b.push_back((values[i]-i) % values[i]);
            mods.push_back(values[i]);
            N *= values[i];
        }
    }
    // ni is N / i
    for(int i = 0; i < mods.size(); i++){
        ni.push_back(N/mods[i]);
    }
    vector<long long> xi;
    for(int i = 0; i < mods.size(); i++){
        xi.push_back(modInverse(ni[i], mods[i]));
    }
    
    //sum BNX
    long long bnx = 0;
    for(int i =0; i < b.size(); i++){
        bnx += b[i]*ni[i]*xi[i];
    }
    return bnx%N;
}

long long part2(vector<string> input){
    // get busIds with gaps
    vector<int> busIds = getBusIdsWithGaps(input[1]);
    return crm(busIds);
}

int main(int argc, char * argv[]){
    cout << "Day13" << endl;
    vector<string> input = loadInput(argv[1]);
    TimeTable ttable = buildInput(input);
    //cout << "Part 1: " << getPart1(ttable) << endl;
    //cout << part2(input) << endl; 
    cout << "Part 2: " <<  part2(input) << endl;
    return 0;
}