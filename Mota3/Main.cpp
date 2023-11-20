#include "Main.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// 基本设置
	System.BasicSet();
	int Width = WindowWidth * ResolutionRatio, Height = WindowHeight * ResolutionRatio;
	initgraph(Width, Height);
	System.Initialization();
	// 窗口居中
	int scrWidth = GetSystemMetrics(SM_CXSCREEN), scrHeight = GetSystemMetrics(SM_CYSCREEN);
	RECT rect;
	GetWindowRect(GameActive, &rect);
	SetWindowPos(GameActive, HWND_TOP, (scrWidth - Width) / 2, (scrHeight - Height) / 2, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
	// 准备描绘
	BeginBatchDraw();
	// 生成场景对象（标题画面）
	System.Scene = new GameTitle;
	// 准备渐变
	System.ReadyTransition();
	// Scene有效时一直执行Main函数
	while (System.Scene != nullptr)
		System.Scene->Main();
	// 执行渐变
	System.DoTransition();
	// 释放内存
	Graphics.Dispose();
	EndBatchDraw();
	closegraph();
	return 0;
}

WindowMessage::WindowMessage(int x, int y) {
	X = x;
	Y = y;
	Z = 0;
	Width = 256;
	Height = 128;
	Opacity = 0;
	HasFunction = true;
	Visible = false;
	Graphics.Windows.push_back(this);
}

void WindowMessage::Refresh() {
	// 不可视和消息队列为空时不执行
	if (System.MessageName.empty()) return;
	if (!Visible) return;
	// 设置描绘格式
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	// 写入人物名和文本内容
	string name = *System.MessageName.begin();
	string content = *System.MessageContent.begin();
	int head = *System.MessageHead.begin();
	if (name != "") { // 人物名不为空时
		X = System.MapX + 16;
		Y = System.MapY + 160;
		Width = 320;
		Height = 160;
		// 为NPC时调整对话框坐标
		if (head >= 0) Y = System.MapY + 32;
		// 绘制人物头像和名字
		if (Opacity > 0) {
			if (head == -1) WindowBitmap(Actors[System.GameVariables[1]].File, 16, 16, 32, 32, 0, 0, 255);
			else WindowBitmap(System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[head].File[System.GameTime / 6 % 2], 16, 16);
			rectangle((X + 16) * ResolutionRatio, (Y + 16) * ResolutionRatio, (X + 48) * ResolutionRatio, (Y + 48) * ResolutionRatio);
			WDrawTexts(48, 0, 160, 20, name, 1, 24L, true);
		}
	}
	else {
		X = System.MapX - 16;
		Y = System.MapY + 192;
		Width = 384;
		Height = 32 + (int)(content.length() / 46) * 32;
	}
	// 绘制文本内容
	if (Opacity > 0) WDrawTexts(0, 32 * (name != "") - 8 * (name == ""), Width - 32, 96, content, 3 + (name == ""), 20L);
	// 还原描绘格式
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	Opacity = System.WindowOpacity;
}

WindowMonsterBook::WindowMonsterBook(int x, int y) {
	X = x;
	Y = y;
	Z = 0;
	Width = 352;
	Height = 352;
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Visible = false;
	Graphics.Windows.push_back(this);
}

