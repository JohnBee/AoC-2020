#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main(){
    ifstream Input("input");
    string inText;
    vector<int> intList;
    while (getline (Input, inText)) {
        //cout << inText << "\n";
        intList.push_back(stoi(inText));
    }
    Input.close();
    // for each pair in vector check if sum 2020 & not its self
    for(int i = 0; i < intList.size(); i++){
        for(int j = i+1; j < intList.size(); j++){
            if(intList[i] + intList[j] == 2020){
                cout << intList[i] << " " << intList[j] << "\n";
                cout << "P1 Answer:" << intList[i] * intList[j] << "\n";
                break;
            }
        }
    }
    // for each triple in vector check if sum 2020 & not its self
    for(int i = 0; i < intList.size(); i++){
        for(int j = i+1; j < intList.size(); j++){
            for(int k = j+1; k < intList.size(); k++){
                if(intList[i] + intList[j] + intList[k] == 2020){
                    cout << intList[i] << " " << intList[j] << " " << intList[k] << "\n";
                    cout << "P2 Answer:" << intList[i] * intList[j] * intList[k] << "\n";
                    break;
                }
            }
        }
    }
    return 0;
}