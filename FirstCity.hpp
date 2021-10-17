# include <Siv3D.hpp>
#include<HamFramework.hpp>
#include"backGround.hpp"
#include"TextManager.hpp"
#include"Battle.hpp"
#include"Gamedata.hpp"
#include"ListMenu.hpp"

struct changerect
{
	Rect rect;
	String Scene;
	Point nextpos;
};


class Field : public SceneManager<String, Gamedata>::Scene
{
public:
	Player *player;
	std::vector<Object*> objects;
	std::vector<changerect> changerects;
	std::vector<String> sceneBattle;
	std::vector<Object*> grounds;
	Mobu* mobu;
	Sound townM;
	ListMenu listMenu;
	Font font = Font(30);
	Vec2 changeBattle;

	Field::~Field()
	{
		for (auto& object : objects)
		{
			delete object;
		}
		for (auto& ground : grounds)
		{
			delete ground;
		}
	}

	void update() override
	{
		townM.play();
		for (auto& object : objects)
		{
			object->update(player);
		}
		std::sort(objects.begin(), objects.end(), [](Object* a, const Object* b) 
		{ 
			return a->pos.y < b->pos.y; 
		});
		/*for (uint32 i = 0; i < objects.size(); i++)
		{
			for (int32 j = objects.size() - 1; j > i; j--)
			{
				if (objects[j]->pos.y < objects[j - 1]->pos.y)
				{
					auto temp = objects[j];
					objects[j] = objects[j - 1];
					objects[j - 1] = temp;
				}
			}
		}*/
		//次のフィールドに移り変わる処理
		for (auto& i : changerects)
		{
			if (player->getBody().intersects(i.rect))
			{
				changeScene(i.Scene);
				m_data->pos = i.nextpos;
				m_data->movement = player->movement;
			}
		}
		//フィールドのモンスターと戦うシーンに移り変わる処理
		for (auto& i : sceneBattle)
		{
			if (player->distance.x > changeBattle.x || player->distance.y > changeBattle.y)
			{
				changeScene(i);
				townM.pause();
				m_data->pos = player->pos;
				player->distance = { 0,0 };
				m_data->movement = player->movement;
			}
		}
		String flag = TextManager::text->getFlag();
		//特定のセリフの話しが終わったらバトルに移り変わる処理
		if (flag != L"")
		{
			townM.pause();
			flag;
			changeScene(flag);
			m_data->pos = player->pos;
			m_data->movement = player->movement;
		}
		TextManager::text->update();

	}

	void draw() const override
	{
		Graphics::SetBackground(Color(15, 15, 15));
		if (Input::KeyX.clicked)
		{
			listMenu.draw();
		}
		Graphics2D::PushTransform(Mat3x2::Translate(-player->pos.x + Window::Size().x / 2, -player->pos.y + Window::Size().y / 2));
		for (auto& i : grounds)
		{
			i->draw();
		}
		for (auto& object : objects)
		{
			object->draw();
		}
		/*for (auto& i : changerects)
		{
			i.rect.draw();
		}*/
		Graphics2D::PopTransform();

		TextManager::text->draw();
	}
};


class firstCity : public Field
{
public:

	void init()
	{
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);
		
		grounds.push_back(new Ground({ -1000,-700 }, { 3000, 2700 }, L"grass", 5, 7));
		grounds.push_back(new Ground({ -380,80 }, { 400, 300 }, L"soil2", 2, 1));
		objects.push_back(new flower({ 80,-600 }, { 100, 100 }, L"flower", 2, 2));
		objects.push_back(new flower({ 150, 0 }, { 100, 100 }, L"flower", 4, 2));
		objects.push_back(new flower({ -450, 200 }, { 100, 100 }, L"flower", 2, 2));
		player = new Player(m_data->pos, { 55, 40 }, { 91, 154 }, { L"AberuR", L"AberuL", L"AberuB", L"AberuF" },m_data->movement);
		objects.push_back(player);
		changeBattle = { INFINITY,INFINITY };
		objects.push_back(new tree({ -750, 1000 }, { 370, 370 }, { -100, -180 }, L"tree1", 9, 3));
		objects.push_back(new tree({ -500, -1300 }, { 370, 370 }, { -100, -190 }, L"tree1", 3, 3));
		objects.push_back(new tree({ -1700,-1200 }, { 370, 370 }, { -50, -100 }, L"tree1", 6, 12));
		objects.push_back(new tree({ 900,-1200 }, { 370, 370 }, { -50, -100 }, L"tree1", 4, 15));

