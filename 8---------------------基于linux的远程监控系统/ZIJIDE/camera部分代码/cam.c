#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <jpeglib.h>
#include <pthread.h>

#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>
#include "camera.h"
#include "convert.h"

#define CAM_DEV_PATH	"/dev/video0"
struct jpg_buf_t jpg;
extern pthread_mutex_t cam_mutex;

void *thread_cam(void *arg)
{
	//完成摄像头初始化采集图片转换图片格式 YUYV --- rgb  --- jpeg
	/*
	初始化  camera_init		
	开始采集
	convert_rgb_to_jpg_init();
	
	while（1）{
		取出一张图片camera_dqbuf ---- yuyv
		yuyv  ----> rgb
		rgb  ----> jpeg
		投放数据缓冲区到队列
	}

	convert_rgb_to_jpg_exit();
	*/
	//初始化  camera_init	
	int i;
	int fd;
	int ret;
	unsigned int width;
	unsigned int height;
	unsigned int size;
	unsigned int index;
	char *yuv;
	char *rgb;
	unsigned int j;
	
	width = 640;
	height = 480;
	
	fd = camera_init(CAM_DEV_PATH, &width, &height, &size, &j);
	printf("cam :width-%d,height-%d\n",width,height);
	rgb = malloc(width * height * 3);
	
	ret = camera_start(fd);
	if(ret == -1)
		pthread_exit(NULL);
	convert_rgb_to_jpg_init();
	
	while(1)
	{
		ret = camera_dqbuf(fd,(void **)&yuv,&size,&index);
		if(ret == -1)
			pthread_exit(NULL);

		//yuyv  ----> rgb	
		convert_yuv_to_rgb(yuv,rgb,width,height,24);
		
		pthread_mutex_lock(&cam_mutex);
		
		//rgb  ----> jpeg
		jpg.jpg_size =  convert_rgb_to_jpg_work(rgb,jpg.jpg_buf,width,height,24,88);
		pthread_mutex_unlock(&cam_mutex);
		
		ret = camera_eqbuf(fd,index);
		//投放数据缓冲区到队列
		if(-1== ret)
			pthread_exit(NULL);
	};
	
	convert_rgb_to_jpg_exit();
	free(rgb);
}
