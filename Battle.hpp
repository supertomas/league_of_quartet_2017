# pragma once
# include <Siv3D.hpp>
# include <HamFramework.hpp>
# include "Enemy.hpp"
# include "Gamedata.hpp"
# include "ListMenu.hpp"
# include "Menu.hpp"
# include "TextManager.hpp"
#include"Status.hpp"
#include"Title.hpp"
enum class mode
{
	comandMenu, skiimenu, target, escape, statusMessage
};

class AllBattle : public SceneManager<String, Gamedata>::Scene
{
public:
	Sound townM;
	Sound BattleM;
	mode ModeScreen = mode::comandMenu;
	String Firstmessage;
	String escapeMessage;
	std::vector<String> Scene;
	std::vector<Images*> image;
	int AttackP;
	int DefenceP;
	int MetalDefence;
	bool a = false;
	int SpeedP;
	Status status;
	bool finish = false;

	Menu menu;

	ListMenu listMenu;

	StatusMessage statusMessage;

	Array<EnemyData> enemies;

	String targetCommand;
	int32 target = 0;
	Stopwatch timer = Stopwatch(true);

	Font font = Font(28);

	bool turnFinish = false;

	int escapeRandom()
	{
		return Random(0, 1);
	}

	String playerAction(String action)
	{
		if (m_data->Hp > 0)
		{
			if (action == L"こうげき")
			{
				int Enemydamage = AttackP - enemies[target].Defence;
				if (Enemydamage > 0)
				{
					enemies[target].Hp = enemies[target].Hp - Enemydamage;
					return Format(L"アベルのこうげき", enemies[target].type, L"に\n", Enemydamage, L"ポイントのダメージを与えた。");
				}
				else if (Enemydamage <= 0)
				{
					return Format(L"アベルのこうげき   しかし\n", enemies[target].type, L"にダメージを与えられない。");
				}
			}
			else if (action == L"きあいだめ")
			{
				if (AttackP < status.AttackPUp(m_data->Level) + 8 + m_data->Level)
				{
					AttackP = Min(AttackP + 4 + m_data->Level, status.AttackPUp(m_data->Level) + 8 + m_data->Level);
					m_data->Mp -= 3;
					return Format(L"アベルは きあいだめ をした\nこうげき力 じょうしょう !");
				}
				else
				{
					return Format(L"アベルは きあいだめ をした\nしかし、これ以上こうげき力 は上がらない");
				}
			}

			else if (action == L"ばくれつけん")
			{
				m_data->Mp -= 5;
				for (uint32 i = 0; i < enemies.size(); i++)
				{
					int Enemydamage = AttackP - m_data->Level * 2 - enemies[i].Defence;
					if (Enemydamage > 0)
					{
						enemies[i].Hp = enemies[i].Hp - Enemydamage;
					}
					else
					{

					}
				}
				return Format(L"アベルのばくれつけん\n敵全体にダメージを与えた。");
			}

			else if (action == L"ホープ")
			{
				if (DefenceP < status.DefencePUp(m_data->Level) + 5 + m_data->Level)
				{
					m_data->Mp -= 3;
					DefenceP = Min(DefenceP + 3 + m_data->Level, status.AttackPUp(m_data->Level) + 5 + m_data->Level);
					return Format(L"アベルはホープ のじゅもんをとなえた\nぼうぎょ力があがった。");
				}
				else
				{
					return Format(L"アベルはホープ のじゅもんをとなえた \nしかし、これ以上ぼうぎょ力 は上がらない");
				}

			}

			else if (action == L"ソニックジャブ")
			{
				m_data->Mp -= 4;
				int Enemydamage = AttackP - m_data->Level * 3 - enemies[target].Defence;
				if (Enemydamage > 0)
				{
					enemies[target].Hp = enemies[target].Hp - Enemydamage;
					SpeedP += 6;
					return Format(L"アベルのソニックジャブ !!  敵に",Enemydamage,L"ダメージ\nを与え、さらにアベルの素早さがあがった。");
				}
				else
				{
					return Format(L"アベルのソニックジャブ!  敵はダメージを\n受けなかったが、アベルの素早さはあがった。");
				}
			}
			else if (action == L"めいそう")
			{
				m_data->Mp = Min(status.MaxMp(m_data->Level),m_data->Mp + 8);
				return Format(L"アベルはめいそうをした。\nMPがかいふく !");
			}
			else if (action == L"メーレ")
			{
				m_data->Mp -= 3;
				m_data->Hp = Min(status.MaxHp(m_data->Level), m_data->Hp + 15);
				return Format(L"アベルは メーレ のじゅもんをとなえた\nHPが かいふく した。");
			}
			else if (action == L"にげる")
			{
				return Format(L"アベルは わざ がだせない");
			}

			else
			{
				// 何かの間違いでどの技にも引っかからなかった時.
				return L"アベルは ぼーっと している";
			}
		}
		else
		{
			return Format(L"アベルに戦う力は残っていない...");
		}
	}


