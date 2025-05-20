#include "connect_wifi.h"

#include <stdio.h>         // Biblioteca manipular strings
#include "pico/stdlib.h"

#include "pico/cyw43_arch.h"

#include "lwip/netif.h"

const char WIFI_SSID[] = "";
const char WIFI_PASSWORD[] = "";

int connect_wifi()
{
    if (cyw43_arch_init()) 
    {
        printf("Não foi possível conectar.");
        return 1;
    }
    printf("Wi-Fi inicializado com sucesso\n"); // Mensagem de sucesso na inicialização
    cyw43_arch_enable_sta_mode(); // Habilita o modo estação (client) para a Pico

    // Conectar à rede WiFI - fazer um loop até que esteja conectado
    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0){
        printf("Tentando conexão...\n");
    }
    printf("Conectado com sucesso! \n");

    // Caso seja a interface de rede padrão.
    if (netif_default)
    {
        printf("IP do dispositivo: %s\n", ipaddr_ntoa(&netif_default->ip_addr));
    }

    return 0;
}