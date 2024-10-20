#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define s 20
void main()
{
 char opcode[s],operand[s],label[s];
 int locctr=0,start,length,val,d;
 FILE *fp1,*fp2,*fp4,*fp3;
 fp1=fopen("file1.txt","r"); // input code
 fp2=fopen("optab.txt","r"); // optab datastructure file 
 fp3=fopen("symtab.txt","w"); // symtab to be created 
 fp4=fopen("intermediate.txt","w"); // output to be created as intermediate
 fscanf(fp1,"%s\t%s\t%x\t",label,opcode,&d);
if(strcmp(opcode,"START")==0){
  start=d;
  locctr=start;
  fprintf(fp4,"\t%s\t%s\t%x\n",label,opcode,d);
 }
 else;
while(strcmp(opcode, "END")!=0) {
  fscanf(fp1,"%s\t%s\t%s",label,opcode,operand);
  if(strcmp(label,"--")!=0)
   fprintf(fp3,"%x\t%s\n",locctr,label);
  fprintf(fp4,"%x\t%s\t%s\t%s\n", locctr,label,opcode,operand);
  if(strcmp(opcode,"WORD")==0){
   locctr+=3;
  }
  else if(strcmp(opcode,"RESW")==0){
   val=atoi(operand);
   locctr+=(3*val);
  }
  else if(strcmp(opcode,"RESB")==0){
   val=atoi(operand);
   locctr+=val;
  }
  else if(strcmp(opcode,"RSUB")==0){
   locctr++;
  }
  else if(strcmp(opcode,"BYTE")==0){
   if(operand[0]=='X' || operand[0]=='x')
   ++locctr;
   else{
   int len=strlen(operand);
   locctr+=len-3;
   }
  }
  else if(strcmp(opcode,"END")!=0)
  locctr+=3;
 }
 length = locctr-start;
 printf("The length of the code : %x\n", length);
 fclose(fp1);
 fclose(fp2);
 fclose(fp3);
 fclose(fp4);
}