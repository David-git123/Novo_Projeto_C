#include "raylib.h"
#include <math.h>
#include "rlgl.h"
#include "raymath.h"
#include <stdlib.h>
#include <stdio.h>
// Structs

// delta é o tempo entre os frames

typedef struct Player1 {
    Vector2 position;
    int speed;
    bool canJump;
    bool canDuck;
} Player1;

typedef struct Inimigo1 {
    Vector2 position;
    int speed;
    float raio;
    Color cor;
    bool ativo;
    float tempoParaRespawn;
} Inimigo1;

typedef struct Player2 {
    Vector2 position;
    int speed;
    bool canJump;
} Player2;

Inimigo1 inimigos[50];

// Função para ler o arquivo de high score
int lerarquivo() {
    int highScore = 0;
    FILE* arquivo = fopen("high_score.txt", "r");
    
    if (arquivo != NULL) {
        fscanf(arquivo, "%d", &highScore);
        fclose(arquivo);
    }
    
    return highScore;
}

// Função para salvar um novo high score
void salvarHighScore(int score) {
    int highScore = lerarquivo();
    
    // Se o novo score for maior, salva
    if (score > highScore) {
        FILE* arquivo = fopen("high_score.txt", "w");
        if (arquivo != NULL) {
            fprintf(arquivo, "%d", score);
            fclose(arquivo);
        }
    }
}


// Inicialização dos inimigos
void InicializarInimigos() {
    for (int i = 0; i < 50; i++) {
        inimigos[i].ativo = false;
        inimigos[i].tempoParaRespawn = GetRandomValue(1, 3);
    }
}

// Função para obter a velocidade baseada no tempo decorrido
int ObterVelocidade(float tempo ) {

    if (tempo < 5.0) {
        return 350;  // Velocidade mais apropriada 
    } 
    else if (tempo < 10.0) {
        return 400;
    }
    else if (tempo < 15.0) {
        return 450;
    }
    else if (tempo < 20.0) {
        return 500;
    }
    else if (tempo < 30.0) {
        return 550;
    }
    else {
        return 600;
    }
}
// Respawn individual
void RespawnInimigo(int i) {
    inimigos[i].position = (Vector2){ 800.0f, 391 };
    inimigos[i].speed = GetRandomValue(100, 200) / 100.0f;
    inimigos[i].raio = 20;
    inimigos[i].cor = RED;
    inimigos[i].ativo = true;
}

// Atualização dos inimigos
void AtualizarInimigos(float delta, float tempoJogo) {

    int velocidadeAtual = ObterVelocidade(tempoJogo);
    for (int i = 0; i < 50; i++) {
        if (inimigos[i].ativo) {
            inimigos[i].position.x -= velocidadeAtual * delta;

            if (inimigos[i].position.x + inimigos[i].raio < 0) {
                inimigos[i].ativo = false;
                inimigos[i].tempoParaRespawn = 2.0f;
            }
        }
        else {
            inimigos[i].tempoParaRespawn -= delta;
            if (inimigos[i].tempoParaRespawn <= 0) {
                RespawnInimigo(i);
            }
        }
    }
}

// Desenho dos inimigos
void DesenharInimigos() {
    for (int i = 0; i < 50; i++) {
        if (inimigos[i].ativo) {
            DrawCircleV(inimigos[i].position, inimigos[i].raio, inimigos[i].cor);
        }
    }
}

// Main
int main(void) {
    const int screenWidth = 1600;
    const int screenHeight = 850;

    InitWindow(screenWidth, screenHeight, "raylib - player com pulo");

    // Inicializa player
    Player1 player = { 0 };
    player.position = (Vector2){ 23, 415 };
    player.speed = 4;
    player.canJump = true;
    player.canDuck = true;

    // Variáveis de física
    float gravity = 0.5f;
    float jumpForce = -10.0f;
    float velocityY = 0;
    float groundY = 415.0f;  // altura do chão (base do player)

    // Variáveis de pontuação
    int score = 0;
    float scoreTimer = 0.0f;  // Temporizador para incrementar a pontuação a cada segundo
    int highScore = lerarquivo();  // Ler o high score salvo

    // Variáveis para controlar o tempo de jogo
    float tempoJogo = 0.0f;

    InicializarInimigos();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();
        bool isDucking = false;

        // Atualiza o tempo de jogo
        tempoJogo += delta;
    

        // Atualiza o temporizador de pontuação
        scoreTimer += delta;
        
        // Incrementa a pontuação a cada segundo
        if (scoreTimer >= 1.0f) {
            score++;
            scoreTimer -= 1.0f;  // Subtrai 1 segundo em vez de zerar, para não perder o tempo fracionário
        }

        AtualizarInimigos(delta, tempoJogo);

        if (IsKeyDown(KEY_DOWN) && player.canDuck) {
            isDucking = true;
        }
        else {
            isDucking = false;
        }

        // Gravidade
        velocityY += gravity;

        // Pulo com seta para cima
        if (IsKeyPressed(KEY_UP) && player.canJump) {
            velocityY = jumpForce;
            player.canJump = false;
        }

        // Aplica movimento vertical
        player.position.y += velocityY;

        // Verifica chão
        if (player.position.y >= groundY) {
            player.position.y = groundY;
            velocityY = 0;
            player.canJump = true;
            player.canDuck = true;
        }

        // Desenho
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DesenharInimigos();

        DrawLine(0, screenHeight / 2, screenWidth, screenHeight / 2, DARKGRAY);
        // Desenha player como retângulo
        float currentHeight;
        if (isDucking == true) {
            currentHeight = 20;
        }
        else {
            currentHeight = 40;
        }

        Rectangle playerRect = { player.position.x - 20, player.position.y - currentHeight, 40.0f, currentHeight };
        DrawRectangleRec(playerRect, RED);


        bool colidiu = false;
        for (int i = 0; i < 50; i++) {
            if (inimigos[i].ativo) {
                // checar colião com fução raylib
                if (CheckCollisionCircleRec(inimigos[i].position, inimigos[i].raio, playerRect)) {
                    colidiu = true;
                    break;
                }
            }
        }

        // Se colidiu, salva o high score e reinicia o jogo
        if (colidiu) {
            salvarHighScore(score);
            highScore = lerarquivo();  
            score = 0;               
            scoreTimer = 0.0f;
            tempoJogo = 0.0f;        
            
            // Reinicia os inimigos
            for (int i = 0; i < 50; i++) {
                inimigos[i].ativo = false;
                inimigos[i].tempoParaRespawn = GetRandomValue(1, 3);
            }
        }

        // Desenha posição do mouse
        Vector2 mousePos = GetMousePosition();
        DrawCircleV(mousePos, 5, RED);
        DrawText(TextFormat("Mouse: [%.0f, %.0f]", mousePos.x, mousePos.y), 10, 10, 20, DARKGRAY);

        // Desenha a pontuação atual e o high score
        DrawText(TextFormat("Score: %d", score), 10, 50, 30, BLACK);
        DrawText(TextFormat("High Score: %d", highScore), 10, 90, 20, DARKGRAY);


        EndDrawing();
    }
    salvarHighScore(score);
    CloseWindow();
    return 0;}