
#include "raylib.h"
#include <stdlib.h>
#include <string.h>


typedef struct dificuldade {
    int qtd_decrescimo;
    int max_loops;
    int tempo_spawn;
    struct dificuldade* proximo;
}dificuldade;



int main(void)
{
    Color cor = BLACK;
    const int screenWidth = 800;
    const int screenHeight = 450;
    int posicao_x_rect = 798;
    int numero = 0;
    int contador = 0;

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
    while (!WindowShouldClose())
    {
        if (contador == atual->max_loops) {
            atual = atual->proximo;
        }

        if (posicao_x_rect <= atual->tempo_spawn) {
            posicao_x_rect = 800;
            contador++;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
       
        DrawRectangle(posicao_x_rect, 100, 100, 100, cor);
        DrawCircle(20, 125, 20, RED);
        posicao_x_rect -= atual->qtd_decrescimo;
        EndDrawing();
        
    }
    CloseWindow();

    return 0;
}
