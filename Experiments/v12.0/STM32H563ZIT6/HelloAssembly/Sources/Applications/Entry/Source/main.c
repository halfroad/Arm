#include <stdio.h>

int counter = 3;

extern void DisableExtrnalInterrupts();
extern void EnableExternalInterrupts();

extern void WriteRegisters();
extern void AddRegisters();

int add(int i, int j)
{
  int res = 0;
  __asm
  (
    "ADD %[result], %[input_i], %[input_j] \t\n"
    "SUB %[result], %[input_i], %[result]  \t\n"
    : [result] "=&r" (res)
    : [input_i] "r" (i), [input_j] "r" (j)
  );
  return res;
}

int main (int argc, char ** args)
{
    DisableExtrnalInterrupts();
    EnableExternalInterrupts();
    
    
    WriteRegisters();
    AddRegisters();
    
    int result = add(2, 3);
    
    while (1)
    {
        
    }
    
    return 0;
}

int addOperands(int a, int b, int c, int d, int e)
{
    return a + b + c + d + e;
}