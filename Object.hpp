#pragma once
# include <Siv3D.hpp>
class Player;
class Object
{
public:
	
	Point pos;
	virtual void update(Player *player) = 0;
	virtual void draw() = 0;
};