void WindowMonsterBook::Refresh() {
	// 不可视时不执行
	if (!Visible) return;
	// 按下取消键时
	if (KeyBoard.TriggerCancel()) {
		PlaySE(System.CancelSE);
		Visible = false;
		return;
	}
	// 无敌人时不执行
	if (FloorEnemies.empty()) return;
	// 设置页数
	MaxPage = FloorEnemies.size() / 5;
	if (FloorEnemies.size() / 5.0 != MaxPage) ++MaxPage;
	string ele1 = "", ele2 = "";
	// 描绘敌人属性
	for (int i = 0; i < min(5, FloorEnemies.size() - 5 * Page); ++i) {
		WindowBitmap(Enemies[FloorEnemies[Page * 5 + i]].File[System.GameTime / 6 % 2], 16, 16 + 64 * i);
		setlinecolor(GREEN);
		setlinestyle(PS_SOLID, 2);
		rectangle((X + 16) * ResolutionRatio, (Y + 16 + 64 * i) * ResolutionRatio, (X + 48) * ResolutionRatio, (Y + 48 + 64 * i) * ResolutionRatio);
		setlinecolor(WHITE);
		setlinestyle(PS_SOLID, 1);
		WDrawTexts(0, 32 + 64 * i, 96, 32, Split(Enemies[FloorEnemies[Page * 5 + i]].Name, "|")[0], 3, 18L);
		if (Enemies[FloorEnemies[Page * 5 + i]].Element.size() == 1) {
			ele1 = Enemies[FloorEnemies[Page * 5 + i]].EnemyElement(Enemies[FloorEnemies[Page * 5 + i]].Element[0]).first;
			ele2 = "";
		}
		else if (Enemies[FloorEnemies[Page * 5 + i]].Element.size() == 2) {
			ele1 = Enemies[FloorEnemies[Page * 5 + i]].EnemyElement(Enemies[FloorEnemies[Page * 5 + i]].Element[0]).first;
			ele2 = Enemies[FloorEnemies[Page * 5 + i]].EnemyElement(Enemies[FloorEnemies[Page * 5 + i]].Element[1]).first;
		}
		else if (Enemies[FloorEnemies[Page * 5 + i]].Element.size() >= 3) {
			ele1 = "详细…";
			ele2 = "";
		}
		WDrawTexts(32, 64 * i, 96, 16, ele1, 0, 18L);
		WDrawTexts(32, 64 * i + 16, 96, 16, ele2, 0, 18L);
		WDrawTexts(80, 64 * i, 96, 32, "体力：" + to_string(Enemies[FloorEnemies[Page * 5 + i]].Hp), 0, 20L);
		WDrawTexts(160, 64 * i, 96, 32, "攻击：" + to_string(Enemies[FloorEnemies[Page * 5 + i]].GotAtk()), 0, 20L);
		WDrawTexts(240, 64 * i, 96, 32, "防御：" + to_string(Enemies[FloorEnemies[Page * 5 + i]].GotDef()), 0, 20L);
		WDrawTexts(80, 20 + 64 * i, 96, 32, "经验：" + to_string(Enemies[FloorEnemies[Page * 5 + i]].Exp), 0, 20L);
		WDrawTexts(160, 20 + 64 * i, 96, 32, "金币：" + to_string(Enemies[FloorEnemies[Page * 5 + i]].Gold), 0, 20L);
		WDrawTexts(240, 20 + 64 * i, 96, 32, "气息：" + to_string(Enemies[FloorEnemies[Page * 5 + i]].Breath), 0, 20L);
		WDrawTexts(240, 40 + 64 * i, 96, 32, "伤害：" + Enemies[FloorEnemies[Page * 5 + i]].GotDamage().second, 0, 20L);
	}
	// 绘制选择矩形
	DrawRect(16, 16 + 64 * Index, Width - 32, 64);
	// 当前页数/总页数
	WindowDrawArrow(232, 336, Page, MaxPage - 1, 64);
	settextcolor(WHITE);
	// 按下左键时
	if (KeyBoard.Press(KeyBoard.KeyLEFT)) {
		if (Page > 0) {
			--Page;
			Index = 0;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// 按下右键时
	if (KeyBoard.Press(KeyBoard.KeyRIGHT)) {
		if (Page < MaxPage - 1) {
			++Page;
			Index = 0;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// 按下上键时
	if (KeyBoard.Press(KeyBoard.KeyUP)) {
		if (Index > 0) {
			--Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// 按下下键时
	if (KeyBoard.Press(KeyBoard.KeyDOWN)) {
		if (Index < min(5, FloorEnemies.size() - 5 * Page) - 1) {
			++Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
}

WindowEncyclopedia::WindowEncyclopedia(int x, int y) {
	X = x;
	Y = y;
	Z = 0;
	Width = 352;
	Height = 352;
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Visible = false;
	Graphics.Windows.push_back(this);
}

void WindowEncyclopedia::Refresh() {
	// 不可视时不执行
	if (!Visible) return;
	// 描绘敌人属性
	WindowBitmap(Enemies[eid].File[System.GameTime / 6 % 2], 128, 16);
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	rectangle((X + 128) * ResolutionRatio, (Y + 16) * ResolutionRatio, (X + 160) * ResolutionRatio, (Y + 48) * ResolutionRatio);
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	WDrawTexts(160, 6, 160, 20, Split(Enemies[eid].Name, "|")[0], 0, 22L, true);
	WDrawTexts(0, 40, 128, 20, "体力:" + to_string(Enemies[eid].Hp), 0, 20L);
	WDrawTexts(85, 40, 128, 20, "攻击:" + to_string(Enemies[eid].GotAtk()), 0, 20L);
	WDrawTexts(170, 40, 128, 20, "防御:" + to_string(Enemies[eid].GotDef()), 0, 20L);
	WDrawTexts(255, 40, 128, 20, "气息:" + to_string(Enemies[eid].Breath), 0, 20L);
	WDrawTexts(0, 60, 128, 20, "经验:" + to_string(Enemies[eid].Exp), 0, 20L);
	WDrawTexts(85, 60, 128, 20, "金币:" + to_string(Enemies[eid].Gold), 0, 20L);
	WDrawTexts(170, 60, 128, 20, "临界:" + (Enemies[eid].GotCrisis() >= 0 ? to_string(Enemies[eid].GotCrisis()) : "无"), 0, 20L);
	WDrawTexts(255, 60, 128, 20, "伤害:" + Enemies[eid].GotDamage().second, 0, 20L);
	WDrawTexts(0, 80, 128, 20, "临界伤害:" + (Enemies[eid].GotCrisis() >= 0 ? Enemies[eid].GotDamage(Enemies[eid].GotCrisis()).second : "无"), 0, 20L);
	WDrawTexts(0, 120, 128, 20, "属性:", 0, 20L);
	for (int i = 0; i < Enemies[eid].Element.size(); ++i) {
		WDrawTexts(0, 140 + 20 * i, 320, 60, Enemies[eid].EnemyElement(Enemies[eid].Element[i]).first, 0, 20L);
		WDrawTexts(48, 140 + 20 * i, 288, 60, Enemies[eid].EnemyElement(Enemies[eid].Element[i]).second, 3, 20L);
	}
}

WindowBattle::WindowBattle(int x, int y) {
	X = x;
	Y = y;
	Z = 0;
	Width = 384;
	Height = 224;
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Visible = false;
	Graphics.Windows.push_back(this);
}

void WindowBattle::Refresh() {
	// 不可视时不执行
	if (!Visible || System.BattleEnemyID < 0) return;
	// 设置描绘格式
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	// 绘制矩形框
	rectangle((X + 32) * ResolutionRatio, (Y + 32) * ResolutionRatio, (X + 64) * ResolutionRatio, (Y + 64) * ResolutionRatio);
	rectangle((X + Width - 64) * ResolutionRatio, (Y + 32) * ResolutionRatio, (X + Width - 32) * ResolutionRatio, (Y + 64) * ResolutionRatio);
	// 还原描绘格式
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	// 描绘双方头像
	WindowBitmap(Enemies[System.BattleEnemyID].File[System.GameTime / 6 % 2], 32, 32);
	WindowBitmap(Actors[System.GameVariables[1]].File, Width - 64, 32, 32, 32, 0, 0);
	// 描绘buff和debuff
	as(buff, 1, 6) BitmapCentre("mting-" + to_string(1291 + buff) + ".png", X + Width - 32, Y + 32);
	if (eweaken) BitmapCentre("mting-710.png", X + 32, Y + 20, 200);
	if (weaken) BitmapCentre("mting-710.png", X + Width - 64, Y + 20, 200);
	// 根据funckey变动提示
	if (funckey == 2) BitmapCentre("mting-1227.png", X + Width - 24, Y + 128);
	else BitmapCentre("mting-1241.png", X + Width - 24, Y + 128);
	as (funckey, 3, 7) BitmapCentre("mting-1227.png", X + Width - 48, Y + 128);
	else BitmapCentre("mting-1215.png", X + Width - 48, Y + 128);
	as (funckey, 8, 12) BitmapCentre("mting-1227.png", X + Width - 72, Y + 128);
	else BitmapCentre("mting-1214.png", X + Width - 72, Y + 128);
	// 描绘双方属性
	WDrawTexts(0, -8, 128, 24, Split(Enemies[System.BattleEnemyID].Name, "|")[0], 1, 22L, true);
	WDrawTexts(72, 16, 64, 20, "体力:", 0, 20L);
	WDrawTexts(72, 40, 64, 20, "攻击力:", 0, 20L);
	WDrawTexts(72, 64, 64, 20, "防御力:", 0, 20L);
	WDrawTexts(72, 88, 64, 20, "疲劳:", 0, 20L);
	WDrawTexts(0, 136, 64, 20, "气息:", 0, 20L);
	WDrawTexts(Width - 112, -8, 96, 24, Actors[System.GameVariables[1]].Name, 1, 22L, true);
	WDrawTexts(Width - 168, 16, 64, 24, ":体力", 2, 20L);
	WDrawTexts(Width - 168, 40, 64, 24, ":攻击力", 2, 20L);
	WDrawTexts(Width - 168, 64, 64, 24, ":防御力", 2, 20L);
	WDrawTexts(Width - 168, 88, 64, 24, ":疲劳", 2, 20L);
	WDrawTexts(Width - 168, 136, 64, 24, "气息:", 2, 20L);
	if (System.GameVariables[13] == 2) WDrawTexts(Width - 168, 156, 96, 24, ":累计伤害", 2, 20L);
	// 各属性变化提示
	if (eVaryAtk != 0) {
		string str = "(";
		if (eVaryAtk < 0)
			settextcolor(GameWindow::TextColor("red"));
		else {
			settextcolor(GameWindow::TextColor("green"));
			str = str + "+";
		}
		WDrawTexts(32, 40, 40, 20, str + to_string(eVaryAtk) + ")", 2, 12L);
	}
	if (eVaryDef != 0) {
		string str = "(";
		if (eVaryDef < 0)
			settextcolor(GameWindow::TextColor("red"));
		else {
			settextcolor(GameWindow::TextColor("green"));
			str = str + "+";
		}
		WDrawTexts(32, 64, 40, 20, str + to_string(eVaryDef) + ")", 2, 12L);
	}
	if (aVaryAtk != 0) {
		string str = "(";
		if (aVaryAtk < 0)
			settextcolor(GameWindow::TextColor("red"));
		else {
			settextcolor(GameWindow::TextColor("green"));
			str = str + "+";
		}
		WDrawTexts(Width - 104, 40, 40, 20, str + to_string(aVaryAtk) + ")", 0, 12L);
	}
	if (aVaryDef != 0) {
		string str = "(";
		if (aVaryDef < 0)
			settextcolor(GameWindow::TextColor("red"));
		else {
			settextcolor(GameWindow::TextColor("green"));
			str = str + "+";
		}
		WDrawTexts(Width - 104, 64, 40, 20, str + to_string(aVaryDef) + ")", 0, 12L);
	}
	settextcolor(WHITE);
	// 撤退按键
	settextcolor(GameWindow::TextColor("yellow"));
	WDrawTexts(-8, 176, 128, 24, "～撤退(Q)～", 0, 22L, true);
	settextcolor(WHITE);
	// 描绘双方数值
	WDrawTexts(120, 16, 64, 20, to_string(eHp), 0, 20L);
	WDrawTexts(120, 40, 64, 20, to_string(eAtk), 0, 20L);
	WDrawTexts(120, 64, 64, 20, to_string(eDef), 0, 20L);
	WDrawTexts(120, 88, 64, 20, to_string(eFatigue), 0, 20L);
	WDrawTexts(Width - 216, 16, 64, 24, to_string(aHp), 2, 20L);
	WDrawTexts(Width - 216, 40, 64, 24, to_string(aAtk), 2, 20L);
	WDrawTexts(Width - 216, 64, 64, 24, to_string(aDef), 2, 20L);
	WDrawTexts(Width - 216, 88, 64, 24, to_string(aFatigue), 2, 20L);
	if (System.GameVariables[13] == 2) WDrawTexts(Width - 216, 156, 64, 24, to_string(LastDamage), 2, 20L);
	// 描绘气息条
	IMAGE* rect = new IMAGE, * rect2 = new IMAGE;
	loadimage(rect, "resource\\picture\\mting-1226.png", 80 * ResolutionRatio, 16 * ResolutionRatio);
	loadimage(rect2, "resource\\picture\\mting-1226.png", 80 * ResolutionRatio,8 * ResolutionRatio);
	OpaqueImage(nullptr, X + 56, Y + 160, 80, 8, 0, 0, rect);
	OpaqueImage(nullptr, X + 56, Y + 160, eBreath * 80 / Enemies[System.BattleEnemyID].Breath, 8, 0, 8, rect);
	OpaqueImage(nullptr, X + Width - 88, Y + 160, 80, 4, 0, 0, rect2);
	OpaqueImage(nullptr, X + Width - 88, Y + 160, aBreath % (Actors[System.GameVariables[1]].BreathLimit / 6) * 80 / (Actors[System.GameVariables[1]].BreathLimit / 6), 4, 0, 4, rect2);
	// 描绘气息格子
	string breathfile1[] = { "mting-1211.png", "mting-1212.png", "mting-1213.png" };
	string breathfile2[] = { "mting-1211_g.png", "mting-1212_g.png", "mting-1213_g.png" };
	for (int i = 0; i < 5; ++i)
		WindowBitmap(aBreath >= Actors[System.GameVariables[1]].BreathLimit / 6 * (i + 1) ? "mting-1211.png" : "mting-1212.png", Width - 88 + 14 * i, 145);
	delete rect;
	delete rect2;
	// 死亡时如果还在加速状态则取消
	if (aHp <= 0) {
		if (System.Accelerate) System.Accelerate = false;
		funckey = 0;
		return;
	}
	// 按下V键时，深呼吸
	if (KeyBoard.Trigger(KeyBoard.KeyV) && aBreath >= Actors[System.GameVariables[1]].BreathLimit / 6) {
		aBreath -= Actors[System.GameVariables[1]].BreathLimit / 6;
		aFatigue = max(0, aFatigue - ((int)(Actors[System.GameVariables[1]].Level / 10) + 1) * 10);
		return;
	}
	// 按下Q键时撤退
	if (KeyBoard.Press(KeyBoard.KeyQ)) {
		funckey = 1;
		return;
	}
	// 按下C键时会心一击
	if (KeyBoard.Trigger(KeyBoard.KeyC) && funckey == 0) {
		funckey = 2;
		return;
	}
	// 按下Z键时使用剑技
	if (KeyBoard.Trigger(KeyBoard.KeyZ) && funckey == 0) {
		if (System.GameVariables[13] != 0) {
			System.HitAchievement(31);
			funckey = 2 + System.GameVariables[13];
			System.GameSwitches[5 + System.GameVariables[13]] = true;
			if (System.GameSwitches[6] && System.GameSwitches[7] && System.GameSwitches[8] && System.GameSwitches[9] && System.GameSwitches[10])
				System.HitAchievement(33);
		}
		return;
	}
	// 按下X键时使用防御术
	if (KeyBoard.Trigger(KeyBoard.KeyX) && funckey == 0) {
		if (System.GameVariables[14] != 0) {
			System.HitAchievement(32);
			funckey = 7 + System.GameVariables[14];
			System.GameSwitches[10 + System.GameVariables[14]] = true;
			if (System.GameSwitches[11] && System.GameSwitches[12] && System.GameSwitches[13] && System.GameSwitches[14] && System.GameSwitches[15])
				System.HitAchievement(34);
		}
		return;
	}
	// 按下SPACE键时加速战斗
	if (KeyBoard.Press(KeyBoard.KeySPACE) && !System.Accelerate) {
		System.Accelerate = true;
		return;
	}
}

WindowGoldShop::WindowGoldShop(int x, int y) {
	X = x;
	Y = y;
	Z = 0;
	Width = 224;
	Height = 256;
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Visible = false;
	Graphics.Windows.push_back(this);
}

void WindowGoldShop::Refresh() {
	// 不可视时不执行
	if (!Visible) return;
	// 设置初始价格
	if (System.GameVariables[8] == 0) System.GameVariables[8] = 20;
	if (System.GameVariables[9] == 0) System.GameVariables[9] = 50;
	// 描绘商店头像
	string face[] = { "mting-495.png", "mting-496.png" };
	WindowBitmap(face[System.GameTime / 6 % 2], 32, 32);
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	rectangle((X + 32) * ResolutionRatio, (Y + 32) * ResolutionRatio, (X + 64) * ResolutionRatio, (Y + 64) * ResolutionRatio);
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	// 描绘文本内容
	WDrawTexts(64, 0, 112, 32, "贪婪之神", 1, 24L, true);
	WDrawTexts(64, 32, 128, 64, "如果你能给我" + to_string(System.GameVariables[8 + ShopID - 1]) + "金币，我便能满足你一个要求。", 3, 20L);
	WDrawTexts(0, 112, Width, 32, "体力+" + to_string(750 * ShopID - 100), 1);
	WDrawTexts(0, 144, Width, 32, "攻击+" + to_string(3 * ShopID), 1);
	WDrawTexts(0, 176, Width, 32, "防御+" + to_string(3 * ShopID), 1);
	WDrawTexts(0, 208, Width, 32, "离开", 1);
	// 描绘选择矩形
	DrawRect(64, 132 + Index * 32, 128, 24);
	// 按下上键时
	if (KeyBoard.Press(KeyBoard.KeyUP)) {
		if (Index > 0) {
			--Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// 按下下键时
	if (KeyBoard.Press(KeyBoard.KeyDOWN)) {
		if (Index < 3) {
			++Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// 按下确认键时
	if (KeyBoard.PressConfirm()) {
		if (Index == 0) {
			if (Actors[System.GameVariables[1]].Gold >= System.GameVariables[8 + ShopID - 1]) {
				Actors[System.GameVariables[1]].Hp += 750 * ShopID - 100;
				Actors[System.GameVariables[1]].Gold -= System.GameVariables[8 + ShopID - 1];
				System.GameVariables[8 + ShopID - 1] += ShopID;
				PlaySE(System.ShopSE);
			}
			else
				PlaySE(System.BuzzerSE);
			return;
		}
		if (Index == 1) {
			if (Actors[System.GameVariables[1]].Gold >= System.GameVariables[8 + ShopID - 1]) {
				Actors[System.GameVariables[1]].Atk += 3 * ShopID;
				Actors[System.GameVariables[1]].Gold -= System.GameVariables[8 + ShopID - 1];
				System.GameVariables[8 + ShopID - 1] += ShopID;
				PlaySE(System.ShopSE);
				if (Actors[System.GameVariables[1]].Atk >= 2000) System.HitAchievement(6);
			}
			else
				PlaySE(System.BuzzerSE);
			return;
		}
		if (Index == 2) {
			if (Actors[System.GameVariables[1]].Gold >= System.GameVariables[8 + ShopID - 1]) {
				Actors[System.GameVariables[1]].Def += 3 * ShopID;
				Actors[System.GameVariables[1]].Gold -= System.GameVariables[8 + ShopID - 1];
				System.GameVariables[8 + ShopID - 1] += ShopID;
				PlaySE(System.ShopSE);
				if (Actors[System.GameVariables[1]].Def >= 2000) System.HitAchievement(7);
			}
			else
				PlaySE(System.BuzzerSE);
			return;
		}
		if (Index == 3) {
			PlaySE(System.CancelSE);
			System.ShopType = 0;
			Visible = false;
			return;
		}
	}
	// 按下取消键时
	if (KeyBoard.TriggerCancel()) {
		PlaySE(System.CancelSE);
		System.ShopType = 0;
		Visible = false;
		return;
	}
}

WindowEXPShop::WindowEXPShop(int x, int y) {
	X = x;
	Y = y;
	Z = 0;
	Width = 224;
	Height = 256;
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Visible = false;
	Graphics.Windows.push_back(this);
}

void WindowEXPShop::Refresh() {
	// 不可视时不执行
	if (!Visible) return;
	// 设置初始价格
	int exprequire[2][3] = { { 90, 40, 40 },{ 240, 100, 100 } }, lvadd[2] = { 1,3 }, atkadd[2] = { 1,3 }, defadd[2] = { 2,5 };
	// 描绘商店头像
	string face[] = { "mting-508.png", "mting-509.png" };
	WindowBitmap(face[System.GameTime / 6 % 2], 32, 32);
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	rectangle((X + 32) * ResolutionRatio, (Y + 32) * ResolutionRatio, (X + 64) * ResolutionRatio, (Y + 64) * ResolutionRatio);
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	// 描绘文本内容
	WDrawTexts(64, 0, 112, 32, "战斗之神", 1, 24L, true);
	WDrawTexts(64, 32, 128, 64, "我可以将你的经验值转化为你的实质力量！", 3, 20L);
	WDrawTexts(0, 112, Width, 32, "升" + to_string(lvadd[ShopID - 1]) + "等级（" + to_string(exprequire[ShopID - 1][0]) + "exp）", 1);
	WDrawTexts(0, 144, Width, 32, "攻击+" + to_string(atkadd[ShopID - 1]) + "（" + to_string(exprequire[ShopID - 1][1]) + "exp）", 1);
	WDrawTexts(0, 176, Width, 32, "防御+" + to_string(defadd[ShopID - 1]) + "（" + to_string(exprequire[ShopID - 1][2]) + "exp）", 1);
	WDrawTexts(0, 208, Width, 32, "离开", 1);
	// 描绘选择矩形
	DrawRect(64, 132 + Index * 32, 128, 24);
	// 按下上键时
	if (KeyBoard.Press(KeyBoard.KeyUP)) {
		if (Index > 0) {
			--Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// 按下下键时
	if (KeyBoard.Press(KeyBoard.KeyDOWN)) {
		if (Index < 3) {
			++Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// 按下确认键时
	if (KeyBoard.PressConfirm()) {
		if (Index == 0) {
			if (Actors[System.GameVariables[1]].Exp >= exprequire[ShopID - 1][0]) {
				Actors[System.GameVariables[1]].Level += lvadd[ShopID - 1];
				if (lvadd[ShopID - 1] + Actors[System.GameVariables[1]].Level > 60) Actors[System.GameVariables[1]].BreathLimit += max(0, 60 - Actors[System.GameVariables[1]].Level) * 6;
				else Actors[System.GameVariables[1]].BreathLimit += 6 * lvadd[ShopID - 1];
				Actors[System.GameVariables[1]].Hp += 250 * lvadd[ShopID - 1];
				Actors[System.GameVariables[1]].Atk += 3 * lvadd[ShopID - 1];
				Actors[System.GameVariables[1]].Def += 3 * lvadd[ShopID - 1];
				Actors[System.GameVariables[1]].Exp -= exprequire[ShopID - 1][0];
				PlaySE(System.ShopSE);
				if (Actors[System.GameVariables[1]].Level >= 135) System.HitAchievement(8);
			}
			else
				PlaySE(System.BuzzerSE);
			return;
		}
		if (Index == 1) {
			if (Actors[System.GameVariables[1]].Exp >= exprequire[ShopID - 1][1]) {
				Actors[System.GameVariables[1]].Atk += atkadd[ShopID - 1];
				Actors[System.GameVariables[1]].Exp -= exprequire[ShopID - 1][1];
				PlaySE(System.ShopSE);
			}
			else
				PlaySE(System.BuzzerSE);
			return;
		}
		if (Index == 2) {
			if (Actors[System.GameVariables[1]].Exp >= exprequire[ShopID - 1][2]) {
				Actors[System.GameVariables[1]].Def += defadd[ShopID - 1];
				Actors[System.GameVariables[1]].Exp -= exprequire[ShopID - 1][2];
				PlaySE(System.ShopSE);
			}
			else
				PlaySE(System.BuzzerSE);
			return;
		}
		if (Index == 3) {
			PlaySE(System.CancelSE);
			System.ShopType = 0;
			Visible = false;
			return;
		}
	}
	// 按下取消键时
	if (KeyBoard.TriggerCancel()) {
		PlaySE(System.CancelSE);
		System.ShopType = 0;
		Visible = false;
		return;
	}
}

WindowKeyShop::WindowKeyShop(int x, int y) {
	X = x;
	Y = y;
	Z = 0;
	Width = 224;
	Height = 256;
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Visible = false;
	Graphics.Windows.push_back(this);
}

void WindowKeyShop::Refresh() {
	// 不可视时不执行
	if (!Visible) return;
	// 设置初始价格
	int keyrequire[] = { 20,80 };
	// 描绘商店头像
	string face[] = { "mting-504.png", "mting-505.png" };
	WindowBitmap(face[System.GameTime / 6 % 2], 32, 32);
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	rectangle((X + 32) * ResolutionRatio, (Y + 32) * ResolutionRatio, (X + 64) * ResolutionRatio, (Y + 64) * ResolutionRatio);
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	// 描绘文本内容
	WDrawTexts(64, 0, 112, 32, "盗贼", 1, 24L, true);
	WDrawTexts(64, 32, 128, 64, "嗨小哥，又来买钥匙啦", 3, 20L);
	WDrawTexts(0, 112, Width, 32, "黄钥匙（" + to_string(keyrequire[0]) + "金币）", 1);
	WDrawTexts(0, 144, Width, 32, "蓝钥匙（" + to_string(keyrequire[1]) + "金币）", 1);
	WDrawTexts(0, 176, Width, 32, "离开", 1);
	// 描绘选择矩形
	DrawRect(64, 132 + Index * 32, 128, 24);
	// 按下上键时
	if (KeyBoard.Press(KeyBoard.KeyUP)) {
		if (Index > 0) {
			--Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// 按下下键时
	if (KeyBoard.Press(KeyBoard.KeyDOWN)) {
		if (Index < 2) {
			++Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// 按下确认键时
	if (KeyBoard.PressConfirm()) {
		if (Index == 0) {
			if (Actors[System.GameVariables[1]].Gold >= keyrequire[0]) {
				Actors[System.GameVariables[1]].Gold -= keyrequire[0];
				++Actors[System.GameVariables[1]].Item[0];
				PlaySE(System.ShopSE);
			}
			else
				PlaySE(System.BuzzerSE);
			return;
		}
		if (Index == 1) {
			if (Actors[System.GameVariables[1]].Gold >= keyrequire[1]) {
				Actors[System.GameVariables[1]].Gold -= keyrequire[1];
				++Actors[System.GameVariables[1]].Item[1];
				PlaySE(System.ShopSE);
			}
			else
				PlaySE(System.BuzzerSE);
			return;
		}
		if (Index == 2) {
			PlaySE(System.CancelSE);
			System.ShopType = 0;
			Visible = false;
			return;
		}
	}
	// 按下取消键时
	if (KeyBoard.TriggerCancel()) {
		PlaySE(System.CancelSE);
		System.ShopType = 0;
		Visible = false;
		return;
	}
}

WindowSDoorShop::WindowSDoorShop(int x, int y) {
	X = x;
	Y = y;
	Z = 0;
	Width = 224;
	Height = 256;
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Visible = false;
	Graphics.Windows.push_back(this);
}

void WindowSDoorShop::Refresh() {
	// 不可视时不执行
	if (!Visible) return;
	// 获得了随意门
	System.ItemRecord[19] = true;
	// 描绘商店头像
	string face[] = { "mting-510.png", "mting-511.png" };
	WindowBitmap(face[System.GameTime / 6 % 2], 32, 32);
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	rectangle((X + 32) * ResolutionRatio, (Y + 32) * ResolutionRatio, (X + 64) * ResolutionRatio, (Y + 64) * ResolutionRatio);
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	// 描绘文本内容
	WDrawTexts(64, 0, 112, 32, "老人", 1, 24L, true);
	WDrawTexts(64, 32, 128, 64, "价格实惠，买到就是赚到", 3, 20L);
	WDrawTexts(0, 112, Width, 32, "买随意门（30金币）", 1);
	WDrawTexts(0, 144, Width, 32, "离开", 1);
	// 描绘选择矩形
	DrawRect(48, 132 + Index * 32, 160, 24);
	// 按下上键时
	if (KeyBoard.Press(KeyBoard.KeyUP)) {
		if (Index == 1) {
			--Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// 按下下键时
	if (KeyBoard.Press(KeyBoard.KeyDOWN)) {
		if (Index == 0) {
			++Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// 按下确认键时
	if (KeyBoard.PressConfirm()) {
		if (Index == 0) {
			if (Actors[System.GameVariables[1]].Gold >= 30) {
				Actors[System.GameVariables[1]].Gold -= 30;
				++Actors[System.GameVariables[1]].Item[19];
				if (Actors[System.GameVariables[1]].Item[19] >= 16) {
					System.HitAchievement(16);
				}
				PlaySE(System.ShopSE);
			}
			else
				PlaySE(System.BuzzerSE);
			return;
		}
		if (Index == 1) {
			PlaySE(System.CancelSE);
			System.ShopType = 0;
			Visible = false;
			return;
		}
		return;
	}
	// 按下取消键时
	if (KeyBoard.TriggerCancel()) {
		PlaySE(System.CancelSE);
		System.ShopType = 0;
		Visible = false;
		return;
	}
}

WindowFloorFly::WindowFloorFly(int x, int y) {
	X = x;
	Y = y;
	Z = 0;
	Width = 192;
	Height = 288;
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Visible = false;
	update = false;
	Graphics.Windows.push_back(this);
}

void WindowFloorFly::Refresh() {
	// 不可视时不执行
	if (!Visible) return;
	// 更新内容
	if (update) {
		for (int i = 0; i < 121; ++i) {
			if (events[i][0] != nullptr) delete events[i][0];
			if (events[i][1] != nullptr) delete events[i][1];
			if (floor != nullptr) delete floor;
			events[i][0] = new IMAGE;
			events[i][1] = new IMAGE;
			floor = new IMAGE;
		}
		for (int i = 0; i < System.Map[viewmapid].MapEvents.size(); ++i) {
			loadimage(events[i][0], ("resource\\picture\\" + System.Map[viewmapid].MapEvents[i].File[0]).c_str(), 11 * ResolutionRatio, 11 * ResolutionRatio);
			loadimage(events[i][1], ("resource\\picture\\" + System.Map[viewmapid].MapEvents[i].File[1]).c_str(), 11 * ResolutionRatio, 11 * ResolutionRatio);
		}
		loadimage(floor, "resource\\picture\\Floor.png", 121 * ResolutionRatio, 121 * ResolutionRatio);
		update = false;
	}
	// 绘制小地图
	DrawSmallMap(35, 16);
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	rectangle((X + 35) * ResolutionRatio, (Y + 16) * ResolutionRatio, (X + 156) * ResolutionRatio, (Y + 137) * ResolutionRatio);
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	// 绘制文本内容
	WDrawTexts(0, 120, 160, 32, Items[4].Name, 1, 20L, true);
	string viewfloorname;
	if (System.GameVariables[3] == 2 || System.GameVariables[3] == 3) viewfloorname = "密室";
	else viewfloorname = to_string(viewfloorid) + " F";
	WDrawTexts(0, 184, 160, 20, viewfloorname, 1, 20L);
	WDrawTexts(0, 240, 160, 32, "-Quit(f)-", 0, 18L);
	WDrawTexts(0, 240, 160, 32, "-Enter-", 2, 18L);
	// 绘制按键
	if (System.GameVariables[3] == 2 || System.GameVariables[3] == 3 || System.FloorRecord[System.GameVariables[3]][0] == viewfloorid) {
		WindowBitmap("mting-1240.png", 88, 224, 16, 16, 32, 32);
		WindowBitmap("mting-1240.png", 88, 240, 16, 16, 32, 48);
	}
	else {
		WindowBitmap("mting-1240.png", 88, 224, 16, 16, 48, 32);
		WindowBitmap("mting-1240.png", 88, 240, 16, 16, 48, 48);
	}
	if (System.GameVariables[3] == 2 || System.GameVariables[3] == 3 || System.FloorRecord[System.GameVariables[3]][System.FloorRecord[System.GameVariables[3]].size() - 1] == viewfloorid) {
		WindowBitmap("mting-1240.png", 88, 184, 16, 16, 0, 32);
		WindowBitmap("mting-1240.png", 88, 168, 16, 16, 0, 48);
	}
	else {
		WindowBitmap("mting-1240.png", 88, 184, 16, 16, 16, 32);
		WindowBitmap("mting-1240.png", 88, 168, 16, 16, 16, 48);
	}
	// 按下下键时
	if (KeyBoard.Press(KeyBoard.KeyDOWN)) {
		if (System.GameVariables[3] != 2 && System.GameVariables[3] != 3 && System.FloorRecord[System.GameVariables[3]][0] != viewfloorid) {
			PlaySE(System.CursorSE);
			viewfloorid = *(find(System.FloorRecord[System.GameVariables[3]].begin(), System.FloorRecord[System.GameVariables[3]].end(), viewfloorid) - 1);
			DataMap temp;
			temp.MapName = to_string(System.GameVariables[3]) + ":" + to_string(viewfloorid);
			viewmapid = find(System.Map.begin(), System.Map.end(), temp) - System.Map.begin();
			update = true;
		}
		return;
	}
	// 按下左键时
	if (KeyBoard.Press(KeyBoard.KeyLEFT)) {
		if (System.GameVariables[3] != 2 && System.GameVariables[3] != 3 && System.FloorRecord[System.GameVariables[3]][0] != viewfloorid) {
			PlaySE(System.CursorSE);
			if (find(System.FloorRecord[System.GameVariables[3]].begin(), System.FloorRecord[System.GameVariables[3]].end(), viewfloorid) - System.FloorRecord[System.GameVariables[3]].begin() >= 5)
				viewfloorid = *(find(System.FloorRecord[System.GameVariables[3]].begin(), System.FloorRecord[System.GameVariables[3]].end(), viewfloorid) - 5);
			else
				viewfloorid = *System.FloorRecord[System.GameVariables[3]].begin();
			DataMap temp;
			temp.MapName = to_string(System.GameVariables[3]) + ":" + to_string(viewfloorid);
			viewmapid = find(System.Map.begin(), System.Map.end(), temp) - System.Map.begin();
			update = true;
		}
		return;
	}
	// 按下上键时
	if (KeyBoard.Press(KeyBoard.KeyUP)) {
		if (System.GameVariables[3] != 2 && System.GameVariables[3] != 3 && System.FloorRecord[System.GameVariables[3]][System.FloorRecord[System.GameVariables[3]].size() - 1] != viewfloorid) {
			PlaySE(System.CursorSE);
			viewfloorid = *(find(System.FloorRecord[System.GameVariables[3]].begin(), System.FloorRecord[System.GameVariables[3]].end(), viewfloorid) + 1);
			DataMap temp;
			temp.MapName = to_string(System.GameVariables[3]) + ":" + to_string(viewfloorid);
			viewmapid = find(System.Map.begin(), System.Map.end(), temp) - System.Map.begin();
			update = true;
		}
		return;
	}
	// 按下右键时
	if (KeyBoard.Press(KeyBoard.KeyRIGHT)) {
		if (System.GameVariables[3] != 2 && System.GameVariables[3] != 3 && System.FloorRecord[System.GameVariables[3]][System.FloorRecord[System.GameVariables[3]].size() - 1] != viewfloorid) {
			PlaySE(System.CursorSE);
			if (System.FloorRecord[System.GameVariables[3]].begin() + System.FloorRecord[System.GameVariables[3]].size() - 1 - find(System.FloorRecord[System.GameVariables[3]].begin(), System.FloorRecord[System.GameVariables[3]].end(), viewfloorid) >= 5)
				viewfloorid = *(find(System.FloorRecord[System.GameVariables[3]].begin(), System.FloorRecord[System.GameVariables[3]].end(), viewfloorid) + 5);
			else
				viewfloorid = *(System.FloorRecord[System.GameVariables[3]].begin() + System.FloorRecord[System.GameVariables[3]].size() - 1);
			DataMap temp;
			temp.MapName = to_string(System.GameVariables[3]) + ":" + to_string(viewfloorid);
			viewmapid = find(System.Map.begin(), System.Map.end(), temp) - System.Map.begin();
			update = true;
		}
		return;
	}
	// 按下确认键时
	if (KeyBoard.TriggerConfirm()) {
		PlaySE("mting-78");
		Visible = false;
		GameSprite stair("mting.png");
		stair.X = System.MapX;
		stair.Y = System.MapY;
		stair.Opacity = 0;
		for (int i = 0; i < 5; ++i) {
			stair.Opacity += 51;
			Graphics.Update();
		}
		Actors[System.GameVariables[1]].MapID = viewmapid;
		Player.Direction = 0;
		if (viewfloorid > System.GameVariables[2]) { // 上楼传送
			Actors[System.GameVariables[1]].X = System.Map[viewmapid].MapEvents[0].X;
			Actors[System.GameVariables[1]].Y = System.Map[viewmapid].MapEvents[0].Y;
		}
		else if (viewfloorid < System.GameVariables[2]) { // 下楼传送
			Actors[System.GameVariables[1]].X = System.Map[viewmapid].MapEvents[1].X;
			Actors[System.GameVariables[1]].Y = System.Map[viewmapid].MapEvents[1].Y;
		}
		else { // 同层传送（分地上地下）
			if (viewfloorid >= 0) {
				Actors[System.GameVariables[1]].X = System.Map[viewmapid].MapEvents[0].X;
				Actors[System.GameVariables[1]].Y = System.Map[viewmapid].MapEvents[0].Y;
			}
			else {
				Actors[System.GameVariables[1]].X = System.Map[viewmapid].MapEvents[1].X;
				Actors[System.GameVariables[1]].Y = System.Map[viewmapid].MapEvents[1].Y;
			}
		}
		System.GameVariables[2] = viewfloorid;
		for (int i = 0; i < 5; ++i)
			Graphics.Update();
		System.NeedRefresh = true;
		for (int i = 0; i < 5; ++i) {
			stair.Opacity -= 51;
			Graphics.Update();
		}
		stair.Dispose();
		return;
	}
	// 按下取消键时
	if (KeyBoard.Trigger(KeyBoard.KeyF) || KeyBoard.TriggerCancel()) {
		PlaySE(System.CancelSE);
		Visible = false;
		return;
	}
}

void WindowFloorFly::Delete() {
	for (int i = 0; i < 121; ++i) {
		delete events[i][0];
		delete events[i][1];
	}
	delete floor;
}

void WindowFloorFly::DrawSmallMap(int x, int y) {
	OpaqueImage(nullptr, X + x, Y + y, floor);
	for (int i = 0; i < System.Map[viewmapid].MapEvents.size(); ++i) {
		if (System.Map[viewmapid].MapEvents[i].Exist) {
			if (System.Map[viewmapid].MapEvents[i].TriggerCondition[0] == 1 && System.GameSwitches[System.Map[viewmapid].MapEvents[i].TriggerCondition[1]] != System.Map[viewmapid].MapEvents[i].TriggerCondition[2])
				continue;
			if (System.Map[viewmapid].MapEvents[i].TriggerCondition[0] == 2 && System.GameVariables[System.Map[viewmapid].MapEvents[i].TriggerCondition[1]] != System.Map[viewmapid].MapEvents[i].TriggerCondition[2])
				continue;
			OpaqueImage(nullptr, X + System.Map[viewmapid].MapEvents[i].X * 11 + x, Y + System.Map[viewmapid].MapEvents[i].Y * 11 + y, events[i][System.GameTime / 6 % 2]);
		}
	}
}

WindowHelp::WindowHelp(int x, int y) {
	X = x;
	Y = y;
	Z = 0;
	Page = 0;
	Width = 352;
	Height = 352;
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Visible = false;
	Graphics.Windows.push_back(this);
}

void WindowHelp::Refresh() {
	// 不可视时不执行
	if (!Visible) return;
	// 添加可用文本
	vector <string> item;
	item.push_back("enter/space=确定"); item.push_back("↑↓←→=控制角色移动/选择物品");
	item.push_back("r=回到标题界面");
	item.push_back("p=开关地图显示伤害");
	item.push_back("shift(长按)=加速进程");
	item.push_back("space=加速战斗（战斗中使用）");
	item.push_back("q=撤退（战斗中使用）");
	item.push_back("c=会心一击（战斗中使用）"); 
	item.push_back("v=呼吸（战斗中使用）");
	item.push_back("c=保存进度");
	item.push_back("v=载入进度");
	item.push_back("左ctrl×2=快速储存（时间距离最近的存档）");
	item.push_back("右ctrl×2=快速读取（时间距离最近的存档）");
	if (Actors[System.GameVariables[1]].Item[3] > 0) item.push_back("d=查看怪物能力");
	if (Actors[System.GameVariables[1]].Item[4] > 0) item.push_back("f=进行楼层传送");
	if (Actors[System.GameVariables[1]].Item[5] > 0) item.push_back("b=打开道具袋");
	if (System.GameVariables[11] > 0) item.push_back("w=查看剑技（切换需50hp）");
	if (System.GameVariables[12] > 0) item.push_back("e=查看防御术（切换需50hp）");
	if (System.GameVariables[11] > 0) item.push_back("z=使用剑技（战斗中使用）");
	if (System.GameVariables[12] > 0) item.push_back("x=使用防御术（战斗中使用）");
	if (System.GameSwitches[19] > 0) item.push_back("t=使用随意门");
	// 根据当前页绘制文本
	for (int i = 0; i < min(15, item.size() - 15 * Page); ++i)
		WDrawTexts(0, 20 * i, 320, 20, item[i + 15 * Page], 0, 20L);
	int maxpage = item.size() / 15;
	// 绘制按键
	WindowDrawArrow(232, 320, Page, maxpage, 64);
	// 按下左键时
	if (KeyBoard.Trigger(KeyBoard.KeyLEFT)) {
		if (Page == 0) return;
		--Page;
		PlaySE(System.CursorSE);
		return;
	}
	// 按下右键时
	if (KeyBoard.Trigger(KeyBoard.KeyRIGHT)) {
		if (Page == maxpage) return;
		++Page;
		PlaySE(System.CursorSE);
		return;
	}
	// 按下取消键时
	if (KeyBoard.Trigger(KeyBoard.KeyL) || KeyBoard.TriggerCancel()) {
		PlaySE(System.CancelSE);
		Visible = false;
		return;
	}
}

WindowSkill::WindowSkill(int x, int y) {
	X = x;
	Y = y;
	Z = 0;
	Type = 0;
	Index = 0;
	Width = 320;
	Height = 192;
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Visible = false;
	Graphics.Windows.push_back(this);
}

void WindowSkill::Refresh() {
	// 不可视时不执行
	if (!Visible) return;
	// 设置技能基本信息
	vector <int> list;
	int levelrequire[2][5] = { { 5, 20, 40, 60, 150 }, { 10, 30, 50, 70, 90 } };
	int breathrequire[2][5] = { { 1, 1, 2, 2, 4 }, { 1, 1, 2, 2, 4 } };
	int fatiguerequire[2][5] = { { 10, 10, 20, 20, 30 }, { 10, 10, 20, 20, 30 } };
	string desc[2][5] = { { "本次伤害*1.5，暂时将(" + to_string(Actors[System.GameVariables[1]].Level) + ")防御转为(" + to_string(Actors[System.GameVariables[1]].Level * 3 / 2) + ")点攻击，但是伤害低于(" + to_string(Actors[System.GameVariables[1]].Level) + ")时无法触发", 
		"本次伤害将附加本次战斗所受总伤害的0.4倍，暂时将(" + to_string(Actors[System.GameVariables[1]].Level) + ")攻击转为(" + to_string(Actors[System.GameVariables[1]].Level * 3 / 2) + ")点防御，但是伤害和所受伤害低于(" + to_string(Actors[System.GameVariables[1]].Level * 2) + ")时无法触发",
		"本次伤害*1.5，清空对方的气息条并让其在本次战斗内再也无法积累气息，但是伤害低于(" + to_string(Actors[System.GameVariables[1]].Level) + ")时无法触发",
		"本次伤害*1.5，汲取对方的气息，但是伤害低于(" + to_string(Actors[System.GameVariables[1]].Level) + ")时无法触发",
		"直接将对方的体力降至1点，但是伤害低于(" + to_string(Actors[System.GameVariables[1]].Level * 3) + ")时无法触发"},
		{ "本次受到伤害减少60%，并免疫造成的特殊效果", 
		"本次受到伤害减少30%，并反射所减少的伤害和特殊效果，但是反射伤害不会高于自身普攻伤害的10倍",
		"本次伤害减少40%，并增加对方35点疲劳", 
		"接下来的2次伤害减少70%", 
		"进入绝对防御状态，接下来的3次伤害会被免疫" } };
	// 根据已有技能添加进vector
	for (int i = 0; i < 5; ++i)
		if (Actors[System.GameVariables[1]].Item[6 + Type * 5 + i] > 0)
			list.push_back(i);
	// 绘制信息
	for (int i = 0; i < list.size(); ++i)
		WindowBitmap("mting-" + to_string(488 + 9 * Type + list[i]) + ".png", 16 + 40 * i, 16);
	WDrawTexts(0, 32, 288, 32, Items[6 + Type * 5 + list[Index]].Name + (System.GameVariables[13 + Type] == list[Index] + 1 ? "（已装备）" : ""));
	WDrawTexts(0, 32, 288, 32, "等级要求:" + to_string(levelrequire[Type][list[Index]]), 2);
	WDrawTexts(0, 64, 288, 32, "疲劳+" + to_string(fatiguerequire[Type][list[Index]]) + "，气息消耗：" + to_string(breathrequire[Type][list[Index]]), 2);
	WDrawTexts(0, 96, 288, 64, desc[Type][list[Index]], 3, 20L);
	// 描绘选择矩形
	DrawRect(16 + 40 * Index, 16, 32, 32);
	// 按下左键时
	if (KeyBoard.Press(KeyBoard.KeyLEFT)) {
		if (Index > 0) {
			PlaySE(System.CursorSE);
			--Index;
		}
		return;
	}
	// 按下右键时
	if (KeyBoard.Press(KeyBoard.KeyRIGHT)) {
		if (Index < list.size() - 1) {
			PlaySE(System.CursorSE);
			++Index;
		}
		return;
	}
	// 按下取消键时
	if (KeyBoard.TriggerCancel()) {
		PlaySE(System.CancelSE);
		Visible = false;
		return;
	}
	// 按下确认键时
	if (KeyBoard.TriggerConfirm()) {
		if (Actors[System.GameVariables[1]].Level >= levelrequire[Type][list[Index]] && Actors[System.GameVariables[1]].Hp > 50) {
			if (System.GameVariables[13 + Type] != list[Index] + 1) {
				PlaySE(System.DecisionSE);
				Actors[System.GameVariables[1]].Hp -= 50;
				System.GameVariables[13 + Type] = list[Index] + 1;
				System.HitAchievement(29 + Type);
			}
		}
		else
			PlaySE(System.BuzzerSE);
		return;
	}
}

WindowCure::WindowCure(int x, int y) {
	X = x;
	Y = y;
	Z = 0;
	Index = 0;
	Width = 320;
	Height = 160;
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Visible = false;
	Graphics.Windows.push_back(this);
}

void WindowCure::Refresh() {
	// 不可视时不执行
	if (!Visible) return;
	// 描绘物品信息
	for (int i = 0; i < 2; ++i) {
		if (Actors[System.GameVariables[1]].Item[17 + i] > 0) {
			WindowBitmap("mting-" + to_string(190 + i) + ".png", 16 + 40 * i, 16);
			WDrawTexts(40 * i, 16, 32, 16, to_string(Actors[System.GameVariables[1]].Item[17 + i]), 2, 16L);
		}
		else
			WindowBitmap("mting-" + to_string(190 + i) + ".png", 16 + 40 * i, 16, 190);
	}
	WDrawTexts(0, 0, 288, 32, Items[17 + Index].Name, 2);
	WDrawTexts(0, 48, 288, 64, Items[17 + Index].Description, 3, 20L);
	// 描绘选择矩形
	DrawRect(16 + 40 * Index, 16, 32, 32);
	// 按下左键时
	if (KeyBoard.Trigger(KeyBoard.KeyLEFT)) {
		if (Index == 1) {
			PlaySE(System.CursorSE);
			--Index;
		}
		return;
	}
	// 按下右键时
	if (KeyBoard.Trigger(KeyBoard.KeyRIGHT)) {
		if (Index == 0) {
			PlaySE(System.CursorSE);
			++Index;
		}
		return;
	}
	// 按下取消键时
	if (KeyBoard.TriggerCancel()) {
		PlaySE(System.CancelSE);
		Visible = false;
		return;
	}
	// 按下确认键时
	if (KeyBoard.RepeatConfirm()) {
		if (Actors[System.GameVariables[1]].Item[17 + Index] > 0) {
			PlaySE(System.DecisionSE);
			--Actors[System.GameVariables[1]].Item[17 + Index];
			if (Index == 0) {
				Actors[System.GameVariables[1]].Status[0] = false;
				Actors[System.GameVariables[1]].Hp += 180;
			}
			else {
				Actors[System.GameVariables[1]].Status[1] = false;
				System.GameVariables[4] = 0;
				Actors[System.GameVariables[1]].Hp += 380;
			}
		}
		else
			PlaySE(System.BuzzerSE);
		return;
	}
}

WindowFileImage::WindowFileImage(int x, int y) {
	X = x;
	Y = y;
	Z = 1;
	Width = 176;
	Height = 176;
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Visible = false;
	Graphics.Windows.push_back(this);
}

void WindowFileImage::Refresh() {
	// 不可视时不执行
	if (!Visible) return;
	// 文件存在时，绘制存档略缩图
	if (ExistFile("save\\save" + to_string(fileid) + ".sav")) {
		if (ExistFile("save\\save" + to_string(fileid) + ".png")) {
			IMAGE img;
			loadimage(&img, ("save\\save" + to_string(fileid) + ".png").c_str(), 640 * 144 * ResolutionRatio / 416, 480 * 144 * ResolutionRatio / 416);
			OpaqueImage(nullptr, X + 16, Y + 16, 144, 144, 192 * 144 / 416, 32 * 144 / 416, &img);
		}
		else
			WDrawTexts(0, 0, 144, 144, "No Image Here!", 1, 24L);
	}
	else
		WDrawTexts(0, 0, 144, 144, "No File Here!", 1, 24L);
}

WindowFileCursor::WindowFileCursor(int x, int y, int rectheight, vector <string> item) {
	X = x;
	Y = y;
	Z = 1;
	Width = 136;
	Height = 176;
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Active = false;
	Visible = false;
	RectHeight = rectheight;
	Items = item;
	Graphics.Windows.push_back(this);
}

void WindowFileCursor::Refresh() {
	// 不可视时不执行
	if (!Visible) return;
	// 描绘按键
	WindowDrawArrow(16, 16, Page, 199, 64, "");
	// 描绘文本内容
	for (int i = 0; i < Items.size(); ++i)
		WDrawTexts(0, 20 + 20 * i, 104, 20, "Save " + to_string(Page * 6 + i), 0, 18L);
	for (int i = 0; i < Items.size(); ++i) {
		if (i == Index) DrawRect(16, 36 + RectHeight * i, Width - 32, RectHeight);
		DrawItem(i, NormalColor());
	}
	// 窗口不活动时不执行
	if (!Active) return;
	// 按下上键时
	if (KeyBoard.Press(KeyBoard.KeyUP)) {
		if (Index > 0) {
			--Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// 按下下键时
	if (KeyBoard.Press(KeyBoard.KeyDOWN)) {
		if (Index < Items.size() - 1) {
			++Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// 按下左键时
	if (KeyBoard.Press(KeyBoard.KeyLEFT)) {
		if (Page > 0) {
			--Page;
			PlaySE(System.CursorSE);
		}
		return;
	}
	//按下右键时
	if (KeyBoard.Press(KeyBoard.KeyRIGHT)) {
		if (Page < 199) {
			++Page;
			PlaySE(System.CursorSE);
		}
		return;
	}
}

void WindowFileCursor::DrawItem(int index, COLORREF colour) {
	WDrawTexts(4, 4 + RectHeight * index, Width, RectHeight, Items[index]);
}

WindowAchievement::WindowAchievement(int x, int y) {
	X = x;
	Y = y;
	Z = 1;
	Index = 0;
	Width = 320;
	Height = 320;
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Visible = false;
	Graphics.Windows.push_back(this);
}

void WindowAchievement::Refresh() {
	// 不可视时不执行
	if (!Visible) return;
	// 描绘成就
	for (int i = 0; i < 12; ++i) {
		settextcolor(System.Achievement[Index * 12 + i] ? TextColor("yellow") : TextColor("gray"));
		WindowBitmap(System.Achievement[Index * 12 + i] ? "mting-1252.png" : "mting-1253.png", 16, 16 + 24 * i);
		WDrawTexts(32, 24 * i + 2, 256, 20, name[Index][i], 0, 20L);
		WDrawTexts(32, 24 * i + 2, 256, 20, System.Achievement[Index * 12 + i] ? description[Index][i] : "待探索……" , 2, 16L);
	}
	settextcolor(NormalColor());
}

WindowGetAchievement::WindowGetAchievement(int x, int y) {
	X = x;
	Y = y;
	Z = 2;
	Width = 160;
	Height = 32;
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Visible = false;
	Graphics.Windows.push_back(this);
}

void WindowGetAchievement::Refresh() {
	// 不可视时不执行
	if (!Visible) return;
	// 描绘内容
	WindowBitmap("mting-1252.png", 8, 4);
	WDrawTexts(16, -9, 80, 20, name, 0, 18L, true);
}

WindowStart::WindowStart(int x, int y) {
	X = x;
	Y = y;
	Z = 2;
	Width = 352;
	Height = 352;
	HasFunction = true;
	Visible = false;
	next = 0;
	Graphics.Windows.push_back(this);
}

void WindowStart::Refresh() {
	// 不可视时不执行
	if (!Visible) return;
	// 描绘内容
	string str = "";
	if (!System.Victory) {
		if (next == 0) {
			str = str + "从前，在距离王国边境数十里处，有一座废弃的高塔。\n";
			str = str + "后面我忘了。";
		}
		else if (next == 1) {
			str = str + "啊啊啊，别走，别走嘛\n";
			str = str + "这其实是一个耳熟能详的故事，勇士持着宝剑闯入高塔拯救公主的故事。\n";
			str = str + "本来这座高塔是王国曾经用来对抗魔物的军事要塞，在魔王被击毙后，剩下的怪物便夹起了尾巴，甚至不敢袭击路人，在数十年的和平之后，大家都认为剩下的怪物不足为惧，便慢慢减少了要塞的看守者。\n";
			str = str + "谁曾想贼心不死的怪物们竟然早就选出了新的魔王，并迅速组织了起来，杀死了卫兵，抢占了高塔，并以高塔为据点向王国发起攻势，并在一次突袭中成功掳掠了王国的公主。\n";
		}
		else {
			str = str + "很可笑吧，一方在数十年的戒备中慢慢松懈，另一方却卧薪尝胆，吹起了反攻的号角，不过也不奇怪，只有千日做贼，哪有千日防贼的道理？\n";
			str = str + "国王迅速组织起了以勇士为首的一支小队，要潜入魔塔救出公主\n";
			str = str + "而我们的故事，就从这里开始……";
		}
	}
	else {
		if (next == 0) {
			str = str + "故事到这里就结束了，勇士成功救出了公主，领到了赏金，在卸甲后过起了悠闲的退休生活。\n";
			str = str + "至于魔塔里面残余的怪物怎么办，现在没人去想，或许它们就此沉寂，或许许多年后又会出来祸害四方，谁知道呢，到时候只要相信后人的智慧就可以了。\n";
			str = str + "故事比较老套，也比较正常，可喜可贺，可喜可贺，正如其名——anecdote";
		}
		else {
			str = str + "恭喜过关，您的过关数据为\n";
			str = str + "等级：" + to_string(Actors[System.GameVariables[1]].Level) + "；生命：" + to_string(Actors[System.GameVariables[1]].Hp) + "；攻击：" + to_string(Actors[System.GameVariables[1]].Atk) + "；防御：" + to_string(Actors[System.GameVariables[1]].Def);
			str = str + "经验：" + to_string(Actors[System.GameVariables[1]].Exp) + "；金币：" + to_string(Actors[System.GameVariables[1]].Gold) + "。\n";
		}
	}
	WDrawTexts(0, 0, 320, 320, str, 3);
	// 按下确认键时
	if (KeyBoard.TriggerConfirm()) {
		if (!System.Victory) {
			if (next == 2) {
				System.GameSwitches[5] = true;
				Visible = false;
			}
			else
				++next;
		}
		else {
			if (next == 1) {
				System.Victory = false;
				System.Scene = new GameTitle;
			}
			else
				++next;
		}
	}
}

void GameTitle::Main() {
	// 先刷新一次
	System.NeedRefresh = true;
	// 生成标题图形
	sprite.ChangeImage(System.TitleName);
	sprite.AddIn();
	head1.ChangeImage("mting-title.png");
	head1.AddIn();
	head1.X = -160;
	head1.Opacity = 0;
	head2.ChangeImage("mting-subtitle.png");
	head2.AddIn();
	head2.X = 104;
	head2.Y = 39;
	head2.Opacity = 0;
	// 生成命令窗口
	window.X = -160;
	window.Y = 224;
	window.Z = 1;
	// 生成成就窗口
	achcursurwindow.X = 176;
	achcursurwindow.Y = 240;
	achcursurwindow.Z = 1;
	achcursurwindow.Active = false;
	achcursurwindow.Visible = false;
	// 播放标题BGM
	System.GameBGM = "sound-0.mp3";
	// 判定继续的有效性
	if (!ExistFile("save\\recent.dat")) {
		fstream file;
		file.open("save\\recent.dat", ios::ate | ios::out);
		file << 0 << endl;
		file.close();
	}
	else
		window.Index = 1;
	// 开始渐变
	Graphics.Update();
	System.DoTransition();
	// 演奏BGM
	PlayBGM(System.GameBGM);
	// 开场动画
	while (head1.X < 0) {
		head1.X += 16;
		head1.Opacity = max(head1.Opacity + 26, 255);
		Graphics.Update();
	}
	while (head2.Y < 87) {
		head2.Y += 6;
		head2.Opacity = max(head2.Opacity + 32, 255);
		Graphics.Update();
	}
	while (window.X < 8) {
		window.X += 14;
		Graphics.Update();
	}
	// 主循环
	while (System.Scene == this) {
		Update();
		Graphics.Update();
	}
	// 释放图形和窗口
	sprite.Dispose();
	head1.Dispose();
	head2.Dispose();
	window.Dispose();
	filewindow.Dispose();
	cursorwindow.Dispose();
	achcursurwindow.Dispose();
	achwindow.Dispose();
	System.ReadyTransition();
}

void GameTitle::Update() {
	// 更新存档窗口
	if (filewindow.Visible) {
		filewindow.fileid = cursorwindow.Page * 6 + cursorwindow.Index;
		// 按下取消键时
		if (KeyBoard.TriggerCancel()) {
			PlaySE(System.CancelSE);
			filewindow.Visible = false;
			cursorwindow.Visible = false;
			cursorwindow.Active = false;
			cursorwindow.Page = 0;
			cursorwindow.Index = 0;
			window.Active = true;
			return;
		}
		// 按下确认键时
		if (KeyBoard.TriggerConfirm()) {
			if (!ExistFile("save\\save" + to_string(filewindow.fileid) + ".sav")) {
				PlaySE(System.BuzzerSE);
				return;
			}
			PlaySE(System.LoadSE);
			filewindow.Visible = false;
			cursorwindow.Visible = false;
			cursorwindow.Active = false;
			cursorwindow.Page = 0;
			cursorwindow.Index = 0;
			GameMap::LoadData(filewindow.fileid);
			System.NeedRefresh = true;
			System.Scene = new GameMap;
			return;
		}
		return;
	}
	// 更新成就窗口
	if (achwindow.Visible) {
		achwindow.Index = achcursurwindow.Index;
		// 按下取消键时
		if (KeyBoard.TriggerCancel()) {
			PlaySE(System.CancelSE);
			window.Active = true;
			achcursurwindow.Active = false;
			achcursurwindow.Visible = false;
			achwindow.Visible = false;
			return;
		}
		return;
	}
	// 按下确认键时
	if (KeyBoard.TriggerConfirm()) {
		if (window.Index == 0) CommandNewGame();
		if (window.Index == 1) CommandContinue();
		if (window.Index == 2) CommandAchievement();
		if (window.Index == 3) CommandExit();
		return;
	}
}

void GameTitle::CommandNewGame() {
	PlaySE(System.DecisionSE);
	System.Reload();
	System.GameSwitches[1] = true;
	System.FloorRecord[0].push_back(0);
	System.Scene = new GameMap;
	return;
}

void GameTitle::CommandContinue() {
	PlaySE(System.DecisionSE);
	window.Active = false;
	filewindow.Visible = true;
	cursorwindow.load = true;
	cursorwindow.Visible = true;
	cursorwindow.Active = true;
	fstream file;
	file.open("save\\recent.dat", ios::in);
	char buffer[128];
	file.getline(buffer, 128);
	file.close();
	int index = atoi(buffer);
	cursorwindow.Page = index / 6;
	cursorwindow.Index = index % 6;
	return;
}

void GameTitle::CommandAchievement() {
	PlaySE(System.DecisionSE);
	window.Active = false;
	achcursurwindow.Active = true;
	achcursurwindow.Visible = true;
	achcursurwindow.Index = 0;
	achwindow.Visible = true;
	achwindow.Index = 0;
	return;
}

void GameTitle::CommandExit() {
	PlaySE(System.DecisionSE);
	System.Scene = nullptr;
	return;
}

void GameMap::Main() {
	// 立即刷新一次
	System.NeedRefresh = true;
	// 初始化地图活动块
	Graphics.MapPatterns.Initialization();
	// 设置地图
	AddRecord();
	SetUp();
	// 执行渐变
	System.DoTransition();
	// 主循环
	while (System.Scene == this) {
		Graphics.Update();
		Update();
	}
	// 释放窗口
	messagewindow.Dispose();
	monsterbookwindow.Dispose();
	encyclopediawindow.Dispose();
	battlewindow.Dispose();
	goldshopwindow.Dispose();
	expshopwindow.Dispose();
	keyshopwindow.Dispose();
	flywindow.Dispose();
	filewindow.Dispose();
	cursorwindow.Dispose();
	helpwindow.Dispose();
	skillwindow.Dispose();
	getachwindow.Dispose();
	startwindow.Dispose();
	System.ReadyTransition();
	Graphics.MapPatterns.Dispose();
}

void GameMap::Update() {
	// 有成就时
	if (!System.WaitingAchievement.empty()) {
		getachwindow.Visible = true;
		getachwindow.name = WindowAchievement::name[System.WaitingAchievement[0] / 12][System.WaitingAchievement[0] % 12];
		if (downward && getachwindow.X == 640) PlaySE("mting-99");
		if (downward) {
			getachwindow.X -= 4;
			if (getachwindow.X == 472) {
				downward = false;
				waitcount = 16;
			}
		}
		else {
			if (!(waitcount-- > 0)) {
				getachwindow.X += 8;
				if (getachwindow.X == 640) {
					getachwindow.Visible = false;
					downward = true;
					System.WaitingAchievement.erase(System.WaitingAchievement.begin());
				}
			}
		}
	}
	// 展开故事开始的窗口
	if (System.Victory) {
		if (startwindow.next == 2) startwindow.next = 0;
		startwindow.Visible = true;
		return;
	}
	if (!System.GameSwitches[5]) {
		startwindow.Visible = true;
		return;
	}
	// 血量归零，游戏结束
	if (System.EndGame) {
		System.EndGame = false;
		GameOver();
		return;
	}
	// 胜利时（这个暂时没啥用）
	if (System.Victory) return;
	// 需要设置地图时重置
	if (System.SetUp) {
		SetUp();
		System.SetUp = false;
	}
	// BGM设置
	string BGM[] = { "sound-1.mp3", "sound-2.mp3", "sound-3.mp3", "sound-4.mp3", "sound-5.mp3", "sound-6.mp3", "sound-7.mp3"};
	if (System.GameVariables[3] == 0) {
		as(System.GameVariables[2], -30, -22) System.GameBGM = BGM[6];
		else as(System.GameVariables[2], -21, -16) System.GameBGM = BGM[3];
		else as(System.GameVariables[2], -15, 0) System.GameBGM = BGM[0];
		else as(System.GameVariables[2], 1, 15) System.GameBGM = BGM[1];
		else as(System.GameVariables[2], 15, 30) System.GameBGM = BGM[2];
	}
	if (System.GameVariables[3] == 1) {
		as(System.GameVariables[2], 0, 10) System.GameBGM = BGM[0];
		else as(System.GameVariables[2], 11, 15) System.GameBGM = BGM[3];
	}
	if (System.GameVariables[3] == 2) System.GameBGM = BGM[4];
	if (System.GameVariables[3] == 3) System.GameBGM = BGM[5];
	if (record != System.GameBGM) {
		record = System.GameBGM;
		PlayBGM(System.GameBGM);
	}
	// 各窗口活动时不再执行
	if (helpwindow.Visible || goldshopwindow.Visible || expshopwindow.Visible || keyshopwindow.Visible || sdoorshopwindow.Visible || flywindow.Visible || skillwindow.Visible || curewindow.Visible)
		return;
	if (monsterbookwindow.Visible || encyclopediawindow.Visible) {
		MonsterBookUpdate();
		return;
	}
	if (filewindow.Visible) {
		FileUpdate();
		return;
	}
	if (System.ShopType != 0) {
		ShopUpdate();
		return;
	}
	if (!System.MessageContent.empty()) {
		messagewindow.Visible = true;
		MessageUpdate();
		return;
	}
	else
		messagewindow.Visible = false;
	// 执行战斗
	if (System.BattleEnemyID >= 0) BattleFunction();
	// 事件执行完毕后的事件变化
	if (System.TransformingEventName != "") {
		System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Name = System.TransformingEventName;
		System.TransRecord[Actors[System.GameVariables[1]].MapID][System.FunctioningEventID] = System.TransformingEventName;
		System.TransformingEventName = "";
		if (System.DirectlyFunction) {
			System.DirectlyFunction = false;
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Order();
		}
	}
	// 屏幕截图
	if (System.ScreenShot) {
		saveimage(System.ShotFile.c_str());
		System.ScreenShot = false;
		System.ShotFile = "";
		return;
	}
	// 执行渐变
	if (System.Transition) System.DoTransition(5);
	// 上下楼图形
	if (System.StairGraphic.Opacity > 0) {
		System.StairGraphic.Opacity -= 51;
		Graphics.Update();
		return;
	}
	// 快捷键处理
	ShortcutKey();
	// 玩家操控处理
	Player.Update();
}

void GameMap::ShortcutKey() {
	// SHIFT键加速
	System.Accelerate = KeyBoard.Press(KeyBoard.KeySHIFT);
	// 按下T键时，随意门操作
	if (KeyBoard.Trigger(KeyBoard.KeyT) && Actors[System.GameVariables[1]].Item[19] > 0 && System.GameSwitches[19]) {
		int dir[4][2] = { { 0, 1 }, { -1, 0 }, { 1, 0 }, { 0, -1 }};
		if (auto [mapev, act] = make_pair(System.Map[Actors[System.GameVariables[1]].MapID], Actors[System.GameVariables[1]]); mapev.HaveAnEvent(act.X + dir[Player.Direction][0], act.Y + dir[Player.Direction][1])) {
			auto ev = mapev.ECheckEvent(act.X + dir[Player.Direction][0], act.Y + dir[Player.Direction][1]);
			if (auto name = Split(ev->Name, "|"); name[0] == "SWALL") {
				PlaySE(System.DecisionSE);
				--Actors[System.GameVariables[1]].Item[19];
				ev->File[0] = "mting-35.png";
				ev->File[1] = "mting-35.png";
				if (name.size() == 1) {
					ev->Name = "DOOR|1";
					System.TransRecord[act.MapID][ev->ID] = "DOOR|1";
				}
				else {
					ev->Name = "SDOOR|" + name[1];
					System.TransRecord[act.MapID][ev->ID] = "SDOOR|" + name[1];
				}
				return;
			}
			else if (name[0] == "ICE") {
				PlaySE(System.DecisionSE);
				--Actors[System.GameVariables[1]].Item[19];
				ev->Exist = false;
				System.EventRecord[act.MapID].insert(ev->ID);
				return;
			}
			else {
				PlaySE(System.BuzzerSE);
				return;
			}
		}
	}
	// 按下B键时，打开道具袋
	if (KeyBoard.Trigger(KeyBoard.KeyB) && Actors[System.GameVariables[1]].Item[5] > 0) {
		curewindow.Index = 0;
		curewindow.Visible = true;
		return;
	}
	// 按下W&E键时，打开技能栏
	if (KeyBoard.Trigger(KeyBoard.KeyW) && System.GameVariables[11] > 0) {
		skillwindow.Type = 0;
		skillwindow.Index = 0;
		skillwindow.Visible = true;
		return;
	}
	if (KeyBoard.Trigger(KeyBoard.KeyE) && System.GameVariables[12] > 0) {
		skillwindow.Type = 1;
		skillwindow.Index = 0;
		skillwindow.Visible = true;
		return;
	}
	// 按下P键时，开关地图显伤
	if (KeyBoard.Trigger(KeyBoard.KeyP)) {
		System.GameSwitches[4] = !System.GameSwitches[4];
		return;
	}
	// 按下L键时，查看帮助列表
	if (KeyBoard.Trigger(KeyBoard.KeyL)) {
		helpwindow.Visible = true;
		return;
	}
	// 按下R键时，询问是否返回
	if (KeyBoard.Trigger(KeyBoard.KeyR)) {
		if (YesNoPrint("你确定要放弃当前进度，返回标题吗？") == IDYES) {
			System.Scene = new GameTitle;
		}
		return;
	}
	// 按下D键时，打开怪物手册
	if (KeyBoard.Trigger(KeyBoard.KeyD) && Actors[System.GameVariables[1]].Item[3] > 0) {
		monsterbookwindow.Index = 0;
		monsterbookwindow.Page = 0;
		monsterbookwindow.FloorEnemies.clear();
		for (auto ev : System.Map[Actors[System.GameVariables[1]].MapID].MapEvents) {
			if (Split(ev.Name, "|")[0] == "MONSTER" && ev.Exist) {
				int id = stoi(Split(ev.Name, "|")[1]);
				if (!VectorFind(monsterbookwindow.FloorEnemies, id)) monsterbookwindow.FloorEnemies.push_back(id);
			}
		}
		sort(monsterbookwindow.FloorEnemies.begin(), monsterbookwindow.FloorEnemies.end(), [](const int& a, const int& b) {
			if (get<0>(Enemies[a].GotDamage()) != get<0>(Enemies[b].GotDamage()))
				return (Enemies[a].GotDamage().first < Enemies[b].GotDamage().first);
			return (Enemies[a].Name < Enemies[b].Name);
			});
		monsterbookwindow.Visible = true;
		return;
	}
	// 按下F键时，打开楼层传送器
	if (KeyBoard.Trigger(KeyBoard.KeyF) && Actors[System.GameVariables[1]].Item[4] > 0) {
		for (auto ev : System.Map[Actors[System.GameVariables[1]].MapID].MapEvents) {
			if (ev.Exist && Split(ev.Name, "|")[0] == "MONSTER") {
				if (Enemies[stoi(Split(ev.Name, "|")[1])].GotP(27)) {
					PlaySE(System.BuzzerSE);
					return;
				}
			}
		}
		if (System.GameSwitches[3]) {
			PlaySE(System.BuzzerSE);
			return;
		}
		if (System.GameVariables[3] == 0 && (System.GameVariables[2] == 30 && !System.GameSwitches[16]) || System.GameVariables[2] == -30) {
			PlaySE(System.BuzzerSE);
			return;
		}
		flywindow.update = true;
		flywindow.viewmapid = Actors[System.GameVariables[1]].MapID;
		if (System.GameVariables[3] == 2) flywindow.viewmapid = 68;
		if (System.GameVariables[3] == 3) flywindow.viewmapid = 94;
		flywindow.viewfloorid = System.GameVariables[2];
		flywindow.Visible = true;
		return;
	}
	// 按下C&V键、双击左/右CTRL键时，存读档
	if (KeyBoard.Trigger(KeyBoard.KeyC)) {
		filewindow.Visible = true;
		cursorwindow.load = false;
		cursorwindow.Visible = true;
		cursorwindow.Active = true;
		fstream file;
		file.open("save\\recent.dat", ios::in);
		char buffer[128];
		file.getline(buffer, 128);
		file.close();
		int index = atoi(buffer);
		cursorwindow.Page = index / 6;
		cursorwindow.Index = index % 6;
		return;
	}
	if (KeyBoard.Trigger(KeyBoard.KeyV)) {
		filewindow.Visible = true;
		cursorwindow.load = true;
		cursorwindow.Visible = true;
		cursorwindow.Active = true;
		fstream file;
		file.open("save\\recent.dat", ios::in);
		char buffer[128];
		file.getline(buffer, 128);
		file.close();
		int index = atoi(buffer);
		cursorwindow.Page = index / 6;
		cursorwindow.Index = index % 6;
		return;
	}
	if (KeyBoard.DoubleClick(VK_LCONTROL)) {
		fstream file;
		file.open("save\\recent.dat", ios::in);
		char buffer[128];
		file.getline(buffer, 128);
		file.close();
		int index = atoi(buffer);
		SaveData(index);
		PlaySE(System.SaveSE);
		System.MessageName.push_back("");
		System.MessageContent.push_back("进度已保存至存档 " + to_string(index) + "！");
		System.MessageHead.push_back(0);
		return;
	}
	if (KeyBoard.DoubleClick(VK_RCONTROL)) {
		fstream file;
		file.open("save\\recent.dat", ios::in);
		char buffer[128];
		file.getline(buffer, 128);
		file.close();
		int index = atoi(buffer);
		PlaySE(System.LoadSE);
		System.ReadyTransition();
		LoadData(index);
		AddRecord();
		SetUp();
		System.NeedRefresh = true;
		System.DoTransition();
		return;
	}
}

void GameMap::MessageUpdate() {
	// 按下确认键时
	if (KeyBoard.TriggerConfirm()) {
		System.MessageContent.erase(System.MessageContent.begin());
		System.MessageName.erase(System.MessageName.begin());
		System.MessageHead.erase(System.MessageHead.begin());
		messagewindow.Opacity = 0;
		return;
	}
}

void GameMap::GameOver() {
	// 停止BGM，播放游戏结束SE
	BGMStop();
	PlaySE("mting-75");
	GameSprite gg("mting-1239.png");
	gg.X = System.MapX + 16;
	gg.Y = System.MapY + 181;
	gg.Opacity = 0;
	for (int i = 0; i < 85; ++i) {
		gg.Opacity += 3;
		gg.Y -= 1;
		Graphics.Update();
	}
	DataMap::WaitCount(120);
	gg.Dispose();
	System.Scene = new GameTitle;
}

void GameMap::MonsterBookUpdate() {
	if (encyclopediawindow.Visible) {
		if (KeyBoard.TriggerCancel()) {
			encyclopediawindow.Visible = false;
			monsterbookwindow.Visible = true;
			return;
		}
		return;
	}
	if (KeyBoard.TriggerConfirm() && !monsterbookwindow.FloorEnemies.empty()) {
		encyclopediawindow.eid = monsterbookwindow.FloorEnemies[monsterbookwindow.Page * 5 + monsterbookwindow.Index];
		encyclopediawindow.Visible = true;
		monsterbookwindow.Visible = false;
		return;
	}
}

void GameMap::FileUpdate() {
	filewindow.fileid = cursorwindow.Page * 6 + cursorwindow.Index;
	// 按下取消键时
	if (KeyBoard.TriggerCancel()) {
		PlaySE(System.CancelSE);
		filewindow.Visible = false;
		cursorwindow.Visible = false;
		cursorwindow.Active = false;
		cursorwindow.Page = 0;
		cursorwindow.Index = 0;
		return;
	}
	// 按下确认键时
	if (KeyBoard.TriggerConfirm()) {
		filewindow.Visible = false;
		cursorwindow.Visible = false;
		cursorwindow.Active = false;
		cursorwindow.Page = 0;
		cursorwindow.Index = 0;
		if (cursorwindow.load) {
			if (!ExistFile("save\\save" + to_string(filewindow.fileid) + ".sav")) {
				PlaySE(System.BuzzerSE);
				return;
			}
			PlaySE(System.LoadSE);
			System.ReadyTransition();
			LoadData(filewindow.fileid);
			AddRecord();
			SetUp();
			System.NeedRefresh = true;
			System.DoTransition();
		}
		else {
			PlaySE(System.SaveSE);
			SaveData(filewindow.fileid);
			System.MessageName.push_back("");
			System.MessageContent.push_back("进度已保存至存档 " + to_string(filewindow.fileid) + "！");
			System.MessageHead.push_back(0);
		}
		return;
	}
}

void GameMap::BattleFunction() {
	// 写入双方信息
	battlewindow.aHp = Actors[System.GameVariables[1]].Hp;
	battlewindow.aAtk = Actors[System.GameVariables[1]].Atk - System.GameVariables[4];
	battlewindow.aDef = Actors[System.GameVariables[1]].Def - System.GameVariables[4];
	battlewindow.aVaryAtk = -System.GameVariables[4];
	battlewindow.aVaryDef = -System.GameVariables[4];
	battlewindow.aFatigue = 0;
	battlewindow.aBreath = Actors[System.GameVariables[1]].Breath;
	battlewindow.eHp = Enemies[System.BattleEnemyID].Hp;
	battlewindow.eAtk = Enemies[System.BattleEnemyID].GotAtk();
	battlewindow.eDef = Enemies[System.BattleEnemyID].GotDef();
	battlewindow.eVaryAtk = Enemies[System.BattleEnemyID].GotAtk() - Enemies[System.BattleEnemyID].Atk;
	battlewindow.eVaryDef = Enemies[System.BattleEnemyID].GotDef() - Enemies[System.BattleEnemyID].Def;
	battlewindow.eFatigue = 0 + 20 * (Actors[System.GameVariables[1]].Item[24] > 0);
	if (Enemies[System.BattleEnemyID].GotP(38)) swap(battlewindow.eFatigue, battlewindow.aFatigue);
	battlewindow.eBreath = 0;
	battlewindow.LastDamage = 0;
	battlewindow.buff = 0;
	battlewindow.funckey = 0;
	battlewindow.weaken = false;
	battlewindow.eweaken = false;
	System.Accelerate = KeyBoard.Press(KeyBoard.KeySHIFT);
	battlewindow.Visible = true;
	// 记录原始状态
	bool originstatus[2] = { Actors[System.GameVariables[1]].Status[0], Actors[System.GameVariables[1]].Status[1] };
	int originweak = System.GameVariables[4];
	// 会变动能力的怪的能力记录
	if (Enemies[System.BattleEnemyID].GotP(21)) {
		int vary = Enemies[System.BattleEnemyID].GotAtk() / 2;
		battlewindow.eAtk += vary;
		battlewindow.eVaryAtk += vary;
	}
	// 战斗过程
	int result = BattleUpdate();
	// 撤退的情况，恢复原始状态
	if (result == 2) {
		Actors[System.GameVariables[1]].Status[0] = originstatus[0];
		Actors[System.GameVariables[1]].Status[1] = originstatus[1];
		System.GameVariables[4] = originweak;
	}
	// 关于疲劳的成就
	if (battlewindow.aFatigue >= 80) System.HitAchievement(10);
	if (battlewindow.eFatigue >= 80) System.HitAchievement(11);
	// 恢复加速
	if (System.Accelerate) System.Accelerate = false;
	// 战斗胜利的情况
	if (result == 0) {
		// 更新状态
		Actors[System.GameVariables[1]].Hp = battlewindow.aHp;
		Actors[System.GameVariables[1]].Breath = battlewindow.aBreath;
		// 变身怪进行变身和记录，其余的事件消失
		if (Enemies[System.BattleEnemyID].GotP(8)) {
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Name = "MONSTER|" + Split(Enemies[System.BattleEnemyID].Name, "|")[2];
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].File[0] = Enemies[stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[2])].File[0];
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].File[1] = Enemies[stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[2])].File[1];
			System.TransRecord[Actors[System.GameVariables[1]].MapID][System.FunctioningEventID] = "MONSTER|" + Split(Enemies[System.BattleEnemyID].Name, "|")[2];
		}
		else if (Enemies[System.BattleEnemyID].GotP(22)) {
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Name = "BARRIER|50";
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].File[0] = "mting-28.png";
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].File[1] = "mting-29.png";
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Through = true;
			System.TransRecord[Actors[System.GameVariables[1]].MapID][System.FunctioningEventID] = "BARRIER|50";
		}
		else if (Enemies[System.BattleEnemyID].GotP(23)) {
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Name = "ICE";
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].File[0] = "mting-245.png";
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].File[1] = "mting-245.png";
			System.TransRecord[Actors[System.GameVariables[1]].MapID][System.FunctioningEventID] = "ICE";
		}
		else if (Enemies[System.BattleEnemyID].GotP(33)) {
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Name = "ICE";
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].File[0] = "mting-245.png";
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].File[1] = "mting-245.png";
			System.TransRecord[Actors[System.GameVariables[1]].MapID][System.FunctioningEventID] = "ICE";
			int dir[8][2] = { { 1, 1 }, { 1, 0 }, { 0, 1 }, { 0, -1 }, { 1, -1 }, { -1, 1 }, { -1, 0 }, { -1, -1 } };
			for (int i = 0; i < 8; ++i) {
				if (int fx = System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].X + dir[i][0], fy = System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Y + dir[i][1], ax = Actors[System.GameVariables[1]].X, ay = Actors[System.GameVariables[1]].Y; System.Map[Actors[System.GameVariables[1]].MapID].Addable(fx, fy) && !(fx == ax && fy == ay)) {
					GameEvent temp;
					temp.ID = System.Map[Actors[System.GameVariables[1]].MapID].MapEvents.size();
					temp.Name = "ICE";
					temp.File[0] = "mting-245.png";
					temp.File[1] = "mting-245.png";
					temp.X = fx;
					temp.Y = fy;
					temp.TriggerCondition[0] = 0;
					temp.TriggerCondition[1] = 0;
					temp.TriggerCondition[2] = 0;
					temp.Exist = true;
					temp.Through = false;
					System.Map[Actors[System.GameVariables[1]].MapID].MapEvents.push_back(temp);
					System.AddRecord[Actors[System.GameVariables[1]].MapID].push(make_tuple(fx, fy, "ICE"));
				}
			}
		}
		else if (Enemies[System.BattleEnemyID].GotP(34)) {
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Name = "BARRIER|50";
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].File[0] = "mting-28.png";
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].File[1] = "mting-29.png";
			System.TransRecord[Actors[System.GameVariables[1]].MapID][System.FunctioningEventID] = "BARRIER|50";
			int dir[8][2] = { { 1, 1 }, { 1, 0 }, { 0, 1 }, { 0, -1 }, { 1, -1 }, { -1, 1 }, { -1, 0 }, { -1, -1 } };
			for (int i = 0; i < 8; ++i) {
				if (int fx = System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].X + dir[i][0], fy = System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Y + dir[i][1], ax = Actors[System.GameVariables[1]].X, ay = Actors[System.GameVariables[1]].Y; System.Map[Actors[System.GameVariables[1]].MapID].Addable(fx, fy) && !(fx == ax && fy == ay)) {
					GameEvent temp;
					temp.ID = System.Map[Actors[System.GameVariables[1]].MapID].MapEvents.size();
					temp.Name = "BARRIER|50";
					temp.File[0] = "mting-28.png";
					temp.File[1] = "mting-29.png";
					temp.X = fx;
					temp.Y = fy;
					temp.TriggerCondition[0] = 0;
					temp.TriggerCondition[1] = 0;
					temp.TriggerCondition[2] = 0;
					temp.Exist = true;
					temp.Through = true;
					System.Map[Actors[System.GameVariables[1]].MapID].MapEvents.push_back(temp);
					System.AddRecord[Actors[System.GameVariables[1]].MapID].push(make_tuple(fx, fy, "BARRIER|50"));
				}
			}
		}
		else {
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Exist = false;
			System.EventRecord[Actors[System.GameVariables[1]].MapID].insert(System.FunctioningEventID);
		}
		// 首战告捷成就
		System.HitAchievement(0);
		// 增加已击杀敌人
		++System.GameVariables[5];
		// 杀敌成就
		if (System.GameVariables[5] >= 200) System.HitAchievement(1);
		// 状态成就
		if (Actors[System.GameVariables[1]].Status[0] || Actors[System.GameVariables[1]].Status[1]) System.HitAchievement(2);
		if (Actors[System.GameVariables[1]].Hp <= 200) System.HitAchievement(3);
		if (Actors[System.GameVariables[1]].Breath == Actors[System.GameVariables[1]].BreathLimit - 1) System.HitAchievement(9);
		if (Actors[System.GameVariables[1]].Exp >= 300) System.HitAchievement(18);
		if (Actors[System.GameVariables[1]].Gold >= 1000) System.HitAchievement(19);
	}
	// 战斗失败
	else if (result == 1) {
		GameOver();
		return;
	}
	System.BattleEnemyID = -1;
	battlewindow.Visible = false;
	// 如果有开关变量变动则
	vector <string> temp = Split(System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Name, "|");
	if (result == 0 && temp.size() == 5) {
		if (stoi(temp[2]) == 1) System.GameSwitches[stoi(temp[3])] = stoi(temp[4]);
		if (stoi(temp[2]) == 2) System.GameVariables[stoi(temp[3])] += stoi(temp[4]);
	}
}

int GameMap::BattleUpdate() {
	// 等待2帧开打
	DataMap::WaitCount(2);
	int singlebreath = Actors[System.GameVariables[1]].BreathLimit / 6;
	int conatk; // 连击
	int animationid; // 动画编号
	int reflexedam = 2 * battlewindow.aAtk + Actors[System.GameVariables[1]].GotThreshold() > battlewindow.eDef ? (battlewindow.aAtk > battlewindow.eDef ? battlewindow.aAtk - battlewindow.eDef : 1) : 0;// 反射伤害
	int abdefense = 0, reflexdefense = 0; // 盾4/5剩余回合数
	bool turn = true; // 轮次
	bool firstround = true; // 是否是第一轮攻击，因为仅有第一轮可以附加状态
	bool lockbr = false; // 锁住气息
	int shieldballres = 2;

	// 先攻
	if (Enemies[System.BattleEnemyID].GotP(5)) turn = false;
	// 循环战斗
	while (true) {
		// 我方轮次，记录连击数
		conatk = max(Enemies[System.BattleEnemyID].Conatk,1);
		if (turn) {
			// 协同攻击
			bool synergy = false;
			// 撤退
			if (battlewindow.funckey == 1) return 2;
			// 我方伤害
			int edam = battlewindow.aAtk + Actors[System.GameVariables[1]].GotThreshold() > battlewindow.eDef ? (battlewindow.aAtk > battlewindow.eDef ? battlewindow.aAtk - battlewindow.eDef : 1) : 0;
			// 弱化状态时伤害减半
			if (battlewindow.weaken) edam /= 2;
			// 反射盾伤害上限
			reflexedam = edam * 10;
			// 设置动画编号
			animationid = 2;
			// 伤害为0的情况
			if (edam == 0) {
				battlewindow.funckey = 0;
				animationid = 9;
			}
			else {
				// 计算疲劳值和减伤
				int fat = min(battlewindow.aFatigue / 10, 10);
				edam = max(edam - edam * fat / 10, 0);
				// 减伤后伤害为0的情况
				if (edam == 0) {
					battlewindow.funckey = 0;
					animationid = 8;
				}
				else {
					// 会心一击
					if (battlewindow.funckey == 2) {
						battlewindow.funckey = 0;
						if (battlewindow.aBreath >= singlebreath && battlewindow.aFatigue <= 50) {
							battlewindow.aBreath -= singlebreath;
							battlewindow.aFatigue += 5 / (Actors[System.GameVariables[1]].Item[21] > 0 ? 2 : 1);
							++animationid;
							edam *= 2 * (Actors[System.GameVariables[1]].Item[21] > 0 ? 1.25 : 1);
							if (Enemies[System.BattleEnemyID].GotP(16)) edam *= 0.75;
							synergy = true;
						}
					}
					// 剑技1
					if (battlewindow.funckey == 3) {
						battlewindow.funckey = 0;
						if (edam >= Actors[System.GameVariables[1]].Level && battlewindow.aDef >= Actors[System.GameVariables[1]].Level && battlewindow.aBreath >= singlebreath && battlewindow.aAtk > battlewindow.eDef && battlewindow.aFatigue <= 50) {
							battlewindow.aBreath -= singlebreath;
							battlewindow.aFatigue += 10 / (Actors[System.GameVariables[1]].Item[21] > 0 ? 2 : 1);
							animationid = 25;
							edam *= 1.5 * (Actors[System.GameVariables[1]].Item[21] > 0 ? 1.25 : 1);
							if (Actors[System.GameVariables[1]].Item[23] > 0) {
								Graphics.Damages.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, true, to_string((int)(edam * 0.6)), GameWindow::TextColor("green") });
								battlewindow.aHp += edam * 0.4;
							}
							battlewindow.aAtk += Actors[System.GameVariables[1]].Level * 1.5;
							battlewindow.aDef -= Actors[System.GameVariables[1]].Level;
							battlewindow.aVaryAtk += Actors[System.GameVariables[1]].Level * 1.5;
							battlewindow.aVaryDef -= Actors[System.GameVariables[1]].Level;
							synergy = true;
						}
					}
					// 剑技2
					if (battlewindow.funckey == 4) {
						battlewindow.funckey = 0;
						if (edam >= 2 * Actors[System.GameVariables[1]].Level && battlewindow.LastDamage >= 2 * Actors[System.GameVariables[1]].Level && battlewindow.aBreath >= singlebreath && battlewindow.aAtk > battlewindow.eDef && battlewindow.aFatigue <= 50) {
							battlewindow.aBreath -= singlebreath;
							battlewindow.aFatigue += 10 / (Actors[System.GameVariables[1]].Item[21] > 0 ? 2 : 1);
							animationid = 27;
							edam += battlewindow.LastDamage * 2 / 5;
							edam *= Actors[System.GameVariables[1]].Item[21] > 0 ? 1.25 : 1;
							edam = min(edam, battlewindow.eHp);
							if (Actors[System.GameVariables[1]].Item[23] > 0) {
								Graphics.Damages.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, true, to_string((int)(edam * 0.6)), GameWindow::TextColor("green") });
								battlewindow.aHp += edam * 0.4;
							}
							battlewindow.LastDamage = 0;
							battlewindow.aDef += Actors[System.GameVariables[1]].Level * 1.5;
							battlewindow.aAtk -= Actors[System.GameVariables[1]].Level;
							battlewindow.aVaryDef += Actors[System.GameVariables[1]].Level * 1.5;
							battlewindow.aVaryAtk -= Actors[System.GameVariables[1]].Level;
							synergy = true;
						}
					}
					// 剑技3
					if (battlewindow.funckey == 5) {
						battlewindow.funckey = 0;
						if (edam >= Actors[System.GameVariables[1]].Level && battlewindow.aBreath >= 2 * singlebreath && battlewindow.aAtk > battlewindow.eDef && battlewindow.aFatigue <= 50) {
							battlewindow.aBreath -= 2 * singlebreath;
							battlewindow.aFatigue += 20 / (Actors[System.GameVariables[1]].Item[21] > 0 ? 2 : 1);
							animationid = 47;
							edam *= 1.5 * (Actors[System.GameVariables[1]].Item[21] > 0 ? 1.25 : 1);
							if (Actors[System.GameVariables[1]].Item[23] > 0) {
								Graphics.Damages.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, true, to_string((int)(edam * 0.6)), GameWindow::TextColor("green") });
								battlewindow.aHp += edam * 0.4;
							}
							battlewindow.eBreath = 0;
							lockbr = true;
							synergy = true;
						}
					}
					// 剑技4
					if (battlewindow.funckey == 6) {
						battlewindow.funckey = 0;
						if (edam >= Actors[System.GameVariables[1]].Level && battlewindow.aBreath >= 2 * singlebreath && battlewindow.aAtk > battlewindow.eDef && battlewindow.aFatigue <= 50) {
							battlewindow.aBreath -= 2 * singlebreath;
							battlewindow.aFatigue += 20 / (Actors[System.GameVariables[1]].Item[21] > 0 ? 2 : 1);
							animationid = 72;
							edam *= 1.5 * (Actors[System.GameVariables[1]].Item[21] > 0 ? 1.25 : 1);
							if (Actors[System.GameVariables[1]].Item[23] > 0) {
								Graphics.Damages.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, true, to_string((int)(edam * 0.6)), GameWindow::TextColor("green") });
								battlewindow.aHp += edam * 0.4;
							}
							battlewindow.aBreath = min(Actors[System.GameVariables[1]].BreathLimit - 1, battlewindow.aBreath + battlewindow.eBreath);
							battlewindow.eBreath = 0;
							synergy = true;
						}
					}
					// 剑技5
					if (battlewindow.funckey == 7) {
						battlewindow.funckey = 0;
						if (edam >= Actors[System.GameVariables[1]].Level * 3 && battlewindow.aBreath >= 4 * singlebreath && battlewindow.aAtk > battlewindow.eDef && battlewindow.aFatigue <= 50) {
							battlewindow.aBreath -= 4 * singlebreath;
							battlewindow.aFatigue += 30 / (Actors[System.GameVariables[1]].Item[21] > 0 ? 2 : 1);
							animationid = 74;
							edam = battlewindow.eHp - 1;
							if (Actors[System.GameVariables[1]].Item[23] > 0) {
								Graphics.Damages.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, true, to_string((int)(edam * 0.6)), GameWindow::TextColor("green") });
								battlewindow.aHp += edam * 0.4;
							}
							synergy = true;
						}
					}
					else // 我方气息增加
						battlewindow.aBreath = min(edam > 0 ? (battlewindow.aBreath + Round(1.0 * battlewindow.eDef / battlewindow.aAtk * 6)) : battlewindow.aBreath, Actors[System.GameVariables[1]].BreathLimit - 1);
				}
			}
			// 在不锁气息的情况下敌方气息增加
			if (!lockbr && animationid != 72)
				battlewindow.eBreath = min(edam > 0 ? (battlewindow.eBreath + Round(edam / 3.0)) : battlewindow.eBreath, Enemies[System.BattleEnemyID].Breath);
			// 扣血、动画和结算
			battlewindow.eHp = max(battlewindow.eHp - edam, 0);
			Graphics.Animations.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, Animation[animationid] });
			Graphics.Damages.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, false, to_string(edam), GameWindow::TextColor("red") });
			// 格挡
			if (synergy && Enemies[System.BattleEnemyID].GotP(37)) {
				battlewindow.aHp = max(0, battlewindow.aHp - edam);
				Actors[System.GameVariables[1]].Status[1] = true;
				Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 64, battlewindow.Y + 32, 0, Animation[14] });
				System.GameVariables[4] += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
			}
			if (synergy && Actors[System.GameVariables[1]].Item[26] > 0) {
				DataMap::WaitCount(max(0, Animation[animationid].Pattern.size() - 2));
				int tempedam = 0.2 * (battlewindow.aAtk + Actors[System.GameVariables[1]].GotThreshold() > battlewindow.eDef ? (battlewindow.aAtk > battlewindow.eDef ? battlewindow.aAtk - battlewindow.eDef : 1) : 0);
				battlewindow.eHp = max(battlewindow.eHp - tempedam, 0);
				Graphics.Animations.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, Animation[25] });
				Graphics.Damages.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, false, to_string(tempedam), GameWindow::TextColor("red") });
				DataMap::WaitCount(Animation[25].Pattern.size());
			}
			else
				DataMap::WaitCount(Animation[animationid].Pattern.size());
			if (!System.Accelerate) DataMap::WaitCount(3);
			if (battlewindow.aHp <= 0) {
				DataMap::WaitCount(3);
				return 1;
			}
			if (battlewindow.eHp <= 0) {
				Actors[System.GameVariables[1]].Exp += Enemies[System.BattleEnemyID].Exp;
				Actors[System.GameVariables[1]].Gold += Enemies[System.BattleEnemyID].Gold;
				PlaySE("mting-69");
				return 0;
			}
		}
		else {
			// 相连刀
			bool succession = false;
			while (conatk--) {
				// 清空气息和反击
				bool brclear = false, beatback = false;
				// 撤退的情况
				if (battlewindow.funckey == 1) return 2;
				// 敌方伤害
				int adam = battlewindow.eAtk + Actors[System.GameVariables[1]].GotThreshold() * 2 > (Enemies[System.BattleEnemyID].GotP(6) ? 0 : battlewindow.aDef) ? (battlewindow.eAtk > (Enemies[System.BattleEnemyID].GotP(6) ? 0 : battlewindow.aDef) ? battlewindow.eAtk - (Enemies[System.BattleEnemyID].GotP(6) ? 0 : battlewindow.aDef) : 1) : 0, recover;
				// 弱化情况下伤害减半
				if (battlewindow.eweaken) adam /= 2;
				// 剑至尊情况下有气息则伤害1.5倍
				if (Enemies[System.BattleEnemyID].GotP(35) && battlewindow.aBreath >= singlebreath) adam *= 1.5;
				// 动画编号
				animationid = Enemies[System.BattleEnemyID].AnimationID;
				// 伤害为0的情况
				if (adam == 0) {
					if (battlewindow.funckey >= 8) battlewindow.funckey = 0;
					animationid = 9;
				}
				else {
					// 计算疲劳值的减伤
					int fat = min(battlewindow.eFatigue / 10, 10);
					int recover = 0;
					bool immune = false, reflex = false;
					adam = max(adam - adam * fat / 10, 0);
					// 吸血
					if (Enemies[System.BattleEnemyID].GotP(3)) recover = adam * stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]) / 100;
					// 减伤后伤害为0的情况
					if (adam == 0) {
						if (battlewindow.funckey >= 8) battlewindow.funckey = 0;
						animationid = 8;
					}
					else {
						// 盾技1
						if (battlewindow.funckey == 8) {
							battlewindow.funckey = 0;
							if (battlewindow.aBreath >= singlebreath && battlewindow.aFatigue <= 50 && shieldballres > 0) {
								battlewindow.aBreath -= singlebreath;
								if (Actors[System.GameVariables[1]].Item[22] > 0) --shieldballres;
								battlewindow.aFatigue += 10;
								immune = true;
							}
						}
						// 盾技2
						if (battlewindow.funckey == 9) {
							battlewindow.funckey = 0;
							if (battlewindow.aBreath >= singlebreath && battlewindow.aFatigue <= 50 && shieldballres > 0) {
								battlewindow.aBreath -= singlebreath;
								if (Actors[System.GameVariables[1]].Item[22] > 0) --shieldballres;
								battlewindow.aFatigue += 10;
								reflex = true;
							}
						}
						// 盾技4
						if (battlewindow.funckey == 11) {
							battlewindow.funckey = 0;
							if (battlewindow.aBreath >= (2 - (Actors[System.GameVariables[1]].Item[22] > 0)) * singlebreath && abdefense == 0 && reflexdefense == 0 && battlewindow.aFatigue <= 50 && shieldballres > 0) {
								battlewindow.aBreath -= (2 - (Actors[System.GameVariables[1]].Item[22] > 0)) * singlebreath;
								if (Actors[System.GameVariables[1]].Item[22] > 0) --shieldballres;
								battlewindow.aFatigue += 20;
								abdefense = 2;
							}
						}
						// 盾技5
						if (battlewindow.funckey == 12) {
							battlewindow.funckey = 0;
							if (battlewindow.aBreath >= (4 - (Actors[System.GameVariables[1]].Item[22] > 0)) * singlebreath && abdefense == 0 && reflexdefense == 0 && battlewindow.aFatigue <= 50 && shieldballres > 0) {
								battlewindow.aBreath -= (4 - (Actors[System.GameVariables[1]].Item[22] > 0)) * singlebreath;
								if (Actors[System.GameVariables[1]].Item[22] > 0) --shieldballres;
								battlewindow.aFatigue += 30;
								reflexdefense = 3;
							}
						}
						// 盾技1，免疫
						if (immune) {
							Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, Animation[26] });
							if (Actors[System.GameVariables[1]].Item[23] > 0) {
								Graphics.Damages.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, true, to_string((int)(0.8 * adam * 3 / 5)), GameWindow::TextColor("green") });
								battlewindow.aHp += 0.8 * adam * 3 / 5;
							}
							adam -= adam * 3 / 5;
							recover -= recover * 3 / 5;
							beatback = true;
						}
						// 盾技2，反射
						int reflexdam = min(adam * 3 / 10, reflexedam);
						if (reflex) {
							Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, Animation[28] });
							adam -= reflexdam;
							recover -= recover * 3 / 10;
							if (Actors[System.GameVariables[1]].Item[23] > 0) {
								Graphics.Damages.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, true, to_string((int)(0.8 * reflexdam)), GameWindow::TextColor("green") });
								battlewindow.aHp += 0.8 * reflexdam;
							}
							battlewindow.eHp = max(battlewindow.eHp - reflexdam, 0);
							Graphics.Damages.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, false, to_string(reflexdam), GameWindow::TextColor("red") });
							beatback = true;
						}
						// 盾技3
						if (battlewindow.funckey == 10) {
							battlewindow.funckey = 0;
							if (battlewindow.aBreath >= (2 - (Actors[System.GameVariables[1]].Item[22] > 0)) * singlebreath && battlewindow.aFatigue <= 50 && shieldballres > 0) {
								battlewindow.aBreath -= (2 - (Actors[System.GameVariables[1]].Item[22] > 0)) * singlebreath;
								Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, Animation[48] });
								if (Actors[System.GameVariables[1]].Item[23] > 0) {
									Graphics.Damages.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, true, to_string((int)(0.8 * adam * 2 / 5)), GameWindow::TextColor("green") });
									battlewindow.aHp += 0.8 * adam * 2 / 5;
								}
								adam -= adam * 2 / 5;
								recover -= recover * 2 / 5;
								if (Actors[System.GameVariables[1]].Item[22] > 0) --shieldballres;
								battlewindow.aFatigue += 20;
								// 魔王
								if (Enemies[System.BattleEnemyID].GotP(38)) battlewindow.aFatigue += 35;
								else battlewindow.eFatigue += 35;
								beatback = true;
							}
						}
						// 盾技4
						if (abdefense > 0) {
							--abdefense;
							Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, Animation[73] });
							if (Actors[System.GameVariables[1]].Item[23] > 0) {
								Graphics.Damages.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, true, to_string((int)(0.8 * adam)), GameWindow::TextColor("green") });
								battlewindow.aHp += 0.8 * adam * 0.7;
							}
							adam *= 0.3;
							recover *= 0.3;
							beatback = true;
						}
						// 盾技5
						if (reflexdefense > 0) {
							--reflexdefense;
							Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, Animation[73] });
							if (Actors[System.GameVariables[1]].Item[23] > 0) {
								Graphics.Damages.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, true, to_string((int)(0.8 * adam)), GameWindow::TextColor("green") });
								battlewindow.aHp += 0.8 * adam;
							}
							adam = 0;
							recover = 0;
							beatback = true;
						}
						// 噬元
						if (Enemies[System.BattleEnemyID].GotP(32)) {
							if (!immune && !reflex && battlewindow.aBreath >= Actors[System.GameVariables[1]].BreathLimit / 6) {
								battlewindow.eBreath = Enemies[System.BattleEnemyID].Breath;
								battlewindow.aBreath -= Actors[System.GameVariables[1]].BreathLimit / 6;
								brclear = true;
								Graphics.Animations.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, Animation[24] });
								DataMap::WaitCount(2);
							}
						}
						// 会心一击
						if (battlewindow.eBreath >= Enemies[System.BattleEnemyID].Breath || succession) {
							// 肥硕
							if (Enemies[System.BattleEnemyID].GotP(16)) battlewindow.eBreath = Enemies[System.BattleEnemyID].Breath / 2;
							else battlewindow.eBreath = 0;
							if (!succession) {
								if (Enemies[System.BattleEnemyID].GotP(38)) battlewindow.aFatigue += 5;
								else battlewindow.eFatigue += 5;
							}
							++animationid;
							// 必杀剑
							if (Enemies[System.BattleEnemyID].GotP(13)) {
								adam *= 3;
								recover *= 3;
							}
							// 暴躁
							else if (Enemies[System.BattleEnemyID].GotP(18)) {
								adam *= 5;
								recover *= 5;
								int loss = battlewindow.eHp / 2;
								battlewindow.eHp -= loss;
								Graphics.Damages.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, false, to_string(loss), GameWindow::TextColor("red") });
							}
							// 剑至尊
							else if (Enemies[System.BattleEnemyID].GotP(35)) {
								adam *= 4;
								recover *= 4;
							}
							else {
								adam *= 2;
								recover *= 2;
							}
							// 磐石
							if (Enemies[System.BattleEnemyID].GotP(10)) {
								battlewindow.eAtk += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
								battlewindow.eDef += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
								battlewindow.eVaryAtk += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
								battlewindow.eVaryDef += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
								Graphics.Animations.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, Animation[15] });
							}
							// 水秘术
							if (Enemies[System.BattleEnemyID].GotP(11)) {
								battlewindow.eHp += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
								Graphics.Animations.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, Animation[24] });
							}
							// 怨咒
							if (Enemies[System.BattleEnemyID].GotP(12)) {
								if (reflex) {
									battlewindow.eDef -= stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
									battlewindow.eVaryDef -= stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
									Graphics.Animations.push({ battlewindow.X + 32, battlewindow.Y + 32, 0, Animation[14] });
								}
								else if (!immune) {
									battlewindow.aDef -= stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
									battlewindow.aVaryDef -= stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
									Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 72, battlewindow.Y + 32, 0, Animation[14] });
								}
							}
							// 嗜血
							if (Enemies[System.BattleEnemyID].GotP(14)) {
								battlewindow.eAtk += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[2]);
								battlewindow.eVaryAtk += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[2]);
								recover *= 2;
								Graphics.Animations.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, Animation[15] });
							}
							// 雷秘术&圣火
							if (Enemies[System.BattleEnemyID].GotP(15) || Enemies[System.BattleEnemyID].GotP(26)) {
								if (!reflex && !immune) {
									battlewindow.aBreath -= battlewindow.aBreath % (Actors[System.GameVariables[1]].BreathLimit / 6);
									Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 64, battlewindow.Y + 32, 0, Animation[14] });
									brclear = true;
								}
							}
							// 灼烧&圣火
							if (Enemies[System.BattleEnemyID].GotP(17) || Enemies[System.BattleEnemyID].GotP(26)) {
								if (reflex) {
									battlewindow.eweaken = true;
									Graphics.Animations.push({ battlewindow.X + 32, battlewindow.Y + 32, 0, Animation[14] });
								}
								else if (!immune) {
									battlewindow.weaken = true;
									Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 64, battlewindow.Y + 32, 0, Animation[14] });
								}
							}
							// 黑魔法
							if (Enemies[System.BattleEnemyID].GotP(24))
								if (!reflex && !immune)
									++battlewindow.buff;
							// 天雷
							if (Enemies[System.BattleEnemyID].GotP(25)) {
								if (!reflex && !immune) {
									battlewindow.aBreath = max(battlewindow.aBreath - Actors[System.GameVariables[1]].BreathLimit / 6, 0);
									Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 64, battlewindow.Y + 32, 0, Animation[14] });
									brclear = true;
								}
							}
							// 相连刀
							if (Enemies[System.BattleEnemyID].GotP(28)) succession = true;
							// 戏法
							if (Enemies[System.BattleEnemyID].GotP(29)) {
								if (reflex) {
									int var = battlewindow.eAtk - battlewindow.eDef;
									battlewindow.eAtk -= var;
									battlewindow.eVaryAtk -= var;
									battlewindow.eDef += var;
									battlewindow.eVaryDef += var;
									Graphics.Animations.push({ battlewindow.X + 32, battlewindow.Y + 32, 0, Animation[14] });
								}
								else if (!immune) {
									int var = battlewindow.aAtk - battlewindow.aDef;
									battlewindow.aAtk -= var;
									battlewindow.aVaryAtk -= var;
									battlewindow.aDef += var;
									battlewindow.aVaryDef += var;
									Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 72, battlewindow.Y + 32, 0, Animation[14] });
								}
							}
							// 暗算
							if (Enemies[System.BattleEnemyID].GotP(36)) {
								if (!immune) {
									if (reflex) {
										Graphics.Animations.push({ battlewindow.X + 32, battlewindow.Y + 32, 0, Animation[14] });
										battlewindow.eAtk -= stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
										battlewindow.eDef -= stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
										battlewindow.eVaryAtk -= stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
										battlewindow.eVaryDef -= stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
									}
									else {
										Actors[System.GameVariables[1]].Status[1] = true;
										Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 64, battlewindow.Y + 32, 0, Animation[14] });
										System.GameVariables[4] += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
									}
								}
							}
						}
						else {
							// 暗算
							if (Enemies[System.BattleEnemyID].GotP(36)) {
								if (!Actors[System.GameVariables[1]].Status[0] && !reflex && !immune) {
									Actors[System.GameVariables[1]].Status[0] = true;
									Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 64, battlewindow.Y + 32, 0, Animation[14] });
									System.GameVariables[6] = Player.Step;
								}
							}
							// 锁气息时不增加
							if (!lockbr)
								battlewindow.eBreath = min(adam > 0 ? (battlewindow.eBreath + Round(1.0 * battlewindow.aDef / battlewindow.aAtk * 6)) : battlewindow.eBreath, Enemies[System.BattleEnemyID].Breath);
						}
						// 中毒
						if (Enemies[System.BattleEnemyID].GotP(1) && firstround) {
							if (!Actors[System.GameVariables[1]].Status[0] && !reflex && !immune) {
								Actors[System.GameVariables[1]].Status[0] = true;
								Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 64, battlewindow.Y + 32, 0, Animation[14] });
								System.GameVariables[6] = Player.Step;
							}
						}
						// 衰弱
						if (Enemies[System.BattleEnemyID].GotP(2) && firstround) {
							if (!immune) {
								if (reflex) {
									Graphics.Animations.push({ battlewindow.X + 32, battlewindow.Y + 32, 0, Animation[14] });
									battlewindow.eAtk -= stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
									battlewindow.eDef -= stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
									battlewindow.eVaryAtk -= stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
									battlewindow.eVaryDef -= stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
								}
								else {
									Actors[System.GameVariables[1]].Status[1] = true;
									Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 64, battlewindow.Y + 32, 0, Animation[14] });
									System.GameVariables[4] += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
								}
							}
						}
						// 粘液拳
						if (Enemies[System.BattleEnemyID].GotP(9)) {
							if (reflex) battlewindow.eFatigue += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
							else if (!immune) battlewindow.aFatigue += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
						}
						// 黑魔法
						if (Enemies[System.BattleEnemyID].GotP(24)) {
							if (!reflex && !immune) ++battlewindow.buff;
							if (battlewindow.buff >= 7) {
								battlewindow.buff = 0;
								adam *= 7;
								recover *= 7;
								Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, Animation[29] });
							}
						}
						// 吸血
						if (Enemies[System.BattleEnemyID].GotP(3)) {
							battlewindow.eHp += recover;
							if (adam > 0) Graphics.Damages.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, true, to_string(recover), GameWindow::TextColor("green") });
						}
					}
				}
				// 暴怒
				if (Enemies[System.BattleEnemyID].GotP(31)) {
					battlewindow.eVaryAtk += battlewindow.eAtk;
					battlewindow.eAtk *= 2;
				}
				// 正常情况下才增加我方气息
				if (!brclear && !Enemies[System.BattleEnemyID].GotP(24))
					battlewindow.aBreath = min(adam > 0 ? (battlewindow.aBreath + Round(adam / 10.0)) : battlewindow.aBreath, Actors[System.GameVariables[1]].BreathLimit - 1);
				// 扣血、动画和结算
				battlewindow.aHp = max(battlewindow.aHp - adam, 0);
				battlewindow.LastDamage += adam;
				Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, Animation[animationid] });
				Graphics.Damages.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, false, to_string(adam), GameWindow::TextColor("red") });
				if (beatback && Actors[System.GameVariables[1]].Item[25] > 0) {
					DataMap::WaitCount(max(0, Animation[Enemies[System.BattleEnemyID].AnimationID].Pattern.size() - 2));
					int tempedam = 0.3 * (battlewindow.aAtk + Actors[System.GameVariables[1]].GotThreshold() > battlewindow.eDef ? (battlewindow.aAtk > battlewindow.eDef ? battlewindow.aAtk - battlewindow.eDef : 1) : 0);
					battlewindow.eHp = max(battlewindow.eHp - tempedam, 0);
					Graphics.Animations.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, Animation[25] });
					Graphics.Damages.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, false, to_string(tempedam), GameWindow::TextColor("red") });
					DataMap::WaitCount(Animation[25].Pattern.size());
				}
				else
					DataMap::WaitCount(Animation[Enemies[System.BattleEnemyID].AnimationID].Pattern.size());
				if (!System.Accelerate) DataMap::WaitCount(3);
				if (battlewindow.aHp <= 0) {
					DataMap::WaitCount(3);
					return 1;
				}
				if (battlewindow.eHp <= 0) {
					Actors[System.GameVariables[1]].Exp += Enemies[System.BattleEnemyID].Exp;
					Actors[System.GameVariables[1]].Gold += Enemies[System.BattleEnemyID].Gold;
					PlaySE("mting-69");
					return 0;
				}
			}
			firstround = false;
		}
		turn = !turn;
	}
}