	void battle(String action)
	{
		// これをソートするとターゲット先が変わるため使用しない.
		// std::sort(enemies.begin(), enemies.end(), [](const EnemyData& a, const EnemyData& b) { return a.Speed < b.Speed; });

		// std::pair<int32, int32> は<スピード, enemiesの添え字>のデータを表す.
		// { 10, 1 } ならenemies[1].Speedが10であることになる.
		Array<std::pair<int32, int32>> characters;

		for (uint32 i = 0; i < enemies.size(); i++)
		{
			characters.emplace_back(enemies[i].Speed, i);
		}

		// enemiesの代わりにcharactersをソート.
		std::sort(characters.begin(), characters.end(), [](const std::pair<int32, int32>& a, const std::pair<int32, int32>& b) { return a.first > b.first; });

		Array<String> messages;
		bool player = false;

		if (action == L"にげる")
		{
			messages.push_back(L"しかしにげられない。");
		}

		// enemiesではなくcharactersを回す.
		for (uint32 i = 0; i < characters.size(); i++)
		{
			// これ以降 i の代わりに index を使うので要注意！
			const int32 speed = characters[i].first;
			const int32 index = characters[i].second;
		
			if (SpeedP > speed && player == false && m_data->Hp > 0)
			{
				messages.push_back(playerAction(action));
				player = true;
			}
			if (enemies[index].Hp > 0)
			{
				// 一度変数に格納しないとif文ごとに違う技が出てしまう.
				const String choice = enemies[index].action[Random(0u, enemies[index].action.size() - 1)];

				if (choice == L"かみつく")
				{
					int Playerdamage = enemies[index].Attack - DefenceP;
					if (Playerdamage > 0)
					{
						m_data->Hp = Max(0,m_data->Hp - Playerdamage);
						messages.push_back(Format(enemies[index].type, L"のかみつくこうげき !\nアベルに", Playerdamage, L"ポイントのダメージ"));
						//SoundAsset(L"damage").playMulti();
					}
					else if(Playerdamage <= 0)
					{
						messages.push_back(Format(enemies[index].type, L"のかみつくこうげき !  しかし\nアベルはダメージを受けなかった。"));
					}
					
				}

				else if (choice == L"こうげき")
				{
					int Playerdamage = enemies[index].Attack - 5 - DefenceP;
					if (Playerdamage > 0)
					{
						m_data->Hp = Max(0, m_data->Hp - Playerdamage);
						messages.push_back(Format(enemies[index].type, L"のつうじょうこうげき !\nアベルに", Playerdamage, L"ポイントのダメージ"));
						//SoundAsset(L"damage").playMulti();
					}
					else if (Playerdamage <= 0)
					{
						messages.push_back(Format(enemies[index].type, L"のつうじょうこうげき !  しかし\nアベルはダメージを受けなかった。"));
					}
				}
				else if (choice == L"つめとぎ")
				{
					enemies[index].Attack += 5;
					messages.push_back(Format(enemies[index].type, L"は つめとぎ をした。\n", enemies[index].type, L"の攻撃力があがった。"));
				}
				else if (choice == L"けいかい")
				{
					messages.push_back(Format(enemies[index].type, L"はけいかいしている。"));
				}
				else if (choice == L"つきさす")
				{
					int Playerdamage = enemies[index].Attack - DefenceP;
					if (Playerdamage > 0)
					{
						m_data->Hp = Max(0, m_data->Hp - Playerdamage);
						messages.push_back(Format(enemies[index].type, L"の つきさす こうげき\nアベルは", Playerdamage, L"ポイントのダメージを受けた"));
						//SoundAsset(L"damage").playMulti();
					}
					else if (Playerdamage <= 0)
					{
						messages.push_back(Format(enemies[index].type, L"のつきさす こうげき !  しかし\nアベルはダメージを受けなかった。"));
					}
				}
				else if (choice == L"様子")
				{
					messages.push_back(Format(enemies[index].type, L"はようすをみている。"));
				}
				else if (choice == L"メーレ")
				{
					enemies[index].Hp = Min(10, enemies[index].Hp + 8);
					messages.push_back(Format(enemies[index].type, L"は メーレ のじゅもん\nをとなえた。"));
					messages.push_back(Format(enemies[index].type, L"はかいふくした。"));
				}

				else if (choice == L"メーレ2")
				{
					enemies[index].Hp = Min(46, enemies[index].Hp + 10);
					messages.push_back(Format(enemies[index].type, L"は メーレ のじゅもん\nをとなえた。"));
					messages.push_back(Format(enemies[index].type, L"はかいふくした。"));
				}

				else if (choice == L"メーレ3")
				{
					enemies[index].Hp = Min(65, enemies[index].Hp + 10);
					messages.push_back(Format(enemies[index].type, L"は メーレ のじゅもん\nをとなえた。"));
					messages.push_back(Format(enemies[index].type, L"はかいふくした。"));
				}
				else if (choice == L"ねばねば")
				{
					SpeedP -= 5;
					messages.push_back(Format(enemies[index].type, L"は ねばねば の液を出した。"));
					messages.push_back(Format(L"からみついて すばやさ が\n下がってしまった。"));
				}

				else if (choice == L"とげラッシュ")
				{
					int Playerdamage = enemies[index].Attack - DefenceP;
					if (Playerdamage > 0)
					{
						m_data->Hp = Max(0, m_data->Hp - Playerdamage);
						messages.push_back(Format(enemies[index].type, L"のとげラッシュ !\nアベルに", Playerdamage, L"ポイントのダメージ"));
						//SoundAsset(L"damage").playMulti();
					}
					else if (Playerdamage <= 0)
					{
						messages.push_back(Format(enemies[index].type, L"のとげラッシュ!  しかし\nアベルはダメージを受けなかった。。"));
					}
					m_data->Hp = m_data->Hp - Playerdamage;
					
				}

				else if (choice == L"ロックガード")
				{
					if (a == false)
					{
						MetalDefence = enemies[index].Defence + 10;
						a = true;
					}
					if (enemies[index].Defence < MetalDefence)
					{
						enemies[index].Defence = Min(enemies[index].Defence + 5, MetalDefence);
						messages.push_back(Format(enemies[index].type, L"のロックガード !!\n", enemies[index].type, L"の防御力があがった。"));
					}
					else
					{

						messages.push_back(Format(enemies[index].type, L"のロックガード !!"));
						messages.push_back(Format(L"しかし、これ以上", enemies[index].type, L"\nの防御力はあがらない。"));
					}
				}

				else if (choice == L"のしかかり")
				{
					int Playerdamage = enemies[index].Attack - DefenceP;
					if (Playerdamage > 0)
					{
						m_data->Hp = Max(0, m_data->Hp - Playerdamage);
						messages.push_back(Format(enemies[index].type, L"はのしかかってきた。\nアベルに", Playerdamage, L"ポイントのダメージ"));
						//SoundAsset(L"damage").playMulti();
					}
					else if (Playerdamage <= 0)
					{
						messages.push_back(Format(enemies[index].type, L"のこうげき !  しかし\nアベルはダメージを受けなかった。"));
					}
				}
				else if(choice == L"ボディープレス")
				{
					int Playerdamage = enemies[index].Attack - 3 - DefenceP;
					if (Playerdamage > 0)
					{
						m_data->Hp = Max(0, m_data->Hp - Playerdamage);
						messages.push_back(Format(enemies[index].type, L"のボディープレス。\nアベルに", Playerdamage, L"ポイントのダメージ受け"));
						messages.push_back(L"防御力が下がってしまった。");
						//SoundAsset(L"damage").playMulti();
						DefenceP = Max(8, DefenceP - 4);
					}
					else if (Playerdamage <= 0)
					{
						messages.push_back(Format(enemies[index].type, L"のボディープレス !  \nアベルはダメージを受けなかったが"));
						messages.push_back(L"防御力が下がってしまった。");
						DefenceP = Max(8,DefenceP - 4);
					}
				}
				else if(choice == L"フレア")
				{
					int Playerdamage = enemies[index].Attack - DefenceP;
					if (Playerdamage > 0)
					{
						m_data->Hp = Max(0, m_data->Hp - Playerdamage);
						messages.push_back(Format(enemies[index].type, L"の じゅもん フレア \nアベルは", Playerdamage, L"ポイントのダメージを受けた"));
						//SoundAsset(L"fire").playMulti();
					}
					else if (Playerdamage <= 0)
					{
						messages.push_back(Format(enemies[index].type, L"のこうげき !  しかし\nアベルはダメージを受けなかった。。"));
					}
				}

				else if (choice == L"ルノバス")
				{
					if (DefenceP > 5)
					{
						messages.push_back(Format(enemies[index].type, L"の ルノバスのじゅもん ！"));
						messages.push_back(Format(L"アベルの 防御力が 下げってしまった"));
						DefenceP = Max(5, DefenceP - 4);
					}
					else
					{
						messages.push_back(Format(enemies[index].type, L"の ルノバスのじゅもん ！"));
						messages.push_back(L"しかし、アベルの 防御力 \nは十分下がっている。");
					}
				}
				else if (choice == L"幻惑の瞳")
				{
					if (AttackP > 19)
					{
						messages.push_back(Format(enemies[index].type, L"の 幻惑の瞳"));
						messages.push_back(L"アベルは みりょうされ \n攻撃力がさがった");
						AttackP = Max(19,AttackP - 8);
					}
					else
					{
						messages.push_back(Format(enemies[index].type, L"の 幻惑の瞳"));
						messages.push_back(L"しかし、アベルの 攻撃力 \nは十分下がっている。");
					}
				}

				else if (choice == L"イリュージョンダンス")
				{
					if (AttackP > 14)
					{
						messages.push_back(Format(enemies[index].type, L"の イリュージョンダンス"));
						messages.push_back(L"アベルは みりょうされ \n攻撃力がさがった");
						AttackP = Max(14, AttackP - 6);
					}
					else
					{
						messages.push_back(Format(enemies[index].type, L"の イリュージョンダンス"));
						messages.push_back(L"しかし、アベルの 攻撃力 \nは十分下がっている。");
					}
				}

				else if (choice == L"火吹き")
				{
					int Playerdamage = enemies[index].Attack - DefenceP;
					if (Playerdamage > 0)
					{
						m_data->Hp = Max(0, m_data->Hp - Playerdamage);
						messages.push_back(Format(enemies[index].type, L"の 火吹き こうげき \nアベルは", Playerdamage, L"ポイントのダメージを受けた"));
					}
					else if (Playerdamage <= 0)
					{
						messages.push_back(Format(enemies[index].type, L"の火吹き こうげき !  しかし\nアベルはダメージを受けなかった。。"));
					}
				}
				else if (choice == L"ただのパンチ")
				{
					int Playerdamage = enemies[index].Attack - 6 - DefenceP;
					if (Playerdamage > 0)
					{
						m_data->Hp = Max(0, m_data->Hp - Playerdamage);
						messages.push_back(Format(enemies[index].type, L"の ただのパンチ\n", Playerdamage, L"ポイントのダメージを受けた"));
					}
					else if (Playerdamage <= 0)
					{
						messages.push_back(Format(enemies[index].type, L"のこうげき !  しかし\nアベルはダメージを受けなかった。。"));
					}
				}

				else if (choice == L"杖")
				{
					int Playerdamage = enemies[index].Attack - 5- DefenceP;
					if (Playerdamage > 0)
					{
						m_data->Hp = Max(0, m_data->Hp - Playerdamage);
						messages.push_back(Format(enemies[index].type, L"は 魔法の杖をふってきた。\n", Playerdamage, L"ポイントのダメージを受けた"));
					}
					else if (Playerdamage <= 0)
					{
						messages.push_back(Format(enemies[index].type, L"は 魔法の杖をふってきた。 !  \nしかしアベルはダメージを受けなかった。。"));
					}
				}
				else if (choice == L"ステップ")
				{
					messages.push_back(Format(enemies[index].type, L"はステップ しはじめた。 \n",enemies[index].type,L"のすばやさがあがった。"));
					enemies[index].Speed += 8;
				}
				else if (choice == L"ボルテックス")
				{
					int Playerdamage = enemies[index].Attack - 4 - DefenceP;
					if (Playerdamage > 0)
					{
						m_data->Hp = Max(0, m_data->Hp - Playerdamage);
						SpeedP -= 3;
						messages.push_back(Format(enemies[index].type, L"の ボルテックス \nアベルは" ,Playerdamage,L"ポイントのダメージを受けた。"));
						messages.push_back(L"さらにしびれてアベルの\n素早さが下がった … ");
					}
					else if (Playerdamage <= 0)
					{
						messages.push_back(Format(enemies[index].type, L"のこうげき !  しかし\nアベルはダメージを受けなかった。"));
					}
				}
				else if (choice == L"はどうショット")
				{
					int Playerdamage = enemies[index].Attack + 3 - DefenceP;
					if (Playerdamage > 0)
					{
						m_data->Hp = Max(0, m_data->Hp - Playerdamage);
						messages.push_back(Format(enemies[index].type, L"の 必殺 はどうショット\nアベルは",Playerdamage,L"ポイントのダメージを受けた。"));
						messages.push_back(Format(L"さらに", enemies[index].type,L"の攻撃力があがった。"));
						enemies[index].Attack += 4;
					}
					else if (Playerdamage <= 0)
					{
						messages.push_back(Format(enemies[index].type, L"のこうげき !  しかし\nアベルはダメージを受けなかった。"));
						messages.push_back(Format(L"しかし", enemies[index].type, L"の攻撃力があがった。"));
						enemies[index].Attack += 4;
					}
				}
				else
				{
					// 何かの間違いでどの技にも引っかからなかった時.

					messages.push_back(Format(enemies[index].type, L"は ぼーっと している"));
				}
			}
		}

		if (player == false)
		{
			messages.push_back(playerAction(action));
		}

		TextManager::text->start(messages);
		//turnFinish = true;
	}

