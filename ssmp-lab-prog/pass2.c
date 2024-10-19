#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char fill[20], op[5], sym[10];


int optabSearch(char a[100]){
    FILE *optab;
    optab = fopen("optab.txt","r");
    char e[100], temp[100];
    temp[0] = '\0';
    fscanf(optab,"%s %s", e, op);
    while( strcmp(temp,e) != 0 ){
        strcpy(temp,e);
        if(strcmp(e, a) == 0){
            fclose(optab);
            return 1;
        }
        fscanf(optab,"%s %s", e, op);
    }
    fclose(optab);
    return 0;
}


int symtabSearch(char a[100]){
    FILE *symtab;
    symtab  = fopen("symtab.txt","r");
    char e[100], temp[100];
    fscanf(symtab,"%s %s", e, sym);
    while( strcmp(temp,e) != 0 ){
        strcpy(temp,e);
        
        if(strcmp(e, a) == 0){
            fclose(symtab);
            return 1;
        }
        fscanf(symtab,"%s %s", e, sym);
    }
    fclose(symtab);
    return 0;
}
int opCount(){
    FILE *imf = fopen("intermediate.txt","r");
    char a[100], b[100], c[100], d[10];
    int count = 0;
    fscanf(imf,"%s %s %s %s", d, a, b, c);
    while(strcmp(b,"LENGTH") != 0) {
        int tempInt = optabSearch(b);
        if( tempInt == 1 ){
            count++;
        }
        fscanf(imf,"%s %s %s %s", d, a, b, c);
    }
    fclose(imf);
    return count;
}
void filler(char a[100], int len, char chr) {
    int i;
    int length = strlen(a);
    length = len-length;
    for(i = 0; i < length; i++) {
        fill[i] = chr;
    }
    fill[i] = '\0';
}

void main() {
    FILE *output, *imf;
    int tempInt;
    char a[100], b[100], c[100], d[100], temp[100], start[6];
    
    output = fopen("output.txt","w");
    imf = fopen("intermediate.txt","r");
    
    fscanf(imf,"%x %s %s %s",&tempInt, a, b, c);
    filler(a,6,'_');
    fprintf(output,"H^%s%s^", a, fill);
    filler(c, 6, '0');
    fprintf(output,"%s%s^",fill,c);
    strcpy(start, c);
    fscanf(imf,"%x %s %s %s",&tempInt, a, b, c);
    while(strcmp(b,"LENGTH") != 0) { 
        fscanf(imf,"%s %s %s %s",temp, a, b, c);
    }
    filler(temp, 6, '0');
    fprintf(output,"%s%s",fill, temp);
    fclose(imf);
    
    imf = fopen("intermediate.txt","r");
    fscanf(imf,"%x %s %s %s",&tempInt, a, b, c); 
    filler(c,6,'0');
    tempInt = opCount();
    int divBy10 = tempInt/10; 
    int divRem = (tempInt % 10)*3; 
    fscanf(imf,"%s %s %s %s",d , a, b, c);
    int i = 0;
    int flag = 0; 
    int countForWord = 0;
    char strForWord[100];
    strForWord[0] = '\0';
    while(1) {
        if(flag == 0) { 
            
            if(i%10 == 0) {
                filler(d,6,'0');
                if(divBy10 > 0) {
                    fprintf(output,"\nT^%s%s^%x", fill, d, 30);
                    divBy10--;
                }else {
                    if((divRem/16) == 0) { 
                        fprintf(output,"\nT^%s%s^0%x", fill, d, divRem);
                    } else {
                        fprintf(output,"\nT^%s%s^%x", fill, d, divRem);
                    }
                }
            }
            if( optabSearch(b) == 1 ) {
                symtabSearch(c);
                if(strcmp(sym,"-") == 0) {
                    fprintf(output,"^%s0000", op);
                }else{
                    fprintf(output,"^%s%s", op, sym);
                }
            }else {
                fprintf(output,"^%s%s", op, sym);
            }
            if(strcmp(b,"RSUB") == 0){
                flag = 1;
                continue;
            }
        } else { 
            if(strcmp(b,"WORD") == 0) {
                if(countForWord == 0) {
                    filler(d,6,'0');
                    fprintf(output,"\nT^%s%s", fill, d);
                }
                filler(c,6,'0');
                strcat(strForWord, "^");
                strcat(strForWord, fill);
                strcat(strForWord, c);
                countForWord++;
            }else {
                if(countForWord > 0) {
                    int countForWorda = countForWord*3;
                    if((countForWorda/16) == 0) { 
                        fprintf(output,"^0%x", countForWorda);
                    } else {
                        fprintf(output,"^%x", countForWorda);
                    }
                    fprintf(output,"%s", strForWord);
                    strForWord[0] = '\0';
                    countForWord = 0;
                }
            }
        }
        if(strcmp(b,"LENGTH") == 0) break;
        fscanf(imf,"%s %s %s %s",d, a, b, c);  
        i++;  
    }
    
    filler(start ,6,'0');
    fprintf(output, "\nE^%s%s", fill, start);
}