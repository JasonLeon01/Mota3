#pragma once
#include "DataInfo.h"

#ifndef __FrameStruct_H__
#define __Basic_H__

/*
��ӶԻ���Ŀ
name	�Ի������֣�""Ϊ�����Ʒ��
head	�Ի��¼��ţ�-1Ϊ���ǣ�
content	�Ի�����
*/
void AddDialogue(int head, string name, string content);

/*
���ƴ��ھ��Σ��������ھ��κ�ѡ����Σ�
backx		���α�������x����
backy		���α�������y����
cornerx		�����ĸ�������x����
cornery		�����ĸ�������y����
sidex		��������������x����
sidey		��������������y����
backlen		���α�������
cornerlen	���νǳ���
sidelen		���α߳���
opacity		��͸����
drawx		���Ƶ�x����
drawy		���Ƶ�y����
drawwidth	���ƵĿ�
drawheight	���Ƶĸ�
select		�Ƿ��Ǵ�ѡ��Ĵ���
*/
void WindowRect(int backx, int backy, int cornerx[], int cornery[], int sidex[], int sidey[], int backlen, int cornerlen, int sidelen, int opacity, int drawx, int drawy, int drawwidth, int drawheight, bool select);

/*
��������
x		���Ƶ�x����
y		���Ƶ�y����
width	���Ƶ����־��ο��
height	���Ƶ����־��θ߶�
content	���Ƶ���������
pos		�����ھ����е�λ�ã�0 ���룻1 ���У�2 �ҿ��룻3���룬���һ���
size	�����ֺ�
bond	�Ƿ�Ӵ�
*/
void DrawTexts(int x, int y, int width, int height, string content, int pos = 0, LONG size = 22, bool bond = false);

// ���ƴ���Ӱ������
void DrawTextsBond(int x, int y, int width, int height, string content, int pos = 0, LONG size = 22, bool bond = false);

// �������֣�����͸���ȣ�
void OpaqueTexts(int x, int y, int width, int height, COLORREF color, string content, int pos = 0, LONG size = 22, bool bond = false, int opacity = 255);

// ������Ч
void PlaySE(string file);

// ���ű�������
void PlayBGM(string file);

// ֹͣ��������
void BGMStop();

#endif

extern HWND GameActive;
extern LOGFONT Font;
extern float ResolutionRatio;
extern int WindowWidth;
extern int WindowHeight;
extern float BGMVolume;
extern float SEVolume;
extern map <string, IMAGE*> ImageCache;

// ���г�����Ļ��࣬���������麯��
class SceneBase {
public:
	virtual void Main() = 0;
};

/*
GameSprite
������Ϸ�������
ͨ��������Ի�ȡ������������
*/
class GameSprite {
public:
	string Name;						// ���ص�ͼƬ�ļ���
	int X;								// x����
	int Y;								// y����
	int Z;								// z����
	int Width;							// ��Ƭ��0Ϊ����Ƭ��
	int Height;							// ��Ƭ�ߣ�0Ϊ����Ƭ��
	int SrcX;							// ��Ƭ��ԭͼ�е�x����
	int SrcY;							// ��Ƭ��ԭͼ�е�y����
	int Angle;							// ��ת��
	int Position;						// ����ԭ�㣨0����1���У�
	int Opacity;						// ��͸����
	bool Visible;						// �Ƿ����
	IMAGE* Image;						// IMAGEָ��

	// ���캯������ʼ������
	GameSprite();
	GameSprite(string name);
	GameSprite(string name, int opacity);
	GameSprite(string name, int x, int y, int opacity);
	// ��þ�����
	int SpriteWidth();
	// ��þ���߶�
	int SpriteHeight();
	// �ı侫��ͼƬ�ļ���
	void ChangeImage(string name);
	// ���뵽Graphics������
	void AddIn();
	// �ͷ�
	void Dispose();
};

