#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cmath>
#include <chrono>
#include <regex>
#include <cstdlib>
#include <ctime>

using namespace std;

class Token {
  public:
    int row, col;
    string value, type;
    Token(int r, int c, string v, string t) {
      row = r;
      col = c;
      value = v;
      type = t;
    }
};

const string TTYPE_NUM = "NUM";
const string TTYPE_OP = "OP";
const string TTYPE_STR = "STR";

typedef void (*FnPtr)(vector<int>*, map<int, int>*, int*, Token*);

class SOLError : public exception {};
const char* COL_ERR = "\033[91m";
const char* COL_ERR_DARK = "\033[41m";
const char* COL_RESET = "\033[0m";

void error(const char* message, int row, int col) {
  cout << COL_ERR_DARK << "! ERROR ! Line " << row+1 << ", Col " << col+1 << ":" << COL_RESET << " " << COL_ERR << message << "\n\n";
  throw SOLError();
}

void push(vector<int>* stack, int v) {
  stack->insert(stack->begin(), v);
}

int pop(vector<int>* stack, Token* t) {
  if (stack->empty())
    error("Tried to pop from empty stack", t->row, t->col);
  int v = stack->at(0);
  stack->erase(stack->begin());
  return v;
}

int dup(vector<int>* stack, Token* t) {
  if (stack->empty())
    error("Tried to copy nonexistent stack value", t->row, t->col);
  return stack->at(0);
}

