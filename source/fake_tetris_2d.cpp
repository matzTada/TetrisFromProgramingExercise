#include <windows.h>
#include <math.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string.h>
#include "sdglib.h"
#include <time.h>
#include <random>
#include <stdlib.h>
#include <fstream>
#include <ctime>
#include <sstream>

static int f[12][25];//上方向で変なバグを起こさないように大きめに作ってある。
static int fmino(0);
static int holdf[8][8];
static int nextf[12][25];
static int ghostf[12][25];
static int ghostfmino(0);
static int tt(0);
static int kind(0), holdkind(4);
static int x(5), y(20), p(0);
static int v(500), downflag(1);
static int displayflag(0);
static int linepoint(0);
static int minosum(0), minosequence[100000];
static double stagecolorr(1), stagecolorg(1), stagecolorb(1);//ブロックの色とか
static int stagepoint(1);
static int harddropflag(0);
static int kv(0);
static int cheatingflag(0);
//ハイスコアを入力するための変数を作ります。
std::string name = "";
static int keyboardflag(0);
static int hslinepoint[300], hsminosum[300], hstt[300];
std::string hsname[300];
static int playertotal(0);

//とりあえずx軸方向の範囲限定はそれぞれのminoの関数でやっている //fminoにすることによっていろんなブロックに対応できるようにした。
void mb(int &x, int &y, int &p, int &fmino){//box
	p = 0;
	switch (p){
	case 0:f[x][y] = fmino; f[x + 1][y] = fmino; f[x][y + 1] = fmino; f[x + 1][y + 1] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11) x = 10 - 1; break;
	default: break;
	}
}
void mz(int &x, int &y, int &p, int &fmino){//z
	p %= 2;
	switch (p){
	case 0: f[x][y + 1] = fmino; f[x + 1][y + 1] = fmino; f[x + 1][y] = fmino; f[x + 2][y] = fmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//横に長い時
	case 1: f[x][y] = fmino; f[x][y + 1] = fmino; f[x + 1][y + 1] = fmino; f[x + 1][y + 2] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//縦に長い時
	default: break;
	}
}
void ms(int &x, int &y, int &p, int &fmino){//s
	p %= 2;
	switch (p){
	case 0: f[x][y] = fmino; f[x + 1][y] = fmino; f[x + 1][y + 1] = fmino; f[x + 2][y + 1] = fmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//横に長い時
	case 1: f[x][y + 1] = fmino; f[x][y + 2] = fmino; f[x + 1][y + 1] = fmino; f[x + 1][y] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//縦に長い時
	default: break;
	}
}
void mi(int &x, int &y, int &p, int &fmino){//i　棒
	p %= 2;
	switch (p){
	case 0: f[x][y] = fmino; f[x + 1][y] = fmino; f[x + 2][y] = fmino; f[x + 3][y] = fmino;
		if (x == 0) x = 1; if (x + 3 >= 11)x = 10 - 3; break;//横に長い時
	case 1: f[x + 1][y - 1] = fmino; f[x + 1][y] = fmino; f[x + 1][y + 1] = fmino; f[x + 1][y + 2] = fmino;
		if (x + 1 == 0) x = 0; if (x + 1 == 11) x = 9; break;//縦に長い時
	default: break;
	}
}
void ml(int &x, int &y, int &p, int &fmino){// L
	switch (p){
	case 0: f[x][y] = fmino; f[x + 1][y] = fmino; f[x + 2][y] = fmino; f[x + 2][y + 1] = fmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//横に長い時
	case 1: f[x][y] = fmino; f[x][y + 1] = fmino; f[x][y + 2] = fmino; f[x + 1][y] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//縦に長い時
	case 2: f[x][y] = fmino; f[x][y + 1] = fmino; f[x + 1][y + 1] = fmino; f[x + 2][y + 1] = fmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//逆横に長い時
	case 3: f[x][y + 2] = fmino; f[x + 1][y + 2] = fmino; f[x + 1][y + 1] = fmino; f[x + 1][y] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//逆縦に長い時
	default: break;
	}
}
void mj(int &x, int &y, int &p, int &fmino){// J
	switch (p){
	case 0: f[x][y] = fmino; f[x + 1][y] = fmino; f[x + 2][y] = fmino; f[x][y + 1] = fmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//横に長い時
	case 1: f[x][y] = fmino; f[x + 1][y] = fmino; f[x + 1][y + 1] = fmino; f[x + 1][y + 2] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//縦に長い時
	case 2: f[x][y + 1] = fmino; f[x + 1][y + 1] = fmino; f[x + 2][y + 1] = fmino; f[x + 2][y] = fmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//逆横に長い時
	case 3: f[x][y] = fmino; f[x][y + 1] = fmino; f[x][y + 2] = fmino; f[x + 1][y + 2] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//逆縦に長い時
	default: break;
	}
}
void mt(int &x, int &y, int &p, int &fmino){// T
	switch (p){
	case 0: f[x][y] = fmino; f[x + 1][y] = fmino; f[x + 2][y] = fmino; f[x + 1][y + 1] = fmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//上凸
	case 1: f[x][y] = fmino; f[x][y + 1] = fmino; f[x][y + 2] = fmino; f[x + 1][y + 1] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//右凸
	case 2: f[x][y + 1] = fmino; f[x + 1][y + 1] = fmino; f[x + 2][y + 1] = fmino; f[x + 1][y] = fmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//下凸
	case 3: f[x][y + 1] = fmino; f[x + 1][y] = fmino; f[x + 1][y + 1] = fmino; f[x + 1][y + 2] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//左凸
	default: break;
	}
}

int kindrandom(int tt){//7個で1ブロックとしブロック1つ内で全種類が出るようにしたいねー
	//ゲームが始まった段階で配列を作っちゃったら楽なのかもしれない。
	int randmino;
	srand(tt);
	randmino = 1 + (int)(rand() * 7 - 1 + 1) / (1 + RAND_MAX);
	if (minosequence[minosum] == 0)return randmino;//配列を作った時になぜか0ができてしまうのでそれの解消用です。たぶんrdの範囲指定がちゃんとしてないからだと思う。
	else return minosequence[minosum];
}

void colorrandom(){//バックの色が変わるようにしようかなという企画。
	srand(tt);
	double color = (double)(rand() * 1 - 0 + 1) / (1 + RAND_MAX);
	switch (rand() % 3){
	case 0:stagecolorr = color; break;
	case 1:stagecolorg = color; break;
	case 2:stagecolorb = color; break;
	default:break;
	}
}

void harddrop(int &y, int &harddropflag, int &kv, int &v){
	if (harddropflag){
		kv = v;
		v = 1;
		harddropflag = 0;
	}
}

