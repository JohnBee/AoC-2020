#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

char getChar(int x, int y, vector<string> lines){
    int maxWidth = lines[0].size();
    int restrictedX = x % maxWidth;
    return lines[y][restrictedX];
}
int countTrees(int right, int down, vector<string> lines){
    int y = down;
    int x = right;
    int treeCount =  0;
    while(y < lines.size()){
        char c = getChar(x, y, lines);
        if(c == '#'){
            treeCount += 1;
        }
        y += down;
        x += right;

    }
    return treeCount;
}

int main(){
    vector<string> lines;
    string line;
    ifstream input("input");
    cout << "hello world\n";
    while(!input.eof()){
        getline(input, line);
        if(line!=""){
            lines.push_back(line);
            cout << line << endl;
        }
        
    }
    input.close();



    cout << "Yep" << "\n";
    cout << lines[0].size() << endl;
    cout << 31 % 31 << endl;
    cout << lines[0][30] << endl;
    cout << lines.size() << endl;
    int trees = countTrees(3,1, lines);
    cout << "Part 1 " << trees << endl;
    unsigned long a = countTrees(1,1, lines);
    unsigned long b = countTrees(3,1, lines);
    unsigned long c = countTrees(5,1, lines);
    unsigned long d = countTrees(7,1, lines);
    unsigned long e = countTrees(1,2, lines);
    unsigned long res = a*b*c*d*e;
    cout << "Part 2 " << res << endl;
    cout << "Finished" << endl;
    return 0;
}