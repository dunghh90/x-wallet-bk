/* This work, "xvcServer.c", is a derivative of "xvcd.c" (https://github.com/tmbinc/xvcd)
* by tmbinc, used under CC0 1.0 Universal (http://creativecommons.org/publicdomain/zero/1.0/).
* "xvcServer.c" is licensed under CC0 1.0 Universal (http://creativecommons.org/publicdomain/zero/1.0/)
* by Avnet and is used by Xilinx for XAPP1251.
*
*  Description : XAPP1251 Xilinx Virtual Cable Server for Linux
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <pthread.h>
#include <limits.h>					/* @Fujitsu */
#include <signal.h>					/* @Fujitsu */

#undef USE_IOCTL		/* ioctlは使用しない */

#ifndef USE_IOCTL
#define MAP_SIZE    0x10000
#define UIO_PATH    "/dev/mem"		/* @Fujitsu */

typedef struct {
    uint32_t length_offset;
    uint32_t tms_offset;
    uint32_t tdi_offset;
    uint32_t tdo_offset;
    uint32_t ctrl_offset;
} jtag_t;
#else /* USE_IOCTL */
#include <sys/ioctl.h>
#include "xvc_ioctl.h"
#define CHAR_DEV_PATH   "/dev/xilinx_xvc_driver"
#endif /* !USE_IOCTL */

static int verbose = 0;

#define XVC_PORT 2542

static int sread(int fd, void *target, int len) {
    unsigned char *t = target;
    while (len) {
        int r = read(fd, t, len);
        if (r <= 0)
            return r;
        t += r;
        len -= r;
    }
    return 1;
}

