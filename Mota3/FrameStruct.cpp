#include "FrameStruct.h"
#include "bass.h"
#include "bassmidi.h"
#pragma comment(lib, "bass.lib")
#pragma comment(lib, "bassmidi.lib")

DataSystem System;
vector <GameActors> Actors;
GamePlayer Player;
vector <GameEnemies> Enemies;
vector <GameItem> Items;
vector <GameAnimation> Animation;
GameGraphics Graphics;
GameKeyBoard KeyBoard;
TCHAR szBuffer[MAX_PATH]{ 0 };
HSTREAM BGM = 0;
NpcInfo npc;
TransInfo trans;

void AddDialogue(int head, string name, string content) {
	System.MessageHead.push_back(head);
	System.MessageName.push_back(name);
	System.MessageContent.push_back(content);
}

void WindowRect(int backx, int backy, int cornerx[], int cornery[], int sidex[], int sidey[], int backlen, int cornerlen, int sidelen, int opacity, int drawx, int drawy, int drawwidth, int drawheight, bool select) {
	// ��͸����Ϊ0ʱ��ִ��
	if (opacity == 0) return;
	// ��������;��β�ȡ
	drawx = drawx * ResolutionRatio;
	drawy = drawy * ResolutionRatio;
	drawwidth = drawwidth * ResolutionRatio;
	drawheight = drawheight * ResolutionRatio;
	int width = drawwidth - 2 * cornerlen * select + select;
	int height = drawheight - 2 * cornerlen * select + 2 * select;
	int x = drawx + cornerlen * select;
	int y = drawy + cornerlen * select;
	backx = backx * (width - 2 * cornerlen * select) / backlen;
	backy = backy * (height - 2 * cornerlen * select) / backlen;
	// ͼ����ʾ��ÿ8�������һ��
	IMAGE* rect = new IMAGE, * side = new IMAGE;
	loadimage(rect, ("resource\\picture\\" + System.WindowskinName).c_str(), 192 * (width - 2 * cornerlen * select) / backlen, 128 * (height - 2 * cornerlen * select) / backlen);
	loadimage(side, ("resource\\picture\\" + System.WindowskinName).c_str());
	WOpaqueImage(nullptr, x, y, width, height, backx, backy, rect, opacity);
	for (int i = 0; i < drawwidth - 2 * cornerlen; i += 8 - 4 * select) {
		WOpaqueImage(nullptr, drawx + i + cornerlen, drawy, 8 - 4 * select, cornerlen, sidex[0] + i % sidelen, sidey[0], side, opacity);
		WOpaqueImage(nullptr, drawx + i + cornerlen, drawy + drawheight - cornerlen, 8 - 4 * select, cornerlen, sidex[1] + i % sidelen, sidey[1], side, opacity);
	}
	for (int i = 0; i < drawheight - 2 * cornerlen; i += 8 - 4 * select) {
		WOpaqueImage(nullptr, drawx, drawy + i + cornerlen, cornerlen, 8 - 4 * select, sidex[2], sidey[2] + i % sidelen, side, opacity);
		WOpaqueImage(nullptr, drawx + drawwidth - cornerlen, drawy + i + cornerlen, cornerlen, 8 - 4 * select, sidex[3], sidey[3] + i % sidelen, side, opacity);
	}
	WOpaqueImage(nullptr, drawx, drawy, cornerlen, cornerlen, cornerx[0], cornery[0], side, opacity);
	WOpaqueImage(nullptr, drawx + drawwidth - cornerlen, drawy, cornerlen, cornerlen, cornerx[1], cornery[1], side, opacity);
	WOpaqueImage(nullptr, drawx, drawy + drawheight - cornerlen, cornerlen, cornerlen, cornerx[2], cornery[2], side, opacity);
	WOpaqueImage(nullptr, drawx + drawwidth - cornerlen, drawy + drawheight - cornerlen, cornerlen, cornerlen, cornerx[3], cornery[3], side, opacity);
	delete rect;
	delete side;
}

void DrawTexts(int x, int y, int width, int height, string content, int pos, LONG size, bool bond) {
	int h = Font.lfHeight * ResolutionRatio;
	Font.lfHeight = size * ResolutionRatio;
	if (bond) Font.lfWeight = 600;
	settextstyle(&Font);
	RECT r = { x * ResolutionRatio, y * ResolutionRatio, (x + width) * ResolutionRatio, (y + height) * ResolutionRatio };
	if (pos == 0) drawtext(content.c_str(), &r, DT_LEFT | DT_WORD_ELLIPSIS); // ����
	if (pos == 1) drawtext(content.c_str(), &r, DT_CENTER | DT_WORD_ELLIPSIS); // ����
	if (pos == 2) drawtext(content.c_str(), &r, DT_RIGHT | DT_WORD_ELLIPSIS); // ����
	if (pos == 3) drawtext(content.c_str(), &r, DT_LEFT | DT_WORDBREAK); // ���󣬵��ǻ���
	if (pos == 4) drawtext(content.c_str(), &r, DT_CENTER | DT_WORDBREAK); // ���У����ǻ���
	if (pos == 5) drawtext(content.c_str(), &r, DT_RIGHT | DT_WORDBREAK); // ���ң����ǻ���
	Font.lfHeight = h * ResolutionRatio;
	Font.lfWeight = 0;
	settextstyle(&Font);
}

void DrawTextsBond(int x, int y, int width, int height, string content, int pos, LONG size, bool bond) {
	COLORREF color = gettextcolor();
	settextcolor(BLACK);
	DrawTexts(x + 1, y + 1, width, height, content, pos, size, bond);
	settextcolor(color);
	DrawTexts(x, y, width, height, content, pos, size, bond);
}

