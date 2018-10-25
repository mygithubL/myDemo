#ifndef __CAM_H__
#define	__CAM_H__

#define JPG_MAX_SIZE	(128 * 1024)
struct jpg_buf_t {
	unsigned char jpg_buf[JPG_MAX_SIZE - sizeof(unsigned int)];
	unsigned int jpg_size;
};

int camera_init(char *devpath, unsigned int *width, unsigned int *height, unsigned int *size, unsigned int *ismjpeg);
int camera_start(int fd);
int camera_dqbuf(int fd, void **buf, unsigned int *size, unsigned int *index);
int camera_eqbuf(int fd, unsigned int index);
int camera_stop(int fd);
int camera_exit(int fd);

void *thread_cam(void *arg);

#endif