#ifndef USE_IOCTL
int handle_data(int fd, volatile jtag_t* ptr) {
#else /* USE_IOCTL */
int handle_data(int fd, int fd_ioctl) {
#endif /* !USE_IOCTL */
    char xvcInfo[32];
    unsigned int bufferSize = 2048;

    sprintf(xvcInfo, "xvcServer_v1.0:%u\n", bufferSize);

    do {
        char cmd[16];
        unsigned char buffer[bufferSize], result[bufferSize / 2];
        memset(cmd, 0, 16);

        if (sread(fd, cmd, 2) != 1)
            return 1;

        if (memcmp(cmd, "ge", 2) == 0) {
            if (sread(fd, cmd, 6) != 1)
                return 1;
            memcpy(result, xvcInfo, strlen(xvcInfo));
            if (write(fd, result, strlen(xvcInfo)) != strlen(xvcInfo)) {
                perror("write");
                return 1;
            }
            if (verbose) {
                printf("%u : Received command: 'getinfo'\n", (int)time(NULL));
                printf("\t Replied with %s\n", xvcInfo);
            }
            break;
        } else if (memcmp(cmd, "se", 2) == 0) {
            if (sread(fd, cmd, 9) != 1)
                return 1;
            memcpy(result, cmd + 5, 4);
            if (write(fd, result, 4) != 4) {
                perror("write");
                return 1;
            }
            if (verbose) {
                printf("%u : Received command: 'settck'\n", (int)time(NULL));
                printf("\t Replied with '%.*s'\n\n", 4, cmd + 5);
            }
            break;
        } else if (memcmp(cmd, "sh", 2) == 0) {
            if (sread(fd, cmd, 4) != 1)
                return 1;
            if (verbose) {
                printf("%u : Received command: 'shift'\n", (int)time(NULL));
            }
        } else {
            fprintf(stderr, "invalid cmd '%s'\n", cmd);
            return 1;
        }

        int len;
        if (sread(fd, &len, 4) != 1) {
            fprintf(stderr, "reading length failed\n");
            return 1;
        }

        int nr_bytes = (len + 7) / 8;
        if (nr_bytes * 2 > sizeof(buffer)) {
            fprintf(stderr, "buffer size exceeded\n");
            return 1;
        }

        if (sread(fd, buffer, nr_bytes * 2) != 1) {
            fprintf(stderr, "reading data failed\n");
            return 1;
        }
        memset(result, 0, nr_bytes);

        if (verbose) {
            printf("\tNumber of Bits  : %d\n", len);
            printf("\tNumber of Bytes : %d \n", nr_bytes);
            printf("\n");
        }

#ifndef USE_IOCTL
        int bytesLeft = nr_bytes;
        int bitsLeft = len;
        int byteIndex = 0;
        int tdi = 0;
        int tms = 0;
        int tdo = 0;

        while (bytesLeft > 0) {
            int shift_num_bytes;
            int shift_num_bits = 32;
            tms = 0;
            tdi = 0;
            tdo = 0;

            if (bytesLeft < 4) {
                shift_num_bits = bitsLeft;
            }
            shift_num_bytes = (shift_num_bits + 7) / 8;

            memcpy(&tms, &buffer[byteIndex], shift_num_bytes);
            memcpy(&tdi, &buffer[byteIndex + nr_bytes], shift_num_bytes);

            ptr->length_offset = shift_num_bits;
            ptr->tms_offset = tms;
            ptr->tdi_offset = tdi;
            ptr->ctrl_offset = 0x01;

            /* Switch this to interrupt in next revision */
            while (ptr->ctrl_offset) {}

            tdo = ptr->tdo_offset;
            memcpy(&result[byteIndex], &tdo, shift_num_bytes);

            bytesLeft -= shift_num_bytes;
            bitsLeft -= shift_num_bits;
            byteIndex += shift_num_bytes;

            if (verbose) {
                printf("LEN : 0x%08x\n", shift_num_bits);
                printf("TMS : 0x%08x\n", tms);
                printf("TDI : 0x%08x\n", tdi);
                printf("TDO : 0x%08x\n", tdo);
            }
        }
#else /* USE_IOCTL */
        struct xil_xvc_ioc xvc_ioc;

        xvc_ioc.opcode = 0x01;
        xvc_ioc.length = len;
        xvc_ioc.tms_buf = buffer;
        xvc_ioc.tdi_buf = buffer + nr_bytes;
        xvc_ioc.tdo_buf = result;

        if (ioctl(fd_ioctl, XDMA_IOCXVC, &xvc_ioc) < 0) {
            int errsv = errno;
            fprintf(stderr, "xvc ioctl error: %s\n", strerror(errsv));
            return errsv;
        }
#endif /* !USE_IOCTL */
        if (write(fd, result, nr_bytes) != nr_bytes) {
            perror("write");
            return 1;
        }
    } while (1);

    /* Note: Need to fix JTAG state updates, until then no exit is allowed */
    return 0;
}

#ifdef USE_IOCTL
void display_driver_properties(int fd_ioctl) {
    int ret = 0;
    struct xil_xvc_properties props;

    ret = ioctl(fd_ioctl, XDMA_RDXVC_PROPS, &props);
    if (ret < 0) {
        perror("failed to read XVC driver properties");
        return;
    }

    printf("INFO: XVC driver character file: %s\n", CHAR_DEV_PATH);
    printf("INFO: debug_bridge base address: 0x%lX\n", props.debug_bridge_base_addr);
    printf("INFO: debug_bridge size: 0x%lX\n", props.debug_bridge_size);
    printf("INFO: debug_bridge device tree compatibility string: %s\n\n", props.debug_bridge_compat_string);
}
#endif /* USE_IOCTL */

#ifndef USE_IOCTL	/* @Fujitsu */
static void func_debugBridgeOn(void) {
	int fd;
	volatile void* ptr = NULL;
	unsigned val;

	if(( fd = open(UIO_PATH, O_RDWR)) < 1 ){
		fprintf(stderr, "open error \n");
		return;
	}
	if( (ptr = (volatile void*) mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0xA0000000)) == MAP_FAILED) {
		fprintf(stderr, "MMAP Failed\n");
		close(fd);
		return;
	}

	/* 0xA0000FC0 read modify */
	val = *((unsigned *)(ptr + 0xFC0));
	*((unsigned *)(ptr + 0xFC0)) = (unsigned)(val | 0x1);

	munmap((void *)ptr, 0x1000);
	close(fd);

	fprintf(stdout, "Debug Bridges Mode Modify(0x%08x -> 0x%08x) \n", val,val | 0x1 );
	return;
}

