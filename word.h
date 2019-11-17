#ifndef WORD_H
#define WORD_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <map>

using namespace std;

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
    void print() const;
private:
    string name;
    int type_num;
    int type_id;
    bool legal;
};

static map<string,int> category = {
    {"begin",1},{"if",2},{"then",3},{"while",4},
    {"do",5},{"end",6},{"variable",10},{"digit",11},
    {"+",13},{"-",14},{"*",15},{"/",16},{":=",17},
    {"<",20},{"<>",21},{"<=",22},{">",23},{">=",24},
    {"==",25},{"!=",26},{";",27},
    {"(",28},{")",29},{"//",30},{"#",0}
};
static vector<string> keyword = {
    "begin","if","then","while","do","end"  
};
static vector<string> opt = {
    "+","-","*","/","<","<=",">",
    ">=","==","<>","!=",":=",";",
    "(",")","//","#"
};

static string annot_sym = "//";

#endif