#include "raylib.h"

int main()
{
    //dimencoes da window
    const int windowWidth = 800;
    const int windowHeight = 450;
    //abrir a window
    InitWindow(windowWidth, windowHeight, "Dasher");

    const int recWidth = 50;
    const int recHeight = 80;
    //margem inferior do retangulo
    int lb_rec = windowHeight - recHeight;
    int posY = lb_rec;

    bool isInAir = false;
    const int jumpVel = -22;
    int velocity = 0;
    //gravidade por (pixles/frame)/frame
    const int gravity = 1;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        //começa a renderizar
        BeginDrawing();
        ClearBackground(WHITE);

        lb_rec = posY + recHeight;

        //verifica se esta no chao
        if (lb_rec >= windowHeight)
        {
            //no chão
            velocity = 0;
            isInAir = false;
        }
        else
        {
            //no ar
            velocity += gravity;
            isInAir = true;
        }

        //pulo
        if (!isInAir && IsKeyPressed(KEY_SPACE))
        {
            velocity += jumpVel;
        }

        posY += velocity;
        DrawRectangle(windowWidth / 2, posY, recWidth, recHeight, BLUE);
        //finaliza a renderizacao
        EndDrawing();
    }
    CloseWindow();
}