void GameMap::ShopUpdate() {
	if (System.ShopType == 1) {
		goldshopwindow.Index = 0;
		goldshopwindow.ShopID = stoi(Split(System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Name, "|")[2]);
		goldshopwindow.Visible = true;
		System.ShopType = 0;
		return;
	}
	if (System.ShopType == 2) {
		expshopwindow.Index = 0;
		expshopwindow.ShopID = stoi(Split(System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Name, "|")[2]);
		expshopwindow.Visible = true;
		System.ShopType = 0;
		return;
	}
	if (System.ShopType == 3) {
		keyshopwindow.Index = 0;
		keyshopwindow.Visible = true;
		System.ShopType = 0;
		return;
	}
	if (System.ShopType == 4) {
		sdoorshopwindow.Index = 0;
		sdoorshopwindow.Visible = true;
		System.ShopType = 0;
		return;
	}
}

void GameMap::LoadData(int fileid) {
	System.Reload();
	string filename = "save\\save" + to_string(fileid) + ".sav";
	ifstream inFile(filename, ios::in);
	char buffer[400005];
	// 读取楼层数据
	memset(buffer, 0, sizeof(buffer));
	inFile.getline(buffer, 400000);
	string datastr = buffer;
	vector <string> gamedata = Split(datastr, ",");
	int cnt = 1;
	for (int i = 0; i < stoi(gamedata[0]); ++i) {
		int target = stoi(gamedata[cnt]);
		int siz = stoi(gamedata[cnt + 1]);
		for (int j = 0; j < siz; ++j)
			System.FloorRecord[target].push_back(stoi(gamedata[cnt + 2 + j]));
		cnt = cnt + 2 + siz;
	}
	datastr.clear();
	gamedata.clear();
	// 读取开关数据
	memset(buffer, 0, sizeof(buffer));
	inFile.getline(buffer, 400000);
	datastr = buffer;
	gamedata = Split(datastr, ",");
	for (int i = 0; i < 5000; ++i)
		System.GameSwitches[i] = stoi(gamedata[i]);
	datastr.clear();
	gamedata.clear();
	// 读取变量数据
	memset(buffer, 0, sizeof(buffer));
	inFile.getline(buffer, 400000);
	datastr = buffer;
	gamedata = Split(datastr, ",");
	for (int i = 0; i < 5000; ++i)
		System.GameVariables[i] = stoi(gamedata[i]);
	datastr.clear();
	gamedata.clear();
	// 读取物品数据
	memset(buffer, 0, sizeof(buffer));
	inFile.getline(buffer, 400000);
	datastr = buffer;
	gamedata = Split(datastr, ",");
	for (int i = 0; i < 99; ++i)
		System.ItemRecord[i] = stoi(gamedata[i]);
	datastr.clear();
	gamedata.clear();
	// 读取宝石数据
	memset(buffer, 0, sizeof(buffer));
	inFile.getline(buffer, 400000);
	datastr = buffer;
	gamedata = Split(datastr, ",");
	for (int i = 0; i < 99; ++i)
		for (int j = 0; j < 99; ++j)
			System.GemRecord[i][j] = stoi(gamedata[i * 99 + j]);
	datastr.clear();
	gamedata.clear();
	// 读取血瓶数据
	memset(buffer, 0, sizeof(buffer));
	inFile.getline(buffer, 400000);
	datastr = buffer;
	gamedata = Split(datastr, ",");
	for (int i = 0; i < 999; ++i)
		System.BottleRecord[i] = stoi(gamedata[i]);
	datastr.clear();
	gamedata.clear();
	// 读取事件数据
	memset(buffer, 0, sizeof(buffer));
	inFile.getline(buffer, 400000);
	datastr = buffer;
	gamedata = Split(datastr, ",");
	cnt = 1;
	for (int i = 0; i < stoi(gamedata[0]); ++i) {
		int target = stoi(gamedata[cnt]);
		int siz = stoi(gamedata[cnt + 1]);
		for (int j = 0; j < siz; ++j)
			System.EventRecord[target].insert(stoi(gamedata[cnt + 2 + j]));
		cnt = cnt + 2 + siz;
	}
	datastr.clear();
	gamedata.clear();
	// 读取变换数据
	memset(buffer, 0, sizeof(buffer));
	inFile.getline(buffer, 400000);
	datastr = buffer;
	gamedata = Split(datastr, ",");
	cnt = 1;
	for (int i = 0; i < stoi(gamedata[0]); ++i) {
		int target = stoi(gamedata[cnt]);
		int siz = stoi(gamedata[cnt + 1]);
		for (int j = 0; j < siz; ++j)
			System.TransRecord[target][stoi(gamedata[cnt + 2 + 2 * j])] = gamedata[cnt + 2 + 2 * j + 1];
		cnt = cnt + 2 + siz * 2;
	}
	datastr.clear();
	gamedata.clear();
	// 读取增添数据
	memset(buffer, 0, sizeof(buffer));
	inFile.getline(buffer, 400000);
	datastr = buffer;
	gamedata = Split(datastr, ",");
	cnt = 1;
	for (int i = 0; i < stoi(gamedata[0]); ++i) {
		int target = stoi(gamedata[cnt]);
		int siz = stoi(gamedata[cnt + 1]);
		for (int j = 0; j < siz; ++j)
			System.AddRecord[target].push(make_tuple(stoi(gamedata[cnt + 2 + 3 * j]), stoi(gamedata[cnt + 2 + 3 * j + 1]), gamedata[cnt + 2 + 3 * j + 2]));
		cnt = cnt + 2 + siz * 3;
	}
	datastr.clear();
	gamedata.clear();
	// 读取主角数据
	memset(buffer, 0, sizeof(buffer));
	inFile.getline(buffer, 400000);
	datastr = buffer;
	gamedata = Split(datastr, ",");
	cnt = 1;
	for (int i = 0; i < stoi(gamedata[0]); ++i) {
		Actors[i].Name = gamedata[1 + 114 * i];
		Actors[i].File = gamedata[1 + 114 * i + 1];
		Actors[i].Status[0] = stoi(gamedata[1 + 114 * i + 2]);
		Actors[i].Status[1] = stoi(gamedata[1 + 114 * i + 3]);
		Actors[i].Level = stoi(gamedata[1 + 114 * i + 4]);
		Actors[i].Hp = stoi(gamedata[1 + 114 * i + 5]);
		Actors[i].Atk = stoi(gamedata[1 + 114 * i + 6]);
		Actors[i].Def = stoi(gamedata[1 + 114 * i + 7]);
		Actors[i].Mdef = stoi(gamedata[1 + 114 * i + 8]);
		Actors[i].Breath = stoi(gamedata[1 + 114 * i + 9]);
		Actors[i].BreathLimit = stoi(gamedata[1 + 114 * i + 10]);
		Actors[i].Exp = stoi(gamedata[1 + 114 * i + 11]);
		Actors[i].Gold = stoi(gamedata[1 + 114 * i + 12]);
		Actors[i].MapID = stoi(gamedata[1 + 114 * i + 13]);
		Actors[i].X = stoi(gamedata[1 + 114 * i + 14]);
		Actors[i].Y = stoi(gamedata[1 + 114 * i + 15]);
		for (int j = 0; j < 99; ++j)
			Actors[i].Item[j] = stoi(gamedata[1 + 114 * i + 16 + j]);
	}
	datastr.clear();
	gamedata.clear();

	inFile.close();
}

