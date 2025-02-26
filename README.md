# Projeto-final-Yuri-Ping-Pong

Aluno: Yuri Silva Reis

Matrícula: TIC370101506

- Pong no Raspberry Pi Pico

Este é um projeto de implementação do clássico jogo Pong utilizando o Raspberry Pi Pico, desenvolvido em C no VS Code. O jogo é exibido em um display OLED e controlado via joystick analógico, com feedback sonoro utilizando um buzzer.

- Funcionalidades:

Movimentação da barra por meio do joystick analógico.

Exibição do Menu.

Exibição do jogo em um display OLED.

Pontuação exibida na tela.

Som utilizando um buzzer.

Led e Matriz de Led usado para interagir com os eventos do jogo.

Implementação otimizada para rodar no Raspberry Pi Pico.

- Componentes Utilizados:

Microcontrolador: Raspberry Pi Pico

Display: OLED SSD1306 (I2C)

Controle: Joystick analógico (potenciômetros X e Y)

Som: Buzzer piezoelétrico

Alimentação: Fonte 5V via USB

- Como Jogar:

Início do jogo: O jogo inicia automaticamente ao ligar o Raspberry Pi Pico.

Menu do jogo: Aperte o botão A para começar o jogo.

Movimentação: Mova o joystick para cima ou para baixo para controlar a barra.

A barra acompanha o movimento do joystick em tempo real.

- Objetivo:

Rebata a bola e impeça que ela ultrapasse sua barra.
Cada vez que você acerta a bola, ela muda de direção.

Se deixar a bola passar, o adversário marca ponto.

- Pontuação:

O jogo exibe a pontuação atual no display OLED.
A cada ponto marcado, um led é exibido como "reação".

- Fim de jogo:

O jogo pode ser configurado para terminar após um determinado número de pontos.
Para reiniciar, basta reiniciar o Raspberry Pi Pico.

- Melhorias Futuras:

Implementar um modo para dois jogadores.
Adicionar uma interface de configuração (ajuste de dificuldade, sons, etc.).

Melhorar a performance gráfica no display OLED.

