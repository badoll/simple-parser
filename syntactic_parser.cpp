#include "syntactic_parser.h"

void
syntactic_parser::synparse(string path)
{
    ifstream file(path);
    if (!file.is_open()) {
        return;
    }
    string line;
    while(getline(file,line)) {
        datas.push_back(line);
    }
    if (!next_line()) {// 添加第一行待分析代码
        return;
    }
    analyse();
}


void
syntactic_parser::analyse()
{
    if (!next_word()) return;
    procedure();
}

void
syntactic_parser::procedure()
{
    if (word_t.name == "begin") {
        if (!next_word()) return;
    } else {
        string err("lack of 'begin'");
        add_error(err);
    }
    statement_str();
    if (word_t.name != "end") {
        string err("lack of 'end'");
        add_error(err);
    }
}

void
syntactic_parser::statement_str()
{
    while (true) {
        statement();
        if (word_t.name == "end") break;
        if (word_t.name == "" && word_t.is_legal()) break; //没有词语了
    }
}

void
syntactic_parser::statement()
{
    while (word_t.type_id != VARIABLE) {
        string err = "error symbol " + word_t.name;
        add_error(err);
        if (!next_word()) return;
        if (word_t.name == "end") return;
    }
    string result = word_t.name;
    string op = "";
    if (!next_word()) return;
    //检查是否有赋值号
    if (error_discover(":=")) {
    //如果没有赋值号则跳过开始分析下一个语句
        return;
    }
    if (expression()) {
        string ag1 = var;
        string ag2 = "";
        semantic_tetrad.push_back(make_tuple(result,ag1,op,ag2));
    }
    //检查是否有分号
    error_discover(";");

}

bool
syntactic_parser::expression()
{
    if (!term()) return false;
    while (word_t.name == "+" || word_t.name == "-") {
        string ag1 = var;
        string op = word_t.name;
        if (!next_word()) return false;
        if (!term()) return false;
        string ag2 = var;
        string result = get_varname();
        semantic_tetrad.push_back(make_tuple(result,ag1,op,ag2));
        var = result;
    }
    return true;
}

bool
syntactic_parser::term()
{
    if (!factor()) return false;
    while (word_t.name == "*" || word_t.name == "/") {
        string ag1 = var;
        string op = word_t.name;
        if (!next_word()) return false;
        if (!factor()) return false;
        string ag2 = var;
        string result = get_varname();
        semantic_tetrad.push_back(make_tuple(result,ag1,op,ag2));
        var = result;
    }
    return true;
}

bool
syntactic_parser::factor()
{
    if (word_t.name == ")") {
        string err = "lack of factor";
        add_error(err);    
        return false;
    }
    if (word_t.name == "(") {
        if (!next_word()) return false;
        if (!expression()) return false;
        if (word_t.name != ")") {
            string err = "lack of ')'";
            add_error(err);    
            return false;
        }
    } else if (word_t.type_id == KEYWORD || 
        (word_t.type_id != VARIABLE && word_t.type_id != DIGIT)) {
        if (word_t.name == ";") {
            string err = "lack of factor";
            add_error(err);
        } else {
            string err = word_t.name + " can't be factor";
            add_error(err);
        }
        next_word();
        return false;
    }
    if (word_t.name != ")") var = word_t.name;
    next_word();
    return true;
}

bool
syntactic_parser::next_word()
{
    word_t = scan();
    while ((word_t.name == ""&&word_t.is_legal()) || word_t.is_annot()) { //loop to scan
        if (linecount == datas.size()) {
            string err = "statement is not completed";
            add_error(err);
            word_t = word("");
            return false;
        }
        if (!next_line()) {
            word_t = word("");
            return false;
        }
        word_t = scan();
    }
    return true;
}

bool
syntactic_parser::next_line()
{
    while (!lexical_parser::next_line(datas[linecount])) {
        if (linecount == datas.size()) {
            return false;
        }
    }
    return true;
}

bool
syntactic_parser::error_discover(string identifier)
{
    if (word_t.name != identifier) {
        string err_word;
        while (word_t.type_id != VARIABLE && word_t.name != ";"&&word_t.name != "end") {
        //跳过一些非变量符号之后如果有赋值号则继续分析该语句
            err_word += word_t.name;
            if (!next_word()) return true;
            if (word_t.name == identifier) {
                string err = "error symbol " + err_word;
                add_error(err);
                next_word();
                return false; 
            }
        }
        string err = "lack of '" + identifier + "'";
        add_error(err);
        if (word_t.name == ";") {
            next_word();
        }
        return true;
    } else {
        next_word();
        return false;
    } 
}

string
syntactic_parser::get_varname()
{
    static int varcount = 1;
    string name = "T" + to_string(varcount++);
    return name;
}

void
syntactic_parser::print_expre()
{
    cout << "Intermediate Code:" << endl;
    for (const auto& t : semantic_tetrad) {
        cout << get<0>(t) << " = " << get<1>(t)<< " " << get<2>(t) << " " << get<3>(t) << endl;
    }
}

bool
syntactic_parser::has_expre()
{
    if (semantic_tetrad.size() > 0) return true;
    return false;
}