static void func_debugBridgeOff(int printflg) {
	int fd;
	volatile void* ptr = NULL;
	unsigned val;

	if(( fd = open(UIO_PATH, O_RDWR)) < 1 ){
		fprintf(stderr, "open error \n");
		return;
	}
	if( (ptr = (volatile void*) mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0xA0000000)) == MAP_FAILED) {
		fprintf(stderr, "MMAP Failed\n");
		close(fd);
		return;
	}

	/* 0xA0000FC0 read modify */
	val = *((unsigned *)(ptr + 0xFC0));
	*((unsigned *)(ptr + 0xFC0)) = (unsigned)(val & 0xFFFFFFFE);

	munmap((void *)ptr, 0x1000);
	close(fd);

	if(printflg != 0){
		fprintf(stdout, "Debug Bridges Mode Modify(0x%08x -> 0x%08x) \n", val,val & 0xFFFFFFFE );
	}
	return;
}

static void func_signalCatch(int no_sig)
{
	switch(no_sig){
		case SIGSEGV:
		case SIGBUS:
		case SIGFPE:
		case SIGHUP:
		case SIGILL:
		case SIGTERM:
		case SIGABRT:
		case SIGTSTP:
		case SIGQUIT:
		case SIGPIPE:
			func_debugBridgeOff(0);
			exit(EXIT_FAILURE);
			break;
		default:
			break;
	}
	return;
}

static void func_signalEntry(void ){
	signal(SIGSEGV,func_signalCatch);
	signal(SIGBUS,func_signalCatch);
	signal(SIGFPE,func_signalCatch);
	signal(SIGHUP,func_signalCatch);
	signal(SIGILL,func_signalCatch);
	signal(SIGINT ,func_signalCatch);
	signal(SIGTERM,func_signalCatch);
	signal(SIGABRT,func_signalCatch);
	signal(SIGTSTP,func_signalCatch);
	signal(SIGQUIT,func_signalCatch);
	signal(SIGURG ,func_signalCatch);
	signal(SIGPIPE,func_signalCatch);
	return;
}

#endif /* USE_IOCTL */


int main(int argc, char **argv) {
    int i;
    int s;
    int c;
    struct sockaddr_in address;
    char hostname[256];

#ifndef USE_IOCTL
    int fd_uio;
    volatile jtag_t* ptr = NULL;

	/* @Fujitsu */
    opterr = 0;
	unsigned long map_addr = 0;
				
    while ((c = getopt(argc, argv, "vm:")) != -1) {
        switch (c) {
            case 'v':
                verbose = 1;
                break;
            case 'm':
				map_addr = strtoul(optarg, NULL, 16);
				if ((ULONG_MAX == map_addr) && (ERANGE == errno)){
					fprintf(stderr, "JTAGM Address Input error: 0x%ld \n", map_addr);
					return -1;
				}
				if((map_addr & 0xFFFFFFFF00000000) != 0){
					fprintf(stderr, "JTAGM Address range over: 0x%ld \n", map_addr);
					return -1;
				}
				break;
            case '?':
                fprintf(stderr, "usage: %s [-v] -m <JTAGM Address>\n", *argv);
                return 1;
        }
    }

	if(map_addr == 0){
		fprintf(stderr, "JTAGM Address range error: 0x%lx \n", map_addr);
		return -1;
	}
	/* @Fujitsu */

    fd_uio = open(UIO_PATH, O_RDWR);
    if (fd_uio < 1) {
        fprintf(stderr, "Failed to open uio: %s\n", UIO_PATH);
        return -1;
    }

    ptr = (volatile jtag_t*) mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_uio, (off_t)map_addr);	/* @Fujitsu */
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "MMAP Failed\n");
        close(fd_uio);
        return -1;
    }
	
	fprintf(stdout, "Mapped JTAGM Address :0x%lx to Virtual address:0x%p \n", map_addr,(void*)ptr);	/* @Fujitsu */

	func_debugBridgeOn();	/* @Fujitsu */

	func_signalEntry();	/* @Fujitsu */

	fflush(stdout);	/* @Fujitsu */
	