void OpaqueTexts(int x, int y, int width, int height, COLORREF color, string content, int pos, LONG size, bool bond, int opacity) {
	// ����������Ϊ��ʱ���õ�
	IMAGE txtimg(width * ResolutionRatio, height * ResolutionRatio);
	SetWorkingImage(&txtimg);
	int h = Font.lfHeight * ResolutionRatio;
	Font.lfHeight = size * ResolutionRatio;
	if (bond) Font.lfWeight = 600;
	settextstyle(&Font);
	settextcolor(color);
	setbkmode(TRANSPARENT);
	// �������
	RECT r = { 0, 0, width * ResolutionRatio, height * ResolutionRatio };
	if (pos == 0) drawtext(content.c_str(), &r, DT_LEFT | DT_WORD_ELLIPSIS);
	if (pos == 1) drawtext(content.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
	if (pos == 2) drawtext(content.c_str(), &r, DT_RIGHT | DT_WORD_ELLIPSIS);
	if (pos == 3) drawtext(content.c_str(), &r, DT_LEFT | DT_WORDBREAK);
	if (pos == 4) drawtext(content.c_str(), &r, DT_CENTER | DT_WORDBREAK);
	if (pos == 5) drawtext(content.c_str(), &r, DT_RIGHT | DT_WORDBREAK);
	Font.lfHeight = h / ResolutionRatio;
	Font.lfWeight = 0;
	settextstyle(&Font);
	settextcolor(WHITE);
	// ����Ĭ�Ϲ�����
	SetWorkingImage(NULL);
	BYTE R, G, B;
	DWORD* P = GetImageBuffer(&txtimg);
	for (int i = width * height * ResolutionRatio * ResolutionRatio - 1; i >= 0; --i) {
		R = GetRValue(P[i]);
		G = GetGValue(P[i]);
		B = GetBValue(P[i]);
		int A = (R == 0 && G == 0 && B == 0) ? 0 : 255;
		P[i] = A << 24 | RGB(R, G, B);
	}
	OpaqueImage(nullptr, x, y, &txtimg, opacity);
}

void PlaySE(string file) {
	char cmd[512];
	sprintf_s(cmd, "resource\\sound\\%s.wav", file.c_str());
	HSTREAM SE = 0;
	::BASS_Init(-1, 44100, 0, 0, 0);
	SE = ::BASS_StreamCreateFile(FALSE, cmd, 0, 0, NULL);
	BASS_ChannelSetAttribute(SE, BASS_ATTRIB_VOL, SEVolume);
	::BASS_ChannelPlay(SE, true);
}

void PlayBGM(string file) {
	file = "resource\\sound\\" + file;
	if (!ExistFile(file)) return;
	::BASS_ChannelStop(BGM);
	::BASS_Init(-1, 44100, 0, 0, 0);
	if (Split(file, ".")[1] == "mid") {
		BGM = ::BASS_MIDI_StreamCreateFile(FALSE, file.c_str(), 0, 0, NULL, NULL);
		BASS_MIDI_FONT SF = { BASS_MIDI_FontInit("ChoriumRevA.SF2",0), -1, 0 };
		BASS_MIDI_FontSetVolume(SF.font, BGMVolume);
		BASS_MIDI_StreamSetFonts(BGM, &SF, 1);
		BASS_ChannelSetAttribute(BGM, BASS_ATTRIB_MIDI_VOICES, 1000);
		HFX FX1 = BASS_ChannelSetFX(BGM, BASS_FX_DX8_REVERB, 1);
		BASS_DX8_REVERB REV = { 0,-5,1280,0.015 };
		BASS_FXSetParameters(FX1, &REV);
	}
	else {
		BGM = ::BASS_StreamCreateFile(FALSE, file.c_str(), 0, 0, NULL);
		BASS_ChannelSetAttribute(BGM, BASS_ATTRIB_VOL, BGMVolume);
	}
	::BASS_ChannelFlags(BGM, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);
	::BASS_ChannelPlay(BGM, true);
}

void BGMStop() {
	::BASS_ChannelStop(BGM);
}

GameSprite::GameSprite() {
	Name = "";
	X = 0;
	Y = 0;
	Z = 0;
	Width = 0;
	Height = 0;
	SrcX = 0;
	SrcY = 0;
	Angle = 0;
	Position = 0;
	Visible = true;
	Image = new IMAGE;
	Opacity = 255;
}

GameSprite::GameSprite(string name) : Name(name) {
	Name = name;
	X = 0;
	Y = 0;
	Z = 0;
	Width = 0;
	Height = 0;
	SrcX = 0;
	SrcY = 0;
	Angle = 0;
	Position = 0;
	Opacity = 255;
	Visible = true;
	Image = ImageCache[name];
	Graphics.Patterns.push_back(this);
}

GameSprite::GameSprite(string name, int opacity) : Name(name), Opacity(opacity) {
	X = 0;
	Y = 0;
	Z = 0;
	Width = 0;
	Height = 0;
	SrcX = 0;
	SrcY = 0;
	Angle = 0;
	Position = 0;
	Visible = true;
	Image = ImageCache[name];
	Graphics.Patterns.push_back(this);
}

GameSprite::GameSprite(string name, int x, int y, int opacity) : Name(name), X(x), Y(y), Opacity(opacity) {
	Z = 0;
	Width = 0;
	Height = 0;
	SrcX = 0;
	SrcY = 0;
	Angle = 0;
	Position = 0;
	Visible = true;
	Image = ImageCache[name];
	Graphics.Patterns.push_back(this);
}

int GameSprite::SpriteWidth() {
	if (Width == 0) return Image->getwidth();
	return Width;
}

int GameSprite::SpriteHeight() {
	if (Height == 0) return Image->getheight();
	return Height;
}

void GameSprite::ChangeImage(string name) {
	Name = name;
	delete Image;
	Image = new IMAGE;
	Image = ImageCache[name];
}

void GameSprite::AddIn() {
	Graphics.Patterns.push_back(this);
}

void GameSprite::Dispose() {
	auto pos = find(Graphics.Patterns.begin(), Graphics.Patterns.end(), this);
	Graphics.Patterns.erase(pos);
}

bool GameKeyBoard::LPress(int key) {
	if (GameActive != GetForegroundWindow()) return false;
	if (GetAsyncKeyState(key) != 0) {
		++hit_times;
		return true;
	}
	return false;
}

bool GameKeyBoard::Press(int key) {
	if (GameActive != GetForegroundWindow()) return false;
	if ((GetAsyncKeyState(key) & 0x8000) != 0) {
		++hit_times;
		return true;
	}
	return false;
}

bool GameKeyBoard::Repeat(int key) {
	if (GameActive != GetForegroundWindow()) return false;
	int result = GetAsyncKeyState(key) & 0x8000;
	if (result != 0) {
		++hit_times;
		if (key_repeat.find(key) == key_repeat.end()) {
			key_repeat[key] = 0;
			return true;
		}
		key_repeat[key]++;
	}
	else {
		key_repeat.erase(key);
		key_hash[key] = 0;
	}
	if (!(key_repeat.find(key) == key_repeat.end()) && key_repeat[key] > 2) {
		key_repeat[key] = 0;
		++hit_times;
		return true;
	}
	else
		return false;
}

bool GameKeyBoard::Trigger(int key) {
	if (GameActive != GetForegroundWindow()) return false;
	int result = GetAsyncKeyState(key) & 0x8000;
	if (key_hash[key] == 1 && result != 0) return false;
	if (result != 0) {
		++hit_times;
		key_hash[key] = 1;
		return true;
	}
	else {
		key_hash[key] = 0;
		return false;
	}
}

bool GameKeyBoard::PressConfirm() {
	return (Press(KeySPACE) || Press(KeyRETURN));
}

bool GameKeyBoard::RepeatConfirm() {
	return (Repeat(KeySPACE) || Repeat(KeyRETURN));
}

bool GameKeyBoard::TriggerConfirm() {
	return (Trigger(KeySPACE) || Trigger(KeyRETURN));
}

bool GameKeyBoard::PressCancel() {
	return (Press(KeyESCAPE) || Press(KeyX));
}

bool GameKeyBoard::RepeatCancel() {
	return (Repeat(KeyESCAPE) || Repeat(KeyX));
}

bool GameKeyBoard::TriggerCancel() {
	return (Trigger(KeyESCAPE) || Trigger(KeyX));
}

bool GameKeyBoard::DoubleClick(int key) {
	int result = Trigger(key);//(GetAsyncKeyState(key) & 0x8000) != 0;
	if (result == 0) return false;
	++hit_times;
	if (double_click[key] == 0) {
		double_click[key] = clock();
		return false;
	}
	else {
		if (clock() - double_click[key] <= 500) {
			double_click[key] = 0;
			return true;
		}
		else {
			double_click[key] = clock();
			return false;
		}
	}
}

int GameKeyBoard::Dir4() {
	// �ķ����ж�
	if (Press(KeyDOWN)) return 0;
	if (Press(KeyLEFT)) return 1;
	if (Press(KeyRIGHT)) return 2;
	if (Press(KeyUP)) return 3;
	return -1;
}

void DataSystem::BasicSet() {
	// ��ȡConfig������
	printf("��ȡ����ing\n");
	vector <string> dat = ReadCsv("Config.ini");
	TitleName = Split(dat[0], "=")[1] + ".png";
	FontName = Split(dat[1], "=")[1];
	WindowskinName = Split(dat[2], "=")[1] + ".png";
	WindowOpacity = stoi(Split(dat[3], "=")[1]);
	MapX = stoi(Split(dat[4], "=")[1]);
	MapY = stoi(Split(dat[5], "=")[1]);
	FrameRate = stoi(Split(dat[6], "=")[1]);
	GameBGM = Split(dat[7], "=")[1] + ".mp3";
	CursorSE = Split(dat[8], "=")[1];
	DecisionSE = Split(dat[9], "=")[1];
	CancelSE = Split(dat[10], "=")[1];
	BuzzerSE = Split(dat[11], "=")[1];
	ShopSE = Split(dat[12], "=")[1];
	SaveSE = Split(dat[13], "=")[1];
	LoadSE = Split(dat[14], "=")[1];
	GateSE = Split(dat[15], "=")[1];
	GetSE = Split(dat[16], "=")[1];
	StairSE = Split(dat[17], "=")[1];
	MoveSE = Split(dat[18], "=")[1];
	ResolutionRatio = stof(Split(dat[19], "=")[1]);
	BGMVolume = stof(Split(dat[20], "=")[1]) / 10.0;
	SEVolume = stof(Split(dat[21], "=")[1]) / 10.0;
	// ������Դ�ļ�
	printf("������Դ�ļ�ing\n");
	string inPath = "resource\\picture\\*.png";//�����ļ����µ�����.png�ļ�
	long handle;
	struct _finddata_t fileinfo;
	handle = _findfirst(inPath.c_str(), &fileinfo);
	do
	{
		ImageCache[fileinfo.name] = new IMAGE;
		loadimage(ImageCache[fileinfo.name], ("resource\\picture\\" + (string)fileinfo.name).c_str());
		if (ResolutionRatio != 1) {
			int w = ImageCache[fileinfo.name]->getwidth() * ResolutionRatio;
			int h = ImageCache[fileinfo.name]->getheight() * ResolutionRatio;
			delete ImageCache[fileinfo.name];
			ImageCache[fileinfo.name] = new IMAGE;
			loadimage(ImageCache[fileinfo.name], ("resource\\picture\\" + (string)fileinfo.name).c_str(), w, h, true);
		}
	} while (!_findnext(handle, &fileinfo));
	_findclose(handle);
	StairGraphic.ChangeImage("mting.png");
	StairGraphic.X = MapX;
	StairGraphic.Y = MapY;
	StairGraphic.Opacity = 0;
	StairGraphic.AddIn();
}

void DataSystem::DataLoad() {
	vector <string> dat;
	// ��ȡActors������
	dat = ReadCsv("library\\Actors.csv");
	for (int i = 1; i < dat.size(); ++i) {
		vector <string> dat2 = Split(dat[i], ",");
		GameActors temp;
		temp.Name = dat2[1];
		temp.File = dat2[2] + ".png";
		temp.Level = stoi(dat2[3]);
		temp.Hp = stoi(dat2[4]);
		temp.Atk = stoi(dat2[5]);
		temp.Def = stoi(dat2[6]);
		temp.Mdef = stoi(dat2[7]);
		temp.Breath = stoi(dat2[8]);
		temp.BreathLimit = stoi(dat2[9]);
		temp.Exp = stoi(dat2[10]);
		temp.Gold = stoi(dat2[11]);
		temp.MapID = stoi(dat2[12]);
		temp.X = stoi(dat2[13]);
		temp.Y = stoi(dat2[14]);
		temp.Status[0] = false;
		temp.Status[1] = false;
		memset(temp.Item, 0, sizeof(temp.Item));
		Actors.push_back(temp);
	}
	// ��ȡAnimations������
	dat = ReadCsv("library\\Animations.csv");
	for (int i = 1; i < dat.size(); ++i) {
		vector <string> dat2 = Split(dat[i], ",");
		GameAnimation temp;
		vector <string> dat3 = Split(dat2[2], "|");
		for (auto& c : dat3)
			c = c + ".png";
		temp.Pattern = dat3;
		temp.SEFile = dat2[3];
		temp.SETime = stoi(dat2[4]);
		Animation.push_back(temp);
	}
	// ��ȡEnemies������
	dat = ReadCsv("library\\Enemies.csv");
	for (int i = 1; i < dat.size(); ++i) {
		vector <string> dat2 = Split(dat[i], ",");
		GameEnemies temp;
		temp.Name = dat2[1];
		temp.File[0] = dat2[2] + ".png";
		temp.File[1] = dat2[3] + ".png";
		vector <string> dat3 = Split(dat2[4], "|");
		for (auto j : dat3)
			temp.Element.push_back(stoi(j));
		temp.Hp = stoi(dat2[5]);
		temp.Atk = stoi(dat2[6]);
		temp.Def = stoi(dat2[7]);
		temp.Breath = stoi(dat2[8]);
		temp.Conatk = stoi(dat2[9]);
		temp.Exp = stoi(dat2[10]);
		temp.Gold = stoi(dat2[11]);
		temp.AnimationID = stoi(dat2[12]);
		Enemies.push_back(temp);
	}
	// ��ȡItems������
	dat = ReadCsv("library\\Items.csv");
	for (int i = 1; i < dat.size(); ++i) {
		vector <string> dat2 = Split(dat[i], ",");
		GameItem temp;
		temp.Name = dat2[1];
		temp.File = dat2[2] + ".png";
		temp.Description = dat2[3];
		temp.Price = stoi(dat2[4]);
		temp.Usable = stoi(dat2[5]);
		temp.Cost = stoi(dat2[6]);
		Items.push_back(temp);
	}
	int id = 0;
	char mapname[128];
	while (true) {
		sprintf_s(mapname, "data\\map%03d.dat", id++);
		if (!ExistFile(mapname)) break;
		dat.clear();
		dat = ReadData(mapname);
		Maps.push_back(*dat.begin());
		DataMap dtmp;
		int evn = stoi(dat[1]);
		dtmp.MapName = dat[0];
		for (int i = 0; i < evn; ++i) {
			GameEvent ev;
			ev.ID = i;
			ev.Name = dat[2 + 9 * i];
			ev.File[0] = dat[3 + 9 * i] + ".png";
			ev.File[1] = dat[4 + 9 * i] + ".png";
			ev.X = stoi(dat[5 + 9 * i]);
			ev.Y = stoi(dat[6 + 9 * i]);
			ev.Through = stoi(dat[7 + 9 * i]);
			ev.TriggerCondition[0] = stoi(dat[8 + 9 * i]);
			ev.TriggerCondition[1] = stoi(dat[9 + 9 * i]);
			ev.TriggerCondition[2] = stoi(dat[10 + 9 * i]);
			dtmp.MapEvents.push_back(ev);
		}
		Map.push_back(dtmp);
	}
}

void DataSystem::Initialization() {
	Reload();
	GameActive = FindWindow(NULL, "Mota3");
	setbkmode(TRANSPARENT);
	Font.lfHeight = 22;
	Font.lfWeight = 500;
	Font.lfQuality = PROOF_QUALITY;
	_tcscpy_s(Font.lfFaceName, FontName.c_str());
	settextstyle(&Font);
}

void DataSystem::Reload() {
	if (!ExistFile("save\\achievement.dat")) {
		fstream file;
		file.open("save\\achievement.dat", ios::ate | ios::out);
		string s = "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
		file << s << endl;
		file.close();
	}
	vector <string> dat = ReadData("save\\achievement.dat");
	for (int i = 0; i < 36; ++i)
		Achievement[i] = stoi(dat[i]);
	Actors.clear();
	Enemies.clear();
	Items.clear();
	Animation.clear();
	FloorRecord.clear();
	EventRecord.clear();
	TransRecord.clear();
	AddRecord.clear();
	Map.clear();
	GameTime = 0;
	MessagePos = 2;
	BattleEnemyID = -1;
	TransformingEventName = "";
	memset(GameVariables, 0, sizeof(GameVariables));
	memset(GameSwitches, false, sizeof(GameSwitches));
	memset(ItemRecord, false, sizeof(ItemRecord));
	memset(GemRecord, false, sizeof(GemRecord));
	memset(BottleRecord, false, sizeof(BottleRecord));
	NeedRefresh = true;
	ScreenShot = false;
	EndGame = false;
	SetUp = true;
	Accelerate = false;
	ShotFile = "";
	MessageName.clear();
	MessageContent.clear();
	MessageHead.clear();
	DataLoad();
	Player.Step = 0;
	for (auto& dat : Actors) {
		dat.Status[0] = false;
		dat.Status[1] = false;
		memset(dat.Item, 0, sizeof(dat.Item));
	}
	Player = { 0, false, true };
}

void DataSystem::ReadyTransition() {
	TransitionGraphic = new IMAGE;
	getimage(TransitionGraphic, 0, 0, WindowWidth * ResolutionRatio, WindowHeight * ResolutionRatio);
	Transition = true;
}

void DataSystem::DoTransition(int time) {
	Transition = false;
	int reduce = ceil(255 * 1.0 / time);
	TransitionOpacity = 255;
	for (int i = 0; i < time; ++i) {
		TransitionOpacity = max(TransitionOpacity - reduce, 0);
		Graphics.Update();
	}
	delete TransitionGraphic;
}

void DataSystem::HitAchievement(int n) {
	if (Achievement[n]) return;
	Achievement[n] = true;
	WaitingAchievement.push_back(n);
}

GameEvent::GameEvent() {
	ID = 0;
	Name = "UNDEFINED";
	File[0] = "UNDEFINED";
	File[1] = "UNDEFINED";
	X = 0;
	Y = 0;
	TriggerCondition[0] = 0;
	TriggerCondition[1] = 0;
	TriggerCondition[2] = 0;
	Exist = true;
	Through = false;
}

GameEvent::GameEvent(int x, int y, int z) {
	GameEvent();
	X = x;
	Y = y;
}

const bool GameEvent::operator == (const GameEvent& ev) {
	return (X == ev.X && Y == ev.Y && Exist);
}

void GameEvent::Order() {
	// ��ȡ�¼�ID
	System.FunctioningEventID = System.Map[Actors[System.GameVariables[1]].MapID].CheckEvent(X, Y);
	// δ�ﵽ�����Ĳ��ɽ���
	if (TriggerCondition[0] != 0) {
		if (TriggerCondition[0] == 3 && System.GameSwitches[TriggerCondition[1]] != TriggerCondition[2]) return;
		if (TriggerCondition[0] == 4 && System.GameVariables[TriggerCondition[1]] != TriggerCondition[2]) return;
	}
	// ����|���ŷָ�
	vector <string> name = Split(Name, "|");
	int kind, val;
	// ·��
	if (name[0] == "BARRIER") {
		val = stoi(name[1]);
		int animeid = 36;
		// �б���ʱ����
		if (Actors[System.GameVariables[1]].Item[20] > 0) {
			val /= 5;
			animeid = 49;
			Exist = false;
			System.EventRecord[Actors[System.GameVariables[1]].MapID].insert(ID);
		}
		Actors[System.GameVariables[1]].Hp = max(Actors[System.GameVariables[1]].Hp - val, 0);
		Graphics.Animations.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, Animation[animeid] });
		Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, false, to_string(val), GameWindow::TextColor("red") });
		if (Actors[System.GameVariables[1]].Hp <= 0) {
			System.EndGame = true;
			return;
		}
		return;
	}
	// NPC
	if (name[0] == "NPC") {
		kind = stoi(name[1]);
		Dialogue(kind);
		return;
	}
	// ����
	if (name[0] == "TRANS") {
		kind = stoi(name[1]);
		Transmission(kind);
		return;
	}
	// ս��
	if (name[0] == "MONSTER") {
		System.BattleEnemyID = stoi(name[1]);
		return;
	}
	// �����ţ���ʵ���ǻ��ţ�
	if (name[0] == "SDOOR") {
		if (Actors[System.GameVariables[1]].Item[0] > 0) {
			PlaySE(System.GateSE);
			--Actors[System.GameVariables[1]].Item[0];
			OpenDoor();
			File[0] = "mting-243.png";
			File[1] = "mting-244.png";
			Through = true;
			Name = "TRANS|" + name[1];
			System.TransRecord[Actors[System.GameVariables[1]].MapID][ID] = "TRANS|" + name[1];
		}
		return;
	}
	// ����
	if (name[0] == "DOOR") {
		if (stoi(name[1]) == 0) {
			PlaySE(System.GateSE);
			OpenDoor();
			Exist = false;
			System.EventRecord[Actors[System.GameVariables[1]].MapID].insert(ID);
			return;
		}
		if (stoi(name[1]) == 1) {
			if (Actors[System.GameVariables[1]].Item[0] > 0) {
				PlaySE(System.GateSE);
				--Actors[System.GameVariables[1]].Item[0];
				OpenDoor();
				Exist = false;
				System.EventRecord[Actors[System.GameVariables[1]].MapID].insert(ID);
			}
			return;
		}
		if (stoi(name[1]) == 2) {
			if (Actors[System.GameVariables[1]].Item[1] > 0) {
				PlaySE(System.GateSE);
				--Actors[System.GameVariables[1]].Item[1];
				OpenDoor();
				Exist = false;
				System.EventRecord[Actors[System.GameVariables[1]].MapID].insert(ID);
			}
			return;
		}
		if (stoi(name[1]) == 3) {
			if (Actors[System.GameVariables[1]].Item[2] > 0) {
				PlaySE(System.GateSE);
				--Actors[System.GameVariables[1]].Item[2];
				OpenDoor();
				Exist = false;
				System.EventRecord[Actors[System.GameVariables[1]].MapID].insert(ID);
			}
			return;
		}
		if (stoi(name[1]) == 4) { // ʥˮ��
			if (Player.Direction == 1) {
				PlaySE(System.GateSE);
				--Actors[System.GameVariables[1]].Item[2];
				OpenDoor();
				Exist = false;
				System.EventRecord[Actors[System.GameVariables[1]].MapID].insert(ID);
			}
			else {
				Dialogue(17);
			}
			return;
		}
	}
	// ��ʯ��
	if (name[0] == "GEM") {
		PlaySE(System.GetSE);
		kind = stoi(name[1]);
		val = 1;
		string type[] = { "�챦ʯ","����ʯ","�̱�ʯ","Ծ��֮��","����ҩˮ","��Ϣҩˮ","Կ�׺�","����", "ʥˮ", "�ⶾ��", "���", "�Ʊ�ʯ"};
		string desc[] = { "��������","��������","ħ������","�ȼ�����","��������","��Ϣ���޼���","��Կ������","�������", "��������", "����ж�״̬", "���˥��״̬", "��Ϣ����"};
		if (name.size() == 3) val = stoi(name[2]);
		if (kind == 1) {
			Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, true, "ATK+" + to_string(val), GameWindow::TextColor("yellow") });
			Actors[System.GameVariables[1]].Atk += val;
		}
		if (kind == 2) {
			Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, true, "DEF+" + to_string(val), GameWindow::TextColor("yellow") });
			Actors[System.GameVariables[1]].Def += val;
		}
		if (kind == 3) {
			Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, true, "MDEF+" + to_string(val), GameWindow::TextColor("yellow") });
			Actors[System.GameVariables[1]].Mdef += val;
		}
		if (kind == 4) {
			Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, true, "Lv+" + to_string(val), GameWindow::TextColor("yellow") });
			Actors[System.GameVariables[1]].Level += val;
			Actors[System.GameVariables[1]].Hp += val * 250;
			Actors[System.GameVariables[1]].Atk += val * 3;
			Actors[System.GameVariables[1]].Def += val * 3;
			if (val + Actors[System.GameVariables[1]].Level > 60) Actors[System.GameVariables[1]].BreathLimit += max(0, 60 - Actors[System.GameVariables[1]].Level) * 6;
			else Actors[System.GameVariables[1]].BreathLimit += val * 6;
		}
		if (kind == 5) {
			Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, true, "EXP+" + to_string(val), GameWindow::TextColor("yellow") });
			Actors[System.GameVariables[1]].Exp += val;
			if (Actors[System.GameVariables[1]].Exp >= 300) System.HitAchievement(18);
		}
		if (kind == 6) {
			Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, true, "BREATH LIMIT-" + to_string(val), GameWindow::TextColor("yellow") });
			Actors[System.GameVariables[1]].BreathLimit -= val * 6;
			if (Actors[System.GameVariables[1]].Breath + 1 >= Actors[System.GameVariables[1]].BreathLimit) Actors[System.GameVariables[1]].Breath = Actors[System.GameVariables[1]].BreathLimit - 1;
		}
		if (kind == 7) {
			Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, true, "ALL KEYS+" + to_string(val), GameWindow::TextColor("yellow") });
			Actors[System.GameVariables[1]].Item[0] += val;
			Actors[System.GameVariables[1]].Item[1] += val;
			Actors[System.GameVariables[1]].Item[2] += val;
			if (Actors[System.GameVariables[1]].Item[2] >= 15) System.HitAchievement(15);
		}
		if (kind == 8) {
			Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, true, "GOLD+" + to_string(val), GameWindow::TextColor("yellow") });
			Actors[System.GameVariables[1]].Gold += val;
			if (Actors[System.GameVariables[1]].Gold >= 1000) System.HitAchievement(19);
		}
		if (kind == 9) {
			Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, true, "Hp+" + to_string(Actors[System.GameVariables[1]].Hp), GameWindow::TextColor("yellow") });
			Actors[System.GameVariables[1]].Hp *= 2;
		}
		if (kind == 10) {
			Actors[System.GameVariables[1]].Status[0] = false;
			Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, true, "Hp+100", GameWindow::TextColor("yellow") });
			Actors[System.GameVariables[1]].Hp += 100;
		}
		if (kind == 11) {
			Actors[System.GameVariables[1]].Status[1] = false;
			System.GameVariables[4] = 0;
			Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, true, "Hp+100", GameWindow::TextColor("yellow") });
			Actors[System.GameVariables[1]].Hp += 100;
		}
		if (kind == 12) {
			Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, true, "BREATH+" + to_string(val), GameWindow::TextColor("yellow") });
			Actors[System.GameVariables[1]].Breath += val * Actors[System.GameVariables[1]].BreathLimit / 6;
			if (Actors[System.GameVariables[1]].Breath + 1 >= Actors[System.GameVariables[1]].BreathLimit) 
				Actors[System.GameVariables[1]].Breath = Actors[System.GameVariables[1]].BreathLimit - 1;
		}
		// �ɾ��ж�
		if (Actors[System.GameVariables[1]].Atk >= 2000) System.HitAchievement(6);
		if (Actors[System.GameVariables[1]].Def >= 2000) System.HitAchievement(7);
		if (Actors[System.GameVariables[1]].Level >= 135) System.HitAchievement(8);
		if (Actors[System.GameVariables[1]].Item[2] >= 15) System.HitAchievement(15);
		if (kind == 9) System.HitAchievement(17);
		// �״λ����Ʒ���Ի����������
		if (!System.GemRecord[kind][val / (kind == 8 ? 100 : 1)]) {
			System.GemRecord[kind][val / (kind == 8 ? 100 : 1)] = true;
			AddDialogue(-1, "", "��á�" + type[kind - 1] + "����" + desc[kind - 1] + (name.size() == 2 ? "" : to_string(val)));
		}
		Exist = false;
		System.EventRecord[Actors[System.GameVariables[1]].MapID].insert(ID);
		return;
	}
	// Ѫƿ
	if (name[0] == "BOTTLE") {
		PlaySE(System.GetSE);
		val = stoi(name[1]);
		Actors[System.GameVariables[1]].Hp += val;
		Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, true, "HP+" + to_string(val), GameWindow::TextColor("green") });
		// �ɾ��ж�
		if (Actors[System.GameVariables[1]].Hp >= 5000) System.HitAchievement(4);
		if (Actors[System.GameVariables[1]].Hp >= 15000) System.HitAchievement(5);
		// �״λ����Ʒ����
		if (!System.BottleRecord[val]) {
			System.BottleRecord[val] = true;
			AddDialogue(-1, "", "��á���ҩ��������+" + to_string(val));
		}
		Exist = false;
		System.EventRecord[Actors[System.GameVariables[1]].MapID].insert(ID);
		return;
	}
	// ��Ʒ
	if (name[0] == "ITEM") {
		PlaySE(System.GetSE);
		kind = stoi(name[1]);
		val = stoi(name[2]);
		string content = "��á�";
		content += Items[kind].Name + "��";
		if (val != 1) content += "��" + to_string(val);
		content += "��";
		content += Items[kind].Description;
		if (!System.ItemRecord[kind] || val != 1) {
			System.ItemRecord[kind] = true;
			System.MessageName.push_back("");
			System.MessageContent.push_back(content);
			System.MessageHead.push_back(-1);
		}
		if ((kind == 17 || kind == 18) && Actors[System.GameVariables[1]].Item[5] == 0) {
			if (kind == 17) {
				Actors[System.GameVariables[1]].Status[0] = false;
				Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, true, "Hp+180", GameWindow::TextColor("yellow") });
				Actors[System.GameVariables[1]].Hp += 180;
			}
			if (kind == 18) {
				Actors[System.GameVariables[1]].Status[1] = false;
				System.GameVariables[4] = 0;
				Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, true, "Hp+380", GameWindow::TextColor("yellow") });
				Actors[System.GameVariables[1]].Hp += 380;
			}
		}
		else
			Actors[System.GameVariables[1]].Item[kind] += val;
		// �ɾ��ж�
		if (kind == 3) System.HitAchievement(12);
		if (kind == 4) System.HitAchievement(13);
		if (kind == 5) System.HitAchievement(14);
		as(kind, 6, 10) {
			System.HitAchievement(20);
			++System.GameVariables[11];
			if (System.GameVariables[11] == 5) System.HitAchievement(22);
		}
		as(kind, 11, 15) {
			System.HitAchievement(21);
			++System.GameVariables[12];
			if (System.GameVariables[12] == 5) System.HitAchievement(23);
		}
		if (Actors[System.GameVariables[1]].Item[2] >= 10) System.HitAchievement(15);
		as(kind, 21, 23) System.GameSwitches[21] = true;
		as(kind, 24, 26) System.GameSwitches[22] = true;
		if (System.GameSwitches[21] && System.GameSwitches[22]) System.HitAchievement(35);
		Exist = false;
		System.EventRecord[Actors[System.GameVariables[1]].MapID].insert(ID);
		return;
	}
	// �̵�
	if (name[0] == "SHOP") {
		kind = stoi(name[1]);
		System.ShopType = kind;
		// �ɾ��ж�
		if (kind == 1) System.HitAchievement(25);
		if (kind == 2) System.HitAchievement(26);
		if (kind == 3) System.HitAchievement(27);
		return;
	}
	// ��¥
	if (name[0] == "UP") {
		PlaySE(System.StairSE);
		for (int i = 0; i < 5; ++i) {
			System.StairGraphic.Opacity += 51;
			Graphics.Update();
		}
		System.NeedRefresh = true;
		string temp = to_string(System.GameVariables[3]) + ":" + to_string(System.GameVariables[2] + 1);
		if (VectorFind(System.Maps, temp)) {
			++System.GameVariables[2];
			Actors[System.GameVariables[1]].MapID = find(System.Maps.begin(), System.Maps.end(), temp) - System.Maps.begin();
			Player.Direction = 0;
			System.SetUp = true;
			if (!VectorFind(System.FloorRecord[System.GameVariables[3]], System.GameVariables[2])) {
				System.FloorRecord[System.GameVariables[3]].push_back(System.GameVariables[2]);
				sort(System.FloorRecord[System.GameVariables[3]].begin(), System.FloorRecord[System.GameVariables[3]].end());
			}
		}
		else
			Print("No such a map!");
		System.HitAchievement(24);
		return;
	}
	// ��¥
	if (name[0] == "DOWN") {
		PlaySE(System.StairSE);
		System.NeedRefresh = true;
		for (int i = 0; i < 5; ++i) {
			System.StairGraphic.Opacity += 51;
			Graphics.Update();
		}
		string temp = to_string(System.GameVariables[3]) + ":" + to_string(System.GameVariables[2] - 1);
		if (VectorFind(System.Maps, temp)) {
			--System.GameVariables[2];
			Actors[System.GameVariables[1]].MapID = find(System.Maps.begin(), System.Maps.end(), temp) - System.Maps.begin();
			Player.Direction = 0;
			System.SetUp = true;
			if (!VectorFind(System.FloorRecord[System.GameVariables[3]], System.GameVariables[2])) {
				System.FloorRecord[System.GameVariables[3]].push_back(System.GameVariables[2]);
				sort(System.FloorRecord[System.GameVariables[3]].begin(), System.FloorRecord[System.GameVariables[3]].end());
			}
		}
		else
			Print("No such a map!");
		return;
	}
	return;
}

