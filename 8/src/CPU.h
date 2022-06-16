#ifndef CPU_H
#define CPU_H

#include <string>
#include <vector>


class CPU{
    private:
        typedef std::string t_operator;
        typedef std::vector<std::string> t_operand;
        typedef std::pair<t_operator, t_operand> t_instruction;
        typedef std::vector<t_instruction> t_program;
        int acc; //accumulator
        int ip; //instruction pointer
        t_program program;
        std::vector<int> ipLog;

        std::vector<std::string> loadInstructionsRaw(std::string filename);
        std::vector<std::string> regexMatch(std::string s, std::string regex_delim, bool exclude=false);
        t_instruction buildInstructionFromString(std::string rawInstruction);
        t_program buildProgram(std::vector<std::string> rawInstructions);
        t_instruction getNextInstruction();
        void logIp(int ip);
        bool instructionPreviouslyExecuted(int ip);
        void executeInstruction(t_instruction instruction);
        
        
    public:
        CPU(std::string filename);
        void reset();
        int getAcc();
        bool run();
        void run_part2();

};


#endif // CPU_H