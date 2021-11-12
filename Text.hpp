#pragma once
#include"Object.hpp"
class Text
{
public:

	bool onFinish = false;
	int32 duration = 40;
	Stopwatch stopwatch;
	String flag;
	Font font;
	Array<Array<String>> scenario = 
	{
		Array<String>
		{
			L"住民A「かがくの　ちからって　すげー！\nSiv3D を　つかうと　みじかい　コードで",
			L"えや　もじを　ひょうじしたり\nおんがくを　ならしたり　できるんだ！」"
		},
		Array<String>
		{
			L"ピエロ「ついに 旅に出るんだな。\nあたらしい であい やモンスターが",
			L"きみを まっているぞ !」"
		},
		Array<String>
		{
			L"住民B「ここは いなかだからな 村をでると\nすぐモンスターがおそってくる。",
			L"はやく都会に引っ越したいものだな」"
		},
		Array<String>
		{
			L"住民C「アベル。イエローとのバトルに\n挑むんだろ!?  あいつなら、",
			L"ここから北の砂漠にいるはずだぜ !!」"
		},
		Array<String>
		{
			L"アベル「今日 は 寝るか・・」",
			L"flag Break"
		},
		Array<String>
		{
			L"メアリー「このさき モンスターがでるけど \nあなたの パワーがあれば倒せるはずよ",
			L"つかれたら家のベッドで休むといいわ。」"
		},
		Array<String>
		{
			L"さっき落石があってな。このさき 通れない\nんだ。すまないがべつの道を行ってくれ。",
		},
		
		Array<String>
		{
			L"アリス「イエローからの挑戦を受ける\nのね。 それならうでだめしに",
			L" このわたしとバトルしましょ !」",
			L"flag Arisu"
		},
		Array<String>
		{
			L"イエロー「この俺を倒しに来たのか...\nせいぜい楽しませてくれよ !」",
			L"flag Yellow"
		},
		Array<String>
		{
			L"敵が群れでおそってきたらさすがの\nおれでもにげるしかないぜ",
			L"おれにもっとじゅもんの 才能があれば...."
		},
		Array<String>
		{
			L"アリス「なかなかやるわね。\n私の負けよ。」"
		},
		Array<String>
		{
			L"ドナルド「目があったらバトルです。\nいきますよ とげボール !!!」",
			L"flag Donald"
		},
		Array<String>
		{
			L"イエロー「俺とのバトルに挑みたいものは\n北の砂漠までこい !!」"
		},
		Array<String>
		{
			L"ドナルド「イエローは2体のモンスター\nを従えています。 レベルを上げて",
			L"全体攻撃の技を覚えた方が\nいいかもしれません」"
		},
		Array<String>
		{
			L"チャレンジャー「あのイエローってやつ...\n おいらじゃ手も足も出なかったぞ",
			L"お前も心して挑んだ方がいいぜ ！」"
		}
	};

	int32 messageIndex;
	Array<String> messages;
	int32 index;

	bool all;

	Text();

	String getFlag()
	{
		String flag2;
		flag2 = flag;
		flag = L"";
		return flag2;
	}

	Rect frame() const;

	void text();

	void update();

	void draw() const;

	void start(int32 phrase);

	bool isActive() const;

	void start(const Array<String> &text);
};