/*
void ghostminoselect(int &x, int &y, int &p, int &kind, int &tt, int &harddropflag, int &kv, int &v){//現在動かしているテトリミノの判定をkindというフラグを元に行っている。
fmino = 4;
switch (kind){
case 1:mb(x, y, p, fmino); break;
case 2:mz(x, y, p, fmino); break;
case 3:ms(x, y, p, fmino); break;
case 4:mi(x, y, p, fmino); break;
case 5:ml(x, y, p, fmino); break;
case 6:mj(x, y, p, fmino); break;
case 7:mt(x, y, p, fmino); break;
default:break;
}
}
void ghostbrock(int &x, int &y, int &p, int &kind, int &tt, int &harddropflag, int &kv, int &v){//現在動かしているテトリミノの判定をkindというフラグを元に行っている。
//まず動いてるやつの一番下を検出し、保存する
int yminu[4], ymaxd[4], yminmax[4], yminflag(0);
for (int i(0); i < 4; i++){ yminu[i] = 0; ymaxd[i]=0; }
for (int i(x); i <= 10; i++){
int flag1(0);
for (int j(1); j <= y + 2; j++){
if (flag1 == 0){
if (f[i][j] == 2){//下から動いてるやつを調べた結果
flag1 = 1;
yminu[yminflag] = j;//あるxのところで
int flag2(0);
for (int k(j - 1); k >= 0; k--){
if (flag2 == 0){
if (f[i][k] == 1){//上から止まっている奴を調べた結果
flag2 = 1;
ymaxd[yminflag] = k;
yminmax[yminflag] = yminu[yminflag] - ymaxd[yminflag];//2と1の間を出している
yminflag++;
}
}
}
}
}
}
}
int minyminmax = yminmax[0];
for (int i = 0; i<4; ++i){
if (yminmax[0]>= yminmax[i]){
minyminmax = yminmax[i];
}
}
int d = y - minyminmax + 3;
if(d>4)ghostminoselect(x, d, p, kind, tt, harddropflag, kv, v);
}
*/