	void resetTarget()
	{
		// target先が死んでないか調べる.
		// 死んでいたら、ループで右隣りに移動する.
		for (uint32 index = 0; index < enemies.size(); index++)
		{
			const int32 newTarget = (target + index) % enemies.size();

			if (enemies[newTarget].Hp > 0)
			{
				target = newTarget;

				break;
			}
		}
	}

	void targetUpdate()
	{
		if (Input::KeyRight.clicked)
		{
			// ターゲットを右に移動し、要素数を超えていたらそれに合わせる.
			target = Min<int32>(target + 1, enemies.size() - 1);

			// target先が死んでないか調べる.
			// 死んでいたら、ループで右隣りに移動する.
			for (uint32 index = 0; index < enemies.size(); index++)
			{
				const int32 newTarget = (target + index) % enemies.size();

				if (enemies[newTarget].Hp > 0)
				{
					target = newTarget;

					break;
				}
			}
		}

		if (Input::KeyLeft.clicked)
		{
			// ターゲットを左に移動し、0になったら0に合わせる.
			target = Max<int32>(target - 1, 0);

			// targetの移動先が死んでないか調べる.
			// 死んでいたら、ループで左隣りに移動する.
			for (uint32 index = 0; index < enemies.size(); index++)
			{
				// 負の値に剰余算は使えないのでenemies.size()を足して、必ず正の数になるようにする.
				const int32 newTarget = (target - index + enemies.size()) % enemies.size();

				if (enemies[newTarget].Hp > 0)
				{
					target = newTarget;

					break;
				}
			}
		}
	}

