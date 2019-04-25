/**
 * @file lispi.h
 * @brief header file for lispi.cc
 *
 * @author Raymond Rindels
 * @date April 3, 2019
 */

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <iterator>
#include <sstream>
#include <map>
#include <cmath>

#include <ctype.h>

using namespace std;

struct Value;
struct Environment;
struct LogStream;

// lisp types
enum ValueType{NUM, SYMBOL, LIST, BOOL, PROC, UDPROC, ERR};

// holds every type of lisp value
struct Value {
    // function pointer for function calls
    typedef Value (*proc_t)(Value);
    
    ValueType type;
    string value;
    list<Value> values;
    bool quote;
    proc_t proc;

    // Constructors
    Value() {}

    Value(ValueType _type) {
        type = _type;
        quote = false;
    }

    Value(ValueType _type, const string &_value) {
        type = _type;
        value = _value;
        quote = false;
    }

    Value(ValueType _type, proc_t _proc) {
        type = _type;
        proc = _proc;
    }

    // Set quote option
    void set_quote(bool _quote) {
        quote = _quote;
    }
};

// maps strings to lisp values
struct Env {
    map<string, Value> mapping;
    
    // variable assignment
    void add(string key, Value value) {
        mapping.insert(pair<string, Value>(key, value));
    }
};

// define commonly used lisp values
const Value NIL(BOOL, "NIL");
const Value T(BOOL, "T");

// interpreter functions
string get_program();
Value read_tokens(list<string> &tokens);
list<string> tokenize(string exp);
string replace(string s, string from, string to);
Value eval(Value value, Env *env);
void print_value(Value value, ofstream &out_file);
void set_global_env(Env *env);

// Built in procedures
Value bi_add(Value exp);
Value bi_sub(Value exp);
Value bi_mul(Value exp);
Value bi_div(Value exp);

Value bi_car(Value exp);
Value bi_cdr(Value exp);

Value bi_gt(Value exp);
Value bi_lt(Value exp);
Value bi_eq(Value exp);
Value bi_neq(Value exp);
Value bi_and(Value exp);
Value bi_or(Value exp);
Value bi_not(Value exp);

Value bi_sqrt(Value exp);
Value bi_pow(Value exp);

Value bi_cons(Value exp);