void brockshadow(int &x, int &y, int &p, int &kind, int tt, int &harddropflag, int &kv, int &v){//床につく、またはほかのミノにくっついたときにラインが消せるかの判別を行う、および次を開始する
	for (int i(x); i <= 10; i++){//これだけでも実はきれいだったりする。動いてるやつから止まってるやつまで塗り潰す
		int flag1(0);
		for (int j(1); j <= y + 2; j++){
			if (flag1 == 0){
				if (f[i][j] == 2){//下から動いてるやつを調べた結果
					flag1 = 1;
					int flag2(0);
					for (int k(j - 1); k >= 0; k--){
						if (flag2 == 0){
							if (f[i][k] == 1){//上から止まっている奴を調べた結果
								flag2 = 1;
								for (int l(k + 1); l <= j - 1; l++){
									if (f[i][l] == 0){
										f[i][l] = 3;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void linedelete(int &x, int &y, int &p){//lineを消すための判断　countはそこにブロックが存在するかどうかのフラグのようなもの
	using namespace SDGLibF;
	//int counttetris(0);
	for (int j(20); j >= 1; j--){//jを上から落とすようにしたら一気に消せるようになった。
		int count(0);
		for (int i(1); i <= 10; i++){
			if (f[i][j] == 1) count++;//
		}
		if (count == 10){//消すところの本体。あるj段の合計が10だったら、上の段を下の段に移すのを繰り返す。
			linepoint++;
			//counttetris++;
			for (int ja(j); ja <= 19; ja++){
				for (int i(1); i <= 10; i++){
					f[i][ja] = f[i][ja + 1];
				}
			}
			if (linepoint % 3 == 0){//いわばステージ的な　ホニャララこ消すごとに背景色が変わり速度が上がる。
				colorrandom();
				if (v <= 30)v -= 1; if (v > 30 && v <= 100)v -= 10; if (v > 100 && v <= 1000)v -= 100; if (v > 1000) v -= 1000; if (v <= 0) v = 10;
				stagepoint++;
			}
		}
	}
	//if (counttetris == 4){} //テトリス決まった時の特別なエフェクトを用意したい
}

void stopnext(int &x, int &y, int &p, int &kind, int tt, int &harddropflag, int &kv, int &v){//床につく、またはほかのミノにくっついたときにラインが消せるかの判別を行う、および次を開始する
	for (int j(0); j <= 20; j++){
		for (int i(1); i <= 10; i++){
			if (f[i][j] == 1 && f[i][j + 1] == 2){//もし止まったら…。
				harddropflag = 0;
				for (int j(1); j <= 20; j++){//動いてた奴を1に書き換えてから
					for (int i(1); i <= 10; i++){
						if (f[i][j] == 2) f[i][j] = 1;
					}
				}
				v = kv;
				linedelete(x, y, p);//行が消せるかどうかを判断して、次を始める。消した後は新しいのを始める
				/*やるたびに作ってくバージョン
				int randmino;
				srand(tt);
				randmino = 1 + (int)(rand() * 7 - 1 + 1) / (1 + RAND_MAX);
				minosequence[minosum + 7] = randmino;
				*/
				kind = kindrandom(tt);
				++minosum;
				x = 5; y = 20; p = 0;

			}
		}
	}

}

void minoselect(int &x, int &y, int &p, int &kind, int &tt, int &harddropflag, int &kv, int &v){//現在動かしているテトリミノの判定をkindというフラグを元に行っている。
	fmino = 2;
	switch (kind){
	case 1:mb(x, y, p, fmino); break;
	case 2:mz(x, y, p, fmino); break;
	case 3:ms(x, y, p, fmino); break;
	case 4:mi(x, y, p, fmino); break;
	case 5:ml(x, y, p, fmino); break;
	case 6:mj(x, y, p, fmino); break;
	case 7:mt(x, y, p, fmino); break;
	default:break;
	}
	stopnext(x, y, p, kind, tt, harddropflag, kv, v);
}

void cleanup(){//動いてるやつをきれいにする
	for (int j(1); j <= 25; j++){
		for (int i(1); i <= 10; i++){
			if (f[i][j] == 2 || f[i][j] == 3 || f[i][j] == 4) f[i][j] = 0;
		}
	}
}

void ghostmb(int &x, int &y, int &p, int &ghostfmino){//box
	p = 0;
	switch (p){
	case 0:ghostf[x][y] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x][y + 1] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11) x = 10 - 1; break;
	default: break;
	}
}
void ghostmz(int &x, int &y, int &p, int &ghostfmino){//z
	p %= 2;
	switch (p){
	case 0:ghostf[x][y + 1] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 2][y] = ghostfmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//横に長い時
	case 1:ghostf[x][y] = ghostfmino; ghostf[x][y + 1] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 1][y + 2] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//縦に長い時
	default: break;
	}
}
void ghostms(int &x, int &y, int &p, int &ghostfmino){//s
	p %= 2;
	switch (p){
	case 0:ghostf[x][y] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 2][y + 1] = ghostfmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//横に長い時
	case 1:ghostf[x][y + 1] = ghostfmino; ghostf[x][y + 2] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 1][y] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//縦に長い時
	default: break;
	}
}
void ghostmi(int &x, int &y, int &p, int &ghostfmino){//i　棒
	p %= 2;
	switch (p){
	case 0:ghostf[x][y] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 2][y] = ghostfmino; ghostf[x + 3][y] = ghostfmino;
		if (x == 0) x = 1; if (x + 3 >= 11)x = 10 - 3; break;//横に長い時
	case 1:ghostf[x + 1][y - 1] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 1][y + 2] = ghostfmino;
		if (x + 1 == 0) x = 0; if (x + 1 == 11) x = 9; break;//縦に長い時
	default: break;
	}
}
void ghostml(int &x, int &y, int &p, int &ghostfmino){// L
	switch (p){
	case 0:ghostf[x][y] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 2][y] = ghostfmino; ghostf[x + 2][y + 1] = ghostfmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//横に長い時
	case 1:ghostf[x][y] = ghostfmino; ghostf[x][y + 1] = ghostfmino; ghostf[x][y + 2] = ghostfmino; ghostf[x + 1][y] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//縦に長い時
	case 2:ghostf[x][y] = ghostfmino; ghostf[x][y + 1] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 2][y + 1] = ghostfmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//逆横に長い時
	case 3:ghostf[x][y + 2] = ghostfmino; ghostf[x + 1][y + 2] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 1][y] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//逆縦に長い時
	default: break;
	}
}
void ghostmj(int &x, int &y, int &p, int &ghostfmino){// J
	switch (p){
	case 0:ghostf[x][y] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 2][y] = ghostfmino; ghostf[x][y + 1] = ghostfmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//横に長い時
	case 1:ghostf[x][y] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 1][y + 2] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//縦に長い時
	case 2:ghostf[x][y + 1] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 2][y + 1] = ghostfmino; ghostf[x + 2][y] = ghostfmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//逆横に長い時
	case 3:ghostf[x][y] = ghostfmino; ghostf[x][y + 1] = ghostfmino; ghostf[x][y + 2] = ghostfmino; ghostf[x + 1][y + 2] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//逆縦に長い時
	default: break;
	}
}
void ghostmt(int &x, int &y, int &p, int &ghostfmino){// T
	switch (p){
	case 0:ghostf[x][y] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 2][y] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//上凸
	case 1:ghostf[x][y] = ghostfmino; ghostf[x][y + 1] = ghostfmino; ghostf[x][y + 2] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//右凸
	case 2:ghostf[x][y + 1] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 2][y + 1] = ghostfmino; ghostf[x + 1][y] = ghostfmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//下凸
	case 3:ghostf[x][y + 1] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 1][y + 2] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//左凸
	default: break;
	}
}
void ghostcleanup(){//動いてるやつをきれいにする
	for (int j(1); j <= 25; j++){
		for (int i(1); i <= 10; i++){
			if (ghostf[i][j] == 2 || ghostf[i][j] == 4)ghostf[i][j] = 0;
		}
	}
}
void ghostcleanup2(){//動いてるやつをきれいにする
	for (int j(1); j <= 25; j++){
		for (int i(1); i <= 10; i++){
			if (ghostf[i][j] == 2)ghostf[i][j] = 0;
		}
	}
}
void ghostbox(int &x, int &y, int &p, int &kind, int &tt, int &harddropflag, int &kv, int &v){//がんばろう
	//ghostfにfの1の部分だけをを読み込む
	for (int j(0); j < 25; j++){
		for (int i(0); i < 12; i++){
			if (f[i][j] == 2) ghostf[i][j] = f[i][j];
		}
	}
	//1を一番上だけ読ませられるかな・・・
	for (int i(1); i <= 10; i++){
		int flag(0);
		for (int j(21); j >= 0; j--){
			if (flag == 0){
				if (f[i][j] == 1) {
					ghostf[i][j] = f[i][j];
					flag = 1;
				}
			}
		}
	}
	int flag1(0);
	for (int k(y); k > 0; k--){

		ghostfmino = 2;
		switch (kind){
		case 1:ghostmb(x, k, p, ghostfmino); break;
		case 2:ghostmz(x, k, p, ghostfmino); break;
		case 3:ghostms(x, k, p, ghostfmino); break;
		case 4:ghostmi(x, k, p, ghostfmino); break;
		case 5:ghostml(x, k, p, ghostfmino); break;
		case 6:ghostmj(x, k, p, ghostfmino); break;
		case 7:ghostmt(x, k, p, ghostfmino); break;
		default:break;
		}
		//stopnext
		if (flag1 == 0){
			for (int j(20); j >= 0; j--){
				for (int i(10); i >= 1; i--){
					if (ghostf[i][j] == 1 && ghostf[i][j + 1] == 2){//もし止まったら…。
						for (int j(1); j <= 20; j++){//動いてた奴を1に書き換えてから
							for (int i(1); i <= 10; i++){
								if (ghostf[i][j] == 2){ ghostf[i][j] = 4; flag1 = 1; }

							}
						}
					}
				}
			}
		}
		ghostcleanup2();
	}
}

void outcsvfile(){
	using namespace std;
	//現在時刻の取得
	struct tm now;
	time_t longtime;
	longtime = time(NULL);
	char date[32];
	longtime = time(NULL);
	localtime_s(&now, &longtime);
	asctime_s(date, sizeof(date), &now);
	//ファイルへの書き込み
	if (name == ""){ name = "noname"; }
	std::ofstream outfile("tetrisdata.txt", std::ios::out | std::ios::app);
	outfile << linepoint << " " << minosum << " " << tt << " " << name << endl;
	outfile.close();
	outfile.clear();
}

