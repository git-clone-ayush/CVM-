#ifndef __GENERATOR_H
#define __GENERATOR_H
#include "lexer.hpp"
#include "parser.hpp"
#include <bits/stdc++.h>
using namespace std;

class Generator
{
    public:
    Generator (ASTNode * root, string filename){
        AST_ROOT = root;
        assemblyfile.open(filename + ".asm");
        
    }
    void writeSections(){

        assemblyfile << sectionData.str();
        assemblyfile << sectionText.str();
        assemblyfile.close();

    }
  void generateReturn(ASTNode * STATEMENT){
    generateExpression(STATEMENT->CHILD);
    sectionText << "mov rdi, rax\n";
    sectionText << "mov rax, 60\n";
    sectionText << "syscall\n";
}
//new generate function , writes directly sectionText



    int lookup(string * str){ //to check for same print statement
        int counter=0;
        for(string * temp : stringreferences){
            if(*temp == *str){
                return counter;
            }
           counter++; 
    }
    return -1;
}
    void sectionDataDefine(string * str, int refnum){
        sectionData<<"SRef"+ to_string(refnum);
        sectionData<<":\ndb \""<<*str + "\"\n";
        sectionData<<"SRef"+ to_string(refnum);
        sectionData<<"_L :equ $-SRef"+ to_string(refnum) ;
        sectionData<<"\n\n";
    }
    void generatePrint(ASTNode * STATEMENT){
        if(!STATEMENT ->CHILD->VALUE){
            cout<<"!!!Generation Error- print has no string linked\n";
            exit(1);
        }

        int refnum=lookup(STATEMENT->CHILD->VALUE);
        if(refnum==-1){
            refnum=stringcounter;
            sectionDataDefine(STATEMENT->CHILD->VALUE, refnum);
            stringreferences.push_back(STATEMENT->CHILD->VALUE);
            stringcounter++;

    }

        sectionText<<"mov rax, 1\n";
        sectionText<<"mov rdi, 1\n";
        sectionText<<"mov rsi, SRef"+to_string(refnum)+"\n";
        sectionText<<"mov rdx, SRef"+to_string(refnum)+"_L\n";
        sectionText<<"syscall\n";
}




    string getVariableLabel(string name) {
    if (variables.find(name) == variables.end()) {
        string label = "Var" + to_string(variablecounter++);
        variables[name] = label;
        sectionData << label << ": dq 0\n";
    }

    return variables[name];
}

void generateVariable(ASTNode * STATEMENT) {
    string label = getVariableLabel(*STATEMENT->VALUE);

    generateExpression(STATEMENT->CHILD);

    sectionText << "mov [" << label << "], rax\n";
}


void generateExpression(ASTNode * node) {
    switch(node->TYPE) {
        case NODE_INT: {
            sectionText << "mov rax, " << *node->VALUE << "\n";
            break;
        }

        case NODE_IDENTIFIER: {
            string label = getVariableLabel(*node->VALUE);
            sectionText << "mov rax, [" << label << "]\n";
            break;
        }

        case NODE_BINARY_EXPR: {
            generateExpression(node->LEFT);
            sectionText << "push rax\n";

            generateExpression(node->RIGHT);
            sectionText << "mov rbx, rax\n";

            sectionText << "pop rax\n";

            if (*node->VALUE == "+") {
                sectionText << "add rax, rbx\n";
            }
            else if (*node->VALUE == "-") {
                sectionText << "sub rax, rbx\n";
            }
            else if (*node->VALUE == "*") {
                sectionText << "imul rax, rbx\n";
            }
            else if (*node->VALUE == "/") {
                sectionText << "cqo\n";
                sectionText << "idiv rbx\n";
            }
            else {
                cout << "Unknown binary operator: " << *node->VALUE << "\n";
                exit(1);
            }

            break;
        }

        default:
            cout << "Cannot generate expression for node: " << nodetoString(node->TYPE) << "\n";
            exit(1);
    }
}

    void generate(){
        stringcounter=0;
        variablecounter = 0;

        //
        sectionData << "section .data\n";
        sectionText << "section .text\nglobal _start\n_start:\n";


        for(ASTNode * current : AST_ROOT->SUB_STATEMENTS){
            switch(current->TYPE){
                case NODE_RETURN:{
                    generateReturn(current);
                    break;
                }
                case NODE_PRINT:{
                    generatePrint(current);
                    break;
                }

                case NODE_VARIABLE:{
                    generateVariable(current);
                    break;
            }

                default:
                {
                    cout<<"UNRECOGNISED NODE: "<<nodetoString(current->TYPE);
                    break;
                }
            }
        }


        writeSections();

    }
    private:
    stringstream sectionData;
    stringstream sectionText;
    ASTNode * AST_ROOT;
    ofstream assemblyfile;
    int stringcounter;
    vector<string *> stringreferences;


    map<string, string> variables; //for variable assignment , stores name and assembly label, since assembly can't use a and b, it converts into Var0 and var1;
    int variablecounter; 


};

#endif
