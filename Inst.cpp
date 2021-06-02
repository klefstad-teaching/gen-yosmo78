// #include "all.h"
#include <iostream>
using namespace std;
#include "error.h"
#include "Inst.h"
#include "tokens.h"
#include "registers.h"

static Oper regOpers[] = {
    new RegisterOper(0),
    new RegisterOper(1),
    new RegisterOper(2),
    new RegisterOper(3),
    new RegisterOper(4),
    new RegisterOper(5),
    new RegisterOper(6),
    new RegisterOper(7),
    new RegisterOper(8),
    new RegisterOper(9),
    new RegisterOper(10),
    new RegisterOper(11),
    new RegisterOper(12),
    new RegisterOper(13),
    new RegisterOper(14),
    new RegisterOper(15),
    0
};

Oper regOper(int i)
{
    return regOpers[i];
}

Oper constOper(int v)
{
    return ImmediateOper :: make (ConstOper :: make(v));
}

int TempLabelOper  ::  TEMP_NUMBER = 10;

void LabelInst :: gen() 
{
    cout << labelName << ":\n"; 
}

void UnaryInst :: gen()
{ 
    compiler_error("undefined method gen in UnaryInst");
}

void NegateInst :: gen()
{
    cout << "    " << operand << " = - (Word)" << operand << ";\n"; 
}

void DecrInst :: gen()
{
    cout << "    (" << operand << ") --;\n";
}

void IncrInst :: gen()
{
    cout << "    (" << operand << ") ++;\n";
}

void NotInst :: gen()
{
    cout << "    (" << operand << ") = !(" << operand << ");\n";
}

void ReturnInst :: gen()
{
    cout << "    return 0;\n";
}


void BinaryInst :: gen()
{
    compiler_error("unknown binary operator");
}

void MoveNotInst :: gen()
{
   cout << "    " << dest << " = !(" << src << ");\n";
}

void PlusInst :: gen()
{
   cout << "    " << dest << " += " << src << ";\n";
}

void MinusInst :: gen()
{
   cout << "    " << dest << " -= " << src << ";\n";
}

void MultInst :: gen()
{
   cout << "    " << dest << " *= " << src << ";\n";
}

void DivInst :: gen()
{
   cout << "    " << dest << " /= " << src << ";\n";
}

void ModInst :: gen()
{
   cout << "    " << dest << " %= " << src << ";\n";
}

    void do_move(Oper dest, Oper src)
    {
        cout << "    " << dest << " = " << src << ";\n";
    }

    void do_push(Oper src)
    {
        cout << "    pushw(" << src << ");\n";
    }

    void do_pop(Oper dest)
    {
        cout << "    popw(" << dest << ");\n";
    }

    void do_pushp(Oper src)
    {
        cout << "    pushp(" << src << ");\n";
    }

    void do_popp(Oper dest)
    {
        cout << "    popp(" << dest << ");\n";
    }


void MoveInst :: gen()
{
   if (dest == TOS)
      do_push(src);
   else if (src == TOS)
      do_pop(dest);
   else
      do_move(dest, src);
}

void AddressInst :: gen()
{
    cout << "    " << dest << " =  & " << src << ";\n";
}

void JumpInst :: gen()
{
    cout << "    goto " << to << ";\n";
}

void JumpZeroInst :: gen()
{
    cout << "    if ( !" << oper << " ) goto " << label << ";\n";
}

void JumpNotZeroInst :: gen()
{
    cout << "    if ( " << oper << " ) goto " << label << ";\n";
}

void CompareAndJumpInst :: gen()
{
    int op = cond;
    cout << "    ";
    cout << "if ( " << left << " ";
    switch (op) {
            case '<':
                    cout << "<";
                    break;
            case LE:
                    cout << "<=";
                    break;
            case '>':
                    cout << ">";
                    break;
            case GE:
                    cout << ">=";
                    break;
            case EQ:
                    cout << "==";
                    break;
            case NE:
                    cout << "!=";
                    break;
            default:
                    compiler_error("CompareAndJumpInst.default");
                    break;
    }
    cout << " " << right << " ) goto " << label <<";\n";
}

