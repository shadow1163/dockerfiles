/*
	skyeye_uart_net.c - skyeye uart device from tcp port
	Copyright (C) 2003 - 2007 Skyeye Develop Group
	for help please send mail to <skyeye-developer@lists.gro.clinux.org>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/

/*
 * Only support linux.
 * 07/16/2009   Michael.Kang  <blackfin.kang@gmail.com>
 */

#include <sys/types.h>
#include <stdio.h>
#include "skyeye_uart.h"
#include "skyeye_pref.h"
#include "skyeye_thread.h"

#ifdef __linux__
#include <net/if.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <sys/socket.h>
/*
#ifdef __FreeBSD__
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_tun.h>
#include <fcntl.h>
#endif
*/
#include <stdlib.h>


/*
 * The structure used to represent the state of uart physical link.
 */
struct uart_link_state{
	struct termios* term;
	struct {
                uint8_t *bufp;  /* Non null if allocated */
                int     head;
                int     tail;
                int     count;
                int     size;
        } in;
	int     tty_skt;        /* socket to send / receive to client on */
        int  tty_attached;
        pthread_t pthread_id;
        pthread_mutex_t tty_lock;
        pthread_cond_t  tty_cv;
};

/* for debug purpose */
static void dump_buf(char * strp, uint8_t * bufp, int len)
{
	int i;

	printf("%s read %d bytes: ", strp, len);
	for(i=0; i<len; i++) printf("0x%02x [%c]",bufp[i],bufp[i]>=32 && bufp[i]<127 ? bufp[i] : '.');
	printf("\n");
}

static int create_term(char * hostname, int port){
	pid_t pid;
	char port_str[32];
	//char *Env_argv[] = { "/bin/ls", "-l", "-a", (char *) 0 };
	//char * argv[]={"/usr/bin/xterm", (char *)0};
	char * argv[]={"xterm","-e","./uart_instance","localhost", "2345"};
	sprintf(port_str, "%d", port);
	switch (pid = fork())
    	{
        	case -1:
        	{
            		perror("The fork failed!");
            		break;
        	}
		case 0:
		{
        	    	//printf("[child]connect to %s:%s!\n", hostname, port_str);
			int ret = execlp("xterm","xterm", "-e", "./uart_instance", hostname, port_str);
			perror("Child:");
			fprintf(stderr, "SKYEYE Error:We need xterm to run the console of uart.Please check if you installed it correctly.\n");
	        	_exit (-1);
        	}
		default:
			break;
	}
	return 0;
}

/* we will fork another process as the console daemon */
static int create_uart_console(struct uart_link_state * ul_state){
	uint8_t buf[1024];
#define	MAXHOSTNAME	256
	char	myhostname[MAXHOSTNAME];
	int sv_skt;
	struct hostent	* hp;
	int on, length;
	struct sockaddr_in server, from;
	char * froms;
	printf("In %s\n", __FUNCTION__);
	sv_skt = socket(AF_INET, SOCK_STREAM, 0);
	if (sv_skt < 0) SKYEYE_ERR("opening stream socket");

	/* enable the reuse of this socket if this process dies */
	if (setsockopt(sv_skt, SOL_SOCKET, SO_REUSEADDR, (uint8_t*)&on, sizeof(on))<0)
		SKYEYE_ERR("turning on REUSEADDR");

		/* bind it */
retry:
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;

	sky_pref_t* pref = get_skyeye_pref();
	server.sin_port = htons(pref->uart_port);	/* bind to an OS selected local port */

	if (bind(sv_skt, (struct sockaddr *)&server, sizeof(server)) < 0) {
		switch (errno) {
		case EAGAIN:
			goto retry;

		case EADDRINUSE:
			SKYEYE_ERR("Port is already in use\n");
		default:
			SKYEYE_ERR("binding tcp stream socket");
		}
	}

	length = sizeof(server);
	if (getsockname(sv_skt, (struct sockaddr *) &server, &length)==-1)
		SKYEYE_ERR("getting socket name");

	listen(sv_skt, 1);

	gethostname(myhostname, MAXHOSTNAME);
	//printf("In %s, before main loop\n", __FUNCTION__);
	/* Create the client xterm */
	//create_term(myhostname, ntohs(server.sin_port));
	/* main loop */
	do {
		if (!ul_state->tty_attached) {
			/*
			PRINTF(("\ndumbserial: Waiting for connection to : %s:%d\n",
			    myhostname, ntohs(ds_server.sin_port)));
			*/
			//printf("Waiting for connection to %s:%d", myhostname, ntohs(server.sin_port));
			length = sizeof(from);
			ul_state->tty_skt = accept(sv_skt, (struct sockaddr *)&from, (int*)&length);

			hp = gethostbyaddr((char *)&from.sin_addr, 4, AF_INET);
			if (hp == (struct hostent *)0) {
				froms = inet_ntoa(from.sin_addr);
				fprintf(stderr,"cant resolve hostname for %s\n", froms);
			} else {
				froms = hp->h_name;
			}
			/*
			PRINTF(("dumbserial: Accepted connection on %s:%d from %s:%d\n",
			    myhostname, ntohs(ds_server.sin_port),
			    froms, ntohs(from.sin_port)));
			*/
			//printf("Accepted connection.\n");
			pthread_mutex_lock(&ul_state->tty_lock);
			ul_state->tty_attached = 1;
			pthread_mutex_unlock(&ul_state->tty_lock);
			pthread_cond_signal(&ul_state->tty_cv);
		} 
		else { /* begin receive data. */
			int res;
			struct pollfd fds;

			//printf("In %s, begin receive data\n", __FUNCTION__);
#define	POLL_TIMEOUT	-1		/* wait forever ? FIXME ? */
			fds.fd = ul_state->tty_skt;
			fds.events = POLLIN|POLLPRI;
#if HOST_OS_SOLARIS9		/* { */
			fds.events |= POLLRDNORM|POLLRDBAND;
#endif				/* } */
			fds.revents = 0;

			res = poll(&fds, 1, POLL_TIMEOUT);

			if (fds.revents & POLLIN) {
				res = read(ul_state->tty_skt, buf, sizeof (buf));
				if (res == 0) {
						/* a read of 0 bytes is an EOF */
					pthread_mutex_lock(&ul_state->tty_lock);
					ul_state->tty_attached = 0;
					pthread_mutex_unlock(&ul_state->tty_lock);
					pthread_cond_signal(&ul_state->tty_cv);
					close(ul_state->tty_skt);
					//SANITY(dsp->tty_skt = -1;);
				} else if (res<0) {
					perror("read");
				} else {
					int i;
					for (i = 0; i < res; i++) {
						if (ul_state->in.count < ul_state->in.size) {
							ul_state->in.bufp[ul_state->in.tail] = buf[i];
							ul_state->in.tail++;
							if (ul_state->in.tail >= ul_state->in.size)
								ul_state->in.tail = 0;
							ul_state->in.count++;
						} else {
							fprintf(stderr, "Overflow for uart link.\n");
						}
					}
				}
			}//if (fds.revents & POLLIN) 
		}
	} while (1);
	return 0;
}

