#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



typedef struct dificuldade {
    int qtd_decrescimo;
    int max_loops;
    int tempo_spawn;
    struct dificuldade* proximo;
}dificuldade;


typedef struct inimigos {
    int altura;
    int largura;
    int posy;
    Texture2D textura_inimigo;
    int id;
}inimigos;


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

int main(void)
{
    int matriz[2][2] = { { 1,2 }, { 3, 4 } };
    int numero_linha;
    int numero_coluna;
    srand(time(NULL));
    Color cor = BLACK;
    const int screenWidth = 800;
    const int screenHeight = 450;
    int posicao_x_rect = 798;
    int numero = 0;
    int contador = 0;
    int contador_frames = 0;
    int contador_frames_passaro = 0;
    int pontuacao = 0;
    int frame_contador_pontuacao = 0;
    int posicao_y_estrela = 50;
    int posicao_x_estrela = 798;
   
    /*int posy = 120;
    int altura = 30;
    int largura = 80;*/

    dificuldade* dificuldade_1 = malloc(sizeof(dificuldade));
    dificuldade* dificuldade_2 = malloc(sizeof(dificuldade));
    dificuldade* dificuldade_3 = malloc(sizeof(dificuldade));

    if (dificuldade_1 == NULL || dificuldade_2 == NULL || dificuldade_3 == NULL) {
        InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
        return 0;
    }

    inimigos inimigo_1;//cacto
    inimigos inimigo_2;//passaro
    inimigos inimigo_3;//big_cacto



    inimigo_1.posy = 120;
    inimigo_1.altura = 30;
    inimigo_1.largura = 80;
    inimigo_1.id = 0;

    inimigo_2.posy = 50;
    inimigo_2.altura = 20;
    inimigo_2.largura = 50;
    inimigo_2.id = 1;

    inimigo_3.posy = 120;
    inimigo_3.altura = 70;
    inimigo_3.largura = 40;
    inimigo_3.id = 2;

    dificuldade_1->qtd_decrescimo = 2;
    dificuldade_1->max_loops = 3;
    dificuldade_1->tempo_spawn = -400;
    dificuldade_1->proximo = dificuldade_2;

    dificuldade_2->qtd_decrescimo = 6;
    dificuldade_2->max_loops = 8;
    dificuldade_2->tempo_spawn = -300;
    dificuldade_2->proximo = dificuldade_3;

    dificuldade_3->qtd_decrescimo = 20;
    dificuldade_3->max_loops = 500;
    dificuldade_3->tempo_spawn = -200;
    dificuldade_3->proximo = NULL;

   
    
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60); //60 frames por segundo

    dificuldade* atual = dificuldade_1;
    Texture2D dinossauro = LoadTexture("resources/dinossauro_parado.png");
    Texture2D dinossauro_esq = LoadTexture("resources/dinossauro_em_pe_1.png");
    Texture2D dinossauro_dir = LoadTexture("resources/dinossauro_em_pe_2.png");
    Texture2D atual_textura=dinossauro;

    Texture2D cacto = LoadTexture("resources/cacto2.png");
    Texture2D big_cacto = LoadTexture("resources/cacto1.png");
    Texture2D passaro_down = LoadTexture("resources/passaro1.png");
    Texture2D passaro_up = LoadTexture("resources/passaro2.png");
    

    inimigo_1.textura_inimigo = cacto;
    inimigo_2.textura_inimigo = passaro_up;
    inimigo_3.textura_inimigo = big_cacto;
    inimigos inimigo_atual = inimigo_1;


int highScore = lerarquivo();

while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("DINO RUN", 300, 100, 40, DARKGRAY);
    DrawText(TextFormat("RECORD: %d", highScore), 300, 160, 35, GRAY);
    DrawText("APERTE ENTER", 300, 220, 30, BLACK);


    EndDrawing();

    if (IsKeyPressed(KEY_ENTER)) {
        break; // Sai da tela para iniciar o jogo
    }
}


    while (!WindowShouldClose())
    {

        
        if (contador_frames <= 16) {
            atual_textura = dinossauro_dir;
        }
        else if (contador_frames >= 16 && contador_frames <= 32) {
            atual_textura = dinossauro_esq;
        }
        else {
            contador_frames = 0;
        }

        if (contador == atual->max_loops) {
            atual = atual->proximo;
        }

        if (posicao_x_rect <= atual->tempo_spawn) {
            posicao_x_rect = 800;
            contador++;
            numero = rand() % 3;
            if (numero == inimigo_1.id) {
                inimigo_atual = inimigo_1;
            }

            else if (numero == inimigo_2.id) {
                inimigo_atual = inimigo_2;
            }
            else if (numero == inimigo_3.id) {
                inimigo_atual = inimigo_3;
            }
        }

        if (inimigo_atual.id == 1) {
            if (contador_frames_passaro<= 16) {
                inimigo_atual.textura_inimigo= passaro_up;
            }
            else if (contador_frames_passaro>= 16 && contador_frames_passaro<= 32) {
                inimigo_atual.textura_inimigo= passaro_down;
            }
            else {
                contador_frames_passaro= 0;
            }
        }
        

        BeginDrawing();
        ClearBackground(RAYWHITE);

                // Retangulos de colisão, precisa usar o x e y e o witdh e height
    Rectangle retangulo_dino = {400, 80, (float)atual_textura.width, (float)atual_textura.height};
    
    Rectangle retangulo_inimigo = {
    posicao_x_rect,
    inimigo_atual.posy,
    (float)inimigo_atual.textura_inimigo.width,
    (float)inimigo_atual.textura_inimigo.height
};

                // Colisão
        bool colidiu = CheckCollisionRecs(retangulo_dino, retangulo_inimigo);


                // desenhar na tela o inimigo
        DrawTexture(inimigo_atual.textura_inimigo,posicao_x_rect,inimigo_atual.posy,WHITE);
                // desenhar na tela o dinossauro
        DrawTexture(atual_textura,400, 80, WHITE);

                // Outlines
        DrawRectangleLinesEx(retangulo_dino, 1, BLUE); // O 1 é quanto forte é a linha
        DrawRectangleLinesEx(retangulo_inimigo, 1, RED);


        DrawText(TextFormat("PONTOS: %d", pontuacao), 10, 40, 20, BLACK);
            // O que ocorre ao colidir
if (colidiu) {
    DrawText("COLISAO!", 10, 10, 20, RED);
    salvarHighScore(pontuacao);
    EndDrawing();
    WaitTime(1.0);
    CloseWindow();
    main();  // reinicia o jogo do zero
    return 0;
}


        posicao_x_rect -= atual->qtd_decrescimo;


        //O que ocrre ao não colidir
        if (!colidiu) {
        frame_contador_pontuacao++; 
        if (frame_contador_pontuacao >= 60) {
        pontuacao++;
        frame_contador_pontuacao = 0;
    }
}

        EndDrawing();
        contador_frames++;
        contador_frames_passaro++;
    }
    CloseWindow();

    return 0;
}