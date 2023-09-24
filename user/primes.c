#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void check(int p[2]){

    //read first,it must be prime
    int first = 0;
    close(p[1]);
    read(p[0],&first,4);
    printf("prime %d\n",first);

    //create child pipe,注意每次我们都创建了pipe，但是不一定每次都fork了进程，应该也可以不用每次都创建pipe
    int p_new[2];
    pipe(p_new);

    int next = 0;
    //读到头了，不需要fork进程
    if(read(p[0],&next,4) == 0){
        close(p_new[0]);
        close(p_new[1]);
        return;
    }
    //需要fork进程
    if(fork() == 0){
        //child，现在p管道可以完全不用，全部关闭，p_new管道成为下一个调用的p管道，传入check
        close(p[0]);
        check(p_new);
    }else{
        do{
            //father，不需要读p_new，但是子进程check之后需要读，因此子进程不能close(p_new[0])
            close(p_new[0]);
            if( next%first!=0 ){
                write(p_new[1],&next,4);

            }
        }while(read(p[0],&next,4)!=0);

        close(p[0]);
        close(p_new[1]);
        wait(0);
    }
    return;
}


int main(){
    int num[34] = {};
    for(int i=2;i<=35;i++) num[i-2] = i;

    //创建第一个管道，写入初始数据
    int p[2];
    pipe(p);
    write(p[1],num,34*4);

    if(fork()==0){
        //child
        //close(p[1]);
        check(p);
    }else{
        //father
        close(p[0]);
        close(p[1]);
        wait(0);
    }


    exit(0);
}