#include <stdio.h>

extern void CopyString(char *destination, const char *source);

int main (int argc, char **args)
{
    const char *source = "First string - source";
    char destination[] = "Second string - destination";  /*  Destination is an array, we are going to change the string. */
    
    CopyString(destination, source);
    
    while (1)
    {
    }
    
    return 0;
}