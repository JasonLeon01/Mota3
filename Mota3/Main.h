#pragma once
#include "FrameStruct.h"

/*
WindowMonsterBook
处理怪物手册的类
*/
class WindowMonsterBook :public GameWindow {
public:
	int Index;					// 选项
	int Page;					// 页数
	vector <int> FloorEnemies;	// 当前层敌人序列

	// 构造函数，初始化对象
	WindowMonsterBook(int x, int y);
	// 刷新
	void Refresh();
private:
	int MaxPage;
};

/*
WindowEncyclopedia
处理怪物详细信息的类
*/
class WindowEncyclopedia :public GameWindow {
public:
	int eid; // 敌人id

	// 构造函数，初始化对象
	WindowEncyclopedia(int x, int y);
	// 刷新
	void Refresh();
};

/*
WindowMessage
处理对话窗口的类
*/
class WindowMessage :public GameWindow {
public:
	// 构造函数，初始化对象
	WindowMessage(int x, int y);
	// 刷新
	void Refresh();
};

/*
WindowBattle
处理战斗窗口的类
*/
class WindowBattle :public GameWindow {
public:
	int aHp;			// 我方生命
	int aAtk;			// 我方攻击
	int aDef;			// 我方防御
	int aFatigue;		// 我方疲劳
	int aBreath;		// 我方气息
	int eHp;			// 敌方体力
	int eAtk;			// 敌方攻击
	int eDef;			// 敌方防御
	int eFatigue;		// 敌方疲劳
	int eBreath;		// 敌方气息
	int funckey;		// 执行的功能
	int aVaryAtk;		// 我方减少攻击
	int aVaryDef;		// 我方减少防御
	int eVaryAtk;		// 敌方减少攻击
	int eVaryDef;		// 敌方减少防御
	int LastDamage;		// 累计伤害
	int buff;			// buff层数
	bool weaken;		// 弱化
	bool eweaken;		// 对方弱化

	// 构造函数，初始化对象
	WindowBattle(int x, int y);
	// 刷新
	void Refresh();
};

/*
WindowGoldShop
处理金币商店的类
*/
class WindowGoldShop :public GameWindow {
public:
	int ShopID;	// 商店id
	int Index;	// 选项

	// 构造函数，初始化对象
	WindowGoldShop(int x, int y);
	// 刷新
	void Refresh();
};

/*
WindowEXPShop
处理经验商店的类
*/
class WindowEXPShop :public GameWindow {
public:
	int ShopID;	// 商店id
	int Index;	// 选项

	// 构造函数，初始化对象
	WindowEXPShop(int x, int y);
	// 刷新
	void Refresh();
};

/*
WindowKeyShop
处理钥匙商店的类
*/
class WindowKeyShop :public GameWindow {
public:
	int Index;	// 选项

	// 构造函数，初始化对象
	WindowKeyShop(int x, int y);
	// 刷新
	void Refresh();
};

/*
WindowSDoorShop
处理随意门商店的类
*/
class WindowSDoorShop :public GameWindow {
public:
	int Index;	// 选项

	// 构造函数，初始化对象
	WindowSDoorShop(int x, int y);
	// 刷新
	void Refresh();
};

/*
WindowFloorFly
处理楼层传送器的类
*/
class WindowFloorFly :public GameWindow {
public:
	int viewfloorid;	// 当前楼层编号
	int viewmapid;		// 当前地图编号
	bool update;		// 是否更新

	// 构造函数，初始化对象
	WindowFloorFly(int x, int y);
	// 刷新
	void Refresh();
	// 清除图形指针
	void Delete();
private:
	IMAGE* events[121][2];
	IMAGE* floor;
	void DrawSmallMap(int x, int y);
};

/*
WindowHelp
处理帮助窗口的类
*/
class WindowHelp :public GameWindow {
public:
	int Page; // 页数

	// 构造函数，初始化对象
	WindowHelp(int x, int y);
	// 刷新
	void Refresh();
};

/*
WindowSkill
处理技能窗口的类
*/
class WindowSkill :public GameWindow {
public:
	int Type, Index; // 页数和选项

	// 构造函数，初始化对象
	WindowSkill(int x, int y);
	// 刷新
	void Refresh();
};

/*
WindowCure
处理草药精华窗口的类
*/
class WindowCure :public GameWindow {
public:
	int Index; // 选项

	// 构造函数，初始化对象
	WindowCure(int x, int y);
	// 刷新
	void Refresh();
};

/*
WindowFileImage
处理显示存档画面的类
*/
class WindowFileImage :public GameWindow {
public:
	int fileid; // 存档id

