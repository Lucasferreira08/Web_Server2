#ifndef SERVER_H
#define SERVER_H

#include "pico/cyw43_arch.h"

#include "lwip/netif.h"
#include "lwip/pbuf.h"  // Lightweight IP stack - manipulação de buffers de pacotes de rede
#include "lwip/tcp.h"   // Lightweight IP stack - fornece funções e estruturas para trabalhar com o protocolo TCP

err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err);

void user_request(char **request);

err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);

int server_init();

#endif 