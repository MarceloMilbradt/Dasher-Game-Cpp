#include "raylib.h"

int main()
{
    //dimencoes da window
    const int windowWidth = 800;
    const int windowHeight = 450;
    //abrir a window
    InitWindow(windowWidth, windowHeight, "Dasher");

    //personagem
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec{
        0.0,              // x
        0.0,              // y
        scarfy.width / 6, // width
        scarfy.height     // height
    };
    Vector2 scarfyPos{
        windowWidth / 2 - scarfyRec.width / 2, // x
        windowHeight / 2 - scarfyRec.height    // y
    };

    bool isInAir = false;
    int velocity = 0;         //valocidade do personagem no eixo y
    const int jumpVel = -600; //aceleração do pulo personagem no eixo y

    //Obstaculos
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    Rectangle nebulaRec{
        0.0,                     // x
        0.0,                     // y
        nebula.width / (float)8, // width
        nebula.height / (float)8 // height
    };

    Vector2 nebulaPos{
        windowWidth,                    // x
        windowHeight - nebulaRec.height // y
    };

    int nebVelocity = -600; //valocidade do obstaculo no eixo x
    
    //variaveis de ambiente
    int frame = 0;                       //frame atual da animação
    const float updateTime = 1.0 / 10.0; //intervalo de tempo (s) até a proxima atualização da animação
    float runningTime = 0;               //tempo consecutivo que o personagem está no chão
    const int gravity = 1'000;           //gravidade por (pixles/s)/s

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

        runningTime += dT;
        //valida se é necessaio atualizar a animação, a cada updateTime(1/10s) atualiza 1 frame
        if (runningTime >= updateTime && !isInAir)
        {
            scarfyRec.x = frame * scarfyRec.width; // pega a animação correspondente no spritesheet
            frame++;
            if (frame > 5) //se chegar no ultimo frame da animação volta para 0
            {
                frame = 0;
            }
            runningTime = 0.0;
        }
        //verifica se esta no chao
        if (scarfyPos.y >= windowHeight - scarfyRec.height)
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
        scarfyPos.y += velocity * dT;

        //atualiza a pos do obstaculo
        nebulaPos.x += nebVelocity * dT;

        //desenha o personagem e o obstaculo
        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);
        DrawTextureRec(nebula, nebulaRec, nebulaPos, WHITE);
        //finaliza a renderizacao
        EndDrawing();
    }
    //finaliza o programa descarregando as texturas
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}