/*
GameKeyBoard
������Ϸ��������
*/
class GameKeyBoard {
public:
	// ȫ���̰����б�
	int KeyBACK = 0x08;
	int KeyTAB = 0x09;
	int KeyRETURN = 0x0D;
	int KeySHIFT = 0x10;
	int KeyCTRL = 0x11;
	int KeyALT = 0x12;
	int KeyPAUSE = 0x13;
	int KeyCAPITAL = 0x14;
	int KeyESCAPE = 0x1B;
	int KeySPACE = 0x20;
	int KeyPRIOR = 0x21;
	int KeyNEXT = 0x22;
	int KeyEND = 0x23;
	int KeyHOME = 0x24;
	int KeyLEFT = 0x25;
	int KeyUP = 0x26;
	int KeyRIGHT = 0x27;
	int KeyDOWN = 0x28;
	int KeySELECT = 0x29;
	int KeyPRINT = 0x2A;
	int KeySNAPSHOT = 0x2C;
	int INSERT = 0x2D;
	int KeyDELETE;
	int Key0 = 0x30;
	int Key1 = 0x31;
	int Key2 = 0x32;
	int Key3 = 0x33;
	int Key4 = 0x34;
	int Key5 = 0x35;
	int Key6 = 0x36;
	int Key7 = 0x37;
	int Key8 = 0x38;
	int Key9 = 0x39;
	int KeyA = 0x41;
	int KeyB = 0x42;
	int KeyC = 0x43;
	int KeyD = 0x44;
	int KeyE = 0x45;
	int KeyF = 0x46;
	int KeyG = 0x47;
	int KeyH = 0x48;
	int KeyI = 0x49;
	int KeyJ = 0x4A;
	int KeyK = 0x4B;
	int KeyL = 0x4C;
	int KeyM = 0x4D;
	int KeyN = 0x4E;
	int KeyO = 0x4F;
	int KeyP = 0x50;
	int KeyQ = 0x51;
	int KeyR = 0x52;
	int KeyS = 0x53;
	int KeyT = 0x54;
	int KeyU = 0x55;
	int KeyV = 0x56;
	int KeyW = 0x57;
	int KeyX = 0x58;
	int KeyY = 0x59;
	int KeyZ = 0x5A;

	// ���������������ж��Ƿ�˫����
	int hit_times = 0;
	// �ᰴ
	bool LPress(int key);
	// ����
	bool Press(int key);
	// ���°���
	bool Repeat(int key);
	// ��ס
	bool Trigger(int key);
	// ����ȷ��
	bool PressConfirm();
	// ���°���ȷ��
	bool RepeatConfirm();
	// ��סȷ��
	bool TriggerConfirm();
	// ����ȡ��
	bool PressCancel();
	// ���°���ȡ��
	bool RepeatCancel();
	// ��סȡ��
	bool TriggerCancel();
	// ˫��
	bool DoubleClick(int key);
	// �ķ����ж�
	int Dir4();
private:
	map <int, int> key_hash, key_repeat, double_click;
}; extern GameKeyBoard KeyBoard;

/*
GameEvent
�����¼�����
ͨ��������Ի�ȡ�¼������Լ������¼�����
*/
class GameEvent {
public:
	int ID;						// �¼�ID
	string Name;				// �¼���
	string File[2];				// �¼�ͼ��
	int X;						// �¼�x����
	int Y;						// �¼�y����
	int TriggerCondition[3];	// �¼��ĳ��ִ�����������һ�1 ���س��֣�2���������֣�3 ���ؿ�ִ�У�4 ������ִ�У��ڶ�����ػ������ţ���������ػ�����������ֵ
	bool Exist;					// �¼��Ƿ񻹴���
	bool Through;				// �¼��Ƿ�ɴ�͸

	//���캯������ʼ������
	GameEvent();
	GameEvent(int x, int y, int z);
	// �ж����¼��Ƿ���ȣ����ڲ�ѯ�¼�����ȵ�������xy������ͬ
	const bool operator == (const GameEvent& ev);
	// �¼�ִ�еĲ���
	void Order();
private:
	// ����
	void OpenDoor();
	// �Ի�
	void Dialogue(int id);
	// ����
	void Transmission(int id);
};

/*
DataMap
�����ͼ���ݵ���
ͨ��������Ի�ȡ��ͼ���Ժ͵�ͼ�е��¼�
�����ж����¼����Ƿ��ͨ�С��Ƿ������Լ��ȴ�����Ϊ��̬�����蹹�����󼴿ɵ���
*/
class DataMap {
public:
	string MapName;								// ��ͼ��
	vector <GameEvent> MapEvents;				// ��ͼ���¼�

