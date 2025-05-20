// Inclui o cabeçalho que declara funções e variáveis globais utilizadas neste módulo
#include "global_manage.h"

// Inclui a biblioteca do display SSD1306 (geralmente OLED I2C)
#include "ssd1306.h"

// Variável global estática para armazenar o estado atual da prioridade
static Prioridade prioridade;

// Variável global estática para armazenar o estado atual do buzzer
static Buzzer buzzer;

// Estrutura que representa o display SSD1306
static ssd1306_t ssd;

// Define a prioridade atual
void set_prioridade(Prioridade p) 
{
    prioridade = p;
}

// Retorna a prioridade atual
Prioridade get_prioridade() 
{
    return prioridade;
}

// Ativa o buzzer (define o estado como ativo)
void ativar_buzzer() 
{
    buzzer = BUZZER_ATIVADO;
}

// Desativa o buzzer (define o estado como inativo)
void desativar_buzzer() 
{
    buzzer = BUZZER_DESATIVADO;
}

// Retorna o estado atual do buzzer
Buzzer get_buzzer() 
{
    return buzzer;
}

// Retorna um ponteiro para a estrutura do display
ssd1306_t *get_ssd_pointer() 
{
    return &ssd;
}