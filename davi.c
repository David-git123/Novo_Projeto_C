#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



typedef struct dificuldade {
    int qtd_decrescimo;// Velocidade que os obstaculos passam na tela no eixo x
    int max_loops;// Maximo de loops para mudar de dificuldade (1 loop = posicao 800 até a 0)
    int tempo_spawn;// "delay" entre spawm de obstaculos, é um tipo de contador de frames
    struct dificuldade* proximo;
}dificuldade;


typedef struct inimigos {
    int altura; // até onde da posicao x o inimigo vai
    int largura;// quanto ele pega do eixo x
    int posy;// posicao y onde o inimigo vai começar
    Texture2D textura_inimigo;// Texture2D é uma struct da raylib, variaveis Texture2D conseguem armazenar png's carregados.
    int id;// para identificar o inimigo
}inimigos;


typedef struct powerups {
    int altura;
    int largura;
    int posy;
    Texture2D textura_powerup;
    int id;
} powerups;
powerups powerup_atual;



int lerarquivo() {
    int highScore;
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
    srand(time(NULL));
    const int screenWidth = 800;
    const int screenHeight = 450;
    int posicao_x_inimigo = 798;
    int numero_aleatorio_inimigo = 0;
    int contador_loops_inimigo = 0;
    int contador_frames= 0;// Contador de frames que vai de 0 32.
    int contador_frames_passaro = 0; // para mudar a textura do passaro

    int pontuacao = 0;
    int frame_contador_pontuacao = 0;

    int frame_contador_caracol = 0;
    int posicao_x_powerup = 0;
    bool powerup_visivel = false;  // powerup começa sem ta visivel
    int frame_powerup_spawn = 0;   // contagem em frames do powerup

    int frame_contador_fogo = 0;
    bool invencibilidade = false;
    int frame_contador_invencibilidade = 0;

    int quadrado_x = 400;// posicao x do dinossauro
    int quadrado_y = 330;// posicao y do dino
    int contador_frames_pulo = 0;
    float valor_decrescimo_pulo = 2.5;
    bool pode_pular = true;

    int cenario_1_x = 0;// A textura do cernario tem 800 pixels, essa vai de 0 a 800
    int cenario_2_x = 800;// Essa vai de 800 a 1600

    dificuldade* dificuldade_1 = malloc(sizeof(dificuldade));
    dificuldade* dificuldade_2 = malloc(sizeof(dificuldade));
    dificuldade* dificuldade_3 = malloc(sizeof(dificuldade));
    dificuldade* dificuldade_4 = malloc(sizeof(dificuldade));
    dificuldade* dificuldade_5 = malloc(sizeof(dificuldade));

    if (dificuldade_1 == NULL || dificuldade_2 == NULL || dificuldade_3 == NULL || dificuldade_4 == NULL || dificuldade_5 == NULL) {// verificacao para saber se deu erro na alocação
        InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
        BeginDrawing();
        ClearBackground(BLUE);// tela azul
        EndDrawing();
        return 0;
    }

    inimigos inimigo_1; // cacto
    inimigos inimigo_2; // passaro
    inimigos inimigo_3; // big_cacto

    inimigo_1.posy = 330;
    inimigo_1.altura = 30;
    inimigo_1.largura = 80;
    inimigo_1.id = 0;

    inimigo_2.posy = 300;
    inimigo_2.altura = 20;
    inimigo_2.largura = 50;
    inimigo_2.id = 1;

    inimigo_3.posy = 330;
    inimigo_3.altura = 70;
    inimigo_3.largura = 40;
    inimigo_3.id = 2;

    dificuldade_1->qtd_decrescimo = 3;
    dificuldade_1->max_loops = 2;
    dificuldade_1->tempo_spawn = -400;
    dificuldade_1->proximo = dificuldade_2;

    dificuldade_2->qtd_decrescimo = 5;
    dificuldade_2->max_loops = 8;
    dificuldade_2->tempo_spawn = -300;
    dificuldade_2->proximo = dificuldade_3;

    dificuldade_3->qtd_decrescimo = 8;
    dificuldade_3->max_loops = 20;
    dificuldade_3->tempo_spawn = -200;
    dificuldade_3->proximo = dificuldade_4;

    dificuldade_4->qtd_decrescimo = 10;
    dificuldade_4->max_loops = 30;
    dificuldade_4->tempo_spawn = -200;
    dificuldade_4->proximo = dificuldade_5;

    dificuldade_5->qtd_decrescimo = 15;
    dificuldade_5->max_loops = 40;
    dificuldade_5->tempo_spawn = -100;
    dificuldade_5->proximo == NULL;


    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60); // 60 frames por segundo
    // Eu só consigo carregar texturas se a janela tiver iniciado
    dificuldade* atual = dificuldade_1;

    Texture2D dinossauro_parado = LoadTexture("resources/dinossauro_parado.png");
    Texture2D dinossauro_esq = LoadTexture("resources/dinossauro_em_pe_1.png");
    Texture2D dinossauro_dir = LoadTexture("resources/dinossauro_em_pe_2.png");
    Texture2D dinossauro_down_dir = LoadTexture("resources/dinossauro_deitado_2.png");
    Texture2D dinossauro_down_esq = LoadTexture("resources/dinossauro_deitado_1.png");

    Texture2D textura_dino_atual = dinossauro_parado;


    Texture2D cenario_1 = LoadTexture("resources/cenario_1.png");
    Texture2D cenario_2 = LoadTexture("resources/cenario_1.png");

    Texture2D cacto = LoadTexture("resources/cacto2.png");
    Texture2D big_cacto = LoadTexture("resources/cacto1.png");
    Texture2D passaro_down = LoadTexture("resources/passaro1.png");
    Texture2D passaro_up = LoadTexture("resources/passaro2.png");

    Texture2D powerup0 = LoadTexture("resources/5.png");
    Texture2D powerup1 = LoadTexture("resources/caracol.png");
    Texture2D powerup2 = LoadTexture("resources/botafogo.png");
    Texture2D powerup3 = LoadTexture("resources/fogo.png");

    Texture2D powerups_texturas[4] = { powerup0, powerup1, powerup2, powerup3 }; // aqui eu coloco os 4 powerups

    inimigo_1.textura_inimigo = cacto;
    inimigo_2.textura_inimigo = passaro_up;
    inimigo_3.textura_inimigo = big_cacto;
    inimigos inimigo_atual = inimigo_1;



    // Textures já foram carregadas anteriormente (powerup1, powerup2, etc.)


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
        contador_frames++;
        contador_frames_passaro++;
        frame_powerup_spawn++;

        if (contador_loops_inimigo == atual->max_loops) {// vai verificar em qual loop eu estou, e mudar a dificuldade baseado nisso.
            atual = atual->proximo;
        }

        if (posicao_x_inimigo <= atual->tempo_spawn) {
            posicao_x_inimigo = 800;// respawno inimigo na poisição 800
            contador_loops_inimigo++;// Ocorreu um loop
            numero_aleatorio_inimigo = rand() % 3;// vai me dar um número de 0 a 2
            if (numero_aleatorio_inimigo == inimigo_1.id) {
                inimigo_atual = inimigo_1;// textura do inmigo atual igual a textura do inimigo 1 e variaveis da struct
            }

            else if (numero_aleatorio_inimigo == inimigo_2.id) {
                inimigo_atual = inimigo_2;
            }
            else if (numero_aleatorio_inimigo == inimigo_3.id) {
                inimigo_atual = inimigo_3;
            }
        }

        if (inimigo_atual.id == 1) {// para mudar a textura do passaro. para bater as asas
            if (contador_frames_passaro <= 16) {
                inimigo_atual.textura_inimigo = passaro_up;
            }
            else if (contador_frames_passaro >= 16 && contador_frames_passaro <= 32) {
                inimigo_atual.textura_inimigo = passaro_down;
            }
            else {
                contador_frames_passaro = 0;
            }
        };

        //IsKeyDown: Verifica se uma tecla foi pressionada
        //IsKeyUp: Verifica se a tecla não está pressionada == !IsKeyDown

        if (IsKeyDown(KEY_UP) && contador_frames_pulo < 10 && pode_pular == true) {// contador frames pulo serve para limitar quantidade de frames que terá para pular. Será incrementado enquanto apertar botão
            quadrado_y -= 20;// vou subir.
            contador_frames_pulo++;
            textura_dino_atual = dinossauro_parado;
        }
        if (IsKeyReleased(KEY_UP) || contador_frames_pulo == 10) {
            pode_pular = false;// Pode pular so vai ser false enquanto ele estiver caindo
        }
        if (pode_pular == false) {
            quadrado_y += valor_decrescimo_pulo;// começa a cair.
            if (IsKeyUp(KEY_DOWN)) {// para saber se não clicou no botão de cair, se cair vai imediatamente para baixo.
                if (quadrado_y >= 330) {// para saber se aterrisou
                    quadrado_y = 330;
                    contador_frames_pulo = 0;
                    pode_pular = true;
                }
            }
        }

        // carregamento de texturas e se abaixar.
        if (IsKeyDown(KEY_DOWN) && contador_frames <= 16) {// 16 frames iniciais ele passa com perna direita levantada. Aqui ele está abaixado
            textura_dino_atual = dinossauro_down_dir;
            quadrado_y = 360;// diminuir a altura quando abaixo
        }
        else if (IsKeyDown(KEY_DOWN) && contador_frames >= 16 && contador_frames <= 32) {// 16 seguintes com a perna esquerda. aqui está abaixado
            textura_dino_atual = dinossauro_down_esq;
            quadrado_y = 360;
        }
        else if (contador_frames <= 16 && quadrado_y == 330) { //Quando está em pé
            textura_dino_atual = dinossauro_dir;
        }
        else if (contador_frames >= 16 && contador_frames <= 32 && quadrado_y == 330) {// Quando está em pé
            textura_dino_atual = dinossauro_esq;
        }
        else {// Resetar quando contador de frames for maior que 32.
            contador_frames = 0;
        }

        if (IsKeyReleased(KEY_DOWN)) {
            quadrado_y = 330;
        }

        // Logica do spawn do powerup
        if (frame_powerup_spawn >= 300 && powerup_visivel == false) {
            int m[2][2] = { {0, 1}, {2, 3} };
            int linha = rand() % 2;
            int coluna = rand() % 2;
            int tipo = m[linha][coluna];  //so faz o sorteio uma vez,  todo o momento em que ele aprecer.

            powerup_atual.textura_powerup = powerups_texturas[tipo];
            powerup_atual.id = tipo;
            powerup_atual.posy = 270;
            powerup_atual.altura = 50;
            powerup_atual.largura = 50;

            posicao_x_powerup = 800;
            powerup_visivel = true;
        }


        // powerup vai diminuindo a cada frame 2 de x
        if (powerup_visivel == true) {
            posicao_x_powerup -= 2;
            if (posicao_x_powerup < 0) {
                powerup_visivel = false; // Esconde powerup após sair da tela
                frame_powerup_spawn = 0;
            }
        }

        if (frame_contador_caracol > 0) {
            posicao_x_inimigo -= 1;
            frame_contador_caracol--;
        }
        else if (frame_contador_fogo > 0) {
            posicao_x_inimigo -= atual->qtd_decrescimo + 2;
            frame_contador_fogo--;
        }
        else {
            posicao_x_inimigo -= atual->qtd_decrescimo;
        }


        if (frame_contador_invencibilidade > 0) {
            invencibilidade = true;
            frame_contador_invencibilidade--;
        }
        else {
            invencibilidade = false;
        }

        cenario_1_x -= dificuldade_1->qtd_decrescimo;
        cenario_2_x -= dificuldade_1->qtd_decrescimo;


        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (cenario_1_x <= -800) cenario_1_x = 800;
        if (cenario_2_x <= -800) cenario_2_x = 800;
        DrawTexture(cenario_1, cenario_1_x, 0, WHITE);
        DrawTexture(cenario_2, cenario_2_x, 0, WHITE);

        Rectangle retangulo_dino = {
            quadrado_x,
            quadrado_y,
            (float)textura_dino_atual.width,
            (float)textura_dino_atual.height };

        Rectangle retangulo_inimigo = {
            posicao_x_inimigo,
            inimigo_atual.posy,
            (float)inimigo_atual.textura_inimigo.width,
            (float)inimigo_atual.textura_inimigo.height
        };


        Rectangle retangulo_powerup = {
            (float)posicao_x_powerup,
            (float)powerup_atual.posy,
            (float)powerup_atual.largura,
            (float)powerup_atual.altura
        };
        // Colisão com powerup
        bool ColidiuPowerup = CheckCollisionRecs(retangulo_dino, retangulo_powerup);

        // Colisão com inimigo
        bool colidiu = CheckCollisionRecs(retangulo_dino, retangulo_inimigo);

        // desenhar os objetos
        DrawTexture(inimigo_atual.textura_inimigo, posicao_x_inimigo, inimigo_atual.posy, WHITE);// Draw texture desenha o PNG que está na variavel. Quarto paramentro muda cor da textura
        DrawTexture(textura_dino_atual, quadrado_x, quadrado_y, WHITE);// Quadrado x é constate, posição x do dino. quadrado y muda, posição y do dino

        // Outlines


        if (powerup_visivel == true) {
            DrawTexture(powerup_atual.textura_powerup, posicao_x_powerup, powerup_atual.posy, WHITE);
        }


        // pontuação display
        DrawText(TextFormat("PONTOS: %d", pontuacao), 10, 40, 20, BLACK);



        //O que ocrre ao não colidir
        if (colidiu == false) {
            frame_contador_pontuacao++;
            if (frame_contador_pontuacao >= 60) {
                pontuacao++;
                frame_contador_pontuacao = 0;
            }
        }
        //             // O que ocorre ao colidir
        if (colidiu == true && invencibilidade == false) {
            DrawText("COLISAO!", 10, 10, 20, RED);
            salvarHighScore(pontuacao);
            EndDrawing();
            WaitTime(1.0);
            CloseWindow();
            main();  // reinicia o jogo do zero
            return 0;
        }



        if (ColidiuPowerup == true && powerup_visivel == true) {
            if (powerup_atual.id == 0) {
                pontuacao += 5;

            }
            else if (powerup_atual.id == 1) {
                frame_contador_caracol = 360;
            }
            else if (powerup_atual.id == 2) {
                frame_contador_invencibilidade = 360;
            }
            else if (powerup_atual.id == 3) {
                frame_contador_fogo = 360;
            }

            powerup_visivel = false;
            frame_powerup_spawn = 0;
        }




        EndDrawing();
    }
    CloseWindow();

    return 0;
}
