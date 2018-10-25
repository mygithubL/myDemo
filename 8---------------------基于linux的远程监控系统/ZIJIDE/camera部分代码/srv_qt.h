#ifndef __SRV_QT__
#define __SRV_QT__

void *thread_srv(void *arg);
int mywrite(int fd, char *buf, int size);
int myread(int fd, char *buf, int size);
void process_qt(int connfd);

#endif