#else /* USE_IOCTL */
    int fd_ioctl;

    fd_ioctl = open(CHAR_DEV_PATH, O_RDWR | O_SYNC);
    if (fd_ioctl < 1) {
        fprintf(stderr, "Failed to open xvc ioctl device driver: %s\n", CHAR_DEV_PATH);
        return -1;
    }

    display_driver_properties(fd_ioctl);
#endif /* !USE_IOCTL */

    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0) {
        perror("socket");
#ifndef USE_IOCTL
        close(fd_uio);	/* @Fujitsu */
#endif
        return 1;
    }

    i = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &i, sizeof i);

    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(XVC_PORT);
    address.sin_family = AF_INET;

    if (bind(s, (struct sockaddr*) &address, sizeof(address)) < 0) {
        perror("bind");
        close(s);
#ifndef USE_IOCTL
        close(fd_uio);	/* @Fujitsu */
#endif
        return 1;
    }

    if (listen(s, 5) < 0) {
        perror("listen");
        close(s);
#ifndef USE_IOCTL
        close(fd_uio);	/* @Fujitsu */
#endif
        return 1;
    }

    if (gethostname(hostname, sizeof(hostname)) != 0) {
        perror("hostname lookup");
        close(s);
#ifndef USE_IOCTL
        close(fd_uio);	/* @Fujitsu */
#endif
        return 1;
    }

    printf("INFO: To connect to this xvcServer instance, use url: TCP:%s:%u\n\n", hostname, XVC_PORT);

    fd_set conn;
    int maxfd = 0;

    FD_ZERO(&conn);
    FD_SET(s, &conn);

    maxfd = s;

    while (1) {
        fd_set read = conn, except = conn;
        int fd;

        if (select(maxfd + 1, &read, 0, &except, 0) < 0) {
            perror("select");
            break;
        }

        for (fd = 0; fd <= maxfd; ++fd) {
            if (FD_ISSET(fd, &read)) {
                if (fd == s) {
                    int newfd;
                    socklen_t nsize = sizeof(address);

                    newfd = accept(s, (struct sockaddr*) &address, &nsize);

                    printf("connection accepted - fd %d\n", newfd);
                    if (newfd < 0) {
                        perror("accept");
                    } else {
                        printf("setting TCP_NODELAY to 1\n");
                        int flag = 1;
                        int optResult = setsockopt(newfd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));
                        if (optResult < 0)
                            perror("TCP_NODELAY error");
                        if (newfd > maxfd) {
                            maxfd = newfd;
                        }
                        FD_SET(newfd, &conn);
                    }
#ifndef USE_IOCTL
                } else if (handle_data(fd, ptr)) {
#else /* USE_IOCTL */
                } else if (handle_data(fd, fd_ioctl)) {
#endif /* !USE_IOCTL */
                    printf("connection closed - fd %d\n", fd);
                    close(fd);
                    FD_CLR(fd, &conn);
                }
            } else if (FD_ISSET(fd, &except)) {
                printf("connection aborted - fd %d\n", fd);
                close(fd);
                FD_CLR(fd, &conn);
                if (fd == s)
                    break;
            }
        }
    }

#ifndef USE_IOCTL
    munmap((void *) ptr, MAP_SIZE);
    close(fd_uio);	/* @Fujitsu */
	func_debugBridgeOff(1);	/* @Fujitsu */
	fflush(stdout);	/* @Fujitsu */

#else /* USE_IOCTL */
    close(fd_ioctl);
#endif /* !USE_IOCTL */

    return 0;
}
