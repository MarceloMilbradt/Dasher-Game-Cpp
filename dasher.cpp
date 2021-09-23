#include "raylib.h"

struct AnimData //struct que representa um objeto animavel
{
    Rectangle rec;     //hitbox da animacao
    Vector2 pos;       //posicao da hitbox
    int frame;         //frame da animacao
    float updateTime;  //intervalo de atualizacao da animacao (s)
    float runningTime; //tempo que a animacao esta rodando
};

int main()
{
    //dimencoes da window
    const int windowDimensions[2] = {
        800, //width
        450, //height
    };
    //abrir a window
    InitWindow(windowDimensions[0], windowDimensions[1], "Dasher");

    //personagem
    //textura
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData{
        {
            //rectangle(hitbox)
            0.0,                     // x
            0.0,                     // y
            (float)scarfy.width / 6, // width
            (float)scarfy.height     // height
        },
        {
            //vector2(posicao)
            (float)windowDimensions[0] / 2 - (scarfy.width / 6) / 2, // x
            (float)windowDimensions[1] / 2 - (scarfy.height)         // y
        },
        0,        // frame
        1 / 10.0, //updateTime
        0,        // running time
    };

    bool isInAir = false;
    int velocity = 0;         //valocidade do personagem no eixo y
    const int jumpVel = -600; //aceleração do pulo personagem no eixo y

    //Obstaculos
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png"); //textura
    AnimData nebulaData{
        {
            //rectangle(hitbox)
            0.0,                     // x
            0.0,                     // y
            nebula.width / (float)8, // width
            nebula.height / (float)8 // height
        },
        {
            //vector2(posicao)
            (float)windowDimensions[0],                    // x
            (float)windowDimensions[1] - nebula.height / 8 // y
        },
        0,      // frame
        1 / 10, //updateTime
        0,      // running time
    };

    int nebVelocity = -200; //valocidade do obstaculo no eixo x

    //variaveis de ambiente
    const int gravity = 1'000; //gravidade por (pixles/s)/s

    //seta os frames a 60 fps
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        /* Pega o ∆T(delta T) (tempo desde o ultimo frame)
        *  garante que a aceleração será a mesma a cada frame, independente de framerate
        */
        const float dT = GetFrameTime();

        //começa a renderizar
        BeginDrawing();
        ClearBackground(WHITE);

        scarfyData.runningTime += dT;
        nebulaData.runningTime += dT;
        //valida se é necessaio atualizar a animação, a cada updateTime(1/10s) atualiza 1 frame
        if (scarfyData.runningTime >= scarfyData.updateTime && !isInAir)
        {
            scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width; // pega a animação correspondente no spritesheet
            scarfyData.frame++;
            if (scarfyData.frame > 5) //se chegar no ultimo frame da animação volta para 0
            {
                scarfyData.frame = 0;
            }
            scarfyData.runningTime = 0.0;
        }

        if (nebulaData.runningTime >= nebulaData.updateTime)
        {
            nebulaData.rec.x = nebulaData.frame * nebulaData.rec.width; // pega a animação correspondente no spritesheet
            nebulaData.frame++;
            if (nebulaData.frame > 8) //se chegar no ultimo frame da animação volta para 0
            {
                nebulaData.frame = 0;
            }
            nebulaData.runningTime = 0.0;
        }
        //verifica se esta no chao
        if (scarfyData.pos.y >= windowDimensions[1] - scarfyData.rec.height)
        {
            //no chão
            velocity = 0;
            isInAir = false;
        }
        else
        {
            //no ar
            velocity += gravity * dT;
            isInAir = true;
        }

        //pulo
        if (!isInAir && IsKeyPressed(KEY_SPACE))
        {
            velocity += jumpVel;
        }
        //atualiza a pos do personagem
        scarfyData.pos.y += velocity * dT;

        //atualiza a pos do obstaculo
        nebulaData.pos.x += nebVelocity * dT;

        //desenha o personagem e o obstaculo
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        DrawTextureRec(nebula, nebulaData.rec, nebulaData.pos, WHITE);
        //finaliza a renderizacao
        EndDrawing();
    }
    //finaliza o programa descarregando as texturas
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}