O projeto implementa um sistema de chamada e alerta remoto para monitoramento de pacientes,
rodando em um Raspberry Pi Pico com FreeRTOS. Por meio de uma interface web (acessível pela

rede Wi-Fi), o usuário pode:
● Ativar ou desativar um alarme sonoro e visual;
● Definir um nível de prioridade (1 a 5) para o chamado.
O dispositivo então sinaliza o estado atual via buzzer, LED RGB, matriz de LEDs e um
display OLED, permitindo atender prontamente às necessidades do paciente.