void GameMap::SaveData(int fileid) {
	// lambda表达式
	constexpr auto writeint = [](string dst, int src) { return dst + to_string(src) + ","; };
	constexpr auto writefint = [](string dst, int siz, int src[]) {
		for (int i = 0; i < siz; ++i)
			dst = dst + to_string(src[i]) + ",";
		return dst;
	};
	constexpr auto writefbool = [](string dst, int siz, bool src[]) {
		for (int i = 0; i < siz; ++i)
			dst = dst + to_string(src[i]) + ",";
		return dst;
	};
	constexpr auto writestr = [](string dst, string src) { return dst + src + ","; };
	// 存入成就
	fstream file;
	file.open("save\\achievement.dat", ios::ate | ios::out);
	string achstr = to_string(System.Achievement[0]);
	for (int i = 1; i < 36; ++i)
		achstr = achstr + "," + to_string(System.Achievement[i]);
	file << achstr << endl;
	file.close();
	// 存入游戏数据
	string filename = "save\\save" + to_string(fileid) + ".sav";
	ofstream outFile(filename, ios::out | ios::binary);
	string gamedata = "";
	// 写入楼层数据
	gamedata = writeint(gamedata, System.FloorRecord.size());
	for (auto m : System.FloorRecord) {
		gamedata = writeint(gamedata, m.first);
		gamedata = writeint(gamedata, m.second.size());
		for (auto f : m.second)
			gamedata = writeint(gamedata, f);
	}
	gamedata.pop_back();
	outFile << gamedata << endl;
	gamedata = "";
	// 写入开关数据
	gamedata = writefbool(gamedata, 5000, System.GameSwitches);
	gamedata.pop_back();
	outFile << gamedata << endl;
	gamedata = "";
	// 写入变量数据
	gamedata = writefint(gamedata, 5000, System.GameVariables);
	gamedata.pop_back();
	outFile << gamedata << endl;
	gamedata = "";
	// 写入物品数据
	gamedata = writefbool(gamedata, 99, System.ItemRecord);
	gamedata.pop_back();
	outFile << gamedata << endl;
	gamedata = "";
	// 写入宝石数据
	for (int i = 0; i < 99; ++i)
		for (int j = 0; j < 99; ++j)
			gamedata = writeint(gamedata, System.GemRecord[i][j]);
	gamedata.pop_back();
	outFile << gamedata << endl;
	gamedata = "";
	// 写入血瓶数据
	gamedata = writefbool(gamedata, 999, System.BottleRecord);
	gamedata.pop_back();
	outFile << gamedata << endl;
	gamedata = "";
	// 写入事件数据
	gamedata = writeint(gamedata, System.EventRecord.size());
	for (auto mp : System.EventRecord) {
		gamedata = writeint(gamedata, mp.first);
		gamedata = writeint(gamedata, mp.second.size());
		for (auto ev : mp.second)
			gamedata = writeint(gamedata, ev);
	}
	gamedata.pop_back();
	outFile << gamedata << endl;
	gamedata = "";
	// 写入变换数据
	gamedata = writeint(gamedata, System.TransRecord.size());
	for (auto mp : System.TransRecord) {
		gamedata = writeint(gamedata, mp.first);
		gamedata = writeint(gamedata, mp.second.size());
		for (auto ev : mp.second) {
			gamedata = writeint(gamedata, ev.first);
			gamedata = writestr(gamedata, ev.second);
		}
	}
	gamedata.pop_back();
	outFile << gamedata << endl;
	gamedata = "";
	// 写入增添数据
	gamedata = writeint(gamedata, System.AddRecord.size());
	for (auto mp : System.AddRecord) {
		gamedata = writeint(gamedata, mp.first);
		gamedata = writeint(gamedata, mp.second.size());
		auto Q = mp.second;
		while (!Q.empty()) {
			auto [evx, evy, evstr] = Q.front();
			Q.pop();
			gamedata = writeint(gamedata, evx);
			gamedata = writeint(gamedata, evy);
			gamedata = writestr(gamedata, evstr);
		}
	}
	gamedata.pop_back();
	outFile << gamedata << endl;
	gamedata = "";
	// 写入主角数据
	gamedata = writeint(gamedata, Actors.size());
	for (auto act : Actors) {
		gamedata = writestr(gamedata, act.Name);
		gamedata = writestr(gamedata, act.File);
		gamedata = writefbool(gamedata, 2, act.Status);
		gamedata = writeint(gamedata, act.Level);
		gamedata = writeint(gamedata, act.Hp);
		gamedata = writeint(gamedata, act.Atk);
		gamedata = writeint(gamedata, act.Def);
		gamedata = writeint(gamedata, act.Mdef);
		gamedata = writeint(gamedata, act.Breath);
		gamedata = writeint(gamedata, act.BreathLimit);
		gamedata = writeint(gamedata, act.Exp);
		gamedata = writeint(gamedata, act.Gold);
		gamedata = writeint(gamedata, act.MapID);
		gamedata = writeint(gamedata, act.X);
		gamedata = writeint(gamedata, act.Y);
		for (int i = 0; i < 99; ++i)
			gamedata = writeint(gamedata, act.Item[i]);
	}
	gamedata.pop_back();
	outFile << gamedata << endl;
	gamedata = "";
	outFile.close();
	file.open("save\\recent.dat", ios::ate | ios::out);
	file << fileid << endl;
	file.close();
	System.ScreenShot = true;
	System.ShotFile = "save\\save" + to_string(fileid) + ".png";
}