	bool win() const
	{
		bool win = true;

		for (uint32 i = 0; i < enemies.size(); ++i)
		{
			// 一人でも生き残ってたら win ではない
			if (enemies[i].Hp > 0)
			{
				win = false;
			}
		}

		return win;
	}

	bool lose() const
	{
		// プレイヤーが死んだらゲームオーバー

		return m_data->Hp <= 0;
	}

	void update() override
	{
		BattleM.play();

		// 「まものがあらわれた」表示中は何もさせない.
		if (timer.ms() < 1000)
		{
			return;
		}

		// これは必ず必要なのでMain.cppに移動
		TextManager::text->update();

		// TextManager起動中は何もしない.
		if (TextManager::text->isActive() == true)
		{
			return;
		}

		// finishがtrueになったらシーン遷移
		for (auto& i : Scene)
		{
			if (finish == true)
			{
				//負けた時だけゲームオーバーに遷移する.
				//勝つか引き分けの時は元のフィールドに戻る.
				if (lose())
				{
					changeScene(L"Gameover");
					BattleM.pause();
				}
				else
				{
					changeScene(i);
					BattleM.pause();
					townM.play();
				}
				return;
			}
		}


			
		// 敗北判定
		if (lose())
		{
			TextManager::text->start({ L"アベルは 力つきてしまった ..." });

			finish = true;

			return;
		}

		// 勝利判定
		if (win())
		{
			int32 allExp = 0;
			int32 allMoney = 0;
		
			for (uint32 i = 0; i < enemies.size(); i++)
			{
				allExp += enemies[i].Exp;
				allMoney += enemies[i].money;
			}

			m_data->Exp += allExp;
			Array<String> messages;
			messages.push_back(L"敵をたおした。");
			messages.push_back(Format(allExp, L"ポイントのけいけんちをかくとく"));
			messages.push_back(Format(allMoney, L"ゼニーをかくとく"));
			if (status.LevelUp(m_data->Exp,m_data->Level))
			{
				ModeScreen = mode::statusMessage;
				messages.push_back(Format(L"アベルのレベルがあがった。"));
				messages.push_back(Format(L"それぞれのステータスがあがった。"));
				m_data->Level += 1;
				m_data->Hp = status.MaxHp(m_data->Level);
				m_data->Mp = status.MaxMp(m_data->Level);
			}


			if (status.getSkill(m_data->Level) == 1 && m_data->skillmessages1 == false)
			{
				messages.push_back(Format(L"アベルはホープのじゅもんを覚えた。"));
				m_data->skillmessages1 = true;
			}

			else if (status.getSkill(m_data->Level) == 2 && m_data->skillmessages2 == false)
			{
				messages.push_back(Format(L"アベルはソニックジャブを覚えた。"));
				m_data->skillmessages2 = true;
			}

			else if (status.getSkill(m_data->Level) == 3 && m_data->skillmessages3 == false)
			{
				messages.push_back(Format(L"アベルは ばくれつけんを覚えた。"));
				m_data->skillmessages3 = true;
			}
			if (ModeScreen == mode::statusMessage)
			{
				statusMessage.setList(
				{
					Format(L"最大HP    +",status.MaxHp(m_data->Level) - status.MaxHp(m_data->Level - 1),L"    ",status.MaxHp(m_data->Level))
					,Format(L"最大MP    +",status.MaxMp(m_data->Level) - status.MaxMp(m_data->Level - 1),L"    ",status.MaxMp(m_data->Level))
					,Format(L"攻撃力    +",status.AttackPUp(m_data->Level) - status.AttackPUp(m_data->Level - 1),L"    ",status.AttackPUp(m_data->Level))
					,Format(L"防御力    +",status.DefencePUp(m_data->Level) - status.DefencePUp(m_data->Level - 1),L"    ",status.DefencePUp(m_data->Level))
					,Format(L"素早さ    +",status.SpeedPUp(m_data->Level) - status.SpeedPUp(m_data->Level - 1),L"    ",status.SpeedPUp(m_data->Level))
				});
			}
			finish = true;
			TextManager::text->start(messages);

			return;
		}

		// コマンドメニューの時の操作
		if (ModeScreen == mode::comandMenu)
		{
			menu.update();
			if (Input::KeyEnter.clicked)
			{
				SoundAsset(L"カーソル音").playMulti();
				if (menu.selected() == L"たたかう")
				{
					menu.setItem({ L"こうげき", L"とくぎ",L"",L""/*, L"ゆうごう", L"アイテム"*/ });
				}
				else if (menu.selected() == L"にげる")
				{
					if (escapeMessage == L"逃げられる")
					{
						if (escapeRandom() == 1)
						{
							TextManager::text->start({ L"うまくにげきれた" });
							finish = true;
						}
						else
						{
							battle(L"にげる");
							ModeScreen = mode::comandMenu;
						}
					}
					else if (escapeMessage == L"逃げられない")
					{
						TextManager::text->start({L"勝負で 逃げてはダメだ !!"});
						ModeScreen = mode::comandMenu;
					}
				}
				else if (menu.selected() == L"こうげき")
				{
					ModeScreen = mode::target;

					// すぐbattleを呼ばずに一度、変数に格納する.
					targetCommand = L"こうげき";

					// これを呼ばないとどうなるかやってみるといいかもしれない.
					resetTarget();
				}

				else if (menu.selected() == L"とくぎ")
				{
					ModeScreen = mode::skiimenu;
					listMenu.setList({ L"きあいだめ",L"メーレ",L"めいそう" });
					if (status.getSkill(m_data->Level) == 1)
					{
						listMenu.setList({ L"きあいだめ",L"メーレ", L"めいそう",L"ホープ" });
					}

					else if (status.getSkill(m_data->Level) == 2)
					{
						listMenu.setList({ L"きあいだめ",L"メーレ", L"めいそう",L"ホープ" ,L"ソニックジャブ" });
					}

					else if (status.getSkill(m_data->Level) == 3)
					{
						listMenu.setList({ L"きあいだめ",L"メーレ", L"ホープ",L"めいそう",L"ソニックジャブ",L"ばくれつけん" });
					}
				}
			}
			if (Input::KeySpace.clicked)
			{
				SoundAsset(L"キャンセル").playMulti();
				menu.setItem({ L"たたかう",L"にげる",L"",L"" });
			}

			return;
		}

		// スキルメニューの時の操作
		if (ModeScreen == mode::skiimenu)
		{
			listMenu.update();

			if (Input::KeyEnter.clicked)
			{
				SoundAsset(L"カーソル音").playMulti();
				if (listMenu.selected() == L"きあいだめ")
				{
					if (m_data->Mp >= 3)
					{
						battle(L"きあいだめ");
						ModeScreen = mode::comandMenu;
					}
					else
					{
						TextManager::text->start({ L"MPがたりない !" });
					}
				}

				if (listMenu.selected() == L"ばくれつけん")
				{
					if (m_data->Mp >= 5)
					{
						ModeScreen = mode::comandMenu;
						battle(L"ばくれつけん");
					}
					else
					{
						TextManager::text->start({ L"MPがたりない !" });
					}
				}

				if (listMenu.selected() == L"メーレ")
				{
					if (m_data->Mp >= 3)
					{
						ModeScreen = mode::comandMenu;
						battle(L"メーレ");
					}
					else
					{
						TextManager::text->start({ L"MPがたりない !" });
					}
				}

				if (listMenu.selected() == L"ホープ")
				{
					if (m_data->Mp >= 3)
					{
						ModeScreen = mode::comandMenu;
						battle(L"ホープ");
					}
					else
					{
						TextManager::text->start({ L"MPがたりない !" });
					}
				}
				if (listMenu.selected() == L"ソニックジャブ")
				{
					if (m_data->Mp >= 4)
					{
						ModeScreen = mode::target;
						targetCommand = L"ソニックジャブ";
						resetTarget();
					}
					else
					{
						TextManager::text->start({ L"MPがたりない !" });
					}
				}
				if (listMenu.selected() == L"めいそう")
				{
					ModeScreen = mode::comandMenu;
					battle(L"めいそう");
				}
			}
			if (Input::KeySpace.clicked)
			{
				SoundAsset(L"キャンセル").playMulti();
				ModeScreen = mode::comandMenu;
			}

			return;
		}

		// ターゲット選択時の操作
		if (ModeScreen == mode::target)
		{
			targetUpdate();

			if (Input::KeyEnter.clicked)
			{
				// 変数に格納したので "こうげき" 以外もターゲットを取れるように.
				battle(targetCommand);
				ModeScreen = mode::comandMenu;
			}

			if (Input::KeySpace.clicked)
			{
				SoundAsset(L"キャンセル").playMulti();
				ModeScreen = mode::comandMenu;
			}
		}
	}

