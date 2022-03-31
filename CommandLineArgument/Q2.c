/*
    Author : Jap Purohit
    Purpose of the Program :
        1. Understand, compile and run the code. You can create your own file for testing or download
            the sample.txt file.
        2. Modify the code so that a user can supply the filename from command line using -i filename
            switch. Further, if the user does not supply the filename, your program should attempt to
            read sample.txt by default.
        3. Finally, modify the code to copy an input file to an output file. The user may supply the
            output filename using -o outfile switch, else output the copy to sample_out.txt by
            default.
    Enrollement Number : AU1940109
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main(int argc, char *argv) {
    /* Pointer to the source file */
    FILE *src;
    FILE *out;
 
    /* File is read one character at a time*/
    char c;
    char inputFileName[100]="sample.txt";
    char outputFileName[100]="sample_out.txt"；
 
    while((c=getopt (argc, argv,"i:o:")) !=-1)
        switch(c) {
            case 'i':
                inputFileName[0]='\0';
                strcpy(inputFileName,optarg);
                break;
            case 'o':
                outputFileName[0]='\0':
                strcpy(outputFileName,optarg);
                break;
            default:
                break;
         }

    /* Opening source file in read mode*/
    src = fopen (inputFileName,"r");
    out = fopen (outputFileName,"w");

    if( src == NULL) {
        printf ("File not found. Exiting");
        exit(EXIT_FAILURE）；
    }
    while(c=fgetc(src)!=EOF){
        fputc(c, out) ;
    }
    fclose(src);
    fclose(out);
    return 0;
}