void CompareAndSetInst :: gen()
{
    int op = cond;
    cout << "    ";
    cout << "(" << dest << ")=" << "(" << left << "";
    switch (op) {
            case '<':
                    cout << "<";
                    break;
            case LE:
                    cout << "<=";
                    break;
            case '>':
                    cout << ">";
                    break;
            case GE:
                    cout << ">=";
                    break;
            case EQ:
                    cout << "==";
                    break;
            case NE:
                    cout << "!=";
                    break;
            default:
                    compiler_error("CompareAndSetInst.default");
                    break;
    }
    cout << "" << right << ");\n";
}

void CallInst :: gen() 
{
   cout << "    " << subprogramName << "();\n";
}

void BeginFileInst :: gen()
{
    cout << "#include \"machine.h\"\n\n";
}

void BeginSubpInst :: gen()
{
    cout << "int " << name << "() { " << endl;
}

void EndSubpInst :: gen()
{
    cout << "} " << endl << endl;
}

// Write out the beginning of a vtable

void BeginVTableInst :: gen()
{
    cout << "Vfn " << name << "[]={ " << endl;
}

// Write out an entry in the Vtable.

void VTableEntryInst :: gen()
{
    cout << "    " << name << ",\n";
}

// End a vtable declaration
 
void EndVTableInst :: gen()
{
    cout <<"    0};\n\n"; 
}


// Write out the header for a String literal

void BeginDataInst :: gen()
{
    cout << "Word " << name << "[]={" << endl;
}

void ConstWordInst :: gen()
{
   cout << "    (Word)" << value << ",\n";
}

void EndDataInst :: gen()
{
    cout << "    0};" << endl;
}

void EnterInst :: gen()
{
    int i;
    do_pushp(regOper(FP_REG));
    do_move(regOper(FP_REG), regOper(SP_REG));
    if (localSize > 0)
    {
        Oper d = regOper(SP_REG);
        cout << "    ";
        d->gen(); 
        cout << " -= " << localSize << ";\n";
    }
    for (i = 0; i < registersUsed ; i++)
        do_push(regOper(MIN_GP_REG+i));
}

void LeaveInst :: gen()
{
    int i;

    for (i = registersUsed-1; i >= 0; i--)
        do_pop(regOper(MIN_GP_REG+i));
    if (localSize > 0)
    {
        Oper d=regOper(SP_REG);
        cout << "    ";
        d->gen(); 
        cout << " += " << localSize << ";\n";
    }
    do_popp(regOper(FP_REG));
    if (paramSize > 0)
    {
        Oper d=regOper(SP_REG);
        cout << "    ";
        d->gen(); 
        cout << " += " << paramSize << ";\n";
    }
}



//  OPERS 

void ImmediateOper :: gen()
{
    cout << "(Word)(";
    immediate->gen();
    cout << ")";
}

void ConstOper :: gen()
{
    cout << constValue;
}

// Output a reference to a hardware register

void RegisterOper :: gen()
{
    switch( regNum ) {
            case RETURN_REG:
                    cout << "R0";
                    return;
            case 1:
                    cout << "R1";
                    return;
            case 2:
                    cout << "R2";
                    return;
            case 3:
                    cout << "R3";
                    return;
            case SP_REG:
                    cout << "SP";
                    return;
            case FP_REG:
                    cout << "FP";
                    return;
            case 6:
                    cout << "R6";
                    return;
            case 7:
                    cout << "R7";
                    return;
            case 8:
                    cout << "TOS";
                    return;
            case 9:
                    cout << "R9";
                    return;
            case 10:
                    cout << "R10";
                    return;
            case 11:
                    cout << "R11";
                    return;
            case 12:
                    cout << "R12";
                    return;
            case 13:
                    cout << "R13";
                    return;
            case 14:
                    cout << "R14";
                    return;
            case 15:
                    cout << "R15";
                    return;
            default:
                    compiler_error("unknown register number in gen");
                    return;
    }
}

void IndexedOper :: gen()
{
    cout << "((WPtr)";
    array->gen();
    cout << ")";
    cout << "[";
    index->gen();
    cout << "]";
}


void IndirectedOper :: gen()
{
    cout << "*((WPtr)(";
    indirected->gen();
    cout << "))"; 
}

void SelectedOper :: gen()
{
    cout << "((WPtr)("; 
    selected->gen();
    cout << ")+" << selectedOffset << ")";
}

void NamedLabelOper :: gen()
{
    cout << labelName;
}


void TempLabelOper :: gen()
{
    cout << "_T" << labelNumber;
}

Oper TOS = regOper(TOS_REG);
