#include "DataInfo.h"

vector < tuple < int, string, string > > NpcInfo::NPCMessage(int id, int evid, vector <string> extrainfo) { 
	vector < tuple < int, string, string > > ret;
	if (id == 0) {
		ret.push_back({ -1, "勇士", \
"你是…？" });
		ret.push_back({ evid, "仙子", \
			"你好，勇士\n\
我是曾经作为这座塔守护者的仙灵，从现在起我和我的同伴们会在塔内尽可能给予你帮助" });
		ret.push_back({ evid, "仙子", \
			"旁边的书是怪物手册，可以查看怪物的能力，希望能帮到你" });
		return ret;
	}
	if (id == 1) { 
		ret.push_back({ evid, "商人", \
"你知道吗，按L就可以查看你当前可以使用的指令列表" });
		ret.push_back({ -1, "勇士", \
"我又不瞎……" });
		return ret;
	}
	if (id == 2) { 
		ret.push_back({ evid, "神秘老人", \
"疲劳会降低你能发挥出的力量，每10点疲劳会降低你一成伤害，如果疲劳达到100，你就没有办法击中对方了" });
		ret.push_back({ evid, "神秘老人", \
"在疲劳高于50点时，你将无法使出包括会心一击以内的任何技能！" });
		ret.push_back({ evid, "神秘老人", \
"但是按V键就可以恢复疲劳值" });
		return ret;
	}
	if (id == 3) {
		ret.push_back({ evid, "神秘老人", \
"更高的等级可以帮助你更好地破除怪物的防御，能让你在攻击大于怪物防御之前至少能造成1点伤害\n\
除此之外，似乎还能对某些技能威力进行加持……" });
		ret.push_back({ evid, "神秘老人", \
"不过在你的攻击真正超过怪物防御之前，你是没有办法对他们使用剑技的" });
		ret.push_back({ evid, "神秘老人", \
"你目前的攻击临界值是：" + extrainfo[0] });
		return ret;
	}
	if (id == 4) { 
		ret.push_back({ evid, "商人", \
"这些常年修炼的怪物，有很多都身怀绝技，和他们打交道之前一定要先了解一下他们" });
		return ret;
	}
	if (id == 6) { 
		ret.push_back({ evid, "商人", \
"你听说过剑技和防御术吗\n\
这种武功秘籍可以很有效地帮助战斗，要是我能搞到一本……啧啧，那就发财了" });
		return ret;
	}
	if (id == 7) { 
		ret.push_back({ evid, "盗贼", \
"天哪，终于有人来救我了" });
		ret.push_back({ evid, "盗贼", \
"老天作证，我当时就是看这里没什么人把守，才想摸进来看看能不能偷到什么值钱的东西的，没想到突然冲进来这么多怪物" });
		ret.push_back({ -1, "勇士", \
"现在你得救了，快出去吧，这里太危险了" });
		ret.push_back({ evid, "盗贼", \
"嘿，你别看我是个盗贼，我也是正儿八经的王国公民\n\
这座塔都成这样了，王国的情况估计也不太好" });
		ret.push_back({ evid, "盗贼", \
"我猜你应该是带着任务来的，我觉得我也应该给你点我力所能及的支持，我这里有不少钥匙……" });
		ret.push_back({ -1, "勇士", \
"都要免费送我吗？你真好" });
		ret.push_back({ evid, "盗贼", \
"……对不起做不到\n\
不过我可以便宜卖给你，绝对管够" });
		return ret;
	}
	if (id == 8) { 
		ret.push_back({ evid, "老人", \
"常回头看看" });
		return ret;
	}
	if (id == 9) { 
		ret.push_back({ evid, "老人", \
"战斗中变得疲惫是很正常的事情，深呼吸（V键）就可以消除部分疲劳\n\
当前你呼吸一次可以回复的疲劳值为：" + extrainfo[0]});
		return ret;
	}
	if (id == 10) { 
		ret.push_back({ -1, "勇士", \
			"公主，终于找到你了，没时间解释了，快跟我走吧" });
		ret.push_back({ evid, "公主？", \
			"…" });
		ret.push_back({ -1, "勇士", \
			"公主…？" });
		ret.push_back({ evid, "公主？", \
			"哈哈哈哈哈…\n\
受死吧！" });
		return ret;
	}
	if (id == 11) { 
		ret.push_back({ -1, "勇士", \
"妖孽，吃我一刀！" });
		ret.push_back({ evid, "史莱姆法师", \
"我说停停\n贫僧不喜打斗，此行乃被迫而来" });
		ret.push_back({ -1, "勇士", \
"？\n你不是个法师吗，既然不喜打斗，来这里做什么" });
		ret.push_back({ evid, "史莱姆法师", \
"上帝曰：我不入地狱，谁入地狱，妹有办法\n\
小伙子，我看你面相一表人才，居然做出此等蠢事，大好的草药精华居然直接喝了" });
		ret.push_back({ -1, "勇士", \
"（拔刀）" });
		ret.push_back({ evid, "史莱姆法师", \
"…我有一道具袋，可以储存这些草药精华，我看咱俩有缘，就送给你了\n\
我不打扰你看，先走了哈（开溜）" });
		ret.push_back({ -1, "勇士", \
"…" });
		return ret;
	}
	if (id == 12) { 
		ret.push_back({ evid, "神秘老人", \
"塔内有一种特殊的墙，在贴上这种神秘符咒之后可以变成黄门，按下T可以使用\n\
我这里有三个，你可以拿去试试\n\
不够的话还可以到我这里来买，价格优惠" });
		return ret;
	}
	if (id == 13) { 
		ret.push_back({ evid, "商人", \
"作为一名商人，最重要的就是考虑周全，提前备齐物资，毕竟来回跑腿真的很麻烦，对吧" });
		ret.push_back({ evid, "商人", \
"话说我听说这要塞里收藏着六大至宝，貌似是三个水晶球和三个法杖一样的东西\n\
要是我能搞到一个，可能我就发达了吧" });
		return ret;
	}
	if (id == 14) { 
		ret.push_back({ evid, "妖精", \
"在这种大型迷宫，你的楼层传送器并不会失效哟，如果迷路了就用用看吧" });
		return ret;
	}
	if (id == 15) { 
		ret.push_back({ evid, "妖精", \
"这是三颗水晶球，分别有不同作用" });
		ret.push_back({ evid, "妖精", \
"火神之水晶球会强化你的会心一击和剑技\n\
水神之水晶球会强化你的防御术\n\
树神之水晶球具有「生」的能力" });
		ret.push_back({ evid, "妖精", \
"但是你只能选择一颗水晶球，拾起一颗时，另外两颗将立刻碎裂" });
		return ret;
	}
	if (id == 16) { 
		ret.push_back({ evid, "妖精", \
"这是三相之力，分别有不同作用" });
		ret.push_back({ evid, "妖精", \
"贤者之力会降低对方的战斗意志\n\
霸者之力会在使用防御术时进行反击\n\
勇者之力会在使用会心一击和剑技时协同攻击" });
		ret.push_back({ evid, "妖精", \
"但是你只能选择其中之一，拾起一个时，另外两个将立刻碎裂" });
		return ret;
	}
	if (id == 17) {
		ret.push_back({ -1, "勇士", \
"这扇门在这边貌似打不开…" });
		return ret;
	}
	if (id == 18) {
		ret.push_back({ -1, "勇士", \
"魔头，受死吧！" });
		ret.push_back({ 112, "魔王", \
"真是如附骨之蛆一般紧追不舍…\n\
来吧，决一死战吧" });
		return ret;
	}
	return { { evid, "NPC", "哔哔哔卟卟卟啵啵啵叭叭叭" } };
}

