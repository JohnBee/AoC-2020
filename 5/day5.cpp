#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#define DEBUG false

using namespace std;

vector<string> readInputFile(){
    vector<string> file;
    string line;
    ifstream input("input");
    while(!input.eof()){
        getline(input, line);
        if(line.length() > 0){
            file.push_back(line);
        }
        
    }
    input.close();
    return file;
}
int decodeRow(string row){
    int lower = 0;
    int upper = 127;
    int step = 64;
    int i;
    for(i = 0; i < 6; i++){
         
        if(row[i] == 'F'){
            upper -= step;
        }
        if(row[i] == 'B'){
            lower += step;
        }
        step = step / 2;
        if(DEBUG){cout << row[i] << " " << lower << " " << upper << endl;}
        
    }
    
    int row_i = ((row[6] == 'F') ? lower : upper);
    if(DEBUG){cout << row[6] << " " << ((row[6] == 'F') ? "lower" : "upper") << " " << row_i << endl;}

    lower = 0;
    upper = 7;
    step = 4;
    for(i = 7; i < row.length()-1; i++){
 
        if(row[i] == 'L'){
            upper -= step;
        }
        if(row[i] == 'R'){
            lower += step;
        }
        step = step / 2;
        if(DEBUG){cout << row[i] << " " << lower << " " << upper << endl;} 
    }
     

    int column_i = ((row[row.length()-1] == 'L') ? lower : upper);
    if(DEBUG){cout << row[row.length()-1] << " " << ((row[row.length()-1] == 'L') ? "lower" : "upper") << " " << column_i << endl;}
    if(DEBUG){cout << "Row: " << row_i << " Column: " << column_i << endl;}
    return(row_i * 8 + column_i);
}

int highestBoardingPass(vector<string> file){
    int highest = 0;
    for(auto l : file){
        int seatId = decodeRow(l);
        if(seatId > highest){
            highest = seatId;
        }
        if(DEBUG){cout << l << " " << seatId << endl;}
    }
    return highest;
}

int findMySeatId(vector<string> file){
    vector<int> seatIds;
    for(auto l : file){
        seatIds.push_back(decodeRow(l));
    }
    sort(seatIds.begin(), seatIds.end());
    for(int i = 0; i < seatIds.size()-1; i++){
        if(seatIds[i+1]-seatIds[i] == 2){
            return seatIds[i]+1;
        }
    }
    return 0;
}


int main(){
    vector<string> file = readInputFile();
    cout << "Part 1: " << highestBoardingPass(file) << endl;
    cout << "Part 2: " << findMySeatId(file) << endl;
    return 0;
}