	void draw() const override
	{
		Graphics::SetBackground(Color(15, 15, 15));
		for (auto& i : image)
		{
			i->draw();
		}
		//getBody()(TextureAsset(texture)).draw();
		for (uint32 i = 0; i < enemies.size(); i++)
		{
			enemies[i].draw();
		}

		menu.draw();

		if (ModeScreen == mode::skiimenu)
		{
			listMenu.draw();
		}

		if (ModeScreen == mode::statusMessage)
		{
			statusMessage.draw();
		}

		if (ModeScreen == mode::target)
		{
			const Triangle cursor({ 270, 220 }, { 330, 220 }, { 300, 260 });
			
			if (enemies.size() == 1)
			{
				cursor.movedBy(300 * 1, 0).draw(Palette::Black);
			}
			else
			{
				cursor.movedBy(300 * target, 0).draw(Palette::Black);
			}

			
		}

		const Rect frame(10, 20, 200, 210);

		frame.draw();
		frame.stretched(2).drawFrame(2, 0, Palette::Black);
		frame.stretched(-2).drawFrame(2, 0, Palette::Black);
		font(L"  アベル", L"\n\nHP    ", m_data->Hp, L"\nMP   ", m_data->Mp, L"\nLv :   ", m_data->Level).draw(20, 20, Palette::Black, 0.7);

		TextManager::text->draw();

		if (timer.ms() < 1000)
		{
			const Rect alert(250, 550, 760, 170);

			alert.draw();
			alert.stretched(2).drawFrame(2, 0, Palette::Black);
			alert.stretched(-2).drawFrame(2, 0, Palette::Black);
			font(Firstmessage).draw(310, 570, Palette::Black, 1.2);
		}
	}
	private:
};