void highscore(){
	using namespace std;
	ifstream infile;
	ofstream outfile;
	infile.open("tetrisdata.txt");
	outfile.open("highscore.txt");

	//入力ファイルの内容を読みこむ
	int flag = 0;

	for (int i(0); i < 100; i++){
		if (infile.eof()){
			flag = i - 1;
			playertotal = i - 1;
			break;
		}
		infile >> hslinepoint[i] >> hsminosum[i] >> hstt[i] >> hsname[i];
		cout << hslinepoint[i] << " " << hsminosum[i] << " " << hstt[i] << " " << hsname[i] << endl;
	}
	//linepoint順に並び替える
	cout << "並び替えます。" << endl;
	for (int i1(0); i1 < 100; i1++){
		for (int i2(0); i2 < 100 - i1 - 1; i2++){
			if (hslinepoint[i2] < hslinepoint[i2 + 1]){
				int m = hslinepoint[i2];
				hslinepoint[i2] = hslinepoint[i2 + 1];
				hslinepoint[i2 + 1] = m;

				int m1 = hsminosum[i2];
				hsminosum[i2] = hsminosum[i2 + 1];
				hsminosum[i2 + 1] = m1;

				int m2 = hstt[i2];
				hstt[i2] = hstt[i2 + 1];
				hstt[i2 + 1] = m2;

				string m3 = hsname[i2];
				hsname[i2] = hsname[i2 + 1];
				hsname[i2 + 1] = m3;
			}
		}
	}
	for (int i(0); i < flag; i++) {
		outfile << i + 1 << " linepoint:" << hslinepoint[i] << " tt:" << hstt[i] << " name:" << hsname[i] << endl;
		cout << i + 1 << " linepoint:" << hslinepoint[i] << " tt:" << hstt[i] << " name:" << hsname[i] << endl;
	}
	infile.close();
	outfile.close();
}

void drawbox(int x, int y, double r, double g, double b){//25×25のboxを作るための関数を作りました。
	//3Dモードだ！！！
	using namespace SDGLibF;//drawcircleがあるのにdrawboxがなく今まで不便だったので簡易版ですが作ってみました。
	SetColor(r, g, b);
	DrawLine(2, x * 25 + 1.5, y * 25 + 1.5, x * 25 + 1.5, y * 25 + 23.5);//left
	DrawLine(2, x * 25 + 1.5, y * 25 + 23.5, x * 25 + 23.5, y * 25 + 23.5);//top
	DrawLine(2, x * 25 + 23.5, y * 25 + 1.5, x * 25 + 23.5, y * 25 + 23.5);//right
	DrawLine(2, x * 25 + 1.5, y * 25 + 1.5, x * 25 + 23.5, y * 25 + 1.5);//botom
	//奥側の描画
	DrawLine(2, x * 25 + 1.5 + 10, y * 25 + 1.5 + 10, x * 25 + 1.5 + 10, y * 25 + 23.5 + 10);//left
	DrawLine(2, x * 25 + 1.5 + 10, y * 25 + 23.5 + 10, x * 25 + 23.5 + 10, y * 25 + 23.5 + 10);//top
	DrawLine(2, x * 25 + 23.5 + 10, y * 25 + 1.5 + 10, x * 25 + 23.5 + 10, y * 25 + 23.5 + 10);//right
	DrawLine(2, x * 25 + 1.5 + 10, y * 25 + 1.5 + 10, x * 25 + 23.5 + 10, y * 25 + 1.5 + 10);//botom
	//前と奥を結ぶ
	DrawLine(2, x * 25 + 1.5, y * 25 + 1.5, x * 25 + 1.5 + 10, y * 25 + 1.5 + 10);//left
	DrawLine(2, x * 25 + 1.5, y * 25 + 23.5, x * 25 + 1.5 + 10, y * 25 + 23.5 + 10);//top
	DrawLine(2, x * 25 + 23.5, y * 25 + 1.5, x * 25 + 23.5 + 10, y * 25 + 1.5 + 10);//right
	DrawLine(2, x * 25 + 23.5, y * 25 + 23.5, x * 25 + 23.5 + 10, y * 25 + 23.5 + 10);//botom
}
void drawboxpaint(int x, int y, double r, double g, double b){//boxを塗り潰すために作った。nextboxでは中抜きにしたかったため別関数で中身だけ定義
	using namespace SDGLibF;
	SetColor(r, g, b);
	for (int i(2); i <= 12; i++){
		DrawLine(2, x * 25 + 1.5 + i, y * 25 + 1.5 + i, x * 25 + 1.5 + i, y * 25 + 23.5 - i);//left
		DrawLine(2, x * 25 + 1.5 + i, y * 25 + 23.5 - i, x * 25 + 23.5 - i, y * 25 + 23.5 - i);//top
		DrawLine(2, x * 25 + 23.5 - i, y * 25 + 1.5 + i, x * 25 + 23.5 - i, y * 25 + 23.5 - i);//right
		DrawLine(2, x * 25 + 1.5 + i, y * 25 + 1.5 + i, x * 25 + 23.5 - i, y * 25 + 1.5 + i);//botom
	}

}

