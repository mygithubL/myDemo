/*===============================================================
*   Copyright (C) 2017 All rights reserved.
*   
*   文件名称：server.c
*   创 建 者：liujing
*   创建日期：2017年02月09日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>

void process_qt(int connfd);

int myread(int fd, char *buf, int size)
{
	int count = 0;
	int ret;
	while (count < size) {
		ret = read(fd, buf+count, size-count);
		count += ret;
	}
	return count;
}

int mywrite(int fd, char *buf, int size)
{
	int count = 0;
	int ret;
	while (count < size) {
		ret = write(fd, buf+count, size-count);
		count += ret;
	}
	return count;
}

int main()
{
	/*1. 创建套接字*/
	int sockfd;
	if (0 > (sockfd = socket(AF_INET, SOCK_STREAM, 0))) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	printf("socket success!\n");

	/*2. 绑定本机地址和端口*/
	struct sockaddr_in myaddr;
	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(8888);
	myaddr.sin_addr.s_addr = inet_addr("192.168.1.222");

	if (0 > bind(sockfd, (struct sockaddr*)&myaddr, sizeof(myaddr))) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
	printf("bind success!\n");

	/*3. 设置监听套接字*/
	if (0 > listen(sockfd, 1024)) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("listen.................\n");

	/*4. 接收客户端发来的连接请求*/
	int connfd;
	if (0 > (connfd = accept(sockfd, NULL, NULL))) {
		perror("accpet");
		exit(EXIT_FAILURE);
	}
	printf("accept success!\n");
	process_qt(connfd);

	close(connfd);
	close(sockfd);

	return 0;
}


//process_qt --- 判断客户端的请求是否为pic，如果是把本地图片内容读取到buf和picbuf， 先发长度，再发数据
void process_qt(int connfd)
{
	char order[5];  
	char buf[20];    //存放图片长度
	char picbuf[1024*1024];   //存放图片的数据
	int ret = 0;

	memset(order, 0, sizeof(order));
	ret = myread(connfd, order, sizeof(order));  //从客户端接收请求
	if (ret != sizeof(order)) {
		perror("read order");
		return;
	}
	printf("order: %s\n", order);

	//判断客户端的请求是否是pic
	if (strcmp(order, "pic") == 0) {
		printf("show pic\n");
		memset(picbuf, 0, sizeof(picbuf));
		memset(buf, 0, sizeof(buf));
		int fd = open("11.jpg", O_RDONLY);
		if (fd < 0) {
			perror("open");
			return;
		}
		printf("open success\n");

		//循环从11.jpg中读取图片数据，长度放入size，数据放入picbuf
		int size = 0;
		while (1) {
			ret = read(fd, picbuf+size, 1024);
			if (ret <= 0)
				break;
			size += ret;
		}
		printf("size = %d\n", size);
		sprintf(buf, "%d", size);   //size 是图片的长度    size:1234    buf: "1234"
		puts(buf);

		ret = mywrite(connfd, buf, sizeof(buf));   //图片长度
		if (ret != sizeof(buf)) {
			perror("write len");
			return;
		}
		ret = mywrite(connfd, picbuf, size);  //图片数据
		if (ret != size) {
			perror("write pic");
			return;
		}
	}
	while (1)
		sleep(1);
}
