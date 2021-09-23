#include "raylib.h"

int main()
{
    //dimencoes da window
    const int windowWidth = 800;
    const int windowHeight = 450;
    //abrir a window
    InitWindow(windowWidth, windowHeight, "Dasher");

    //gravidade por (pixles/s)/s
    const int gravity = 1'000;

    //personagem
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec;
    Vector2 scarfyPos;

    scarfyRec.width = scarfy.width / 6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;

    scarfyPos.x = windowWidth / 2 - scarfyRec.width / 2;
    scarfyPos.y = windowHeight / 2 - scarfyRec.height;
    bool isInAir = false;
    int velocity = 0;  //no eixo y
    const int jumpVel = -600; //no eixo y

    //Obstaculos
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    Rectangle nebulaRec{
        0.0,                // x
        0.0,                // y
        nebula.width / (float)8, // width
        nebula.height / (float)8 // height
    };
    Vector2 nebulaPos{
        windowWidth,                    // x
        windowHeight - nebulaRec.height // y
    };
    int nebVelocity = -600; //no eixo x

    int frame = 0;
    const float updateTime = 1.0 / 10.0;
    float runningTime = 0;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        /* Pega o delta T (tempo desde o ultimo frame)
        *  garante que a aceleração será a mesma a cada frame
        */
        const float dT = GetFrameTime();

        //começa a renderizar
        BeginDrawing();
        ClearBackground(WHITE);
        runningTime += dT;
        if (runningTime >= updateTime && !isInAir)
        {
            scarfyRec.x = frame * scarfyRec.width;
            frame++;
            if (frame > 5)
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

        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);
        DrawTextureRec(nebula, nebulaRec, nebulaPos, WHITE);
        //finaliza a renderizacao
        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}