void displayfunc1(){
	if (displayflag == 0){
		using namespace SDGLibF;

		char buff[100];
		SetColor(0, 0, 0);
		sprintf_s(buff, "fake tetris:::press '  ' to START ::: ");
		DrawString(220, 60, buff);
		char buff7[100];
		SetColor(1, 0, 0);
		sprintf_s(buff7, "                              s");
		DrawString(220, 60, buff7);
		char buff1[100];
		SetColor(0, 0, 0);
		sprintf_s(buff1, "KEIO SD-Engineer");
		DrawString(180, 30, buff1);
		char buff6[100];
		SetColor(0.99, 0.54, 0);
		sprintf_s(buff6, "Tadanori Matsui");
		DrawString(390, 30, buff6);

	}
}
void displayfunc2boxes(){
	using namespace SDGLibF;
	//床の描画
	for (int i(0); i <= 11; i++){
		if (f[i][0] == 1)	{
			drawbox(i, 0, 0, 1, 0);
			drawboxpaint(i, 0, 0, 0.7 + stagecolorg*0.3, 0);
		}
	}
	//壁の描画
	for (int j(0); j <= 21; j++){
		drawbox(0, j, 0, 1, 0);
		drawboxpaint(0, j, 0, 0.7 + stagecolorg*0.3, 0);
		drawbox(11, j, 0, 1, 0);
		drawboxpaint(11, j, 0, 0.7 + stagecolorg*0.3, 0);
	}
	//ブロックの描画
	for (int j(1); j <= 21; j++){
		for (int i(1); i <= 10; i++){
			switch (f[i][j]){
				//case 0: drawbox(i, j, 0.5 + stagecolorr*0.5, 0.5 + stagecolorg*0.5, 0.5 + stagecolorb*0.5); break;
			case 1: drawbox(i, j, 0, 0, 0.7 + stagecolorb*0.3); drawboxpaint(i, j, 0.5 + stagecolorr*0.5, 0.5 + stagecolorg*0.5, 1); break;//動いてないほうは青くしました。
			case 2: drawbox(i, j, 0.7 + stagecolorr*0.3, 0, 0); drawboxpaint(i, j, 1, 0.5 + stagecolorg*0.5, 0.5 + stagecolorb*0.5); break;//動いてる方は赤くしました。
			case 3: drawbox(i, j, 0.75, 0.75, 0.75); drawboxpaint(i, j, 0.99, 0.99, 0.99); break;//ブロックシャドーのまっすぐの描画灰色
			case 4: drawbox(i, j, 0.5 + stagecolorr*0.5, 0.5 + stagecolorg*0.5, 0.5 + stagecolorb*0.5); drawboxpaint(i, j, 0.75, 0.75, 0.75); break;//ゴーストブロックの方
			default: break;
			}
		}
	}
	for (int j(22); j <= 23; j++){//はみだしている部分のやつ
		for (int i(1); i <= 10; i++){
			switch (f[i][j]){
			case 1: drawbox(i, j, 0, 0, 0.7 + stagecolorb*0.3); drawboxpaint(i, j, 0.5 + stagecolorr*0.5, 0.5 + stagecolorg*0.5, 1); break;//動いてないほうは青くしました。
			case 2: drawbox(i, j, 0.7 + stagecolorr*0.3, 0, 0); drawboxpaint(i, j, 1, 0.5 + stagecolorg*0.5, 0.5 + stagecolorb*0.5); break;//動いてる方は赤くしました。
			default: break;
			}
		}
	}
}
void nextmb(int &x, int &y, int &p){//box
	p = 0; nextf[x][y] = 2; nextf[x + 1][y] = 2; nextf[x][y + 1] = 2; nextf[x + 1][y + 1] = 2;
}
void nextmz(int &x, int &y, int &p){//z
	p = 0; nextf[x][y + 1] = 2; nextf[x + 1][y + 1] = 2; nextf[x + 1][y] = 2; nextf[x + 2][y] = 2;
}
void nextms(int &x, int &y, int &p){//s
	p = 0; nextf[x][y] = 2; nextf[x + 1][y] = 2; nextf[x + 1][y + 1] = 2; nextf[x + 2][y + 1] = 2;
}
void nextmi(int &x, int &y, int &p){//i　棒
	p = 0; nextf[x][y] = 2; nextf[x + 1][y] = 2; nextf[x + 2][y] = 2; nextf[x + 3][y] = 2;
}
void nextml(int &x, int &y, int &p){// L
	p = 0; nextf[x][y] = 2; nextf[x + 1][y] = 2; nextf[x + 2][y] = 2; nextf[x + 2][y + 1] = 2;
}
void nextmj(int &x, int &y, int &p){// J
	p = 0; nextf[x][y] = 2; nextf[x + 1][y] = 2; nextf[x + 2][y] = 2; nextf[x][y + 1] = 2;
}
void nextmt(int &x, int &y, int &p){// T
	p = 0; nextf[x][y] = 2; nextf[x + 1][y] = 2; nextf[x + 2][y] = 2; nextf[x + 1][y + 1] = 2;
}
void nextm0(int &x, int &y, int &p){// nextbox用なんだかわからない奴用です。 0が出ちゃったときにどうしようっていう
	p = 0;
	for (int i(0); i < 5; i++){
		for (int j(0); j < 4; j++){
			nextf[x + i - 1][y + j - 1] = 2;
		}
	}
}
void displayfunc2nextboxes(){//次に出てくる奴の表示
	int nextboxesy[4];
	for (int j(0); j < 25; j++){
		for (int i(0); i < 12; i++){
			nextf[i][j] = 0;
		}
	}
	int a(1), c(0);
	for (int i(0); i < 4; i++) {
		nextboxesy[i] = 18 - i * 5;
		switch (minosequence[minosum + i]){
		case 1:nextmb(a, nextboxesy[i], c); break;
		case 2:nextmz(a, nextboxesy[i], c); break;
		case 3:nextms(a, nextboxesy[i], c); break;
		case 4:nextmi(a, nextboxesy[i], c); break;
		case 5:nextml(a, nextboxesy[i], c); break;
		case 6:nextmj(a, nextboxesy[i], c); break;
		case 7:nextmt(a, nextboxesy[i], c); break;
		case 0:nextm0(a, nextboxesy[i], c); break;
		default:break;
		}
		//仕切りを作ろう
		for (int j(0); j < 5; j++){
			nextf[j][18 - i * 5 - 2] = 3;
		}
	}
	for (int j(0); j < 25; j++){
		for (int i(0); i < 12; i++){
			if (nextf[i][j] == 2)drawbox(i + 13, j, 0, 1, 1);//水色
			if (nextf[i][j] == 3)drawbox(i + 13, j, 1, 0.5, 0.75);//
		}
	}
}
void holdmb(int &x, int &y, int &p){//box
	holdf[x][y] = 2; holdf[x + 1][y] = 2; holdf[x][y + 1] = 2; holdf[x + 1][y + 1] = 2;
}
void holdmz(int &x, int &y, int &p){//z
	holdf[x][y + 1] = 2; holdf[x + 1][y + 1] = 2; holdf[x + 1][y] = 2; holdf[x + 2][y] = 2;
}
void holdms(int &x, int &y, int &p){//s
	holdf[x][y] = 2; holdf[x + 1][y] = 2; holdf[x + 1][y + 1] = 2; holdf[x + 2][y + 1] = 2;
}
void holdmi(int &x, int &y, int &p){//i　棒
	holdf[x][y] = 2; holdf[x + 1][y] = 2; holdf[x + 2][y] = 2; holdf[x + 3][y] = 2;
}
void holdml(int &x, int &y, int &p){// L
	holdf[x][y] = 2; holdf[x + 1][y] = 2; holdf[x + 2][y] = 2; holdf[x + 2][y + 1] = 2;
}
void holdmj(int &x, int &y, int &p){// J
	holdf[x][y] = 2; holdf[x + 1][y] = 2; holdf[x + 2][y] = 2; holdf[x][y + 1] = 2;
}
void holdmt(int &x, int &y, int &p){// T
	holdf[x][y] = 2; holdf[x + 1][y] = 2; holdf[x + 2][y] = 2; holdf[x + 1][y + 1] = 2;
}
void displayfunc2holdboxes(){//次に出てくる奴の表示
	int a(2), b(2), c(0);
	for (int j(1); j <= 4; j++){
		for (int i(1); i <= 5; i++){
			holdf[i][j] = 0;
		}
	}
	switch (holdkind){
	case 1:holdmb(a, b, c); break;
	case 2:holdmz(a, b, c); break;
	case 3:holdms(a, b, c); break;
	case 4:holdmi(a, b, c); break;
	case 5:holdml(a, b, c); break;
	case 6:holdmj(a, b, c); break;
	case 7:holdmt(a, b, c); break;
	default:break;
	}
	//hold枠の定義
	for (int j(0); j < 6; j++){
		holdf[0][j] = 3;
		holdf[6][j] = 3;
	}
	for (int i(0); i < 7; i++){
		holdf[i][0] = 3;
		holdf[i][5] = 3;
	}

	for (int j(0); j < 6; j++){
		for (int i(0); i < 7; i++){
			if (holdf[i][j] == 2)drawbox(i - 7, j + 16, 0.5, 0, 0.5);//紫
			if (holdf[i][j] == 3)drawbox(i - 7, j + 16, 1, 0.5, 0);//オレンジ
		}
	}
}
void displayfunc2ghostboxes(){
	using namespace SDGLibF;
	//ブロックの描画
	for (int j(21); j >= 0; j--){
		for (int i(1); i <= 10; i++){
			switch (ghostf[i][j]){
			case 4: drawbox(i, j, 0.5 + stagecolorr*0.5, 0.5 + stagecolorg*0.5, 0.5 + stagecolorb*0.5); drawboxpaint(i, j, 0.75, 0.75, 0.75); break;//ゴーストブロックの方
			default: break;
			}
		}
	}
}
void displayfunc2keycon(){
	using namespace SDGLibF;
	char buff5[100], buff6[100], buff7[100], buff8[100], buff9[100];
	SetColor(1, 0, 1);
	sprintf_s(buff9, "[z:<-][x:down][c:->]");
	sprintf_s(buff8, "[Space:harddrop]");
	sprintf_s(buff7, "[Enter:rotation]");
	sprintf_s(buff6, "[Backspace:hold]");
	sprintf_s(buff5, "[v:speedup][V:down][S:pose]");

	DrawString(-190, 130, buff9);
	DrawString(-190, 100, buff8);
	DrawString(-190, 70, buff7);
	DrawString(-190, 40, buff6);
	DrawString(-190, 10, buff5);
}
void displayfunc2(){
	if (displayflag == 1){
		using namespace SDGLibF;
		displayfunc2boxes();//分けることでプログラムをすっきりかけるようにしたつもりです
		displayfunc2nextboxes();//次に出てくる奴らの表示
		displayfunc2holdboxes();
		displayfunc2keycon();
		//displayfunc2ghostboxes();

		//時間の表示、速度かけ数の表示
		char buff1[100];
		SetColor(0, 0, 0);
		sprintf_s(buff1, "tt=%d v=%d linepoint=%d minosum=%d", tt, v, linepoint, minosum);
		DrawString(30, 570, buff1);
		//次の奴らの表示
		char buff3[100], buff4[100];
		SetColor(0, 0, 0);
		sprintf_s(buff3, "stage [%d]", stagepoint);
		DrawString(310, 540, buff3);
		sprintf_s(buff4, "next::%d %d %d %d %d %d %d", minosequence[minosum + 0], minosequence[minosum + 1], minosequence[minosum + 2], minosequence[minosum + 3]
			, minosequence[minosum + 4], minosequence[minosum + 5], minosequence[minosum + 6]);
		DrawString(310, 510, buff4);
		//カラー表示
		char buff5[100];
		SetColor(0, 0, 0);
		sprintf_s(buff5, "r=%3.2f g=%3.2f b=%3.2f", stagecolorr, stagecolorg, stagecolorb);
		DrawString(30, 540, buff5);
		//現在時刻の取得
		struct tm now;
		time_t longtime;
		longtime = time(NULL);
		char timestr[32];
		longtime = time(NULL);
		localtime_s(&now, &longtime);
		asctime_s(timestr, sizeof(timestr), &now);
		SetColor(0, 0, 0);
		DrawString(330, 0, timestr);


		cleanup();
		ghostcleanup();
	}
}
void displayfunc3(){
	if (displayflag == 2){
		using namespace SDGLibF;
		char buff[100], buff1[100];
		if (keyboardflag == 1) SetColor(1, 0, 0);
		if (keyboardflag == 0) SetColor(0, 0, 0);
		sprintf_s(buff, "GAME OVER");
		DrawString(100, 300, buff);
		SetColor(0, 0, 0);
		sprintf_s(buff1, "score ::: linepoint:%d minosum=%d", linepoint, minosum);
		DrawString(10, 270, buff1);

		//名前の入力
		SetColor(0, 0, 0);
		DrawString(110, 240, name.c_str());

		char buff01[100];
		if (keyboardflag == 1){
			DrawLine(2, 80, 235, 260, 235);
			sprintf_s(buff01, "please input your name => ENTER");
			DrawString(10, 210, buff01);
		}

		//highscoreの表示
		if (keyboardflag == 0){
			SetColor(0, 0, 0);
			char bufftotal[100];
			sprintf_s(bufftotal, "player total:%d", playertotal);
			DrawString(-190, 570, bufftotal);
			char buff02[100];
			sprintf_s(buff02, "1st. linepoint:%d name:%s", hslinepoint[0], hsname[0].c_str());
			DrawString(-190, 545, buff02);
			char buff03[100];
			sprintf_s(buff03, "2nd. linepoint:%d name:%s", hslinepoint[1], hsname[1].c_str());
			DrawString(-190, 520, buff03);
			char buff04[100];
			sprintf_s(buff04, "3rd. linepoint:%d name:%s", hslinepoint[2], hsname[2].c_str());
			DrawString(-190, 495, buff04);
			char buff05[100];
			sprintf_s(buff05, "4th. linepoint:%d name:%s", hslinepoint[3], hsname[3].c_str());
			DrawString(-190, 470, buff05);
			char buff06[100];
			sprintf_s(buff06, "5th. linepoint:%d name:%s", hslinepoint[4], hsname[4].c_str());
			DrawString(-190, 445, buff06);
		}


		//署名的なもの
		char buff3[100];
		SetColor(0, 0, 0);
		sprintf_s(buff3, "KEIO SD-Engineer");
		DrawString(130, 90, buff3);
		char buff6[100];
		SetColor(0.99, 0.54, 0);
		sprintf_s(buff6, "Tadanori Matsui");
		DrawString(340, 90, buff6);
		if (keyboardflag == 0){
			char buff4[100];
			SetColor(0, 0, 0);
			sprintf_s(buff4, "THANK YOU FOR PLAYING");
			DrawString(150, 60, buff4);
			char buff5[100];
			SetColor(0, 0, 0);
			sprintf_s(buff5, "End ::: Esc    RESTART :::    -> ");
			DrawString(150, 30, buff5);
			char buff7[100];
			SetColor(1, 0, 0);
			sprintf_s(buff7, "                                             r       s");
			DrawString(150, 30, buff7);
		}
	}
}
void displayfunc(){//描画関数
	using namespace SDGLibF;
	Before(); // Set drawing plane
	displayfunc1();//最初
	displayfunc2();//プレイ中
	displayfunc3();//最後
	After(); // Set drawn plane
}