class Battle :public AllBattle
{
public:

	void init()
	{
		
		AttackP = status.AttackPUp(m_data->Level);
		DefenceP = status.DefencePUp(m_data->Level);
		SpeedP = status.SpeedPUp(m_data->Level);
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);
		BattleM = SoundAsset(L"BattleM");
		SoundAsset(L"BattleM").setVolume(0.2);
		menu.setItem({ L"たたかう",L"にげる",L"",L"" });
		image.push_back(new Images({ 0,0 }, { 1200,750 }, L"forest"));
		const auto enemyNumber = Random(1u, 2u);
		escapeMessage = String(L"逃げられる");
		Firstmessage = String( L"モンスターがあらわれた");
		for (uint32 i = 0; i < enemyNumber; i++)
		{
			uint32 typeNumber = Random(1, 3);
			
			//引数　左から　名前 HP スピード　ぼうぎょ力　こうげき力　経験値　金
			if (enemyNumber == 1)
			{
				if (typeNumber == 1)
				{
					enemies.push_back(EnemyData(L"ワーウルフ", 15, 15, 9, 10, 13, 8, { L"かみつく",L"つめとぎ",L"けいかい" }, { 500,250 }, { 250,250 }));
				}
				else if (typeNumber == 2)
				{
					enemies.push_back(EnemyData(L"納豆坊", 20, 7, 8, 12, 11, 9, { L"ねばねば",L"つきさす",L"こうげき" }, { 500 ,250 }, { 250,250 }));
				}
				else if (typeNumber == 3)
				{
					enemies.push_back(EnemyData(L"キャロットマン", 12, 8, 6, 7, 10, 5, { L"つきさす",L"様子",L"メーレ" }, { 500 ,250 }, { 250,250 }));
				}
			}
			else
			{
				if (typeNumber == 1)
				{
					enemies.push_back(EnemyData(L"ワーウルフ", 15, 15, 9, 10, 13, 8, { L"かみつく",L"つめとぎ",L"けいかい" }, { 200 + i * 300,250 }, { 250,250 }));
				}
				else if (typeNumber == 2)
				{
					enemies.push_back(EnemyData(L"納豆坊", 20, 7, 8, 12, 11, 9, { L"ねばねば",L"つきさす",L"こうげき" }, { 200 + i * 300,250 }, { 250,250 }));
				}
				else if (typeNumber == 3)
				{
					enemies.push_back(EnemyData(L"キャロットマン", 12, 8, 6, 7, 10, 5, { L"つきさす",L"様子",L"メーレ" }, { 200 + i * 300,250 }, { 250,250 }));
				}
			}
			
		}

