#include "server.h"

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include "global_manage.h"
// #include "ssd1306.h"

// Tratamento do request do usuário - digite aqui
// Função que trata requisições do usuário.
// Recebe como parâmetro um ponteiro para uma string (a requisição HTTP completa).
void user_request(char **request)
{
    // Verifica se a string da requisição contém "GET /alarm_on"
    if (strstr(*request, "GET /alarm_on") != NULL)
    {
        // Muda estado do buzzer para ativo
        ativar_buzzer();
    }
    // Verifica se a requisição é "GET /alarm_off"
    else if (strstr(*request, "GET /alarm_off") != NULL)
    {
        // Seta o estado da prioridade como "DISPLAY_DESATIVADO"
        set_prioridade(DISPLAY_DESATIVADO);
        desativar_buzzer();
    }
    // Verifica se a requisição contém um valor de prioridade na URL
    else if (strstr(*request, "GET /prioridade?valor=") != NULL)
    {
        // Captura a posição do parâmetro "?valor=" na string da requisição
        char *param_pos = strstr(*request, "?valor=");

        // Converte o valor após o parâmetro para um inteiro
        int prioridade = atoi(param_pos + 7); // pula os 7 caracteres de "?valor="

        // Verifica se a prioridade está entre os valores válidos (de 1 a 5, por exemplo)
        if (prioridade >= PRIORIDADE_UM && prioridade <= PRIORIDADE_CINCO) 
        {
            // Atualiza a prioridade com o valor recebido
            set_prioridade(prioridade);

            // Toca o buzzer para indicar que a prioridade foi alterada
            ativar_buzzer();
        }
    }
};

// Função de callback para processar requisições HTTP
err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    if (!p)
    {
        tcp_close(tpcb);
        tcp_recv(tpcb, NULL);
        return ERR_OK;
    }

    // Alocação do request na memória dinámica
    char *request = (char *)malloc(p->len + 1);
    memcpy(request, p->payload, p->len);
    request[p->len] = '\0';

    printf("Request: %s\n", request);

    // Tratamento de request - Controle dos LEDs
    user_request(&request);     // Chamando função para tratar o request

    // Cria a resposta HTML
    // char html[1024];
    char html[2048];

    // Instruções html do webserver
    snprintf(html, sizeof(html), // Formatar uma string e armazená-la em um buffer de caracteres
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "\r\n"
             "<!DOCTYPE html>\n"
             "<html lang=\"pt-br\">\n"
             "<head>\n"
             "<meta charset=\"UTF-8\">\n"
             "<title> Controle de Paciente </title>\n"
             "<style>\n"
             "body { background-color:rgb(2, 64, 93); font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }\n"
             "h1 { font-size: 64px; margin-bottom: 30px; }\n"
             "button { background-color: LightGray; font-size: 36px; margin: 10px; padding: 20px 40px; border-radius: 10px; }\n"
             ".temperature { font-size: 48px; margin-top: 30px; color: #333; }\n"
             "</style>\n"
             "</head>\n"
             "<body>\n"
             "<h1>Controle de Paciente</h1>\n"
             "<form action=\"./alarm_on\"><button>Ativar Alarme</button></form>\n"
             "<form action=\"./alarm_off\"><button>Desativar Chamado</button></form>\n"
             "<h1>Escolha o nível de urgência:</h1>\n"
             "<form action=\"./prioridade\" method=\"get\">\n"
             "<button type=\"submit\" name=\"valor\" value=\"1\">1</button>\n"
             "<button type=\"submit\" name=\"valor\" value=\"2\">2</button>\n"
             "<button type=\"submit\" name=\"valor\" value=\"3\">3</button>\n"
             "<button type=\"submit\" name=\"valor\" value=\"4\">4</button>\n"
             "<button type=\"submit\" name=\"valor\" value=\"5\">5</button>\n"
             "</form>\n"
             "</body>\n"
             "</html>\n");

    // Escreve dados para envio (mas não os envia imediatamente).
    tcp_write(tpcb, html, strlen(html), TCP_WRITE_FLAG_COPY);

    // Envia a mensagem
    tcp_output(tpcb);

    //libera memória alocada dinamicamente
    free(request);
    
    //libera um buffer de pacote (pbuf) que foi alocado anteriormente
    pbuf_free(p);

    return ERR_OK;
}

// Função de callback ao aceitar conexões TCP
err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    tcp_recv(newpcb, tcp_server_recv);
    return ERR_OK;
}

int server_init() 
{
    // Configura o servidor TCP - cria novos PCBs TCP. É o primeiro passo para estabelecer uma conexão TCP.
    struct tcp_pcb *server = tcp_new();
    if (!server)
    {
        printf("Falha ao criar servidor TCP\n");
        return 1;
    }

    //vincula um PCB (Protocol Control Block) TCP a um endereço IP e porta específicos.
    if (tcp_bind(server, IP_ADDR_ANY, 80) != ERR_OK)
    {
        printf("Falha ao associar servidor TCP à porta 80.\n");
        return 1;
    }

    // Coloca um PCB (Protocol Control Block) TCP em modo de escuta, permitindo que ele aceite conexões de entrada.
    server = tcp_listen(server);

    // Define uma função de callback para aceitar conexões TCP de entrada. É um passo importante na configuração de servidores TCP.
    tcp_accept(server, tcp_server_accept);
    printf("Servidor ouvindo na porta 80:%d\n", IP_ADDR_ANY->addr);

    // while (true)
    // {
    //     /* 
    //     * Efetuar o processamento exigido pelo cyw43_driver ou pela stack TCP/IP.
    //     * Este método deve ser chamado periodicamente a partir do ciclo principal 
    //     * quando se utiliza um estilo de sondagem pico_cyw43_arch 
    //     */
    //     cyw43_arch_poll(); // Necessário para manter o Wi-Fi ativo
    //     //sleep_ms(100);      // Reduz o uso da CPU
    //     printf("Funcionando!\n");
    //     //sleep_ms(100);      // Reduz o uso da CPU
    // }

    // // //Desligar a arquitetura CYW43.
    // cyw43_arch_deinit();
    return 0;
}