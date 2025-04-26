#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#define TCP_PORT 4242
#define DEBUG_printf printf
#define BUF_SIZE 1024
#define POLL_TIME_S 5

#define WIFI_SSID "iSpan-R201"
#define WIFI_PASSWORD "66316588"

typedef struct TCP_SERVER_T_{
	struct tcp_pcb *server_pcb;
	struct tcp_pcb *client_pcb;
	volatile bool msg_ready;
	char buffer_recv[BUF_SIZE];
	int recv_len;
} TCP_SERVER_T;

TCP_SERVER_T* tcp_server_init(void);
err_t tcp_server_close(void *arg);
err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb);
void tcp_server_err(void *arg, err_t err);
err_t tcp_server_accept(void *arg, struct tcp_pcb *client_pcb, err_t err);
bool tcp_server_open(void *arg);


#endif