void down(){
	if (downflag % 2 == 1){
		tt += 1;
		if (v != 0){
			if (tt % v == 0) {
				y--;
				if (y <= 0) {
					y = 1;
					v = kv;
				}
			}
		}
	}//時間経過で下に落ちるようなの
	//yの範囲の制限 xの制限はclass monoで補正済み 実際y軸方向は自動で落ちるはずなのでそんなに問題ないかも。
}

void swap(int &x, int &y){
	int k;
	k = x;
	x = y;
	y = k;
}

void cheating(){
	if (cheatingflag == 1){
		for (int i(0); i < 10; i++){
			minosequence[i + minosum] = 4;
		}
		cheatingflag = 0;
	}

}

void simulationnameinput(void){
	using namespace SDGLibF;
	ReDraw();
}

void simulation(void){//実際に動かす関数
	using namespace SDGLibF;
	down();
	minoselect(x, y, p, kind, tt, harddropflag, kv, v);
	harddrop(y, harddropflag, kv, v);
	ghostbox(x, y, p, kind, tt, harddropflag, kv, v);
	brockshadow(x, y, p, kind, tt, harddropflag, kv, v);
	cheating();

	//床の定義 //床と壁が抜けないようにこちらにも入れた
	for (int i(1); i <= 10; i++){
		f[i][0] = 1;
	}
	//壁の定義
	for (int j(0); j <= 24; j++){
		f[0][j] = 1;
		f[11][j] = 1;
	}
	//ゲームオーバーの判別、画面切り替え、ファイル出力
	if (f[5][20] == 1 || f[6][20] == 1){
		displayflag = 2; IdleFunc(simulationnameinput); keyboardflag = 1;
	}
	//uncrashable(x);
	ReDraw();
}


