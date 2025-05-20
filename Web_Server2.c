#include <stdio.h>             // Biblioteca padrão de entrada/saída (ex: printf)
#include <string.h>            // Biblioteca para manipulação de strings
#include "pico/stdlib.h"       // Biblioteca do SDK do Raspberry Pi Pico (funções básicas de I/O, delays, etc.)

// Inclusão de arquivos de cabeçalho específicos do projeto
#include "connect_wifi.h"      // Contém a função para conectar-se a uma rede Wi-Fi
#include "server.h"            // Contém funções relacionadas à inicialização de um servidor (provavelmente HTTP ou socket)
#include "init_config.h"       // Contém funções para inicializar periféricos e configurações gerais
#include "global_manage.h"     // Pode conter variáveis globais e funções auxiliares compartilhadas

#include "FreeRTOS.h"        // Kernel FreeRTOS
#include "task.h"            // API de criação e controle de tarefas FreeRTOS

#include "hardware/pwm.h"       // API de PWM para controle de sinais sonoros
#include "hardware/clocks.h"    // API de clocks do RP2040

// buzzer.c arquivo com funções para controle do buzzer
// connect_wifi.c arquivo com funções para conexão wi-fi
// global_manage.c arquivo com funções para controle das varáveis globais
// init_config.c arquivo com funções para inicialização dos periférico
// server.c arquivo com funções relativas ao web_server
// ssd1306.c arquivo com as funções de controle de display

// Tarefa que controla o web_server
void vServerTask()
{
    // Conecta ao Wi-Fi; se falhar, encerra o programa retornando 
    int result = connect_wifi();
    if (result) return;

    // Inicializa o servidor; se falhar, encerra o programa retornando
    result = server_init();
    if (result) return;

    while (true)
    {
        /* 
        * Efetuar o processamento exigido pelo cyw43_driver ou pela stack TCP/IP.
        * Este método deve ser chamado periodicamente a partir do ciclo principal 
        * quando se utiliza um estilo de sondagem pico_cyw43_arch 
        */
        cyw43_arch_poll(); // Necessário para manter o Wi-Fi ativo
        //sleep_ms(100);      // Reduz o uso da CPU
        printf("Funcionando!\n");
        vTaskDelay(pdMS_TO_TICKS(100));    // Esperar 100ms
    }

    //Desligar a arquitetura CYW43.
    cyw43_arch_deinit();
}

// Tarefa que controla o display
void vDisplayTask()
{
    // Inicializa o display (OLED/SSD1306)
    display_init(get_ssd_pointer());

    while (true)
    {
        desenhar_prioridade(get_ssd_pointer()); // Atualiza o display com o novo estado de prioridade
        vTaskDelay(pdMS_TO_TICKS(100));    // Esperar 100ms
    }
}

// Tarefa que controla o display
void vBuzzerTask()
{
    // Inicializa o buzzer
    buzzer_init();
    // Inicializa os pinos GPIO dos leds
    init_pins();

    while (true)
    {
        if (get_buzzer()==BUZZER_ATIVADO) 
        {
            gpio_put(RED_PIN, 0);
            gpio_put(BLUE_PIN, 1);
            pwm_set_gpio_level(BUZZER_PIN, 2048);
            vTaskDelay(pdMS_TO_TICKS(100));    // Esperar 100ms  

            gpio_put(BLUE_PIN, 0);
            gpio_put(RED_PIN, 1);
            pwm_set_gpio_level(BUZZER_PIN, 0);
            vTaskDelay(pdMS_TO_TICKS(100));    // Esperar 100ms 
            gpio_put(RED_PIN, 0);
        }
        // else 
        // {
        //     gpio_put(BLUE_PIN, 0);
        //     gpio_put(RED_PIN, 0);
        //     vTaskDelay(pdMS_TO_TICKS(100));    // Esperar 100ms 
        // }
    }
}

int main()
{
    // Inicializa todas as interfaces padrão de entrada/saída
    stdio_init_all();

    sleep_ms(10000);

    // Desativa o buzzer (define o estado como inativo)
    desativar_buzzer();
    // Define a prioridade atual
    set_prioridade(DISPLAY_DESATIVADO);

    xTaskCreate(vServerTask, "Server Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vDisplayTask, "Display Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vBuzzerTask, "Buzzer Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

    // Inicia o scheduler do FreeRTOS
    vTaskStartScheduler();
    
    // Se voltar aqui, o bootrom exibe mensagem de erro
    panic_unsupported();

    // Se tudo ocorreu bem, retorna 0 (execução bem-sucedida)
    return 0;
}
