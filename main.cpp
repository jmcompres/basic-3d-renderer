#include <SDL2/SDL.h>
#include <iostream>
#include <bits/stdc++.h>
#include <cmath>

//
//constantes
const double povAngle = M_PI/6;
double povAnglex = atan(-360/(240-360-240*(1/std::tan(povAngle))));
//
//variables globales
double yangle = 0;
double xangle = 0;

typedef struct
{
    double x,y,z;
    int *connected;
} D3POINT;
typedef struct 
{
    double x,y;
} D2POINT;
typedef struct
{
    D3POINT pc,p1,p2,p3,p4,p5,p6,p7,p8;
} CUBE;

//
//prototipos de funciones
void renderObjects(SDL_Renderer*,D3POINT*, D3POINT* ,D3POINT*,int);
void setCube(CUBE*,int,int,int);
void updatePosition(SDL_Event,D3POINT*);
void updateCamera(SDL_Event);
void drawLines(D3POINT*,D3POINT*,int,SDL_Renderer*);
void freeall(D3POINT*,int);

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("3D RENDERER", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "No se pudo crear el renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    //variables
    int pointsq = 13;
    bool running = true;
    D3POINT pov;
    pov.x = pov.z = pov.y = 0;
    pov.x = 10;
    pov.y = 5;
    CUBE c1;
    setCube(&c1,0,10,50);

    //
    //
    //Triángulo de prueba
    D3POINT tp1,tp2,tp3,tp4,tp5;
    tp1.connected = (int*)malloc(sizeof(int)*4), tp1.connected[0] = 10, tp1.connected[1] = 11, tp1.connected[2] = 13, tp1.connected[3] = -1;
    tp2.connected = (int*)malloc(sizeof(int)*3), tp2.connected[0] = 12, tp2.connected[1] = 13, tp2.connected[2] = -1;
    tp3.connected = (int*)malloc(sizeof(int)*3), tp3.connected[0] = 12, tp3.connected[1] = 13, tp3.connected[2] = -1;
    tp4.connected = (int*)malloc(sizeof(int)*2), tp4.connected[0] = 13, tp4.connected[1] = -1;
    tp5.connected = (int*)malloc(1*sizeof(int)), tp5.connected[0] = -1;

    tp1.x = tp3.x = -5;
    tp2.x = tp4.x = 5;
    tp1.y = tp2.y = tp3.y = tp4.y = 0;
    tp1.z = tp2.z = 5;
    tp3.z = tp4.z = 15;
    tp5.x = 0, tp5.y = 10, tp5.z = 10;
    //
    //
    //

    D3POINT points[] = {c1.p1,c1.p2,c1.p3,c1.p4,c1.p5,c1.p6,c1.p7,c1.p8,tp1,tp2,tp3,tp4,tp5};
    //variables de puntos de proyección
    D3POINT ppoints[pointsq];

    // Bucle principal
    while (running) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                //movimiento
                updatePosition(event,&pov);
                updateCamera(event);
            }
        }

        //limpiar
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        //dibujar líneas del cubo
        renderObjects(renderer,points, ppoints, &pov, pointsq);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(win);
    SDL_Quit();
    freeall(points, pointsq);

    return 0;
}

void setCube(CUBE *c, int cx, int cy, int cz)
{
    c->pc.x = cx;
    c->pc.y = cy;
    c->pc.z = cz;
    
    int dist = c->pc.y;

    c->p1.x = c->p3.x = c->p5.x = c->p7.x = c->pc.x-dist;
    c->p2.x = c->p4.x = c->p6.x = c->p8.x = c->pc.x+dist;
    c->p1.y = c->p2.y = c->p3.y = c->p4.y = 0;
    c->p5.y = c->p6.y = c->p7.y = c->p8.y = c->pc.y+dist;
    c->p1.z = c->p2.z = c->p5.z = c->p6.z = c->pc.z-dist;
    c->p3.z = c->p4.z = c->p7.z = c->p8.z = c->pc.z+dist;

    c->p1.connected = (int*)malloc(4*sizeof(int)), c->p1.connected[0] = 2, c->p1.connected[1] = 5, c->p1.connected[2] = 3, c->p1.connected[3] = -1;
    c->p2.connected = (int*)malloc(3*sizeof(int)), c->p2.connected[0] = 4, c->p2.connected[1] = 6, c->p2.connected[2] = -1;
    c->p3.connected = (int*)malloc(3*sizeof(int)), c->p3.connected[0] = 4, c->p3.connected[1] = 7, c->p3.connected[2] = -1;
    c->p4.connected = (int*)malloc(2*sizeof(int)), c->p4.connected[0] = 8, c->p4.connected[1] = -1;
    c->p5.connected = (int*)malloc(3*sizeof(int)), c->p5.connected[0] = 7, c->p5.connected[1] = 6, c->p5.connected[2] = -1;
    c->p6.connected = (int*)malloc(2*sizeof(int)), c->p6.connected[0] = 8, c->p6.connected[1] = -1;
    c->p7.connected = (int*)malloc(2*sizeof(int)), c->p7.connected[0] = 8, c->p7.connected[1] = -1;
    c->p8.connected = (int*)malloc(1*sizeof(int)), c->p8.connected[0] = -1;

    return;
}