void GameMap::SetUp() {
	// 按地图变更记录事件
	for (auto rc : System.TransRecord) {
		for (auto c : rc.second) {
			System.Map[rc.first].MapEvents[c.first].Name = c.second;
			auto name = Split(c.second, "|");
			// 变身怪
			if (name[0] == "MONSTER") {
				System.Map[rc.first].MapEvents[c.first].File[0] = Enemies[stoi(name[1])].File[0];
				System.Map[rc.first].MapEvents[c.first].File[1] = Enemies[stoi(name[1])].File[1];
			}
			// 随意门
			if (System.Map[rc.first].MapEvents[c.first].Name == "DOOR|1" || Split(System.Map[rc.first].MapEvents[c.first].Name, "|")[0] == "SDOOR") {
				System.Map[rc.first].MapEvents[c.first].File[0] = "mting-35.png";
				System.Map[rc.first].MapEvents[c.first].File[1] = "mting-35.png";
			}
			// 变熔岩的怪
			if (System.Map[rc.first].MapEvents[c.first].Name == "BARRIER|50") {
				System.Map[rc.first].MapEvents[c.first].File[0] = "mting-28.png";
				System.Map[rc.first].MapEvents[c.first].File[1] = "mting-29.png";
				System.Map[rc.first].MapEvents[c.first].Through = true;
			}
			// 变冰块的怪
			if (System.Map[rc.first].MapEvents[c.first].Name == "ICE") {
				System.Map[rc.first].MapEvents[c.first].File[0] = "mting-245.png";
				System.Map[rc.first].MapEvents[c.first].File[1] = "mting-245.png";
			}
			// 变传送门的
			if (Split(System.Map[rc.first].MapEvents[c.first].Name, "|")[0] == "TRANS") {
				System.Map[rc.first].MapEvents[c.first].File[0] = "mting-243.png";
				System.Map[rc.first].MapEvents[c.first].File[1] = "mting-244.png";
				System.Map[rc.first].MapEvents[c.first].Through = true;
			}
		}
	}
	// 消除记录事件
	for (auto rc : System.EventRecord)
		for (auto c : rc.second) {
			if (c >= System.Map[rc.first].MapEvents.size()) break;
			System.Map[rc.first].MapEvents[c].Exist = false;
		}
	System.NeedRefresh = true;
}

void GameMap::AddRecord() {
	// 按地图变更增加事件
	for (auto rc : System.AddRecord) {
		auto Q = rc.second;
		while (!Q.empty()) {
			auto [evx, evy, evstr] = Q.front();
			Q.pop();
			GameEvent temp;
			temp.ID = System.Map[rc.first].MapEvents.size();
			temp.Name = evstr;
			temp.X = evx;
			temp.Y = evy;
			temp.TriggerCondition[0] = 0;
			temp.TriggerCondition[1] = 0;
			temp.TriggerCondition[2] = 0;
			temp.Exist = true;
			// 变冰的
			if (temp.Name == "ICE") {
				temp.File[0] = "mting-245.png";
				temp.File[1] = "mting-245.png";
				temp.Through = false;
			}
			// 变熔岩的
			if (temp.Name == "BARRIER|50") {
				temp.File[0] = "mting-28.png";
				temp.File[1] = "mting-29.png";
				temp.Through = true;
			}
			System.Map[rc.first].MapEvents.push_back(temp);
		}
	}
}
