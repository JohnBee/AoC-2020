#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <exception>
#include <math.h>

#define PI 3.14159265

using namespace std;

typedef pair<char, int> action;

struct Ship{
    pair<double, double> position = make_pair(0, 0);
    pair<double, double> waypoint = make_pair(10, -1);
    enum e_direction {NORTH, EAST, SOUTH, WEST};
    double facing = 0; // 0 = East
    void run(const vector<action> & actions){
        for(action a : actions){
            if(a.first=='N'){
                position.second -= a.second;
            }
            if(a.first=='E'){
                position.first += a.second;
            }
            if(a.first=='S'){
                position.second += a.second;
            }
            if(a.first=='W'){
                position.first -= a.second;
            }
            if(a.first=='L'){
                facing -= a.second;
            }
            if(a.first=='R'){
                facing += a.second;
            }
            if(a.first=='F'){
                // angle to radians
                double angle = (facing*PI/180);
                double yChange = sin(angle)*a.second;
                double xChange = cos(angle)*a.second;
                position.first +=xChange;
                position.second +=yChange;
            }
        }
    }
    void runPart2(vector<action> actions){
        for(action a : actions){
            if(a.first=='N'){
                waypoint.second -= a.second;
            }
            if(a.first=='E'){
                waypoint.first += a.second;
            }
            if(a.first=='S'){
                waypoint.second += a.second;
            }
            if(a.first=='W'){
                waypoint.first -= a.second;
            }
            if(a.first=='L'){
                // rotate waypoint around the ship using the number of degrees
                // origin is still 0,0 as waypoint is always centered around the ship
                double angle = (-a.second*PI/180);

                double xChange = cos(angle)*(waypoint.first) - sin(angle)*(waypoint.second);
                double yChange = sin(angle)*(waypoint.first) + cos(angle)*(waypoint.second);
                
                waypoint.first = round(xChange);
                waypoint.second = round(yChange);
            }
            if(a.first=='R'){
                // rotate waypoint around the ship using the number of degrees
                // origin is still 0,0 as waypoint is always centered around the ship
                double angle = (a.second*PI/180);

                double xChange = cos(angle)*(waypoint.first) - sin(angle)*(waypoint.second);
                double yChange = sin(angle)*(waypoint.first) + cos(angle)*(waypoint.second);
                
                waypoint.first = round(xChange);
                waypoint.second = round(yChange);
            }
            if(a.first=='F'){
                double xDist = waypoint.first;
                double yDist = waypoint.second;
                position.first += xDist * a.second;
                position.second += yDist * a.second;

            }
        }

    }
    double manhattanDistance(){
        return abs(position.first)+abs(position.second);
    }

};
action splitStringToAction(string s){
    regex s_regex("[A-Z]|[0-9]+");
    vector<string> out(sregex_token_iterator(s.begin(), s.end(), s_regex, 0), sregex_token_iterator());
    action a_out;
    a_out.first = out.at(0).at(0);
    a_out.second = stoi(out.at(1));
    return a_out;
}

vector<action> loadInput(string filename){
    cout << "[Working] loading input" << endl;
    ifstream file;
    file.open(filename);
    if(file.bad()){
        runtime_error("cannot open file.");
    }
    string line;
    vector<action> out;
    while(!file.eof()){
        getline(file, line);
        if(line.length()!=0){
            out.push_back(splitStringToAction(line));
        }
    }
	file.close();
    return out;
}

int main(int argc, char * argv[]){
    cout << "Day 12" << endl;
    if(argc < 2){

        runtime_error("missing file parameter.");
        return 0;
    }
    vector<action> actions = loadInput(argv[1]);
    Ship ship;
    ship.run(actions);
    cout << "Part 1: " << ship.manhattanDistance() << endl;
    Ship ship2;
    ship2.runPart2(actions);
    cout << "Part 2: " << ship2.manhattanDistance() << endl;
    return 0;
}