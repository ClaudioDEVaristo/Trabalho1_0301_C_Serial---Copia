# Projeto BitDogLab - Controle de LEDs e Display

## Descrição do Projeto
Este projeto tem como objetivo a interação com componentes eletrônicos conectados à placa BitDogLab, permitindo o controle de LEDs RGB, uma matriz 5x5 de LEDs e um display SSD1306, através de um monitor serial e botoes físicos.

## Componentes Utilizados
- **Matriz 5x5 de LEDs (endereçáveis) WS2812** conectada ao GPIO 7.
- **LED RGB** conectado aos GPIOs 11, 12 e 13.
- **Botão A** conectado ao GPIO 5.
- **Botão B** conectado ao GPIO 6.
- **Display SSD1306** conectado via I2C (GPIO 14 e GPIO 15).

## Funcionalidades
### 1. Modificação da Biblioteca `font.h`
- Adicionar caracteres minúsculos à biblioteca `font.h`.
- Criar novos caracteres de forma criativa.

### 2. Entrada de Caracteres via PC
- Utilização do **Serial Monitor** do VS Code para digitar caracteres.
- Exibição dos caracteres digitados no **display SSD1306**.
- **Observação:** Apenas um caractere será enviado de cada vez.
- Quando um número de **0 a 9** for digitado, um símbolo correspondente ao número será exibido na matriz **5x5 WS2812**.

### 3. Interação com o Botão A
- Pressionar o **Botão A** alterna o estado do **LED RGB Verde** (ligado/desligado).
- A operação será registrada de duas formas:
  - Uma mensagem informativa será exibida no **display SSD1306**.
  - Um texto descritivo será enviado ao **Serial Monitor**.

### 4. Interação com o Botão B
- Pressionar o **Botão B** alterna o estado do **LED RGB Azul** (ligado/desligado).
- A operação será registrada de duas formas:
  - Uma mensagem informativa será exibida no **display SSD1306**.
  - Um texto descritivo será enviado ao **Serial Monitor**.

## Como Utilizar
1. Certifique-se de que todos os componentes estão conectados corretamente.
2. Compile e carregue o código na placa BitDogLab.
3. Utilize o **Serial Monitor** para enviar caracteres e visualizar a saída no display.
4. Pressione os botões **A** e **B** para testar a interação com os LEDs.

## Link para o vídeo no YouTube

