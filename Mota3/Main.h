#pragma once
#include "FrameStruct.h"

/*
WindowMonsterBook
��������ֲ����
*/
class WindowMonsterBook :public GameWindow {
public:
	int Index;					// ѡ��
	int Page;					// ҳ��
	vector <int> FloorEnemies;	// ��ǰ���������

	// ���캯������ʼ������
	WindowMonsterBook(int x, int y);
	// ˢ��
	void Refresh();
private:
	int MaxPage;
};

/*
WindowEncyclopedia
���������ϸ��Ϣ����
*/
class WindowEncyclopedia :public GameWindow {
public:
	int eid; // ����id

	// ���캯������ʼ������
	WindowEncyclopedia(int x, int y);
	// ˢ��
	void Refresh();
};

/*
WindowMessage
����Ի����ڵ���
*/
class WindowMessage :public GameWindow {
public:
	// ���캯������ʼ������
	WindowMessage(int x, int y);
	// ˢ��
	void Refresh();
};

/*
WindowBattle
����ս�����ڵ���
*/
class WindowBattle :public GameWindow {
public:
	int aHp;			// �ҷ�����
	int aAtk;			// �ҷ�����
	int aDef;			// �ҷ�����
	int aFatigue;		// �ҷ�ƣ��
	int aBreath;		// �ҷ���Ϣ
	int eHp;			// �з�����
	int eAtk;			// �з�����
	int eDef;			// �з�����
	int eFatigue;		// �з�ƣ��
	int eBreath;		// �з���Ϣ
	int funckey;		// ִ�еĹ���
	int aVaryAtk;		// �ҷ����ٹ���
	int aVaryDef;		// �ҷ����ٷ���
	int eVaryAtk;		// �з����ٹ���
	int eVaryDef;		// �з����ٷ���
	int LastDamage;		// �ۼ��˺�
	int buff;			// buff����
	bool weaken;		// ����
	bool eweaken;		// �Է�����

	// ���캯������ʼ������
	WindowBattle(int x, int y);
	// ˢ��
	void Refresh();
};

/*
WindowGoldShop
�������̵����
*/
class WindowGoldShop :public GameWindow {
public:
	int ShopID;	// �̵�id
	int Index;	// ѡ��

	// ���캯������ʼ������
	WindowGoldShop(int x, int y);
	// ˢ��
	void Refresh();
};

/*
WindowEXPShop
�������̵����
*/
class WindowEXPShop :public GameWindow {
public:
	int ShopID;	// �̵�id
	int Index;	// ѡ��

	// ���캯������ʼ������
	WindowEXPShop(int x, int y);
	// ˢ��
	void Refresh();
};

/*
WindowKeyShop
����Կ���̵����
*/
class WindowKeyShop :public GameWindow {
public:
	int Index;	// ѡ��

	// ���캯������ʼ������
	WindowKeyShop(int x, int y);
	// ˢ��
	void Refresh();
};

/*
WindowSDoorShop
�����������̵����
*/
class WindowSDoorShop :public GameWindow {
public:
	int Index;	// ѡ��

	// ���캯������ʼ������
	WindowSDoorShop(int x, int y);
	// ˢ��
	void Refresh();
};

/*
WindowFloorFly
����¥�㴫��������
*/
class WindowFloorFly :public GameWindow {
public:
	int viewfloorid;	// ��ǰ¥����
	int viewmapid;		// ��ǰ��ͼ���
	bool update;		// �Ƿ����

	// ���캯������ʼ������
	WindowFloorFly(int x, int y);
	// ˢ��
	void Refresh();
	// ���ͼ��ָ��
	void Delete();
private:
	IMAGE* events[121][2];
	IMAGE* floor;
	void DrawSmallMap(int x, int y);
};

/*
WindowHelp
����������ڵ���
*/
class WindowHelp :public GameWindow {
public:
	int Page; // ҳ��

	// ���캯������ʼ������
	WindowHelp(int x, int y);
	// ˢ��
	void Refresh();
};

/*
WindowSkill
�����ܴ��ڵ���
*/
class WindowSkill :public GameWindow {
public:
	int Type, Index; // ҳ����ѡ��

	// ���캯������ʼ������
	WindowSkill(int x, int y);
	// ˢ��
	void Refresh();
};

/*
WindowCure
�����ҩ�������ڵ���
*/
class WindowCure :public GameWindow {
public:
	int Index; // ѡ��

	// ���캯������ʼ������
	WindowCure(int x, int y);
	// ˢ��
	void Refresh();
};

/*
WindowFileImage
������ʾ�浵�������
*/
class WindowFileImage :public GameWindow {
public:
	int fileid; // �浵id

	// ���캯������ʼ������
	WindowFileImage(int x, int y);
	// ˢ��
	void Refresh();
};

/*
WindowFileCursor
������ʾ�浵��Ŵ��ڵ���
*/
class WindowFileCursor :public GameWindow {
public:
	int Index;				// ��ǰѡ��
	int Page;				// ҳ��
	int RectHeight;			// ���θ߶�
	bool Active;			// �Ƿ�
	bool load;				// �Ƿ��Ƕ���
	vector <string> Items;	// ��Ŀ����

	// ���캯������ʼ������
	WindowFileCursor(int x, int y, int rectheight, vector <string> item);
	// ˢ��
	void Refresh();
	// ������Ŀ
	void DrawItem(int index, COLORREF colour);
};

/*
WindowAchievement
������ʾ�ɾͻ������
*/
class WindowAchievement :public GameWindow {
public:
	int Index;
	static vector < vector <string> > name, description;

