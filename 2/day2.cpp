#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

vector<string> splitString(string str, string delim = " "){
    vector<string> out;
    int start = 0;
    int end = str.find(delim);
    while (end != -1){
        out.push_back(str.substr(start, end - start));
        start = end + delim.size();
        end = str.find(delim, start);
    }
    out.push_back(str.substr(start, end - start));
    return out;
}
int countLetters(char letter, string word){
    int count = 0;
    for(auto c: word){
        if(c==letter){
            count += 1;
        }
    }
    return count;
}
bool meetsPolicy_part1(string policy){
    // split string in to tokens [count] [letter]: [password]
    vector<string> tokens = splitString(policy," ");
    // get letter count;
    vector<string> countTokens = splitString(tokens[0],"-");
    int start = stoi(countTokens[0]);
    int end = stoi(countTokens[1]);

    char letter = splitString(tokens[1],":")[0][0];
    string word = tokens[2];
    int count = countLetters(letter, word);

    //cout << start << "-" << end << " " << count << " " << letter <<  " " << word << " " << ((count < start || count > end) ? "False" : "True") << endl;

    if(count < start || count > end){
        return false;
    }
    return true;
}
bool countLetters_part2(int start, int end, char letter, string word){
    bool s = (word[start-1] == letter);
    bool e = (word[end-1] == letter);
    return s ^ e;
    
}
bool meetsPolicy_part2(string policy){
    // split string in to tokens [count] [letter]: [password]
    vector<string> tokens = splitString(policy," ");
    // get letter count;
    vector<string> countTokens = splitString(tokens[0],"-");
    int start = stoi(countTokens[0]);
    int end = stoi(countTokens[1]);

    char letter = splitString(tokens[1],":")[0][0];
    string word = tokens[2];
    bool count = countLetters_part2(start, end, letter, word);

    cout << start << "-" << end << " " << count << " " << letter <<  " " << word << endl;

    return count;
}


int main(){
    vector<string> passwords;
    string pass;
    ifstream input("input");
    cout << "hello world\n";
    while(!input.eof()){
        getline(input, pass);
        if(pass!=""){
            passwords.push_back(pass);
        }
        
    }
    input.close();
    int totalMeetPolicy_part1 = 0;
    int totalMeetPolicy_part2 = 0;
    cout << passwords.size() << endl;
    for(auto p: passwords){
        if(meetsPolicy_part1(p)){
            totalMeetPolicy_part1 += 1;
        }
    }
    for(auto p: passwords){
        if(meetsPolicy_part2(p)){
            totalMeetPolicy_part2 += 1;
        }
    }

    cout << "P1: " << totalMeetPolicy_part1 << endl; 
    cout << "P2: " << totalMeetPolicy_part2 << endl; 
    return 0;
}