		objects.push_back(new House01({ 400, -400 }, { 500, 500 }, { 8, 50 }, { 150,70 }, { 300, 80 }, 108, L"house", this));
		objects.push_back(new House02({ -300, 0 }, { 500, 500 }, { 8, 50 }, { 150,70 }, { 300, 80 }, 108, L"house", this));
		objects.push_back(new House03({ 350, 450 }, { 500, 500 }, { 8, 50 }, { 130, 80 }, { 360, 110 }, 175, L"house2", this));

		objects.push_back(new Mobu({ -100, 500 }, { 40, 30 }, MobuDirection::Backward, { 94 ,158 }, { L"Mobu2R", L"Mobu2L", L"Mobu2B", L"Mobu2F" }, 0));
		objects.push_back(new Mobu({ 500, -100 }, { 40, 30 }, MobuDirection::Right, { 100.5 ,173 }, { L"PieroR", L"PieroL", L"PieroB", L"PieroF" }, 1));
		objects.push_back(new Mobu({ 100, -900 }, { 40, 30 }, MobuDirection::Forward, { 94,158 }, { L"Lady1R", L"Lady1L", L"Lady1B", L"Lady1F" }, 5));
		objects.push_back(new Mobu({ -150, -500 }, { 40, 30 }, MobuDirection::Forward, 2, { 82, 155 }, { L"DoctorR", L"DoctorL", L"DoctorB", L"DoctorF" }, 3));
		if (m_data->finish == false)
		{
			changerects.push_back(changerect{ Rect(-100, -1300, 1000, 20), L"FirstField",{ 130,750 } });
		}
		else
		{
			changerects.push_back(changerect{ Rect(-100, -1300, 1000, 20), L"FirstField2",{ 130,750 } });
		}
	}
};

class firstField :public Field
{
public:
	void init()
	{
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);
		grounds.push_back(new Ground({ -1000,-700 }, { 3000, 2700 }, L"grass", 5, 7));
		objects.push_back(new flower({ 460, -320 }, { 100, 100 }, L"flower", 2, 2));
		objects.push_back(new flower({ 380, -750 }, { 100, 100 }, L"flower", 4, 2));
		grounds.push_back(new Ground({ -850,-2100 }, { 3000, 2700 }, L"soil", 3, 1));
		objects.push_back(new tree({ 800,-1200 }, { 370, 370 }, { -50, -100 }, L"tree1", 6, 15));
		objects.push_back(new tree({ -1200,-1200 }, { 370, 370 }, { -50, -100 }, L"tree1", 5, 17));
		objects.push_back(new tree({ -1400,-2300 }, { 370, 370 }, { -50, -100 }, L"tree1", 1, 20));
		objects.push_back(new tree({ -900,-2500 }, { 370, 370 }, { -50, -180 }, L"tree1", 20, 5));
		objects.push_back(new tree({ 50,200 }, { 370, 370 }, { -80, -230 }, L"tree1", 4, 2));
		objects.push_back(new tree({ -230,-850 }, { 370, 370 }, { -80, -230 }, L"tree1", 3, 2));
		objects.push_back(new rock({ 1790, -1350 }, { 780, 780 }, { 470, 350 }, L"stone"));
		player = new Player(m_data->pos, { 55, 40 }, { 91, 154 }, { L"AberuR", L"AberuL", L"AberuB", L"AberuF" },m_data->movement);
		changeBattle = { Random(100,450),Random(100,450) };
		objects.push_back(new Mobu({ 505, -370 }, { 40, 30 }, MobuDirection::Left, 2, { 94 ,158 }, { L"Mobu2R", L"Mobu2L", L"Mobu2B", L"Mobu2F" }, 9));
		objects.push_back(new Mobu({ 1140, -1610 }, { 40, 30 }, MobuDirection::Backward, { 71.5 ,158 }, { L"Mobu1R", L"Mobu1L", L"Mobu1B", L"Mobu1F" }, 6));
		objects.push_back(player);
		objects.push_back(new Mobu({ -1155, -1900 }, { 40, 30 }, MobuDirection::Right,{98.2 ,167 }, { L"MajyoR", L"MajyoL", L"MajyoB", L"MajyoF" }, 7));
		changerects.push_back(changerect{ Rect(-300, 800, 1000, 20), L"FirstCity",{ 385,-1130 } });
		if (m_data->finish2 == false)
		{
			changerects.push_back(changerect{ Rect(-1300, -2335, 400, 10), L"SecondField",{ 0,720 } });
		}
		else
		{
			changerects.push_back(changerect{ Rect(-1300, -2335, 400, 10), L"SecondField2",{ 0,720 } });
		}
		sceneBattle.push_back(String(L"Battle"));
	}

	
};