		Scene.push_back(String(L"FirstField"));
	}
};
	// 変わったように見えるが、実際には順番を入れ替えただけ.
	// 押されたキー中心の振り分けから mode 中心に変えて一連の処理を縦に見やすく.
	
class Battle2 :public AllBattle
{
public:

	void init()
	{
		image.push_back(new Images({ 0,0 }, { 1200,750 }, L"rockBackground"));
		AttackP = status.AttackPUp(m_data->Level);
		DefenceP = status.DefencePUp(m_data->Level);
		SpeedP = status.SpeedPUp(m_data->Level);
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);
		BattleM = SoundAsset(L"BattleM");
		SoundAsset(L"BattleM").setVolume(0.2);
		menu.setItem({ L"たたかう",L"にげる",L"",L"" });
		Firstmessage = String( L"モンスターがあらわれた" );
		escapeMessage = String(L"逃げられる");
		const auto enemyNumber = Random(1u, 2u);

		for (uint32 i = 0; i < enemyNumber; i++)
		{
			uint32 typeNumber = Random(1, 3);
			if (enemyNumber == 1)
			{
				//引数　左から　名前 HP スピード　ぼうぎょ力　こうげき力　経験値　金
				if (typeNumber == 1)
				{
					enemies.push_back(EnemyData(L"ワーウルフ", 24, 17, 9, 13, 15, 10, { L"かみつく",L"つめとぎ",L"けいかい" }, { 500 ,250 }, { 250,250 }));
				}

				else if (typeNumber == 2)
				{
					enemies.push_back(EnemyData(L"メタルエッグ", 15, 5, 17, 11, 18, 8, { L"のしかかり",L"ロックガード" }, { 500 ,250 }, { 250,250 }));
				}

				else if (typeNumber == 3)
				{
					enemies.push_back(EnemyData(L"とげボール", 20, 10, 7, 17, 16, 15, { L"とげラッシュ",L"様子" ,L"こうげき" }, { 500 ,250 }, { 250,250 }));
				}

			}
			else
			{
				//引数　左から　名前 HP スピード　ぼうぎょ力　こうげき力　経験値　金
				if (typeNumber == 1)
				{
					enemies.push_back(EnemyData(L"ワーウルフ", 24, 17, 9, 13, 15, 10, { L"かみつく",L"つめとぎ",L"けいかい" }, { 200 + i*300 ,250 }, { 250,250 }));
				}

				else if (typeNumber == 2)
				{
					enemies.push_back(EnemyData(L"メタルエッグ", 15, 5, 17, 11, 18, 8, { L"のしかかり",L"ロックガード"}, { 200 + i*300 ,250 }, { 250,250 }));
				}

				else if (typeNumber == 3)
				{
					enemies.push_back(EnemyData(L"とげボール", 20, 10, 7, 17, 16, 15, { L"とげラッシュ",L"様子" ,L"こうげき" }, { 200 + i * 300 ,250 }, { 250,250 }));
				}
			}
		}

		Scene.push_back(String(L"SecondField"));
	}
};

class Battle3 :public AllBattle
{
public:

	void init()
	{
		image.push_back(new Images({ 0,0 }, { 1200,750 }, L"rockBackground"));
		AttackP = status.AttackPUp(m_data->Level);
		DefenceP = status.DefencePUp(m_data->Level);
		SpeedP = status.SpeedPUp(m_data->Level);
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);
		BattleM = SoundAsset(L"BattleM");
		SoundAsset(L"BattleM").setVolume(0.2);
		menu.setItem({ L"たたかう",L"にげる",L"",L"" });
		escapeMessage = String(L"逃げられる");
		Firstmessage = String(L"モンスターがあらわれた");
		const auto enemyNumber = Random(2u, 3u);

		for (uint32 i = 0; i < enemyNumber; i++)
		{
			uint32 typeNumber = Random(1, 4);

				//引数　左から　名前 HP スピード　ぼうぎょ力　こうげき力　経験値　金
				if (typeNumber == 1)
				{
					enemies.push_back(EnemyData(L"ワーウルフ", 37, 23, 9, 20, 23, 15, { L"かみつく",L"つめとぎ",L"けいかい" }, { 200 + i * 300 ,250 }, { 250,250 }));
				}

				else if (typeNumber == 2)
				{
					enemies.push_back(EnemyData(L"メタルエッグ", 18, 5, 20, 16, 29, 11, { L"のしかかり",L"ロックガード",L"様子" }, { 200 + i * 300 ,250 }, { 250,250 }));
				}

				else if (typeNumber == 3)
				{
					enemies.push_back(EnemyData(L"とげボール", 30, 10, 7, 24, 25, 20, { L"とげラッシュ",L"様子" ,L"こうげき" }, { 200 + i * 300 ,250 }, { 250,250 }));
				}
				else if (typeNumber == 4)
				{
					enemies.push_back(EnemyData(L"納豆坊", 40, 7, 8, 18, 21, 18, { L"ねばねば",L"つきさす",L"こうげき" }, { 200 + i * 300,250 }, { 250,250 }));
				}
		}

