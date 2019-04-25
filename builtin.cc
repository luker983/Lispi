/**
 * @file builtin.cc
 * @brief built in functions for lispi.cc
 *
 * @author Raymond Rindels
 * @date April 8, 2019
 */

#include "lispi.h"

using namespace std;

// adds all values in list
Value bi_add(Value exp) {
    list<Value>::iterator it = exp.values.begin();

    long sum(stol(it->value, nullptr, 10));
    ++it;
    while (it != exp.values.end()) {
        sum += stol((it++)->value, nullptr, 10);
    }
    return Value(NUM, to_string(sum));
}

// subtracts values after the top of the list from the first element of the list
Value bi_sub(Value exp) {
    list<Value>::iterator it = exp.values.begin();
    
    long diff(stol(it->value, nullptr, 10));
    ++it;
    while (it != exp.values.end()) {
        diff -= stol((it++)->value, nullptr, 10);
    }
    return Value(NUM, to_string(diff));
}

// multiplies all values in list
Value bi_mul(Value exp) {
    list<Value>::iterator it = exp.values.begin();
    
    long prod(stol(it->value, nullptr, 10));
    ++it;
    while (it != exp.values.end()) {
        prod *= stol((it++)->value, nullptr, 10);
    }
    return Value(NUM, to_string(prod));
}

// divides first value by all following values
Value bi_div(Value exp) {
    list<Value>::iterator it = exp.values.begin();
    
    long quot(stol(it->value, nullptr, 10));
    long value;
    ++it;
    while (it != exp.values.end()) {
        value = stol((it++)->value, nullptr, 10);
        if (value == 0)
            return Value(ERR, "DZ");
        quot /= value;
    }
    return Value(NUM, to_string(quot));
}

// returns first element of list
Value bi_car(Value exp) {
    list<Value>::iterator it = exp.values.begin();
    return it->values.front();
}

// returns list without first element
Value bi_cdr(Value exp) {
    list<Value>::iterator it = exp.values.begin();
    it->values.pop_front();
    return *it;
}

// true if first value is greater than all following values, false if not
Value bi_gt(Value exp) {
    list<Value>::iterator it = exp.values.begin();
    
    long max(stol(it->value, nullptr, 10));
    ++it;
    while (it != exp.values.end()) {
        if (stol((it++)->value, nullptr, 10) > max) {
            return NIL;
        }
    }

    return T;
}

// true if first value is lesser than all following values, false if not
Value bi_lt(Value exp) {
    list<Value>::iterator it = exp.values.begin();
    
    long min(stol(it->value, nullptr, 10));
    ++it;
    while (it != exp.values.end()) {
        if (stol((it++)->value, nullptr, 10) < min) {
            return NIL;
        }
    }

    return T;
}

// true if all values are equal, false if not
Value bi_eq(Value exp) {
    list<Value>::iterator it = exp.values.begin();
    
    long succ(stol(it->value, nullptr, 10));
    ++it;
    while (it != exp.values.end()) {
        if (stol((it++)->value, nullptr, 10) != succ) {
            return NIL;
        }
    }

    return T;
}

// false if all values are equal, true if not
Value bi_neq(Value exp) {
    list<Value>::iterator it = exp.values.begin();
    
    long fail(stol(it->value, nullptr, 10));
    ++it;
    while (it != exp.values.end()) {
        if (stol((it++)->value, nullptr, 10) == fail) {
            return NIL;
        }
    }

    return T;
}

// false if one value is false, true if all values are true
Value bi_and(Value exp) {
    list<Value>::iterator it = exp.values.begin();
    
    while (it != exp.values.end()) {
        if ((it++)->value == "NIL") {
            return NIL;
       } 
    }

    return T;
}

// false if all values are false, true if one value is true
Value bi_or(Value exp) {
    list<Value>::iterator it = exp.values.begin();
    
    while (it != exp.values.end()) {
        if ((it++)->value == "T") {
            return T;
        }
    }

    return NIL;
}

// if true, return false, otherwise return true
Value bi_not(Value exp) {
    if (exp.values.front().value == "T") {
        return NIL;
    }

    return T;
}

// square root of first value, trailing zeros are removed
Value bi_sqrt(Value exp) {
    list<Value>::iterator it = exp.values.begin();

    float result = sqrt(stof(it->value, nullptr));
    
    // remove trailing zeros
    string s = to_string(result);
    s.erase(s.find_last_not_of('0') + 1, string::npos); 
    s.erase(s.find_last_not_of('.') + 1, string::npos);

    return Value(SYMBOL, s); 
}

// first value raised to second value, trailing zeros are removed
Value bi_pow(Value exp) {
    list<Value>::iterator it = exp.values.begin();

    float result = pow(stof(it->value, nullptr), stof((++it)->value, nullptr));

    // remove trailing zeros
    string s = to_string(result);
    s.erase(s.find_last_not_of('0') + 1, string::npos); 
    s.erase(s.find_last_not_of('.') + 1, string::npos);

    return Value(SYMBOL, s); 
}

// place first value inside of second value
Value bi_cons(Value exp) {
    Value temp(LIST);
    temp = exp.values.front();
    exp.values.pop_front();
    exp.values.front().values.push_front(temp);
    
    return exp.values.front();
}
