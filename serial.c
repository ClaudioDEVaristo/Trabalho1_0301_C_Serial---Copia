#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "hardware/uart.h"
#include "hardware/pio.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

//Variáveis globais
uint static volatile ultimoTime = 0;
volatile bool estado_led_verd = false;
volatile bool estado_led_azul = false;
volatile bool reiniciar_loop = false; 
ssd1306_t ssd;
bool estadoBotao;
int resistencia;
float r, g, b;

//Define a pinagem do ssd1306 por i2c
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

//define dos botões
#define bot_A 5
#define bot_B 6

// Definições PWM
#define PWM_RED_PIN 13   // LED vermelho no pino 13
#define PWM_GREEN_PIN 11 // LED verde no pino 11
#define PWM_BLUE_PIN 12  // LED azul no pino 12
#define PWM_WRAP 255     // Resolução de 8 bits (0-255)

// Inicializa um canal PWM
void pwm_init_pin(uint pin) {
  gpio_set_function(pin, GPIO_FUNC_PWM);
  uint slice = pwm_gpio_to_slice_num(pin);
  pwm_config config = pwm_get_default_config();
  pwm_config_set_wrap(&config, PWM_WRAP);
  pwm_init(slice, &config, true);
}

//Inicia as funções
void setup();
void display();

//Inicia os pinos, define como entrada e saída e inicia o ssd1306
void setup(){
  stdio_init_all();   //Inicializa o monitor serial

  // Inicializa PWM para os LEDs RGB
  pwm_init_pin(PWM_RED_PIN);
  pwm_init_pin(PWM_GREEN_PIN);
  pwm_init_pin(PWM_BLUE_PIN);

  gpio_init(bot_A);
  gpio_set_dir(bot_A, GPIO_IN);
  gpio_pull_up(bot_A);

  gpio_init(bot_B);
  gpio_set_dir(bot_B, GPIO_IN);
  gpio_pull_up(bot_B);

  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA);                                        // Pull up the data line
  gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd);                                         // Configura o display
  ssd1306_send_data(&ssd);

  // Envia os dados para o display
  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);
}

void set_led_color(float r, float g, float b) {
  // Converte valores de 0.0-1.0 para 0-255
  uint16_t red = (uint16_t)(r * PWM_WRAP);
  uint16_t green = (uint16_t)(g * PWM_WRAP);
  uint16_t blue = (uint16_t)(b * PWM_WRAP);

  // Define os níveis PWM
  pwm_set_gpio_level(PWM_RED_PIN, red);
  pwm_set_gpio_level(PWM_GREEN_PIN, green);
  pwm_set_gpio_level(PWM_BLUE_PIN, blue);
}

 void cor_para_rgb_baseada_resistencia(int resistencia, float *r, float *g, float *b) {
    // Exemplo: Resistência baixa = vermelho, média = verde, alta = azul
    const int max_resistencia = 1000000; // 1MΩ

    float valor = (float)resistencia / max_resistencia;
    valor = valor > 1.0f ? 1.0f : valor; // Limita a 1.0

    // Gradiente de cores (personalizável):
    *r = 1.0f - valor;       // Vermelho diminui
    *g = valor * 0.5f;       // Verde aumenta parcialmente
    *b = valor;              // Azul aumenta
}

// Estrutura para armazenar o mapeamento das cores (nome -> dígito resistor)
typedef struct {
  char *nome;
  int   valor;
} CorResistor;

// Tabela com as cores de resistores
static CorResistor tabela[] = {
  {"preto", 0}, {"marrom",  1}, {"vermelho", 2}, {"laranja", 3},
  {"amarelo", 4}, {"verde", 5}, {"azul", 6}, {"violeta", 7},
  {"cinza",  8}, {"branco",  9}
};

// Converte o nome da cor em um valor de dígito (0 a 9) para o cálculo do resistor
int cor_para_digito(const char *cor) {
  for (int i = 0; i < 10; i++) {
      if (strcmp(tabela[i].nome, cor) == 0) {
          return tabela[i].valor;
      }
  }
  return -1; // Cor inválida
}

// Calcula a resistência baseado em 3 faixas de cor
int calcular_resistencia(int d1, int d2, int d3) {
  // Se qualquer dígito for inválido, retorna -1
  if (d1 < 0 || d2 < 0 || d3 < 0) {
      return -1;
  }
  // Fórmula: (d1 * 10 + d2) * (10^d3)
  return (int)((d1 * 10 + d2) * pow(10, d3));
}