class firstField2 :public Field
{
public:
	void init()
	{
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);
		grounds.push_back(new Ground({ -1000,-700 }, { 3000, 2700 }, L"grass", 5, 7));
		objects.push_back(new flower({ 460, -320 }, { 100, 100 }, L"flower", 2, 2));
		objects.push_back(new flower({ 380, -750 }, { 100, 100 }, L"flower", 4, 2));
		grounds.push_back(new Ground({ -850,-2100 }, { 3000, 2700 }, L"soil", 3, 1));
		objects.push_back(new tree({ 800,-1200 }, { 370, 370 }, { -50, -100 }, L"tree1", 6, 15));
		objects.push_back(new tree({ -1200,-1200 }, { 370, 370 }, { -50, -100 }, L"tree1", 5, 17));
		objects.push_back(new tree({ -1400,-2300 }, { 370, 370 }, { -50, -100 }, L"tree1", 1, 20));
		objects.push_back(new tree({ -900,-2500 }, { 370, 370 }, { -50, -180 }, L"tree1", 20, 5));
		objects.push_back(new tree({ 50,200 }, { 370, 370 }, { -80, -230 }, L"tree1", 4, 2));
		objects.push_back(new tree({ -230,-850 }, { 370, 370 }, { -80, -230 }, L"tree1", 3, 2));
		objects.push_back(new rock({ 1790, -1350 }, { 780, 780 }, { 470, 350 }, L"stone"));
		player = new Player(m_data->pos, { 55, 40 }, { 91, 154 }, { L"AberuR", L"AberuL", L"AberuB", L"AberuF" }, m_data->movement);
		changeBattle = { Random(100,450),Random(100,450) };
		objects.push_back(new Mobu({ 505, -370 }, { 40, 30 }, MobuDirection::Left, 2, { 94 ,158 }, { L"Mobu2R", L"Mobu2L", L"Mobu2B", L"Mobu2F" }, 9));
		objects.push_back(new Mobu({ 1140, -1610 }, { 40, 30 }, MobuDirection::Backward, { 71.5 ,158 }, { L"Mobu1R", L"Mobu1L", L"Mobu1B", L"Mobu1F" }, 6));
		objects.push_back(player);
		objects.push_back(new Mobu({ -1155, -1900 }, { 40, 30 }, MobuDirection::Right, { 98.2 ,167 }, { L"MajyoR", L"MajyoL", L"MajyoB", L"MajyoF" }, 10));
		changerects.push_back(changerect{ Rect(-300, 800, 1000, 20), L"FirstCity",{ 385,-1130 } });
		if (m_data->finish2 == false)
		{
			changerects.push_back(changerect{ Rect(-1300, -2335, 400, 10), L"SecondField",{ 0,720 } });
		}
		else
		{
			changerects.push_back(changerect{ Rect(-1300, -2335, 400, 10), L"SecondField2",{ 0,720 } });
		}
		sceneBattle.push_back(String(L"Battle"));
		m_data->finish = true;
	}
};
class secondField :public Field
{
public:

