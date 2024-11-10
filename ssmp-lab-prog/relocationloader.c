#include<stdio.h>
#include<stdlib.h>
FILE *f1,*f2;
char line[100],hexaddr[20]; //line is used to store the line read from the file, hexaddr is used to store the address in hexadecimal
int i,addr,startaddr; //i is used as a counter, addr is used to store the address in decimal and startaddr is used to store the starting address

int main()
{
    f1=fopen("object.txt","r");  //Input file
    f2=fopen("output2.txt","w");    //Output file
    fprintf(f2,"Memory Address\t Object Code\n"); //Header for neatness
    fprintf(f2,"---------------\t-----------\n"); //Header for neatness
    printf("Enter the starting address: "); 
    scanf("%x", &startaddr); //Taking the starting address as input
    do{
        fscanf(f1,"%s",line); //Reading the line from the file
        if (line[0] == 'T') { //If the line is a text record
            for (i = 1; i < 7; i++) {
                hexaddr[i - 1] = line[i]; //Extracting the address from the line
            }
            sscanf(hexaddr, "%x", &addr);
            for (i = 9; line[i] != '\0'; i += 2) {
                fprintf(f2, "%04x\t\t\t\t%c%c\n", startaddr+addr, line[i], line[i + 1]); //Printing the address and the object code with the starting address added
                addr++;
            }
        }
        else if (line[0] == 'H') {
            continue; //If the line is a header record, it will be skipped
        }
        else if (line[0] == 'E') {
            break;  //If the line is an end record, the program will break
        }else{
            break; //If the file is not in the correct format, the program will break
        }
    }while(1); //Infinite loop
    fclose(f1);
    fclose(f2);
}