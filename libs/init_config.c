#include "init_config.h"        // Declarações de pinos, constantes e protótipos de funções de inicialização

#include "hardware/pwm.h"       // API de PWM para controle de sinais sonoros
#include "hardware/clocks.h"    // API de clocks do RP2040

// Configura e inicializa o display SSD1306 via I2C
void display_init(ssd1306_t *ssd)
{
    // Inicializa o barramento I2C na porta definida a 400 kHz
    i2c_init(I2C_PORT, 400 * 1000);

    // Configura pinos SDA e SCL para modo I2C
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    // Habilita resistores de pull-up internos nas linhas I2C
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Chama rotina da biblioteca para inicializar o SSD1306 (endereço, dimensões, I2C)
    ssd1306_init(ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    // Configura parâmetros internos do display (scroll, inversão, etc.)
    ssd1306_config(ssd);
    // Envia buffer de inicialização ao display
    ssd1306_send_data(ssd);

    // Limpa a tela (preenche com pixels apagados) e envia ao display
    ssd1306_fill(ssd, false);
    ssd1306_send_data(ssd);
}

// Configura o PWM no pino do buzzer para gerar o tom desejado
void buzzer_init() 
{
    // Associa o pino do buzzer à função PWM
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);

    // Recupera o slice de PWM correspondente ao pino
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);

    // Carrega a configuração padrão de PWM
    pwm_config config = pwm_get_default_config();
    // Ajusta o divisor de clock para que a frequência seja BUZZER_FREQUENCY com 12-bit de resolução
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096));
    // Inicializa o slice de PWM com a configuração e já habilita o módulo
    pwm_init(slice_num, &config, true);

    // Define nível inicial zero (silêncio)
    pwm_set_gpio_level(BUZZER_PIN, 0);
}

void init_pins() 
{
    gpio_init(RED_PIN);
    gpio_set_dir(RED_PIN, GPIO_OUT);
    gpio_init(BLUE_PIN);
    gpio_set_dir(BLUE_PIN, GPIO_OUT);
}

// Inicializa o PIO para a matriz de LEDs e retorna o número do state machine usado
uint pio_init(PIO pio)
{
    // Ajusta o clock do RP2040 para 128 MHz (128000 kHz)
    set_sys_clock_khz(128000, false);

    // Carrega o programa PIO na memória do PIO e retorna o offset onde ele foi colocado
    uint offset = pio_add_program(pio, &pio_matrix_program);

    // Reserva um state machine livre (bloqueante) e retorna seu índice
    uint sm = pio_claim_unused_sm(pio, true);

    // Inicializa o state machine com o programa carregado, definindo pino de saída
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    return sm;
}