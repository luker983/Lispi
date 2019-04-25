/**
 * @file lispi.cc
 * @brief lisp interpreter in c++
 *
 * @author Raymond Rindels
 * @date April 2, 2019
 */

#include "lispi.h"

int main() {
    Env env;
    Value value;
    ofstream out_file;

    cout << "Lispi - A Lisp Interpreter\n";
    
    // initialize environment with default values
    set_global_env(&env); 
    // open output file
    out_file.open("results.txt");

    // read, evaluate, print loop
    while(1) {
        // read user input
        string program = get_program();
        // divide input into tokens
        list<string> tokens(tokenize(program));
        // create abstract syntax tree
        Value parse = read_tokens(tokens);

        // check for errors
        if (parse.type != ERR) {
            value = eval(parse, &env);
        } else {
            value = parse;
        }

        // output to stdout and output file
        print_value(value, out_file);
        cout << endl;
        out_file << endl;

        // (quit) signals evaluator to return 'bye'
        if (value.value == "bye") {
            break;    
        }
    }

    out_file.close();
    return 0;
}

/**
 * reads lisp program from user
 *
 * @return user input string
 */
string get_program() {
    string program;

    cout << "> ";
    getline(cin, program);
    
    return program;
}

/**
 * reads in tokens and produces abstract syntax tree
 *
 * @param tokens list of tokens from tokenize()
 * @return abstract syntax tree
 */
Value read_tokens(list<string> &tokens) {
    string token(tokens.front());
    tokens.pop_front();

    // signals quotation
    if (token == "'") {
        Value q = read_tokens(tokens);
        q.set_quote(true);
        return q;
    }
    // beginning of list
    if (token == "(") {
        Value v(LIST);
        // push tokens until end of list is reached
        while (tokens.front() != ")") {
            v.values.push_back(read_tokens(tokens));
        }
        tokens.pop_front();
        return v;
    // error, end of list should be handled in '('
    } else if (token == ")") {
        string e = "P";
        Value err(ERR, e);
        return err;
    // atom: check for digit, bool, or symbol
    } else {
        enum ValueType type;
        if (isdigit(token[0])) {
            type = NUM;
        } else if (token == "T" || token == "NIL") { 
            type = BOOL;
        } else {
            type = SYMBOL;
        }
        Value atom(type, token);
        return atom;
    } 
}

/**
 * divides string into tokens using white-space, apostrophes, and parentheses
 *
 * @param exp user input string
 * @return list of tokens
 */
list<string> tokenize(string exp) {
    list<string> tokens;

    // pad parentheses and apostrophes with whitespace
    exp = replace(exp, "(", " ( ");
    exp = replace(exp, ")", " ) ");
    exp = replace(exp, "'", " ' ");

    // insert each token into a list
    istringstream iss(exp);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(tokens));

    return tokens;
}

/**
 * replaces a portion of a string with another string
 *
 * @param s full string
 * @param from the substring being replaced
 * @param to the substring replacing 'from'
 * @return the modified string
 */
string replace(string s, string from, string to) {
    int n = 0;
    int next; 

    while ((next = s.find(from, n)) != string::npos) {
        s.replace(next, from.size(), to);
        n = next + to.size(); 
    }

    return s;
}

/**
 * evaluates a lisp expression
 *
 * @param value lisp expression
 * @param env the environment in which the expression is evaluated
 * @return resulting lisp value
 */