void GameEvent::OpenDoor() {
	string F1 = File[0], F2 = File[1];
	for (int i = 1; i < 4; ++i) {
		File[0] = Split(F1, ".")[0] + "_" + to_string(i) + ".png";
		File[1] = Split(F2, ".")[0] + "_" + to_string(i) + ".png";
		System.NeedRefresh = true;
		Graphics.Update();
	}
}

void GameEvent::Dialogue(int id) {
	vector <string> extrainfo;
	if (id == 0) System.TransformingEventName = "NPC|5";
	if (id == 3) extrainfo.push_back(to_string(Actors[System.GameVariables[1]].GotThreshold()));
	if (id == 5) {
		if (Actors[System.GameVariables[1]].Item[16] > 0) {
			--Actors[System.GameVariables[1]].Item[16];
			System.GameSwitches[17] = true;
			AddDialogue(ID, "����", \
"�ף����ʮ�ּܡ�\n\
�����������ع���Կ�ף�");
			AddDialogue(ID, "����", \
"û�뵽�������ع����Ѿ�ռ���ˣ��ٶ����Ǿ��ˡ�");
			AddDialogue(ID, "����", \
"�����ҾͰ����ǰ�����µ�ͨ·��");
		}
		else {
			AddDialogue(ID, "����", \
"���Ͱ���");
		}
		return;
	}
	if (id == 7) System.TransformingEventName = "SHOP|3";
	if (id == 9) extrainfo.push_back(to_string(((int)(Actors[System.GameVariables[1]].Level / 10) + 1) * 10));
	if (id == 10) System.TransformingEventName = "MONSTER|27|1|16|1";
	if (id == 11) { System.TransformingEventName = "ITEM|5|1"; System.DirectlyFunction = true; }
	if (id == 12) { Actors[System.GameVariables[1]].Item[19] += 3; System.ItemRecord[19] = true; System.GameSwitches[19] = true; System.TransformingEventName = "SHOP|4"; }
	if (id == 18) Exist = false;
	auto info = npc.NPCMessage(id, ID, extrainfo);
	for (auto sinfo : info) {
		auto [nid, name, content] = sinfo;
		AddDialogue(nid, name, content);
	}
}

