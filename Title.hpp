#pragma once
# include <Siv3D.hpp>
#include<HamFramework.hpp>
#include"Gamedata.hpp"
class Title :public SceneManager<String,Gamedata>::Scene
{
public:
	//Player* player;
	void init()
	{
		TextureAsset::Register(L"sky", L"Images/sky.jpg");
		TextureAsset::Register(L"title2", L"Images/title2.png");
	}
	void update() override
	{
		if (choice == 0 && Input::KeyEnter.clicked)
		{
			changeScene(L"In3");
			//player->getPos().x = 300;
			SoundAsset(L"decide").playMulti();
		}
		if (choice == 1 && Input::KeyEnter.clicked)
		{
			changeScene(L"HowToPlay");
			SoundAsset(L"decide").playMulti();
		if (Input::KeyDown.clicked)
		{		}

			choice++;
		}
		if (Input::KeyUp.clicked)
		{
			choice--;
		}
		if (choice < 0)
		{
			choice = 0;
		}
		if (choice > 1)
		{
			choice = 1;
		}
	}

	void draw() const override
	{
		TextureAsset(L"sky").scale(0.8, 0.8).draw();
		TextureAsset(L"title").scale(1.0, 1.0).draw();
		font(L"ゲームスタート").drawCenter(950, 500, Palette::Black);
		font(L"操作方法").drawCenter(950, 600, Palette::Black);
		for (int i = 0; i < 2; i++)
		{
			if (choice == 0 + i * 1)
			{
				Triangle({ 710, 470 + i * 100 }, { 750, 500 + i * 100 }, { 710, 530 + i * 100 }).draw(Palette::Black);
			}
		}
		font2(L"エンターキーをおしてね").draw(100, 550, Palette::Black);
	}
private:
	Font font = Font(30);
	Font font2 = Font(40);
	int choice = 0;
};

class Images
{
public:
	Images(Point _pos, Point _size, String _texture)
	{
		pos = _pos;
		size = _size;
		texture = _texture;
	};
	
	Rect getBody() const
	{
		return Rect(pos.x, pos.y, size.x, size.y);
	}
	void draw()
	{
		getBody()(TextureAsset(texture)).draw();
	}
private:
	Point pos, size;
	String texture;
};

class HowToPlay :public SceneManager<String,Gamedata>::Scene
{
public:
	std::vector<Images*> images;
	~HowToPlay()
	{
		for (auto& image : images)
		{
			delete image;
		}
	}
	void init()
	{
		TextureAsset::Register(L"sky", L"Images/sky.jpg");
		images.push_back(new Images({ 100,50 }, { 150,150 }, L"cross"));
		images.push_back(new Images({ 100,250 }, { 150,150 }, L"enter"));
		images.push_back(new Images({ 100,450 }, { 180,100 }, L"space"));
	}
	void update() override
	{
		if (Input::KeySpace.clicked)
		{
			changeScene(L"Title");
		}
	}
	void draw() const override
	{
		font(L"主人公のいどうやこうもく\nの選択に使う").draw(300, 40, Palette::Black);
		font(L"決定する しらべる 話しかける\n話しをすすめるときに使う").draw(300, 240, Palette::Black);
		font(L"やめる もどるなどのときに使う").draw(300, 450, Palette::Black);
		font2(L"スペースキーをおしてね").draw(150, 550, Palette::Black);
		for (auto& i : images)
		{
			i->draw();
		}
	}
private:
	Font font = Font(40);
	Font font2 = Font(45);
};

