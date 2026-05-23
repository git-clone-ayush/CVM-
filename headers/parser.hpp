
#ifndef __PARSER_H_
#define __PARSER_H_
#include "lexer.hpp"
#include <bits/stdc++.h>
using namespace std;

enum NodeType{

    NODE_ROOT,
    NODE_VARIABLE,
    NODE_RETURN,
    NODE_PRINT,
    NODE_INT,
    NODE_STRING,

    NODE_BINARY_EXPR,
    NODE_IDENTIFIER,
};

string nodetoString(enum NodeType type){
    switch(type){
        case NODE_ROOT: return "NODE_ROOT";
        case NODE_VARIABLE: return "NODE_VARIABLE";
        case NODE_RETURN: return "NODE_RETURN";
        case NODE_PRINT: return "NODE_PRINT";
        case NODE_INT: return "NODE_INT";
        case NODE_STRING: return "NODE_STRING";

        case NODE_BINARY_EXPR: return "NODE_BINARY_EXPR";
        case NODE_IDENTIFIER: return "NODE_IDENTIFIER";

        default: return "UNRECOGNISED_NODE_TYPE";
    }
}


struct ASTNode{  //Abstract Syntax Tree Node
    enum NodeType TYPE;
    string * VALUE;
    ASTNode * CHILD;

    ASTNode * LEFT;
    ASTNode * RIGHT;  //for mathematical expressions;

    vector<ASTNode *> SUB_STATEMENTS;


};


class Parser{

    public:
    Parser(vector <Token *> tokens){
        parsertoken=tokens;
        index=0;
        limit=tokens.size();
        current=parsertoken.at(index);

    }

    Token * proceed(enum type TYPE){  //to check if current token is of expected type, if not throw error 
        if(current->TYPE != TYPE){
            cout<<"[!] Syntax Error: Made a mistake Padwan\n";
            exit(1);
        }
        index++;
        current=parsertoken.at(index);
        return current;
    }

    ASTNode * parseINT(){
        if(current ->TYPE != TOKEN_INT){
            cout<<"[!] Syntax Error: Made a mistake Padwan\n";
            exit(1);
        }

         ASTNode * newNode= new ASTNode();
            newNode->TYPE=NODE_INT;
            newNode ->VALUE= &current->VALUE;

            proceed(TOKEN_INT);

            return newNode;
        }

    ASTNode * parseID(){
        string * buffer= &current->VALUE;
        proceed(TOKEN_ID);
        proceed(TOKEN_EQUALS);

            ASTNode * newNode= new ASTNode();
            newNode->TYPE=NODE_VARIABLE;
            newNode->VALUE = buffer;
            newNode ->CHILD= parseExpression();

            return newNode;
            
    
    }

    ASTNode * parseRETURN(){
        proceed(TOKEN_KEYWORD);

        ASTNode * newNode= new ASTNode();
        newNode->TYPE=NODE_RETURN;
        newNode->CHILD = parseExpression();
        return newNode;
    }


    ASTNode * parsePRINT(){
        proceed(TOKEN_KEYWORD);

        ASTNode * newNode= new ASTNode();
        newNode->TYPE=NODE_PRINT;
        proceed(TOKEN_LEFT_PAREN);
        proceed(TOKEN_QUOTES);
        newNode->CHILD=parseSTRING();
        proceed(TOKEN_QUOTES);
        proceed(TOKEN_RIGHT_PAREN);

        return newNode;
    }



    ASTNode * parseKEYWORD(){
        if(current->VALUE=="return"){
            return parseRETURN();
        }
        else if(current->VALUE=="print"){
            return parsePRINT();
        }
        else{
            cout<<"[!] Syntax Error: Made a mistake Padwan\n";
            exit(1);
        }
    
    
    }

    ASTNode * parseSTRING(){
        if(current->TYPE != TOKEN_STRING){
            cout<<"[!] Syntax Error: Made a mistake Padwan\n";
            exit(1);
        }

         ASTNode * newNode= new ASTNode();
            newNode->TYPE=NODE_STRING;
            newNode ->VALUE= &current->VALUE;

            proceed(TOKEN_STRING);

            return newNode;
    }

    ASTNode * parseIdentifier(){ //
        if (current->TYPE != TOKEN_ID){
            cout<<"[!] Syntax Error: Made a mistake Padwan\n";
            exit(1);
        }

         ASTNode * newNode= new ASTNode();
            newNode->TYPE=NODE_IDENTIFIER;
            newNode ->VALUE= &current->VALUE;

            proceed(TOKEN_ID);

            return newNode;
    }

 ASTNode * parseFactor(){
    if(current->TYPE == TOKEN_INT){
        return parseINT();
    }
    else if(current->TYPE == TOKEN_ID){
        return parseIdentifier();
    }
    else if(current->TYPE == TOKEN_LEFT_PAREN){
        proceed(TOKEN_LEFT_PAREN);
        ASTNode * expr = parseExpression();
        proceed(TOKEN_RIGHT_PAREN);
        return expr;
    }
    else{
        cout<<"[!] Syntax Error: Made a mistake Padwan\n";
        exit(1);
    }
}


    ASTNode * parseTerm(){
         ASTNode * left = parseFactor();

    while (current->TYPE == TOKEN_MULTIPLY || current->TYPE == TOKEN_DIVIDE) {
        string * op = &current->VALUE;

        if (current->TYPE == TOKEN_MULTIPLY) {
            proceed(TOKEN_MULTIPLY);
        } else {
            proceed(TOKEN_DIVIDE);
        }

        ASTNode * right = parseFactor();

        ASTNode * newNode = new ASTNode();
        newNode->TYPE = NODE_BINARY_EXPR;
        newNode->VALUE = op;
        newNode->LEFT = left;
        newNode->RIGHT = right;

        left = newNode;
    }

    return left;
    }

    ASTNode * parseExpression(){
        ASTNode * left = parseTerm();

    while (current->TYPE == TOKEN_PLUS || current->TYPE == TOKEN_MINUS) {
        string * op = &current->VALUE;

        if (current->TYPE == TOKEN_PLUS) {
            proceed(TOKEN_PLUS);
        } else {
            proceed(TOKEN_MINUS);
        }

        ASTNode * right = parseTerm();

        ASTNode * newNode = new ASTNode();
        newNode->TYPE = NODE_BINARY_EXPR;
        newNode->VALUE = op;
        newNode->LEFT = left;
        newNode->RIGHT = right;

        left = newNode;
    }

    return left;
    
    
    }


    ASTNode * parse(){
        ASTNode * root= new ASTNode();
        root->TYPE=NODE_ROOT;

        while(current->TYPE != TOKEN_EOF){

            switch(current->TYPE){
                case TOKEN_ID: {root -> SUB_STATEMENTS.push_back(parseID());
                    break;
                }

                case TOKEN_KEYWORD :{
                    root ->SUB_STATEMENTS.push_back(parseKEYWORD());
                    break;
                }
                default:{
                    cout<<"[!] Syntax Error: Made a mistake Padwan\n";
                    exit(1);
                }
            }
            proceed(TOKEN_SEMICOLON); //All statement should end with semicolon, if want without it then remove , I prefer it


        }


        return root;

    }

    private:
    int limit;
    int index;
    Token * current;
    vector<Token *> parsertoken;

};











#endif
