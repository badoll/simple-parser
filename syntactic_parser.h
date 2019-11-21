#ifndef SYNTACTIC_PARSER_H
#define SYNTACTIC_PARSER_H
#include "lexical_parser.h"
#include "word.h"
#include <tuple>
class syntactic_parser: public lexical_parser
{
public:
    syntactic_parser() = default;
    void synparse(string filepath);
    void analyse();
    bool next_word();
    bool next_line();
    void procedure();
    void statement();
    void statement_str();
    bool expression();
    bool term();
    bool factor();
    bool error_discover(string identifier);
    string get_varname();
    void print_expre();
    bool has_expre();
private:
    word word_t;
    string var;
    vector<string> datas;
    vector<tuple<string, string, string, string> > semantic_tetrad;
    //tuple<resule,ag1,op,ag2>
};

#endif