void GameEvent::Transmission(int id) {
	PlaySE(File[0] == "mting-243.png" ? "mting-78" : System.StairSE);
	GameSprite stair("mting.png");
	stair.X = System.MapX;
	stair.Y = System.MapY;
	stair.Opacity = 0;
	for (int i = 0; i < 5; ++i) {
		stair.Opacity += 51;
		Graphics.Update();
	}
	Player.Direction = 0;
	auto [mpid, mpx, mpy] = trans.Transmission(id);
	Actors[System.GameVariables[1]].MapID = mpid;
	if (mpx != -1) Actors[System.GameVariables[1]].X = mpx;
	if (mpy != -1) Actors[System.GameVariables[1]].Y = mpy;
	if (id == 1) {
		if (!VectorFind(System.FloorRecord[1], 1)) {
			System.FloorRecord[1].push_back(1);
			sort(System.FloorRecord[1].begin(), System.FloorRecord[1].end());
		}
		System.GameVariables[3] = 1;
		System.GameVariables[2] = 1;
	}
	if (id == 2) {
		System.GameVariables[3] = 0;
		System.GameVariables[2] = -6;
	}
	if (id == 3)
		System.GameSwitches[3] = true;
	if (id == 4)
		System.GameSwitches[3] = false;
	if (id == 5) {
		if (!VectorFind(System.FloorRecord[1], 11)) {
			System.FloorRecord[1].push_back(11);
			sort(System.FloorRecord[1].begin(), System.FloorRecord[1].end());
		}
		System.GameVariables[3] = 1;
		System.GameVariables[2] = 11;
	}
	if (id == 6) {
		if (!VectorFind(System.FloorRecord[0], -16)) {
			System.FloorRecord[0].push_back(-16);
			sort(System.FloorRecord[0].begin(), System.FloorRecord[0].end());
		}
		System.GameVariables[3] = 0;
		System.GameVariables[2] = -16;
	}
	if (id == 7)
		System.GameVariables[3] = 3;
	if (id == 8)
		System.GameVariables[3] = 0;
	if (id == 9)
		System.GameVariables[3] = 2;
	if (id == 10)
		System.GameVariables[3] = 1;
	if (id == 11) {
		if (!VectorFind(System.FloorRecord[1], 15)) {
			System.FloorRecord[1].push_back(15);
			sort(System.FloorRecord[1].begin(), System.FloorRecord[1].end());
		}
		System.GameVariables[3] = 1;
		System.GameVariables[2] = 15;
	}
	if (id == 12) {
		if (!VectorFind(System.FloorRecord[0], -20)) {
			System.FloorRecord[1].push_back(-20);
			sort(System.FloorRecord[0].begin(), System.FloorRecord[0].end());
		}
		System.GameVariables[3] = 0;
		System.GameVariables[2] = -20;
	}
	if (id == 53)
		System.GameSwitches[3] = true;
	if (id == 54)
		System.GameSwitches[3] = false;
	for (int i = 0; i < 5; ++i)
		Graphics.Update();
	System.NeedRefresh = true;
	for (int i = 0; i < 5; ++i) {
		stair.Opacity -= 51;
		Graphics.Update();
	}
	stair.Dispose();
}

