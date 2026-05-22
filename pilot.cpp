#include <bits/stdc++.h>

#include "headers/lexer.hpp"
#include "headers/parser.hpp"

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
    cout<<"source code is \n"<<sourceCode<<"\n"; 
    
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

    cout<<"THis is no. of statements\n"<< root ->SUB_STATEMENTS.size()<<"\n";



    

    cout<<"This is the end of pilot.cpp\n";

    
    return 0;
}