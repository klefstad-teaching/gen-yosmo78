#include <iostream>
using namespace std;

#include "error.h"
#include "Inst.h"
#include "registers.h"

RegisterManager RM;

int _maxRegsUsed;

void clearRegsUsed()
{
    _maxRegsUsed = 0;
}

int getNumRegsUsed()
{
    return _maxRegsUsed;
}

void updateMaxRegsUsed(int v)
{
    _maxRegsUsed = max(_maxRegsUsed, v);
}

