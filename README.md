# Lispi - A Lisp Interpreter
### Raymond Rindels
### April 2019

## Compile
In the same directory as *Makefile*, *lispi.cc*, *lispi.h*, and *builtin.cc*, run `make` to compile.

## Run
`./lispi` to run the binary.

## Use

### Saving Output
All session output is saved in *results.txt*. This file will be overwritten if a new session is started.

### Valid Expressions
 
| Construct             | Syntax                                            | Example                               |
| --------------------- | ------------------------------------------------- | ------------------------------------- |
| Variable Reference    | *variable*                                        | x -> 10                               |
| Constant Literal      | *number* or *boolean*                             | T -> T                                |
| Quotation             | *'expression*                                     | '(+ 1 1) -> (+ 1 1)                   |
| Conditional           | *(if test consequence alternate)*                 | (if (> 10 20) (+ 1 1) (+ 3 3)) -> 6   |
| Variable Definition   | *(define variable expression)*                    | (define x 10)                         |   
| Function Call         | *(function args ...)*                             | (sqrt (+ 8 8)) -> 4                   |
| Variable Assignment   | *(set! variable expression)*                      | (set! x (+ 5 6))                      |
| Function Definition   | *(defun function_name (variable ...) expression)* | (defun square (x) (* x x))            |

### Predifined Operations

| Operation | Syntax | Example |
| --------- | ------ | ------- |
|       |       |          | 
