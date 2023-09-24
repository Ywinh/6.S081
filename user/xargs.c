#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int 
main(int argc,char *argv[]){
    
    char* argv_new[MAXARG];
    //copy old args,舍弃argv_new[0]
    for(int i=1;i<argc;i++){
        argv_new[i-1] = argv[i];
    }

    char c = '1';

    while(read(0,&c,1)){
        //从标准输入中读取一行，空格代表其中一个参数的结束，\n表示下一行
        int i = argc-1;
        int len = 0; 
        char buf[1000];
        char *p;
        p = buf;
        //p是一个指针，不断指向每一个字符串的开始

        //read a line
        do{
            if(c == '\n' || c == ' '){
                //都要先结算p字符串
                buf[len++] = 0;
                argv_new[i++] = p;
                p = buf + len;
                if(c == '\n'){
                    if(fork() == 0){
                        exec(argv_new[0],argv_new);
                        exit(1);
                    }else{
                        wait(0);
                        break;
                    }
                }
            }else{   
                buf[len++] = c;
            }
            
        }while(read(0,&c,1));

        
    }   





    exit(0);
}