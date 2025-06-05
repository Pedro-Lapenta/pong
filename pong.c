#include "raylib.h"
#include <stdio.h>

void UpdatePlayerRect(Vector2 *playerRectPos, float rectSpeed, int rectHeight, int screenHeight) {
    if (playerRectPos->y >= (screenHeight - rectHeight)) {
        if (IsKeyDown(KEY_UP))
            playerRectPos->y -= rectSpeed;
    }
    else if (playerRectPos->y <= 0) {
        if (IsKeyDown(KEY_DOWN))
            playerRectPos->y += rectSpeed;
    }
    else {
        if (IsKeyDown(KEY_UP))
            playerRectPos->y -= rectSpeed;
        if (IsKeyDown(KEY_DOWN))
            playerRectPos->y += rectSpeed;
    }
}

void UpdateAIRect(Vector2 *AIRectPos, float AIRectSpeed, int rectHeight, int screenHeight, Vector2 ballPosition, Vector2 ballSpeed, int rectWidth, int screenWidth) {
    if (ballSpeed.x > 0 && ballPosition.x > screenWidth / 2) {
        if (AIRectPos->y + rectHeight/2 < ballPosition.y)
            AIRectPos->y += AIRectSpeed;
        else if (AIRectPos->y + rectHeight/2 > ballPosition.y)
            AIRectPos->y -= AIRectSpeed;

        if (AIRectPos->y < 0) AIRectPos->y = 0;
        if (AIRectPos->y > screenHeight - rectHeight) AIRectPos->y = screenHeight - rectHeight;
    }
}

void BallBoardCollision(Vector2 *ballPosition, int ballRadius, int screenWidth, int screenHeight, Vector2 playerRectPos, Vector2 AIRectPos, int *playerScore, int *AIScore, bool *ballPaused) {
    // Ball Board Collision
    if ((ballPosition->x >= (screenWidth - ballRadius))){
        (*playerScore)++;

        ballPosition->x = (float)screenWidth/2;
        ballPosition->y = (float)screenHeight/2;

        ballPosition->x = playerRectPos.x + 20;
        ballPosition->y = playerRectPos.y;

        *ballPaused = true;
    }
    if (ballPosition->x <= ballRadius){
        (*AIScore)++;

        ballPosition->x = (float)screenWidth/2;
        ballPosition->y = (float)screenHeight/2;

        ballPosition->x = AIRectPos.x - 10;
        ballPosition->y = AIRectPos.y;

        *ballPaused = true;
    }
}

void RoofCollision(Vector2 *ballPosition, Vector2 *ballSpeed, int ballRadius, int screenHeight) {
    if ((ballPosition->y >= (screenHeight - ballRadius)) || (ballPosition->y <= ballRadius))
        ballSpeed->y *= -1.0f;
}

void BallRectCollision(Vector2 *ballPosition, Vector2 *ballSpeed, int ballRadius, Vector2 playerRectPos, Vector2 AIRectPos, int rectWidth, int rectHeight) {
    // Player Rect
    Rectangle playerRect = { playerRectPos.x, playerRectPos.y, rectWidth, rectHeight };
    Rectangle aiRect = { AIRectPos.x, AIRectPos.y, rectWidth, rectHeight };

    // Player collision
    if (CheckCollisionCircleRec(*ballPosition, ballRadius, playerRect)) {
        if (ballPosition->y < playerRect.y || ballPosition->y > playerRect.y + rectHeight) {
            ballSpeed->y *= -1.0f; // top/base collision
        } else {
            ballSpeed->x *= -1.0f; // side collision
        }
        printf("Ball -> PLAYER RECT\n");
        fflush(stdout);
    }

    // AI collision
    if (CheckCollisionCircleRec(*ballPosition, ballRadius, aiRect)) {
        if (ballPosition->y < aiRect.y || ballPosition->y > aiRect.y + rectHeight) {
            ballSpeed->y *= -1.0f;
        } else {
            ballSpeed->x *= -1.0f;
        }
        printf("Ball -> AI RECT\n");
        fflush(stdout);
    }
}

int main(void)
{
    // Init
    const int screenWidth = 800;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "Pong");
    
    SetTargetFPS(60); // Set FPS to 60
    // Game variables

    int playerScore = 0;
    int AIScore = 0;

    // ball
    Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };
    Vector2 ballSpeed = { 7.0f, 7.0f };
    int ballRadius = 7;
    Color ballColor = RAYWHITE;

    bool ballPaused = false;

    // Rect
    int rectWidth = 5;
    int rectHeight = 55;
    float rectSpeed = 10;
    float AIRectSpeed = 6.2;

 
    // Player rect
    Vector2 playerRectPos = {10, screenHeight/2 };

    // AI rect
    Vector2 AIRectPos =  {screenWidth - 10, screenHeight / 2};


    // Loop principal do jogo
    while (!WindowShouldClose())
    {

        // Updates

        // Ball

        if(ballPaused){
            if (IsKeyPressed(KEY_SPACE))
                ballPaused = false;
        } else {
            ballPosition.x += ballSpeed.x;
            ballPosition.y += ballSpeed.y;            
        }


        // Collisions

        // Player Rect Board Collision and movement
        UpdatePlayerRect(&playerRectPos, rectSpeed, rectHeight, screenHeight);

        // AI Rect Board Collision and movement
        UpdateAIRect(&AIRectPos, AIRectSpeed, rectHeight, screenHeight, ballPosition, ballSpeed, rectWidth, screenWidth);

        // Ball Board Collision
        BallBoardCollision(&ballPosition, ballRadius, screenWidth, screenHeight, playerRectPos, AIRectPos, &playerScore, &AIScore, &ballPaused);

        // Roof collision
        RoofCollision(&ballPosition, &ballSpeed, ballRadius, screenHeight);

        //Ball Rect Collision
        BallRectCollision(&ballPosition, &ballSpeed, ballRadius, playerRectPos, AIRectPos, rectWidth, rectHeight);

        // Draw
        BeginDrawing();
            ClearBackground(BLACK);
            
            // Draw Rects
            DrawRectangle((int)playerRectPos.x, (int)playerRectPos.y, rectWidth, rectHeight, BLUE);
            DrawRectangle((int)AIRectPos.x, (int)AIRectPos.y, rectWidth, rectHeight, RED);

            // Draw Ball
            DrawCircleV(ballPosition, ballRadius, ballColor);

            // Draw net
            DrawLine(screenWidth/2, 0, screenWidth/2, screenHeight, GRAY);

            // Draw "space to continue"
            if (ballPaused) {
                DrawText("Press SPACE to continue", screenWidth/2 - MeasureText("Press SPACE to continue", 20)/2, screenHeight/2 - 10, 20, RED);
            }

            // Draw Player Score
            char playerScoreText[32];
            sprintf(playerScoreText, "%d", playerScore);
            DrawText(playerScoreText, screenWidth/3 - MeasureText(playerScoreText, 30)/2, 20, 30, BLUE);

            // Draw AI Score
            char AIScoreText[32];
            sprintf(AIScoreText, "%d", AIScore);
            DrawText(AIScoreText, 2*screenWidth/3 - MeasureText(AIScoreText, 30)/2, 20, 30, RED);

        EndDrawing();
    }

    // End
    playerScore = 0;
    AIScore = 0;

    CloseWindow();

    return 0;
}