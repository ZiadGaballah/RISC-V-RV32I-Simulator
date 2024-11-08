#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
using namespace std;

void add(int &dest , int &s1, int &s2){
    dest = s1 + s2;
}

void addi(int &dest , int &s1, int imm){
    dest = s1 + imm;
}
void sub(int &dest , int &s1, int &s2){
    dest = s1 - s2;
}

void subi(int &dest , int &s1, int imm){
    dest = s1 - imm;
}

void lui(int& dest , int imm){
    dest = unsigned(imm << 12);
}

void jal(string label, unordered_map<string,int> labels , int& pc, int& dest){
    dest = pc + 1;
    pc = labels[label];
}
void get_instructions(vector<pair<string,string>> &instructions){
    ifstream program("program.txt");
    string curr_line;
    while(!program.eof()){
        getline(program,curr_line);
        if (curr_line.empty() || curr_line[0] == '#') continue;
        stringstream str(curr_line);
        pair<string,string> line;
        getline(str, line.first, ' ');
        getline(str , line.second);
        instructions.push_back(line);
    }
}
string get_format(string operation){
    if(operation == "add" || operation == "sub") return "r";
    else if(operation == "addi" || operation == "subi") return "i";
    else return "u";
}
void perform_instruction(pair<string,string> instruction , vector<int> &reg , vector<int>& mem, unordered_map<string, int> &regtoindex, unordered_map<string,int> labels, int& pc){
    string format = get_format(instruction.first);
    string dest;
    string source1;
    string source2;
    string imm;
    stringstream str(instruction.second);
    if(format == "r"){
        getline(str,dest,',');
        getline(str ,source1, ',');
        getline(str,source2);
        if(instruction.first == "add"){
            add(reg[regtoindex[dest]],reg[regtoindex[source1]],reg[regtoindex[source2]]);
        }
        else{
            sub(reg[regtoindex[dest]],reg[regtoindex[source1]],reg[regtoindex[source2]]);
        }
    }

    else if(format == "i"){
        getline(str,dest,',');
        getline(str,source1,',');
        getline(str,imm);
        if(instruction.first == "addi"){ 
            addi(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm));
        }
        else{
            subi(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm));
        }


    }

    else if(format == "u"){
        getline(str, dest , ',');
        getline(str, imm);
        if(instruction.first == "lui") lui(reg[regtoindex[dest]], stoi(imm));
        else if(instruction.first == "jal") jal(imm,labels,pc, reg[regtoindex[dest]]);
    }
}

void map_reg(unordered_map<string,int>& regtoindex , vector<string> reg_name){
    for(int i = 0 ; i < 32 ; i++){
       regtoindex[reg_name[i]] = i;
    }
    regtoindex["zero"] = 0;
    regtoindex["ra"] = 1;
    regtoindex["sp"] = 2;
    regtoindex["gp"] = 3;
    regtoindex["tp"] = 4;
    regtoindex["t0"] = 5;
    regtoindex["t1"] = 6;
    regtoindex["t2"] = 7;
    regtoindex["s0"] = 8;
    regtoindex["s1"] = 9;
    regtoindex["a0"] = 10;
    regtoindex["a1"] = 11;
    regtoindex["a2"] = 12;
    regtoindex["a3"] = 13;
    regtoindex["a4"] = 14;
    regtoindex["a5"] = 15;
    regtoindex["a6"] = 16;
    regtoindex["a7"] = 17;
    regtoindex["s2"] = 18;
    regtoindex["s3"] = 19;
    regtoindex["s4"] = 20;
    regtoindex["s5"] = 21;
    regtoindex["s6"] = 22;
    regtoindex["s7"] = 23;
    regtoindex["s8"] = 24;
    regtoindex["s9"] = 25;
    regtoindex["s10"] = 26;
    regtoindex["s11"] = 27;
    regtoindex["t3"] = 28;
    regtoindex["t4"] = 29;
    regtoindex["t5"] = 30;
    regtoindex["t6"] = 31;
    
}
void assembeler(vector<int>& reg , vector<int>& mem , vector<pair<string,string>> &instructions, vector<string>& reg_name){
    unordered_map<string, int> regtoindex;
    unordered_map<string,int> labels;
    int pc  = 0;
    map_reg(regtoindex,reg_name);
    get_instructions(instructions);
    for(int i = 0 ; i < instructions.size() ; i++){
        string label = instructions[i].first;
        if (label.back() == ':') {
            labels[label.substr(0, label.size() - 1)] = i;
        }
    }
    
    while(pc < instructions.size()){
        pair<string, string> instruction = instructions[pc];
        perform_instruction(instruction, reg, mem, regtoindex, labels, pc);
        pc++; 
    }

}
void  print_reg(vector<int> reg , vector<string> reg_name){
    for(int i = 0 ; i < 32 ; i++){
        cout << reg_name[i] << " : " << reg[i] << "\n";
    }
}
int main(){
    vector<int> reg(32);
    vector<string> reg_name(32);
    for(int i = 0 ; i < 32 ;i++){
        reg_name[i] = "x" + to_string(i);
    }
    vector<int> mem(1024);
    vector<pair<string,string>> instructions;
    assembeler(reg,mem,instructions,reg_name);
    print_reg(reg,reg_name);

}