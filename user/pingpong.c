#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc,int argv[]){
    int p[2];
    int q[2];
    char buf[8];

    pipe(p);//parent
    pipe(q);//child

    if(fork() == 0){
        int id = getpid();
        close(p[1]);
        read(p[0],buf,1);
        printf("%d: received ping\n",id);
        close(q[0]);
        write(q[1],buf,1);
        exit(0);
    }else{
        write(p[1],buf,1);
        int id = getpid();
        close(q[1]);
        read(q[0],buf,1);
        printf("%d: received pong\n",id);

    }
    exit(0);
}