void tanaka(){//おまけ　初期ブロックの設定　田中
	f[3][17] = 1; f[4][17] = 1; f[5][17] = 1; f[6][17] = 1; f[7][17] = 1;
	f[3][16] = 1; f[5][16] = 1; f[7][16] = 1;
	f[3][15] = 1; f[4][15] = 1; f[5][15] = 1; f[6][15] = 1; f[7][15] = 1;
	f[3][14] = 1; f[5][14] = 1; f[7][14] = 1;
	f[3][13] = 1; f[4][13] = 1; f[5][13] = 1; f[6][13] = 1; f[7][13] = 1;
	f[5][11] = 1;
	f[3][10] = 1; f[4][10] = 1; f[5][10] = 1; f[6][10] = 1; f[7][10] = 1;
	f[3][9] = 1; f[5][9] = 1; f[7][9] = 1;
	f[3][8] = 1; f[4][8] = 1; f[5][8] = 1; f[6][8] = 1; f[7][8] = 1;
	f[5][7] = 1;
	f[5][6] = 1;
}
void taichi(){//おまけ　初期ブロックの設定　太一
	f[5][18] = 1;
	f[2][17] = 1; f[3][17] = 1; f[4][17] = 1; f[5][17] = 1; f[6][17] = 1; f[7][17] = 1; f[8][17] = 1;
	f[5][16] = 1;
	f[4][15] = 1; f[6][15] = 1;
	f[3][14] = 1;  f[7][14] = 1;
	f[2][13] = 1;  f[8][13] = 1;
	f[5][13] = 1;
	f[2][10] = 1; f[3][10] = 1; f[4][10] = 1; f[5][10] = 1; f[6][10] = 1; f[7][10] = 1; f[8][10] = 1;
}
void randombypi(){//randomライブラリが入っていない学校用の乱数生成関数
	using namespace std;
	std::string s;
	int len(0);
	s = "1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412";//種の生成のために円周率を使ってみる。
	len = s.length();
	for (int i(0); i < len; i++){
		srand(s[i] * 31);
		minosequence[i] = rand() % 7 + 1;
	}

}

/////ここからがvisual studio2008ではできない randomクラスの実装を早くもとむ
void randombyrandom(){
for (int j(0); j <= 200; j++){
std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(1, 7);
for (int i(0); i <= 6; i++){
//srand((unsigned int)time(NULL));
//minosequence[i+j*7] = (1 + (int)(rand() * 7 - 1 + 1) / 1 + RAND_MAX)%(i+1);
minosequence[i + j * 7] = distribution(generator);
std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<int> minominomino(1, 7);
minosequence[i + j * 7] = minominomino(mt);
}					 //ほんとは7個出してブロックにしたいんだけどねー
}
}
////

void definition(){//初期値定義関数
	//中身の初期値の定義
	for (int i(1); i <= 10; i++){
		for (int j(1); j <= 24; j++){
			f[i][j] = 0;
		}
	}
	//床の定義
	for (int i(1); i <= 10; i++){
		f[i][0] = 1;
	}
	//壁の定義
	for (int j(0); j <= 24; j++){
		f[0][j] = 1;
		f[11][j] = 1;
	}

	//tanaka();
	//taichi();

	//randombypi();
	randombyrandom();

	//tanaka();//初期ブロックの設定のおまけです
	//taichi();//初期ブロックの設定のおまけです

	//holdの初期値の定義
	for (int i(0); i < 8; i++){
		for (int j(0); j <= 8; j++){
			holdf[i][j] = 0;
		}
	}
	//nextの初期値の定義
	for (int i(0); i < 12; i++){
		for (int j(0); j < 25; j++){
			nextf[i][j] = 0;
		}
	}
	//ghostの初期値の定義
	for (int i(0); i < 12; i++){
		for (int j(0); j < 25; j++){
			ghostf[i][j] = 0;
		}
	}
}