const bool DataMap::operator == (const DataMap& mp) {
	return (MapName == mp.MapName);
}

bool DataMap::HaveAnEvent(int x, int y) {
	for (auto ev : MapEvents) {
		if (ev.X == x && ev.Y == y && ev.Exist) {
			if (ev.TriggerCondition[0] != 0) {
				if (ev.TriggerCondition[0] == 1 && System.GameSwitches[ev.TriggerCondition[1]] != ev.TriggerCondition[2]) return false;
				if (ev.TriggerCondition[0] == 2 && System.GameVariables[ev.TriggerCondition[1]] != ev.TriggerCondition[2]) return false;
			}
			return true;
		}
	}
	return false;
}

bool DataMap::HaveAnEEvent(int x, int y) {
	for (auto ev : MapEvents)
		if (ev.X == x && ev.Y == y)
			return true;
	return false;
}

int DataMap::CheckEvent(int x, int y) {
	GameEvent temp;
	temp.X = x;
	temp.Y = y;
	return (find(MapEvents.begin(), MapEvents.end(), temp) - MapEvents.begin());
}

GameEvent* DataMap::ECheckEvent(int x, int y) {
	return (&System.Map[Actors[System.GameVariables[1]].MapID].MapEvents[System.Map[Actors[System.GameVariables[1]].MapID].CheckEvent(x, y)]);
}

pair <int, int> DataMap::HeAt(int id) {
	return { MapEvents[id].X, MapEvents[id].Y };
}

bool DataMap::Passible(int x, int y) {
	// ������ͼ����
	if (x > 10 || x < 0 || y > 10 || y < 0) return false;
	// �Ƿ����¼�
	if (HaveAnEvent(x, y)) {
		auto ev = ECheckEvent(x, y);
		if (Split(ev->Name, "|")[0] == "BARRIER") return true;
		return (ev->Through);
	}
	return true;
}

bool DataMap::Addable(int x, int y) {
	// ������ͼ����
	if (x > 10 || x < 0 || y > 10 || y < 0) return false;
	// �Ƿ����¼�
	if (HaveAnEvent(x, y)) {
		auto ev = ECheckEvent(x, y);
		// ·�ϲ����ٵ���
		if (Split(ev->Name, "|")[0] == "BARRIER") return false;
		// ���ܶ´�����
		if (Split(ev->Name, "|")[0] == "TRANS") return false;
		// ¥�ݲ����ٵ���
		if (ev->Name == "UP") return false;
		if (ev->Name == "DOWN") return false;
		return (ev->Through);
	}
	return true;
}

bool DataMap::HaveEventsAlong(pair <int, int> ev1, pair <int, int> ev2) {
	// ��ͬ�в�ͬ��
	if (ev1.first!= ev2.first && ev1.second != ev2.second) return true;
	// �غ���
	if (ev1.first == ev2.first && ev1.second == ev2.second) return false;
	// ͬ��
	if (ev1.first == ev2.first) {
		for (int i = min(ev1.second, ev2.second); i < max(ev1.second, ev2.second); ++i) {
			if (!System.Map[Actors[System.GameVariables[1]].MapID].Passible(ev1.first, i)) {
				if (i == ev1.second || i == ev2.second) continue;
				return true;
			}
		}
	}
	// ͬ��
	else if (ev1.second == ev2.second) {
		for (int i = min(ev1.first, ev2.first); i < max(ev1.first, ev2.first); ++i) {
			if (!System.Map[Actors[System.GameVariables[1]].MapID].Passible(i, ev1.second)) {
				if (i == ev1.first || i == ev2.first) continue;
				return true;
			}
		}
	}
	return false;
}

bool DataMap::Beside(pair <int, int> ev1, pair <int, int> ev2) {
	return ((ev1.first == ev2.first || ev1.second == ev2.second) && abs(ev1.first - ev2.first) <= 1 && abs(ev1.second - ev2.second) <= 1);
}

void DataMap::WaitCount(int times) {
	for (int i = 0; i < times; ++i)
		Graphics.Update();
}

GameWindow::GameWindow() {
	X = 0;
	Y = 0;
	Z = 0;
	Width = 32;
	Height = 32;
	Opacity = System.WindowOpacity;
	HasFunction = false;
	Visible = true;
}

GameWindow::GameWindow(int x, int y, int width, int height, int opacity) {
	X = x;
	Y = y;
	Z = 0;
	Width = width;
	Height = height;
	Opacity = opacity;
	Visible = true;
	Graphics.Windows.push_back(this);
}

void GameWindow::DrawRect(int x, int y, int width, int rectheight) {
	int cornerx[] = { 128, 158, 128, 158 };
	int cornery[] = { 64, 64, 94, 94 };
	int sidex[] = { 130, 130, 128,  158 };
	int sidey[] = { 64, 94, 66, 66 };
	int opacity = abs(135 - 15 * (System.GameTime % 16)) + 105;
	WindowRect(130, 66, cornerx, cornery, sidex, sidey, 28, 2, 28, opacity, x + X, y + Y, width, rectheight, true);
}

void GameWindow::WDrawTexts(int x, int y, int width, int height, string content, int pos, LONG size, bool bond) {
	DrawTexts(x + X + 16, y + Y + 16, width, height, content, pos, size, bond);
}

void GameWindow::WindowBitmap(string file, int x, int y, int opacity) {
	BitmapOpaque(file, x + X, y + Y, opacity);
}

void GameWindow::WindowBitmap(string file, int x, int y, int width, int height, int sx, int sy, int opacity) {
	BitmapOpaque(file, x + X, y + Y, width, height, sx, sy, opacity);
}

void GameWindow::WindowDrawArrow(int x, int y, int now, int page, int width, string pagestr) {
	if (now == 0) BitmapOpaque("mting-1240.png", X + x, Y + y, 16, 16, 0, 0);
	else BitmapOpaque("mting-1240.png", X + x, Y + y, 16, 16, 16, 0);
	if (now == page) BitmapOpaque("mting-1240.png", X + x + 24 + width, Y + y, 16, 16, 32, 0);
	else BitmapOpaque("mting-1240.png", X + x + 24 + width, Y + y, 16, 16, 48, 0);
	DrawTexts(X + x + 20, Y + y, width, 18, pagestr + " " + to_string(now + 1) + "/" + to_string(page + 1), 1, 18L);
}

COLORREF GameWindow::NormalColor() {
	return RGB(255, 255, 255);
}

COLORREF GameWindow::DisabledColor() {
	return RGB(128, 128, 128);
}

COLORREF GameWindow::SystemColor() {
	return RGB(192, 224, 255);
}

COLORREF GameWindow::CrisisColor() {
	return RGB(255, 255, 64);
}

COLORREF GameWindow::TextColor(string type) {
	if (type == "white") return RGB(255, 255, 255);
	if (type == "gray") return RGB(175, 175, 175);
	if (type == "black") return RGB(0, 0, 0);
	if (type == "red") return RGB(255, 50, 50);
	if (type == "yellow") return RGB(255, 255, 128);
	if (type == "orange") return RGB(255, 185, 25);
	if (type == "blue") return RGB(128, 255, 255);
	if (type == "green") return RGB(128, 255, 128);
	if (type == "pink") return RGB(255, 128, 255);
	return RGB(255, 255, 255);
}

void GameWindow::Dispose() {
	auto pos = find(Graphics.Windows.begin(), Graphics.Windows.end(), this);
	Graphics.Windows.erase(pos);
}

GameSelectWindow::GameSelectWindow() {
	Index = 0;
	RectHeight = 32;
	Active = true;
}

GameSelectWindow::GameSelectWindow(int width, int rectheight, vector <string> item) {
	X = 0;
	Y = 0;
	Z = 0;
	Width = width;
	Height = 32 + rectheight * item.size();
	Opacity = System.WindowOpacity;
	HasFunction = true;
	Active = true;
	RectHeight = rectheight;
	Items = item;
	Graphics.Windows.push_back(this);
}

