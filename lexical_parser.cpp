#include "lexical_parser.h"
static bool is_letter(char c);
static bool is_num(char c);
static bool is_symbol(char c);

void
lexical_parser::lexparse(string path)
{
    ifstream file(path);
    if (!file.is_open()) {
        return;
    }
    string line;
    while(getline(file,line)) {
        next_line(line);
        analyse(line);
    }
}

void
lexical_parser::analyse(string str)
{
    while (true) {
        word w = scan();
        if (w.name == "" && !w.is_legal()) break;//这一行没有词语了就退出
        if (annot) {
        //若本行有注释符号，则退出循环
            break;
        }
    }
}

word
lexical_parser::scan()
{
    while (*sit_line == ' ' && sit_line != data.end()) sit_line++;//过滤空格
    if (sit_line == data.end()) {
        return word("");
    }
    string symbol,varname,digit;
    word sym,dig,word_t;
    if (is_letter(*sit_line)) { // variable or keyword
        while ((is_letter(*sit_line)||is_num(*sit_line)) 
            && *sit_line!=' ' && sit_line!=data.end()) {
        //如果变量不止一个字符，则连接在一起
            varname += *sit_line;
            sit_line++;
        }
        word_t = word(varname);
        words.push_back(word_t);
    } else if (is_num(*sit_line)) { //digit
        while ((is_letter(*sit_line)||is_num(*sit_line)) 
            && *sit_line!=' '&& sit_line!=data.end()) {
            digit += *sit_line;
            sit_line++;
        }
        dig = word(digit);
        if (!dig.is_legal()) {
        //非法词语，数字加字母
            add_error("variable '" + digit + "' is illegal");
            word_t = word(false);
        } else {
            word_t = dig;
            words.push_back(dig);
        }
    } else { //type : operator
        word_t = divide_symbol();
        if (word_t.is_legal()) {
            words.push_back(word_t);
        }
    }
    return word_t;
}

word
lexical_parser::divide_symbol()
{
    word sym,word_t;
    string tmp,symbol;
    string::iterator tmpit = sit_line;
    while (is_symbol(*sit_line) && *sit_line!=' '
        && sit_line!=data.end()) {
    //如果符号不止一个字符，则连接在一起
        tmp += *sit_line;
        sym = word(tmp);
        if (sym.is_annot()) {
        //注释符号则退出
            annot = true;
            return sym;
        }
        if (sym.is_legal()) {
            symbol = tmp;
            tmpit = sit_line;
        }
        sit_line++;
    }
    if (symbol == "") {//illegal symbol
        int pos,minpos = tmp.size();
        for (const auto& s : opt) {
            if ((pos = tmp.find(s)) != string::npos) {
                if (pos < minpos) minpos = pos;
            }
        }
        tmp = tmp.substr(0,minpos);
        sit_line = tmpit + minpos;  //省略非法符号
        add_error("symbol '" + tmp + "' is illegal");
        word_t = word(false);
    } else { //连续符号中前面有合法操作符的情况
        word_t = word(symbol);
        sit_line = tmpit + 1;
    }
    return word_t;
}

bool
lexical_parser::next_line(string line)
{
    annot = false;
    linecount++;
    data = line;
    if (line == "") {
        return false;
    } else {
        sit_line = data.begin();
    }
    return true;
    
}

void
lexical_parser::add_error(string err_msg)
{
    err_msgs.push_back("Line " + to_string(linecount) + ": " + err_msg);
}

vector<word>
lexical_parser::get_words() const
{
    return words;
}

void
lexical_parser::print_words() const
{
    cout << "Words: ";
    for (const auto& word : words) {
        word.print();
    }
    cout << endl;
}

bool
lexical_parser::has_error () const
{
    if (err_msgs.size() > 0) return true;
    return false;
}

void
lexical_parser::print_errors() const
{
    cout << "ERROR:" << endl;
    for (int i = 0; i < err_msgs.size(); ++i)
        cout << err_msgs[i] << endl;
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
is_symbol(char c)
{
    if (!is_letter(c)&&!is_num(c)) return true;
    return false;
}
