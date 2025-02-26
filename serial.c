#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "inc/config_matriz.h"
#include "hardware/uart.h"
#include "hardware/pio.h"
#include "hardware/irq.h"

//Variáveis globais
uint static volatile ultimoTime = 0;
volatile bool estado_led_verd = false;
volatile bool estado_led_azul = false;
ssd1306_t ssd;
bool estadoBotao;

//Define a pinagem do ssd1306 por i2c
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

//define do pino da matriz de leds
#define OUT_PIN 7

//define dos leds azul e verde
#define led_verd 11
#define led_azul 12
#define led_vermelho 13

//define dos botões
#define bot_A 5
#define bot_B 6

//String para lógica das letras
char str[10] = "ENTRADA: ";

//Inicia as funções
void setup();
void display();

//Inicia os pinos, define como entrada e saída e inicia o ssd1306
void setup(){
  stdio_init_all();   //Inicializa o monitor serial

  gpio_init(led_verd);
  gpio_set_dir(led_verd, GPIO_OUT);

  gpio_init(led_azul);
  gpio_set_dir(led_azul, GPIO_OUT);

  gpio_init(led_vermelho);
  gpio_set_dir(led_vermelho, GPIO_OUT);

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

// Estrutura para armazenar o mapeamento das cores (nome -> dígito resistor)
typedef struct {
  char *nome;
  int   valor;
} CorResistor;

// Tabela com as cores típicas de resistores
static CorResistor tabela[] = {
  {"preto",   0}, {"marrom",  1}, {"vermelho", 2}, {"laranja", 3},
  {"amarelo", 4}, {"verde",   5}, {"azul",     6}, {"violeta", 7},
  {"cinza",   8}, {"branco",  9}
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
        estado_led_verd = !estado_led_verd;   // Inverte o estado
        ssd1306_fill(&ssd, false);
        ssd1306_send_data(&ssd);
        ssd1306_draw_string(&ssd, "SIMULANDO...", 8, 30);
        ssd1306_send_data(&ssd); 
        printf("O botao A foi precionado!!\n");
       
      }
      else if(botao == bot_B){
        estado_led_azul = !estado_led_azul;   // Inverte o estado
        ssd1306_fill(&ssd, false);
        ssd1306_send_data(&ssd);
        ssd1306_draw_string(&ssd, "SAINDO...", 8, 30);
        ssd1306_send_data(&ssd); 
        printf("O botao B foi precionado!!\n");
      
      }
    }
  }
}

char esperar_botao() {
  while (1) {  // Loop infinito
      if (!gpio_get(bot_A)) {  // Verifica se o botão A foi pressionado
        return 'A';
      }
      if (!gpio_get(bot_B)) {  // Verifica se o botão B foi pressionado
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

// Função que converte o nome da cor em valores RGB (float)

void cor_para_rgb(const char *cor, float *r, float *g, float *b) {
  if (strcmp(cor, "preto") == 0) {
      *r = 0.0f; *g = 0.0f; *b = 0.0f;
  } else if (strcmp(cor, "marrom") == 0) {
      // Aproximação para marrom
      *r = 0.30f; *g = 0.15f; *b = 0.0f;
  } else if (strcmp(cor, "vermelho") == 0) {
      *r = 1.0f; *g = 0.0f; *b = 0.0f;
  } else if (strcmp(cor, "laranja") == 0) {
      *r = 1.0f; *g = 0.5f; *b = 0.0f;
  } else if (strcmp(cor, "amarelo") == 0) {
      *r = 1.0f; *g = 1.0f; *b = 0.0f;
  } else if (strcmp(cor, "verde") == 0) {
      *r = 0.0f; *g = 1.0f; *b = 0.0f;
  } else if (strcmp(cor, "azul") == 0) {
      *r = 0.0f; *g = 0.0f; *b = 1.0f;
  } else if (strcmp(cor, "violeta") == 0) {
      *r = 0.54f; *g = 0.0f; *b = 1.0f;
  } else if (strcmp(cor, "cinza") == 0) {
      *r = 0.5f; *g = 0.5f; *b = 0.5f;
  } else if (strcmp(cor, "branco") == 0) {
      *r = 1.0f; *g = 1.0f; *b = 1.0f;
  } else {
      // Valor padrão para cor não reconhecida
      *r = 0.0625f; *g = 0.0625f; *b = 0.0625f;
  }
}

//Função principal
int main()
{
  stdio_init_all();
  setup(); // Chama a função que configura os pinos 
  PIO pio = pio_config();
  display();

  //Chama as funções de callback
  gpio_set_irq_enabled_with_callback(bot_A, GPIO_IRQ_EDGE_FALL, true, &callback_button);
  gpio_set_irq_enabled_with_callback(bot_B, GPIO_IRQ_EDGE_FALL, true, &callback_button);
    
    while (true)
    { 
      char cor1[20], cor2[20], cor3[20];
      if (stdio_usb_connected())
      { 
      ssd1306_draw_string(&ssd, "ESCOLHA A", 8, 10);
      ssd1306_draw_string(&ssd, "PRIMEIRA COR", 8, 30);
      ssd1306_send_data(&ssd);  
      scanf("%s", cor1);
      ssd1306_fill(&ssd, false);
      ssd1306_send_data(&ssd);

      ssd1306_draw_string(&ssd, "ESCOLHA A", 8, 10);
      ssd1306_draw_string(&ssd, "SEGUNDA COR", 8, 30);
      ssd1306_send_data(&ssd);  
      scanf("%s", cor2);
      ssd1306_fill(&ssd, false);
      ssd1306_send_data(&ssd);

      ssd1306_draw_string(&ssd, "ESCOLHA A", 8, 10);
      ssd1306_draw_string(&ssd, "TERCEIRA COR", 8, 30);
      ssd1306_send_data(&ssd);  
      scanf("%s", cor3);
      ssd1306_fill(&ssd, false);
      ssd1306_send_data(&ssd);

      // Converte cada cor em dígito
      int d1 = cor_para_digito(cor1);
      int d2 = cor_para_digito(cor2);
      int d3 = cor_para_digito(cor3);

      int resistencia = calcular_resistencia(d1, d2, d3);

      char buffer[16];
      sprintf(buffer, "%d OHMS", resistencia);

      ssd1306_draw_string(&ssd, "RESISTENCIA", 8, 10);
      ssd1306_draw_string(&ssd, "CALCULADA", 8, 30);
      ssd1306_draw_string(&ssd, buffer, 8, 50);
      ssd1306_send_data(&ssd);
      sleep_ms(4000);
      ssd1306_fill(&ssd, false);
      ssd1306_send_data(&ssd);

      ssd1306_draw_string(&ssd, "GOSTARIA DE", 8, 10);
      ssd1306_draw_string(&ssd, "SIMULAR ", 8, 30);
      ssd1306_draw_string(&ssd, "A  SIM  B  NAO", 8, 50);
      ssd1306_send_data(&ssd);

      char escolha = esperar_botao();  // Aguarda a escolha do usuário
      }
    }
    return 0;
  }
   