void GameSelectWindow::Refresh() {
	// ������Ŀ
	for (int i = 0; i < Items.size(); ++i) {
		if (i == Index) DrawRect(16, 16 + RectHeight * i, Width - 32, RectHeight);
		DrawItem(i, NormalColor());
	}
	// ���ʱ������
	if (!Active) return;
	// �����ϼ�ʱ
	if (KeyBoard.Repeat(KeyBoard.KeyUP)) {
		if (Index > 0) {
			--Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
	// �����¼�ʱ
	if (KeyBoard.Repeat(KeyBoard.KeyDOWN)) {
		if (Index < Items.size() - 1) {
			++Index;
			PlaySE(System.CursorSE);
		}
		return;
	}
}

void GameSelectWindow::DrawItem(int index, COLORREF colour) {
	WDrawTexts(4, 4 + RectHeight * index, Width, RectHeight, Items[index]);
}

int GameActors::GotThreshold() {
	if (Level <= 10) return Level * 2;
	if (Level <= 20) return 10 + Level;
	if (Level <= 40) return 20 + Level / 2;
	if (Level <= 60) return 30 + Level / 4;
	return 45;
}

void GamePlayer::Update() {
	// �����в��ж�
	if (MovingCount > 0) return;
	// ���ݰ�����ȡ����
	int dir = KeyBoard.Dir4();
	Direction = dir >= 0 ? dir : Direction;
	if (dir == 0) {
		if (System.Map[Actors[System.GameVariables[1]].MapID].Passible(Actors[System.GameVariables[1]].X, Actors[System.GameVariables[1]].Y + 1)) {
			PlaySE(System.MoveSE);
			++Step;
			MovingCount = MovingCount < 2 ? 2 : 4;
			++Actors[System.GameVariables[1]].Y;
			ChangeStep();
			if (System.Map[Actors[System.GameVariables[1]].MapID].HaveAnEvent(Actors[System.GameVariables[1]].X, Actors[System.GameVariables[1]].Y)) {
				while (MovingCount > 0)
					Graphics.Update();
				System.Map[Actors[System.GameVariables[1]].MapID].ECheckEvent(Actors[System.GameVariables[1]].X, Actors[System.GameVariables[1]].Y)->Order();
			}
		}
		else {
			if (System.Map[Actors[System.GameVariables[1]].MapID].HaveAnEvent(Actors[System.GameVariables[1]].X, Actors[System.GameVariables[1]].Y + 1))
				System.Map[Actors[System.GameVariables[1]].MapID].ECheckEvent(Actors[System.GameVariables[1]].X, Actors[System.GameVariables[1]].Y + 1)->Order();
		}
	}
	else if (dir == 1) {
		if (System.Map[Actors[System.GameVariables[1]].MapID].Passible(Actors[System.GameVariables[1]].X - 1, Actors[System.GameVariables[1]].Y)) {
			PlaySE(System.MoveSE);
			++Step;
			MovingCount = MovingCount < 2 ? 2 : 4;
			--Actors[System.GameVariables[1]].X;
			ChangeStep();
			if (System.Map[Actors[System.GameVariables[1]].MapID].HaveAnEvent(Actors[System.GameVariables[1]].X, Actors[System.GameVariables[1]].Y)) {
				while (MovingCount > 0)
					Graphics.Update();
				System.Map[Actors[System.GameVariables[1]].MapID].ECheckEvent(Actors[System.GameVariables[1]].X, Actors[System.GameVariables[1]].Y)->Order();
			}
		}
		else {
			if (System.Map[Actors[System.GameVariables[1]].MapID].HaveAnEvent(Actors[System.GameVariables[1]].X - 1, Actors[System.GameVariables[1]].Y))
				System.Map[Actors[System.GameVariables[1]].MapID].ECheckEvent(Actors[System.GameVariables[1]].X - 1, Actors[System.GameVariables[1]].Y)->Order();
		}
	}
	else if (dir == 2) {
		if (System.Map[Actors[System.GameVariables[1]].MapID].Passible(Actors[System.GameVariables[1]].X + 1, Actors[System.GameVariables[1]].Y)) {
			PlaySE(System.MoveSE);
			++Step;
			MovingCount = MovingCount < 2 ? 2 : 4;
			++Actors[System.GameVariables[1]].X;
			ChangeStep();
			if (System.Map[Actors[System.GameVariables[1]].MapID].HaveAnEvent(Actors[System.GameVariables[1]].X, Actors[System.GameVariables[1]].Y)) {
				while (MovingCount > 0)
					Graphics.Update();
				System.Map[Actors[System.GameVariables[1]].MapID].ECheckEvent(Actors[System.GameVariables[1]].X, Actors[System.GameVariables[1]].Y)->Order();
			}
		}
		else {
			if (System.Map[Actors[System.GameVariables[1]].MapID].HaveAnEvent(Actors[System.GameVariables[1]].X + 1, Actors[System.GameVariables[1]].Y))
				System.Map[Actors[System.GameVariables[1]].MapID].ECheckEvent(Actors[System.GameVariables[1]].X + 1, Actors[System.GameVariables[1]].Y)->Order();
		}
	}
	else if (dir == 3) {
		if (System.Map[Actors[System.GameVariables[1]].MapID].Passible(Actors[System.GameVariables[1]].X, Actors[System.GameVariables[1]].Y - 1)) {
			PlaySE(System.MoveSE);
			++Step;
			MovingCount = MovingCount < 2 ? 2 : 4;
			--Actors[System.GameVariables[1]].Y;
			ChangeStep();
			if (System.Map[Actors[System.GameVariables[1]].MapID].HaveAnEvent(Actors[System.GameVariables[1]].X, Actors[System.GameVariables[1]].Y)) {
				while (MovingCount > 0)
					Graphics.Update();
				System.Map[Actors[System.GameVariables[1]].MapID].ECheckEvent(Actors[System.GameVariables[1]].X, Actors[System.GameVariables[1]].Y)->Order();
			}
		}
		else {
			if (System.Map[Actors[System.GameVariables[1]].MapID].HaveAnEvent(Actors[System.GameVariables[1]].X, Actors[System.GameVariables[1]].Y - 1))
				System.Map[Actors[System.GameVariables[1]].MapID].ECheckEvent(Actors[System.GameVariables[1]].X, Actors[System.GameVariables[1]].Y - 1)->Order();
		}
	}
	return;
}

void GamePlayer::ChangeStep() {
	// �л���¼
	int jiaji = 0;
	// �ж�״̬
	if (Actors[System.GameVariables[1]].Status[0]) {
		// �ɾ��ж�
		if (Step - System.GameVariables[6] >= 50) System.HitAchievement(28);
		// ��Ѫ�ж�
		Actors[System.GameVariables[1]].Hp = max(Actors[System.GameVariables[1]].Hp - 10, 0);
		if (Actors[System.GameVariables[1]].Hp <= 0) {
			System.EndGame = true;
			return;
		}
	}
	// ��ͼ�˺��ж�
	for (auto ev : System.Map[Actors[System.GameVariables[1]].MapID].MapEvents) {
		if (!ev.Exist) continue;
		auto name = Split(ev.Name, "|");
		if (name[0] == "MONSTER") {
			int id = stoi(name[1]);
			if (DataMap::Beside({ ev.X, ev.Y }, { Actors[System.GameVariables[1]].X, Actors[System.GameVariables[1]].Y })) {
				// ����
				if (Enemies[id].GotP(19)) { // ����
					int adam = max(0, Enemies[id].GotAtk() - (Enemies[id].GotP(6) ? 0 : Actors[System.GameVariables[1]].Def - System.GameVariables[4]));
					if (adam > 0) {
						Graphics.Animations.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, Animation[Enemies[id].AnimationID] });
						Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, false, to_string(adam), GameWindow::TextColor("red") });
						Actors[System.GameVariables[1]].Hp = max(Actors[System.GameVariables[1]].Hp - adam, 0);
						if (Enemies[id].GotP(1) && Actors[System.GameVariables[1]].Status[0] == false) { // �ж��ֵ�����Ҳ����
							Actors[System.GameVariables[1]].Status[0] = true;
							System.GameVariables[6] = Player.Step;
						}
						if (Enemies[id].GotP(2)) { // ˥���ֵ�����Ҳ��˥��
							Actors[System.GameVariables[1]].Status[1] = true;
							System.GameVariables[4] += stoi(Split(Enemies[id].Name, "|")[1]);
						}
						if (Actors[System.GameVariables[1]].Hp <= 0) {
							System.EndGame = true;
							return;
						}
					}
				}
				// �л�
				if (Enemies[id].GotP(20)) {
					++jiaji;
					if (jiaji == 2) {
						jiaji = 0;
						int adam = 2 * max(0, Enemies[id].GotAtk() - (Enemies[id].GotP(6) ? 0 : Actors[System.GameVariables[1]].Def - System.GameVariables[4]));
						if (adam > 0) {
							Graphics.Animations.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, Animation[Enemies[id].AnimationID] });
							Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, false, to_string(adam), GameWindow::TextColor("red") });
							Actors[System.GameVariables[1]].Hp = max(Actors[System.GameVariables[1]].Hp - adam, 0);
							if (Enemies[id].GotP(1) && Actors[System.GameVariables[1]].Status[0] == false) { // �ж��ֵļл�Ҳ����
								Actors[System.GameVariables[1]].Status[0] = true;
								System.GameVariables[6] = Player.Step;
							}
							if (Enemies[id].GotP(2)) { // ˥���ֵļл�Ҳ��˥��
								Actors[System.GameVariables[1]].Status[1] = true;
								System.GameVariables[4] += 2 * stoi(Split(Enemies[id].Name, "|")[1]);
							}
							if (Actors[System.GameVariables[1]].Hp <= 0) {
								System.EndGame = true;
								return;
							}
						}
					}
				}
			}
			if (Enemies[id].GotP(21)) {
				if (!DataMap::HaveEventsAlong({ Actors[System.GameVariables[1]].X,Actors[System.GameVariables[1]].Y }, { ev.X,ev.Y })) {
					int adam = max(0, Enemies[id].GotAtk() - (Enemies[id].GotP(6) ? 0 : Actors[System.GameVariables[1]].Def - System.GameVariables[4]));
					if (adam > 0) {
						Graphics.Animations.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, Animation[Enemies[id].AnimationID] });
						Graphics.Damages.push({ System.MapX + Actors[System.GameVariables[1]].X * 32 + 16, System.MapY + Actors[System.GameVariables[1]].Y * 32 + 16, 0, false, to_string(adam), GameWindow::TextColor("red") });
						Actors[System.GameVariables[1]].Hp = max(Actors[System.GameVariables[1]].Hp - adam, 0);
						if (Enemies[id].GotP(1) && Actors[System.GameVariables[1]].Status[0] == false) { // �ж��ֵĹ�Ҳ����
							Actors[System.GameVariables[1]].Status[0] = true;
							System.GameVariables[6] = Player.Step;
						}
						if (Enemies[id].GotP(2)) { // ˥���ֵĹ�Ҳ��˥��
							Actors[System.GameVariables[1]].Status[1] = true;
							System.GameVariables[4] += stoi(Split(Enemies[id].Name, "|")[1]);
						}
						if (Actors[System.GameVariables[1]].Hp <= 0) {
							System.EndGame = true;
							return;
						}
					}
				}
			}
		}

	}
}

const bool GameEnemies::operator < (GameEnemies& en) {
	if (GotDamage() != en.GotDamage()) return (GotDamage() < en.GotDamage());
	return (Name < en.Name);
}

