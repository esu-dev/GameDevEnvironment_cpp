#include "Square.h"

Square::Square()
	: _x(0)
	, _y(0)
	, _w(0)
	, _h(0)
{

}

Square::Square(float x, float y, float w, float h)
	: _x(x)
	, _y(y)
	, _w(w)
	, _h(h)
{

}

Square::Square(const Texture& texture, float x, float y, float w, float h)
	: _texture(texture)
	, _x(x)
	, _y(y)
	, _w(w)
	, _h(h)
{

}

void Square::Draw()
{
	//D3D.Draw2D(_texture, _x, _y, _w, _h);
}