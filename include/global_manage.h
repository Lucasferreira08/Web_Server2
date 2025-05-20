#ifndef GLOBAL_MANAGE_H
#define GLOBAL_MANAGE_H

#include "ssd1306.h"

typedef enum 
{
    DISPLAY_DESATIVADO=0,
    PRIORIDADE_UM=1,
    PRIORIDADE_DOIS=2,
    PRIORIDADE_TRES=3,
    PRIORIDADE_QUATRO=4,
    PRIORIDADE_CINCO=5
} Prioridade;

typedef enum 
{
    BUZZER_DESATIVADO=0,
    BUZZER_ATIVADO=1
} Buzzer;

void set_prioridade(Prioridade p);

Prioridade get_prioridade();

void ativar_buzzer();

void desativar_buzzer();

Buzzer get_buzzer();

ssd1306_t *get_ssd_pointer();

#endif