	void init()
	{
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);
		grounds.push_back(new Ground({ -1000,-700 }, { 3000, 2700 }, L"soil", 7, 7));
		objects.push_back(new Rock({ 270,585 }, { 370, 370 }, { -70, -120 }, L"stone", 14, 3));
		objects.push_back(new Rock({ -1200,-2500 }, { 370, 370 }, { -50, -100 }, L"stone", 5, 20));
		objects.push_back(new Rock({ -205, -1000 }, { 370, 370 }, { -50, -100 }, L"stone", 13, 5));
		objects.push_back(new Rock({ 3045, -1480 }, { 370, 370 }, { -50, -100 }, L"stone", 5, 15));
		objects.push_back(new Rock({ 0, -2080 }, { 370, 370 }, { -50, -100 }, L"stone", 16, 3));
		player = new Player(m_data->pos, { 55, 40 }, { 91, 154 }, { L"AberuR", L"AberuL", L"AberuB", L"AberuF" }, m_data->movement);
		objects.push_back(player);
		changeBattle = { Random(100,450),Random(100,450) };
		objects.push_back(new Mobu({ 2635, -950 }, { 40, 30 }, MobuDirection::Right, { 100.5 ,173 }, { L"PieroR", L"PieroL", L"PieroB", L"PieroF" }, 11));
		if (m_data->finish == false)
		{
			changerects.push_back(changerect{ Rect(-300, 800, 1000, 20), L"FirstField", {-1150,-2150} });
		}

		else
		{
			changerects.push_back(changerect{ Rect(-300, 800, 1000, 20), L"FirstField2",{ -1150,-2150 } });
		}
		changerects.push_back(changerect{ Rect(1320,-1680,50,480) , L"ThirdField", { 1500,-25 }});
		//objects.push_back(new Rock({ 2200, -1080 }, { 370, 370 }, { -50, -100 }, L"stone", 8, 1));
		sceneBattle.push_back(String(L"Battle2" ));
	}
};

class secondField2 :public Field
{
public:

	void init()
	{
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);
		grounds.push_back(new Ground({ -1000,-700 }, { 3000, 2700 }, L"soil", 7, 7));
		objects.push_back(new Rock({ 270,585 }, { 370, 370 }, { -70, -120 }, L"stone", 14, 3));
		objects.push_back(new Rock({ -1200,-2500 }, { 370, 370 }, { -50, -100 }, L"stone", 5, 20));
		objects.push_back(new Rock({ -205, -1000 }, { 370, 370 }, { -50, -100 }, L"stone", 13, 5));
		objects.push_back(new Rock({ 3045, -1480 }, { 370, 370 }, { -50, -100 }, L"stone", 5, 15));
		objects.push_back(new Rock({ 0, -2080 }, { 370, 370 }, { -50, -100 }, L"stone", 16, 3));
		player = new Player(m_data->pos, { 55, 40 }, { 91, 154 }, { L"AberuR", L"AberuL", L"AberuB", L"AberuF" }, m_data->movement);
		objects.push_back(player);
		changeBattle = { Random(100,450),Random(100,450) };
		objects.push_back(new Mobu({ 2635, -950 }, { 40, 30 }, MobuDirection::Right, { 100.5 ,173 }, { L"PieroR", L"PieroL", L"PieroB", L"PieroF" }, 13));
		if (m_data->finish == false)
		{
			changerects.push_back(changerect{ Rect(-300, 800, 1000, 20), L"FirstField",{ -1150,-2150 } });
		}

