#include <stdio.h>

int main(int argc, char const *argv[])
{   
    printf("\nPrinting the arguments passed : \n"); 
    for(int i=0;i<argc;i++){
        printf("%s\n",argv[i]);
    }
    return 0;
}

