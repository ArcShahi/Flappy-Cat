#include "raylib.h"
#include <iostream>
#include <vector>

namespace rl
{
	constexpr int WIDTH = 1200;
	constexpr int HIGHT = 900;
}


class Cat
{
public:
	Cat()
		:x{ 200 },y{rl::HIGHT/2}
	{}

public:

	void show()
	{
		DrawCircle(int(x), int(y), radius, RED);
		DrawCircleLines(int(x), int(y), radius, BLACK);
	}

	void update(float dt)
	{
		this->velocity.y += gravity;
		this->y += velocity.y*dt;
		
		check_bounds();
	}

	void cat_leap(){  velocity.y += up_thrust; }

public:

	float x{};
	float y{};
	const float radius{ 16 };


private:

	void check_bounds()
	{
		// stops when it falls to ground

		if (this->y > rl::HIGHT)
		{
			this->y = rl::HIGHT;
			this->velocity.y = 0.0f;
		}

		if (this->y < 0)
		{
			this->y = radius;
			this->velocity.y = 0.0f;

		}
			

	}

private:

	const float gravity{ 6.54f };
	const float up_thrust{ -200.f };
	Vector2 velocity{};
	Vector2 acceleration{};
};


class Wall
{
public:
	Wall()
		:top( GetRandomValue(100,rl::HIGHT / 2) ),bottom(GetRandomValue(100,rl::HIGHT/2)),
		  spawn_x{rl::WIDTH}
	{}
	
	void show()
	{
		DrawRectangle(this->spawn_x, 0, thicc, top, GRAY); 
		DrawRectangle(this->spawn_x, rl::HIGHT - bottom, thicc, bottom, GRAY);

		
	}

	bool off_screen() const{ return spawn_x + thicc < 0; }

	void update(float dt)
	{
		this->spawn_x -= speed*dt;
	}

public:

	float top{};
	float bottom{};
	float spawn_x{};
	float thicc{ 50 };
	
private:

	 float speed{250}; // wall moving speed

};


int main()
{

	Cat cat{};
	// Array of walls
	std::vector<Wall> walls{};
	float spawn_t = 0.f;


	InitWindow(rl::WIDTH, rl::HIGHT, "Flappy Cat");
	SetTargetFPS(120);

	while (!WindowShouldClose())
	{
		auto dt = GetFrameTime();

		// accumulate time
		
		spawn_t += dt;

		if (IsKeyPressed(KEY_SPACE)){  cat.cat_leap(); }

		if (spawn_t >= 1.f) { // every 2 sec
			walls.emplace_back();
			spawn_t = 0.0f;// reset timer
		}

		
		cat.update(dt);

		/// RENDERING
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawFPS(10, 10);

		cat.show();

		// Draw all pipes
            

		for (int i = walls.size()-1; i >=0; i--)
		{
			walls[i].show();
			walls[i].update(dt);

			/// Collision Detection
			if (CheckCollisionCircleRec({ cat.x, cat.y }, cat.radius,
				{ walls[i].spawn_x, 0, walls[i].thicc, walls[i].top}) ||  // Top wall collision
				CheckCollisionCircleRec({ cat.x, cat.y }, cat.radius,
					{ walls[i].spawn_x, rl::HIGHT - walls[i].bottom, walls[i].thicc, 
					walls[i].bottom})) // Bottom wall collision
			{
				DrawRectangleLines(5, 5, rl::WIDTH,rl::HIGHT,RED);
			}


			// removes if it goes off screen

			

			if (walls[i].off_screen())
			{
				walls.erase(walls.begin()+i);
				
			}
		}

		EndDrawing();
	}
	

	



}


