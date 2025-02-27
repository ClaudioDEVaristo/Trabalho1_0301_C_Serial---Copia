Aqui está um arquivo `README.md` completo para o projeto:

---

# Resistor Calculator: Simulador de Resistores com Feedback Visual

Este projeto é um simulador de resistores baseado em código de cores, desenvolvido para o microcontrolador **Raspberry Pi Pico**. Ele permite ao usuário inserir as cores das faixas de um resistor, calcula o valor da resistência e simula a cor correspondente em um LED RGB, exibindo informações em um display OLED. O sistema é interativo, utilizando botões para confirmação e reinício.

---

## Funcionalidades

- **Cálculo de resistência**: Converte cores de faixas de resistores em valores de resistência.
- **Exibição no OLED**: Mostra o valor da resistência e instruções no display SSD1306.
- **Simulação de cor**: Usa um LED RGB para simular a cor correspondente à resistência calculada.
- **Interação com botões**: Botões para confirmar a simulação ou reiniciar o processo.

---

## Componentes Utilizados

- **Microcontrolador**: Raspberry Pi Pico.
- **Display**: OLED SSD1306 (128x64 pixels, comunicação I2C).
- **LED RGB**: Controle de cores via PWM.
- **Botões**: Dois botões para interação (A e B).
- **Resistores**: Pull-up para os botões.

---

## Esquema de Conexões

| Pino do Pico | Componente          | Função               |
|--------------|---------------------|----------------------|
| GP5          | Botão A             | Entrada digital      |
| GP6          | Botão B             | Entrada digital      |
| GP11         | LED Verde           | Saída PWM            |
| GP12         | LED Azul            | Saída PWM            |
| GP13         | LED Vermelho        | Saída PWM            |
| GP14         | OLED SDA            | I2C Data             |
| GP15         | OLED SCL            | I2C Clock            |

---

## Como Usar

1. **Conectar o hardware**:
   - Conecte o display OLED aos pinos I2C (SDA: GP14, SCL: GP15).
   - Conecte o LED RGB aos pinos PWM (Vermelho: GP13, Verde: GP11, Azul: GP12).
   - Conecte os botões aos pinos GP5 (Botão A) e GP6 (Botão B).

2. **Compilar e carregar o firmware**:
   - Utilize o ambiente de desenvolvimento configurado com o SDK do Raspberry Pi Pico.
   - Compile o código e carregue-o no Pico.

3. **Interagir com o sistema**:
   - O display OLED exibirá instruções para inserir as cores das faixas do resistor.
   - Insira as cores via terminal (ex: "vermelho", "verde", "azul"). *SEMPRE COLOCANDO O ESPAÇO NO FINAL DA COR*
   - O valor da resistência será exibido no OLED.
   - Pressione o Botão A para simular a cor no LED RGB.
   - Pressione o Botão B para reiniciar o processo.

---

## Estrutura do Código

O código está organizado nas seguintes funções principais:

- **`setup()`**: Configura pinos, I2C, PWM e inicializa o display OLED.
- **`set_led_color(float r, float g, float b)`**: Define a cor do LED RGB.
- **`cor_para_rgb_baseada_resistencia()`**: Mapeia a resistência para uma cor RGB.
- **`calcular_resistencia()`**: Calcula o valor da resistência com base nas cores.
- **`callback_button()`**: Trata eventos de pressionamento dos botões.
- **`display()`**: Exibe mensagens no OLED.

---

## Exemplo de Uso

1. Inicie o sistema.
2. Insira as cores das faixas do resistor:
   ```
   Escolha a primeira cor: vermelho
   Escolha a segunda cor: verde
   Escolha a terceira cor: azul
   ```
3. O sistema exibirá no OLED:
   ```
   RESISTÊNCIA CALCULADA: 2500000 OHMS
   ```
4. Pressione o Botão A para simular a cor no LED RGB.
5. Pressione o Botão B para reiniciar.

---

## Dependências

- **Bibliotecas**:
  - `pico/stdlib.h`: Funções básicas do Pico.
  - `hardware/i2c.h`: Comunicação I2C com o OLED.
  - `hardware/pwm.h`: Controle PWM do LED RGB.
  - `ssd1306.h`: Driver para o display OLED.

- **Ferramentas**:
  - Ambiente de desenvolvimento configurado com o SDK do Raspberry Pi Pico.
  - Compilador GCC para ARM.

---

## Testes e Validação

O projeto foi testado com diversas combinações de cores, garantindo que:
- O cálculo da resistência está correto.
- A cor do LED RGB corresponde à resistência calculada.
- A interação com os botões é responsiva.

---

## Melhorias Futuras

1. Adicionar suporte a mais cores de resistores.
2. Implementar uma interface gráfica mais elaborada no OLED.
3. Adicionar suporte a resistores de 4 e 5 faixas.

---

## Referências

- [Documentação do Raspberry Pi Pico](https://www.raspberrypi.com/documentation/microcontrollers/)
- [Datasheet do Display SSD1306](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
- [Guia de Configuração PWM](https://www.raspberrypi.com/documentation/microcontrollers/pwm.html)

---


