#ifndef WORD_H
#define WORD_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <map>

using namespace std;
#define ILLEGAL false

enum type{
    KEYWORD,
    OPERATOR,
    VARIABLE,
    DIGIT
};

class word
{
public:
    friend class syntactic_parser;
    friend class lexical_parser;
    word() = default;
    word(string);
    word(bool is_legal):legal(is_legal) {}
    bool is_legal() const;
    bool is_annot() const;
    bool is_conditopt() const;
    bool is_conditkw() const;
    void print() const;
private:
    string name;
    int type_num;
    int type_id;
    bool legal;
};

static map<string,int> category = {
    {"main",1},{"if",2},{"while",3},
    {"+",4},{"-",5},{"*",6},{"/",7},{"=",8},
    {"<",9},{"<=",10},{">",11},{">=",12},
    {"==",13},{"!=",14},{";",15},
    {"(",16},{")",17},{"{",18},{"}",19},{"//",20},
    {"variable",21},{"digit",22},
    {"#",0}
};
static vector<string> keyword = {
    "if","while","main"
};
static vector<string> opt = {
    "+","-","*","/","<","<=",">",
    ">=","==","!=","=",";",
    "(",")","{","}","//","#"
};

static vector<string> condition_opt = {
    "<","<=","==",">=",">","!="
};

static vector<string> condition_kw = {
    "if","while"
};

static string annot_sym = "//";

bool is_keyword(string word);
bool is_operator(string word);
bool is_variable(string word);
bool is_digit(string word);
bool is_letter(char c);
bool is_num(char c);

#endif