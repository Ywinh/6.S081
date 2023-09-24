#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char*path){
    //找到最后的name
    char*p;
    static char buf[DIRSIZ+1];

    for(p=path+strlen(path);p>=path && *p!='/';p--)
        ;
    p++;

    //if(strlen(p)>=DIRSIZ)
        return p;

  // Return blank-padded name.为什么这样做？
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));//什么意思
    return buf;
}


void 
find(char *path,char*file){
    //在path这个dir下寻找file，如果fmt(path)就是file则终止

    char buf[512],*p;
    int fd;
    struct stat st;
    struct dirent de;

    if( (fd = open(path,0)) < 0 ){
        fprintf(2,"find: cannot open %s\n",path);
        return;
    }

    if( fstat(fd,&st)<0 ){
        fprintf(2,"find: cannot stat %s\n",&path);
        return;
    }

    switch(st.type){
        case T_FILE:
            if(strcmp(fmtname(path),file) == 0)
                printf("%s\n",path);
            break;//这个地方还必须是break，而不是return，为什么
        case T_DIR:
            strcpy(buf,path);
            p = buf + strlen(buf);
            //p = path + strlen(path)为什么错误
            *p++ = '/';
            while(read(fd,&de,sizeof(de)) == sizeof(de)){//这个地方确实是循环，因为我们不在"." ".."这两个目录中递归，需要循环读取
                if(de.inum == 0 || strcmp(de.name,".")==0 || strcmp(de.name,"..")==0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0; 

                //递归
                //这个地方不能是find(path,file)，为什么？
                //答，因为我们是对path进行strcpy得到的buf，然后一直对buf操作
                find(buf,file);              
            }
            break;       
    }
    close(fd);
    return;
}


int main(int argc,char *argv[]){
    //应该有几个参数？
    //3个参数，argv[0],argv[1]:dir,argv[2]:target file
    //如果只有两个参数，那么默认在当前目录寻找
    if( argc < 2 ){
        fprintf(2,"parameter error");
        exit(0);
    }
    if( argc == 2 ){
        find(".",argv[1]);
        exit(0);
    }

    find(argv[1],argv[2]);
    exit(0);

}