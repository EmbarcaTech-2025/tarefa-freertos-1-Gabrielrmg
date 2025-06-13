#include <stdio.h> // Inclui a biblioteca padrão de entrada e saída
#include "pico/stdlib.h" // Inclui a biblioteca padrão para o Raspberry Pi Pico
#include "FreeRTOS.h" // Inclui a biblioteca FreeRTOS para gerenciamento de tarefas
#include "task.h" // Inclui a biblioteca de tarefas do FreeRTOS

/* ---- ajuste de pinos ---- */
#define LED_R   13 // Define o pino para o LED vermelho
#define LED_G   11 // Define o pino para o LED verde
#define LED_B   12 // Define o pino para o LED azul
#define BUZZER  10 // Define o pino para o buzzer
#define BTN_A    5 // Define o pino para o botão A
#define BTN_B    6 // Define o pino para o botão B

static TaskHandle_t ledTaskHandle  = NULL; // Handle para a tarefa do LED
static TaskHandle_t buzzTaskHandle = NULL; // Handle para a tarefa do buzzer

// Função da tarefa que controla os LEDs
void vLedTask(void *arg) {
    const uint pins[3] = {LED_R, LED_G, LED_B}; // Array com os pinos dos LEDs
    for (int i=0; i<3; i++) { // Inicializa os pinos dos LEDs
        gpio_init(pins[i]); // Inicializa o pino
        gpio_set_dir(pins[i], GPIO_OUT); // Define o pino como saída
    }
    uint8_t idx = 0; // Índice para controlar qual LED acender
    for (;;) { // Loop infinito
        for (int i=0; i<3; i++) // Acende o LED correspondente ao índice
            gpio_put(pins[i], i==idx); // Acende o LED se o índice corresponder
        idx = (idx+1)%3; // Atualiza o índice para o próximo LED
        vTaskDelay(pdMS_TO_TICKS(500)); // Aguarda 500 ms
    }
}

// Função da tarefa que controla o buzzer
void vBuzzerTask(void *arg) {
    gpio_init(BUZZER); // Inicializa o pino do buzzer
    gpio_set_dir(BUZZER, GPIO_OUT); // Define o pino como saída
    for (;;) { // Loop infinito
        gpio_put(BUZZER, 1); // Liga o buzzer
        vTaskDelay(pdMS_TO_TICKS(100)); // Aguarda 100 ms
        gpio_put(BUZZER, 0); // Desliga o buzzer
        vTaskDelay(pdMS_TO_TICKS(900)); // Aguarda 900 ms
    }
}

// Função da tarefa que controla os botões
void vButtonTask(void *arg) {
    gpio_init(BTN_A); // Inicializa o pino do botão A
    gpio_pull_up(BTN_A); // Ativa o resistor pull-up para o botão A
    gpio_init(BTN_B); // Inicializa o pino do botão B
    gpio_pull_up(BTN_B); // Ativa o resistor pull-up para o botão B
    bool ledSusp = false, buzzSusp = false; // Variáveis para controlar o estado de suspensão das tarefas
    for (;;) { // Loop infinito
        // Verifica se o botão A foi pressionado
        if (!gpio_get(BTN_A) && !ledSusp) { // Se o botão A está pressionado e a tarefa de LED não está suspensa
            vTaskSuspend(ledTaskHandle); // Suspende a tarefa do LED
            ledSusp = true; // Atualiza o estado de suspensão
        } else if (gpio_get(BTN_A) && ledSusp) { // Se o botão A não está pressionado e a tarefa de LED está suspensa
            vTaskResume(ledTaskHandle); // Retoma a tarefa do LED
            ledSusp = false; // Atualiza o estado de suspensão
        }

        // Verifica se o botão B foi pressionado
        if (!gpio_get(BTN_B) && !buzzSusp) { // Se o botão B está pressionado e a tarefa do buzzer não está suspensa
            vTaskSuspend(buzzTaskHandle); // Suspende a tarefa do buzzer
            buzzSusp = true; // Atualiza o estado de suspensão
        } else if (gpio_get(BTN_B) && buzzSusp) { // Se o botão B não está pressionado e a tarefa do buzzer está suspensa
            vTaskResume(buzzTaskHandle); // Retoma a tarefa do buzzer
            buzzSusp = false; // Atualiza o estado de suspensão
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Aguarda 100 ms antes de verificar novamente
    }
}

// Função principal
int main(void) {
    stdio_init_all(); // Inicializa a entrada e saída padrão
    // Cria as tarefas do FreeRTOS
    xTaskCreate(vLedTask, "LED", 256, NULL, 2, &ledTaskHandle); // Cria a tarefa do LED
    xTaskCreate(vBuzzerTask, "Buzz", 256, NULL, 2, &buzzTaskHandle); // Cria a tarefa do buzzer
    xTaskCreate(vButtonTask, "Btns", 256, NULL, 3, NULL); // Cria a tarefa dos botões
    vTaskStartScheduler(); // Inicia o escalonador do FreeRTOS
    while (1) {} // Loop infinito (nunca deve ser alcançado)
}
