#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


//Function prototypes for each instruction handler
void parseInstruction(char* line);
void handleADD(char* line);
void handleAND(char* line);
void handleNOT(char* line);
void handleBR(char* line);
void handleLD(char* line);
void handleLDI(char* line);
void handleLDR(char* line);
void handleLEA(char* line);
void handleST(char* line);
void handleSTI(char* line);
void handleSTR(char* line);
void handleTRAP(char* line);
char* decimalToBinary(int decimal, int numBits);

//Parses and routes an assembly line
void parseInstruction(char* line)
{
    char keyword[10];
    //Extracts the keyword of the command
    scanf(line, '%s', keyword); 

    //Compare and dispatch to the correct handler
    if(strcmp(keyword, "ADD") == 0)
    {
        handleADD(line);
    }
    else if (strcmp(keyword, "AND") == 0)
    {
        handleAND(line);
    }
    else if (strcmp(keyword, "NOT") == 0)
    {
        handleNOT(line);
    }
    else if (strcmp(keyword, "BR") == 0)
    {
        handleBR(line);
    }
    else if (strcmp(keyword, "LD") == 0)
    {
        handleLD(line);
    }
    else if (strcmp(keyword, "LDI") == 0)
    {
        handleLDI(line);
    }
    else if (strcmp(keyword, "LDR") == 0)
    {
        handleLDR(line);
    }
    else if (strcmp(keyword, "LEA") == 0)
    {
        handleLEA(line);
    }
    else if (strcmp(keyword, "ST") == 0)
    {
        handleST(line);
    }
    else if (strcmp(keyword, "STI") == 0)
    {
        handleSTI(line);
    }
    else if (strcmp(keyword, "STR") == 0)
    {
        handleSTR(line);
    }
    else if (strcmp(keyword, "TRAP") == 0)
    {
        handleTRAP(line);
    }
    else
    {
        printf("Unrecognized instruction: %s\n", keyword);
    }
    
}
// Converts a decimal number to a binary string with a fixed number of bits
char* decimalToBinary(int decimal, int numBits) {
    char* binary = (char*)malloc(numBits + 1);
    if (binary == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    binary[numBits] = '\0'; // Null-terminate the string

    for (int i = numBits - 1; i >= 0; i--) {
        int remainder = decimal % 2;
        if (remainder == 1) {
            binary[i] = '1';
        } else {
            binary[i] = '0';
        }
        decimal /= 2; // Divide by 2 to shift to the next bit
    }

    return binary;
}

//Quick function that removes commas when translating them to binary
void stripComma(char* str)
{
    char* comma = strchr(str, ',');
    if (comma) *comma = '\0';
}

//Get register number from R0-R7
int parseRegister(char* reg)
{
    if (reg[0] == 'R' && reg[1] >= '0' && reg[1] <= '7')
    {
        int regNum = atoi(&reg[1]); //The number of the register becomes an int
        return regNum;
    }
    return -1; //error, likely to be removed later
}

//function that handles ADD operation
void handleADD(char* line)
{
    char* token;
    char* tokens[4];
    int count = 0;

    //Tokenize the line
    token = strtok(line, " \t");
    while (token!= NULL && count < 4)
    {
        token = strtok(NULL, " \t");
        //When a token is found, remove the comma and and add the token to the tokens array
        if (token)
        {
            stripComma(token);
            tokens[count++] = token;
        }
    }

    //If there are more tokens than expected, print error message and return
    if (count < 3)
    {
        printf("Invalid ADD format\n");
        return;
    }

    int dr = parseRegister(tokens[0]);
    int sr1 = parseRegister(tokens[1]);

    //Start building binary
    char* opCode = strdup("0001");
    char* drBits = decimalToBinary(dr, 3);
    char* sr1Bits = decimalToBinary(sr1, 3);
    //Bit that notates whether second operand is an SR or immediate value
    char* modeBit;
    char* result = (char*)malloc(17);

    //Check if the second operand is a register or immediate value
    //If the second operand is a register
    if (tokens[2][0] == 'R')
    {
        int operand2 = parseRegister(tokens[2]);
        modeBit = strdup("0");
        char*sr2Bits = decimalToBinary(operand2, 5);

        snprintf(result, 21, "%s %s %s %s %s", opCode, drBits, sr1Bits, modeBit, sr2Bits);
    }
    
    //If the second operand is an immediate value
    else
    {
        int operand2 = atoi(&tokens[2][1]);
        if(operand2 < -16 || operand2 > 15)
        {
            printf("Immediate value out of range (-16 to 15)\n");
            return;
        }
        modeBit = strdup("1");
        //Handle negative numbers as 2's complement
        if(operand2 < 0)
        {
            operand2 = (1 << 5) + operand2;
        }
        char* immBits = decimalToBinary(operand2, 5);

        snprintf(result, 21, "%s %s %s %s %s", opCode, drBits, sr1Bits, modeBit, immBits);
    }
    printf("Binary ADD: %s\n", result);

    free(drBits);
    free(sr1Bits);
    free(result);
}

//function that handles AND operation
void handleAND(char* line)
{
    char* token;
    char* tokens[4];
    int count = 0;

    //Tokenize the line
    token = strtok(line, " \t");
    while (token!= NULL && count < 4)
    {
        token = strtok(NULL, " \t");
        if (token)
        {
            stripComma(token);
            tokens[count++] = token;
        }
    }

    if (count < 3)
    {
        printf("Invalid ADD format\n");
        return;
    }

    int dr = parseRegister(tokens[0]);
    int sr1 = parseRegister(tokens[1]);

    if (dr == -1 || sr1 == -1)
    {
        printf("Invalid register in ADD\n");
        return;
    }

    //Start building binary
    char* opCode = strdup("0101");
    char* drBits = decimalToBinary(dr, 3);
    char* sr1Bits = decimalToBinary(sr1, 3);
    //Bit that notates whether second operand is an SR or immediate value
    char* modeBit;
    char* result = (char*)malloc(17);

    //Check if the second operand is a register or immediate value
    //If the second operand is a register
    if (tokens[2][0] == 'R')
    {
        int operand2 = parseRegister(tokens[2]);
        modeBit = strdup("0");
        char*sr2Bits = decimalToBinary(operand2, 5);

        snprintf(result, 21, "%s %s %s %s %s", opCode, drBits, sr1Bits, modeBit, sr2Bits);
    }
    
    //If the second operand is an immediate value
    else
    {
        int operand2 = atoi(&tokens[2][1]);
        if(operand2 < -16 || operand2 > 15)
        {
            printf("Immediate value out of range (-16 to 15)\n");
            return;
        }
        modeBit = strdup("1");
        //Handle negative numbers as 2's complement
        if(operand2 < 0)
        {
            operand2 = (1 << 5) + operand2;
        }
        char* immBits = decimalToBinary(operand2, 5);

        snprintf(result, 21, "%s %s %s %s %s", opCode, drBits, sr1Bits, modeBit, immBits);
    }
    printf("Binary ADD: %s\n", result);

    free(drBits);
    free(sr1Bits);
    free(result);
}

//function that handles NOT operation
void handleNOT(char* line) {
    char* token;
    char* tokens[3];
    int count = 0;

    // Tokenize the line
    token = strtok(line, " \t");
    while (token != NULL && count < 3) {
        token = strtok(NULL, " \t");
        if (token) {
            stripComma(token);
            tokens[count++] = token;
        }
    }

    int dr  = parseRegister(tokens[0]);
    int sr  = parseRegister(tokens[1]);

    if (dr == -1 || sr == -1) {
        printf("Invalid register in NOT\n");
        return;
    }

    // Start building binary
    char* opcode  = strdup("1001");                    // 4 bits for opcode
    char* drBits  = decimalToBinary(dr, 3);            // 3 bits for DR
    char* srBits  = decimalToBinary(sr, 3);            // 3 bits for SR
    char* sixOnes = strdup("111111");                  // 6 bits for the 6 ones at the end

    // Allocate memory for the result
    char* result = (char*)malloc(21);  
    snprintf(result, 21, "%s %s %s %s", opcode, drBits, srBits, sixOnes);

    printf("Binary NOT: %s\n", result);

    // Free memory
    free(drBits);
    free(srBits);
    free(result);
}

//function that handles BR operation
void handleBR(char* line)
{
    char* token;
    char* tokens[3];
    int count = 0;

    //Tokenize the line
    token = strtok(line, " \t");
    while (token != NULL && count < 3)
    {
        token = strtok(NULL, " \t");
        if (token)
        {
            stripComma(token);
            tokens[count++] = token;
        }
    }

    //Flag conditions and PC offset
    char* condFlags = tokens[0];
    int pcOffset = atoi(tokens[1]);

    //Convert condition flags to bits
    int nFlag = (strchr(condFlags, 'N') != NULL);
    int zFlag = (strchr(condFlags, 'Z') != NULL);
    int pFlag = (strchr(condFlags, 'P') != NULL);

    //Combine the flags into a single integer
    int flags = (nFlag << 2) | (zFlag << 1) | pFlag;

    //Start building binary
    char* opcode = strdup("0000");
    char* flagsBits = decimalToBinary(flags, 3);
    char* pcOffsetBits = decimalToBinary(pcOffset, 9);

    //Allocate memory for the result
    char* result = (char*)malloc(22);
    snprintf(result, 22, "%s %s %s", opcode, flagsBits, pcOffsetBits);

    printf("Binary BR: %s\n", result);

    //Free memory
    free(flagsBits);
    free(pcOffsetBits);
    free(result);
}

//function that handles LD operation
void handleLD(char* line) {
    char* token;
    char* tokens[2];
    int count = 0;

    // Tokenize the line
    token = strtok(line, " \t");
    while (token != NULL && count < 2) {
        token = strtok(NULL, " \t");
        if (token) {
            stripComma(token);
            tokens[count++] = token;
        }
    }

    if (count < 2) {
        printf("Invalid LD format\n");
        return;
    }

    int dr  = parseRegister(tokens[0]);
    int pcOffset = atoi(tokens[1]);

    if (dr == -1) {
        printf("Invalid register in LD\n");
        return;
    }

    // Start building binary
    char* opcode = strdup("0010");                    // 4 bits for opcode
    char* drBits = decimalToBinary(dr, 3);            // 3 bits for DR
    char* pcOffsetBits = decimalToBinary(pcOffset, 9); // 9 bits for the PCoffset

    // Allocate memory for the result
    char* result = (char*)malloc(19); // 4 + 3 + 9 = 16, with spaces
    snprintf(result, 19, "%s %s %s", opcode, drBits, pcOffsetBits);

    printf("Binary LD: %s\n", result);

    // Free memory
    free(drBits);
    free(pcOffsetBits);
    free(result);
}

//function that handles LDI operation
void handleLDI(char* line)
{
    char* token;
    char*tokens[2];
    int count = 0;

    //Tokenize the line
    token = strtok(line, " \t");
    while (token != NULL && count < 2)
    {
        token = strtok(NULL, " \t");
        if (token)
        {
            stripComma(token);
            tokens[count++] = token;
        }
    }

    int dr = parseRegister(tokens[0]);
    int pcOffset = atoi(tokens[1]);

    //Start building binary
    char* opcode = strdup("1010");
    char* drBits = decimalToBinary(dr, 3);
    char* pcOffsetBits = decimalToBinary(pcOffset, 9);

    //Allocate memory for the result
    char* result = (char*)malloc(19);
    snprintf(result, 19, "%s %s %s", opcode, drBits, pcOffsetBits);

    printf("Binary LDI: %s\n", result);

    //Free memory
    free(drBits);
    free(pcOffsetBits);
    free(result);
}

//function that handles LDR operation
void handleLDR(char* line)
{
    char* token;
    char* tokens[3];
    int count = 0;

    //Tokenize the line
    token = strtok(line, " \t");
    while (token != NULL && count < 3)
    {
        token = strtok(NULL, " \t");
        if (token)
        {
            stripComma(token);
            tokens[count++] = token;
        }
    }

    int dr = parseRegister(tokens[0]);
    int baseR = parseRegister(tokens[1]);
    int offset = atoi(tokens[2]);
    
    //Start building binary
    char* opcode = strdup("0110");
    char* drBits = decimalToBinary(dr, 3);
    char* baseRBits = decimalToBinary(baseR, 3);
    char* offsetBits = decimalToBinary(offset, 6);

    //Allocate memory for the result
    char* result = (char*)malloc(22);
    snprintf(result, 22, "%s %s %s %s", opcode, drBits, baseRBits, offsetBits);

    printf("Binary LDR: %s\n", result);

    //Free memory
    free(drBits);
    free(baseRBits);
    free(offsetBits);
    free(result);
}

//function that handles LEA operation
void handleLEA(char* line)
{
    char* token;
    char* tokens[2];
    int count = 0;

    //Tokenize the line
    token = strtok(line, " \t");
    while (token != NULL && count < 2)
    {
        token = strtok(NULL, " \t");
        if (token)
        {
            stripComma(token);
            tokens[count++] = token;
        }
    }

    int dr = parseRegister(tokens[0]);
    int pcOffset = atoi(tokens[1]);

    //Start building binary
    char* opcode = strdup("1110");
    char* drBits = decimalToBinary(dr, 3);
    char* pcOffsetBits = decimalToBinary(pcOffset, 9);

    //Allocate memory for the result
    char* result = (char*)malloc(19);
    snprintf(result, 19, "%s %s %s", opcode, drBits, pcOffsetBits);

    printf("Binary LEA: %s \n", result);

    //Free memory
    free(drBits);
    free(pcOffsetBits);
    free(result);
}

//function that handles ST operation
void handleST(char* line)
{
    char* token;
    char* tokens[2];
    int count = 0;

    //Tokenize the line
    token = strtok(line, " \t");
    while (token != NULL && count < 2)
    {
        token = strtok(NULL, " \t");
        if (token)
        {
            stripComma(token);
            tokens[count++] = token;
        }
    }

    int sr = parseRegister(tokens[0]);
    int pcOffset = atoi(tokens[1]);

    //Start building binary
    char* opcode = strdup("0011");
    char* srBits = decimalToBinary(sr, 3);
    char* pcOffsetBits = decimalToBinary(pcOffset, 9);

    //Allocate memory for the result
    char* result = (char*)malloc(22);
    snprintf(result, 22, "%s %s %s", opcode, srBits, pcOffsetBits);

    printf("Binary ST: %s\n", result);

    //Free memory
    free(srBits);
    free(pcOffsetBits);
    free(result);
}

//function that handles STI operation
void handleSTI(char* line)
{
    char* token;
    char* tokens[2];
    int count = 0;

    //Tokenize the line
    token = strtok(line, " \t");
    while (token != NULL && count < 2)
    {
        token = strtok(NULL, " \t");
        if (token)
        {
            stripComma(token);
            tokens[count++] = token;
        }
    }

    int sr = parseRegister(tokens[0]);
    int pcOffset = atoi(tokens[1]);

    //Start building binary
    char* opcode = strdup("1011");
    char* srBits = decimalToBinary(sr, 3);
    char* pcOffsetBits = decimalToBinary(pcOffset, 9);

    //Allocate memory for the result
    char* result = (char*)malloc(22);
    snprintf(result, 22, "%s %s %s", opcode, srBits, pcOffsetBits);

    printf("Binary STI: %s\n", result);

    //Free memory
    free(srBits);
    free(pcOffsetBits);
    free(result);
}

//function that handles STR operation
void handleSTR (char* line)
{
    char* token;
    char* tokens[3];
    int count = 0;

    //Tokenize the line
    token = strtok(line, " \t");
    while (token != NULL && count < 3)
    {
        token = strtok(NULL, " \t");
        if (token)
        {
            stripComma(token);
            tokens[count++] = token;
        }
    }

    int sr = parseRegister(tokens[0]);
    int baseR = parseRegister(tokens[1]);
    int offset = atoi(tokens[2]);

    //Start building binary
    char* opcode = strdup("0111");
    char* srBits = decimalToBinary(sr, 3);
    char* baseRBits = decimalToBinary(baseR, 3);
    char* offsetBits = decimalToBinary(offset, 6);

    //Allocate memory for the result
    char* result = (char*)malloc(22);
    snprintf(result, 22, "%s %s %s %s", opcode, srBits, baseRBits, offsetBits);

    printf("Binary STR: %s\n", result);

    //Free memory
    free(srBits);
    free(baseRBits);
    free(offsetBits);
    free(result);
}

//function that handles TRAP operation
void handleTRAP(char* line)
{
    char* token;
    char* trapVectorStr;

    //Get first token and then the trap vector
    token = strtok(line, " \t");
    token = strtok(NULL, " \t");
    if (!token)
    {
        printf("Invalid TRAP format\n");
    }

    trapVectorStr = token;

    //Convert trap vector to integer
    int trapVector = 0;
    trapVector = atoi(trapVectorStr + 1);

    //Build the binary instruction
    char* opcode = strdup("1111");
    char* unused = strdup("0000");
    char* trapVectorBits = decimalToBinary(trapVector, 8);

    //Final result
    char* result = (char*)malloc(22);
    snprintf(result, 22, "%s %s %s", opcode, unused, trapVectorBits);
    printf("Binary TRAP: %s\n", result);

    //Clean up
    free(trapVectorBits);
    free(result);


}

//function that takes in an assembly file and translates the .asm code into .bin
void assembleFile(const char* inputFileName, const char* outputFileName) {
    FILE* inputFile = fopen(inputFileName, "r");
    if (!inputFile) {
        perror("Failed to open input file");
        return;
    }

    FILE* outputFile = fopen(outputFileName, "w");
    if (!outputFile) {
        perror("Failed to open output file");
        fclose(inputFile);
        return;
    }

    //line variable with a max of 255 characters allowed in it
    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
        // Strip newline
        line[strcspn(line, "\r\n")] = '\0';

        // Skip empty lines
        if (strlen(line) == 0) continue;

        // Get binary result
        char* binaryLine = processInstruction(line);
        if (binaryLine) {
            // Remove spaces from binaryLine before writing
            for (int i = 0; binaryLine[i]; i++) {
                if (binaryLine[i] != ' ')
                    fputc(binaryLine[i], outputFile);
            }
            fputc('\n', outputFile);
            free(binaryLine);
        } else {
            fprintf(stderr, "Unrecognized instruction: %s\n", line);
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}
