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
    string generateReturn(ASTNode * STATEMENT){
        if(!STATEMENT ->CHILD->VALUE){
            cout<<"!!!Generation Error- return has no int\n";
            exit(1);
        }
        stringstream codeBuffer;
        codeBuffer<<"mov rax ,60\n";
        codeBuffer<<"mov rdi, "+ *STATEMENT->CHILD->VALUE<<"\n";
        codeBuffer<<"syscall\n";
        return codeBuffer.str();
    
    
    }

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
    void generate(){
        stringcounter=0;

        //
        sectionData << "section .data\n";
        sectionText << "section .text\nglobal _start\n_start:\n";


        for(ASTNode * current : AST_ROOT->SUB_STATEMENTS){
            switch(current->TYPE){
                case NODE_RETURN:{
                    sectionText<< generateReturn(current);
                    break;
                }
                case NODE_PRINT:{
                    generatePrint(current);
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

};

#endif
