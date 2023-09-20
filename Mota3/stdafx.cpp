#include "stdafx.h"

HWND GameActive;
LOGFONT Font;
// 游戏相关常量定义
float ResolutionRatio;					// 分辨率
int WindowWidth = 640;					// 窗口宽
int WindowHeight = 480;					// 窗口高
float BGMVolume;						// BGM音量
float SEVolume;							// SE音量
map <string, IMAGE*> ImageCache;		// 图片缓存

int Round(double n) {
	return floor(n + 0.5);
}

int Random(int n) {
	return (rand() % n);
}

int QuickPow(int a, int b) {
	int res = 1;
	while (b) {
		if (b & 1) res = res * a;
		a = a * a;
		b >>= 1;
	}
	return res;
}

void WOpaqueImage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, int opacity) {
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER,0,opacity,AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

void WOpaqueImage(IMAGE* dstimg, int x, int y, int width, int height, int sx, int sy, IMAGE* srcimg, int opacity) {
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = width;
	int h = height;
	BLENDFUNCTION bf = { AC_SRC_OVER,0,opacity,AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, sx, sy, w, h, bf);
}

void OpaqueImage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, int opacity) {
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	if (ResolutionRatio != 1) {
		x *= ResolutionRatio;
		y *= ResolutionRatio;
	}
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER,0,opacity,AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

void OpaqueImage(IMAGE* dstimg, int x, int y, int width, int height, int sx, int sy, IMAGE* srcimg, int opacity) {
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = width * ResolutionRatio;
	int h = height * ResolutionRatio;
	if (ResolutionRatio != 1) {
		x *= ResolutionRatio;
		y *= ResolutionRatio;
		sx *= ResolutionRatio;
		sy *= ResolutionRatio;
	}
	BLENDFUNCTION bf = { AC_SRC_OVER,0,opacity,AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, sx, sy, w, h, bf);
}

void BitmapOpaque(string file, int x, int y, int width, int height, int sx, int sy, int opacity) {
	if (!ExistFile("resource\\picture\\" + file)) return;
	OpaqueImage(nullptr, x, y, width, height, sx, sy, ImageCache[file], opacity);
}

void BitmapOpaque(string file, int x, int y, int opacity) {
	if (!ExistFile("resource\\picture\\" + file)) return;
	OpaqueImage(nullptr, x, y, ImageCache[file], opacity);
}

void ResizeBitmapOpaque(string file, int x, int y, int width, int height, int sx, int sy, float ratio, int opacity) {
	if (!ExistFile("resource\\picture\\" + file)) return;
	IMAGE img;
	int imgwidth = ImageCache[file]->getwidth() * ratio;
	int imgheight = ImageCache[file]->getheight() * ratio;
	loadimage(&img, ("resource\\picture\\" + file).c_str(), imgwidth, imgheight, true);
	OpaqueImage(nullptr, x, y, imgwidth / ResolutionRatio, imgheight / ResolutionRatio, sx / ResolutionRatio, sy / ResolutionRatio, &img, opacity);
}

void ResizeBitmapOpaque(string file, int x, int y, float ratio, int opacity) {
	if (!ExistFile("resource\\picture\\" + file)) return;
	IMAGE img;
	int imgwidth = ImageCache[file]->getwidth() * ratio;
	int imgheight = ImageCache[file]->getheight() * ratio;
	loadimage(&img, ("resource\\picture\\" + file).c_str(), imgwidth, imgheight, true);
	OpaqueImage(nullptr, x, y, &img, opacity);
}

void BitmapCentre(string file, int x, int y, int opacity) {
	if (!ExistFile("resource\\picture\\" + file)) return;
	WOpaqueImage(nullptr, x * ResolutionRatio - ImageCache[file]->getwidth() / 2, y * ResolutionRatio - ImageCache[file]->getheight() / 2, ImageCache[file], opacity);
}

void Print(string content) {
	MessageBox(NULL, content.c_str(), "Hint", MB_SYSTEMMODAL | MB_OK);
}

int YesNoPrint(string content) {
	return MessageBox(NULL, content.c_str(), "Confirmation", MB_SYSTEMMODAL | MB_ICONEXCLAMATION | MB_YESNO);
}

bool StringInclude(string source, string target) {
	return (source.find(target) != -1);
}

bool ExistFile(string filename) {
	return (_access(filename.c_str(), 0) != -1);
}

vector <string> Split(const string& s, const string& seperator) {
	vector <string> result;
	typedef string::size_type string_size;
	string_size i = 0;
	while (i != s.size()) {
		int flag = 0;
		while (i != s.size() && !flag) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x) {
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
			}
		}
		flag = 0;
		string_size j = i;
		while (j != s.size() && !flag) {
			for (string_size x = 0; x < seperator.size(); ++x) {
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			}
			if (!flag) ++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}

vector <string> ReadData(string file) {
	vector <string> ret;
	fstream data;
	char buffer[8192];
	data.open(file.c_str(), ios::in);
	data.getline(buffer, 8192);
	string str = buffer;
	ret = Split(str, ",");
	data.close();
	return ret;
}

vector <string> ReadCsv(string file) {
	vector <string> ret;
	fstream data;
	char buffer[4096];
	data.open(file.c_str(), ios::in);
	while (!data.eof()) {
		data.getline(buffer, 4096);
		string str = buffer;
		if (str != "") ret.push_back(buffer);
	}
	data.close();
	return ret;
}
