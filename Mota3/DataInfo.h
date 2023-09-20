#pragma once

#include "stdafx.h"

class NpcInfo {
public:
	vector < tuple < int, string, string > > NPCMessage(int id, int evid, vector <string> extrainfo = {});
};

class TransInfo {
public:
	tuple <int, int, int> Transmission(int id);
};
