#include "word.h"

word::word(string w):
legal(true)
{
    name = w;
    if (is_keyword(w)) {
        type_num = category[name];
        type_id = KEYWORD;
    } else if (is_operator(w)) {
        type_num = category[name];
        type_id = OPERATOR;
    } else if (is_variable(w)) {
        type_num = category["variable"];
        type_id = VARIABLE;
    } else if (is_digit(w)){
        type_num = category["digit"];
        type_id = DIGIT;
    } else if(w == "") {

    } else {
        legal = false;
    }
}

bool
word::is_legal() const
{
    return legal;
}

bool
word::is_annot() const
{
    if (name == annot_sym) return true;
    return false;
}

bool
word::is_conditopt() const
{
    if (type_id != OPERATOR) return false;
    for (const auto& opt : condition_opt) {
        if (name == opt) return true;
    }
    return true;
}

bool
word::is_conditkw() const
{
    if (type_id != KEYWORD) return false;
    for (const auto& kw : condition_kw) {
        if (name == kw) return true;
    }
    return true;
}

void
word::print() const
{
    cout << "(" << type_num << "," << name << ")" << " ";
}

bool
is_keyword(string word)
{
    vector<string>::iterator vit;
    for (vit = keyword.begin(); vit != keyword.end(); ++vit) {
        if (word == *vit) {
            return true;
        }
    }
    return false;
}

bool
is_letter(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >='A' && c <= 'Z')) return true;
    return false;
}

bool
is_num(char c)
{
    if (c >= '0' && c <= '9') return true;
    return false;
}

bool
is_digit(string word)
{
    for (int i = 0; i < word.size(); ++i) {
        if (!is_num(word[i])) return false;
    }
    return true;
}

bool
is_variable(string word)
{
    if (!is_letter(word[0])) return false;
    for (int i = 0; i < word.size(); ++i) {
        if (!is_letter(word[i]) && !is_num(word[i])) return false;
    }
    return true;
}

bool
is_operator(string word)
{
    vector<string>::iterator vit;
    for (vit = opt.begin(); vit != opt.end(); ++vit) {
        if (word == *vit) {
            return true;
        }
    }
    return false;
}
