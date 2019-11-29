#ifndef SYNTACTIC_PARSER_H
#define SYNTACTIC_PARSER_H
#include "lexical_parser.h"
#include "word.h"
#include <tuple>
class syntactic_parser: public lexical_parser
{
public:
    syntactic_parser() = default;
    void start(string filepath);
    void analyse();
    bool next_word();
    bool next_line();
    void procedure();
    void statement();
    void code_block();
    void statement_str();
    bool expression();
    bool term();
    bool factor();
    void condition();
    bool error_discover(string identifier);
    string get_varname();
    void print_icode();
    bool has_icode();
private:
    word word_t;
    string var;
    vector<string> datas;
    vector<tuple<string, string, string, string, bool> > icode_quin;

    //tuple<resule,ag1,op,ag2>
};

#endif