Value eval(Value value, Env *env) {
    list<Value>::iterator it = value.values.begin();
  
    // quotations evaluate to themselves
    if (value.quote) {
        return value;
    // add symbol to environment with the given value
    } else if (value.type == SYMBOL) {
        return env->mapping.find(value.value)->second;
    // many types evaluate to themselves
    } else if (value.type == NUM || value.quote || value.type == BOOL || value.type == ERR) {
        return value;
    // empty expressions are equivalent to False, or NIL
    } else if (value.values.empty()) {
        return NIL;
    // the first element of a list is a function/special form call
    } else if (it->type == SYMBOL) {
        // (if test consequence alternate), conditional
        if (it->value == "if") {
            Value test = *(++it);
            Value cons = *(++it);
            Value alt = *(++it);

            if (eval(test, env).value == "T") {
                return eval(cons, env);
            } else {
                return eval(alt, env);
            }
        // (define variable expression), defines variable
        } else if (it->value == "define") {
            Value key = *(++it);
            Value v = eval(*(++it), env);
            env->add(key.value, v);
            return v;
        // (defun function (params) expression), declare user defined function
        } else if (it->value == "defun") {
            Value key = *(++it);
            it++;
            Value v(UDPROC);
            while (it != value.values.end()) {
                v.values.push_back(*(it++));
            }
            env->add(key.value, v);
            return v;
        // (set! variable expression), update existing variable
        } else if (it->value == "set!") {
            Value key = *(++it);
            Value v = eval(*(++it), env);
            env->mapping.find(key.value)->second = v;
            return v;
        // (quit), exit the program
        } else if (it->value == "quit") {
            Value bye(SYMBOL, "bye");
            return bye;
        // otherwise, built-in or user-defined function call
        } else {
            // get procedure address or user-defined expression
            Value proc(eval(*it, env));
            // if user-defined...
            if (proc.type == UDPROC) {
                // create new environment for static scoping
                Env proc_env;
                set_global_env(&proc_env);
                // add own function call to environment for recursion
                proc_env.add(it->value, proc);

                Value params = proc.values.front();
                Value exp = proc.values.back();
            
                // copy parameters to new environment
                it++;
                list<Value>::iterator param_it = params.values.begin();
                while (it != value.values.end()) {
                    proc_env.add((param_it++)->value, eval(*(it++), env));
                }        

                return eval(exp, &proc_env);
            }
    
            // place parameters in a new list and evaluate built-in function
            it++;
            Value v(LIST);           
             while (it != value.values.end()) {
                v.values.push_back(eval(*(it++), env));
            }
            return proc.proc(v);
        }
    // if value provided matches none of the above rules, raise an error
    } else {
        Value error(ERR, "NF");
        error.values.push_back(*it);
        return error;
    }
}

/**
 * prints out all types of lisp values
 *
 * @param value lisp value being printed
 * @param out_file output file for recording session
 */
void print_value(Value value, ofstream &out_file) {
    // lists and user-defined procedures print recursively
    if (value.type == LIST || value.type == UDPROC) {
        cout << "(";
        out_file << "(";
        while (value.values.size() > 0) {
            Value v = value.values.front();
            value.values.pop_front();
            print_value(v, out_file);
            if (value.values.size() != 0) {
                cout << " ";
                out_file << " ";
            }
        }
        cout << ")";
        out_file << ")";
    // for debugging purposes, a PROC type should never be printed
    } else if (value.type == PROC) {
        cout << "PROC";
        out_file << "PROC";
    // errors
    } else if (value.type == ERR) {
        // not a function
        if (value.value == "NF") {
            cout << value.values.front().value << " is not a function name; try using a symbol instead";
            out_file << value.values.front().value << " is not a function name; try using a symbol instead";
        // parenthesis error
        } else if (value.value == "P") {
            cout << "An object cannot start with ')'";
            out_file << "An object cannot start with ')'";
        // divide by zero
        } else if (value.value == "DZ") {
            cout << "Cannot divide by zero";
            out_file << "Cannot divide by zero";
        }
    // some sort of atom
    } else {
        cout << value.value;
        out_file << value.value;
    }
}

/**
 * set global environment variables
 *
 * @param env environment being modified
 */
void set_global_env(Env *env) {
    /* arithmetic operations */
    env->add("+", Value(PROC, &bi_add));
    env->add("-", Value(PROC, &bi_sub));
    env->add("*", Value(PROC, &bi_mul));
    env->add("/", Value(PROC, &bi_div));

    /* get first and remove first operations */
    env->add("car", Value(PROC, &bi_car));
    env->add("cdr", Value(PROC, &bi_cdr));

    /* boolean operations */
    env->add(">", Value(PROC, &bi_gt)); 
    env->add("<", Value(PROC, &bi_lt)); 
    env->add("=", Value(PROC, &bi_eq)); 
    env->add("!=", Value(PROC, &bi_neq)); 
    env->add("and", Value(PROC, &bi_and)); 
    env->add("or", Value(PROC, &bi_or)); 
    env->add("not", Value(PROC, &bi_not));

    /* power operations */
    env->add("sqrt", Value(PROC, &bi_sqrt));
    env->add("pow", Value(PROC, &bi_pow)); 

    /* concatentation operation */
    env->add("cons", Value(PROC, &bi_cons)); 
}