//rotina da interrupção
  void callback_button(uint botao, uint32_t events)
{
  uint timeAtual = to_us_since_boot(get_absolute_time()); // Tempo em milissegundos 
  //250 ms para evitar ação dupla do botão
  if(timeAtual - ultimoTime > 250000){
    ultimoTime = timeAtual;

   estadoBotao = gpio_get(botao);

    if(!estadoBotao){
      if(botao == bot_A){
        ssd1306_fill(&ssd, false);
        ssd1306_send_data(&ssd);
        ssd1306_draw_string(&ssd, "SIMULANDO...", 8, 30);
        ssd1306_send_data(&ssd); 
        printf("O botao A foi precionado!!\n");

        // Obtém a cor RGB baseada na resistência (exemplo: mapeamento personalizado)
         cor_para_rgb_baseada_resistencia(resistencia, &r, &g, &b);

        // Define a cor do LED
        set_led_color(r, g, b);

      }else if(botao == bot_B){
        reiniciar_loop = true;
        ssd1306_fill(&ssd, false);
        ssd1306_send_data(&ssd);
        ssd1306_draw_string(&ssd, "SAINDO...", 8, 30);
        ssd1306_send_data(&ssd); 
        printf("O botao B foi precionado!!\n");
        set_led_color(0, 0, 0);
      
      }
    }
  }
}

char esperar_botao() {
  while (1) {  // Loop infinito
    if (reiniciar_loop) { // Se B foi pressionado, retorna 'B'
      reiniciar_loop = false;
      return 'B';
  }
      sleep_ms(10);  // Pequeno delay para evitar leituras erradas
  }
}

//função para desenhar e atulizar o display oled
void display(){

      ssd1306_draw_string(&ssd, "RESISTOR", 20, 10);
      ssd1306_draw_string(&ssd, "CALCULATOR", 20, 30);
      ssd1306_send_data(&ssd);
      sleep_ms(3000);
      ssd1306_fill(&ssd, false);
      ssd1306_send_data(&ssd);

      ssd1306_draw_string(&ssd, "DEFINA AS", 8, 10);
      ssd1306_draw_string(&ssd, "3 CORES", 8, 30);
      ssd1306_draw_string(&ssd, "DO RESISTOR", 8, 50); // Desenha uma string    
      ssd1306_send_data(&ssd); // Atualiza o display
      ssd1306_fill(&ssd, false);
      ssd1306_send_data(&ssd);
}

//Função principal
int main()
{
  stdio_init_all();
  setup(); // Chama a função que configura os pinos 
  display();

  //Chama as funções de callback
  gpio_set_irq_enabled_with_callback(bot_A, GPIO_IRQ_EDGE_FALL, true, &callback_button);
  gpio_set_irq_enabled_with_callback(bot_B, GPIO_IRQ_EDGE_FALL, true, &callback_button);
    
    while (true)
    { 
      reiniciar_loop = false;

      char cor1[20], cor2[20], cor3[20];
   
      ssd1306_draw_string(&ssd, "ESCOLHA A", 8, 10);
      ssd1306_draw_string(&ssd, "PRIMEIRA COR", 8, 30);
      ssd1306_send_data(&ssd);  
      scanf(" %19s", cor1); 
      ssd1306_fill(&ssd, false);
      ssd1306_send_data(&ssd);

      ssd1306_draw_string(&ssd, "ESCOLHA A", 8, 10);
      ssd1306_draw_string(&ssd, "SEGUNDA COR", 8, 30);
      ssd1306_send_data(&ssd);  
      scanf(" %19s", cor2); 
      ssd1306_fill(&ssd, false);
      ssd1306_send_data(&ssd);

      ssd1306_draw_string(&ssd, "ESCOLHA A", 8, 10);
      ssd1306_draw_string(&ssd, "TERCEIRA COR", 8, 30);
      ssd1306_send_data(&ssd);  
      scanf(" %19s", cor3); 
      ssd1306_fill(&ssd, false);
      ssd1306_send_data(&ssd);

      if (reiniciar_loop) {
        continue; // Volta para o início do while(true)
    }

      // Converte cada cor em dígito
      int d1 = cor_para_digito(cor1);
      int d2 = cor_para_digito(cor2);
      int d3 = cor_para_digito(cor3);

           
       if (reiniciar_loop) { // Se B foi pressionado durante o cálculo, reinicie
        continue;
    }

      if (d1 == -1 || d2 == -1 || d3 == -1) {
        ssd1306_fill(&ssd, false);
        ssd1306_draw_string(&ssd, "COR INVALIDA!", 8, 30);
        ssd1306_send_data(&ssd);
        sleep_ms(3000);
        continue; // Reinicia o loop
    }

     resistencia = calcular_resistencia(d1, d2, d3);

      if (resistencia == -1) {
        ssd1306_draw_string(&ssd, "ERRO: COR INVALIDA", 8, 50);
    }else{
      char buffer[16];
      sprintf(buffer, "%d OHMS", resistencia);

      ssd1306_draw_string(&ssd, "RESISTENCIA", 8, 10);
      ssd1306_draw_string(&ssd, "CALCULADA", 8, 30);
      ssd1306_draw_string(&ssd, buffer, 8, 50);
      ssd1306_send_data(&ssd);
      sleep_ms(4000);
      ssd1306_fill(&ssd, false);
      ssd1306_send_data(&ssd);
    }
      ssd1306_draw_string(&ssd, "GOSTARIA DE", 8, 10);
      ssd1306_draw_string(&ssd, "SIMULAR ", 8, 30);
      ssd1306_draw_string(&ssd, "A  SIM  B  NAO", 8, 50);
      ssd1306_send_data(&ssd);

      char escolha = esperar_botao();  // Aguarda a escolha do usuário
    }
    return 0;
  }

 
   