tuple <int, int, int> TransInfo::Transmission(int id) {
	if (id == 1) return { 37, -1, -1 }; // 前往侧塔1F
	if (id == 2) return { 36, -1, -1 }; // 前往地下6F
	if (id == 3) return { 51, 5, 10 }; // 前往密室1
	if (id == 4) return { 46, 8, 8 }; // 出密室1
	if (id == 5) return { 58, -1, -1 }; // 前往侧塔16F
	if (id == 6) return { 57, -1, -1 }; // 前往地下16F
	if (id == 7) return { 94, 5, 5 }; // 传送往地下密室群
	if (id == 8) return { 59, 10, 10 }; // 密室群传送回地下
	if (id == 9) return { 68, 5, 10 }; // 传送往侧塔密室群
	if (id == 10) return { 66, 6, 0 }; // 密室群传送回侧塔
	if (id == 11) return { 66, -1, -1 }; // 前往侧塔15F
	if (id == 12) return { 65, -1, -1 }; // 前往地下20F
	if (id == 13) return { 69, 10, -1 }; // (3,5)传送至(2,5)
	if (id == 14) return { 77, -1, 10 }; // (3,5)传送至(3,4)
	if (id == 15) return { 68, 0, -1 };// (2,5)传送至(3,5)
	if (id == 16) return { 70, 10, -1 };// (2,5)传送至(1,5)
	if (id == 17) return { 88, -1, 10 };// (2,5)传送至(2,4)
	if (id == 18) return { 69, 0, -1 };// (1,5)传送至(2,5)
	if (id == 19) return { 71, -1, 10 };// (1,5)传送至(1,4)
	if (id == 20) return { 70, -1, 0 };// (1,4)传送至(1,5)
	if (id == 21) return { 72, -1, 10 };// (1,4)传送至(1,3)
	if (id == 22) return { 71, -1, 0 };// (1,3)传送至(1,4)
	if (id == 23) return { 73, -1, 10 };// (1,3)传送至(1,2)
	if (id == 24) return { 89, 0, -1 };// (1,3)传送至(2,3)
	if (id == 25) return { 72, -1, 0 };// (1,2)传送至(1,3)
	if (id == 26) return { 74, -1, 10 };// (1,2)传送至(1,1)
	if (id == 27) return { 73, -1, 0 };// (1,1)传送至(1,2)
	if (id == 28) return { 75, 0, -1 };// (1,1)传送至(2,1)
	if (id == 29) return { 74, 10, -1 };// (2,1)传送至(1,1)
	if (id == 30) return { 76, 0, -1 };// (2,1)传送至(3,1)
	if (id == 31) return { 75, 10, -1 };// (3,1)传送至(2,1)
	if (id == 32) return { 68, -1, 0 };// (3,4)传送至(3,5)
	if (id == 33) return { 78, -1, 10 };// (3,4)传送至(3,3)
	if (id == 34) return { 77, -1, 0 };// (3,3)传送至(3,4)
	if (id == 35) return { 88, 10, -1 };// (3,4)传送至(2,4)
	if (id == 36) return { 77, 0, -1 };// (2,4)传送至(3,4)
	if (id == 37) return { 92, 0, -1 };// (3,4)传送至(4,4)
	if (id == 38) return { 77, 10, -1 };// (4,4)传送至(3,4)
	if (id == 39) return { 79, -1, 10 };// (3,3)传送至(3,2)
	if (id == 40) return { 78, -1, 0 };// (3,2)传送至(3,3)
	if (id == 41) return { 89, 10, -1 };// (3,3)传送至(2,3)
	if (id == 42) return { 78, 0, -1 };// (2,3)传送至(3,3)
	if (id == 43) return { 91, 0, -1 };// (3,3)传送至(4,3)
	if (id == 44) return { 78, 10, -1 };// (4,3)传送至(3,3)
	if (id == 45) return { 90, 10, -1 };// (3,2)传送至(2,2)
	if (id == 46) return { 79, 0, -1 }; // (2,2)传送至(3,2)
	if (id == 47) return { 80, 0, -1 }; // (3,2)传送至(4,2)
	if (id == 48) return { 79, 10, -1 }; // (4,2)传送至(3,2)
	if (id == 49) return { 82, -1, 10 }; // (4,2)传送至(4,1)
	if (id == 50) return { 80, -1, 0 }; // (4,1)传送至(4,2)
	if (id == 51) return { 91, -1, 0 }; // (4,2)传送至(4,3)
	if (id == 52) return { 80, -1, 10 }; // (4,3)传送至(4,2)
	if (id == 53) return { 81, 5, 5 }; // (4,2)传送至密室
	if (id == 54) return { 80, 9, 10 }; // 密室传送至(4,2)
	if (id == 55) return { 83, 0, -1 }; // (4,1)传送至(5,1)
	if (id == 56) return { 82, 10, -1 }; // (5,1)传送至(4,1)
	if (id == 57) return { 84, -1, 0 }; // (5,1)传送至(5,2)
	if (id == 58) return { 83, -1, 10 }; // (5,2)传送至(5,1)
	if (id == 59) return { 85, -1, 0 }; // (5,2)传送至(5,3)
	if (id == 60) return { 84, -1, 10 }; // (5,3)传送至(5,2)
	if (id == 61) return { 86, -1, 0 }; // (5,3)传送至(5,4)
	if (id == 62) return { 85, -1, 10 }; // (5,4)传送至(5,3)
	if (id == 63) return { 87, -1, 0 }; // (5,4)传送至(5,5)
	if (id == 64) return { 86, -1, 10 }; // (5,5)传送至(5,4)
	if (id == 65) return { 69, -1, 0 }; // (2,4)传送至(2,5)
	if (id == 66) return { 89, -1, 10 }; // (2,4)传送至(2,3)
	if (id == 67) return { 88, -1, 0 }; // (2,3)传送至(2,4)
	if (id == 68) return { 90, -1, 10 }; // (2,3)传送至(2,2)
	if (id == 69) return { 89, -1, 0 }; // (2,2)传送至(2,3)
	if (id == 70) return { 72, 10, -1 }; // (2,3)传送至(1,3)
	if (id == 71) return { 93, -1, 0 }; // (4,4)传送至(4,5)
	if (id == 72) return { 92, -1, 10 }; // (4,5)传送至(4,4)
	if (id == 73) return { 95, -1, 10 }; // 1传送至3
	if (id == 74) return { 94, -1, 0 }; // 3传送至1
	if (id == 75) return { 107, -1, 0 }; // 1传送至14
	if (id == 76) return { 94, -1, 10 }; // 14传送至1
	if (id == 77) return { 96, 10, -1 }; // 3传送至2
	if (id == 78) return { 95, 0, -1 }; // 2传送至3
	if (id == 79) return { 97, 0, -1 }; // 3传送至4
	if (id == 80) return { 95, 10, -1 }; // 4传送至3
	if (id == 81) return { 98, -1, 10 }; // 4传送至8
	if (id == 82) return { 97, -1, 0 }; // 8传送至4
	if (id == 83) return { 99, -1, 10 }; // 8传送至12
	if (id == 84) return { 98, -1, 0 }; // 12传送至8
	if (id == 85) return { 100, 0, -1 }; // 8传送至9
	if (id == 86) return { 98, 10, -1 }; // 9传送至8
	if (id == 87) return { 101, 10, -1 }; // 12传送至11
	if (id == 88) return { 99, 0, -1 }; // 11传送至12
	if (id == 89) return { 102, 5, 5 }; // 12传送至密室
	if (id == 90) return { 99, 1, 5 }; // 密室传送至12
	if (id == 91) return { 103, 10, -1 }; // 11传送至10
	if (id == 92) return { 101, 0, -1 }; // 10传送至11
	if (id == 93) return { 104, -1, 0 }; // 10传送至6
	if (id == 94) return { 103, -1, 10 }; // 6传送至10
	if (id == 95) return { 105, -1, 0 }; // 11传送至7
	if (id == 96) return { 101, -1, 10 }; // 7传送至11
	if (id == 97) return { 105, 0, -1 }; // 6传送至7
	if (id == 98) return { 104, 10, -1 }; // 7传送至6
	if (id == 99) return { 106, 10, -1 }; // 6传送至5
	if (id == 100) return { 104, 0, -1 }; // 5传送至6
	if (id == 101) return { 108, 0, -1 }; // 14传送至15
	if (id == 102) return { 107, 10, -1 }; // 15传送至14
	if (id == 103) return { 109, -1, 0 }; // 14传送至18
	if (id == 104) return { 107, -1, 10 }; // 18传送至14
	if (id == 105) return { 110, 10, -1 }; // 18传送至17
	if (id == 106) return { 109, 0, -1 }; // 17传送至18
	if (id == 107) return { 111, -1, 0 }; // 15传送至19
	if (id == 108) return { 108, -1, 10 }; // 19传送至15
	if (id == 109) return { 111, 0, -1 }; // 18传送至19
	if (id == 110) return { 109, 10, -1 }; // 19传送至18
	if (id == 111) return { 112, -1, 10 }; // 17传送至13
	if (id == 112) return { 110, -1, 0 }; // 13传送至17
	if (id == 113) return { 113, -1, 0 }; // 17传送至21
	if (id == 114) return { 110, -1, 10 }; // 21传送至17
	if (id == 115) return { 114, 10, -1 }; // 17传送至16
	if (id == 116) return { 110, 0, -1 }; // 16传送至17
	if (id == 117) return { 117, -1, 0 }; // 19传送至23
	if (id == 118) return { 111, -1, 10 }; // 23传送至19
	if (id == 119) return { 116, -1, 0 }; // 18传送至22
	if (id == 120) return { 109, -1, 10 }; // 22传送至18
	if (id == 121) return { 116, 0, -1 }; // 21传送至22
	if (id == 122) return { 113, 10, -1 }; // 22传送至21
	if (id == 123) return { 115, 4, 6 }; // 16传送至20
	if (id == 124) return { 114, 0, 5 }; // 20传送至16
	if (id == 125) return { 115, 0, -1 }; // 19传送至20
	if (id == 126) return { 111, 10, -1 }; // 20传送至19
	if (id == 127) return { 117, 0, -1 }; // 22传送至23
	if (id == 128) return { 116, 10, -1 }; // 23传送至22

	return { -1, -1, -1 };
}
