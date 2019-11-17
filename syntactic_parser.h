#ifndef SYNTACTIC_PARSER_H
#define SYNTACTIC_PARSER_H
#include "lexical_parser.h"
#include "word.h"
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
    void expression();
    void term();
    void factor();
    bool error_discover(string identifier);
private:
    word word_t;
    vector<string> datas;
};

#endif
