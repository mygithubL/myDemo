#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "camera.h"
#include "srv_qt.h"

pthread_t cam_tid;
pthread_t srv_tid;

pthread_mutex_t cam_mutex;

extern struct jpg_buf_t jpg;

int main(int argc, char *argv[])
{
	int ret;

	pthread_mutex_init(&cam_mutex, NULL);

	/* camera thread */
	ret = pthread_create(&cam_tid, NULL, thread_cam, NULL);
	if (ret) {
		errno = ret;
		perror("create camera thread");
		exit(EXIT_FAILURE);
	} else
		printf("create camera thread success\n");

	ret = pthread_detach(cam_tid);
	if (ret) {
		errno = ret;
		perror("detach camera thread");
		exit(EXIT_FAILURE);
	} else
		printf("detach camera thread success\n");

	sleep(3);
	
	/* 通信模块 thread */
	ret = pthread_create(&srv_tid, NULL, thread_srv, NULL);
	if (ret) {
		errno = ret;
		perror("create qt server thread\n");
		exit(EXIT_FAILURE);
	} else
		printf("create qt server thread success\n");

	ret = pthread_detach(srv_tid);
	if (ret) {
		errno = ret;
		perror("detach qt server thread\n");
		exit(EXIT_FAILURE);
	} else
		printf("detach qt server thread success\n");

	/* main thread, process environment data form m0 board or process short message */
	while (1) {
		sleep(1);
	}

	pthread_mutex_destroy(&cam_mutex);

	exit(EXIT_SUCCESS);
}