	// �ж�����ͼ�Ƿ���ȣ���ȵ������ǵ�ͼ����ͬ
	const bool operator == (const DataMap& mp);
	// ��(x,y)���Ƿ�����¼�
	bool HaveAnEvent(int x, int y);
	// ��(x,y)���Ƿ�����¼�������ȥ��Ҳ�᷵�أ�
	bool HaveAnEEvent(int x, int y);
	// ������(x,y)�����¼����
	int CheckEvent(int x, int y);
	// ������(x,y)�����¼�ָ��
	GameEvent* ECheckEvent(int x, int y);
	// ָ��id�¼���λ��
	pair <int, int> HeAt(int id);
	// �ж���(x,y)���Ƿ����ͨ��
	bool Passible(int x, int y);
	// �ж���(x,y)�Ƿ���ԼӶ���
	bool Addable(int x, int y);
	// �ж����¼����Ƿ����ͨ��
	static bool HaveEventsAlong(pair <int, int> ev1, pair <int, int> ev2);
	// �ж����¼��Ƿ�����
	static bool Beside(pair <int, int> ev1, pair <int, int> ev2);
	// �ȴ�����λ֡��
	static void WaitCount(int times);
};

/*
DataSystem
������Ϸ���ݵ���
���Ա��������ص���Ϸ����
��ȡ���ݡ����¼������ݡ���ʼ�����ڴ�����
*/
class DataSystem {
public:
	map <int, vector <int> > FloorRecord;		// ��¼ȥ����¥��
	vector <int> WaitingAchievement;			// �ȴ���ʾ�ĳɾ�
	vector <DataMap> Map;						// ��ͼ
	vector <string> Maps;						// ��¼��ͼ��
	SceneBase* Scene;							// �¼�����
	IMAGE* TransitionGraphic;					// ׼������
	GameSprite StairGraphic;					// ����¥ͼ��
	long long GameTime = 0;						// ��Ϸʱ��
	int MessagePos;								// ����λ��
	int FrameRate;								// ˢ��Ƶ�ʣ���λms
	int WindowOpacity;							// ���ڵĲ�͸����
	int MapX;									// ��ͼ����Ϸ���ڵ�X����
	int MapY;									// ��ͼ����Ϸ���ڵ�Y����
	int BattleEnemyID;							// ���ڽ��е�ս���е���ID
	int FunctioningEventID;						// ���ڴ�����¼�ID
	int TransitionOpacity;						// ���ɲ�͸����
	int ShopType;								// �̵�����
	int GameVariables[5000];					// ����
	bool NeedRefresh;							// �Ƿ���Ҫˢ�µ�ͼ��ˢ�½������¼���ͼ��
	bool ScreenShot;							// �Ƿ���Ļ��ͼ
	bool DirectlyFunction;						// �¼��������Ƿ�ֱ��ִ��
	bool EndGame;								// ��Ϸ����
	bool Transition;							// ���ɱ�־
	bool GameSwitches[5000];					// ����
	bool ItemRecord[99];						// ��ȡ��Ʒ��¼
	bool GemRecord[99][99];						// ��ȡ��ʯ��¼
	bool BottleRecord[999];						// ��ȡѪƿ��¼
	bool Achievement[36];						// �ɾͼ�¼
	bool Victory;								// ����
	bool SetUp;									// ���õ�ͼ
	bool Accelerate;							// ����
	string WindowskinName;						// �����ļ���
	string TitleName;							// �����ļ���
	string FontName;							// ������
	string GameBGM;								// ��Ϸ��������
	string CursorSE;							// ѡ����Ч
	string DecisionSE;							// ȷ����Ч
	string CancelSE;							// ȡ����Ч
	string BuzzerSE;							// ������Ч
	string ShopSE;								// �̵���Ч
	string SaveSE;								// �浵��Ч
	string LoadSE;								// ������Ч
	string GateSE;								// ������Ч
	string GetSE;								// ��ȡ������Ч
	string StairSE;								// ����¥��Ч
	string MoveSE;								// �ƶ���Ч
	string ShotFile;							// ��Ļ��ͼ�ļ���
	string TransformingEventName;				// �¼�������ı��Ϊ������
	vector <string> MessageName;				// ������ʾ�Ľ�ɫ��
	vector <string> MessageContent;				// ������ʾ������
	vector <int> MessageHead;					// ������ʾ���¼�ID
	map < int, set <int> > EventRecord;			// �¼���¼
	map < int, map <int, string> > TransRecord;	// �����¼
	map <int, queue < tuple <int, int, string > > > AddRecord; // �����¼

