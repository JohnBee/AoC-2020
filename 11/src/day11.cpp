#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <exception>
#include <map>

using namespace std;

typedef vector<vector<char>> t_layout;
typedef pair<int, int> t_coord;
typedef map<t_coord, vector<t_coord>> t_occupiedMap;
typedef vector<vector<vector<t_coord>>> t_occupiedMap_v;

t_layout loadInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        cout << "missing file: " << filename << endl;
        throw runtime_error("Cannot open file.");
    }
    string line;
    t_layout layout;
    vector<char> cMap;
    while(!file.eof()){
        cMap.clear();
        getline(file, line);
        if(line.length() > 0){
            
            for(auto c : line){
                cMap.push_back(c);
            }
        }
        layout.push_back(cMap);
    }
	file.close();
    return layout;
}
int adjacentOccupied(const t_layout & layout, int _x, int _y){
    int adjOcc = 0;
    for(int y = _y-1; y <= _y+1; y++){
        for(int x = _x-1; x <= _x+1; x++){
            if(x>=0){
                if(y>=0){
                    if(y < layout.size()){
                        if(x < layout[y].size()){
                            if(!(x == _x && y == _y)){
                                if(layout[y][x] == '#'){
                                    adjOcc +=1;
                                }
                            }
                        }
                    }
                }
            } 
        }
    }
    return adjOcc;
}
t_layout stepLayout(t_layout & layout){
    //make empty
    t_layout next;
    for(auto y : layout){
        vector<char> row;
        for(auto x : y){
            row.push_back('.');
        }
        next.push_back(row);
    }
    for(int y = 0; y < layout.size(); y++){
        for(int x = 0; x < layout[y].size(); x ++){
            next[y][x] = layout[y][x];
            if(layout[y][x]=='L'){
                //if seat is empty and no occupied seats around, then seat becomes occupied.
                if(adjacentOccupied(layout, x, y) == 0){
                    next[y][x] = '#';
                }
            }
            if(layout[y][x]=='#'){
                if(adjacentOccupied(layout, x, y) >= 4){
                    next[y][x] = 'L';
                }
            }
        }
    }
    return next;
}
t_layout copyLayout(const t_layout & layout){
    t_layout out;
    for(int y = 0; y < layout.size(); y++){
        out.push_back({});
        for(int x = 0; x < layout[y].size(); x ++){
            out[y].push_back(layout[y][x]);
        }
    }
    return out;
}
void printLayout(const t_layout & layout){
    for(auto y : layout){
        for(auto x : y){
            cout << x;
        }
        cout << endl;
    }
    cout << endl;
}
bool sameLayout(const t_layout & l1, const t_layout & l2){
    if(l1.size() != l2.size()){
        return false;
    }
    for(int y = 0; y < l1.size(); y++){
        for(int x = 0; x < l1[y].size(); x++){
            if(l1[y][x] != l2[y][x]){
                return false;
            }
        }
    }
    return true;
}
int countOccupiedSeats(const t_layout & layout){
    int out = 0;
    for(auto row : layout){
        for(auto seat : row){
            if(seat == '#'){
                out +=1;
            }
        }
    }
    return out;
}
pair<int,int> seatExistsDirectional(const t_layout & layout, int startx, int starty, int directX, int directY){
    // x 0 +1 -1
    // y 0 +1 -1 to create 8 way directional

    int curx = startx;
    int cury = starty;
    while(curx >= 0 && cury >= 0 && cury < layout.size() && curx < layout[cury].size()){
        curx += directX;
        cury += directY;
        if(curx < 0 || cury < 0 || cury >= layout.size() || curx >= layout[cury].size()){
            return make_pair(-1, -1);
        }
        if(layout[cury][curx]=='#' || layout[cury][curx]=='L'){
            return make_pair(curx, cury);

        }
    }
    return make_pair(-1, -1);;
}
t_occupiedMap_v buildOccupiedLookupMap(const t_layout & layout){
    t_occupiedMap_v occMap;
    for(int y = 0; y < layout.size(); y++){
        occMap.push_back({}); //y
        for(int x = 0; x < layout[y].size(); x++){
            // x
            occMap[y].push_back({}); //
            pair<int,int> currentCoord = make_pair(x,y);
            vector<int> directions = {-1, 0, 1};
            for(auto i : directions){
                for(auto j : directions){
                    if(i !=0 || j != 0){
                        t_coord occupiedCoord = seatExistsDirectional(layout, x, y, j, i);
                        if(occupiedCoord.first != -1){
                            occMap[y][x].push_back(occupiedCoord);
                        }
                    }
                }
            }
        }
    }
    return occMap;
}
int countOccupiedFirstDirectional(const t_layout & layout, const t_occupiedMap_v & occMap, int startx, int starty){
    // check from given seat if the first seat found in 8 directions is occupied or not
    int out = 0;
    // up
    vector<t_coord> lookupCoords = occMap[starty][startx];
    for(pair<int,int> coords : lookupCoords){
        if(layout[coords.second][coords.first] == '#'){
            out +=1;
        }
    }
    return out;
}
t_layout stepLayoutDirectional(const t_layout & layout, const t_occupiedMap_v & occMap){
    //make empty
    t_layout next;
    for(auto y : layout){
        vector<char> row;
        for(auto x : y){
            row.push_back('.');
        }
        next.push_back(row);
    }
    for(int y = 0; y < layout.size(); y++){
        for(int x = 0; x < layout[y].size(); x ++){
            next[y][x] = layout[y][x];
            int adjcount = 0;
            if(layout[y][x]=='L' || layout[y][x]=='#'){
                adjcount = countOccupiedFirstDirectional(layout, occMap, x, y);
            }
            if(layout[y][x]=='L'){
                //if seat is empty and no occupied seats around, then seat becomes occupied.
                if(adjcount == 0){
                    next[y][x] = '#';
                }
            }
            if(layout[y][x]=='#'){
                if(adjcount >= 5){
                    next[y][x] = 'L';
                }
            }
        }
    }
    return next;
}
void part1(string filename){
    t_layout layout = loadInput(filename);
    t_layout prev;
    int steps = 0;
    while(!sameLayout(layout, prev)){
        prev = copyLayout(layout);
        layout = stepLayout(layout);
        // steps += 1;
        // cout << "steps: " << steps << endl;
    }
    cout << "Part 1: " << countOccupiedSeats(layout) << endl;

}
void part2(string filename){
    t_layout layout = loadInput(filename);
    t_layout prev;
    int steps = 0;
    cout << "[Working] Building map." << endl;
    t_occupiedMap_v occMap = buildOccupiedLookupMap(layout);
    cout << "[Done] Built map." << endl;
    while(!sameLayout(layout, prev)){
        prev = copyLayout(layout);
        //printLayout(layout);
        layout = stepLayoutDirectional(layout, occMap);
        //steps += 1;
        //cout << "steps: " << steps << endl;
    }
    cout << "Part 2: " << countOccupiedSeats(layout) << endl;

}
int main(int argc, char * argv[]){
    cout << "Day 11" << endl;
    if(argc < 2){
        runtime_error("missing filename arg.");
    }
    part1(argv[1]);
    part2(argv[1]);
    return 0;
}