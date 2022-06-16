#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <iostream>
#include "CPU.h"
#include <algorithm>

#define PART_1 false
#define PART_2 true

std::vector<std::string> CPU::loadInstructionsRaw(std::string filename){
    std::vector<std::string> instructionsRaw;
    std::ifstream file;
    file.open(filename);
    std::string line;
    if(file.fail()){
        std::cout << "can not find file: " << filename << std::endl;
        exit(0);
    }
    while(!file.eof()){
        std::getline(file, line);
        instructionsRaw.push_back(line);
    }
    file.close();
    return instructionsRaw;
}
std::vector<std::string> CPU::regexMatch(std::string s, std::string regex_delim, bool exclude){
    std::regex s_regex(regex_delim);
    int toExclude = exclude ? -1 : 0;
    std::vector<std::string> out(std::sregex_token_iterator(s.begin(), s.end(), s_regex, toExclude), std::sregex_token_iterator());
    return out;
}
CPU::t_instruction CPU::buildInstructionFromString(std::string rawInstruction){
    std::vector<std::string> splitString = regexMatch(rawInstruction, " ", true);
    t_operator _operator = splitString.at(0); //get the operator, should be the first string
    t_operand _operand;
    for(std::vector<std::string>::iterator i = splitString.begin()+1; i!=splitString.end(); i++){
        _operand.push_back(*i);
    }
    return std::make_pair(_operator,_operand);
}
CPU::t_program CPU::buildProgram(std::vector<std::string> rawInstructions){
    t_program _program;
    for(auto ri : rawInstructions){
        _program.push_back(buildInstructionFromString(ri));
    }
    return _program;
}
CPU::t_instruction CPU::getNextInstruction(){
    t_instruction instruction = program.at(ip);
    ip++;
    return instruction;
}
void CPU::executeInstruction(t_instruction instruction){
    if(instruction.first == "nop"){
        // do nothing
        return;
    }
    if(instruction.first=="acc"){
        //increase accumulator
        int incr = std::stoi(instruction.second.at(0));
        acc += incr;
        return;
    }
    if(instruction.first=="jmp"){
        int loc = std::stoi(instruction.second.at(0));
        ip += loc-1; // -1 to account for the instruciton pointer pointing at the next instruction already.
        return;
    }
}
void CPU::logIp(int ip){
    ipLog.push_back(ip);
}
bool CPU::instructionPreviouslyExecuted(int ip){
    if(std::find(ipLog.begin(), ipLog.end(),ip)!=ipLog.end()){
        //found ip in list
        return true;
    }
    return false;
}
void CPU::reset(){
    acc = 0;
    ip = 0;
    ipLog.clear();
}
int CPU::getAcc(){
    return acc;
}
bool CPU::run(){
    t_instruction instruction;
    while(ip < program.size()){
        if(instructionPreviouslyExecuted(ip)==false){
            logIp(ip);
        }
        else{
            return false;
        }
        instruction = getNextInstruction();
        executeInstruction(instruction);
    }
    return true;
}
void CPU::run_part2(){
    // find all the indexes where instruction is a nop or jmp.
    std::vector<t_program::iterator> indexes;
    for(t_program::iterator i = program.begin(); i!=program.end(); ++i){
        if(i->first == "nop" || i->first == "jmp"){
            indexes.push_back(i);
        }
    }
    for(t_program::iterator ti : indexes){
        reset();
        if(ti->first == "nop"){
            ti->first = "jmp";
        }
        else if(ti->first=="jmp"){
            ti->first="nop";
        }
        if(run()){
            //program fixed
            return;
        }
        else{
            // swap them back
            if(ti->first == "nop"){
                ti->first = "jmp";
            }
            else if(ti->first=="jmp"){
                ti->first="nop";
            }
        }
    }
    return;

}
CPU::CPU(std::string filename){
    acc = 0;
    ip = 0;
    program = buildProgram(loadInstructionsRaw(filename));
}