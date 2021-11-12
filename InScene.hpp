#pragma once
# include <Siv3D.hpp>
#include<HamFramework.hpp>
#include"Mobu.hpp"
#include"Indoor.hpp"
class In :public SceneManager<String, Gamedata>::Scene
{
	public:
	Player *player;
	std::vector<Object*> objects;
	Rect rect;
	bool a = false;
	
	In::~In()
	{
		for (auto& object : objects)
		{
			delete object;
		}
	}

	void update() override
	{
		std::sort(objects.begin(), objects.end(), [](Object* a, const Object* b)
		{
			return a->pos.y < b->pos.y;
		});

		for (auto& object : objects)
		{
			object->update(player);
		}

		if (player->getFace().intersects(rect))
		{
			player->pos -= player->speed;
			a = true;
		}
		if (a == true && Input::KeyEnter.clicked)
		{
			TextManager::text->start(4);
		}
		String flag = TextManager::text->getFlag();
		if (flag != L"")
		{
			changeScene(flag);
			m_data->pos = player->pos;
			m_data->Firstmessage = true;
		}
		TextManager::text->update();
	}
	void draw() const override
	{
		Graphics::SetBackground(Color(15, 15, 15));
		Graphics2D::PushTransform(Mat3x2::Translate(-player->pos.x + Window::Size().x / 2, -player->pos.y + Window::Size().y / 2));
		for (auto& object : objects)
		{
			object->draw();
		}
		Graphics2D::PopTransform();
		TextManager::text->draw();
	}
private:
};
class In1 :public In
{
	void init()
	{
		player = new Player(m_data->pos, { 55, 40 }, { 95, 152 }, { L"AberuR", L"AberuL", L"AberuB", L"AberuF" },m_data->movement);
		objects.push_back(new Indoor({ 0, 0 }, { 800,760 }, { 360, 480 }, { 5, 5 }, L"indoor", this, { 310, -340}));
		objects.push_back(player);
	}

};

class In2 :public In
{
	void init()
	{
		player = new Player(m_data->pos, { 55, 40 }, { 95, 152 }, { L"AberuR", L"AberuL", L"AberuB", L"AberuF" },m_data->movement);
		objects.push_back(new Indoor({ 0, 0 }, { 800,760 }, { 360, 480 }, { 5, 5 }, L"indoor", this, { -390, 40}));
		objects.push_back(player);
		objects.push_back(new Mobu({ -290, 620 }, { 40, 30 }, MobuDirection::Forward, { 71.5, 155 }, { L"Mobu1R", L"Mobu1L", L"Mobu1B", L"Mobu1F" }, 2));
	}
};

class In3 :public In
{
	void init()
	{
		if (m_data->Firstmessage == false)
		{
			TextManager::text->start(12);
		}
		player = new Player(m_data->pos, { 55, 40 }, { 95, 152 }, { L"AberuR", L"AberuL", L"AberuB", L"AberuF" },m_data->movement);
		objects.push_back(new mainIndoor({ 0, 0 }, { 700,660 }, { 180, 260 }, { 5, 5 }, L"mainHouse", this, { 280, 490 }));
		objects.push_back(player);
		rect = Rect(-320, 180, 200, 100);
	}
};
