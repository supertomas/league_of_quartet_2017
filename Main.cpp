#include <Siv3D.hpp>
#include<Hamframework.hpp>
#include"Battle.hpp"
#include"InScene.hpp"
#include"Gamedata.hpp"
#include"Title.hpp"
#include"FirstCity.hpp"

void Main()
{
	Window::SetTitle(L"リーグオブカルテット");
	Window::Resize(1200, 750);
	
	SceneManager<String,Gamedata> manager;
	manager.add<Title>(L"Title");
	manager.add<HowToPlay>(L"HowToPlay");
	manager.add<GameOver>(L"Gameover");
	manager.add<Break>(L"Break");
	manager.add<In3>(L"In3");
	manager.add<firstCity>(L"FirstCity");
	manager.add<firstField>(L"FirstField");
	manager.add<firstField2>(L"FirstField2");
	manager.add<secondField>(L"SecondField");
	manager.add<secondField2>(L"SecondField2");
	manager.add<thirdField>(L"ThirdField");
	manager.add<Clear>(L"Clear");
	
	
	manager.add<In1>(L"In");
	manager.add<In2>(L"In2");
	
	manager.add<Battle>(L"Battle");
	manager.add<Battle2>(L"Battle2");
	manager.add<Battle3>(L"Battle3");
	manager.add<Yellow>(L"Yellow");
	manager.add<Arisu>(L"Arisu");
	manager.add<Donald>(L"Donald");


	//オブジェクト
	TextureAsset::Register(L"blackboard", L"Images/blackboard.png");
	TextureAsset::Register(L"floor", L"Images/floor.png");
	TextureAsset::Register(L"wallR", L"Images/wallRight.jpg");
	TextureAsset::Register(L"wallL", L"Images/wallLeft.jpg");
	TextureAsset::Register(L"wall", L"Images/wall.jpg");
	TextureAsset::Register(L"ground", L"Images/ground.png");
	TextureAsset::Register(L"school", L"Images/school.png");
	TextureAsset::Register(L"tree1", L"Images/tree1.png");
	TextureAsset::Register(L"building", L"Images/building.png");
	TextureAsset::Register(L"wallL", L"Images/wallL.png");
	TextureAsset::Register(L"wallR", L"Images/wallR.png");
	TextureAsset::Register(L"cross", L"Images/cross.png");
	TextureAsset::Register(L"enter", L"Images/enter.jpg");
	TextureAsset::Register(L"space", L"Images/space.png");

	//家や背景
	TextureAsset::Register(L"house", L"Images/house.png");
	TextureAsset::Register(L"indoor", L"Images/indoor.png");
	TextureAsset::Register(L"indoor", L"Images/indoor.png");
	TextureAsset::Register(L"house2", L"Images/house2.png");
	TextureAsset::Register(L"mainHouse", L"Images/mainHouse.png");
	TextureAsset::Register(L"flower", L"Images/flower.png");
	TextureAsset::Register(L"grass", L"Images/grass.png");
	TextureAsset::Register(L"soil", L"Images/soil.jpg");
	TextureAsset::Register(L"soil2", L"Images/soil2.png");
	TextureAsset::Register(L"stone", L"Images/stone1.png");
	TextureAsset::Register(L"title", L"Images/title.png");
	TextureAsset::Register(L"title2", L"Images/title2.png");
	TextureAsset::Register(L"forest", L"Images/forest.png");
	TextureAsset::Register(L"rockBackground", L"Images/rockBackground.png");
	//キャラ
	TextureAsset::Register(L"CharaL", L"Images/chara/CharaL.png");
	TextureAsset::Register(L"CharaR", L"Images/chara/CharaR.png");
	TextureAsset::Register(L"CharaB", L"Images/chara/CharaB.png");
	TextureAsset::Register(L"CharaF", L"Images/chara/CharaF.png");

	TextureAsset::Register(L"AberuL", L"Images/chara/AberuL.png");
	TextureAsset::Register(L"AberuR", L"Images/chara/AberuR.png");
	TextureAsset::Register(L"AberuB", L"Images/chara/AberuB.png");
	TextureAsset::Register(L"AberuF", L"Images/chara/AberuF.png");

	TextureAsset::Register(L"Mobu1L", L"Images/chara/Mobu1L.png");
	TextureAsset::Register(L"Mobu1R", L"Images/chara/Mobu1R.png");
	TextureAsset::Register(L"Mobu1B", L"Images/chara/Mobu1B.png");
	TextureAsset::Register(L"Mobu1F", L"Images/chara/Mobu1F.png");

	TextureAsset::Register(L"Mobu2L", L"Images/chara/Mobu2L.png");
	TextureAsset::Register(L"Mobu2R", L"Images/chara/Mobu2R.png");
	TextureAsset::Register(L"Mobu2B", L"Images/chara/Mobu2B.png");
	TextureAsset::Register(L"Mobu2F", L"Images/chara/Mobu2F.png");

	TextureAsset::Register(L"DoctorL", L"Images/chara/DoctorL.png");
	TextureAsset::Register(L"DoctorR", L"Images/chara/DoctorR.png");
	TextureAsset::Register(L"DoctorB", L"Images/chara/DoctorB.png");
	TextureAsset::Register(L"DoctorF", L"Images/chara/DoctorF.png");

	TextureAsset::Register(L"Lady1L", L"Images/chara/Lady1L.png");
	TextureAsset::Register(L"Lady1R", L"Images/chara/Lady1R.png");
	TextureAsset::Register(L"Lady1B", L"Images/chara/Lady1B.png");
	TextureAsset::Register(L"Lady1F", L"Images/chara/Lady1F.png");

	TextureAsset::Register(L"PieroL", L"Images/chara/PieroL.png");
	TextureAsset::Register(L"PieroR", L"Images/chara/PieroR.png");
	TextureAsset::Register(L"PieroB", L"Images/chara/PieroB.png");
	TextureAsset::Register(L"PieroF", L"Images/chara/PieroF.png");

	TextureAsset::Register(L"MajyoL", L"Images/chara/MajyoL.png");
	TextureAsset::Register(L"MajyoR", L"Images/chara/MajyoR.png");
	TextureAsset::Register(L"MajyoB", L"Images/chara/MajyoB.png");
	TextureAsset::Register(L"MajyoF", L"Images/chara/MajyoF.png");
	//敵
	TextureAsset::Register(L"ワーウルフ", L"Images/Enemy/wolf.png");
	TextureAsset::Register(L"キャロットマン", L"Images/Enemy/carotman.png");
	TextureAsset::Register(L"とげボール", L"Images/Enemy/togebo.png");
	TextureAsset::Register(L"メタルエッグ", L"Images/Enemy/metaruEgg.png");
	TextureAsset::Register(L"ピエロ", L"Images/Enemy/piero.png");
	TextureAsset::Register(L"納豆坊", L"Images/Enemy/natto.png");
	TextureAsset::Register(L"イエロー", L"Images/Enemy/a.png");
	TextureAsset::Register(L"アリス", L"Images/Enemy/majyo.png");
	TextureAsset::Register(L"ドナルド", L"Images/Enemy/piero.png");
	//音
	SoundAsset::Register(L"wall", L"Sound/wall.wav");
	SoundAsset::Register(L"damage", L"Sound/damage.wav");
	SoundAsset::Register(L"カーソル音", L"Sound/カーソル音.mp3");
	SoundAsset::Register(L"セレクト音", L"Sound/セレクト音.mp3");
	SoundAsset::Register(L"キャンセル", L"Sound/キャンセル.mp3");
	SoundAsset::Register(L"fire", L"Sound/fire.wav");
	SoundAsset::Register(L"decide", L"Sound/choice.mp3");
	SoundAsset::Register(L"話しかける", L"Sound/話しかける.mp3");
	SoundAsset::Register(L"Town", L"Sound/town.mp3", SoundLoop(true));
	SoundAsset::Register(L"BattleM", L"Sound/battle.wav", SoundLoop(true));
	SoundAsset::Register(L"break", L"Sound/break.mp3", SoundLoop(true));
	TextManager::Create();
	while (System::Update())
	{
		manager.updateAndDraw();
	}

}