	// 构造函数，初始化对象
	WindowFileImage(int x, int y);
	// 刷新
	void Refresh();
};

/*
WindowFileCursor
处理显示存档序号窗口的类
*/
class WindowFileCursor :public GameWindow {
public:
	int Index;				// 当前选项
	int Page;				// 页数
	int RectHeight;			// 矩形高度
	bool Active;			// 是否活动
	bool load;				// 是否是读档
	vector <string> Items;	// 项目序列

	// 构造函数，初始化对象
	WindowFileCursor(int x, int y, int rectheight, vector <string> item);
	// 刷新
	void Refresh();
	// 绘制项目
	void DrawItem(int index, COLORREF colour);
};

/*
WindowAchievement
处理显示成就画面的类
*/
class WindowAchievement :public GameWindow {
public:
	int Index;
	static vector < vector <string> > name, description;

	// 构造函数，初始化对象
	WindowAchievement(int x, int y);
	// 刷新
	void Refresh();
};
vector < vector <string> > WindowAchievement::name = { { "首战告捷", "小有成就", "不得劲啊", "大难不死", "肉盾", "血站", "攻击姿态", "防御姿态", "一代宗师", "凝神储息", "爷不打了", "小子，使点劲" }, { "识破你了", "尘世闲游", "职业冒险家", "持匙人", "ドコデモドア", "神圣的加护", "学有所成", "梦园藏金", "见习剑术", "防守入门", "剑大师", "盾大师" }, { "初入天地", "少年，你渴望力量吗", "凡人，臣服于我", "开锁专家", "边走边吐", "剑术学徒", "因为怕疼所以…", "小试牛刀", "这玩意还挺硬的", "诸武精通", "立体防御", "神之力" }};
vector < vector <string> > WindowAchievement::description = { { "第一次击杀怪物", "杀敌200", "陷入异常状态", "战斗胜利后剩余体力不超过200", "血量达到5000", "血量达到15000", "攻击超过2000", "防御超过2000", "等级超过135", "气息全满", "战斗中疲劳超过80", "战斗中使怪物疲劳超过80" }, { "获得怪物手册", "获得楼层传送器", "得到冒险道具袋", "持有至少10把红钥匙", "持有至少16个随意门", "获得圣水", "经验超过300", "金币超过1000", "获得第一本剑技书", "获得第一本防御术书", "集齐五个剑技", "集齐五个防御术" }, { "进入魔塔", "开启贪婪之神商店", "开启战斗之神商店", "救出盗贼", "在单次中毒状态下走了超过50步", "第一次装备剑技", "第一次装备防御术", "第一次使用剑技", "第一次使用防御术", "使用过所有的剑技", "使用过所有的防御术", "获得水晶球和三相之力之一" } };

/*
WindowGetAchievement
处理显示获得成就画面的类
*/
class WindowGetAchievement :public GameWindow {
public:
	string name;

	WindowGetAchievement(int x, int y);
	void Refresh();
};

/*
WindowStart
处理开局对话画面的类
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
处理标题画面的类
*/
class GameTitle :public SceneBase {
public:
	// 主函数
	void Main();
	// 更新画面
	void Update();
	void CommandNewGame();
	void CommandContinue();
	void CommandAchievement();
	void CommandExit();
private:
	GameSprite sprite, head1, head2;
	GameSelectWindow window = GameSelectWindow(160, 32, { "新游戏", "继续", "成就", "退出游戏" });
	WindowFileImage filewindow = WindowFileImage(320, 216);
	WindowFileCursor cursorwindow = WindowFileCursor(176, 216, 20, { "", "", "", "", "", "" });
	GameSelectWindow achcursurwindow = GameSelectWindow(128, 32, { "战斗专家","旅途的财富","冒险的回忆" });
	WindowAchievement achwindow = WindowAchievement(312, 144);
};

/*
GameMap
处理游戏地图的类
*/
class GameMap :public SceneBase {
public:
	// 主函数
	void Main();
	// 更新画面
	void Update();
	// 读档
	static void LoadData(int fileid);
	// 存档
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

	// 快捷键处理
	void ShortcutKey();
	// 消息处理
	void MessageUpdate();
	// 游戏结束处理
	void GameOver();
	// 怪物手册处理
	void MonsterBookUpdate();
	// 存档处理
	void FileUpdate();
	// 战斗处理
	void BattleFunction();
	int BattleUpdate();
	// 商店处理
	void ShopUpdate();
	// 设置地图
	void SetUp();
	void AddRecord();
};

/*
main
各声明结束后，从这里开始实际处理
*/

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
