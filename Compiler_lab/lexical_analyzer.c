#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>

char keywords[][10]={"int","main","return"};
char operators[]="+-*/=<>";
char seperators[]=",(){};";

FILE *f1;

void init_stdout_to_file(){
    freopen("output.txt","w",stdout);
}

bool is_identifier(char c){
    if(isdigit(c))
        return false;
    return true;
}

bool is_keyword(char *c){
    for(int i=0;i<(sizeof(keywords)/sizeof(keywords[0]));i++){
        if(strcmp(c,keywords[i])==0){
            return true;
        }
    }
    return false;
}

bool is_operator(char c){

    for(int i=0;i<strlen(operators);i++){
        if(c==operators[i]){
            return true;
        }
    }
    return false;
}

bool is_seperator(char c){
    for(int i=0;i<strlen(seperators);i++){
        if(seperators[i]==c){
            return true;
        }
    }
    return false;
}

void tokenize(){
    char c;
    init_stdout_to_file();
    while(!feof(f1)){
        char buff[50];
        c=getc(f1);
        if(isspace(c)){
            continue;
        }
        else if(c=='#'){
            printf("(#, preprocessor directive)\n");
            printf("(");
            do{
                c=getc(f1);
                if(isalnum(c)||c=='_')
                    printf("%c",c);
            }while(isalnum(c)||c=='_');
            printf(",preprocessor instruction)\n");
            ungetc(c,f1);
        }
        else if(is_operator(c)){
            switch(c){
                case '=':
                    printf("(%c, Assign)\n",c);
                    break;
                default:
                    printf("(%c, Operator)\n",c);
                    break;
            }
            
        }
        else if(is_seperator(c)){
            printf("('%c', Seperator)\n",c);
        }else if(isalpha(c)||c=='_'){
           // printf("gets in here\n");
            ungetc(c,f1);
           // printf("reaches here unget\n");
            int i=0;
            do{
                
                c=fgetc(f1);
                if(is_operator(c)||is_seperator(c) || isspace(c) ){
                    ungetc(c,f1);
                    break;
                }
                buff[i++]=c;
            }while(true);
            buff[i]='\0';
            //printf("reaches here\n");
            // int len=strlen(buff);
            // if(is_seperator(buff[len-1])){
            //     while(is_seperator(buff[len-1])){
            //         buff[len-1]='\0';
            //         len--;
            //     }
            // }
            if(is_keyword(buff)){
                printf("(%s, Keyword)\n",buff);
            }
            else{
                printf("(%s, Identifier)\n",buff);
            }
        }

    }

}

void main(){
    f1=fopen("Files/input.c","r");
    tokenize();
}