#include <iostream>
#include "CPU.h"

using namespace std;

#define PART_1 true
#define PART_2 true

int main(int argc, char*argv[]){
    cout << "hello world" << endl;
    if(argc < 2){
        cout << "missing path to input file" << endl;
        return 1;
        
    }
    CPU cpu(argv[1]);
    cout << "loaded cpu" << endl;
    cout << "running cpu" << endl;
    if(PART_1){
        cpu.run();
        cout << "Part 1: " << cpu.getAcc() << endl;
        cpu.reset();
    }
    if(PART_2){
        cpu.run_part2();
        cout << "Part 2: " << cpu.getAcc() << endl;
        cpu.reset();
    }
    
    return 0;
}