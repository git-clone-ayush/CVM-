#ifndef __LEXER_HPP__
#define __LEXER_HPP__
#include <bits/stdc++.h>
using namespace std; 


enum type{
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_EQUALS,
    TOKEN_SEMICOLON,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_KEYWORD,
    TOKEN_EOF,
    TOKEN_STRING,
    TOKEN_QUOTES,

    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,

};
struct Token{
    enum type TYPE;
    string VALUE;
};


class Lexer{            //class are backbone for OOPS, since learning oops, think of class as blueprint where we can define data members and member functions
public:  //public accessssible everywhere
    Lexer(string sourceCode){
        source=sourceCode;
        cursor=0;
        size= sourceCode.length();
        current=sourceCode.at(cursor);
        linenumber=1;
        characternumber=1;



    }
    char advance(){  // returns the current char and moves the cursor ahead
        if(cursor<size){
            char temp=current;
            cursor++;
            characternumber++;
            current=(cursor<size)?source.at(cursor):'\0';
            return temp;
        }
        else{
            return '\0';
        }


    }


    char peek(int offset=0){  //helsp to look ahead without moving the cursor, needed for recognising tokens like ==
        if(cursor+offset<size){
            return source.at(cursor+offset);
        }
        else{
            return '\0';
        }
    }
    void checkandskip(){
        while(current==' ' || current=='\n' || current=='\t' || current=='\r'){
            if(current=='\n'){
                linenumber++;
                characternumber=0;
            }
           
            advance();
        }

    }


    vector<string> keywords={"return","print"}; //keeping simple for now will add more later

    Token *tokenizeID_KEY(){
        stringstream buffer;
        buffer<< advance();

        while(isalnum(current) || current=='_'){
            buffer<<advance();
        }

        Token *newToken=new Token();

    newToken->TYPE=(find(keywords.begin(),keywords.end(),buffer.str())!=keywords.end())?TOKEN_KEYWORD:TOKEN_ID; //if buffer string is keyword then type assigned is KEYWORD else ID
        newToken->VALUE=buffer.str();

        return newToken;

    }

    Token *tokenizeINT(){

        stringstream buffer;
        buffer<< advance();

        while(isdigit(current)){
            buffer<<advance();
        }

        Token *newToken=new Token();

        newToken->TYPE=TOKEN_INT;
        newToken->VALUE=buffer.str();

        return newToken;
    }

    Token *tokenizespecial(enum type TYPE){
        Token * newToken= new Token();
        newToken->TYPE=TYPE;
        newToken->VALUE= string(1, advance());

        return newToken;
    }

    Token *tokenizeSTRING(){
        stringstream buffer;
        while (current!='"' ){
            if(current== '\0'){
                cout<<"LEXER eroor missing quoates\n";
            }
            buffer<<advance();
        }
        
        Token *newToken=new Token();
        newToken->TYPE=TOKEN_STRING;
        newToken->VALUE=buffer.str();
        return newToken;
    }

    vector<Token *> tokenize(){
        vector<Token *> tokens;
        bool notEOF=true;
        while(cursor<size && notEOF){
            checkandskip();
            if(isalpha(current) || current=='_'){ //variable name start with letter and underscore only
                //logic for ids
                tokens.push_back(tokenizeID_KEY());
                continue;
            }

            if (isdigit(current)){
                //logic for int in yoda
                tokens.push_back(tokenizeINT());
                continue;
            }

            switch(current){
                case ';':
                    tokens.push_back(tokenizespecial(TOKEN_SEMICOLON));
                    break;

                case '=':
                    tokens.push_back(tokenizespecial(TOKEN_EQUALS));
                    break;

                case '(':
                    tokens.push_back(tokenizespecial(TOKEN_LEFT_PAREN));
                    break;
                case ')':
                    tokens.push_back(tokenizespecial(TOKEN_RIGHT_PAREN));
                    break;

                case '"':
                    {tokens.push_back(tokenizespecial(TOKEN_QUOTES));
                    tokens.push_back(tokenizeSTRING());
                    tokens.push_back(tokenizespecial(TOKEN_QUOTES));
                    break;}

                case '+':
                    tokens.push_back(tokenizespecial(TOKEN_PLUS));
                    break;
                case '-':
                    tokens.push_back(tokenizespecial(TOKEN_MINUS));
                    break;
                case '*':
                    tokens.push_back(tokenizespecial(TOKEN_MULTIPLY));
                    break;
                case '/':
                    tokens.push_back(tokenizespecial(TOKEN_DIVIDE));
                    break;

                case 0:
                {
                    tokens.push_back(tokenizespecial(TOKEN_EOF));
                    break;
                }    

                default:
                {
                    cout<<"Unidentified Character-LEXER Error\n";
                    cout<<"at line "<<linenumber<<" , character "<<characternumber<<"\n";
                    advance();
                    break;
                }
            }
        }


        return tokens;
    }
    private: //accessible only within the class
    string source;
    int cursor;
    int size;
    char current;
    int linenumber;
    int characternumber;


};

string typeToString(enum type t){
    switch(t){
        case TOKEN_ID:
            return "TOKEN_ID";
        case TOKEN_INT:
            return "TOKEN_INT";
        case TOKEN_EQUALS:
            return "TOKEN_EQUALS";
        case TOKEN_SEMICOLON:
            return "TOKEN_SEMICOLON";
        case TOKEN_LEFT_PAREN:
            return "TOKEN_LEFT_PAREN";
        case TOKEN_RIGHT_PAREN:
            return "TOKEN_RIGHT_PAREN";
        case TOKEN_KEYWORD:
            return "TOKEN_KEYWORD";
        case TOKEN_EOF:
            return "TOKEN_EOF";
        case TOKEN_STRING:
            return "TOKEN_STRING";
        case TOKEN_QUOTES:
            return "TOKEN_QUOTES";
        
        case TOKEN_PLUS:
            return "TOKEN_PLUS";
        case TOKEN_MINUS:
            return "TOKEN_MINUS";
        case TOKEN_MULTIPLY:    
            return "TOKEN_MULTIPLY";
        case TOKEN_DIVIDE:
            return "TOKEN_DIVIDE";

        default:
            return "UNKNOWN_TOKEN";
    }
}

#endif