	// ��ȡ����
	void BasicSet();
	void DataLoad();
	// ��Ϸ�����ʼ��
	void Initialization();
	// ���¼�������
	void Reload();
	// ׼������
	void ReadyTransition();
	// ��ʼ����
	void DoTransition(int time = 8);
	// �����ɾ�
	void HitAchievement(int n);
}; extern DataSystem System;

/*
GameWindow
������Ϸ���ڵĻ��࣬�������麯��Refresh
ͨ��������Ի�ȡ�������ԣ����ƾ����Լ��ڴ�����������ֺ�ͼ��
*/
class GameWindow {
public:
	int X;						// ����x����
	int Y;						// ����y����
	int Z;						// ����z����
	int Width;					// ���ڿ�
	int Height;					// ���ڸ�
	int Opacity;				// ���ڲ�͸����
	bool HasFunction;			// �Ƿ��й��ܣ�Ϊtrue����Զ�ִ��Refresh������
	bool Visible;				// �Ƿ����
	virtual void Refresh() = 0;	// ˢ�£����麯����

	// ���캯������ʼ������
	GameWindow();
	GameWindow(int x, int y, int width, int height, int opacity = System.WindowOpacity);
	// ���ƾ���
	void DrawRect(int x, int y, int width, int rectheight);
	// ���ƴ������֣�xyΪ0ʱ�����ڴ������Ͻǣ�
	void WDrawTexts(int x, int y, int width, int height, string content, int pos = 0, LONG size = 22, bool bond = false);
	// ���ƴ���ͼ�Σ�xyΪ0ʱ�����ڴ������Ͻǣ�
	void WindowBitmap(string file, int x, int y, int opacity = 255);
	void WindowBitmap(string file, int x, int y, int width, int height, int sx, int sy, int opacity = 255);
	// ���ƴ��ڼ�ͷ��xyΪ0ʱ�����ڴ������Ͻǣ�
	void WindowDrawArrow(int x, int y, int now, int page, int width, string pagestr = "PAGE");
	// ��ȡ��ͨ����ɫ
	static COLORREF NormalColor();
	// ��ȡ��Ч����ɫ
	static COLORREF DisabledColor();
	// ��ȡϵͳ����ɫ
	static COLORREF SystemColor();
	// ��ȡΣ������ɫ
	static COLORREF CrisisColor();
	// ��ȡ����ɫ
	static COLORREF TextColor(string type);
	// �ͷ�
	void Dispose();
};

/*
GameSelectWindow
�����ѡ����ڵ���
*/
class GameSelectWindow :public GameWindow {
public:
	int Index;				// ��ǰѡ��
	int RectHeight;			// ���θ߶�
	bool Active;			// �Ƿ�
	vector <string> Items;	// ��Ŀ��

	// ���캯������ʼ������
	GameSelectWindow();
	GameSelectWindow(int width, int rectheight, vector <string> item);
	// ˢ��
	void Refresh();
	// ������Ŀ
	void DrawItem(int index, COLORREF colour);
};

/*
GameActors
�洢��ɫ���ݵĽṹ��
*/
struct GameActors {
	string Name;		// ��ɫ��
	string File;		// ��ɫͼ���ļ���
	bool Status[2];		// ״̬
	int Level;			// �ȼ�
	int Hp;				// ����
	int Atk;			// ����
	int Def;			// ����
	int Mdef;			// ħ��
	int Breath;			// ��Ϣ
	int BreathLimit;	// ��Ϣ����
	int Exp;			// ����
	int Gold;			// ���
	int Item[99];		// ��Ʒ
	int MapID;			// ������ͼID
	int X;				// ����x����
	int Y;				// ����y����

