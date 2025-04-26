#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pico/stdlib.h> 
#include "pico/cyw43_arch.h"

#include "tcp_server.h"

TCP_SERVER_T* tcp_server_init(void) {
    TCP_SERVER_T *state = calloc(1, sizeof(TCP_SERVER_T)); //return a pointer to the allocated space
    if (!state) {
        DEBUG_printf("failed to allocate state\n");
        return NULL;
    }
    return state;
}

err_t tcp_server_close(void *arg) {
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
    err_t err = ERR_OK;
    if (state->client_pcb != NULL) {
        tcp_arg(state->client_pcb, NULL);
        tcp_poll(state->client_pcb, NULL, 0);
        tcp_recv(state->client_pcb, NULL);
        tcp_err(state->client_pcb, NULL);
        err = tcp_close(state->client_pcb);
        if (err != ERR_OK) {
            DEBUG_printf("close failed %d, calling abort\n", err);
            tcp_abort(state->client_pcb); //強制立即中斷連線 
            err = ERR_ABRT;
        }
        state->client_pcb = NULL;
    }
    if (state->server_pcb) {
        tcp_arg(state->server_pcb, NULL);
        tcp_close(state->server_pcb);
        state->server_pcb = NULL;
    }
    return err;
}


err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
    
    //檢查接收到的資料包是否為NULL，是的話就釋放資源
	if (!p) {
        DEBUG_printf("Client closed the connection\n");
        
        if (state->client_pcb){
			tcp_arg(state->client_pcb, NULL);
			tcp_poll(state->client_pcb, NULL, 0);
			tcp_recv(state->client_pcb, NULL);
			tcp_err(state->client_pcb, NULL);
			tcp_close(state->client_pcb);
			state->client_pcb = NULL;
		}
		return ERR_OK;
    }
    
    cyw43_arch_lwip_check();
    if (p->tot_len > 0) {
        DEBUG_printf("tcp_server_recv %d/%d err %d\n", p->tot_len, state->recv_len, err);

        //Receive the buffer
		int len = p->tot_len < BUF_SIZE-1 ? p->tot_len:BUF_SIZE-1;
		pbuf_copy_partial(p, state->buffer_recv, len, 0);
		state->buffer_recv[len]='\0';
    	state->msg_ready = true;
        tcp_recved(tpcb, len); //通知lwIP已經接收並處理了len bytes的資料，以將buffer釋放 
    }
    pbuf_free(p);
    return ERR_OK;
}

err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb) {
    DEBUG_printf("tcp_server_poll_fn\n");
    return tcp_server_close(arg); 
}

void tcp_server_err(void *arg, err_t err) {
    if (err != ERR_ABRT) {
        DEBUG_printf("tcp_client_err_fn %d\n", err);
        tcp_server_close(arg);
    }
}

err_t tcp_server_accept(void *arg, struct tcp_pcb *client_pcb, err_t err) {
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
    if (err != ERR_OK || client_pcb == NULL) {
        DEBUG_printf("Failure in accept\n");
        return ERR_VAL;
    }
    DEBUG_printf("Client connected\n");

    state->client_pcb = client_pcb;
    tcp_arg(client_pcb, state);   //將state傳進client_pcb的context，讓其他callback也能拿到這個state
    tcp_recv(client_pcb, tcp_server_recv); //註冊資料接收的callback 
    
	tcp_poll(client_pcb, tcp_server_poll, POLL_TIME_S * 2); 
    /*
	註冊polling callback，每隔一段時間會自動觸發tcp_server_poll()，
	TCP timer的polling週期預設是500 ms，所以假如POLL_TIME_S=5，
	則實際上polling週期為 5 * 2 * 0.5 = 5秒 
	*/ 
	
	tcp_err(client_pcb, tcp_server_err); //註冊錯誤處理的callback 

    return ERR_OK;
}

bool tcp_server_open(void *arg) {
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
    DEBUG_printf("Starting server at %s on port %u\n", ip4addr_ntoa(netif_ip4_addr(netif_list)), TCP_PORT);

	//建立一個TCP protocol control block的指標，代表一個TCP連線
    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_ANY); 
    if (!pcb) {
        DEBUG_printf("failed to create pcb\n");
        return false;
    }

	//把PCB bind到本機某個port上，NULL表示0.0.0.0，即不指定特定IP，也是INADDR_ANY的意思，代表接收任何送來本機該port的封包  
    err_t err = tcp_bind(pcb, NULL, TCP_PORT);
    if (err) {
        DEBUG_printf("failed to bind to port %u\n", TCP_PORT);
        return false;
    }

	//將PCB變成監聽狀態
    state->server_pcb = tcp_listen_with_backlog(pcb, 1); //1是backlog，也就是最多同時允許幾個尚未被accept的連線  
    if (!state->server_pcb) {
        DEBUG_printf("failed to listen\n");
        if (pcb) {
            tcp_close(pcb);
        }
        return false;
    }

	//將state傳給lwIP，之後callback裡可以拿回來用
    tcp_arg(state->server_pcb, state);
    
    //當有連線進來時，註冊tcp_server_accept()這支callback function來處理 
    tcp_accept(state->server_pcb, tcp_server_accept);

    return true;
}