void keyboardfunc(unsigned char k, int xx, int yy){
	using namespace SDGLibF;
	if (keyboardflag == 0){
		switch (k) {
		case 27:  exit(0);
			//case 'C': SetCursor(GLUT_CURSOR_RIGHT_ARROW); break;
			//case 'c': SetCursor(GLUT_CURSOR_WAIT); break;
		case 's': if (displayflag == 0) { displayflag = 1; IdleFunc(simulation); } break;
		case 'z': x--; break;
		case 'c': x++; break;
		case 'x': y--; break;
		case VK_RETURN: p++; if (p >= 4) p %= 4; break; //時計回転
		case 'l': p--; if (p <= -1) p += 4; break; //反時計回転
		case 'v': if (v <= 100)v -= 10; if (v > 100 && v <= 1000)v -= 100; if (v > 1000) v -= 1000; if (v <= 0) v = 100; break;//速度あげる
		case 'V': if (v <= 1000)v += 100; if (v > 1000) v += 1000; if (v > 10000) v = 10000; break;//速度下げる
		case 'S': downflag += 1; break;//時を止める
		case VK_BACK: swap(kind, holdkind);
		case 'r':if (displayflag == 2){//リスタート
					 displayflag = 0;
					 tt = 0;
					 kind = minosequence[minosum], holdkind = 4;
					 x = 5, y = 20, p = 0;
					 v = 500, downflag = 1;
					 linepoint = 0;
					 minosum = 0;
					 stagecolorr = 1, stagecolorg = 1, stagecolorb = 1;//ブロックの色とか
					 stagepoint = 1;
					 definition();
					 harddropflag = 0;
					 name = "";
		}break;
		case 'C':cheatingflag = 1; break;

		case VK_SPACE:harddropflag = 1;
		default:break;
		}
	}
	if (keyboardflag == 1){//まったく非効率的な極みですが、ほかに方法がわかりませんでした。
		switch (k) {
		case 'a':name += k; break;		case 'b':name += k; break;		case 'c':name += k; break;
		case 'd':name += k; break;		case 'e':name += k; break;		case 'f':name += k; break;
		case 'g':name += k; break;		case 'h':name += k; break;		case 'i':name += k; break;
		case 'j':name += k; break;		case 'k':name += k; break;		case 'l':name += k; break;
		case 'm':name += k; break;		case 'n':name += k; break;		case 'o':name += k; break;
		case 'p':name += k; break;		case 'q':name += k; break;		case 'r':name += k; break;
		case 's':name += k; break;		case 't':name += k; break;		case 'u':name += k; break;
		case 'v':name += k; break;		case 'w':name += k; break;		case 'x':name += k; break;
		case 'y':name += k; break;		case 'z':name += k; break;
		case 'A':name += k; break;		case 'B':name += k; break;		case 'C':name += k; break;
		case 'D':name += k; break;		case 'E':name += k; break;		case 'F':name += k; break;
		case 'G':name += k; break;		case 'H':name += k; break;		case 'I':name += k; break;
		case 'J':name += k; break;		case 'K':name += k; break;		case 'L':name += k; break;
		case 'M':name += k; break;		case 'N':name += k; break;		case 'O':name += k; break;
		case 'P':name += k; break;		case 'Q':name += k; break;		case 'R':name += k; break;
		case 'S':name += k; break;		case 'T':name += k; break;		case 'U':name += k; break;
		case 'V':name += k; break;		case 'W':name += k; break;		case 'X':name += k; break;
		case 'Y':name += k; break;		case 'Z':name += k; break;
		case VK_BACK: if (name.size() >= 1) name.erase(name.size() - 1); break;
		case VK_RETURN:outcsvfile(); highscore(); keyboardflag = 0; break;
		default:break;
		}
	}
}

int main(void){
	using namespace SDGLibF;
	definition();
	kind = kindrandom(tt);
	SDGLib mygraphic(800, 600, "- Fake TETRiS - (61218525 松井忠宗)", -200, 600, 0, 600.0);
	mygraphic.SetCursor(GLUT_CURSOR_WAIT);
	mygraphic.Keyboard(keyboardfunc);
	mygraphic.Display(displayfunc);
	mygraphic.Start();
	return 0;
}

/*
いい点
・nextの表示
・関数の多用による拡張のしやすさ
・配列を無理やりデカルト座標系の方向に合わせることによってグラフィックをやりやすくした。
・アルゴリズムをすべて1から考えたこと、証拠として手書きの紙がある。
・ホールド機能を追加しました。
・ホールドを使っても位置と状態は保存される。
・ホールド、ネクストそれぞれで配列を新たに作成することによって配列で重複するなどのバグがないように工夫。
・linepointでカウントすることによって例えば10段消したらスピードあげ、背景色を変更した。ステージのようなもの。
・最初から（リセット）を付けました。最後の画面からのみ移れるようにしました。
・次のやつがでるようになった。
・一気に落ちる（ハードドロップを疑似的に実装しました。（速度を最小にすることで実現）
・同時に消したときに一気に消すのを作らねばテトリスになんねぇ上から回すことで解決
・なんか床が消えちゃうことがたまに起きる→床と壁を常に定義し続けることで解決
・visual studioのverによってrandomライブラリの使用可否異なるためそれに対応できるようにした。
・すべて長い棒になるコマンドを追加しました。指定した回数だけ出した後は元の数列に戻ります。

要改善
・積み重なってる奴に横からぶつからないようにする

よりよくなるために
・連続消しによるスコアアップ
・テトリミノの出し方を7個で1ブロックにして1ブロックで全部出るようにしたい。
・自動操縦できたらいいな
・同じスタート画面でいくつかのゲームが選べるようにしたい
・または積み重なっているところの中で円が跳ね返ってると面白いかもしれないですね　派手だし。ただ積み重なっているだけでも楽しめる。
・着地する場所の表示　ゴーストブロック
・ハイスコアがあるといいねー、txtファイルまたはexcelファイルへの書き出しおよび読み出しの実装
・アイテムがあると面白いんだけどなー
・テトリミノごとに色変えて、したにくっついてからも色変えられるようにしたい。これはあれか、クラスかベクトルを使うと出来るのかな？配列の
　1つごとにたくさんの情報を持たせるイメージ
 ・対戦型にする
 ・3Dが実装できればいいなって感じ。
 ・さいしょの画面でグラフィックの変更の可否やゲームの選択を問いたい。
 ・初期ブロックの設定に関してはExcelファイルから読み込んでそのまま出力できれば面白いのではないだろうか。
 ・ぷよぷよとテトリス、文字合わせゲームとテトリスみたいに、色が4つそろったら消える、または1段そろったら消えるとかも面白そうだよね


 気づいたこと
 ・7個までを1-7で順番に割っていった余りを採用すると偏りが生じてしまうので7個で1ブロックの作成が難しい。
 ・randombypiはrandombyrandomに比べて偏りがある。なぜなら0-9までの数字を7で割ったときのあまりで考えるため
 　1,2,3,4がほかの数字よりも出やすい仕様になっている。
  ・文字入力をcmdからやらせようとすると途中で止まってしまってウィンドウを変えたり面倒なことが多いので
  　keyboardfuncの中をフラグでわけて文字入力ができるようにしました。
   ・ReDrawを入れる場所の関係で、何もないがただReDrawをし続ける関数を作っておいた方がいいようだ
   */