void renderObjects(SDL_Renderer *renderer, D3POINT *points, D3POINT *ppoints, D3POINT *pov, int npoints)
{
    for (int i = 0; i<npoints; i++)
    {
        double tempz,tempx,tempy;

        //actualizar rotación
        tempz = (*(points+i)).z-pov->z;
        tempx = (*(points+i)).x-pov->x;
        tempy = (*(points+i)).y-pov->y;
        (*(ppoints+i)).x = (tempx)*std::cos(yangle)+((tempz)*std::sin(yangle));
        (*(ppoints+i)).z = -(tempx*std::sin(yangle))+(tempz*std::cos(yangle));
        (*(ppoints+i)).y = (tempy)*std::cos(xangle)-(((*(ppoints+i)).z)*std::sin(xangle));
        (*(ppoints+i)).z = (tempy)*std::sin(xangle)+(((*(ppoints+i)).z)*std::cos(xangle));
        (*(ppoints+i)).x += pov->x;
        (*(ppoints+i)).z += pov->z;
        (*(ppoints+i)).y += pov->y;

        //actualizar puntos de proyección
        (*(ppoints+i)).y = 240-240*((*(ppoints+i)).y-pov->y)/(((*(ppoints+i)).z-pov->z)*std::tan(povAngle));
        (*(ppoints+i)).x = 320+240*((*(ppoints+i)).x-pov->x)/(((*(ppoints+i)).z-pov->z)*std::tan(povAngle));
    }
    drawLines(ppoints,points, npoints,renderer);
}

void updatePosition(SDL_Event event, D3POINT *pov)
{
    SDL_Keycode keyPressed = event.key.keysym.sym;
    switch (keyPressed)
    {
        case SDLK_w:
            pov->z += cos(yangle);
            pov->x -= sin(yangle);
            break;
        case SDLK_s:
            pov->z -= cos(yangle);
            pov->x += sin(yangle);
            break;
        case SDLK_a:
            pov->x -= cos(yangle);
            pov->z -= sin(yangle);
            break;
        case SDLK_d:
            pov->x += cos(yangle);
            pov->z += sin(yangle);
            break;
        case SDLK_q:
            pov->y += 1;
            break;
        case SDLK_e:
            pov->y -= 1;
            break;
    }
}

void updateCamera(SDL_Event event)
{
    SDL_Keycode keyPressed = event.key.keysym.sym;
    switch (keyPressed)
    {
        case SDLK_i:
            xangle -= M_PI/90;
            break;
        case SDLK_k:
            xangle += M_PI/90;
            break;
        case SDLK_j:
            yangle += M_PI/90;
            break;
        case SDLK_l:
            yangle -= M_PI/90;
            break;
    }
}

void drawLines(D3POINT *ppoints, D3POINT *points, int pointsq, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    for (int i = 0; i<pointsq; i++)
    {
        D3POINT p = *(points+i);
        for (int j = 0; *(p.connected+j)!=-1;j++)
            SDL_RenderDrawLine(renderer,(*(ppoints+i)).x,(*(ppoints+i)).y,(*(ppoints+(*(p.connected+j))-1)).x,(*(ppoints+(*(p.connected+j))-1)).y);
    }
}

void freeall(D3POINT points[], int n)
{
    for (int i = 0; i<n; i++)
    {
        free(points[i].connected);
        points[i].connected = nullptr;
    }
}