#include <stdio.h>
#include <stdlib.h>

// This describes an abstract machine similar to the x86 two-address machine.
// the SP points at the value on top of the stack
// FP is the activation record (Frame) pointer.  It will always point at the
// old FP (that of the caller).  The first parameter is 1 word below
// the FP and the first local variable of size s words will be -s words
// before it. So initial parameter offset is 1 and initial local offset
// is 0.  Remember all sizes must now be in words not bytes.
// should compile with g++

typedef long long int Word; // most instructions deal with words

typedef Word * WPtr;
typedef Word Register; 

// The registers
Register  R0; // the return register
Register  R1, R2, R3; // General purpose registers

// Control Stack and operations

#define STK_SIZE 50000
typedef Word     ControlStack[STK_SIZE];
ControlStack STK;

// stack pointer and frame pointer
WPtr STKEnd = STK+STK_SIZE-1;
WPtr SP = STKEnd;
WPtr FP = STKEnd;

// arg is a quad word
#define pushw(W) (*--SP) = (Word)(W)
#define popw(W) (W) = (Word)(*SP++)

// arg is a quad word pointer
#define pushp(W) (*--SP) = reinterpret_cast<Word>(W)
#define popp(W) (W) = reinterpret_cast<WPtr>(*SP++)

/*
void dumpStack()
{
    for (WPtr p = SP; p < STK+STK_SIZE; p++)
        cout << "value at " << (Word)p << " : " << *p << endl;
}

void dumpRegs()
{
    cout << "R0 = " << R0 << endl;
    cout << "R1 = " << R1 << endl;
    cout << "R2 = " << R2 << endl;
    cout << "R3 = " << R3 << endl;
    cout << "SP = " << (Word)SP << endl;
    cout << "FP = " << (Word)FP << endl;
}
*/

#include "lib.c"
