#ifndef LEXICAL_PARSER_H
#define LEXICAL_PARSER_H
#include "word.h"
#include <fstream>
using namespace std;

class lexical_parser
{    
public:
    lexical_parser():linecount(0),annot(false){}
    void lexparse(string filepath);
    void analyse(string str);
    bool next_line(string line);
    word scan();
    word divide_symbol();
    void print_words() const;
    vector<word> get_words() const;
    bool has_error() const;
    void print_errors() const;
    void add_error(string err_msg);
protected:
    vector<word> words;
    string data;
    string::iterator sit_line;
    int linecount;
    vector<string> err_msgs;
    bool annot;
};

#endif