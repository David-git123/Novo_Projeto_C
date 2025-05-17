
#include "raylib.h"
#include <stdlib.h>
#include <string.h>


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


int main(void)
{
    Color cor = BLACK;
    const int screenWidth = 800;
    const int screenHeight = 450;
    int posicao_x_rect = 798;
    int numero = 0;
    int contador = 0;
    int contador_frames = 0;
    int contador_frames_passaro = 0;

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
    SetTargetFPS(60);

    dificuldade* atual = dificuldade_1;
    Texture2D dinossauro = LoadTexture("C:/Users/Twobr/Downloads/dinossauro_1.png");
    Texture2D dinossauro_esq = LoadTexture("C:/Users/Twobr/Downloads/dinossauro_esq.png");
    Texture2D dinossauro_dir = LoadTexture("C:/Users/Twobr/Downloads/dinossauro_dir.png");
    Texture2D atual_textura=dinossauro;

    Texture2D cacto = LoadTexture("C:/Users/Twobr/Downloads/cacto-Photoroom.png");
    Texture2D big_cacto = LoadTexture("C:/Users/Twobr/Downloads/big_cacto-Photoroom.png");
    Texture2D passaro_down = LoadTexture("C:/Users/Twobr/Downloads/passaro_down-Photoroom.png");
    Texture2D passaro_up = LoadTexture("C:/Users/Twobr/Downloads/passaro_up-Photoroom.png");
    

    inimigo_1.textura_inimigo = cacto;
    inimigo_2.textura_inimigo = passaro_up;
    inimigo_3.textura_inimigo = big_cacto;
    inimigos inimigo_atual = inimigo_1;
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
        DrawTexture(inimigo_atual.textura_inimigo,posicao_x_rect,inimigo_atual.posy,WHITE);
        DrawTexture(atual_textura,400, 80, WHITE);
        posicao_x_rect -= atual->qtd_decrescimo;
        EndDrawing();
        contador_frames++;
        contador_frames_passaro++;
    }
    CloseWindow();

    return 0;
}