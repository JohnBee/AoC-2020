#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

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

vector<string> readInputFile(){
    vector<string> file;
    string line;
    ifstream input("input");
    cout << "hello world\n";
    while(!input.eof()){
        getline(input, line);
        file.push_back(line);
    }
    input.close();
    return file;
}

vector<vector<string>> inputToBlocks(vector<string> lines){
    vector<vector<string>> out;
    vector<string> block;
    for(auto l : lines){
        if(l.length() > 1){
            block.push_back(l);
        }
        else {
            out.push_back(block);
            block.clear();
        }
    }
    return out;
}
vector<string> fixBlockToOneIdPerLine(vector<string> block){
    vector<string> fixedBlock;
    for(auto l : block){
        vector<string> bSplit = splitString(l, " ");
        for(auto bs : bSplit){
            fixedBlock.push_back(bs);
        }
    }
    return fixedBlock;
}
int countValidPassports(vector<vector<string>> blocks){
    int count = 0;
    for(auto b : blocks){
        if(b.size() == 8){
            count +=1;
        }
        else {
            // check for missing cid
            if(b.size() == 7){
                bool valid = true;
                for(auto e : b){
                    vector<string> eSplit = splitString(e,":");
                    if(eSplit[0] == "cid"){
                        valid = false;
                        break;
                    }
                    
                }
                if(valid){
                    count += 1;
                }
            }
        }
    }
    return count;
}
bool validField(string field, string value){
    if(field == "byr"){
        //birth year
        int year = stoi(value);
        if(year < 1920 || year > 2002){
            cout << "wrong birth year" << endl;
            return false;
        }
    }
    if(field == "iyr"){
        //issue year
        int year = stoi(value);
        if(year < 2010 || year > 2020){
            cout << "wrong issue year" << endl;
            return false;
        }
    }
    if(field == "eyr"){
        //Expiration year
        int year = stoi(value);
        if(year < 2020 || year > 2030){
            cout << "wrong expiration year" << endl;
            return false;
        }
    }
    if(field == "hgt"){
        //height
        if(value[value.length()-1] == 'm'){
            //cm
            vector<string> splitCm = splitString(value,"cm");
            int cm = stoi(splitCm[0]);
            if(cm < 150 || cm > 193){
                cout << "wrong height cm" << endl;
                return false;
            }
        }
        else if(value[value.length()-1] == 'n') {
            //in
            vector<string> splitIn = splitString(value,"in");
            int in = stoi(splitIn[0]);
            if(in < 59 || in > 76){
                cout << "wrong height in" << endl;
                return false;
            }
        }
        else {
            cout << "missing height post fix cm/in" << endl;
            return false;
        }
    }
    if(field == "hcl"){
        if(value.length() != 7){
            cout << "hcl not 7 long" << endl;
            return false;
        }
        if(value[0] != '#'){
            cout << "hcl not begin with #" << endl;
            return false;
        }
        string validChars = "0123456789abcdef";
        for(int i = 1; i < value.length() ; i++){
            if(validChars.find(value[i], 0) == string::npos){
                cout << "Hcl contains invalid characters" << endl;
                return false;
            }
        }
    }
    if(field == "ecl"){
        vector<string> validCol = {"amb", "blu","brn", "gry","grn","hzl","oth"};
        vector<string>::iterator it;
        it = std::find(validCol.begin(), validCol.end(), value);
        if(it == validCol.end()){
            cout << "invalid eye colour" << endl;
            return false;
        }
    }
    if(field == "pid"){
        if(value.length() != 9){
            cout << "pid not length 9" << endl;
            return false;
        }
        string validChars = "0123456789";
        for(int i = 1; i < value.length() ; i++){
            if(validChars.find(value[i], 0) == string::npos){
                cout << "pid invalid character" << endl;
                return false;
            }
        }
    }

    return true;
}
void printBlock(vector<string> block){
    for(auto l : block){
        cout << l << endl;
    }
}
int countValidPassportsPart2(vector<vector<string>> blocks){
    int count = 0;
    for(auto b : blocks){
        bool valid = true;
        if(b.size() == 7){
            for(auto e : b){
                vector<string> eSplit = splitString(e,":");
                if(eSplit[0] == "cid"){
                    cout << "7 length, has cid" << endl;
                    valid = false;
                    break;
                }
                
            }
        }
        if(b.size() != 8 && b.size() != 7){
            cout << "wrong size: " << b.size() << endl;
            valid = false;
        }
        // check fields
        for(auto e : b){
            vector<string> eSplit = splitString(e, ":");
            if(!validField(eSplit[0], eSplit[1])){
                valid = false;
                break;
            }
        }
        if(valid){
            count += 1;
        }
        else{
            printBlock(b);
        }
    }
    return count;
}
int main(){
    vector<string> file = readInputFile();
    // for(auto f : file){
    //     cout << f << endl;
    // }
    vector<vector<string>> blocks = inputToBlocks(file);
    // fixed blocks
    vector<vector<string>> fixedBlocks;
    for(auto b : blocks){
        fixedBlocks.push_back(fixBlockToOneIdPerLine(b));
    }
    // // print blocks
    // for(auto b : fixedBlocks){
    //     for(auto l : b){
    //         cout << l << endl;
    //     }
    //     cout << endl;
    // }
    int countValid = countValidPassports(fixedBlocks);
    cout << "P1 " << countValid << endl;

    int countValid2 = countValidPassportsPart2(fixedBlocks);
    cout << "P2 " << countValid2 << endl;
    return 0;
}