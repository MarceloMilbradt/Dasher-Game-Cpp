#include "raylib.h"
#include <cmath>

struct AnimData //struct que representa um objeto animavel
{
    Rectangle rec;     //hitbox da animacao
    Vector2 pos;       //posicao da hitbox
    int frame;         //frame da animacao
    float updateTime;  //intervalo de atualizacao da animacao (s)
    float runningTime; //tempo que a animacao esta rodando
    float maxFrames;   //maximos de frames da animacao
};
struct BackGround
{
    Texture2D texture; //textura do fundo
    Vector2 pos;       //posicao do funco
    int velocity;      //velocidade em que o fundo se mexe
    float scale;       //escala da imagem no fundo
};
//atualiza o fundo, verificando se é necessario resetar sua posição
BackGround updateBackground(BackGround data, float deltaT)
{
    data.pos.x += data.velocity * deltaT;
    //se a imagem saiu do frame visivel é resetada sua posição
    if (data.pos.x <= -data.texture.width * data.scale)
    {
        data.pos.x = 0;
    }
    return data;
}
//verifica se está no chao
bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimation(AnimData data, float deltaT)
{
    //atualiza o running time
    data.runningTime += deltaT;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        //atualiza o frame de animacao
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > data.maxFrames)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{
    //dimencoes da window
    const int windowDimensions[2] = {
        960, //width
        600, //height
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
        5,        //maximo de frames
    };

    bool isInAir = false;
    int velocity = 0;         //valocidade do personagem no eixo y
    const int jumpVel = -600; //aceleração do pulo personagem no eixo y

    //Obstaculos
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png"); //textura
    const int numberOfNebulas = 10;
    AnimData nebulas[numberOfNebulas]{};
    for (int i = 0; i < numberOfNebulas; i++)
    {
        //rectangle(hitbox)
        nebulas[i].rec.y = 0.0;                           // y
        nebulas[i].rec.x = 0.0;                           // x
        nebulas[i].rec.width = nebula.width / (float)8;   // width
        nebulas[i].rec.height = nebula.height / (float)8; // height
        //vector2(posicao)
        nebulas[i].pos.y = windowDimensions[1] - nebula.height / 8; // y
        nebulas[i].pos.x = windowDimensions[0] + (i * 300);         // x

        nebulas[i].frame = 0;         // frame
        nebulas[i].runningTime = 0.0; //updateTime
        nebulas[i].updateTime = 0.0;  // running time
        nebulas[i].maxFrames = 7;
    }

    int nebVelocity = -200; //valocidade do obstaculo no eixo x

    //variaveis de ambiente
    const int gravity = 1'000; //gravidade por (pixles/s)/s

    //monta o fundo carregando as texturas
    const int numberOfBgs = 3;
    Texture2D textures[numberOfBgs] = {
        LoadTexture("textures/far-buildings.png"),
        LoadTexture("textures/back-buildings.png"),
        LoadTexture("textures/foreground.png"),
    };

    BackGround bgs[numberOfBgs];
    //monta o fundo usando a lista de texturas
    for (int i = 0; i < numberOfBgs; i++)
    {
        bgs[i].texture = textures[i];
        bgs[i].pos = {0.0, 0.0};
        bgs[i].velocity = -10 * pow(2, (i + 1));                                  //calcula a velocidade, sendo elas 20 40 80, com base em 2^i+1, dando um efeito de paralaxing
        bgs[i].scale = (textures[i].height * 100.0 / windowDimensions[1] / 10.0); // a escala é calculada pela porcentagem da resolução pelo tamanho da imagem
    }

    float finishLine = nebulas[numberOfNebulas - 1].pos.x;
    bool collision = false;

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

        //desenha o plano de fundo
        for (int i = 0; i < numberOfBgs; i++)
        {
            //atualiza os fundos
            bgs[i] = updateBackground(bgs[i], dT);

            /*adicionar um buffer de fundos com base na escala da resolução
            * sendo assim caso o scale for 3, terá 3 imagens em sequencia gerando fundo
            * dando a impressao de fundo infinito
            */
            for (int j = 0; j < bgs[i].scale; j++)
            {
                float bgxPos = bgs[i].pos.x + (bgs[i].texture.width * bgs[i].scale * j);
                //copia a posição em y do bg
                Vector2 bgPos{bgxPos, bgs[i].pos.y};
                DrawTextureEx(bgs[i].texture, bgPos, 0, bgs[i].scale, WHITE);
            }
        }

        //valida se é necessaio atualizar a animação, a cada updateTime(1/10s) atualiza 1 frame
        if (!isInAir)
        {
            scarfyData = updateAnimation(scarfyData, dT);
        }
        //verifica se esta no chao
        if (isOnGround(scarfyData, windowDimensions[1]))
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
            velocity += jumpVel; //adiciona a velocidade do pulo à velocidade do personagem, fazendo assim a aceleração dele
        }

        for (AnimData data : nebulas)
        {
            const float pad = 55;                      //padding, usado para aumentar a precisao das hitboxes
            Rectangle nebRec;                          //monta a hitbox das nebulas
            nebRec.x = data.pos.x + pad;               // x + padding
            nebRec.y = data.pos.y + pad;               // y + padding
            nebRec.width = data.rec.width - 2 * pad;   // width
            nebRec.height = data.rec.height - 2 * pad; // height

            Rectangle scarfyRec;                       //monta a hitbox do personagem
            scarfyRec.x = scarfyData.pos.x;            // x
            scarfyRec.y = scarfyData.pos.y;            // y
            scarfyRec.width = scarfyData.rec.width;    // width
            scarfyRec.height = scarfyData.rec.height;  // height
            if (CheckCollisionRecs(nebRec, scarfyRec)) //detecta a colisão entre as duas hitboxes
            {
                collision = true;
            }
        }

        finishLine += nebVelocity * dT; //move a linha de chegada

        //atualiza a pos do personagem
        scarfyData.pos.y += velocity * dT;

        if (scarfyData.pos.x >= finishLine) //se a posicao do personagem no eixo x for maior que a linha de chegada, o jogo termina como vitoria
        {
            DrawText("Vitória!!!", windowDimensions[0] / 2.4, windowDimensions[1] / 2, 40, WHITE);
        }
        else if (collision) //se houve colisão o jogo acaba, como derrota
        {
            DrawText("Game over", windowDimensions[0] / 2.4, windowDimensions[1] / 2, 40, WHITE);
        }
        else
        {
            //percorre a lista de nebulas atualizando sua posição
            for (int i = 0; i < (numberOfNebulas); i++)
            {
                nebulas[i] = updateAnimation(nebulas[i], dT);
                //atualiza a pos do obstaculo
                nebulas[i].pos.x += nebVelocity * dT;
                DrawTextureRec(nebula, nebulas[i].rec, nebulas[i].pos, WHITE);
            }
            //desenha o personagem e o obstaculo
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }
        //finaliza a renderizacao
        EndDrawing();
    }
    //finaliza o programa descarregando as texturas
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    for (int i = 0; i < numberOfBgs; i++)
    {
        UnloadTexture(bgs[i].texture);
    }

    CloseWindow();
}