bool is_number(const string &s) {
  return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

void cmd_print(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  cout << dup(stack, t);
}

void cmd_print_char(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  cout << (char) dup(stack, t);
}

void cmd_dup(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  push(stack, dup(stack, t));
}

void cmd_pop(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  pop(stack, t);
}

void cmd_over(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  if (stack->size() < 2)
    error("Tried to copy nonexistent stack value", t->row, t->col);
  push(stack, stack->at(1));
}

void cmd_swap(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  if (stack->size() == 1)
    error("Tried to swap with single element", t->row, t->col);
  int a = pop(stack, t);
  stack->insert(stack->begin()+1, a);
}

void cmd_rot(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  stack->insert(stack->begin()+2, a);
}

void cmd_bottom(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  stack->push_back(a);
}

void cmd_top(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  if (stack->empty())
    error("Tried to pop from empty stack", t->row, t->col);
  push(stack, stack->back());
  stack->pop_back();
}

void cmd_add(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  int b = pop(stack, t);
  push(stack, a+b);
}

void cmd_sub(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  int b = pop(stack, t);
  push(stack, b-a);
}

void cmd_mul(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  int b = pop(stack, t);
  push(stack, a*b);
}

void cmd_div(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  int b = pop(stack, t);
  push(stack, b/a);
}

void cmd_mod(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  int b = pop(stack, t);
  push(stack, b%a);
}

void cmd_exp(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  int b = pop(stack, t);
  push(stack, pow(b, a));
}

void cmd_equals(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  int b = pop(stack, t);
  push(stack, a == b);
}

void cmd_not_equals(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  int b = pop(stack, t);
  push(stack, a != b);
}

void cmd_and(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  int b = pop(stack, t);
  push(stack, a && b);
}

void cmd_or(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  int b = pop(stack, t);
  push(stack, a || b);
}

void cmd_not(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  push(stack, (int) !a);
}

void cmd_greater(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  int b = pop(stack, t);
  push(stack, a > b);
}

void cmd_less(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  int b = pop(stack, t);
  push(stack, a < b);
}

void cmd_greatereq(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  int b = pop(stack, t);
  push(stack, a >= b);
}

void cmd_lesseq(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  int b = pop(stack, t);
  push(stack, a <= b);
}

void cmd_if(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  if (a == 0)
    *index += 1;
}

void cmd_jump(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  if (!labels->count(a)) {
    char* s;
    sprintf(s, "Could not find label `%d`", a);
    error(s, t->row, t->col);
  }
  *index = (*labels)[a];
}

void cmd_input_int(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  string inp;
  getline(cin, inp);
  if (is_number(inp))
    push(stack, stoi(inp));
  else
    error("Expected integer input", t->row, t->col);
}

void cmd_input_str(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  string inp;
  getline(cin, inp);
  reverse(inp.begin(), inp.end());
  for (auto &c : inp) {
    push(stack, (int) c);
  }
  push(stack, inp.size());
}

void cmd_random(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  int a = pop(stack, t);
  int b = pop(stack, t);
  push(stack, rand() % (a - b) + b);
}

void cmd_stacksize(vector<int>* stack, map<int, int>* labels, int* index, Token* t) {
  push(stack, stack->size());
}

map<string, FnPtr> COMMANDS = {
  {"!", cmd_print},
  {".", cmd_print_char},
  {"#", cmd_dup},
  {"_", cmd_pop},
  {"<", cmd_over},
  {"$", cmd_swap},
  {">", cmd_rot},
  {"b", cmd_bottom},
  {"t", cmd_top},
  {"+", cmd_add},
  {"-", cmd_sub},
  {"*", cmd_mul},
  {"/", cmd_div},
  {"%", cmd_mod},
  {"^", cmd_exp},
  {"=", cmd_equals},
  {"~", cmd_not_equals},
  {"&", cmd_and},
  {"|", cmd_or},
  {"n", cmd_not},
  {"g", cmd_greater},
  {"l", cmd_less},
  {"G", cmd_greatereq},
  {"L", cmd_lesseq},
  {"?", cmd_if},
  {"j", cmd_jump},
  {"i", cmd_input_int},
  {"I", cmd_input_str},
  {"r", cmd_random},
  {"s", cmd_stacksize}
};

const unordered_set<char> IGNORED = {' ', '\t'};

const char COMMENT = '\\';
const string LABEL = "@";
const char STR_DELIM = '\"';


string get_file_text(string file) {
  ifstream t(file);
  stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

void print_vec(vector<int> vec) {
  cout << "[";
  for (int i = 0; i < vec.size(); i++) {
    cout << vec[i];
    if (i < vec.size()-1)
      cout << ", ";
  }
  cout << "]" << endl;
}

void print_map(map<int, int> m) {
  cout << "{";
  int i = 0;
  for (const auto& n : m) {
    cout << n.first << ": " << n.second;
    if (i < m.size()-1)
      cout << ", ";
    i++;
  }
  cout << "}" << endl;
}

void lex_update_num(string* this_num, int row, int col, vector<Token>* out) {
  if (this_num->size() > 0) {
    Token t = Token(row, col, *this_num, TTYPE_NUM);
    out->push_back(t);
    *this_num = "";
  }
}

vector<Token> lex(string str) {
  int row = 0;
  int col = -1;
  bool commented = false;
  bool in_string = false;
  string this_string = "";
  string this_num = "";
  vector<Token> out;
  for (auto &c : str) {
    col++;
    // check string
    if (c == STR_DELIM && !commented) {
      in_string = !in_string;
      if (!this_string.empty()) {
        this_string = regex_replace(this_string, regex(R"(\\n)"), "\n");
        this_string = regex_replace(this_string, regex(R"(\\033)"), "\033");
        out.push_back(Token(row, col, this_string, TTYPE_STR));
        this_string.clear();
      }
      continue;
    }
    if (in_string) {
      this_string += c;
      continue;
    }
    // check comment
    if (c == COMMENT) {
      commented = !commented;
      continue;
    }
    if (commented) {
      continue;
    }
    if (IGNORED.count(c)) {
      lex_update_num(&this_num, row, col, &out);
      continue;
    }
    string cstr = string(1, c);
    if (c == '\n') {
      row++;
      col = -1;
      lex_update_num(&this_num, row, col, &out);
    }
    else if (COMMANDS.count(cstr) || cstr == LABEL) {
      lex_update_num(&this_num, row, col, &out);
      out.push_back(Token(row, col, cstr, TTYPE_OP));
    }
    else if (isdigit(c)) {
      if (c == '0' && this_num.size() == 0)
        out.push_back(Token(row, col, "0", TTYPE_NUM));
      else
        this_num += c;
      continue;
    }
    else
      error(string("Unrecognized token `" + cstr + "`").c_str(), row, col);
  }
  lex_update_num(&this_num, row, col, &out);
  return out;
}

void init_labels(vector<Token>* tokens, map<int, int>* labels) {
  for (int i = 0; i < tokens->size(); i++) {
    Token t = tokens->at(i);
    if (t.value == LABEL) {
      if (i == 0)
        error("Expected number before label", t.row, t.col);

      Token pt = tokens->at(i-1);
      if (pt.type != TTYPE_NUM) {
        error("Expected number before label", pt.row, pt.col);
      }
      else {
        (*labels)[stoi(pt.value)] = i-2;
        tokens->erase(tokens->begin()+i-1, tokens->begin()+i+1);
        i -= 2;
      }
    }
  }
}

void sol_interpret(string file, bool debug=false, bool timed=false, bool step=false) {
  srand(time(NULL));
  auto time_start = chrono::high_resolution_clock::now();
  string file_text = get_file_text(file);

  vector<Token> tokens = lex(file_text);
  map<int, int> labels;
  init_labels(&tokens, &labels);
  vector<int> stack;
  int index = 0;

  while (index < tokens.size()) {
    Token t = tokens[index];
    string v = t.value;
    if (t.type == TTYPE_NUM) {
      push(&stack, stoi(v));
    }
    else if (t.type == TTYPE_STR) {
      cout << v;
    }
    else {
      COMMANDS[v](&stack, &labels, &index, &t);
    }
    index++;
    if (step) {
      cout << v << endl;
      print_vec(stack);
      string s;
      getline(cin, s);
    }
  }
  if (timed) {
    auto time_end = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast<chrono::microseconds>(time_end-time_start);
    cout << "\nInterpreted in " << time.count() / 1000.0 << " ms" << endl;
  }

  if (debug) {
    cout << "\n";
    print_vec(stack);
    print_map(labels);
  }
}
