#ifndef _APP_H_
#define _APP_H_

#include <raylib.h>

#if !defined(RAYGUI_IMPLEMENTATION)
    #include "raygui.h"
#endif

#include "ping_pong/PingPong.h"
#include "arkanoid/Arkanoid.h"
#include "asteroids/Asteroids.h"

class App {
public:
    App() {
        InitWindow(screenWidth, screenHeight, "Raylib Projects");

        center = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

        SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

        runningApp = nullptr;
    }

    ~App() {
        CloseWindow();        // Close window and OpenGL context
    }

    void Run() {
        // Main game loop
        while (!WindowShouldClose())    // Detect window close button or ESC key
        {
            // Update
            //----------------------------------------------------------------------------------
            // NOTE: All variables update happens inside GUI control functions
            //----------------------------------------------------------------------------------

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();

                ClearBackground(RAYWHITE);

                if(runningApp != nullptr) {
                    if(runningApp->Closed()) {
                        delete runningApp;
                        runningApp = nullptr;
                        
                    } else {
                        runningApp->Run();
                    }
                } else {
                    if (GuiButton({center.x - 50, 100, 100, 20}, "Ping Pong"))
                    {
                        runningApp = new PingPong::PingPong();
                    }
                    else if (GuiButton({center.x - 50, 130, 100, 20}, "Arkanoid"))
                    {
                        runningApp = new Arkanoid::Arkanoid();
                    }
                    else if (GuiButton({center.x - 50, 160, 100, 20}, "Asteroids"))
                    {
                        runningApp = new Asteroids::Asteroids();
                    }
                } 

                DrawFPS(10, 10);


            EndDrawing();
            //----------------------------------------------------------------------------------
        }

    }

private:
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    Vector2 center;

    float outerRadius = 180.0f;
    float startAngle = 0.0f;
    float endAngle = 180.0f;
    float segments = 10.0f;
    float minSegments = 4;

    AppBase* runningApp;

};

#endif