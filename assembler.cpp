#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <cctype>
#include <algorithm>
#include <iomanip>
#include <bitset>
using namespace std;
const int MEMORY_START_ADDRESS = 100;

void and_op(int &dest, int &s1, int &s2) {
    dest = s1 & s2;
}

void or_op(int &dest, int &s1, int &s2) {
    dest = s1 | s2;
}

void xor_op(int &dest, int &s1, int &s2) {
    dest = s1 ^ s2;
}

void andi(int &dest , int &s1, int imm){
    dest = s1 & imm;
}

void ori(int &dest , int &s1, int imm){
    dest = s1 | imm;
}

void xori(int &dest , int &s1, int imm){
    dest = s1 ^ imm;
}

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
void lw(int &dest, int &base, int offset, vector<uint8_t> &mem) {
    int address = base + offset - MEMORY_START_ADDRESS;
    if (address >= 0 && (address + 3) < mem.size()) {
        dest = (mem[address]) | 
               (mem[address + 1] << 8) | 
               (mem[address + 2] << 16) | 
               (mem[address + 3] << 24);
    } else {
        cerr << "Memory access error at address: " << address << endl;
    }
}

void lhu(int &dest, int &base, int offset, vector<uint8_t> &mem) {
    int address = base + offset - MEMORY_START_ADDRESS;
    if (address >= 0 && (address + 1) < mem.size()) {
        dest = (mem[address]) | 
               (mem[address + 1] << 8);
    } else {
        cerr << "Memory access error at address: " << address << endl;
    }
}

void lh(int &dest, int &base, int offset, vector<uint8_t> &mem) {
    int address = base + offset - MEMORY_START_ADDRESS;
    if (address >= 0 && (address + 1) < mem.size()) {
        dest = (mem[address]) | 
               (mem[address + 1] << 8);
        if (dest & 0x8000) { 
            dest |= 0xFFFF0000; 
        }
    } else {
        cerr << "Memory access error at address: " << address << endl;
    }
}

void lbu(int &dest, int &base, int offset, vector<uint8_t> &mem) {
    int address = base + offset - MEMORY_START_ADDRESS;
    if (address >= 0 && address < mem.size()) {
        dest = mem[address];
    } else {
        cerr << "Memory access error at address: " << address << endl;
    }
}

void lb(int &dest, int &base, int offset, vector<uint8_t> &mem) {
    int address = base + offset - MEMORY_START_ADDRESS;
    if (address >= 0 && address < mem.size()) {
        dest = mem[address];
        if (dest & 0x80) { 
            dest |= 0xFFFFFF00; 
        }
    } else {
        cerr << "Memory access error at address: " << address << endl;
    }
}


void lui(int& dest , int imm){
    dest = imm << 12;
}

void jal(string label, unordered_map<string,int> labels , int& pc, int& dest){
    dest = pc + 1;
    pc = labels[label];
}

void jalr(int& dest , int imm , int& src, int& pc){
    dest = pc;
    pc = src + imm - 1;
}

void beq(string label, unordered_map<string,int> labels, int& pc, int& source1, int& source2){
    if (source1 == source2) pc = labels[label];
}

void bne(string label, unordered_map<string,int> labels, int& pc, int& source1, int& source2){
    if (source1 != source2) pc = labels[label];
}

void blt(string label, unordered_map<string,int> labels, int& pc, int& source1, int& source2){
    if (source1 < source2) pc = labels[label];
}

void bge(string label, unordered_map<string,int> labels, int& pc, int& source1, int& source2){
    if (source1 >= source2) pc = labels[label];
}

void bltu(string label, unordered_map<string,int> labels, int& pc, int& source1, int& source2){
    if (unsigned(source1) < unsigned(source2)) pc = labels[label];
}

void bgeu(string label, unordered_map<string,int> labels, int& pc, int& source1, int& source2){
    if (unsigned(source1) >= unsigned(source2)) pc = labels[label];
}

void slt(int &dest , int &s1, int &s2){
    dest = s1 < s2;
}

void sltu(int &dest , int &s1, int &s2){
    dest = unsigned(s1) < unsigned(s2);
}

void slti(int &dest , int &s1, int imm){
    dest = s1 < imm;
}

void sltiu(int &dest , int &s1, int imm){
    dest = unsigned(s1) < unsigned(imm);
}

void sll(int &dest, int &s1, int &s2){
    dest = s1 << (s2 & 0x1F);
}

void slli(int &dest, int &s1, int imm){
    dest = s1 << (imm & 0x1F);
}

void srl(int &dest, int &s1, int &s2){
    dest = unsigned(s1) >> (s2 & 0x1F);
}

