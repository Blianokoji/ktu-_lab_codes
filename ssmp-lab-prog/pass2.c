#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define s 10

int symtabSearch(char *label);
int optabSearch(char *opcode);
void asciitoHex(char *operand,int a);
char hex,opc[s],addr[s];  // Global variable to store opcode and symbol table search results
int counter=0;  // Global variable to store hex value and counter

int main() {
    FILE *output, *imf, *len, *obj;
    int length, size;
    char  opcode[s], operand[s], label[s], start[6],locctr[s];
    
    // Open files
    output = fopen("output.txt", "w");
    imf = fopen("intermediate.txt", "r");
    obj = fopen("obj.txt", "w");
    len = fopen("length.txt", "r");
    if (output == NULL || imf == NULL || obj == NULL || len == NULL) {
        printf("Error opening files\n");
        return 1;
    }
    // Read the program length and size from length.txt
    fscanf(len, "%x %x", &length, &size);
    fclose(len);
    // Read the first line from intermediate file
    fscanf(imf, "%s %s %s", label, opcode, operand);
    // If START directive is found, initialize `start` and output the header record
    if (strcmp(opcode, "START") == 0) {
        strcpy(start, operand);
        fprintf(output, "\t\t\t%s\t\t\t%s\t\t%s\n", label, opcode, operand);  // Assembly listing first line
        fprintf(obj, "H^%s^00%s^%06X", label, operand, length);  // Header record
        fscanf(imf, "%s %s %s %s", locctr, label, opcode, operand);  // Read the next line
    } else {
        strcpy(start, "0000");
    }

    // Process each line until END directive
    while (strcmp(opcode, "END") != 0) {
        printf("%s %s %s %s\n", locctr, label, opcode, operand);  // Debugging output--------------------------
        if (counter == 0 && strcmp(opcode, "RESW") != 0 && strcmp(opcode, "RESB") != 0) {
            fprintf(obj, "\nT^00%s^%02x", locctr,size);  // Text record
        }
        if (strcmp(label, "--") != 0) {
            if(symtabSearch(label)==0){
                printf("Error: Symbol not found\n");
                return 1;
            }  // Look up in symbol table
        }
        if (strcmp(opcode, "BYTE") == 0) {
            fprintf(output, "%s\t\t%s\t\t%s\t\t%s\t\t", locctr, label, opcode, operand);
            fprintf(obj, "^");  // Object code for BYTE
            for (int i = 2; i < strlen(operand) - 1; i++) {
                asciitoHex(operand,i);
                fprintf(obj, "%x", (int)hex);  // Object code for BYTE
                fprintf(output, "%02x", (int)hex);
                counter++;
            }
            fprintf(output, "\n");

        } else if (strcmp(opcode, "WORD") == 0) {
            fprintf(output, "%s\t\t%s\t\t%s\t\t%s\t\t%06x\n", locctr, label, opcode, operand,atoi(operand));
            fprintf(obj, "^%06X", atoi(operand));  // Object code for WORD
            counter+=3;  // Increment counter by 3 for each instruction

        } else if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0) {
            fprintf(output, "%s\t\t%s\t\t%s\t\t%s\n", locctr, label, opcode, operand);
            // No object code for reserved words/bytes
        } else {
            optabSearch(opcode);  // Look up in opcode table to generate object code
            symtabSearch(operand);  // Look up in symbol table
            fprintf(output, "%s\t\t%s\t\t\t%s\t\t\t%s\t\t%s%s\n", locctr, label, opcode, operand,opc,addr);
            fprintf(obj, "^%s%s", opc,addr);  // Object code for other instructions
            counter+=3;  // Increment counter by 3 for each instruction
        }
        fscanf(imf, "%s\t%s\t%s\t%s", locctr, label, opcode, operand);  // Read next line
        
        if (counter >=size) {
            counter = 0;  // Reset counter
        }
    }
    fprintf(output, "%s\t\t%s\t\t\t%s\t\t\t%s\n", locctr, label, opcode, operand);  // Assembly listing last line
    fprintf(obj, "\nE^00%s\n", start);  // End record
    // Close files
    fclose(output);
    fclose(imf);
    fclose(obj);
    return 0;
}

// Placeholder functions for symbol and operation table searches
int symtabSearch(char *label) {
    // Implementation here to search for the symbol and return address
    FILE *f1;
    int flag=0;
    f1=fopen("symtab.txt","r");  //Input file
    char sym[s];
    do{
        fscanf(f1,"%s %s",sym,addr); //Reading the line from the file
        if (strcmp(sym, label) == 0) {
            //printf("Symbol found: %s\n", sym);
            flag=1;
            break;
        }
    }while(!feof(f1)); //Infinite loop
    rewind(f1);
    fclose(f1);
    if (flag==1) {
        return 1;
    }
    return 0;
}

int optabSearch(char *opcode) {
    // Implementation here to search for opcode and return machine code
    FILE *f2;
    int flag=0;
    f2=fopen("optab.txt","r");  //Input file
    char opd[s];
    do{
        fscanf(f2,"%s %s",opd,opc); //Reading the line from the file
        if (strcmp(opd, opcode) == 0) {
            flag=1;
            break;
        }
    }while(!feof(f2)); //Infinite loop
    rewind(f2);
    fclose(f2);
    if (flag==1) {
        return 1;
    }
    return 0;
}

void asciitoHex(char *operand,int a) {
    // Implementation here to convert ASCII to Hex
    strcpy(&hex, &operand[a]);
}
