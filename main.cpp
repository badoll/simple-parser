#include "lexical_parser.h"
#include "syntactic_parser.h"

int main(int ac, char** av)
{
    // lexical_parser lp;
    // lp.lexparse("data.txt");
    // lp.print_words();
    syntactic_parser sp;
    sp.start(*(av+1));
    sp.print_words();
    cout << endl;
    if (!sp.has_error()) {
        cout << "success" << endl;
    } else {
        sp.print_errors();
    }
    cout << endl;
    if (sp.has_icode()) sp.print_icode();
}