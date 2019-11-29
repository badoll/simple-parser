#include "syntactic_parser.h"
void
syntactic_parser::start(string path)
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
    if (word_t.name == "main") {
        if (!next_word()) return;
    } else {
        string err("lack of function name");
        add_error(err);
    }
    word t1 = word_t;
    if (!next_word()) return;
    word t2 = word_t;
    if (t1.name != "(" || t2.name != ")") {
        string err("parameter list error");
        add_error(err);
    }
    if (!next_word()) return;
    code_block();
    tuple<string,string,string,string,bool> halt;
    halt = make_tuple("","","halt","",false);
    icode_quin.insert(icode_quin.end(),halt);
}

void
syntactic_parser::code_block()
{
    error_discover("{");
    statement_str();
    if (word_t.name != "}") {
        string err("lack of '}'");
        add_error(err);
    }
}

void
syntactic_parser::statement_str()
{
    while (true) {
        statement();
        if (word_t.name == "}") break;
        if (word_t.name == "" && word_t.is_legal()) break; //没有词语了
    }
}

void
syntactic_parser::statement()
{
    while (word_t.type_id != VARIABLE && !word_t.is_conditkw()) {
        string err = "error symbol " + word_t.name;
        add_error(err);
        if (!next_word()) return;
        if (word_t.name == "}") return;
    }

    if (word_t.is_conditkw()) {
        condition();
    } else {
        string result = word_t.name;
        string op = "";
        if (!next_word()) return;
        //检查是否有赋值号
        if (error_discover("=")) {
        //如果没有赋值号则跳过开始分析下一个语句
            return;
        }
        if (expression()) {
            string ag1 = var;
            string ag2 = "";
            icode_quin.push_back(make_tuple(result,ag1,op,ag2,false));
        }
        //检查是否有分号
        error_discover(";");
    }
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
        icode_quin.push_back(make_tuple(result,ag1,op,ag2,false));
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
        icode_quin.push_back(make_tuple(result,ag1,op,ag2,false));
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

void
syntactic_parser::condition()
{
    word cdt =  word_t;
    if (!next_word()) return;
    error_discover("(");
    string ag1 = word_t.name;
    if (!next_word()) return;
    string opt = word_t.name;
    if (!next_word()) return;
    string ag2 = word_t.name;
    if (!next_word()) return;
    error_discover(")");

    tuple<string,string,string,string,bool> filler;
    filler = make_tuple("","","","",false);
    icode_quin.insert(icode_quin.end(),filler); //暂时添加两个空值填充到中间代码的元组中
    icode_quin.insert(icode_quin.end(),filler);
    
    int pst = icode_quin.size();
    code_block();
    int blocklen = icode_quin.size() - pst;
    tuple<string,string,string,string,bool> quin;
    int goto_t, goto_f;
    if (cdt.name == "if") {
        goto_t = pst + 1; //符合条件，goto语句块内部中间代码
        quin = make_tuple(to_string(goto_t),ag1,opt,ag2,true);
        icode_quin[pst-2] = quin;
        // 不符合条件则跳出
        goto_f = pst + blocklen + 1;
        quin = make_tuple(to_string(goto_f),"","","",true);
        icode_quin[pst-1] = quin;
    } else if (cdt.name == "while") {
        goto_t = pst + 1;//符合条件，goto语句块内部中间代码
        quin = make_tuple(to_string(goto_t),ag1,opt,ag2,true);
        icode_quin[pst-2] = quin;
        // 不符合条件则跳出
        goto_f = pst + blocklen + 2;
        quin = make_tuple(to_string(goto_f),"","","",true);
        icode_quin[pst-1] = quin;
        // 跳回判断语句
        goto_t = pst - 1;
        quin = make_tuple(to_string(goto_t),"","","",true);
        icode_quin.insert(icode_quin.end(),quin);
    }

    next_word();
}

bool
syntactic_parser::next_word()
{
    word_t = scan();
    while ((word_t.name == ""&&word_t.is_legal()) || word_t.is_annot()) { //loop to scan
        if (linecount == datas.size() || !next_line()) {
            string err = "statement is not completed";
            add_error(err);
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
    while (!lexical_parser::next_line(datas[linecount])) {//空行
        if (linecount == datas.size()) {
            return false;
        }
    }
    return true;
}

bool
syntactic_parser::error_discover(string identifier)
{
/* 
 * 只能恢复以分号作为结束标志或者变量作为开始标志的语句中的错误
 */
    if (word_t.name != identifier) {
        string err_word;
        while (word_t.type_id != VARIABLE && word_t.name != ";"&& word_t.name != "}") {
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
syntactic_parser::print_icode()
{
    cout << "Intermediate Code:" << endl;
    string result, ag1, opt, ag2;
    bool is_cdt;
    for (int i = 0; i < icode_quin.size(); ++i) {
        result =  get<0>(icode_quin[i]);
        ag1 =  get<1>(icode_quin[i]);
        opt =  get<2>(icode_quin[i]);
        ag2 =  get<3>(icode_quin[i]);
        is_cdt =  get<4>(icode_quin[i]);
        if (is_cdt) { //跳转语句
            if (opt != "") {
                cout << "(" << i+1 << "): if " << ag1 << " " << opt
                 << " " << ag2 << " goto (" <<result << ")" << endl; 
            } else {
                cout << "(" << i+1 << "): goto (" <<result << ")" << endl; 
            }
        } else {
            if (opt == "halt") {
                cout << "(" << i+1 << "): halt"  << endl;
            } else if (opt != "") {
                cout << "(" << i+1 << "): " << result << " = " << ag1
                << " " << opt << " " << ag2 << endl;
            } else {
                cout << "(" << i+1 << "): " << result << " = " << ag1 << endl;
            }
        }
    }
}

bool
syntactic_parser::has_icode()
{
    if (icode_quin.size() > 0) return true;
    return false;
}