void srli(int &dest, int &s1, int imm){
    dest = unsigned(s1) >> (imm & 0x1F);
}

void sra(int &dest, int &s1, int &s2){
    dest = s1 >> (s2 & 0x1F);
}

void srai(int &dest, int &s1, int imm){
    dest = s1 >> (imm & 0x1F);
}
void mul(int &dest, int &s1, int &s2){
    dest = s1 * s2;
}
void div(int &dest, int &s1, int &s2){
    dest = s1 / s2;
}
void sb(vector<uint8_t> &mem, int &s1, int offset, int &src) {
    int address = s1 + offset - MEMORY_START_ADDRESS;
    if (address >= 0 && address < mem.size()) {
        mem[address] = src & 0xFF;
    }
}

void sh(vector<uint8_t> &mem, int &s1, int offset, int &src) {
    int address = s1 + offset - MEMORY_START_ADDRESS;
    if (address >= 0 && address < mem.size()){
        mem[address] = src & 0xFF;
        mem[address + 1] = (src >> 8) & 0xFF;
    }
}

void sw(vector<uint8_t> &mem, int &s1, int offset, int &src) {
    int address = s1 + offset - MEMORY_START_ADDRESS; 
    if (address >= 0 && (address + 3) < mem.size()) { 
        mem[address] = src & 0xFF;
        mem[address + 1] = (src >> 8) & 0xFF;
        mem[address + 2] = (src >> 16) & 0xFF;
        mem[address + 3] = (src >> 24) & 0xFF;
    } else {
        cerr << "Memory access error at address: " << address << endl;
    }
}

void get_instructions(vector<pair<string, string>> &instructions){
    ifstream program("instructions.txt");
    string curr_line;
    while (!program.eof()) {
        getline(program, curr_line);
        transform(curr_line.begin(), curr_line.end(), curr_line.begin(), ::tolower);
        if (curr_line.empty() || curr_line[0] == '#') continue;
        stringstream str(curr_line);
        pair<string, string> line;
        getline(str, line.first, ' ');
        getline(str, line.second);
        instructions.push_back(line);
    }
}

string get_format(string operation) {
    unordered_map<string, string> instruction_map;
    instruction_map["add"] = "R";
    instruction_map["sub"] = "R";
    instruction_map["and"] = "R";
    instruction_map["or"] = "R";
    instruction_map["xor"] = "R";
    instruction_map["sll"] = "R";
    instruction_map["srl"] = "R";
    instruction_map["slt"] = "R";
    instruction_map["sltu"] = "R";
    instruction_map["sra"] = "R";
    instruction_map["mul"] = "R";
    instruction_map["div"] = "R";
    instruction_map["rem"] = "R";
    instruction_map["addi"] = "I";
    instruction_map["subi"] = "I";
    instruction_map["andi"] = "I";
    instruction_map["ori"] = "I";
    instruction_map["xori"] = "I";
    instruction_map["lw"] = "I";
    instruction_map["beq"] = "I";
    instruction_map["bne"] = "I";
    instruction_map["blt"] = "I";
    instruction_map["bge"] = "I";
    instruction_map["bltu"] = "I";
    instruction_map["bgeu"] = "I";
    instruction_map["jalr"] = "I";
    instruction_map["lbu"] = "I";
    instruction_map["lb"] = "I";
    instruction_map["lhu"] = "I";
    instruction_map["lh"] = "I";
    instruction_map["slti"] = "I";
    instruction_map["sltiu"] = "I";
    instruction_map["jal"] = "U";
    instruction_map["lui"] = "U";
    instruction_map["sb"] = "S";
    instruction_map["sh"] = "S";
    instruction_map["sw"] = "S";
    instruction_map["slli"] = "I";
    instruction_map["srli"] = "I";
    instruction_map["srai"] = "I";
    return instruction_map[operation];
}

