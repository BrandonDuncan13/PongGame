#include "raylib.h"

struct Ball // ball game object
{
	// attributes or member variables for a ball object
	float x, y;
	float speedX, speedY;
	float radius;

	void draw() // method to draw the ball
	{
		DrawCircle((int) x, (int) y, radius, WHITE);
	}
};

struct Paddle // paddle game object
{
	float x, y;
	float speed;
	float width, height;

	Rectangle getRect()
	{
		return Rectangle{ x - (width / 2), y - (height / 2), width, height };
	}

	void draw() // draw a paddle starting from the middle
	{
		DrawRectangleRec(getRect(), WHITE);
	}
};

int main()
{
	// opens the application window
	InitWindow(800, 600, "Pong Arcade Game");
	SetWindowState(FLAG_VSYNC_HINT);

	// create a ball object
	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 5;
	ball.speedX = 300;
	ball.speedY = 300;

	// create a paddle object for the left paddle
	Paddle leftPaddle;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.x = 50;
	leftPaddle.y = (GetScreenHeight() / 2);
	leftPaddle.speed = 500;

	// now for the right paddle
	Paddle rightPaddle;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.x = (GetScreenWidth() - 50);
	rightPaddle.y = (GetScreenHeight() / 2);
	rightPaddle.speed = 500;

	const char* winnerText = nullptr;

	// game loop
	while (!WindowShouldClose())
	{
		// multiply by frame time since different laptops will call game loop at different FPS
		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();

		if (ball.y > GetScreenHeight())
		{
			// sometimes ball gets stuck at bottom of screen due to a large GetFrameTime
			ball.y = GetScreenHeight();
			// change direction of ball
			ball.speedY *= -1;
		}
		else if (ball.y < 0)
		{
			ball.y = 0;
			ball.speedY *= -1;
		}

		if (IsKeyDown(KEY_W)) // recieving input for the left paddle
		{
			leftPaddle.y -= leftPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_S))
		{
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_UP)) // recieving input for the right paddle
		{
			rightPaddle.y -= rightPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_DOWN))
		{
			rightPaddle.y += rightPaddle.speed * GetFrameTime();
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle // check for ball collisions with left paddle
			{ leftPaddle.getRect() }))
		{
			if (ball.speedX < 0) // to prevent multiple hits on same paddle only check collision once
			{
				// change direction of ball and add speed multiplier
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle // check for ball collisions with left paddle
			{ rightPaddle.getRect() }))
		{
			if (ball.speedX > 0) // only check collision when ball traveling right
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
			}
		}

		if (ball.x < 0) // display who won the game
		{
			winnerText = "Right Player Wins!";
		}
		if (ball.x > GetScreenWidth())
		{
			winnerText = "Left Player Wins!";
		}
		if (winnerText && IsKeyPressed(KEY_SPACE)) // resets the game when space is pressed
		{
			ball.x = (GetScreenWidth() / 2);
			ball.y = (GetScreenHeight() / 2);
			ball.speedX = 300;
			ball.speedY = 300;
			winnerText = nullptr;
		}
		
		// starts rendering process
		BeginDrawing();
			// colors background of window
			ClearBackground(BLACK);

			// draw the ball and both paddles on the screen from their centers
			ball.draw();
			leftPaddle.draw();
			rightPaddle.draw();

			if (winnerText)
			{
				int textWidth = MeasureText(winnerText, 60);
				DrawText(winnerText, (GetScreenWidth() / 2) - (textWidth / 2), (GetScreenHeight() / 2) - 30, 60, YELLOW);
			}

			DrawFPS(10, 10);
			// ends rendering process and handles events
		EndDrawing();
	}

	// closes the application window
	CloseWindow();

	return 0;
}