	// ���캯������ʼ������
	WindowAchievement(int x, int y);
	// ˢ��
	void Refresh();
};
vector < vector <string> > WindowAchievement::name = { { "��ս���", "С�гɾ�", "���þ���", "���Ѳ���", "���", "Ѫվ", "������̬", "������̬", "һ����ʦ", "����Ϣ", "ү������", "С�ӣ�ʹ�㾢" }, { "ʶ������", "��������", "ְҵð�ռ�", "�ֳ���", "�ɥ��ǥ�ɥ�", "��ʥ�ļӻ�", "ѧ������", "��԰�ؽ�", "��ϰ����", "��������", "����ʦ", "�ܴ�ʦ" }, { "�������", "���꣬�����������", "���ˣ���������", "����ר��", "���߱���", "����ѧͽ", "��Ϊ�������ԡ�", "С��ţ��", "�����⻹ͦӲ��", "���侫ͨ", "�������", "��֮��" }};
vector < vector <string> > WindowAchievement::description = { { "��һ�λ�ɱ����", "ɱ��200", "�����쳣״̬", "ս��ʤ����ʣ������������200", "Ѫ���ﵽ5000", "Ѫ���ﵽ15000", "��������2000", "��������2000", "�ȼ�����135", "��Ϣȫ��", "ս����ƣ�ͳ���80", "ս����ʹ����ƣ�ͳ���80" }, { "��ù����ֲ�", "���¥�㴫����", "�õ�ð�յ��ߴ�", "��������10�Ѻ�Կ��", "��������16��������", "���ʥˮ", "���鳬��300", "��ҳ���1000", "��õ�һ��������", "��õ�һ����������", "�����������", "�������������" }, { "����ħ��", "����̰��֮���̵�", "����ս��֮���̵�", "�ȳ�����", "�ڵ����ж�״̬�����˳���50��", "��һ��װ������", "��һ��װ��������", "��һ��ʹ�ý���", "��һ��ʹ�÷�����", "ʹ�ù����еĽ���", "ʹ�ù����еķ�����", "���ˮ���������֮��֮һ" } };

/*
WindowGetAchievement
������ʾ��óɾͻ������
*/
class WindowGetAchievement :public GameWindow {
public:
	string name;

	WindowGetAchievement(int x, int y);
	void Refresh();
};

/*
WindowStart
�����ֶԻ��������
*/
class WindowStart :public GameWindow {
public:
	string name;
	int next;

	WindowStart(int x, int y);
	void Refresh();
};

/*
GameTitle
������⻭�����
*/
class GameTitle :public SceneBase {
public:
	// ������
	void Main();
	// ���»���
	void Update();
	void CommandNewGame();
	void CommandContinue();
	void CommandAchievement();
	void CommandExit();
private:
	GameSprite sprite, head1, head2;
	GameSelectWindow window = GameSelectWindow(160, 32, { "����Ϸ", "����", "�ɾ�", "�˳���Ϸ" });
	WindowFileImage filewindow = WindowFileImage(320, 216);
	WindowFileCursor cursorwindow = WindowFileCursor(176, 216, 20, { "", "", "", "", "", "" });
	GameSelectWindow achcursurwindow = GameSelectWindow(128, 32, { "ս��ר��","��;�ĲƸ�","ð�յĻ���" });
	WindowAchievement achwindow = WindowAchievement(312, 144);
};

/*
GameMap
������Ϸ��ͼ����
*/
class GameMap :public SceneBase {
public:
	// ������
	void Main();
	// ���»���
	void Update();
	// ����
	static void LoadData(int fileid);
	// �浵
	static void SaveData(int fileid);
private:
	string record = "";
	bool downward = true;
	int waitcount = 0;
	WindowMessage messagewindow = WindowMessage(System.MapX + 96, System.MapY + 224);
	WindowMonsterBook monsterbookwindow = WindowMonsterBook(System.MapX, System.MapY);
	WindowEncyclopedia encyclopediawindow = WindowEncyclopedia(System.MapX, System.MapY);
	WindowBattle battlewindow = WindowBattle(System.MapX - 16, System.MapY + 64);
	WindowGoldShop goldshopwindow = WindowGoldShop(System.MapX + 64, System.MapY + 32);
	WindowEXPShop expshopwindow = WindowEXPShop(System.MapX + 64, System.MapY + 32);
	WindowKeyShop keyshopwindow = WindowKeyShop(System.MapX + 64, System.MapY + 32);
	WindowSDoorShop sdoorshopwindow = WindowSDoorShop(System.MapX + 64, System.MapY + 32);
	WindowFloorFly flywindow = WindowFloorFly(System.MapX + 96, System.MapY + 32);
	WindowFileImage filewindow = WindowFileImage(System.MapX + 160, System.MapY + 96);
	WindowFileCursor cursorwindow = WindowFileCursor(System.MapX + 16, System.MapY + 96, 20, { "", "", "", "", "", "" });
	WindowHelp helpwindow = WindowHelp(System.MapX, System.MapY);
	WindowSkill skillwindow = WindowSkill(System.MapX + 16, System.MapY + 80);
	WindowCure curewindow = WindowCure(System.MapX + 16, System.MapY + 96);
	WindowGetAchievement getachwindow = WindowGetAchievement(640, 32);
	WindowStart startwindow = WindowStart(System.MapX, System.MapY);

	// ��ݼ�����
	void ShortcutKey();
	// ��Ϣ����
	void MessageUpdate();
	// ��Ϸ��������
	void GameOver();
	// �����ֲᴦ��
	void MonsterBookUpdate();
	// �浵����
	void FileUpdate();
	// ս������
	void BattleFunction();
	int BattleUpdate();
	// �̵괦��
	void ShopUpdate();
	// ���õ�ͼ
	void SetUp();
	void AddRecord();
};

/*
main
�����������󣬴����￪ʼʵ�ʴ���
*/

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
