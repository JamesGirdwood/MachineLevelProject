#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to convert decimal to binary
char* decimalToBinary(int num, int bits) {
    //Start with a static char array of 16 values
    static char binaryNum[16];
    int i;

    // Initialize binaryNum array with '0's
    for (i = 0; i < bits; i++) 
    {
        binaryNum[i] = '0';
    }

    //Null terminate the string so it cuts the array down to defined values
    binaryNum[bits] = '\0';

    i = bits - 1;
    while (num > 0) 
    {
        //Converts binary digit to 1 or 0 based on whether there is a remainder
        binaryNum[i--] = (num % 2) + '0';
        num /= 2;
    }

    return binaryNum;
}

// Function to parse the ADD instruction
char* parseAdd(char* line) {
    printf("Parsing line: %s\n", line);  

    // Extract DR, SR1, and operand2 (either numeric value or SR2)
    char* dr = strtok(line, " ");
    char* sr1 = strtok(NULL, ", ");
    char* operand2 = strtok(NULL, ", ");

    //Print out which registers are what for debugging purposes
    printf("DR: %s, SR1: %s, Operand2: %s\n", dr, sr1, operand2);

    // String used to return
    static char instruction[16];

    // Convert the DR and SR registers into their integer values
    //+ 1 insures that the atoi function will skip the R in the register name
    int DRValue = atoi(dr + 1);  
    int SR1Value = atoi(sr1 + 1); 

    // Convert the DR and SR register numbers into binary strings
    char* BinDR = decimalToBinary(DRValue, 3); 
    char* BinSR = decimalToBinary(SR1Value, 3);

    // Check if operand2 is a register
    if (operand2[0] == 'R') {
        int SR2Value = atoi(operand2 + 1);
        char* BinSR2 = decimalToBinary(SR2Value, 3);  
        // Construct the instruction string for register mode
        sprintf(instruction, "0001 %s %s 0 00%s", BinDR, BinSR, BinSR2);
        return instruction;

    } 
    //If Operand2 is not a register, it's a numeric value
    else 
    {
        int NumValue = atoi(operand2);
        char* BinNum = decimalToBinary(NumValue, 5); 
        // Construct the instruction string for immediate mode
        sprintf(instruction, "0001 %s %s 1 %s", BinDR, BinSR, BinNum);
        return instruction;
    }
}

char* parseAnd(char* line)
{
    //Mentions what line is being AND-ed for debugging purposes
    printf("Parsing line: %s \n", line);

    //Extract DR, SR1, and operand2(can be either a numeric value or SR2)
    char* dr = strtok(line, " ");
    char* sr1 =strtok(NULL, ", ");
    char* operand2 = strtok(NULL, ", ");

    //Shows which registers are what, for debugging purposes
    printf("DR: %s, SR1, %s, Operand2: %s \n", dr, sr1, operand2);

    static char instruction[16];

    //Skip R in the register names and convert R number to int
    int DRValue = atoi(dr + 1);
    int SR1Value = atoi(sr1 + 1);

    //Convert the DR and SR register numbers into binary strings
    char* BinDR = decimalToBinary(DRValue, 3);
    char* BinSR = decimalToBinary(SR1Value, 3);

    //Check if operand2 is a register or immediate value
    if (operand2[0] == 'R')
    {
        int SR2Value = atoi(operand2 + 1);
        char* BinSR2 = decimalToBinary(SR2Value, 5);

        //Construct the instruction string for if operand2 is a register
        sprintf(instruction, "0101 %s %s 0 %s", BinDR, BinSR, BinSR2);
        return instruction;
    }
    //if operand2 is not a register, it's a numeric value
    else
    {
        int NumValue = atoi(operand2);
        char* BinNum = decimalToBinary(NumValue, 5);
        sprintf(instruction, "0101 %s %s 1 %s", BinDR, BinSR, BinNum);
        return instruction;
    }
}
