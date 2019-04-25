##
# @file Makefile
# Makefile for Lisp Interpreter
#
# @author Raymond Rindels
# @date April 18
##

# C++ compiler and flags
CCC = g++
CCFLAGS = -g

# targets, object files and main interpreter program
TARGET = lispi
OBJECT = builtin

all: $(TARGET)

# compile Lisp interpreter, requires builtin object
$(TARGET): $(TARGET).cc $(OBJECT)
	$(CCC) $(CCFLAGS) -o $(TARGET) $(OBJECT).o $(TARGET).cc

# compile builtin object
$(OBJECT): $(OBJECT).cc
	$(CCC) $(CCFLAGS) -c $(OBJECT).cc

# remove binary and object files
clean:
	$(RM) $(TARGET) $(OBJECT).o
