#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "py_exec.h"

void py_exec()
{
    /* 実行ファイルのパス */
    char passbuf[] = "python3 ./python/main.py";
    while(1){
        system(passbuf);
        sleep(1);
    }
    
    pthread_exit(0);
}