		else
		{
			changerects.push_back(changerect{ Rect(-300, 800, 1000, 20), L"FirstField2",{ -1150,-2150 } });
		}
		changerects.push_back(changerect{ Rect(1320,-1680,50,480) , L"ThirdField",{ 1500,-25 } });
		//objects.push_back(new Rock({ 2200, -1080 }, { 370, 370 }, { -50, -100 }, L"stone", 8, 1));
		sceneBattle.push_back(String(L"Battle2"));
		m_data->finish2 = true;
	}




};
class thirdField :public Field
{
	void init()
	{
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);
		grounds.push_back(new Ground({ -2000,-1500 }, { 3000, 2700 }, L"soil", 8, 8));
		changeBattle = { Random(100,450),Random(100,450) };
		objects.push_back(new Mobu({ -1990, -2150 }, { 40, 30 }, MobuDirection::Forward, { 94 ,158 }, { L"Mobu2R", L"Mobu2L", L"Mobu2B", L"Mobu2F" }, 14));
		objects.push_back(new Mobu({ -1885, -3000 }, { 40, 30 }, MobuDirection::Forward, { 95.2, 152 }, { L"CharaR", L"CharaL", L"CharaB", L"CharaF" }, 8));
		player = new Player(m_data->pos, { 55, 40 }, { 91, 154 }, { L"AberuR", L"AberuL", L"AberuB", L"AberuF" }, m_data->movement);
		objects.push_back(player);
		if (m_data->finish2 == false)
		{
			changerects.push_back(changerect{ Rect(1605,-260,50,480) , L"SecondField",{ 1450,-1500 } });
		}
		else
		{
			changerects.push_back(changerect{ Rect(1605,-260,50,480) , L"SecondField2",{ 1450,-1500 } });
		}
		objects.push_back(new Rock({ 200, -305 }, { 370, 370 }, { -50, -100 }, L"stone", 20, 1));
		objects.push_back(new Rock({ -350, 280 }, { 370, 370 }, { -50, -100 }, L"stone", 25, 1));
		objects.push_back(new Rock({ -400, -1000 }, { 370, 370 }, { -50, -100 }, L"stone", 1, 7));
		objects.push_back(new Rock({ 145, -1350 }, { 370, 370 }, { -50, -100 }, L"stone", 1, 6));
		objects.push_back(new Rock({ -1520, -1500 }, { 370, 370 }, { -50, -100 }, L"stone", 9, 1));
		objects.push_back(new Rock({ -2005, -1000 }, { 370, 370 }, { -50, -100 }, L"stone", 9, 1));
		objects.push_back(new Rock({ -2200, -2225 }, { 370, 370 }, { -50, -100 }, L"stone", 1, 7));
		objects.push_back(new Rock({ -1580, -2220 }, { 370, 370 }, { -50, -100 }, L"stone", 1, 4));
		objects.push_back(new Rock({ -2680, -2350 }, { 370, 370 }, { -50, -100 }, L"stone", 3, 1));
		objects.push_back(new Rock({ -1565, -2350 }, { 370, 370 }, { -50, -100 }, L"stone", 3, 1));
		objects.push_back(new Rock({ -2880, -3300 }, { 370, 370 }, { -50, -100 }, L"stone", 1, 6));
		objects.push_back(new Rock({ -980, -3300 }, { 370, 370 }, { -50, -100 }, L"stone", 1, 6));
		objects.push_back(new Rock({ -2880, -3400 }, { 370, 370 }, { -50, -100 }, L"stone", 10, 1));
		sceneBattle.push_back(String(L"Battle3"));
	}

};
class Clear :public SceneManager<String, Gamedata>::Scene
{
public:
	Font font = Font(60);
	Font font2 = Font(40);
	Player* player;
	const String text3 = L"Game     Clear";
	void init()
	{
		Window::Resize(1200, 750);
		TextureAsset::Register(L"sky", L"Images/sky.jpg");
	}
	void update() override
	{
		if (Input::KeyEnter.clicked)
		{
			m_data->pos = { 0,400 };
			changeScene(L"Title");
		}
	}
	void draw() const override
	{
		TextureAsset(L"sky").scale(0.8, 0.8).draw();
		font(text3).drawCenter(600, 300);
	}
};


