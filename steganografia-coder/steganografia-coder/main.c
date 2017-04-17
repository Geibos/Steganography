/*
 * Copyright (c) Евгений Крамсаков.
 * СПбПУ, кафедра ИБКС, 2017.
 * Все права защищены.
 *
 * Copiright (c) Evgeniy Kramsakov.
 * SPbPU, IBKS, 2017.
 * All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE_OF_TEXT 24583 // 256*256 - sizeof pallete.


int coder(void);
int decoder(void);
int Q=0;
int main() {
    int c;
    do {
        printf("Choose action: \n1 - encoding;\n2 - decoding;\n3 - exit.\n");
        scanf("%d",&c);
        if (c==1) coder();
                if (c==2) decoder();
    } while (c!=3);
    return 0;
}

int coder() {
    int i=0, j=0, simbol=0, simbol_file=0, t=0, k=0, d=0;
    unsigned char c, s, f;
    int buf[8], text[8];
    FILE *input = fopen("input.bmp", "rb+");;
    FILE *output = fopen("output.bmp", "wb");
    FILE *message = fopen("input.txt", "r");
    if ((input == NULL) || (message == NULL))
    {
        printf("Error: can't open the file!\n");
        return 0;
    }
    
    
    fseek(input, 0, SEEK_END);
    simbol = (int) ftell(input);
    rewind(input);

    
    fseek(message, 0, SEEK_END);
    simbol_file = (int) ftell(message);
    rewind(message);
    
    
    if(simbol_file>MAX_SIZE_OF_TEXT){
        printf("Too much simbols in text\n");
        exit(1);
    }
    
    Q=simbol_file;
    while(simbol>1) {
        if(i>=1000 && i<(simbol_file-1)+1000) {
            while(i>=1000 && i<(simbol_file-1)+1000) {
                s= (unsigned char) getc(message);
                for(j=7; j>=0 ; j--) { /*загоняем символ из текста в массив*/
                    text[j]=s%2;
                    s= (unsigned char) (s/2);
                }
                t=7;
                k=0;
                while(t>=0) {
                    fread(&c,sizeof(unsigned char),1,input);
                    for(j=7; j>=0 ; j--){    /*загоняем символ из рисунка в массив*/
                        buf[j]=c%2;
                        c= (unsigned char) (c/2);
                    }
                    buf[7]=text[k];
                    for(j=0; j<=7; j++){
                        d= (int) (d+buf[j]*pow((double)2, (double)(7-j)));
                    }
                    c= (unsigned char) d;
                    fwrite(&c,sizeof(unsigned char),1,output);
                    t--;
                    simbol--;
                    k++;
                    d=0;
                }
                t=7;
                i++;
            }
        }
        else {
            fread(&c,sizeof(unsigned char),1,input);
            fwrite(&c,sizeof(unsigned char),1,output);
            simbol--;
            i++;
        }
    }
    fclose(message);
    fclose(input);
    fclose (output);
    printf("Written %d simbols\n", Q);
    return 0;
}


int decoder() {
    int i=0, j=0, simbol=0, simbol_file=0, t=0, k=0, d=0;
    unsigned char c;
    int buf[8], text[8];
    FILE *input;
    FILE *output;
    input=fopen("output.bmp","rb+");
    output=fopen("output.txt","w");
    if (input == NULL) {
        printf("Error: can't open the file!\n");
        return 0;
    }
    
    fseek(input, 0, SEEK_END);
    simbol = (int) ftell(input);
    rewind(input);
    
    simbol_file=Q;
    while(simbol>1) {
        if(i>=1000 && i<(simbol_file-1)+1000) {
            while(i>=1000 && i<(simbol_file-1)+1000){
                t=7;
                k=0;
                while(t>=0) {
                    fread(&c,sizeof(unsigned char),1,input);
                    for(j=7; j>=0 ; j--) {  /* загоняем символ из рисунка в массив*/
                        buf[j]=c%2;
                        c= (unsigned char) (c/2);
                    }
                    text[k]=buf[7];
                    t--;
                    simbol--;
                    k++;
                    d=0;
                } 	
                for(j=0; j<=7; j++) {
                    d= (int) (d+text[j]*pow((double)2, (double)(7-j)));
                }
                c= (unsigned char) d;
                fputc(c,output);
                t=7;
                i++;		
            }
        }
        else {
            fread(&c,sizeof(unsigned char),1,input); 
            simbol--;
            i++;
        }		
    }
    fclose(output);
    fclose(input);
    return 0;
} 