		Scene.push_back(String(L"ThirdField"));
	}
};
class Arisu :public AllBattle
{
public:

	void init()
	{
		Firstmessage = String( L"魔法使いのアリスが\n勝負をしかけてきた" );
		AttackP = status.AttackPUp(m_data->Level);
		DefenceP = status.DefencePUp(m_data->Level);
		SpeedP = status.SpeedPUp(m_data->Level);
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);
		BattleM = SoundAsset(L"BattleM");
		SoundAsset(L"BattleM").setVolume(0.2);
		image.push_back(new Images({ 0,0 }, { 1200,750 }, L"forest"));
		menu.setItem({ L"たたかう",L"にげる",L"",L"" });
		escapeMessage = String(L"逃げられない");
		//引数　左から　名前 HP スピード　ぼうぎょ力　こうげき力　経験値　金
		enemies.push_back(EnemyData(L"アリス", 42, 15, 12, 18, 35, 20, { L"フレア", L"ルノバス" ,L"メーレ2",L"杖"}, { 500 ,250 }, { 155,235 }));
		Scene.push_back(String(L"FirstField2"));
	}

private:

};

class Donald :public AllBattle
{
public:

	void init()
	{
		image.push_back(new Images({ 0,0 }, { 1200,750 }, L"rockBackground"));
		Firstmessage = String(L"ピエロのドナルドが\n勝負をしかけてきた");
		escapeMessage = String(L"逃げられない");
		AttackP = status.AttackPUp(m_data->Level);
		DefenceP = status.DefencePUp(m_data->Level);
		SpeedP = status.SpeedPUp(m_data->Level);
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);
		BattleM = SoundAsset(L"BattleM");
		SoundAsset(L"BattleM").setVolume(0.2);
		menu.setItem({ L"たたかう",L"にげる",L"",L"" });
		//引数　左から　名前 HP スピード　ぼうぎょ力　こうげき力　経験値　金
		enemies.push_back(EnemyData(L"とげボール", 24, 10, 7, 20, 13, 15, { L"とげラッシュ",L"様子" }, { 200 ,250 }, { 250,250 }));
		enemies.push_back(EnemyData(L"ドナルド", 53, 18, 13, 24, 52, 20, { L"ただのパンチ", L"イリュージョンダンス" ,L"火吹き",L"ステップ" }, { 500 ,250 }, { 155,235 }));

		Scene.push_back(String(L"SecondField2"));
	}

private:

};
class Yellow :public AllBattle
{
public:

	void init()
	{
		image.push_back(new Images({ 0,0 }, { 1200,750 }, L"rockBackground"));
		Firstmessage = String( L"イエローが勝負をしかけてきた。");
		escapeMessage = String(L"逃げられない");
		AttackP = status.AttackPUp(m_data->Level);
		DefenceP = status.DefencePUp(m_data->Level);
		SpeedP = status.SpeedPUp(m_data->Level);
		townM = SoundAsset(L"Town");
		SoundAsset(L"Town").setVolume(0.2);
		BattleM = SoundAsset(L"BattleM");
		SoundAsset(L"BattleM").setVolume(0.2);
		menu.setItem({ L"たたかう",L"にげる",L"",L"" });
		//引数　左から　名前 HP スピード　ぼうぎょ力　こうげき力　経験値　金
		enemies.push_back(EnemyData(L"ワーウルフ", 35, 26, 10, 21, 20, 20, { L"かみつく",L"つめとぎ",L"けいかい" }, { 200 ,250 }, { 250,250 }));
		enemies.push_back(EnemyData(L"イエロー", 65, 20, 15, 28, 60, 40, { L"フレア", L"ボルテックス",  L"はどうショット", L"幻惑の瞳" ,L"メーレ3"}, { 500 ,250 }, { 155,235 }));
		enemies.push_back(EnemyData(L"メタルエッグ", 21, 5, 22, 18, 20, 20, { L"のしかかり",L"ロックガード" ,L"ボディープレス" }, { 800 ,250 }, { 250,250 }));

		Scene.push_back(String(L"Clear"));
	}
};



class GameOver :public SceneManager<String, Gamedata>::Scene
{
public:
	Font font = Font(60);
	Status status;
	const String text4 = L"Game Over";
	const String text5 = L"Please click space";
	void draw() const override
	{
		//Graphics::SetBackground(Palette::Blue);
		font(text4).drawCenter(600, 300);
		font(text5).drawCenter(600, 500);
	}
	void update() override
	{
		if (Input::KeySpace.clicked)
		{
			changeScene(L"In3");
			m_data->pos = { 0,400 };
			m_data->Hp = status.MaxHp(m_data->Level)/2;
			m_data->Mp = status.MaxMp(m_data->Level)/2;
		}
	}
private:

};

class Break :public SceneManager<String, Gamedata>::Scene
{
public:
	Font font = Font(60);
	const String text6 = L"エンターキーをおしてね";
	Status status;
	Sound breakM, townM;
	void draw() const override
	{
		Graphics::SetBackground(Palette::Blue);
		font(text6).drawCenter(600, 300);
	}

	void update() override
	{
		if (Input::KeyEnter.clicked)
		{
			changeScene(L"In3");
			m_data->Hp = status.MaxHp(m_data->Level), m_data->Mp = status.MaxMp(m_data->Level);
		}
	}

};