pair <int, string> GameEnemies::GotDamage(int atk, int def) {
	int vampire = 0;
	int first = 0;
	int ma = atk == 0 ? Actors[System.GameVariables[1]].Atk - System.GameVariables[4] : atk;
	int md = def == 0 ? Actors[System.GameVariables[1]].Def - System.GameVariables[4] : def;
	int ehp = Hp;
	int ea = Atk;
	int ed = Def;
	// ���
	if (VectorFind(Element, 4) || VectorFind(Element, 30)) ed = max(ed, ma - 1);
	// �ȹ�
	if (VectorFind(Element, 5)) first = max(Conatk, 1);
	// ħ��
	if (VectorFind(Element, 6)) md = 0;
	// ����
	if (VectorFind(Element, 21)) ea += ea / 2;
	// ��Ѫ
	if (VectorFind(Element, 3)) vampire = (ea - md) * stoi(Split(Name, "|")[1]) / 100;
	// �¹�
	if (VectorFind(Element, 30))
		if (ma > ed)
			ea = ma - 1;
	// �����ٽ�ֵҲ�򲻹�
	if (ma + Actors[System.GameVariables[1]].GotThreshold() <= ed) return { 999999, "DIE" };
	// ֱ����ɱ���������ȹ��˺�
	if (ehp + vampire * first <= ma - ed) return { max((ea - md) * first,0), to_string(max((ea - md) * first,0)) };
	// ���㵥�غ��˺�
	int edam = ma > ed ? (ma - ed) : (ma + Actors[System.GameVariables[1]].GotThreshold() > ed ? 1 : 0);
	// �����˺�
	if (edam > vampire * max(1, Conatk)) {
		int eatimes = (ehp + vampire * first) / (edam - vampire * max(1, Conatk));
		if (eatimes == (ehp + vampire * first) * 1.0 / (edam - vampire * max(1, Conatk)))
			--eatimes;
		eatimes += first;
		eatimes = max(eatimes, 0);
		if (md >= ea + Actors[System.GameVariables[1]].GotThreshold() * 2) return { 0, "0" };
		int singledam = ea > md ? (ea - md) : 1;
		int res = max(0, singledam * max(1, Conatk) * eatimes);
		if (VectorFind(Element, 31) && eatimes > 1) res *= (QuickPow(2, eatimes) - 1) / eatimes;
		return { res, to_string(res) };
	}
	return { 999999, "DIE" };
}

int GameEnemies::GotAtk() {
	return GotP(30) ? (Actors[System.GameVariables[1]].Atk - System.GameVariables[4] > GotDef() ? Actors[System.GameVariables[1]].Atk - System.GameVariables[4] - 1 : Atk) : Atk;
}

int GameEnemies::GotDef() {
	return (GotP(4) || GotP(30)) ? max(Actors[System.GameVariables[1]].Atk - System.GameVariables[4] - 1, Def) : Def;
}

bool GameEnemies::GotP(int p) {
	return (VectorFind(Element, p));
}

int GameEnemies::GotCrisis() {
	if (Actors[System.GameVariables[1]].Atk + Actors[System.GameVariables[1]].GotThreshold() <= GotDef()) return (GotDef() + 1 - Actors[System.GameVariables[1]].GotThreshold());
	if (Actors[System.GameVariables[1]].Atk <= GotDef()) return (GotDef() + 1);
	if (Actors[System.GameVariables[1]].Atk >= Hp + GotDef()) return -2;
	if (GotP(4) || GotP(30)) return -1;
	return (ceil(Hp * 1.0 / max(ceil(Hp * 1.0 / (Actors[System.GameVariables[1]].Atk - System.GameVariables[4] - GotDef())) - 1, 0)) + GotDef() + System.GameVariables[4]);
}

pair <string, string> GameEnemies::EnemyElement(int elementid) {
	string name = "��ͨ", description = "û������Ч��";
	if (elementid == 1) {
		name = "�ж�";
		description = "����Ĺ����ḽ���ж�״̬";
	}
	if (elementid == 2) {
		name = "˥��";
		description = "����ĵ�һ�ֹ����ḽ��˥��״̬";
	}
	if (elementid == 3) {
		name = "��Ѫ";
		description = "��������˺����ָ��˺�ֵ" + Split(Name, "|")[1] + "%������";
	}
	if (elementid == 4) {
		name = "���";
		description = "����ķ����������ҷ�����-1";
	}
	if (elementid == 5) {
		name = "�ȹ�";
		description = "ս����ʼʱ�������ȹ���";
	}
	if (elementid == 6) {
		name = "ħ��";
		description = "ս��ʱ������Ĺ��������ҷ��ķ���";
	}
	if (elementid == 7) {
		name = to_string(Conatk) + "����";
		description = "����ÿ�غϽ���" + to_string(Conatk) + "�ι���";
	}
	if (elementid == 8) {
		name = "����";
		description = "ս\
�����������һ����";
	}
	if (elementid == 9) {
		name = "ճҺȭ";
		description = "ս��ʱ������ÿ�ι����������ҷ�" + Split(Name, "|")[1] + "��ƣ��";
	}
	if (elementid == 10) {
		name = "��ʯ";
		description = "����һ���󣬻��������" + Split(Name, "|")[1] + "�㹥��";
	}
	if (elementid == 11) {
		name = "ˮ����";
		description = "����һ���󣬻�ظ�����" + Split(Name, "|")[1] + "������";
	}
	if (elementid == 12) {
		name = "Թ��";
		description = "����һ���󣬻ή���ҷ�" + Split(Name, "|")[1] + "�����";
	}
	if (elementid == 13) {
		name = "��ɱ��";
		description = "����һ�������ƽʱ3�����˺�";
	}
	if (elementid == 14) {
		name = "��Ѫ";
		description = "����һ��ʱ����ظ�˫����Ѫ��������" + Split(Name, "|")[2] + "�㹥��";
	}
	if (elementid == 15) {
		name = "������";
		description = "����һ��ʱ��������ҷ���ǰ����Ϣ��";
	}
	if (elementid == 16) {
		name = "��˶";
		description = "�ܵ�����һ��ʱ���˺�����1/4���������һ��ʱ������һ����Ϣ";
	}
	if (elementid == 17) {
		name = "����";
		description = "����һ���󣬱���ս�����ҷ���ֻ�����һ���˺�";
	}
	if (elementid == 18) {
		name = "����";
		description = "����һ�����5���˺������ǻ���ʧ��ǰһ������";
	}
	if (elementid == 19) {
		name = "����";
		description = "��������Χʱ�����ܵ��乥��";
	}
	if (elementid == 20) {
		name = "�л�";
		description = "���������͹����м�ʱ�����ܵ����߹�ͬ����";
	}
	if (elementid == 21) {
		name = "����";
		description = "����ʱ���͸ù����м�û���谭�����ܵ����﹥������սʱ�乥��������50%";
	}
	if (elementid == 22) {
		name = "������";
		description = "���������һ������";
	}
	if (elementid == 23) {
		name = "������";
		description = "���������һ����飬��������������";
	}
	if (elementid == 24) {
		name = "��ħ��";
		description = "��������������Ϣ��ÿ�ι�������ҷ�����һ��ӡ�ǣ�����һ�����������һ�㣬��ӡ�ǵ����߲�ʱ�ᱬը������߱��˺�";
	}
	if (elementid == 25) {
		name = "����";
		description = "����һ���󣬻�����ҷ�һ����Ϣ";
	}
	if (elementid == 26) {
		name = "ʥ��";
		description = "����һ���󣬱���ս�����ҷ���ֻ�����һ���˺�������յ�ǰ��Ϣ��";
	}
	if (elementid == 27) {
		name = "а��";
		description = "�����Թ������ʱ���޷�ʹ��¥�㴫����";
	}
	if (elementid == 28) {
		name = "������";
		description = "����һ���󣬱��غϺ����������һ��";
	}
	if (elementid == 29) {
		name = "Ϸ��";
		description = "����һ���ύ���ҷ�������ֵ";
	}
	if (elementid == 30) {
		name = "ħ��";
		description = "�ҷ�������������������ʱ�����Ĺ��������������ҷ�����-1";
	}
	if (elementid == 31) {
		name = "��ŭ";
		description = "ÿ�ι����󹥻����ᷭ��";
	}
	if (elementid == 32) {
		name = "��Ԫ";
		description = "�ҷ�ӵ��һ��������Ϣʱ������ǰ���ȡһ����Ϣ������һ��";
	}
	if (elementid == 33) {
		name = "ѩ��";
		description = "��������Χ�Ÿ��Ϊ����";
	}
	if (elementid == 34) {
		name = "����";
		description = "��������Χ�Ÿ��Ϊ����";
	}
	if (elementid == 35) {
		name = "������";
		description = "�ҷ���һ��������Ϣʱ�˺�����50%������һ�����4���˺�";
	}
	if (elementid == 36) {
		name = "����";
		description = "��ͨ�����ḽ���ж�������һ������˥��";
	}
	if (elementid == 37) {
		name = "��";
		description = "����һ���ͽ������˺���ȶ��������˥��";
	}
	if (elementid == 38) {
		name = "ħ��";
		description = "��������»�õ�ƣ���ɶԷ��е�";
	}
	return { name, description };
}

SpriteSetMap::SpriteSetMap() {
	Active = false;
	ActorWidth = 0;
	ActorHeight = 0;
}

void SpriteSetMap::Initialization() {
	Active = true;
	ActorWidth = ImageCache[Actors[System.GameVariables[1]].File]->getwidth() / 4;
	ActorHeight = ImageCache[Actors[System.GameVariables[1]].File]->getheight() / 4;
}

