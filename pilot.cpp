#include <bits/stdc++.h>

#include "headers/lexer.hpp"
#include "headers/parser.hpp"
#include "headers/generator.hpp"

using namespace std;
int main(int argc, char** argv) {
    if(argc<2)
    {
        cout<<"supply source file\n";
        exit(1);
    }
    //example.yd
    cout<<"reading from source file "<<argv[1]<<"\n";
    ifstream sourceFileStream(argv[1]);

    stringstream buffer;

    char temp;
    while(sourceFileStream.get(temp))
    {
        buffer<<temp;
    }

    string sourceCode = buffer.str();
    //cout<<"source code is \n"<<sourceCode<<"\n"; 
    //sourceCode.append("\0"); //uncomment to print source code as well 
    
    Lexer lexer(sourceCode);

    
    vector<Token *> tokens=lexer.tokenize();
    
    int counter=0;
    if(tokens.back()->TYPE!=TOKEN_EOF){
        Token * EOFnode =new Token();
        EOFnode ->TYPE= TOKEN_EOF;
        tokens.push_back(EOFnode);
    }

    for (Token *temp :tokens){
        counter++;
        cout<<"Token "<<counter<<" : Type = "<<typeToString(temp->TYPE)<<" , Value = "<<temp->VALUE<<"\n";
    }

    Parser parser(tokens);

    ASTNode * root = parser.parse();

    //cout<<"THis is no. of statements\n"<< root ->SUB_STATEMENTS.size()<<"\n"; //don't need it now

    Generator generator(root, argv[1]);

    generator.generate();



string filenamewithoutExtension= argv[1];
filenamewithoutExtension.pop_back(); //removing m from .yd
filenamewithoutExtension.pop_back(); //removing d from .yd
filenamewithoutExtension.pop_back(); //removing . from .yd


    stringstream assemblerInstruction;
    assemblerInstruction<<"nasm -f elf64 "<<filenamewithoutExtension<<".yd.asm -o "<<filenamewithoutExtension<<".o";
    system(assemblerInstruction.str().c_str());

    stringstream linkerInstruction;
    linkerInstruction<<"ld -o "<<filenamewithoutExtension<<" "<<filenamewithoutExtension<<".o";
    system(linkerInstruction.str().c_str());

    cout<<"This is the end \n";

    
    return 0;
}