static int init_ul_state(struct uart_link_state* ul_state){
	/* initilizaiton the rx fifo */
	ul_state->in.count = 0;
	ul_state->in.head = 0;
	ul_state->in.tail = 0;
	/* we allocate the 1024 byte for rx buffer  */
	ul_state->in.size = 1024;
	if(((ul_state->in.bufp = malloc(1024))) == NULL)
		return -1;
	
	ul_state->tty_skt = -1;
	ul_state->tty_attached = 0;
	pthread_mutex_init(&ul_state->tty_lock, NULL);
        pthread_cond_init(&ul_state->tty_cv, NULL);

	/* 
	 * create the management thread for the uart console,
	 * then wait for it tell us the console running before we return to 
	 * the simulator
	 */
	pthread_mutex_lock(&ul_state->tty_lock);
        create_thread(create_uart_console, (void*)ul_state, &(ul_state->pthread_id));
        /* should probably timeout here incase child doesn't start */
        while (!ul_state->tty_attached) pthread_cond_wait(&ul_state->tty_cv, &ul_state->tty_lock);
        pthread_mutex_unlock(&ul_state->tty_lock);

	return No_exp;
}

int uart_net_open(struct uart_device *uart_dev)
{
	int i,ret;
	struct uart_link_state * ul_state;

	/* FIXME, we should indicate meaningful error code for the failure of malloc */
	if((ul_state = malloc(sizeof(struct uart_link_state))) == NULL) return -1;

	uart_dev->priv = ul_state; /* the priv data used to save the internal uart link state */
	init_ul_state(ul_state);
	//ret=pthread_create(&id,NULL,(void *) create_uart_console,NULL);
	//create_uart_console(ul_state);
	return 0;
}


int uart_net_close(struct uart_device *uart_dev)
{
	if(uart_dev->priv != NULL)
	{
		struct uart_link_state *ul_state;
		ul_state = (struct uart_link_state *)uart_dev->priv;

		free(ul_state->in.bufp);
		ul_state->in.bufp = NULL;

		free(uart_dev->priv);
		uart_dev->priv = NULL;
	}
	return 0;
}


int uart_net_read(struct uart_device *uart_dev, void *buf, size_t count, struct timeval *timeout)
{
	int ret = -1;
	char* buf1 = buf;
	struct uart_link_state * ul_state = uart_dev->priv;
	if (ul_state->tty_attached) {
		/* move the charaters in buffer */
		int i = 0;
               //printf("In %s, count=%d, head=%d, tail=%d\n", __FUNCTION__, count, ul_state->in.head, ul_state->in.tail);
		while(i < count && (ul_state->in.head < ul_state->in.tail)){
			buf1[i] =  ul_state->in.bufp[ul_state->in.head];
			i++;
			ul_state->in.head++;
                       //printf("In %s, i=%d, count=%d, head=%d, tail=%d\n", __FUNCTION__, i, count, ul_state->in.head, ul_state->in.tail);
		}
               /* read the char expected */
		if((i == count) || (ul_state->in.head == ul_state->in.tail))
			ret = i;
	}
	return ret;
}


int uart_net_write(struct uart_device *uart_dev, void *buf, size_t count)
{
	int ret = -1;
	struct uart_link_state * ul_state = uart_dev->priv;
	if (ul_state->tty_attached) {
        	if((ret = write(ul_state->tty_skt, buf, count)) >= 0)
			return 0;
        }
	return ret;
}
#else
int uart_net_open(struct uart_device *uart_dev)
{
        return -1;
}

int uart_net_close(struct uart_device *uart_dev)
{
        return -1;
}

int uart_net_read(struct uart_device *uart_dev, void *buf, size_t count, struct timeval *timeout)
{
        return -1;
}

int uart_net_write(struct uart_device *uart_dev, void *buf, size_t count)
{
        return -1;
}
#endif //