	// ��ȡ�����ٽ�ֵ
	int GotThreshold();
}; extern vector <GameActors> Actors;

/*
GamePlayer
����Ŀǰ�ٿؽ�ɫ����
�������ߺͲ����仯�ķ������ڴ�����
*/
class GamePlayer {
public:
	int Direction;		// ����
	int MovingCount;	// �ƶ���ʱ
	bool Visible;		// �Ƿ����
	int Step;			// ����

	// ���£������ƶ���
	void Update();
	// �ı䲽��
	void ChangeStep();
}; extern GamePlayer Player;

/*
GameEnemies
������Ϸ���˵���
����ȷ�����˵ĸ������������Ժ��˺�
*/
class GameEnemies {
public:
	string Name;			// ����
	string File[2];			// �ļ���
	vector <int> Element;	// ����
	int Hp;					// ����
	int Atk;				// ����
	int Def;				// ����
	int Breath;				// ��Ϣ
	int Conatk;				// ����
	int Exp;				// ����
	int Gold;				// ���
	int AnimationID;		// �������

	// ����С�ںţ���������
	const bool operator < (GameEnemies& en);
	// ��ȡ�˺�
	pair <int, string> GotDamage(int atk = 0, int def = 0);
	// ��ȡ��ʵ����
	int GotAtk();
	// ��ȡ��ʵ����
	int GotDef();
	// �Ƿ�ӵ��ĳ����
	bool GotP(int p);
	// ��ȡ�ٽ�
	int GotCrisis();
	// ��ȡ�������Լ�������
	pair <string, string> EnemyElement(int elementid);
}; extern vector <GameEnemies> Enemies;

/*
GameItem
�洢��Ʒ���ݵĽṹ��
*/
struct GameItem {
	string Name;		// ����
	string File;		// �ļ���
	string Description;	// ����
	int Price;			// �ۼ�
	bool Usable;		// �Ƿ����ʹ��
	bool Cost;			// �Ƿ�����
}; extern vector <GameItem> Items;

/*
GameAnimation
�洢�������ݵĽṹ��
*/
struct GameAnimation {
	vector <string> Pattern;	// ͼ������
	string SEFile;				// ��Ч��
	int SETime;					// ִ����Ч��֡��
}; extern vector <GameAnimation> Animation;

/*
SpriteAnimation
�洢��ͼִ�ж������ݵĽṹ��
*/
struct SpriteAnimation {
	int ActX;			// ����x����
	int ActY;			// ����y����
	int CurrentFrame;	// Ŀǰִ�е���֡��
	GameAnimation Type;	// ִ�еĶ���
};

/*
SpriteDamage
�洢�˺��������ݵĽṹ��
*/
struct SpriteDamage {
	int ActX;			// ����x����
	int ActY;			// ����y����
	int CurrentFrame;	// ���ֵ�ǰ֡
	bool Vertival;		// ��ֱ����
	string Damage;		// ��������
	COLORREF Color;		// ����ɫ
};

/*
SpriteSetMap
�����ͼ��������
*/
class SpriteSetMap {
public:
	bool Active; // �Ƿ�
	// ���캯������ʼ������
	SpriteSetMap();
	// ��ʼ������
	void Initialization();
	// ״̬��
	void MapStatus();
	// ˢ��
	void Update();
	// �ͷ�
	void Dispose();
private:
	int ActorWidth;
	int ActorHeight;
};

/*
GameGraphics
�����ͼ���������ڸ��ºͶ������µ���
�����ֲ�Ϊ�ȿ�ʼ�Ķ��������ϲ�
���Ҫ����ִ�еĶ��������²㣬��stack��Ϊqueue
*/
class GameGraphics {
public:
	vector <GameSprite*> Patterns;		// ����ͼ������
	SpriteSetMap MapPatterns;			// ��ͼ���
	vector <GameWindow*> Windows;		// ��������
	stack <SpriteAnimation> Animations;	// �������У�����ջ�У�
	stack <SpriteDamage> Damages;		// �������У�����ջ�У�

	// ����
	void Update();
	// �ͷţ��������
	void Dispose();
}; extern GameGraphics Graphics;