void SpriteSetMap::MapStatus() {
	// û��״̬���Ͳ���ʾ
	if (!System.GameSwitches[1]) return;
	// ״̬������
	BitmapOpaque("mting-mapstatus.png", 0, 0);
	// ����ͼƬ���
	DrawTexts(0, 460, 640, 32, "��PRESS L��", 2, 20L);
	BitmapOpaque(Actors[System.GameVariables[1]].File, 4, 40, 32, 32, 0, 0);
	if (System.GameSwitches[3] || System.GameVariables[3] == 2 || System.GameVariables[3] == 3)
		DrawTexts(352, 6, 96, 20, System.Maps[Actors[System.GameVariables[1]].MapID], 1, 20L);
	else {
		if (System.GameVariables[3] == 0) {
			if (System.GameVariables[2] >= 0) DrawTexts(352, 6, 96, 20, "����", 0, 20L);
			else DrawTexts(352, 6, 96, 20, "����", 0, 20L);
		}
		if (System.GameVariables[3] == 1) DrawTexts(352, 6, 96, 20, "����", 0, 20L);
		DrawTexts(352, 6, 96, 20, "��", 2, 20L);
		DrawTexts(388, 6, 36, 20, to_string(abs(System.GameVariables[2])), 1, 20L);
	}
	string status = "";
	if (Actors[System.GameVariables[1]].Status[0]) status = status + "�ж�";
	if (Actors[System.GameVariables[1]].Status[1]) status = status + (status != "" ? "/" : "") + "˥��";
	DrawTexts(40, 40, 96, 24, status == "" ? "��������" : "��" + status + "��", 0, 18L);
	DrawTexts(40, 60, 96, 24, "Lv.");
	DrawTexts(8, 124, 64, 20, "����");
	DrawTexts(8, 144, 64, 20, "����");
	DrawTexts(8, 164, 64, 20, "����");
	DrawTexts(8, 184, 64, 20, "��Ϣ");
	DrawTexts(8, 224, 64, 20, "����");
	DrawTexts(8, 244, 64, 20, "���");
	DrawTexts(36, 60, 96, 20, to_string(Actors[System.GameVariables[1]].Level), 2, 20L);
	DrawTexts(36, 124, 96, 20, to_string(Actors[System.GameVariables[1]].Hp), 2, 20L);
	DrawTexts(36, 144, 96, 20, to_string(Actors[System.GameVariables[1]].Atk), 2, 20L);
	DrawTexts(36, 164, 96, 20, to_string(Actors[System.GameVariables[1]].Def), 2, 20L);
	if (Actors[System.GameVariables[1]].Status[1]) {
		settextcolor(GameWindow::TextColor("red"));
		DrawTexts(48, 144, 96, 20, "(-" + to_string(System.GameVariables[4]) + ")", 0, 14L);
		DrawTexts(48, 164, 96, 20, "(-" + to_string(System.GameVariables[4]) + ")", 0, 14L);
		settextcolor(GameWindow::TextColor("white"));
	}
	BitmapOpaque("mting-1226.png", 48, 204, 80, 4);
	BitmapOpaque("mting-1226.png", 48, 204, Actors[System.GameVariables[1]].Breath % (Actors[System.GameVariables[1]].BreathLimit / 6) * 80 / (Actors[System.GameVariables[1]].BreathLimit / 6), 4, 0, 4);
	string breathfile1[] = { "mting-1211.png", "mting-1212.png", "mting-1213.png" };
	string breathfile2[] = { "mting-1211_g.png", "mting-1212_g.png", "mting-1213_g.png" };
	for (int i = 0; i < 5; ++i)
		BitmapOpaque(Actors[System.GameVariables[1]].Breath >= Actors[System.GameVariables[1]].BreathLimit / 6 * (i + 1) ? "mting-1211.png" : "mting-1212.png", 48 + 14 * i, 188);
	DrawTexts(36, 224, 96, 20, to_string(Actors[System.GameVariables[1]].Exp), 2, 20L);
	DrawTexts(36, 244, 96, 20, to_string(Actors[System.GameVariables[1]].Gold), 2, 20L);
	vector < pair <int, int> > itm;
	for (int i = 0; i < Items.size(); ++i)
		if (System.ItemRecord[i] > 0)
			itm.push_back({ i, Actors[System.GameVariables[1]].Item[i] });
	for (int i = 0; i < itm.size(); ++i) {
		ResizeBitmapOpaque(Items[itm[i].first].File, 1 + 26 * (i % 6), 292 + 24 * (i / 6), 0.75, 128 + (itm[i].second > 0) * 127);
		if (Items[itm[i].first].Usable && itm[i].second > 0)
			DrawTexts(2 + 26 * (i % 6), 304 + 24 * (i / 6), 24, 20, to_string(itm[i].second), 2, 16L);
	}
}

void SpriteSetMap::Update() {
	// ����ˢ��
	if (System.NeedRefresh) {
		ActorWidth = ImageCache[Actors[System.GameVariables[1]].File]->getwidth() / 4;
		ActorHeight = ImageCache[Actors[System.GameVariables[1]].File]->getheight() / 4;
		System.NeedRefresh = false;
	}
	// ��ʾ״̬��
	MapStatus();
	// û��ʼ��Ϸ֮ǰ��ִ��
	if (!System.GameSwitches[5]) return;
	// ���Ƶذ�
	BitmapOpaque("Floor.png", System.MapX, System.MapY);
	// ���Ƶ�ͼ�¼�
	for (auto [i, mapev, act] = make_tuple(0, System.Map[Actors[System.GameVariables[1]].MapID].MapEvents, Actors[System.GameVariables[1]]); i < mapev.size(); ++i) {
		if (!mapev[i].Exist) continue;
		if (mapev[i].TriggerCondition[0] != 0) {
			if (mapev[i].TriggerCondition[0] == 1 && System.GameSwitches[mapev[i].TriggerCondition[1]] != mapev[i].TriggerCondition[2])
				continue;
			if (mapev[i].TriggerCondition[0] == 2 && System.GameVariables[mapev[i].TriggerCondition[1]] != mapev[i].TriggerCondition[2])
				continue;
		}
		int x = System.MapX + mapev[i].X * 32;
		int y = System.MapY + mapev[i].Y * 32;
		BitmapOpaque(mapev[i].File[System.GameTime / 6 % 2], x, y);
		// ��ͼ��ʾ�˺�
		if (auto namesplit = Split(mapev[i].Name, "|"); System.GameSwitches[4] && act.Item[3] > 0 && namesplit[0] == "MONSTER") {
			int eid = stoi(namesplit[1]);
			if (Enemies[eid].GotDamage().first < act.Hp / 8)
				settextcolor(GameWindow::TextColor("white"));
			else if (Enemies[eid].GotDamage().first < act.Hp / 4)
				settextcolor(GameWindow::TextColor("green"));
			else if (Enemies[eid].GotDamage().first < act.Hp / 2)
				settextcolor(GameWindow::TextColor("blue"));
			else if (Enemies[eid].GotDamage().first < act.Hp * 3 / 4)
				settextcolor(GameWindow::TextColor("yellow"));
			else if (Enemies[eid].GotDamage().first < act.Hp)
				settextcolor(GameWindow::TextColor("red"));
			else
				settextcolor(GameWindow::TextColor("black"));
			DrawTextsBond(x, y + 20, 32, 32, Enemies[eid].GotDamage().second, 0, 12L, true);
			settextcolor(GameWindow::TextColor("white"));
			if (Enemies[eid].GotCrisis() >= 0 && Actors[System.GameVariables[1]].Atk < Enemies[eid].GotCrisis())
				DrawTextsBond(x, y + 8, 32, 32, to_string(Enemies[eid].GotCrisis()), 0, 12L, true);
		}
	}
	// ����ɫ������ʾʱ����
	if (!Player.Visible) return;
	int x = 0, y = 0;
	int w = ActorWidth / ResolutionRatio;
	int h = ActorHeight / ResolutionRatio;
	int sx = max(0, Player.MovingCount - 1) * ActorWidth / ResolutionRatio;
	int sy = ActorHeight * Player.Direction / ResolutionRatio;
	if (Player.Direction == 0) {
		x = System.MapX + Actors[System.GameVariables[1]].X * 32;
		y = System.MapY + Actors[System.GameVariables[1]].Y * 32 - 8 * Player.MovingCount;
	}
	if (Player.Direction == 1) {
		x = System.MapX + Actors[System.GameVariables[1]].X * 32 + 8 * Player.MovingCount;
		y = System.MapY + Actors[System.GameVariables[1]].Y * 32;
	}
	if (Player.Direction == 2) {
		x = System.MapX + Actors[System.GameVariables[1]].X * 32 - 8 * Player.MovingCount;
		y = System.MapY + Actors[System.GameVariables[1]].Y * 32;
	}
	if (Player.Direction == 3) {
		x = System.MapX + Actors[System.GameVariables[1]].X * 32;
		y = System.MapY + Actors[System.GameVariables[1]].Y * 32 + 8 * Player.MovingCount;
	}
	BitmapOpaque(Actors[System.GameVariables[1]].File, x, y, w, h, sx, sy);
	if (Player.MovingCount > 0) --Player.MovingCount;
}

void SpriteSetMap::Dispose() {
	Active = false;
}

void GameGraphics::Update() {
	// ����
	cleardevice();
	// ��潥��
	if (System.Transition) {
		OpaqueImage(nullptr, 0, 0, System.TransitionGraphic, 255);
		FlushBatchDraw();
		++System.GameTime;
		Sleep(System.FrameRate / (System.Accelerate ? 4 : 1));
		return;
	}
	// ����ͼ
	if (MapPatterns.Active) MapPatterns.Update();
	for (int z = 0; z < 9; ++z) {
		// ��洰��
		if (!Windows.empty()) {
			for (auto win : Windows) {
				if (!win->Visible) continue;
				if (win->Z != z) continue;
				int cornerx[] = { 128, 176, 128, 176 };
				int cornery[] = { 0, 0, 48, 48 };
				int sidex[] = { 144, 144, 128, 176 };
				int sidey[] = { 0, 48, 16, 16 };
				WindowRect(0, 0, cornerx, cornery, sidex, sidey, 128, 16, 32, win->Opacity, win->X, win->Y, win->Width, win->Height, false);
				if (win->HasFunction) win->Refresh();
			}
		}
		// ���ͼ��
		if (!Patterns.empty()) {
			for (auto spr : Patterns) {
				if (!spr->Visible) continue;
				if (spr->Z != z) continue;
				if (spr->Width == 0 || spr->Height == 0) {
					if (spr->Position == 0) OpaqueImage(nullptr, spr->X, spr->Y, spr->Image, spr->Opacity);
					if (spr->Position == 1) OpaqueImage(nullptr, spr->X - spr->SpriteWidth() / 2, spr->Y - spr->SpriteHeight() / 2, spr->Image, spr->Opacity);
				}
				else
					OpaqueImage(nullptr, spr->X, spr->Y, spr->Width, spr->Height, spr->SrcX, spr->SrcY, spr->Image, spr->Opacity);
			}
		}
	}
	// ��涯��
	int index = 0;
	stack <SpriteAnimation> stk;
	while (!Animations.empty()) {
		SpriteAnimation temp = Animations.top();
		Animations.pop();
		BitmapCentre(temp.Type.Pattern[temp.CurrentFrame], temp.ActX, temp.ActY);
		if (temp.CurrentFrame == temp.Type.SETime) PlaySE(temp.Type.SEFile);
		++temp.CurrentFrame;
		if (temp.CurrentFrame < temp.Type.Pattern.size()) stk.push(temp);
	}
	if (!stk.empty()) swap(Animations, stk);
	stack <SpriteDamage> dstk;
	// ��浯������
	while (!Damages.empty()) {
		SpriteDamage temp = Damages.top();
		Damages.pop();
		if (temp.Damage == "0") continue;
		OpaqueTexts(temp.ActX + temp.CurrentFrame * 2 * !temp.Vertival, temp.ActY - temp.CurrentFrame * temp.Vertival, 20 * temp.Damage.size(), 20, temp.Color, temp.Damage, 0, 20L, true, 255 - 15 * temp.CurrentFrame);
		++temp.CurrentFrame;
		if (temp.CurrentFrame <= 17) dstk.push(temp);
	}
	if (!dstk.empty()) swap(Damages, dstk);
	if (System.TransitionOpacity > 0) OpaqueImage(nullptr, 0, 0, System.TransitionGraphic, System.TransitionOpacity);
	FlushBatchDraw();
	++System.GameTime;
	Sleep(System.FrameRate / (System.Accelerate ? 4 : 1));
}

void GameGraphics::Dispose() {
	cleardevice();
}
