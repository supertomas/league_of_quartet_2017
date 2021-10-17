#pragma once
#include <Siv3D.hpp>
#include"Player.hpp"


class tree : public Object
{
public :
	tree(Point _pos, Point _region, Point _wall, String _texture, int _n, int _N)
	{
		pos = _pos;
		texture = _texture;
		region = _region;
		wall = _wall;
		n = _n;
		N = _N;
	}

	Rect getRegion() const
	{
		return Rect(pos.x - region.x / 2 + i * 200, pos.y - region.y + j * 200, region.x, region.y);
	}

	Rect getWall()
	{
		return Rect(pos.x + wall.x , pos.y + wall.y, 185 * n, 190 * N);
	}

	void update(Player *player)
	{
		if (player->getBody().intersects(getWall()))
		{
			player->pos -= player->speed;
		}
	}

	void draw()
	{
		//getRegion().draw();
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < N; j++)
			{
				getRegion()(TextureAsset(texture)).draw();
			}
		}
		
	}
private:
	Point region, size, wall;
	Point shadow;
	String texture;
	int n, N, i, j;
};

class Rock : public Object
{
public:
	Rock(Point _pos, Point _region, Point _wall, String _texture, int _n, int _N)
	{
		pos = _pos;
		texture = _texture;
		region = _region;
		wall = _wall;
		n = _n;
		N = _N;
	}

	Rect getRegion() const
	{
		return Rect(pos.x - region.x / 2 + i * 200, pos.y - region.y + j * 200, region.x, region.y);
	}

	Rect getWall()
	{
		return Rect(pos.x + wall.x, pos.y + wall.y, 190 * n, 180 * N);
	}

	void update(Player *player)
	{
		if (player->getBody().intersects(getWall()))
		{
			player->pos -= player->speed;
		}
	}

	void draw()
	{
		//getWall().draw();
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < N; j++)
			{
				getRegion()(TextureAsset(texture)).draw();
			}
		}

	}
private:
	Point region, size, wall;
	String texture;
	int n, N, i, j;
};
class Ground : public Object
{
public:
	Ground(Point _pos, Point _region, String _texture, int _n, int _N)
	{
		pos = _pos;
		texture = _texture;
		region = _region;
		n = _n;
		N = _N;
	}

	Rect getRegion() const
	{
		return Rect(pos.x - region.x / 2 + i * 670, pos.y - region.y + j * 570, region.x, region.y);
	}

	void update(Player *player)
	{
		player->getVoid();
	}

	void draw()
	{
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < N; j++)
			{
				getRegion()(TextureAsset(texture)).draw();
			}
		}
	}
private:
	Point pos, region, wall;
	Point shadow;
	String texture;
	int n, N, i, j;
};

class flower : public Object
{
public:
	flower(Point _pos, Point _region, String _texture, int _n, int _N)
	{
		pos = _pos;
		texture = _texture;
		region = _region;
		n = _n;
		N = _N;
	}

	Rect getRegion() const
	{
		return Rect(pos.x - region.x / 2 + i * 60, pos.y - region.y + j * 40, region.x, region.y);
	}

	void update(Player *player)
	{
		player->getVoid();
	}

	void draw()
	{
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < N; j++)
			{
				getRegion()(TextureAsset(texture)).draw();
			}
		}
	}
private:
	Point pos, region, size;
	Point shadow;
	String texture;
	int n, N, i, j;
};

class rock : public Object
{
public:
	rock(Point _pos, Point _region, Point _wall, String _texture)
	{
		pos = _pos;
		region = _region;
		wall = _wall;
		texture = _texture;
	}


	Rect getRegion() const
	{
		return Rect(pos.x - region.x / 2 , pos.y - region.y , region.x, region.y);
	}

	Rect getWall()
	{
		return Rect(pos.x - wall.x / 2, pos.y - wall.y, wall.x, wall.y + 80);
	}

	void update(Player *player)
	{
		if (player->getBody().intersects(getWall()))
		{
			player->pos -= player->speed;
		}
	}

	void draw()
	{
		getRegion()(TextureAsset(texture)).draw();
		//getWall().draw();
	}
private:
	Point region, wall;
	String texture;
};

