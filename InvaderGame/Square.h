#pragma once

#include "framework.h"
#include "DirectX.h"
#include "Component.h"

class Square
{
public:
	Square();
	Square(float x, float y, float w, float h);
	Square(const Texture& texture, float x, float y, float w, float h);
	void Draw();

private:
	Texture _texture;
	float _x;
	float _y;
	float _w;
	float _h;
};