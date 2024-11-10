#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define s 20

void main()
{
  char opcode[s],operand[s],label[s];
  int locctr=0,start,length,val,d;
  FILE *fp1,*fp2,*fp4,*fp3;
  fp1 = fopen("file1.txt", "r");  // input code
  fp2 = fopen("optab.txt", "r");  // optab datastructure file 
  fp3 = fopen("symtab.txt", "w+"); // symtab to be created (using "w+" for read and write)
  fp4 = fopen("intermediate.txt", "w"); // output to be created as intermediate
  fscanf(fp1, "%s\t%s\t%x\t", label, opcode, &d);
  if (strcmp(opcode, "START") == 0) {
    start = d;
    locctr = start;
    fprintf(fp4, "\t\t%s\t\t%s\t\t%x\n", label, opcode, d);
  } else {
    locctr = 0;
  }

  while (strcmp(opcode, "END") != 0) {
    fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);
    if (strcmp(label, "--") != 0) {
      int flag = 0, addr;
      char sym[10];

      while (!feof(fp3)) {
        fscanf(fp3, "%s\t%x", sym, &addr);
        if (strcmp(label, sym) == 0) {
          flag = 1;
          break;
        }
      }
      if (flag == 1) {
        //printf("Duplicate symbol\n");
        exit(1);
      } else {
        fprintf(fp3, "%s\t%x\n", label, locctr);
      }
      rewind(fp3);  // Reset file pointer for each new symbol search
    }

    fprintf(fp4, "%x\t\t%s\t\t%s\t\t%s\n", locctr, label, opcode, operand);

    if (strcmp(opcode, "WORD") == 0) {
      locctr += 3;
    } else if (strcmp(opcode, "RESW") == 0) {
      val = atoi(operand);
      locctr += (3 * val);
    } else if (strcmp(opcode, "RESB") == 0) {
      val = atoi(operand);
      locctr += val;
    } else if (strcmp(opcode, "RSUB") == 0) {
      locctr++;
    } else if (strcmp(opcode, "BYTE") == 0) {
      if (operand[0] == 'X' || operand[0] == 'x')
        ++locctr;
      else {
        int len = strlen(operand);
        locctr += len - 3;
      }
    } else if (strcmp(opcode, "END") != 0) {
      locctr += 3;
    }
  }

  length = locctr - start;
  printf("The length of the code : %x\n", length);

  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
  fclose(fp4);
}
