#include <mqueue.h>
#include <stdio.h>

int main() {

    mqd_t queue;
    struct mq_attr attr;
    attr.mq_maxmsg = 2;
    attr.mq_flags = O_NONBLOCK;
     
    queue = mq_open("prova", O_CREAT);

    if(queue < 0) {
        printf("error");
    }

    return 0;
}