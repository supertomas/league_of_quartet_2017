#pragma once
#include<Siv3D.hpp>
class Gamedata
{
public:
	Point pos = {0,400};
	int32 Hp = 35, Mp = 16, Exp = 0, Level = 1, Gold = 0;
	Point movement = {0,0};
	bool skillmessages1 = false;
	bool skillmessages2 = false;
	bool skillmessages3 = false;
	bool finish = false;
	bool finish2 = false;
	bool Firstmessage = false;
private:

};

