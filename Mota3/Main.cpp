#include "Main.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// ��������
	System.BasicSet();
	int Width = WindowWidth * ResolutionRatio, Height = WindowHeight * ResolutionRatio;
	initgraph(Width, Height);
	System.Initialization();
	// ���ھ���
	int scrWidth = GetSystemMetrics(SM_CXSCREEN), scrHeight = GetSystemMetrics(SM_CYSCREEN);
	RECT rect;
	GetWindowRect(GameActive, &rect);
	SetWindowPos(GameActive, HWND_TOP, (scrWidth - Width) / 2, (scrHeight - Height) / 2, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
	// ׼�����
	BeginBatchDraw();
	// ���ɳ������󣨱��⻭�棩
	System.Scene = new GameTitle;
	// ׼������
	System.ReadyTransition();
	// Scene��Чʱһֱִ��Main����
	while (System.Scene != nullptr)
		System.Scene->Main();
	// ִ�н���
	System.DoTransition();
	// �ͷ��ڴ�
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
	// �����Ӻ���Ϣ����Ϊ��ʱ��ִ��
	if (System.MessageName.empty()) return;
	if (!Visible) return;
	// ��������ʽ
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	// д�����������ı�����
	string name = *System.MessageName.begin();
	string content = *System.MessageContent.begin();
	int head = *System.MessageHead.begin();
	if (name != "") { // ��������Ϊ��ʱ
		X = System.MapX + 16;
		Y = System.MapY + 160;
		Width = 320;
		Height = 160;
		// ΪNPCʱ�����Ի�������
		if (head >= 0) Y = System.MapY + 32;
		// ��������ͷ�������
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
	// �����ı�����
	if (Opacity > 0) WDrawTexts(0, 32 * (name != "") - 8 * (name == ""), Width - 32, 96, content, 3 + (name == ""), 20L);
	// ��ԭ����ʽ
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
	// ������ʱ��ִ��
	if (!Visible) return;
	// ����ȡ����ʱ
	if (KeyBoard.TriggerCancel()) {
		PlaySE(System.CancelSE);
		Visible = false;
		return;
	}
	// �޵���ʱ��ִ��
	if (FloorEnemies.empty()) return;
	// ����ҳ��
	MaxPage = FloorEnemies.size() / 5;
	if (FloorEnemies.size() / 5.0 != MaxPage) ++MaxPage;
	string ele1 = "", ele2 = "";
	// ����������
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
			ele1 = "��ϸ��";
			ele2 = "";
		}
		WDrawTexts(32, 64 * i, 96, 16, ele1, 0, 18L);
		WDrawTexts(32, 64 * i + 16, 96, 16, ele2, 0, 18L);
		WDrawTexts(80, 64 * i, 96, 32, "������" + to_string(Enemies[FloorEnemies[Page * 5 + i]].Hp), 0, 20L);
		WDrawTexts(160, 64 * i, 96, 32, "������" + to_string(Enemies[FloorEnemies[Page * 5 + i]].GotAtk()), 0, 20L);
		WDrawTexts(240, 64 * i, 96, 32, "������" + to_string(Enemies[FloorEnemies[Page * 5 + i]].GotDef()), 0, 20L);
		WDrawTexts(80, 20 + 64 * i, 96, 32, "���飺" + to_string(Enemies[FloorEnemies[Page * 5 + i]].Exp), 0, 20L);
		WDrawTexts(160, 20 + 64 * i, 96, 32, "��ң�" + to_string(Enemies[FloorEnemies[Page * 5 + i]].Gold), 0, 20L);
		WDrawTexts(240, 20 + 64 * i, 96, 32, "��Ϣ��" + to_string(Enemies[FloorEnemies[Page * 5 + i]].Breath), 0, 20L);
		WDrawTexts(240, 40 + 64 * i, 96, 32, "�˺���" + Enemies[FloorEnemies[Page * 5 + i]].GotDamage().second, 0, 20L);
	}
	// ����ѡ�����
	DrawRect(16, 16 + 64 * Index, Width - 32, 64);
	// ��ǰҳ��/��ҳ��
	WindowDrawArrow(232, 336, Page, MaxPage - 1, 64);
	settextcolor(WHITE);
	// �������ʱ
	if (KeyBoard.Press(KeyBoard.KeyLEFT)) {
		if (Page > 0) {
			--Page;
			Index = 0;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// �����Ҽ�ʱ
	if (KeyBoard.Press(KeyBoard.KeyRIGHT)) {
		if (Page < MaxPage - 1) {
			++Page;
			Index = 0;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// �����ϼ�ʱ
	if (KeyBoard.Press(KeyBoard.KeyUP)) {
		if (Index > 0) {
			--Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// �����¼�ʱ
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
	// ������ʱ��ִ��
	if (!Visible) return;
	// ����������
	WindowBitmap(Enemies[eid].File[System.GameTime / 6 % 2], 128, 16);
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	rectangle((X + 128) * ResolutionRatio, (Y + 16) * ResolutionRatio, (X + 160) * ResolutionRatio, (Y + 48) * ResolutionRatio);
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	WDrawTexts(160, 6, 160, 20, Split(Enemies[eid].Name, "|")[0], 0, 22L, true);
	WDrawTexts(0, 40, 128, 20, "����:" + to_string(Enemies[eid].Hp), 0, 20L);
	WDrawTexts(85, 40, 128, 20, "����:" + to_string(Enemies[eid].GotAtk()), 0, 20L);
	WDrawTexts(170, 40, 128, 20, "����:" + to_string(Enemies[eid].GotDef()), 0, 20L);
	WDrawTexts(255, 40, 128, 20, "��Ϣ:" + to_string(Enemies[eid].Breath), 0, 20L);
	WDrawTexts(0, 60, 128, 20, "����:" + to_string(Enemies[eid].Exp), 0, 20L);
	WDrawTexts(85, 60, 128, 20, "���:" + to_string(Enemies[eid].Gold), 0, 20L);
	WDrawTexts(170, 60, 128, 20, "�ٽ�:" + (Enemies[eid].GotCrisis() >= 0 ? to_string(Enemies[eid].GotCrisis()) : "��"), 0, 20L);
	WDrawTexts(255, 60, 128, 20, "�˺�:" + Enemies[eid].GotDamage().second, 0, 20L);
	WDrawTexts(0, 80, 128, 20, "�ٽ��˺�:" + (Enemies[eid].GotCrisis() >= 0 ? Enemies[eid].GotDamage(Enemies[eid].GotCrisis()).second : "��"), 0, 20L);
	WDrawTexts(0, 120, 128, 20, "����:", 0, 20L);
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
	// ������ʱ��ִ��
	if (!Visible || System.BattleEnemyID < 0) return;
	// ��������ʽ
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	// ���ƾ��ο�
	rectangle((X + 32) * ResolutionRatio, (Y + 32) * ResolutionRatio, (X + 64) * ResolutionRatio, (Y + 64) * ResolutionRatio);
	rectangle((X + Width - 64) * ResolutionRatio, (Y + 32) * ResolutionRatio, (X + Width - 32) * ResolutionRatio, (Y + 64) * ResolutionRatio);
	// ��ԭ����ʽ
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	// ���˫��ͷ��
	WindowBitmap(Enemies[System.BattleEnemyID].File[System.GameTime / 6 % 2], 32, 32);
	WindowBitmap(Actors[System.GameVariables[1]].File, Width - 64, 32, 32, 32, 0, 0);
	// ���buff��debuff
	as(buff, 1, 6) BitmapCentre("mting-" + to_string(1291 + buff) + ".png", X + Width - 32, Y + 32);
	if (eweaken) BitmapCentre("mting-710.png", X + 32, Y + 20, 200);
	if (weaken) BitmapCentre("mting-710.png", X + Width - 64, Y + 20, 200);
	// ����funckey�䶯��ʾ
	if (funckey == 2) BitmapCentre("mting-1227.png", X + Width - 24, Y + 128);
	else BitmapCentre("mting-1241.png", X + Width - 24, Y + 128);
	as (funckey, 3, 7) BitmapCentre("mting-1227.png", X + Width - 48, Y + 128);
	else BitmapCentre("mting-1215.png", X + Width - 48, Y + 128);
	as (funckey, 8, 12) BitmapCentre("mting-1227.png", X + Width - 72, Y + 128);
	else BitmapCentre("mting-1214.png", X + Width - 72, Y + 128);
	// ���˫������
	WDrawTexts(0, -8, 128, 24, Split(Enemies[System.BattleEnemyID].Name, "|")[0], 1, 22L, true);
	WDrawTexts(72, 16, 64, 20, "����:", 0, 20L);
	WDrawTexts(72, 40, 64, 20, "������:", 0, 20L);
	WDrawTexts(72, 64, 64, 20, "������:", 0, 20L);
	WDrawTexts(72, 88, 64, 20, "ƣ��:", 0, 20L);
	WDrawTexts(0, 136, 64, 20, "��Ϣ:", 0, 20L);
	WDrawTexts(Width - 112, -8, 96, 24, Actors[System.GameVariables[1]].Name, 1, 22L, true);
	WDrawTexts(Width - 168, 16, 64, 24, ":����", 2, 20L);
	WDrawTexts(Width - 168, 40, 64, 24, ":������", 2, 20L);
	WDrawTexts(Width - 168, 64, 64, 24, ":������", 2, 20L);
	WDrawTexts(Width - 168, 88, 64, 24, ":ƣ��", 2, 20L);
	WDrawTexts(Width - 168, 136, 64, 24, "��Ϣ:", 2, 20L);
	if (System.GameVariables[13] == 2) WDrawTexts(Width - 168, 156, 96, 24, ":�ۼ��˺�", 2, 20L);
	// �����Ա仯��ʾ
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
	// ���˰���
	settextcolor(GameWindow::TextColor("yellow"));
	WDrawTexts(-8, 176, 128, 24, "������(Q)��", 0, 22L, true);
	settextcolor(WHITE);
	// ���˫����ֵ
	WDrawTexts(120, 16, 64, 20, to_string(eHp), 0, 20L);
	WDrawTexts(120, 40, 64, 20, to_string(eAtk), 0, 20L);
	WDrawTexts(120, 64, 64, 20, to_string(eDef), 0, 20L);
	WDrawTexts(120, 88, 64, 20, to_string(eFatigue), 0, 20L);
	WDrawTexts(Width - 216, 16, 64, 24, to_string(aHp), 2, 20L);
	WDrawTexts(Width - 216, 40, 64, 24, to_string(aAtk), 2, 20L);
	WDrawTexts(Width - 216, 64, 64, 24, to_string(aDef), 2, 20L);
	WDrawTexts(Width - 216, 88, 64, 24, to_string(aFatigue), 2, 20L);
	if (System.GameVariables[13] == 2) WDrawTexts(Width - 216, 156, 64, 24, to_string(LastDamage), 2, 20L);
	// �����Ϣ��
	IMAGE* rect = new IMAGE, * rect2 = new IMAGE;
	loadimage(rect, "resource\\picture\\mting-1226.png", 80 * ResolutionRatio, 16 * ResolutionRatio);
	loadimage(rect2, "resource\\picture\\mting-1226.png", 80 * ResolutionRatio,8 * ResolutionRatio);
	OpaqueImage(nullptr, X + 56, Y + 160, 80, 8, 0, 0, rect);
	OpaqueImage(nullptr, X + 56, Y + 160, eBreath * 80 / Enemies[System.BattleEnemyID].Breath, 8, 0, 8, rect);
	OpaqueImage(nullptr, X + Width - 88, Y + 160, 80, 4, 0, 0, rect2);
	OpaqueImage(nullptr, X + Width - 88, Y + 160, aBreath % (Actors[System.GameVariables[1]].BreathLimit / 6) * 80 / (Actors[System.GameVariables[1]].BreathLimit / 6), 4, 0, 4, rect2);
	// �����Ϣ����
	string breathfile1[] = { "mting-1211.png", "mting-1212.png", "mting-1213.png" };
	string breathfile2[] = { "mting-1211_g.png", "mting-1212_g.png", "mting-1213_g.png" };
	for (int i = 0; i < 5; ++i)
		WindowBitmap(aBreath >= Actors[System.GameVariables[1]].BreathLimit / 6 * (i + 1) ? "mting-1211.png" : "mting-1212.png", Width - 88 + 14 * i, 145);
	delete rect;
	delete rect2;
	// ����ʱ������ڼ���״̬��ȡ��
	if (aHp <= 0) {
		if (System.Accelerate) System.Accelerate = false;
		funckey = 0;
		return;
	}
	// ����V��ʱ�������
	if (KeyBoard.Trigger(KeyBoard.KeyV) && aBreath >= Actors[System.GameVariables[1]].BreathLimit / 6) {
		aBreath -= Actors[System.GameVariables[1]].BreathLimit / 6;
		aFatigue = max(0, aFatigue - ((int)(Actors[System.GameVariables[1]].Level / 10) + 1) * 10);
		return;
	}
	// ����Q��ʱ����
	if (KeyBoard.Press(KeyBoard.KeyQ)) {
		funckey = 1;
		return;
	}
	// ����C��ʱ����һ��
	if (KeyBoard.Trigger(KeyBoard.KeyC) && funckey == 0) {
		funckey = 2;
		return;
	}
	// ����Z��ʱʹ�ý���
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
	// ����X��ʱʹ�÷�����
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
	// ����SPACE��ʱ����ս��
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
	// ������ʱ��ִ��
	if (!Visible) return;
	// ���ó�ʼ�۸�
	if (System.GameVariables[8] == 0) System.GameVariables[8] = 20;
	if (System.GameVariables[9] == 0) System.GameVariables[9] = 50;
	// ����̵�ͷ��
	string face[] = { "mting-495.png", "mting-496.png" };
	WindowBitmap(face[System.GameTime / 6 % 2], 32, 32);
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	rectangle((X + 32) * ResolutionRatio, (Y + 32) * ResolutionRatio, (X + 64) * ResolutionRatio, (Y + 64) * ResolutionRatio);
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	// ����ı�����
	WDrawTexts(64, 0, 112, 32, "̰��֮��", 1, 24L, true);
	WDrawTexts(64, 32, 128, 64, "������ܸ���" + to_string(System.GameVariables[8 + ShopID - 1]) + "��ң��ұ���������һ��Ҫ��", 3, 20L);
	WDrawTexts(0, 112, Width, 32, "����+" + to_string(750 * ShopID - 100), 1);
	WDrawTexts(0, 144, Width, 32, "����+" + to_string(3 * ShopID), 1);
	WDrawTexts(0, 176, Width, 32, "����+" + to_string(3 * ShopID), 1);
	WDrawTexts(0, 208, Width, 32, "�뿪", 1);
	// ���ѡ�����
	DrawRect(64, 132 + Index * 32, 128, 24);
	// �����ϼ�ʱ
	if (KeyBoard.Press(KeyBoard.KeyUP)) {
		if (Index > 0) {
			--Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// �����¼�ʱ
	if (KeyBoard.Press(KeyBoard.KeyDOWN)) {
		if (Index < 3) {
			++Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// ����ȷ�ϼ�ʱ
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
	// ����ȡ����ʱ
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
	// ������ʱ��ִ��
	if (!Visible) return;
	// ���ó�ʼ�۸�
	int exprequire[2][3] = { { 90, 40, 40 },{ 240, 100, 100 } }, lvadd[2] = { 1,3 }, atkadd[2] = { 1,3 }, defadd[2] = { 2,5 };
	// ����̵�ͷ��
	string face[] = { "mting-508.png", "mting-509.png" };
	WindowBitmap(face[System.GameTime / 6 % 2], 32, 32);
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	rectangle((X + 32) * ResolutionRatio, (Y + 32) * ResolutionRatio, (X + 64) * ResolutionRatio, (Y + 64) * ResolutionRatio);
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	// ����ı�����
	WDrawTexts(64, 0, 112, 32, "ս��֮��", 1, 24L, true);
	WDrawTexts(64, 32, 128, 64, "�ҿ��Խ���ľ���ֵת��Ϊ���ʵ��������", 3, 20L);
	WDrawTexts(0, 112, Width, 32, "��" + to_string(lvadd[ShopID - 1]) + "�ȼ���" + to_string(exprequire[ShopID - 1][0]) + "exp��", 1);
	WDrawTexts(0, 144, Width, 32, "����+" + to_string(atkadd[ShopID - 1]) + "��" + to_string(exprequire[ShopID - 1][1]) + "exp��", 1);
	WDrawTexts(0, 176, Width, 32, "����+" + to_string(defadd[ShopID - 1]) + "��" + to_string(exprequire[ShopID - 1][2]) + "exp��", 1);
	WDrawTexts(0, 208, Width, 32, "�뿪", 1);
	// ���ѡ�����
	DrawRect(64, 132 + Index * 32, 128, 24);
	// �����ϼ�ʱ
	if (KeyBoard.Press(KeyBoard.KeyUP)) {
		if (Index > 0) {
			--Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// �����¼�ʱ
	if (KeyBoard.Press(KeyBoard.KeyDOWN)) {
		if (Index < 3) {
			++Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// ����ȷ�ϼ�ʱ
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
	// ����ȡ����ʱ
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
	// ������ʱ��ִ��
	if (!Visible) return;
	// ���ó�ʼ�۸�
	int keyrequire[] = { 20,80 };
	// ����̵�ͷ��
	string face[] = { "mting-504.png", "mting-505.png" };
	WindowBitmap(face[System.GameTime / 6 % 2], 32, 32);
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	rectangle((X + 32) * ResolutionRatio, (Y + 32) * ResolutionRatio, (X + 64) * ResolutionRatio, (Y + 64) * ResolutionRatio);
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	// ����ı�����
	WDrawTexts(64, 0, 112, 32, "����", 1, 24L, true);
	WDrawTexts(64, 32, 128, 64, "��С�磬������Կ����", 3, 20L);
	WDrawTexts(0, 112, Width, 32, "��Կ�ף�" + to_string(keyrequire[0]) + "��ң�", 1);
	WDrawTexts(0, 144, Width, 32, "��Կ�ף�" + to_string(keyrequire[1]) + "��ң�", 1);
	WDrawTexts(0, 176, Width, 32, "�뿪", 1);
	// ���ѡ�����
	DrawRect(64, 132 + Index * 32, 128, 24);
	// �����ϼ�ʱ
	if (KeyBoard.Press(KeyBoard.KeyUP)) {
		if (Index > 0) {
			--Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// �����¼�ʱ
	if (KeyBoard.Press(KeyBoard.KeyDOWN)) {
		if (Index < 2) {
			++Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// ����ȷ�ϼ�ʱ
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
	// ����ȡ����ʱ
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
	// ������ʱ��ִ��
	if (!Visible) return;
	// �����������
	System.ItemRecord[19] = true;
	// ����̵�ͷ��
	string face[] = { "mting-510.png", "mting-511.png" };
	WindowBitmap(face[System.GameTime / 6 % 2], 32, 32);
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	rectangle((X + 32) * ResolutionRatio, (Y + 32) * ResolutionRatio, (X + 64) * ResolutionRatio, (Y + 64) * ResolutionRatio);
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	// ����ı�����
	WDrawTexts(64, 0, 112, 32, "����", 1, 24L, true);
	WDrawTexts(64, 32, 128, 64, "�۸�ʵ�ݣ��򵽾���׬��", 3, 20L);
	WDrawTexts(0, 112, Width, 32, "�������ţ�30��ң�", 1);
	WDrawTexts(0, 144, Width, 32, "�뿪", 1);
	// ���ѡ�����
	DrawRect(48, 132 + Index * 32, 160, 24);
	// �����ϼ�ʱ
	if (KeyBoard.Press(KeyBoard.KeyUP)) {
		if (Index == 1) {
			--Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// �����¼�ʱ
	if (KeyBoard.Press(KeyBoard.KeyDOWN)) {
		if (Index == 0) {
			++Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// ����ȷ�ϼ�ʱ
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
	// ����ȡ����ʱ
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
	// ������ʱ��ִ��
	if (!Visible) return;
	// ��������
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
	// ����С��ͼ
	DrawSmallMap(35, 16);
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, 2);
	rectangle((X + 35) * ResolutionRatio, (Y + 16) * ResolutionRatio, (X + 156) * ResolutionRatio, (Y + 137) * ResolutionRatio);
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);
	// �����ı�����
	WDrawTexts(0, 120, 160, 32, Items[4].Name, 1, 20L, true);
	string viewfloorname;
	if (System.GameVariables[3] == 2 || System.GameVariables[3] == 3) viewfloorname = "����";
	else viewfloorname = to_string(viewfloorid) + " F";
	WDrawTexts(0, 184, 160, 20, viewfloorname, 1, 20L);
	WDrawTexts(0, 240, 160, 32, "-Quit(f)-", 0, 18L);
	WDrawTexts(0, 240, 160, 32, "-Enter-", 2, 18L);
	// ���ư���
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
	// �����¼�ʱ
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
	// �������ʱ
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
	// �����ϼ�ʱ
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
	// �����Ҽ�ʱ
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
	// ����ȷ�ϼ�ʱ
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
		if (viewfloorid > System.GameVariables[2]) { // ��¥����
			Actors[System.GameVariables[1]].X = System.Map[viewmapid].MapEvents[0].X;
			Actors[System.GameVariables[1]].Y = System.Map[viewmapid].MapEvents[0].Y;
		}
		else if (viewfloorid < System.GameVariables[2]) { // ��¥����
			Actors[System.GameVariables[1]].X = System.Map[viewmapid].MapEvents[1].X;
			Actors[System.GameVariables[1]].Y = System.Map[viewmapid].MapEvents[1].Y;
		}
		else { // ͬ�㴫�ͣ��ֵ��ϵ��£�
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
	// ����ȡ����ʱ
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
	// ������ʱ��ִ��
	if (!Visible) return;
	// ��ӿ����ı�
	vector <string> item;
	item.push_back("enter/space=ȷ��"); item.push_back("��������=���ƽ�ɫ�ƶ�/ѡ����Ʒ");
	item.push_back("r=�ص��������");
	item.push_back("p=���ص�ͼ��ʾ�˺�");
	item.push_back("shift(����)=���ٽ���");
	item.push_back("space=����ս����ս����ʹ�ã�");
	item.push_back("q=���ˣ�ս����ʹ�ã�");
	item.push_back("c=����һ����ս����ʹ�ã�"); 
	item.push_back("v=������ս����ʹ�ã�");
	item.push_back("c=�������");
	item.push_back("v=�������");
	item.push_back("��ctrl��2=���ٴ��棨ʱ���������Ĵ浵��");
	item.push_back("��ctrl��2=���ٶ�ȡ��ʱ���������Ĵ浵��");
	if (Actors[System.GameVariables[1]].Item[3] > 0) item.push_back("d=�鿴��������");
	if (Actors[System.GameVariables[1]].Item[4] > 0) item.push_back("f=����¥�㴫��");
	if (Actors[System.GameVariables[1]].Item[5] > 0) item.push_back("b=�򿪵��ߴ�");
	if (System.GameVariables[11] > 0) item.push_back("w=�鿴�������л���50hp��");
	if (System.GameVariables[12] > 0) item.push_back("e=�鿴���������л���50hp��");
	if (System.GameVariables[11] > 0) item.push_back("z=ʹ�ý�����ս����ʹ�ã�");
	if (System.GameVariables[12] > 0) item.push_back("x=ʹ�÷�������ս����ʹ�ã�");
	if (System.GameSwitches[19] > 0) item.push_back("t=ʹ��������");
	// ���ݵ�ǰҳ�����ı�
	for (int i = 0; i < min(15, item.size() - 15 * Page); ++i)
		WDrawTexts(0, 20 * i, 320, 20, item[i + 15 * Page], 0, 20L);
	int maxpage = item.size() / 15;
	// ���ư���
	WindowDrawArrow(232, 320, Page, maxpage, 64);
	// �������ʱ
	if (KeyBoard.Trigger(KeyBoard.KeyLEFT)) {
		if (Page == 0) return;
		--Page;
		PlaySE(System.CursorSE);
		return;
	}
	// �����Ҽ�ʱ
	if (KeyBoard.Trigger(KeyBoard.KeyRIGHT)) {
		if (Page == maxpage) return;
		++Page;
		PlaySE(System.CursorSE);
		return;
	}
	// ����ȡ����ʱ
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
	// ������ʱ��ִ��
	if (!Visible) return;
	// ���ü��ܻ�����Ϣ
	vector <int> list;
	int levelrequire[2][5] = { { 5, 20, 40, 60, 150 }, { 10, 30, 50, 70, 90 } };
	int breathrequire[2][5] = { { 1, 1, 2, 2, 4 }, { 1, 1, 2, 2, 4 } };
	int fatiguerequire[2][5] = { { 10, 10, 20, 20, 30 }, { 10, 10, 20, 20, 30 } };
	string desc[2][5] = { { "�����˺�*1.5����ʱ��(" + to_string(Actors[System.GameVariables[1]].Level) + ")����תΪ(" + to_string(Actors[System.GameVariables[1]].Level * 3 / 2) + ")�㹥���������˺�����(" + to_string(Actors[System.GameVariables[1]].Level) + ")ʱ�޷�����", 
		"�����˺������ӱ���ս���������˺���0.4������ʱ��(" + to_string(Actors[System.GameVariables[1]].Level) + ")����תΪ(" + to_string(Actors[System.GameVariables[1]].Level * 3 / 2) + ")������������˺��������˺�����(" + to_string(Actors[System.GameVariables[1]].Level * 2) + ")ʱ�޷�����",
		"�����˺�*1.5����նԷ�����Ϣ���������ڱ���ս������Ҳ�޷�������Ϣ�������˺�����(" + to_string(Actors[System.GameVariables[1]].Level) + ")ʱ�޷�����",
		"�����˺�*1.5����ȡ�Է�����Ϣ�������˺�����(" + to_string(Actors[System.GameVariables[1]].Level) + ")ʱ�޷�����",
		"ֱ�ӽ��Է�����������1�㣬�����˺�����(" + to_string(Actors[System.GameVariables[1]].Level * 3) + ")ʱ�޷�����"},
		{ "�����ܵ��˺�����60%����������ɵ�����Ч��", 
		"�����ܵ��˺�����30%�������������ٵ��˺�������Ч�������Ƿ����˺�������������չ��˺���10��",
		"�����˺�����40%�������ӶԷ�35��ƣ��", 
		"��������2���˺�����70%", 
		"������Է���״̬����������3���˺��ᱻ����" } };
	// �������м�����ӽ�vector
	for (int i = 0; i < 5; ++i)
		if (Actors[System.GameVariables[1]].Item[6 + Type * 5 + i] > 0)
			list.push_back(i);
	// ������Ϣ
	for (int i = 0; i < list.size(); ++i)
		WindowBitmap("mting-" + to_string(488 + 9 * Type + list[i]) + ".png", 16 + 40 * i, 16);
	WDrawTexts(0, 32, 288, 32, Items[6 + Type * 5 + list[Index]].Name + (System.GameVariables[13 + Type] == list[Index] + 1 ? "����װ����" : ""));
	WDrawTexts(0, 32, 288, 32, "�ȼ�Ҫ��:" + to_string(levelrequire[Type][list[Index]]), 2);
	WDrawTexts(0, 64, 288, 32, "ƣ��+" + to_string(fatiguerequire[Type][list[Index]]) + "����Ϣ���ģ�" + to_string(breathrequire[Type][list[Index]]), 2);
	WDrawTexts(0, 96, 288, 64, desc[Type][list[Index]], 3, 20L);
	// ���ѡ�����
	DrawRect(16 + 40 * Index, 16, 32, 32);
	// �������ʱ
	if (KeyBoard.Press(KeyBoard.KeyLEFT)) {
		if (Index > 0) {
			PlaySE(System.CursorSE);
			--Index;
		}
		return;
	}
	// �����Ҽ�ʱ
	if (KeyBoard.Press(KeyBoard.KeyRIGHT)) {
		if (Index < list.size() - 1) {
			PlaySE(System.CursorSE);
			++Index;
		}
		return;
	}
	// ����ȡ����ʱ
	if (KeyBoard.TriggerCancel()) {
		PlaySE(System.CancelSE);
		Visible = false;
		return;
	}
	// ����ȷ�ϼ�ʱ
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
	// ������ʱ��ִ��
	if (!Visible) return;
	// �����Ʒ��Ϣ
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
	// ���ѡ�����
	DrawRect(16 + 40 * Index, 16, 32, 32);
	// �������ʱ
	if (KeyBoard.Trigger(KeyBoard.KeyLEFT)) {
		if (Index == 1) {
			PlaySE(System.CursorSE);
			--Index;
		}
		return;
	}
	// �����Ҽ�ʱ
	if (KeyBoard.Trigger(KeyBoard.KeyRIGHT)) {
		if (Index == 0) {
			PlaySE(System.CursorSE);
			++Index;
		}
		return;
	}
	// ����ȡ����ʱ
	if (KeyBoard.TriggerCancel()) {
		PlaySE(System.CancelSE);
		Visible = false;
		return;
	}
	// ����ȷ�ϼ�ʱ
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
	// ������ʱ��ִ��
	if (!Visible) return;
	// �ļ�����ʱ�����ƴ浵����ͼ
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
	// ������ʱ��ִ��
	if (!Visible) return;
	// ��水��
	WindowDrawArrow(16, 16, Page, 199, 64, "");
	// ����ı�����
	for (int i = 0; i < Items.size(); ++i)
		WDrawTexts(0, 20 + 20 * i, 104, 20, "Save " + to_string(Page * 6 + i), 0, 18L);
	for (int i = 0; i < Items.size(); ++i) {
		if (i == Index) DrawRect(16, 36 + RectHeight * i, Width - 32, RectHeight);
		DrawItem(i, NormalColor());
	}
	// ���ڲ��ʱ��ִ��
	if (!Active) return;
	// �����ϼ�ʱ
	if (KeyBoard.Press(KeyBoard.KeyUP)) {
		if (Index > 0) {
			--Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// �����¼�ʱ
	if (KeyBoard.Press(KeyBoard.KeyDOWN)) {
		if (Index < Items.size() - 1) {
			++Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// �������ʱ
	if (KeyBoard.Press(KeyBoard.KeyLEFT)) {
		if (Page > 0) {
			--Page;
			PlaySE(System.CursorSE);
		}
		return;
	}
	//�����Ҽ�ʱ
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
	// ������ʱ��ִ��
	if (!Visible) return;
	// ���ɾ�
	for (int i = 0; i < 12; ++i) {
		settextcolor(System.Achievement[Index * 12 + i] ? TextColor("yellow") : TextColor("gray"));
		WindowBitmap(System.Achievement[Index * 12 + i] ? "mting-1252.png" : "mting-1253.png", 16, 16 + 24 * i);
		WDrawTexts(32, 24 * i + 2, 256, 20, name[Index][i], 0, 20L);
		WDrawTexts(32, 24 * i + 2, 256, 20, System.Achievement[Index * 12 + i] ? description[Index][i] : "��̽������" , 2, 16L);
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
	// ������ʱ��ִ��
	if (!Visible) return;
	// �������
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
	// ������ʱ��ִ��
	if (!Visible) return;
	// �������
	string str = "";
	if (!System.Victory) {
		if (next == 0) {
			str = str + "��ǰ���ھ��������߾���ʮ�ﴦ����һ�������ĸ�����\n";
			str = str + "���������ˡ�";
		}
		else if (next == 1) {
			str = str + "�����������ߣ�������\n";
			str = str + "����ʵ��һ����������Ĺ��£���ʿ���ű�������������ȹ����Ĺ��¡�\n";
			str = str + "���������������������������Կ�ħ��ľ���Ҫ������ħ�������к�ʣ�µĹ���������β�ͣ���������Ϯ��·�ˣ�����ʮ��ĺ�ƽ֮�󣬴�Ҷ���Ϊʣ�µĹ��ﲻ��Ϊ�壬������������Ҫ���Ŀ����ߡ�\n";
			str = str + "˭�������Ĳ����Ĺ����Ǿ�Ȼ���ѡ�����µ�ħ������Ѹ����֯��������ɱ������������ռ�˸��������Ը���Ϊ�ݵ������������ƣ�����һ��ͻϮ�гɹ�°���������Ĺ�����\n";
		}
		else {
			str = str + "�ܿ�Ц�ɣ�һ������ʮ��Ľ䱸��������и����һ��ȴ��н�����������˷����ĺŽǣ�����Ҳ����֣�ֻ��ǧ������������ǧ�շ����ĵ���\n";
			str = str + "����Ѹ����֯��������ʿΪ�׵�һ֧С�ӣ�ҪǱ��ħ���ȳ�����\n";
			str = str + "�����ǵĹ��£��ʹ����￪ʼ����";
		}
	}
	else {
		if (next == 0) {
			str = str + "���µ�����ͽ����ˣ���ʿ�ɹ��ȳ��˹������쵽���ͽ���ж�׺���������е��������\n";
			str = str + "����ħ���������Ĺ�����ô�죬����û��ȥ�룬�������Ǿʹ˳��ţ������������ֻ���������ķ���˭֪���أ���ʱ��ֻҪ���ź��˵��ǻ۾Ϳ����ˡ�\n";
			str = str + "���±Ƚ����ף�Ҳ�Ƚ���������ϲ�ɺأ���ϲ�ɺأ�������������anecdote";
		}
		else {
			str = str + "��ϲ���أ����Ĺ�������Ϊ\n";
			str = str + "�ȼ���" + to_string(Actors[System.GameVariables[1]].Level) + "��������" + to_string(Actors[System.GameVariables[1]].Hp) + "��������" + to_string(Actors[System.GameVariables[1]].Atk) + "��������" + to_string(Actors[System.GameVariables[1]].Def);
			str = str + "���飺" + to_string(Actors[System.GameVariables[1]].Exp) + "����ң�" + to_string(Actors[System.GameVariables[1]].Gold) + "��\n";
		}
	}
	WDrawTexts(0, 0, 320, 320, str, 3);
	// ����ȷ�ϼ�ʱ
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
	// ��ˢ��һ��
	System.NeedRefresh = true;
	// ���ɱ���ͼ��
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
	// ���������
	window.X = -160;
	window.Y = 224;
	window.Z = 1;
	// ���ɳɾʹ���
	achcursurwindow.X = 176;
	achcursurwindow.Y = 240;
	achcursurwindow.Z = 1;
	achcursurwindow.Active = false;
	achcursurwindow.Visible = false;
	// ���ű���BGM
	System.GameBGM = "sound-0.mp3";
	// �ж���������Ч��
	if (!ExistFile("save\\recent.dat")) {
		fstream file;
		file.open("save\\recent.dat", ios::ate | ios::out);
		file << 0 << endl;
		file.close();
	}
	else
		window.Index = 1;
	// ��ʼ����
	Graphics.Update();
	System.DoTransition();
	// ����BGM
	PlayBGM(System.GameBGM);
	// ��������
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
	// ��ѭ��
	while (System.Scene == this) {
		Update();
		Graphics.Update();
	}
	// �ͷ�ͼ�κʹ���
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
	// ���´浵����
	if (filewindow.Visible) {
		filewindow.fileid = cursorwindow.Page * 6 + cursorwindow.Index;
		// ����ȡ����ʱ
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
		// ����ȷ�ϼ�ʱ
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
	// ���³ɾʹ���
	if (achwindow.Visible) {
		achwindow.Index = achcursurwindow.Index;
		// ����ȡ����ʱ
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
	// ����ȷ�ϼ�ʱ
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
	// ����ˢ��һ��
	System.NeedRefresh = true;
	// ��ʼ����ͼ���
	Graphics.MapPatterns.Initialization();
	// ���õ�ͼ
	AddRecord();
	SetUp();
	// ִ�н���
	System.DoTransition();
	// ��ѭ��
	while (System.Scene == this) {
		Graphics.Update();
		Update();
	}
	// �ͷŴ���
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
	// �гɾ�ʱ
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
	// չ�����¿�ʼ�Ĵ���
	if (System.Victory) {
		if (startwindow.next == 2) startwindow.next = 0;
		startwindow.Visible = true;
		return;
	}
	if (!System.GameSwitches[5]) {
		startwindow.Visible = true;
		return;
	}
	// Ѫ�����㣬��Ϸ����
	if (System.EndGame) {
		System.EndGame = false;
		GameOver();
		return;
	}
	// ʤ��ʱ�������ʱûɶ�ã�
	if (System.Victory) return;
	// ��Ҫ���õ�ͼʱ����
	if (System.SetUp) {
		SetUp();
		System.SetUp = false;
	}
	// BGM����
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
	// �����ڻʱ����ִ��
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
	// ִ��ս��
	if (System.BattleEnemyID >= 0) BattleFunction();
	// �¼�ִ����Ϻ���¼��仯
	if (System.TransformingEventName != "") {
		System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Name = System.TransformingEventName;
		System.TransRecord[Actors[System.GameVariables[1]].MapID][System.FunctioningEventID] = System.TransformingEventName;
		System.TransformingEventName = "";
		if (System.DirectlyFunction) {
			System.DirectlyFunction = false;
			System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Order();
		}
	}
	// ��Ļ��ͼ
	if (System.ScreenShot) {
		saveimage(System.ShotFile.c_str());
		System.ScreenShot = false;
		System.ShotFile = "";
		return;
	}
	// ִ�н���
	if (System.Transition) System.DoTransition(5);
	// ����¥ͼ��
	if (System.StairGraphic.Opacity > 0) {
		System.StairGraphic.Opacity -= 51;
		Graphics.Update();
		return;
	}
	// ��ݼ�����
	ShortcutKey();
	// ��Ҳٿش���
	Player.Update();
}

void GameMap::ShortcutKey() {
	// SHIFT������
	System.Accelerate = KeyBoard.Press(KeyBoard.KeySHIFT);
	// ����T��ʱ�������Ų���
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
	// ����B��ʱ���򿪵��ߴ�
	if (KeyBoard.Trigger(KeyBoard.KeyB) && Actors[System.GameVariables[1]].Item[5] > 0) {
		curewindow.Index = 0;
		curewindow.Visible = true;
		return;
	}
	// ����W&E��ʱ���򿪼�����
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
	// ����P��ʱ�����ص�ͼ����
	if (KeyBoard.Trigger(KeyBoard.KeyP)) {
		System.GameSwitches[4] = !System.GameSwitches[4];
		return;
	}
	// ����L��ʱ���鿴�����б�
	if (KeyBoard.Trigger(KeyBoard.KeyL)) {
		helpwindow.Visible = true;
		return;
	}
	// ����R��ʱ��ѯ���Ƿ񷵻�
	if (KeyBoard.Trigger(KeyBoard.KeyR)) {
		if (YesNoPrint("��ȷ��Ҫ������ǰ���ȣ����ر�����") == IDYES) {
			System.Scene = new GameTitle;
		}
		return;
	}
	// ����D��ʱ���򿪹����ֲ�
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
	// ����F��ʱ����¥�㴫����
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
	// ����C&V����˫����/��CTRL��ʱ�������
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
		System.MessageContent.push_back("�����ѱ������浵 " + to_string(index) + "��");
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
	// ����ȷ�ϼ�ʱ
	if (KeyBoard.TriggerConfirm()) {
		System.MessageContent.erase(System.MessageContent.begin());
		System.MessageName.erase(System.MessageName.begin());
		System.MessageHead.erase(System.MessageHead.begin());
		messagewindow.Opacity = 0;
		return;
	}
}

void GameMap::GameOver() {
	// ֹͣBGM��������Ϸ����SE
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
	// ����ȡ����ʱ
	if (KeyBoard.TriggerCancel()) {
		PlaySE(System.CancelSE);
		filewindow.Visible = false;
		cursorwindow.Visible = false;
		cursorwindow.Active = false;
		cursorwindow.Page = 0;
		cursorwindow.Index = 0;
		return;
	}
	// ����ȷ�ϼ�ʱ
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
			System.MessageContent.push_back("�����ѱ������浵 " + to_string(filewindow.fileid) + "��");
			System.MessageHead.push_back(0);
		}
		return;
	}
}

void GameMap::BattleFunction() {
	// д��˫����Ϣ
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
	// ��¼ԭʼ״̬
	bool originstatus[2] = { Actors[System.GameVariables[1]].Status[0], Actors[System.GameVariables[1]].Status[1] };
	int originweak = System.GameVariables[4];
	// ��䶯�����Ĺֵ�������¼
	if (Enemies[System.BattleEnemyID].GotP(21)) {
		int vary = Enemies[System.BattleEnemyID].GotAtk() / 2;
		battlewindow.eAtk += vary;
		battlewindow.eVaryAtk += vary;
	}
	// ս������
	int result = BattleUpdate();
	// ���˵�������ָ�ԭʼ״̬
	if (result == 2) {
		Actors[System.GameVariables[1]].Status[0] = originstatus[0];
		Actors[System.GameVariables[1]].Status[1] = originstatus[1];
		System.GameVariables[4] = originweak;
	}
	// ����ƣ�͵ĳɾ�
	if (battlewindow.aFatigue >= 80) System.HitAchievement(10);
	if (battlewindow.eFatigue >= 80) System.HitAchievement(11);
	// �ָ�����
	if (System.Accelerate) System.Accelerate = false;
	// ս��ʤ�������
	if (result == 0) {
		// ����״̬
		Actors[System.GameVariables[1]].Hp = battlewindow.aHp;
		Actors[System.GameVariables[1]].Breath = battlewindow.aBreath;
		// ����ֽ��б���ͼ�¼��������¼���ʧ
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
		// ��ս��ݳɾ�
		System.HitAchievement(0);
		// �����ѻ�ɱ����
		++System.GameVariables[5];
		// ɱ�гɾ�
		if (System.GameVariables[5] >= 200) System.HitAchievement(1);
		// ״̬�ɾ�
		if (Actors[System.GameVariables[1]].Status[0] || Actors[System.GameVariables[1]].Status[1]) System.HitAchievement(2);
		if (Actors[System.GameVariables[1]].Hp <= 200) System.HitAchievement(3);
		if (Actors[System.GameVariables[1]].Breath == Actors[System.GameVariables[1]].BreathLimit - 1) System.HitAchievement(9);
		if (Actors[System.GameVariables[1]].Exp >= 300) System.HitAchievement(18);
		if (Actors[System.GameVariables[1]].Gold >= 1000) System.HitAchievement(19);
	}
	// ս��ʧ��
	else if (result == 1) {
		GameOver();
		return;
	}
	System.BattleEnemyID = -1;
	battlewindow.Visible = false;
	// ����п��ر����䶯��
	vector <string> temp = Split(System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.FunctioningEventID].Name, "|");
	if (result == 0 && temp.size() == 5) {
		if (stoi(temp[2]) == 1) System.GameSwitches[stoi(temp[3])] = stoi(temp[4]);
		if (stoi(temp[2]) == 2) System.GameVariables[stoi(temp[3])] += stoi(temp[4]);
	}
}

int GameMap::BattleUpdate() {
	// �ȴ�2֡����
	DataMap::WaitCount(2);
	int singlebreath = Actors[System.GameVariables[1]].BreathLimit / 6;
	int conatk; // ����
	int animationid; // �������
	int reflexedam = 2 * battlewindow.aAtk + Actors[System.GameVariables[1]].GotThreshold() > battlewindow.eDef ? (battlewindow.aAtk > battlewindow.eDef ? battlewindow.aAtk - battlewindow.eDef : 1) : 0;// �����˺�
	int abdefense = 0, reflexdefense = 0; // ��4/5ʣ��غ���
	bool turn = true; // �ִ�
	bool firstround = true; // �Ƿ��ǵ�һ�ֹ�������Ϊ���е�һ�ֿ��Ը���״̬
	bool lockbr = false; // ��ס��Ϣ
	int shieldballres = 2;

	// �ȹ�
	if (Enemies[System.BattleEnemyID].GotP(5)) turn = false;
	// ѭ��ս��
	while (true) {
		// �ҷ��ִΣ���¼������
		conatk = max(Enemies[System.BattleEnemyID].Conatk,1);
		if (turn) {
			// Эͬ����
			bool synergy = false;
			// ����
			if (battlewindow.funckey == 1) return 2;
			// �ҷ��˺�
			int edam = battlewindow.aAtk + Actors[System.GameVariables[1]].GotThreshold() > battlewindow.eDef ? (battlewindow.aAtk > battlewindow.eDef ? battlewindow.aAtk - battlewindow.eDef : 1) : 0;
			// ����״̬ʱ�˺�����
			if (battlewindow.weaken) edam /= 2;
			// ������˺�����
			reflexedam = edam * 10;
			// ���ö������
			animationid = 2;
			// �˺�Ϊ0�����
			if (edam == 0) {
				battlewindow.funckey = 0;
				animationid = 9;
			}
			else {
				// ����ƣ��ֵ�ͼ���
				int fat = min(battlewindow.aFatigue / 10, 10);
				edam = max(edam - edam * fat / 10, 0);
				// ���˺��˺�Ϊ0�����
				if (edam == 0) {
					battlewindow.funckey = 0;
					animationid = 8;
				}
				else {
					// ����һ��
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
					// ����1
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
					// ����2
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
					// ����3
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
					// ����4
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
					// ����5
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
					else // �ҷ���Ϣ����
						battlewindow.aBreath = min(edam > 0 ? (battlewindow.aBreath + Round(1.0 * battlewindow.eDef / battlewindow.aAtk * 6)) : battlewindow.aBreath, Actors[System.GameVariables[1]].BreathLimit - 1);
				}
			}
			// �ڲ�����Ϣ������µз���Ϣ����
			if (!lockbr && animationid != 72)
				battlewindow.eBreath = min(edam > 0 ? (battlewindow.eBreath + Round(edam / 3.0)) : battlewindow.eBreath, Enemies[System.BattleEnemyID].Breath);
			// ��Ѫ�������ͽ���
			battlewindow.eHp = max(battlewindow.eHp - edam, 0);
			Graphics.Animations.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, Animation[animationid] });
			Graphics.Damages.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, false, to_string(edam), GameWindow::TextColor("red") });
			// ��
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
			// ������
			bool succession = false;
			while (conatk--) {
				// �����Ϣ�ͷ���
				bool brclear = false, beatback = false;
				// ���˵����
				if (battlewindow.funckey == 1) return 2;
				// �з��˺�
				int adam = battlewindow.eAtk + Actors[System.GameVariables[1]].GotThreshold() * 2 > (Enemies[System.BattleEnemyID].GotP(6) ? 0 : battlewindow.aDef) ? (battlewindow.eAtk > (Enemies[System.BattleEnemyID].GotP(6) ? 0 : battlewindow.aDef) ? battlewindow.eAtk - (Enemies[System.BattleEnemyID].GotP(6) ? 0 : battlewindow.aDef) : 1) : 0, recover;
				// ����������˺�����
				if (battlewindow.eweaken) adam /= 2;
				// ���������������Ϣ���˺�1.5��
				if (Enemies[System.BattleEnemyID].GotP(35) && battlewindow.aBreath >= singlebreath) adam *= 1.5;
				// �������
				animationid = Enemies[System.BattleEnemyID].AnimationID;
				// �˺�Ϊ0�����
				if (adam == 0) {
					if (battlewindow.funckey >= 8) battlewindow.funckey = 0;
					animationid = 9;
				}
				else {
					// ����ƣ��ֵ�ļ���
					int fat = min(battlewindow.eFatigue / 10, 10);
					int recover = 0;
					bool immune = false, reflex = false;
					adam = max(adam - adam * fat / 10, 0);
					// ��Ѫ
					if (Enemies[System.BattleEnemyID].GotP(3)) recover = adam * stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]) / 100;
					// ���˺��˺�Ϊ0�����
					if (adam == 0) {
						if (battlewindow.funckey >= 8) battlewindow.funckey = 0;
						animationid = 8;
					}
					else {
						// �ܼ�1
						if (battlewindow.funckey == 8) {
							battlewindow.funckey = 0;
							if (battlewindow.aBreath >= singlebreath && battlewindow.aFatigue <= 50 && shieldballres > 0) {
								battlewindow.aBreath -= singlebreath;
								if (Actors[System.GameVariables[1]].Item[22] > 0) --shieldballres;
								battlewindow.aFatigue += 10;
								immune = true;
							}
						}
						// �ܼ�2
						if (battlewindow.funckey == 9) {
							battlewindow.funckey = 0;
							if (battlewindow.aBreath >= singlebreath && battlewindow.aFatigue <= 50 && shieldballres > 0) {
								battlewindow.aBreath -= singlebreath;
								if (Actors[System.GameVariables[1]].Item[22] > 0) --shieldballres;
								battlewindow.aFatigue += 10;
								reflex = true;
							}
						}
						// �ܼ�4
						if (battlewindow.funckey == 11) {
							battlewindow.funckey = 0;
							if (battlewindow.aBreath >= (2 - (Actors[System.GameVariables[1]].Item[22] > 0)) * singlebreath && abdefense == 0 && reflexdefense == 0 && battlewindow.aFatigue <= 50 && shieldballres > 0) {
								battlewindow.aBreath -= (2 - (Actors[System.GameVariables[1]].Item[22] > 0)) * singlebreath;
								if (Actors[System.GameVariables[1]].Item[22] > 0) --shieldballres;
								battlewindow.aFatigue += 20;
								abdefense = 2;
							}
						}
						// �ܼ�5
						if (battlewindow.funckey == 12) {
							battlewindow.funckey = 0;
							if (battlewindow.aBreath >= (4 - (Actors[System.GameVariables[1]].Item[22] > 0)) * singlebreath && abdefense == 0 && reflexdefense == 0 && battlewindow.aFatigue <= 50 && shieldballres > 0) {
								battlewindow.aBreath -= (4 - (Actors[System.GameVariables[1]].Item[22] > 0)) * singlebreath;
								if (Actors[System.GameVariables[1]].Item[22] > 0) --shieldballres;
								battlewindow.aFatigue += 30;
								reflexdefense = 3;
							}
						}
						// �ܼ�1������
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
						// �ܼ�2������
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
						// �ܼ�3
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
								// ħ��
								if (Enemies[System.BattleEnemyID].GotP(38)) battlewindow.aFatigue += 35;
								else battlewindow.eFatigue += 35;
								beatback = true;
							}
						}
						// �ܼ�4
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
						// �ܼ�5
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
						// ��Ԫ
						if (Enemies[System.BattleEnemyID].GotP(32)) {
							if (!immune && !reflex && battlewindow.aBreath >= Actors[System.GameVariables[1]].BreathLimit / 6) {
								battlewindow.eBreath = Enemies[System.BattleEnemyID].Breath;
								battlewindow.aBreath -= Actors[System.GameVariables[1]].BreathLimit / 6;
								brclear = true;
								Graphics.Animations.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, Animation[24] });
								DataMap::WaitCount(2);
							}
						}
						// ����һ��
						if (battlewindow.eBreath >= Enemies[System.BattleEnemyID].Breath || succession) {
							// ��˶
							if (Enemies[System.BattleEnemyID].GotP(16)) battlewindow.eBreath = Enemies[System.BattleEnemyID].Breath / 2;
							else battlewindow.eBreath = 0;
							if (!succession) {
								if (Enemies[System.BattleEnemyID].GotP(38)) battlewindow.aFatigue += 5;
								else battlewindow.eFatigue += 5;
							}
							++animationid;
							// ��ɱ��
							if (Enemies[System.BattleEnemyID].GotP(13)) {
								adam *= 3;
								recover *= 3;
							}
							// ����
							else if (Enemies[System.BattleEnemyID].GotP(18)) {
								adam *= 5;
								recover *= 5;
								int loss = battlewindow.eHp / 2;
								battlewindow.eHp -= loss;
								Graphics.Damages.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, false, to_string(loss), GameWindow::TextColor("red") });
							}
							// ������
							else if (Enemies[System.BattleEnemyID].GotP(35)) {
								adam *= 4;
								recover *= 4;
							}
							else {
								adam *= 2;
								recover *= 2;
							}
							// ��ʯ
							if (Enemies[System.BattleEnemyID].GotP(10)) {
								battlewindow.eAtk += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
								battlewindow.eDef += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
								battlewindow.eVaryAtk += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
								battlewindow.eVaryDef += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
								Graphics.Animations.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, Animation[15] });
							}
							// ˮ����
							if (Enemies[System.BattleEnemyID].GotP(11)) {
								battlewindow.eHp += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
								Graphics.Animations.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, Animation[24] });
							}
							// Թ��
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
							// ��Ѫ
							if (Enemies[System.BattleEnemyID].GotP(14)) {
								battlewindow.eAtk += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[2]);
								battlewindow.eVaryAtk += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[2]);
								recover *= 2;
								Graphics.Animations.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, Animation[15] });
							}
							// ������&ʥ��
							if (Enemies[System.BattleEnemyID].GotP(15) || Enemies[System.BattleEnemyID].GotP(26)) {
								if (!reflex && !immune) {
									battlewindow.aBreath -= battlewindow.aBreath % (Actors[System.GameVariables[1]].BreathLimit / 6);
									Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 64, battlewindow.Y + 32, 0, Animation[14] });
									brclear = true;
								}
							}
							// ����&ʥ��
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
							// ��ħ��
							if (Enemies[System.BattleEnemyID].GotP(24))
								if (!reflex && !immune)
									++battlewindow.buff;
							// ����
							if (Enemies[System.BattleEnemyID].GotP(25)) {
								if (!reflex && !immune) {
									battlewindow.aBreath = max(battlewindow.aBreath - Actors[System.GameVariables[1]].BreathLimit / 6, 0);
									Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 64, battlewindow.Y + 32, 0, Animation[14] });
									brclear = true;
								}
							}
							// ������
							if (Enemies[System.BattleEnemyID].GotP(28)) succession = true;
							// Ϸ��
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
							// ����
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
							// ����
							if (Enemies[System.BattleEnemyID].GotP(36)) {
								if (!Actors[System.GameVariables[1]].Status[0] && !reflex && !immune) {
									Actors[System.GameVariables[1]].Status[0] = true;
									Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 64, battlewindow.Y + 32, 0, Animation[14] });
									System.GameVariables[6] = Player.Step;
								}
							}
							// ����Ϣʱ������
							if (!lockbr)
								battlewindow.eBreath = min(adam > 0 ? (battlewindow.eBreath + Round(1.0 * battlewindow.aDef / battlewindow.aAtk * 6)) : battlewindow.eBreath, Enemies[System.BattleEnemyID].Breath);
						}
						// �ж�
						if (Enemies[System.BattleEnemyID].GotP(1) && firstround) {
							if (!Actors[System.GameVariables[1]].Status[0] && !reflex && !immune) {
								Actors[System.GameVariables[1]].Status[0] = true;
								Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 64, battlewindow.Y + 32, 0, Animation[14] });
								System.GameVariables[6] = Player.Step;
							}
						}
						// ˥��
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
						// ճҺȭ
						if (Enemies[System.BattleEnemyID].GotP(9)) {
							if (reflex) battlewindow.eFatigue += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
							else if (!immune) battlewindow.aFatigue += stoi(Split(Enemies[System.BattleEnemyID].Name, "|")[1]);
						}
						// ��ħ��
						if (Enemies[System.BattleEnemyID].GotP(24)) {
							if (!reflex && !immune) ++battlewindow.buff;
							if (battlewindow.buff >= 7) {
								battlewindow.buff = 0;
								adam *= 7;
								recover *= 7;
								Graphics.Animations.push({ battlewindow.X + battlewindow.Width - 48, battlewindow.Y + 48, 0, Animation[29] });
							}
						}
						// ��Ѫ
						if (Enemies[System.BattleEnemyID].GotP(3)) {
							battlewindow.eHp += recover;
							if (adam > 0) Graphics.Damages.push({ battlewindow.X + 48, battlewindow.Y + 48, 0, true, to_string(recover), GameWindow::TextColor("green") });
						}
					}
				}
				// ��ŭ
				if (Enemies[System.BattleEnemyID].GotP(31)) {
					battlewindow.eVaryAtk += battlewindow.eAtk;
					battlewindow.eAtk *= 2;
				}
				// ��������²������ҷ���Ϣ
				if (!brclear && !Enemies[System.BattleEnemyID].GotP(24))
					battlewindow.aBreath = min(adam > 0 ? (battlewindow.aBreath + Round(adam / 10.0)) : battlewindow.aBreath, Actors[System.GameVariables[1]].BreathLimit - 1);
				// ��Ѫ�������ͽ���
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
	// ��ȡ¥������
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
	// ��ȡ��������
	memset(buffer, 0, sizeof(buffer));
	inFile.getline(buffer, 400000);
	datastr = buffer;
	gamedata = Split(datastr, ",");
	for (int i = 0; i < 5000; ++i)
		System.GameSwitches[i] = stoi(gamedata[i]);
	datastr.clear();
	gamedata.clear();
	// ��ȡ��������
	memset(buffer, 0, sizeof(buffer));
	inFile.getline(buffer, 400000);
	datastr = buffer;
	gamedata = Split(datastr, ",");
	for (int i = 0; i < 5000; ++i)
		System.GameVariables[i] = stoi(gamedata[i]);
	datastr.clear();
	gamedata.clear();
	// ��ȡ��Ʒ����
	memset(buffer, 0, sizeof(buffer));
	inFile.getline(buffer, 400000);
	datastr = buffer;
	gamedata = Split(datastr, ",");
	for (int i = 0; i < 99; ++i)
		System.ItemRecord[i] = stoi(gamedata[i]);
	datastr.clear();
	gamedata.clear();
	// ��ȡ��ʯ����
	memset(buffer, 0, sizeof(buffer));
	inFile.getline(buffer, 400000);
	datastr = buffer;
	gamedata = Split(datastr, ",");
	for (int i = 0; i < 99; ++i)
		for (int j = 0; j < 99; ++j)
			System.GemRecord[i][j] = stoi(gamedata[i * 99 + j]);
	datastr.clear();
	gamedata.clear();
	// ��ȡѪƿ����
	memset(buffer, 0, sizeof(buffer));
	inFile.getline(buffer, 400000);
	datastr = buffer;
	gamedata = Split(datastr, ",");
	for (int i = 0; i < 999; ++i)
		System.BottleRecord[i] = stoi(gamedata[i]);
	datastr.clear();
	gamedata.clear();
	// ��ȡ�¼�����
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
	// ��ȡ�任����
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
	// ��ȡ��������
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
	// ��ȡ��������
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
	// lambda���ʽ
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
	// ����ɾ�
	fstream file;
	file.open("save\\achievement.dat", ios::ate | ios::out);
	string achstr = to_string(System.Achievement[0]);
	for (int i = 1; i < 36; ++i)
		achstr = achstr + "," + to_string(System.Achievement[i]);
	file << achstr << endl;
	file.close();
	// ������Ϸ����
	string filename = "save\\save" + to_string(fileid) + ".sav";
	ofstream outFile(filename, ios::out | ios::binary);
	string gamedata = "";
	// д��¥������
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
	// д�뿪������
	gamedata = writefbool(gamedata, 5000, System.GameSwitches);
	gamedata.pop_back();
	outFile << gamedata << endl;
	gamedata = "";
	// д���������
	gamedata = writefint(gamedata, 5000, System.GameVariables);
	gamedata.pop_back();
	outFile << gamedata << endl;
	gamedata = "";
	// д����Ʒ����
	gamedata = writefbool(gamedata, 99, System.ItemRecord);
	gamedata.pop_back();
	outFile << gamedata << endl;
	gamedata = "";
	// д�뱦ʯ����
	for (int i = 0; i < 99; ++i)
		for (int j = 0; j < 99; ++j)
			gamedata = writeint(gamedata, System.GemRecord[i][j]);
	gamedata.pop_back();
	outFile << gamedata << endl;
	gamedata = "";
	// д��Ѫƿ����
	gamedata = writefbool(gamedata, 999, System.BottleRecord);
	gamedata.pop_back();
	outFile << gamedata << endl;
	gamedata = "";
	// д���¼�����
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
	// д��任����
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
	// д����������
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
	// д����������
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
	// ����ͼ�����¼�¼�
	for (auto rc : System.TransRecord) {
		for (auto c : rc.second) {
			System.Map[rc.first].MapEvents[c.first].Name = c.second;
			auto name = Split(c.second, "|");
			// �����
			if (name[0] == "MONSTER") {
				System.Map[rc.first].MapEvents[c.first].File[0] = Enemies[stoi(name[1])].File[0];
				System.Map[rc.first].MapEvents[c.first].File[1] = Enemies[stoi(name[1])].File[1];
			}
			// ������
			if (System.Map[rc.first].MapEvents[c.first].Name == "DOOR|1" || Split(System.Map[rc.first].MapEvents[c.first].Name, "|")[0] == "SDOOR") {
				System.Map[rc.first].MapEvents[c.first].File[0] = "mting-35.png";
				System.Map[rc.first].MapEvents[c.first].File[1] = "mting-35.png";
			}
			// �����ҵĹ�
			if (System.Map[rc.first].MapEvents[c.first].Name == "BARRIER|50") {
				System.Map[rc.first].MapEvents[c.first].File[0] = "mting-28.png";
				System.Map[rc.first].MapEvents[c.first].File[1] = "mting-29.png";
				System.Map[rc.first].MapEvents[c.first].Through = true;
			}
			// �����Ĺ�
			if (System.Map[rc.first].MapEvents[c.first].Name == "ICE") {
				System.Map[rc.first].MapEvents[c.first].File[0] = "mting-245.png";
				System.Map[rc.first].MapEvents[c.first].File[1] = "mting-245.png";
			}
			// �䴫���ŵ�
			if (Split(System.Map[rc.first].MapEvents[c.first].Name, "|")[0] == "TRANS") {
				System.Map[rc.first].MapEvents[c.first].File[0] = "mting-243.png";
				System.Map[rc.first].MapEvents[c.first].File[1] = "mting-244.png";
				System.Map[rc.first].MapEvents[c.first].Through = true;
			}
		}
	}
	// ������¼�¼�
	for (auto rc : System.EventRecord)
		for (auto c : rc.second) {
			if (c >= System.Map[rc.first].MapEvents.size()) break;
			System.Map[rc.first].MapEvents[c].Exist = false;
		}
	System.NeedRefresh = true;
}

void GameMap::AddRecord() {
	// ����ͼ��������¼�
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
			// �����
			if (temp.Name == "ICE") {
				temp.File[0] = "mting-245.png";
				temp.File[1] = "mting-245.png";
				temp.Through = false;
			}
			// �����ҵ�
			if (temp.Name == "BARRIER|50") {
				temp.File[0] = "mting-28.png";
				temp.File[1] = "mting-29.png";
				temp.Through = true;
			}
			System.Map[rc.first].MapEvents.push_back(temp);
		}
	}
}
