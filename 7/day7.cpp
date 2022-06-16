#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>

#define DEBUG_1 false
#define DEBUG_2 false

using namespace std;

using conversion_map = map<string, vector<pair<int, string>>>;
using bag_contents = pair<int, string>;

vector<string> readInputFile(){
    vector<string> out;
    ifstream file;
    file.open("input");
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

vector<string> splitString(string s, string regex_delim){
    regex s_regex(regex_delim);
    vector<string> out(sregex_token_iterator(s.begin(), s.end(), s_regex, -1), sregex_token_iterator());
    return out;
}
vector<string> regexMatch(string s, string regex_delim){
    regex s_regex(regex_delim);
    vector<string> out(sregex_token_iterator(s.begin(), s.end(), s_regex, 0), sregex_token_iterator());
    return out;
}

string cleanUpString(string s){
    regex s_regex("( bags| bag| contain)");
    string cleaned;
    regex_replace(back_inserter(cleaned), s.begin(),s.end(), s_regex,"");
    return cleaned;
}
vector<string> extractStringInfo(string s){
    string cleaned = cleanUpString(s);
    vector<string> components = regexMatch(cleaned, "[0-9]? ?[a-z]+ [a-z]+");
    return components;
}
void updateConversionMap(conversion_map &convMap, string from, bag_contents to){
    convMap[from].push_back(to);
}

bag_contents extractCountAndType(string s){
    vector<string> list = regexMatch(s, "[0-9]|[a-z][ a-z]+");
    bag_contents out = make_pair(stoi(list[0]), list[1]);
    return out;
}
void printVectorString(vector<string> input){
    for(auto s : input){
        cout << s << endl;
    }
}
void printConversionMap(conversion_map const &convMap){
    for(auto it = convMap.cbegin(); it != convMap.cend(); ++it){
        cout << it->first << endl;
        for(auto sc : it->second){
            cout << sc.first << " " << sc.second << endl;
        }
    }
}
conversion_map buildConversionMap(vector<string> input){
    conversion_map convMap;
    for(string s : input){
        vector<string> extracted = extractStringInfo(s);
        for(vector<string>::iterator i = extracted.begin()+1; i!=extracted.end(); i++){
            // cout << *i << endl;
            if((*i)[1] == 'n'){
                // no bags
                updateConversionMap(convMap, *extracted.begin(), make_pair(0,""));
            }
            else{
                bag_contents to = extractCountAndType(*i);
                updateConversionMap(convMap, *extracted.begin(), to);
            }
            
            
        }
    }
    return convMap;
}
deque<string> getNext(conversion_map convMap, string current){
    deque<string> out = {};
    for(bag_contents c : convMap[current]){
        if(c.second != ""){
            out.push_back(c.second);
        }
    }
    return out;
}
bool BFS(string start, string goal, conversion_map convMap){
    deque<string> toVisit = {start};
    deque<string> visited = {};
    
    while(toVisit.size() > 0){
        string current = toVisit.front();
        toVisit.pop_front();
        if(current == goal){
            return true;
        }
        else{
            deque<string> next = getNext(convMap, current);
            for(auto n : next){
                if(find(visited.begin(), visited.end(), n) == visited.end()){
                    toVisit.push_back(n);
                }
            }
            visited.push_back(current);
        }
    }

    return false;

}
bool reachesShinyGold(string start, conversion_map convMap){
    return(BFS(start, "shiny gold", convMap));
}
int countHowManyBagsReachShinyGold(conversion_map convMap){
    int count = 0;
    for(auto it = convMap.cbegin(); it != convMap.cend(); ++it){
        if(reachesShinyGold(it->first, convMap)&&(it->first!="shiny gold")){
            count++;
            if(DEBUG_2){cout << it->first << " " << count << endl;};
        }
    }
    return count;
}
vector<bag_contents> getNextWithCount(conversion_map convMap, bag_contents current){
    vector<bag_contents> out = {};
    for(bag_contents c : convMap[current.second]){
        if(c.second != ""){
            out.push_back(c);
        }
        else {
            out.push_back(make_pair(1,current.second));
        }
    }
    return out;
}
int countBags(vector<bag_contents> bags, vector<bag_contents> openedBags){
    int count = 0;
    for(auto b : bags){
        count += b.first;
    }
    for(auto b : openedBags){
        count += b.first;
    }
    return count;
}
int countHowManyBagsAreWithinShinyGold(conversion_map convMap){
    vector<bag_contents> bags = {make_pair(1,"shiny gold")};
    int prevSize = 0;
    vector<bag_contents> openedBags = {};
    int newSize = countBags(bags, openedBags);
    
    while(prevSize != newSize){
        // continue to expand the bags
        vector<bag_contents> expandedBags = {};
        for(auto b : bags){
            vector<bag_contents> t_expanded = getNextWithCount(convMap, b);
            for(auto t : t_expanded){
                // add bags to existing
                expandedBags.push_back(make_pair(t.first*b.first, t.second));
            }
            //check if bag we're opening contains it's self then the bag is empty and we didn't 'open' it, if it does contains something then we opened it.
            if(b.second != expandedBags.at(expandedBags.size()-1).second){
                openedBags.push_back(b);
            }
            
        }
        bags = expandedBags;
        prevSize = newSize;
        newSize = countBags(bags, openedBags);
    } 
    // sum up the ammount of bags, -1 as we're not including the shiny gold bag.
    return countBags(bags, openedBags)-1;
}
int main(){
    // cout << "hello world" << endl;
    vector<string> text = readInputFile();
    conversion_map convMap = buildConversionMap(text);
    //printConversionMap(convMap);
    cout << "Part 1: " << countHowManyBagsReachShinyGold(convMap) << endl;
    cout << "Part 2: " << countHowManyBagsAreWithinShinyGold(convMap) << endl;
    return 0;
}