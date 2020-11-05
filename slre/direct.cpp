/**
 * программисты придумали Stack Overflow (С) Soer
*/
#include "direct.hpp"
#include <stdio.h>
#include <stdlib.h>
//*****************************************************************
#ifdef _WIN32

/* Compiling for Windows */

#include <windows.h>

char* list_dir(const char* direction)
{  
    char* retval = (char*)malloc(1024);

    WIN32_FIND_DATA f;
    HANDLE h = FindFirstFile("./*", &f);
    if(h != INVALID_HANDLE_VALUE)
    {
        do{
            sprintf(retval,"%s%s\n",retval,f.cFileName);
           // puts(f.cFileName);

        }while(FindNextFile(h, &f));
    }else{
        fprintf(stderr, "Error opening directory\n");
    }
    return retval;
}
//*****************************************************************
#elif __unix__

/* Compiling for UNIX / POSIX */

#include <sys/types.h>
#include <dirent.h>


char* list_dir(const char* direction)
{   
    char* retval=(char*)malloc(1024);

    DIR *dir = opendir(direction);

    if(dir)
    {
        struct dirent *ent;
        while((ent = readdir(dir)) != 0L)
        {
            sprintf(retval,"%s%s\n",retval,ent->d_name);
        }
    }else{
        fprintf(stderr, "Error opening directory\n");
    }

    closedir (dir);
    return retval;
    //free(retval);
}
//*****************************************************************

#elif __TURBOC__

/* Compiling for MS-DOS */

#include <dir.h>

char* list_dir(const char* direction)
{   
    char* retval=(char*)malloc(1024);
    struct ffblk ffblk;
    if(findfirst("*.*", &ffblk, 0) == 0)
    {
        do{
            sprintf(retval,"%s%s\n",retval,ffblk.ff_name);
          //  puts(ffblk.ff_name);    
            
          } while(findnext(&ffblk) == 0);
    }
    else
    {
        fprintf(stderr, "Error opening directory\n");
    }
    return retval;
}
//*****************************************************************
#else
#error Unsupported Implementation
#endif

#include <string.h>

bool find_name(const char* direction,const char* name){

    bool retval=false;
    //составляем список директории
    char* list = list_dir(direction);

    //append \n
    char* inwork_name=(char*)malloc(strlen(name)+1);
    sprintf(inwork_name,"%s\n",name);

    if(strstr(list,inwork_name))
    retval=true;

    free(list);
    free(inwork_name);
    printf("%i\n",retval);
    return retval;
}

bool touch(const char* path){
    bool retval=false;
    FILE* stream=fopen(path,"rb");
    if (stream){
        retval=true;
        fclose(stream);
    }
    
    return retval;
}
// int main(){
//     wchar_t g=0;
//     if(find_name("./","blabla"))
//     puts("ok");
//     if(find_name("./",(char*)U"direct.cpp"))
//     puts("ok1");
// }