void perform_instruction(pair<string, string> instruction, vector<int> &reg, vector<uint8_t>& mem, unordered_map<string, int> &regtoindex, unordered_map<string, int> labels, int& pc) {
    string format = get_format(instruction.first);
    string dest;
    string source1;
    string source2;
    string imm;
    stringstream str(instruction.second);
    if (format == "R") {
        getline(str, dest, ',');
        getline(str, source1, ',');
        getline(str, source2);
        if (instruction.first == "add") {
            add(reg[regtoindex[dest]], reg[regtoindex[source1]], reg[regtoindex[source2]]);
        } else if (instruction.first == "slt") {
            slt(reg[regtoindex[dest]], reg[regtoindex[source1]], reg[regtoindex[source2]]);
        } else if (instruction.first == "sltu") {
            sltu(reg[regtoindex[dest]], reg[regtoindex[source1]], reg[regtoindex[source2]]);
        } else if (instruction.first == "and") {
            and_op(reg[regtoindex[dest]], reg[regtoindex[source1]], reg[regtoindex[source2]]);
        } else if (instruction.first == "or") {
            or_op(reg[regtoindex[dest]], reg[regtoindex[source1]], reg[regtoindex[source2]]);
        } else if (instruction.first == "xor") {
            xor_op(reg[regtoindex[dest]], reg[regtoindex[source1]], reg[regtoindex[source2]]);
        } else if (instruction.first == "sub") {
            sub(reg[regtoindex[dest]], reg[regtoindex[source1]], reg[regtoindex[source2]]);
        } else if (instruction.first == "sll") {
            sll(reg[regtoindex[dest]], reg[regtoindex[source1]], reg[regtoindex[source2]]);
        } else if (instruction.first == "srl") {
            srl(reg[regtoindex[dest]], reg[regtoindex[source1]], reg[regtoindex[source2]]);
        } else if (instruction.first == "sra") {
            sra(reg[regtoindex[dest]], reg[regtoindex[source1]], reg[regtoindex[source2]]);
        }else if(instruction.first == "mul"){
            mul(reg[regtoindex[dest]], reg[regtoindex[source1]], reg[regtoindex[source2]]);
        }else if(instruction.first == "div"){    
            div(reg[regtoindex[dest]], reg[regtoindex[source1]], reg[regtoindex[source2]]); 
        }
    } else if (format == "I") {
        if (instruction.first == "addi") { 
            getline(str, dest, ',');
            getline(str, source1, ',');
            getline(str, imm);
            addi(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm));
        } else if (instruction.first == "subi") {
            getline(str, dest, ',');
            getline(str, source1, ',');
            getline(str, imm);
            subi(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm));
        } else if (instruction.first == "andi") {
            getline(str, dest, ',');
            getline(str, source1, ',');
            getline(str, imm);
            andi(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm));
        } else if (instruction.first == "ori") {
            getline(str, dest, ',');
            getline(str, source1, ',');
            getline(str, imm);
            ori(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm));
        } else if (instruction.first == "xori") {
            getline(str, dest, ',');
            getline(str, source1, ',');
            getline(str, imm);
            xori(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm));
        } else if (instruction.first == "beq") {
            getline(str, source1, ',');
            getline(str, source2, ',');
            getline(str, imm);
            beq(imm, labels, pc, reg[regtoindex[source1]], reg[regtoindex[source2]]);
        } else if (instruction.first == "bne") {
            getline(str, source1, ',');
            getline(str, source2, ',');
            getline(str, imm);
            bne(imm, labels, pc, reg[regtoindex[source1]], reg[regtoindex[source2]]);
        } else if (instruction.first == "blt") {
            getline(str, source1, ',');
            getline(str, source2, ',');
            getline(str, imm);
            blt(imm, labels, pc, reg[regtoindex[source1]], reg[regtoindex[source2]]);
        } else if (instruction.first == "bge") {
            getline(str, source1, ',');
            getline(str, source2, ',');
            getline(str, imm);
            bge(imm, labels, pc, reg[regtoindex[source1]], reg[regtoindex[source2]]);
        } else if (instruction.first == "bltu") {
            getline(str, source1, ',');
            getline(str, source2, ',');
            getline(str, imm);
            bltu(imm, labels, pc, reg[regtoindex[source1]], reg[regtoindex[source2]]);
        } else if (instruction.first == "bgeu") {
            getline(str, source1, ',');
            getline(str, source2, ',');
            getline(str, imm);
            bgeu(imm, labels, pc, reg[regtoindex[source1]], reg[regtoindex[source2]]);
        } else if (instruction.first == "slti") {
            getline(str, dest, ',');
            getline(str, source1, ',');
            getline(str, imm);
            slti(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm));
        } else if (instruction.first == "sltiu") {
            getline(str, dest, ',');
            getline(str, source1, ',');
            getline(str, imm);
            sltiu(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm));
        } else if (instruction.first == "lw") {
            getline(str, dest, ',');
            getline(str, imm, '(');
            getline(str, source1, ')');
            lw(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm), mem);
        } else if (instruction.first == "lhu") {
            getline(str, dest, ',');
            getline(str, imm, '(');
            getline(str, source1, ')');
            lhu(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm), mem);
        } else if (instruction.first == "lh") {
            getline(str, dest, ',');
            getline(str, imm, '(');
            getline(str, source1, ')');
            lh(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm), mem);
        } else if (instruction.first == "lbu") {
            getline(str, dest, ',');
            getline(str, imm, '(');
            getline(str, source1, ')');
            lbu(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm), mem);
        } else if (instruction.first == "lb") {
            getline(str, dest, ',');
            getline(str, imm, '(');
            getline(str, source1, ')');
            lb(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm), mem);
        } else if (instruction.first == "slli") {
            getline(str, dest, ',');
            getline(str, source1, ',');
            getline(str, imm);
            slli(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm));
        } else if (instruction.first == "srli") {
            getline(str, dest, ',');
            getline(str, source1, ',');
            getline(str, imm);
            srli(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm));
        } else if (instruction.first == "srai") {
            getline(str, dest, ',');
            getline(str, source1, ',');
            getline(str, imm);
            srai(reg[regtoindex[dest]], reg[regtoindex[source1]], stoi(imm));
        }
    } else if (format == "U") {
        getline(str, dest, ',');
        getline(str, imm);
        if (instruction.first == "lui") lui(reg[regtoindex[dest]], stoi(imm));
        else if (instruction.first == "jal") jal(imm, labels, pc, reg[regtoindex[dest]]);
    } else if (format == "S") {
        if (instruction.first == "sb") {
            getline(str, dest, ',');
            getline(str, imm, '(');
            getline(str, source1, ')');
            sb(mem, reg[regtoindex[source1]], stoi(imm), reg[regtoindex[dest]]);
        } else if (instruction.first == "sh") {
            getline(str, dest, ',');
            getline(str, imm, '(');
            getline(str, source1, ')');
            sh(mem, reg[regtoindex[source1]], stoi(imm), reg[regtoindex[dest]]);
        } else if (instruction.first == "sw") {
            getline(str, dest, ',');
            getline(str, imm, '(');
            getline(str, source1, ')');
            sw(mem, reg[regtoindex[source1]], stoi(imm), reg[regtoindex[dest]]);
        }
    }

    if (dest == "x0") reg[0] = 0;
}

void map_reg(unordered_map<string, int>& regtoindex, vector<string> reg_name) {
    for (int i = 0; i < 32; i++) {
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

void assembler(vector<int>& reg, vector<uint8_t>& mem, vector<pair<string, string>> &instructions, vector<string>& reg_name) {
    unordered_map<string, int> regtoindex;
    unordered_map<string, int> labels;
    int pc = 0;
    map_reg(regtoindex, reg_name);
    get_instructions(instructions);
    for (int i = 0; i < instructions.size(); i++) {
        string label = instructions[i].first;
        if (label.back() == ':') {
            labels[label.substr(0, label.size() - 1)] = i;
        }
    }

    while (pc < instructions.size()) {
        pair<string, string> instruction = instructions[pc];
        if (instruction.first == "ebreak") return;
        perform_instruction(instruction, reg, mem, regtoindex, labels, pc);
        pc++;
    }
}

void print_reg(const vector<int>& reg, const vector<string>& reg_name) {
    for (int i = 0; i < 32; i++) {
        cout << reg_name[i] << " : "
             << "Decimal: " << reg[i] << "     "
             << "Hex:" << hex << uppercase << reg[i] << dec << "     "
             << "Binary: " << bitset<32>(reg[i])
             << "\n";
    }
}


void print_mem(const vector<uint8_t> &mem, int start, int end) {
    cout << "Memory contents from " << start + MEMORY_START_ADDRESS << " to " << end + MEMORY_START_ADDRESS << ":\n";
    for (int i = start; i <= end; i += 4) {
        if (i + 3 < mem.size()) {
            int value = (mem[i]) |
                        (mem[i + 1] << 8) |
                        (mem[i + 2] << 16) |
                        (mem[i + 3] << 24);

            cout << "mem[" << setw(3) << i + MEMORY_START_ADDRESS << "] : "
                 << "Decimal: " << value << "     "
                 << "Hex: 0x" << hex << uppercase << setw(8) << setfill('0') << value << dec << "     "
                 << "Binary: " << bitset<32>(value)
                 << "\n";
        } else {
            cerr << "Memory access error at address: " << i + MEMORY_START_ADDRESS << endl;
            break;
        }
    }
}

int main() {
    vector<int> reg(32);
    vector<string> reg_name(32);
    for (int i = 0; i < 32; i++) {
        reg_name[i] = "x" + to_string(i);
    }
    vector<uint8_t> mem(1024);
    vector<pair<string, string>> instructions;
    assembler(reg, mem, instructions, reg_name);
    print_reg(reg, reg_name);
    print_mem(mem, 0, 256);
}
