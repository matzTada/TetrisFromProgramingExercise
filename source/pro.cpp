/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////3次元テトリス//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

//3Dfake tetrisの方
static int f[12][12][25];//x,y,z
static int fmino(0);
static int tt(0);
static int kind(0), holdkind(4);
static int x(3), y(3), z(10), pf(0), pr(0);
static int v(1000), downflag(1);
static int stagecount(3);//いくつ消したらステージを変更するかのカウント。
static int displayflag(0);
static int linepoint(0);
static int minosum(0), minosequence[100000];
static double stagecolorr(1), stagecolorg(1), stagecolorb(1);//ブロックの色とか
static int stagepoint(1);
static int harddropflag(0);
static int kv(0);
static int cheatingflag(0);
static double d = 0.125;
//wallのon off
static int wallflag(0);

//視点変更の係数
static double ox(150), oy(350), l(20);//座標系の定義
static double viewa(1), viewb(1);
static double viewcubea(1), viewcubeb(1.5), viewcubec(2);
//オイラー角なるものがあるようだ。
static double pi = 3.14;
static double ea(0), eb(0), ec(0);//ea,eb,ecの単位は度とする

void MI(int &x, int &y, int&z, int &pf, int &pr, int &fmino){//棒ですね。kind1
	pf = 0; pr = 0;
	if (pf == 0){
		switch (pr){
		case 0: f[x][y][z] = fmino; f[x][y][z + 1] = fmino; f[x][y][z + 2] = fmino; f[x][y][z + 3] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		default: break;
		}
	}
}
void MB(int &x, int &y, int&z, int &pf, int &pr, int &fmino){//箱ですね。kind2
	pf = 0; pr = pr % 2;
	if (pf == 0){
		switch (pr){
		case 0: f[x][y][z] = fmino; f[x][y + 1][z] = fmino; f[x][y][z + 1] = fmino; f[x][y + 1][z + 1] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y + 1 >= 6) y = 5 - 1;
			break;
		case 1: f[x][y][z] = fmino; f[x + 1][y][z] = fmino; f[x][y][z + 1] = fmino; f[x + 1][y][z + 1] = fmino;
			if (x <= 0) x = 1; if (x + 1 >= 6) x = 5 - 1;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		default: break;
		}
	}
}
void MZ(int &x, int &y, int&z, int &pf, int &pr, int &fmino){//ＳとかＺとかですね。kind3
	pf = pf % 2;
	if (pf == 0){
		switch (pr){
		case 0: f[x][y][z] = fmino; f[x][y][z + 1] = fmino; f[x][y + 1][z + 1] = fmino; f[x][y + 1][z + 2] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y + 1 >= 6) y = 5 - 1;
			break;
		case 1: f[x + 1][y][z] = fmino; f[x + 1][y][z + 1] = fmino; f[x][y][z + 1] = fmino; f[x][y][z + 2] = fmino;
			if (x <= 0) x = 1; if (x + 1 >= 6) x = 5 - 1;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		case 2: f[x][y + 1][z] = fmino; f[x][y + 1][z + 1] = fmino; f[x][y][z + 1] = fmino; f[x][y][z + 2] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y + 1 >= 6) y = 5 - 1;
			break;
		case 3: f[x][y][z] = fmino; f[x][y][z + 1] = fmino; f[x + 1][y][z + 1] = fmino; f[x + 1][y][z + 2] = fmino;
			if (x <= 0) x = 1; if (x + 1 >= 6) x = 5 - 1;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		default: break;
		}
	}
	if (pf == 1){
		switch (pr){
		case 0: f[x][y][z] = fmino; f[x][y + 1][z] = fmino; f[x][y + 1][z + 1] = fmino; f[x][y + 2][z + 1] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y + 2 >= 6) y = 5 - 2;
			break;
		case 1: f[x][y][z + 1] = fmino; f[x + 1][y][z + 1] = fmino; f[x + 1][y][z] = fmino; f[x + 2][y][z] = fmino;
			if (x <= 0) x = 1; if (x + 2 >= 6) x = 5 - 2;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		case 2: f[x][y][z + 1] = fmino; f[x][y + 1][z + 1] = fmino; f[x][y + 1][z] = fmino; f[x][y + 2][z] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y + 2 >= 6) y = 5 - 2;
			break;
		case 3: f[x][y][z] = fmino; f[x + 1][y][z] = fmino; f[x + 1][y][z + 1] = fmino; f[x + 2][y][z + 1] = fmino;
			if (x <= 0) x = 1; if (x + 2 >= 6) x = 5 - 2;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		default: break;
		}
	}
}
void MT(int &x, int &y, int&z, int &pf, int &pr, int &fmino){//ＳとかＺとかですね。kind4
	if (pf == 0){
		pr = pr % 2;
		switch (pr){
		case 0: f[x][y][z] = fmino; f[x][y + 1][z] = fmino; f[x][y + 2][z] = fmino; f[x][y + 1][z + 1] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y + 2 >= 6) y = 5 - 2;
			break;
		case 1: f[x][y][z] = fmino; f[x + 1][y][z] = fmino; f[x + 2][y][z] = fmino; f[x + 1][y][z + 1] = fmino;
			if (x <= 0) x = 1; if (x + 2 >= 6) x = 5 - 2;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		default: break;
		}
	}
	if (pf == 1){
		switch (pr){
		case 0: f[x][y][z] = fmino; f[x][y][z + 1] = fmino; f[x][y][z + 2] = fmino; f[x][y + 1][z + 1] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y + 1 >= 6) y = 5 - 1;
			break;
		case 1: f[x + 1][y][z] = fmino; f[x + 1][y][z + 1] = fmino; f[x + 1][y][z + 2] = fmino; f[x][y][z + 1] = fmino;
			if (x <= 0) x = 1; if (x + 1 >= 6) x = 5 - 1;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		case 2: f[x][y + 1][z] = fmino; f[x][y + 1][z + 1] = fmino; f[x][y + 1][z + 2] = fmino; f[x][y][z + 1] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y + 1 >= 6) y = 5 - 1;
			break;
		case 3: f[x][y][z] = fmino; f[x][y][z + 1] = fmino; f[x][y][z + 2] = fmino; f[x + 1][y][z + 1] = fmino;
			if (x <= 0) x = 1; if (x + 1 >= 6) x = 5 - 1;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		default: break;
		}
	}
	if (pf == 2){
		pr = pr % 2;
		switch (pr){
		case 0: f[x][y + 1][z] = fmino; f[x][y][z + 1] = fmino; f[x][y + 1][z + 1] = fmino; f[x][y + 2][z + 1] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y + 2 >= 6) y = 5 - 2;
			break;
		case 1: f[x + 1][y][z] = fmino; f[x][y][z + 1] = fmino; f[x + 1][y][z + 1] = fmino; f[x + 2][y][z + 1] = fmino;
			if (x <= 0) x = 1; if (x + 2 >= 6) x = 5 - 2;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		default: break;
		}
	}
}
void ML(int &x, int &y, int&z, int &pf, int &pr, int &fmino){//LとかJとかですね。kind4
	if (pf == 0){
		switch (pr){
		case 0: f[x][y][z] = fmino; f[x][y][z + 1] = fmino; f[x][y][z + 2] = fmino; f[x][y + 1][z] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y + 1 >= 6) y = 5 - 1;
			break;
		case 1: f[x][y][z] = fmino; f[x + 1][y][z] = fmino; f[x + 1][y][z + 1] = fmino; f[x + 1][y][z + 2] = fmino;
			if (x <= 0) x = 1; if (x + 1 >= 6) x = 5 - 1;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		case 2: f[x][y][z] = fmino; f[x][y + 1][z] = fmino; f[x][y + 1][z + 1] = fmino; f[x][y + 1][z + 2] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y + 1 >= 6) y = 5 - 1;
			break;
		case 3: f[x][y][z] = fmino; f[x][y][z + 1] = fmino; f[x][y][z + 2] = fmino; f[x + 1][y][z] = fmino;
			if (x <= 0) x = 1; if (x + 1 >= 6) x = 5 - 1;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		default: break;
		}
	}
	if (pf == 1){
		switch (pr){
		case 0: f[x][y][z] = fmino; f[x][y + 1][z] = fmino; f[x][y + 2][z] = fmino; f[x][y][z + 1] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y + 2 >= 6) y = 5 - 2;
			break;
		case 1: f[x][y][z] = fmino; f[x + 1][y][z] = fmino; f[x + 2][y][z] = fmino; f[x + 2][y][z + 1] = fmino;
			if (x <= 0) x = 1; if (x + 2 >= 6) x = 5 - 2;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		case 2: f[x][y][z] = fmino; f[x][y + 1][z] = fmino; f[x][y + 2][z] = fmino; f[x][y + 2][z + 1] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y + 2 >= 6) y = 5 - 2;
			break;
		case 3: f[x][y][z] = fmino; f[x + 1][y][z] = fmino; f[x + 2][y][z] = fmino; f[x][y][z + 1] = fmino;
			if (x <= 0) x = 1; if (x + 2 >= 6) x = 5 - 2;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		default: break;
		}
	}
	if (pf == 2){
		switch (pr){
		case 0: f[x][y][z] = fmino; f[x][y][z + 1] = fmino; f[x][y][z + 2] = fmino; f[x][y + 1][z + 2] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y + 1 >= 6) y = 5 - 1;
			break;
		case 1: f[x + 1][y][z] = fmino; f[x + 1][y][z + 1] = fmino; f[x + 1][y][z + 2] = fmino; f[x][y][z + 2] = fmino;
			if (x <= 0) x = 1; if (x + 1 >= 6) x = 5 - 1;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		case 2: f[x][y + 1][z] = fmino; f[x][y + 1][z + 1] = fmino; f[x][y + 1][z + 2] = fmino; f[x][y][z + 2] = fmino;
			if (x <= 0) x = 1; if (x >= 6) x = 5;
			if (y <= 0) y = 1; if (y + 1 >= 6) y = 5 - 1;
			break;
		case 3: f[x][y][z] = fmino; f[x][y][z + 1] = fmino; f[x][y][z + 2] = fmino; f[x + 1][y][z + 2] = fmino;
			if (x <= 0) x = 1; if (x + 1 >= 6) x = 5 - 1;
			if (y <= 0) y = 1; if (y >= 6) y = 5;
			break;
		default: break;
		}
	}
}

int kindrandom(int tt){//7個で1ブロックとしブロック1つ内で全種類が出るようにしたいねー
	//ゲームが始まった段階で配列を作っちゃったら楽なのかもしれない。
	int randmino;
	srand(tt);
	randmino = 1 + (int)(rand() * 5 - 1 + 1) / (1 + RAND_MAX);
	if (minosequence[minosum] == 0)return randmino;//配列を作った時になぜか0ができてしまうのでそれの解消用です。たぶんrdの範囲指定がちゃんとしてないからだと思う。
	else return minosequence[minosum];
}

void colorrandom(int tt, int minosum){//バックの色が変わるようにしようかなという企画。
	srand(tt*minosum);
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

void brockshadow(int &x, int &y, int &z, int &pf, int &pr, int &kind, int tt, int &harddropflag, int &kv, int &v){//床につく、またはほかのミノにくっついたときにラインが消せるかの判別を行う、および次を開始する
	for (int iy(y); iy <= 10; iy++){
		for (int ix(x); ix <= 10; ix++){//これだけでも実はきれいだったりする。動いてるやつから止まってるやつまで塗り潰す
			int flag1(0);
			for (int j(1); j <= z + 2; j++){
				if (flag1 == 0){
					if (f[ix][iy][j] == 2){//下から動いてるやつを調べた結果
						flag1 = 1;
						int flag2(0);
						for (int k(j - 1); k >= 0; k--){
							if (flag2 == 0){
								if (f[ix][iy][k] == 1){//上から止まっている奴を調べた結果
									flag2 = 1;
									for (int l(k + 1); l <= j - 1; l++){
										if (f[ix][iy][l] == 0){
											f[ix][iy][l] = 3;
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
}

void linedelete(int &x, int &y, int&z, int &pf, int &pr, int &fmino, int tt){//yz平面、xz平面それぞれについて考える
	using namespace SDGLibF;
	//クロスにした時のことを考えてフラグを用いる。
	int flagyz(0), flagxz(0);
	int deleetflagyz[4], deleetflagxz[4];
	int yzix[4], xziy[4];
	int yzj[4], xzj[4];
	for (int i(0); i < 4; i++){
		deleetflagyz[i] = 0;
		deleetflagxz[i] = 0;
		yzix[i] = 0;
		xziy[i] = 0;
		yzj[i] = 20;
		xzj[i] = 20;
	}
	//yz方向についての判別
	for (int ix(1); ix <= 5; ix++){
		for (int j(20); j >= 1; j--){//jを上から落とすようにしたら一気に消せるようになった。
			int count(0);
			for (int iy(1); iy <= 5; iy++){
				if (f[ix][iy][j] == 1) count++;//
			}
			if (count == 5){
				deleetflagyz[flagyz] = 1;
				yzix[flagyz] = ix;
				yzj[flagyz] = j;
				flagyz++;
			}
		}
	}
	//xz方向についての判別
	for (int iy(1); iy <= 5; iy++){
		for (int j(20); j >= 1; j--){//jを上から落とすようにしたら一気に消せるようになった。
			int count(0);
			for (int ix(1); ix <= 5; ix++){
				if (f[ix][iy][j] == 1) count++;//
			}
			if (count == 5){
				deleetflagxz[flagxz] = 1;
				xziy[flagxz] = iy;
				xzj[flagxz] = j;
				flagxz++;
			}
		}
	}
	//消す部分　かぶった時がえぐすぎる・・・。いやそうでもない。なぜならかぶった場所はz方向には何もないはずなので。
	for (int i(0); i < 4; i++){
		if (deleetflagyz[i] == 1){//消すところの本体。あるj段の合計が10だったら、上の段を下の段に移すのを繰り返す。
			linepoint++;
			for (int ja(yzj[i]); ja <= 19; ja++){
				for (int k(1); k <= 10; k++){
					f[yzix[i]][k][ja] = f[yzix[i]][k][ja + 1];
				}
			}
			if (linepoint % stagecount == 0){//いわばステージ的な　ホニャララこ消すごとに背景色が変わり速度が上がる。
				colorrandom(tt, minosum);
				if (v <= 30)v -= 1; if (v > 30 && v <= 100)v -= 10; if (v > 100 && v <= 1000)v -= 100; if (v > 1000) v -= 1000; if (v <= 0) v = 10;
				stagepoint++;
			}
		}
		if (deleetflagxz[i] == 1){//消すところの本体。あるj段の合計が10だったら、上の段を下の段に移すのを繰り返す。
			linepoint++;
			for (int ja(xzj[i]); ja <= 19; ja++){
				for (int k(1); k <= 5; k++){
					f[k][xziy[i]][ja] = f[k][xziy[i]][ja + 1];
				}
			}
			if (linepoint % stagecount == 0){//いわばステージ的な　ホニャララこ消すごとに背景色が変わり速度が上がる。
				colorrandom(tt, minosum);
				if (v <= 30)v -= 1; if (v > 30 && v <= 100)v -= 10; if (v > 100 && v <= 1000)v -= 100; if (v > 1000) v -= 1000; if (v <= 0) v = 10;
				stagepoint++;
			}
		}

	}
}

void stopnext(int &x, int &y, int&z, int &pf, int &pr, int &kind, int tt, int &harddropflag, int &kv, int &v){//床につく、またはほかのミノにくっついたときにラインが消せるかの判別を行う、および次を開始する
	for (int j(0); j <= 20; j++){
		for (int ix(1); ix <= 5; ix++){
			for (int iy(1); iy <= 5; iy++){
				if (f[ix][iy][j] == 1 && f[ix][iy][j + 1] == 2){//もし止まったら…。
					harddropflag = 0;
					v = kv;
					for (int j(1); j <= 20; j++){//動いてた奴を1に書き換える
						for (int ix(1); ix <= 5; ix++){
							for (int iy(1); iy <= 5; iy++){
								if (f[ix][iy][j] == 2) f[ix][iy][j] = 1;
							}
						}
					}
					linedelete(x, y, z, pf, pr, fmino, tt);//行が消せるかどうかを判断して、次を始める。消した後は新しいのを始める
					kind = kindrandom(tt);
					++minosum;
					x = 3; y = 3; z = 10; pf = 0; pr = 0;
					//colorrandom(tt, minosum);
				}
			}
		}
	}
}

void minoselect(int &x, int &y, int&z, int &pf, int &pr, int &kind, int &tt, int &harddropflag, int &kv, int &v){
	fmino = 2;
	switch (kind){
	case 1:MI(x, y, z, pf, pr, fmino); break;
	case 2:MB(x, y, z, pf, pr, fmino); break;
	case 3:MZ(x, y, z, pf, pr, fmino); break;
	case 4:MT(x, y, z, pf, pr, fmino); break;
	case 5:ML(x, y, z, pf, pr, fmino); break;
	default:break;
	}
	stopnext(x, y, z, pf, pr, kind, tt, harddropflag, kv, v);
}

void cleanup(){//動いてるやつをきれいにする
	for (int j(1); j <= 25; j++){
		for (int ix(1); ix <= 10; ix++){
			for (int iy(1); iy <= 10; iy++){
				if (f[ix][iy][j] == 2 || f[ix][iy][j] == 3 || f[ix][iy][j] == 4) f[ix][iy][j] = 0;
			}
		}
	}
}

double ctx(double x, double y, double z){//coridinate transformation座標変換関数
	//回転行列からの回転のやつ
	x -= 3;
	y -= 3;
	z -= 13;
	double a = (cos(ea *pi / 180)*cos(eb *pi / 180)*cos(ec *pi / 180) - sin(ea *pi / 180)*sin(ec *pi / 180))*x + (-cos(ea *pi / 180)*cos(eb *pi / 180)*sin(ec *pi / 180) - sin(ea *pi / 180)*cos(ec *pi / 180))*y + cos(ea *pi / 180)*sin(eb *pi / 180)*z;
	double b = (sin(ea *pi / 180)*cos(eb *pi / 180)*cos(ec *pi / 180) + cos(ea *pi / 180)*sin(ec *pi / 180))*x + (-sin(ea *pi / 180)*cos(eb *pi / 180)*sin(ec *pi / 180) + cos(ea *pi / 180)*cos(ec *pi / 180))*y + sin(ea *pi / 180)*sin(eb *pi / 180)*z;
	double c = (-sin(eb *pi / 180)*cos(ec *pi / 180))*x + (sin(eb *pi / 180)*sin(ec *pi / 180))*y + (cos(eb *pi / 180))*z;
	return (ox - viewcubec * l*a + viewcubeb * l*b)*viewa;
}

double cty(double x, double y, double z){//coridinate transformation座標変換関数
	//回転行列からの回転のやつ
	x -= 3;
	y -= 3;
	z -= 13;
	double a = (cos(ea *pi / 180)*cos(eb *pi / 180)*cos(ec *pi / 180) - sin(ea *pi / 180)*sin(ec *pi / 180))*x + (-cos(ea *pi / 180)*cos(eb *pi / 180)*sin(ec *pi / 180) - sin(ea *pi / 180)*cos(ec *pi / 180))*y + cos(ea *pi / 180)*sin(eb *pi / 180)*z;
	double b = (sin(ea *pi / 180)*cos(eb *pi / 180)*cos(ec *pi / 180) + cos(ea *pi / 180)*sin(ec *pi / 180))*x + (-sin(ea *pi / 180)*cos(eb *pi / 180)*sin(ec *pi / 180) + cos(ea *pi / 180)*cos(ec *pi / 180))*y + sin(ea *pi / 180)*sin(eb *pi / 180)*z;
	double c = (-sin(eb *pi / 180)*cos(ec *pi / 180))*x + (sin(eb *pi / 180)*sin(ec *pi / 180))*y + (cos(eb *pi / 180))*z;
	return (oy - viewcubea * l*a - viewcubeb*l*b + viewcubec * l*c)*viewb + 200;
}

void drawcube(int x, int y, int z, double r, double g, double b){//25×25のboxを作るための関数…　上から見てうまくいくのかは正直わかりません。
	using namespace SDGLibF;//drawcircleがあるのにdrawboxがなく今まで不便だったので簡易版ですが作ってみました。
	SetColor(r, g, b);
	//上側の描写
	//(x,y,z)の位置を画面上のピクセル単位に表すと…　[x,y,z]→(x,y)=(o-2*l*x+1.5*l*y,o-1*l*x-1.5*l*y+2*l*z)
	//座標変換の関数を定義しておいた。
	//下
	DrawLine(2, ctx(x + 1 - d, y + 1 - d, z + 0 + d), cty(x + 1 - d, y + 1 - d, z + 0 + d), ctx(x + 0 + d, y + 1 - d, z + 0 + d), cty(x + 0 + d, y + 1 - d, z + 0 + d));//　1
	DrawLine(2, ctx(x + 0 + d, y + 0 + d, z + 0 + d), cty(x + 0 + d, y + 0 + d, z + 0 + d), ctx(x + 0 + d, y + 1 - d, z + 0 + d), cty(x + 0 + d, y + 1 - d, z + 0 + d));//　2
	DrawLine(2, ctx(x + 1 - d, y + 0 + d, z + 0 + d), cty(x + 1 - d, y + 0 + d, z + 0 + d), ctx(x + 0 + d, y + 0 + d, z + 0 + d), cty(x + 0 + d, y + 0 + d, z + 0 + d));//　3
	DrawLine(2, ctx(x + 1 - d, y + 0 + d, z + 0 + d), cty(x + 1 - d, y + 0 + d, z + 0 + d), ctx(x + 1 - d, y + 1 - d, z + 0 + d), cty(x + 1 - d, y + 1 - d, z + 0 + d));//　4
	//縦線
	DrawLine(2, ctx(x + 0 + d, y + 1 - d, z + 0 + d), cty(x + 0 + d, y + 1 - d, z + 0 + d), ctx(x + 0 + d, y + 1 - d, z + 1 + d), cty(x + 0 + d, y + 1 - d, z + 1 + d));//　5
	DrawLine(2, ctx(x + 0 + d, y + 0 + d, z + 0 + d), cty(x + 0 + d, y + 0 + d, z + 0 + d), ctx(x + 0 + d, y + 0 + d, z + 1 + d), cty(x + 0 + d, y + 0 + d, z + 1 + d));//　6
	DrawLine(2, ctx(x + 1 - d, y + 0 + d, z + 0 + d), cty(x + 1 - d, y + 0 + d, z + 0 + d), ctx(x + 1 - d, y + 0 + d, z + 1 + d), cty(x + 1 - d, y + 0 + d, z + 1 + d));//　7
	DrawLine(2, ctx(x + 1 - d, y + 1 - d, z + 0 + d), cty(x + 1 - d, y + 1 - d, z + 0 + d), ctx(x + 1 - d, y + 1 - d, z + 1 + d), cty(x + 1 - d, y + 1 - d, z + 1 + d));//　8
	//上
	DrawLine(2, ctx(x + 1 - d, y + 1 - d, z + 1 + d), cty(x + 1 - d, y + 1 - d, z + 1 + d), ctx(x + 0 + d, y + 1 - d, z + 1 + d), cty(x + 0 + d, y + 1 - d, z + 1 + d));//　9
	DrawLine(2, ctx(x + 0 + d, y + 0 + d, z + 1 + d), cty(x + 0 + d, y + 0 + d, z + 1 + d), ctx(x + 0 + d, y + 1 - d, z + 1 + d), cty(x + 0 + d, y + 1 - d, z + 1 + d));//　10
	DrawLine(2, ctx(x + 1 - d, y + 0 + d, z + 1 + d), cty(x + 1 - d, y + 0 + d, z + 1 + d), ctx(x + 0 + d, y + 0 + d, z + 1 + d), cty(x + 0 + d, y + 0 + d, z + 1 + d));//　11
	DrawLine(2, ctx(x + 1 - d, y + 0 + d, z + 1 + d), cty(x + 1 - d, y + 0 + d, z + 1 + d), ctx(x + 1 - d, y + 1 - d, z + 1 + d), cty(x + 1 - d, y + 1 - d, z + 1 + d));//　12
}

void paintcubetop(int x, int y, int z, double r, double g, double b){//上側の塗りつぶし
	using namespace SDGLibF;
	SetColor(r, g, b);
	//上
	for (double i(1.625); i <= 4; i += 0.250){
		DrawLine(2, ctx(x + 1 - d * (i), y + 1 - d * (i), z + 1 + 0.125), cty(x + 1 - d * (i), y + 1 - d * (i), z + 1 + 0.125), ctx(x + 0 + d * (i), y + 1 - d * (i), z + 1 + 0.125), cty(x + 0 + d * (i), y + 1 - d * (i), z + 1 + 0.125));//　9
		DrawLine(2, ctx(x + 0 + d * (i), y + 0 + d * (i), z + 1 + 0.125), cty(x + 0 + d * (i), y + 0 + d * (i), z + 1 + 0.125), ctx(x + 0 + d * (i), y + 1 - d * (i), z + 1 + 0.125), cty(x + 0 + d * (i), y + 1 - d * (i), z + 1 + 0.125));//　10
		DrawLine(2, ctx(x + 1 - d * (i), y + 0 + d * (i), z + 1 + 0.125), cty(x + 1 - d * (i), y + 0 + d * (i), z + 1 + 0.125), ctx(x + 0 + d * (i), y + 0 + d * (i), z + 1 + 0.125), cty(x + 0 + d * (i), y + 0 + d * (i), z + 1 + 0.125));//　11
		DrawLine(2, ctx(x + 1 - d * (i), y + 0 + d * (i), z + 1 + 0.125), cty(x + 1 - d * (i), y + 0 + d * (i), z + 1 + 0.125), ctx(x + 1 - d * (i), y + 1 - d * (i), z + 1 + 0.125), cty(x + 1 - d * (i), y + 1 - d * (i), z + 1 + 0.125));//　12
	}
}

void displayfunc1(){
	if (displayflag == 0){
		using namespace SDGLibF;
		char buff[100];
		SetColor(0, 0, 0);
		sprintf_s(buff, "3Dfake tetris:::press '  ' to START ::: ");
		DrawString(220, 60, buff);
		char buff7[100];
		SetColor(1, 0, 0);
		sprintf_s(buff7, "                                   q");
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




void drawbox(int x, int y, double r, double g, double b){//25×25のboxを作るための関数を作りました。
	using namespace SDGLibF;//drawcircleがあるのにdrawboxがなく今まで不便だったので簡易版ですが作ってみました。
	SetColor(r, g, b);
	DrawLine(2, x * 25 + 1.5, y * 25 + 1.5, x * 25 + 1.5, y * 25 + 23.5);//left
	DrawLine(2, x * 25 + 1.5, y * 25 + 23.5, x * 25 + 23.5, y * 25 + 23.5);//top
	DrawLine(2, x * 25 + 23.5, y * 25 + 1.5, x * 25 + 23.5, y * 25 + 23.5);//right
	DrawLine(2, x * 25 + 1.5, y * 25 + 1.5, x * 25 + 23.5, y * 25 + 1.5);//botom
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
void displayfunc2boxes(){
	using namespace SDGLibF;
	//ブロックの描画
	for (int j(1); j <= 23; j++){
		for (int ix(1); ix <= 5; ix++){
			for (int iy(1); iy <= 5; iy++){
				int depth(j);
				if (depth >= 21)depth -= 10;
				if (depth >= 11)depth -= 10;
				switch (f[ix][iy][j]){
				case 1: drawcube(ix, iy, j, 0, 0, 0.7 + stagecolorb*0.3); paintcubetop(ix, iy, j, 0.3 + 0.7*stagecolorr*0.1*depth, 0.3 + 0.7*stagecolorg*0.1*depth, 1); break;//動いてないほうは青くしました。
				case 2: drawcube(ix, iy, j, 0.7 + stagecolorr*0.3, 0, 0); paintcubetop(ix, iy, j, 1, 0.3 + 0.7*stagecolorg*0.1*depth, 0.3 + 0.7*stagecolorb*0.1*depth); break;//動いてる方は赤くしました。
				case 3: drawcube(ix, iy, j, 0.85, 0.85, 0.85); break;//ブロックシャドーのまっすぐの描画灰色
				default: break;
				}
			}
		}
	}
}
void displayfunc2wall(){
	using namespace SDGLibF;
	if (wallflag == 1){
		//壁の描画
		for (int j(0); j <= 10; j++){
			//yz壁
			for (int iy(0); iy <= 5; iy++){
				drawcube(0, iy, j, 0, 1, 0);
				paintcubetop(0, iy, j, 0, 0.7 + 0.3*stagecolorg*0.05*j, 0);
			}
			//xz壁	
			for (int ix(0); ix <= 5; ix++){
				drawcube(ix, 0, j, 0, 1, 0);
				paintcubetop(ix, 0, j, 0, 0.7 + 0.3*stagecolorg*0.05*j, 0);
			}
		}
	}
}
void displayfunc2floor(){
	using namespace SDGLibF;
	//床の描画
	for (int ix(1); ix <= 5; ix++){
		for (int iy(1); iy <= 5; iy++){
			drawcube(ix, iy, 0, 0, 0.7 + stagecolorg*0.3, 0);
			paintcubetop(ix, iy, 0, 0, 0.7 + stagecolorg*0.3, 0);
		}
	}
}

//overviewの方
static int of[12][12];//overviewの方
static double ofj[12][12];
//overviewの作成z軸方向から見た時のx,yの射影？x,yがイメージに合わせるとx↓y→になってるので難しい…
void overview(){
	for (int iy(1); iy <= 5; iy++){
		for (int ix(1); ix <= 5; ix++){
			int flag2(0);//上から止まってるやつを調べる
			for (int k(23); k >= 1; k--){
				if (flag2 == 0){
					if (f[ix][iy][k] == 1){//上から止まっている奴を調べた結果
						flag2 = 1;
						of[ix][iy] = 1;
						ofj[ix][iy] = k;
					}
				}
			}
			int flag3(0);//下から動いてるやつを調べる
			for (int k(1); k <= 23; k++){
				if (flag3 == 0){
					if (f[ix][iy][k] == 2){//上から止まっている奴を調べた結果
						flag3 = 1;
						of[ix][iy] = 2;
						ofj[ix][iy] = k;
					}
				}
			}
		}
	}

}
void ofcleanup(){//動いてるやつをきれいにする
	for (int j(1); j <= 10; j++){
		for (int i(1); i <= 10; i++){
			if (of[i][j] == 2 || of[i][j] == 3 || of[i][j] == 1) of[i][j] = 0;
		}
	}
}
void displayfunc2overview(){
	using namespace SDGLibF;
	for (int j(1); j <= 5; j++){
		for (int i(1); i <= 5; i++){
			if (ofj[i][j] >= 11)ofj[i][j] -= 10;
			if (of[i][j] == 1)drawbox(-i + 25, -j + 20, 0, 0, 0.3 + 0.7*0.1*ofj[i][j]);
			if (of[i][j] == 2)drawbox(-i + 25, -j + 20, 0.3 + 0.7*0.1*ofj[i][j], 0, 0);
			if (of[i][j] == 1)drawboxpaint(-i + 25, -j + 20, 0.3 + 0.7*stagecolorr*0.1*ofj[i][j], 0.3 + 0.7*stagecolorg*0.1*ofj[i][j], 1);
			if (of[i][j] == 2)drawboxpaint(-i + 25, -j + 20, 1, 0.3 + 0.7*stagecolorg*0.1*ofj[i][j], 0.3 + 0.7*stagecolorb*0.1*ofj[i][j]);
		}
	}
	//枠
	for (int k(0); k < 6; k++){
		drawbox(-k + 25, 0 + 20, 0, 1, 0);//
		drawbox(0 + 25, -k + 20, 0, 1, 0);//
	}

	SetColor(1, 0, 0);//x軸
	DrawLine(3, 500 - 5, 525 + 2, 650 + 2, 525 + 2);
	SetColor(0, 0, 1);//y軸
	DrawLine(3, 650 + 2, 375 - 2, 650 + 2, 525 + 2);
	ofcleanup();
}


//xz方向
static int ofxz[6][25];//overviewの方
static double ofxzj[6][25];
void overviewxz(){
	for (int iy(1); iy <= 5; iy++){
		for (int k(1); k <= 15; k++){
			for (int ix(1); ix <= 5; ix++){
				if (f[ix][iy][k] == 1 || f[ix][iy][k] == 2) {
					ofxz[ix][k] = f[ix][iy][k];
					ofxzj[ix][k] = iy;
				}
				/*
				int flag2(0);//上から止まってるやつを調べる
				for (int iy(5); iy >= 1; iy--){
				if (flag2 == 0){
				if (f[ix][iy][k] == 1){//上から止まっている奴を調べた結果
				flag2 = 1;
				ofxz[ix][k] = 1;
				ofxzj[ix][k] = y;
				}
				}
				}
				int flag3(0);//下から動いてるやつを調べる
				for (int iy(1); iy <= 5; iy++){
				if (flag3 == 0){
				if (f[ix][iy][k] == 2){//上から止まっている奴を調べた結果
				flag3 = 1;
				ofxz[ix][k] = 2;
				ofxzj[ix][k] = iy;
				}
				}
				}
				*/
			}
		}
	}
}
void ofcleanupxz(){//動いてるやつをきれいにする
	for (int j(1); j <= 15; j++){
		for (int i(1); i <= 6; i++){
			if (ofxz[i][j] == 2 || ofxz[i][j] == 3 || ofxz[i][j] == 1) ofxz[i][j] = 0;
		}
	}
}
void displayfunc2overviewxz(){
	using namespace SDGLibF;
	for (int j(1); j <= 15; j++){
		for (int i(1); i <= 5; i++){
			if (ofxzj[i][j] >= 11)ofxzj[i][j] -= 10;
			if (ofxz[i][j] == 1)drawbox(-i + 25 + 7, j + 10, 0, 0, 0.5 + 0.5*0.2*ofxzj[i][j]);
			if (ofxz[i][j] == 2)drawbox(-i + 25 + 7, j + 10, 0.5 + 0.5*0.2*ofxzj[i][j], 0, 0);
			//if (ofxz[i][j] == 1)drawboxpaint(-i + 25+7, j + 10, 0.3 + 0.7*stagecolorr*0.1*ofxzj[i][j], 0.3 + 0.7*stagecolorg*0.1*ofxzj[i][j], 1);
			//if (ofxz[i][j] == 2)drawboxpaint(-i + 25+7, j + 10, 1, 0.3 + 0.7*stagecolorg*0.1*ofxzj[i][j], 0.3 + 0.7*stagecolorb*0.1*ofxzj[i][j]);
		}
	}
	//枠
	for (int k(0); k < 6; k++){
		drawbox(-k + 25 + 7, 0 + 10, 0, 1, 0);//
	}
	for (int k(0); k < 11; k++){
		drawbox(0 + 25 + 7, k + 10, 0, 1, 0);//
	}

	SetColor(1, 0, 0);//x軸
	DrawLine(3, 500 - 5 + 175, 375 - 2 - 125, 650 + 2 + 175, 375 - 2 - 125);
	SetColor(0, 1, 0);//z軸
	DrawLine(3, 650 + 2 + 175, 375 - 2 - 125, 650 + 2 + 175, 650 + 2 - 125);
	ofcleanupxz();
}

//yz方向
static int ofyz[6][25];//overviewの方
static double ofyzj[6][25];
void overviewyz(){
	for (int ix(1); ix <= 5; ix++){
		for (int k(1); k <= 15; k++){
			for (int iy(1); iy <= 5; iy++){
				if (f[ix][iy][k] == 1 || f[ix][iy][k] == 2) {
					ofyz[iy][k] = f[ix][iy][k];
					ofyzj[iy][k] = ix;
				}
			}
		}
	}
}
void ofcleanupyz(){//動いてるやつをきれいにする
	for (int j(1); j <= 15; j++){
		for (int i(1); i <= 6; i++){
			if (ofyz[i][j] == 2 || ofyz[i][j] == 3 || ofyz[i][j] == 1) ofyz[i][j] = 0;
		}
	}
}
void displayfunc2overviewyz(){
	using namespace SDGLibF;
	for (int j(1); j <= 15; j++){
		for (int i(1); i <= 5; i++){
			if (ofyzj[i][j] >= 11)ofyzj[i][j] -= 10;
			if (ofyz[i][j] == 1) drawbox(i + 25 + 7 + 2, j + 10, 0, 0, 0.5 + 0.5*0.2*ofyzj[i][j]);
			if (ofyz[i][j] == 2) drawbox(i + 25 + 7 + 2, j + 10, 0.5 + 0.5*0.2*ofyzj[i][j], 0, 0);
			//if (ofyz[i][j] == 1)drawboxpaint(-i + 25+7, j + 10, 0.3 + 0.7*stagecolorr*0.1*ofyzj[i][j], 0.3 + 0.7*stagecolorg*0.1*ofyzj[i][j], 1);
			//if (ofyz[i][j] == 2)drawboxpaint(-i + 25+7, j + 10, 1, 0.3 + 0.7*stagecolorg*0.1*ofyzj[i][j], 0.3 + 0.7*stagecolorb*0.1*ofyzj[i][j]);
		}
	}
	//枠
	for (int k(0); k < 6; k++){
		drawbox(k + 25 + 7 + 2, 0 + 10, 0, 1, 0);//
	}
	for (int k(0); k < 11; k++){
		drawbox(0 + 25 + 7 + 2, k + 10, 0, 1, 0);//
	}

	SetColor(0, 0, 1);//y軸
	DrawLine(3, 500 - 5 + 175 + 175, 375 - 2 - 125, 650 + 2 + 175 + 175, 375 - 2 - 125);
	SetColor(0, 1, 0);//z軸
	DrawLine(3, 650 + 2 + 175 + 25, 375 - 2 - 125, 650 + 2 + 175 + 25, 650 + 2 - 125);
	ofcleanupyz();
}

void displayfunc2keycon(){
	using namespace SDGLibF;
	char buff5[100], buff6[100], buff7[100], buff8[100], buff9[100];
	SetColor(1, 0, 1);
	sprintf_s(buff9, "[a,4 : x++] [w,8 : y--] [d,6 : x--] [x,2 : y++] [s,5 : z--] [Space,0 : harddrop]");
	sprintf_s(buff8, "[Enter : frontchange] [ [ : z-rotation] [Backspace : holdchange]");
	sprintf_s(buff7, "[v : speedup] [V : speeddown] [S : pose]");
	sprintf_s(buff6, "[y : rotation ea+] [h : eb+] [g : ec+] [o : parallel+] [l : zoomin] [R : viewreset]");
	sprintf_s(buff5, "[Y : rotation ea-] [H : eb-] [G : ec-] [O : parallel-] [L : zoomout] [W : wall]");

	DrawString(400, 60, buff9);
	DrawString(400, 35, buff8);
	DrawString(400, 0, buff7);
	DrawString(400, -35, buff6);
	DrawString(400, -60, buff5);
}
void displayfunc2(){
	if (displayflag == 1){
		using namespace SDGLibF;
		//時間の表示、速度かけ数の表示
		//座標軸の描画
		SetColor(1, 0, 0);//x軸
		DrawLine(3, ctx(1, 1, 1), cty(1, 1, 1), ctx(25, 1, 1), cty(25, 1, 1));
		SetColor(0, 0, 1);//y軸
		DrawLine(3, ctx(1, 1, 1), cty(1, 1, 1), ctx(1, 25, 1), cty(1, 25, 1));
		SetColor(0, 0.7, 0);//z軸
		DrawLine(3, ctx(1, 1, 1), cty(1, 1, 1), ctx(1, 1, 25), cty(1, 1, 25));

		displayfunc2floor();
		displayfunc2wall();
		displayfunc2overview();
		displayfunc2overviewxz();
		displayfunc2overviewyz();

		displayfunc2boxes();//分けることでプログラムをすっきりかけるようにしたつもりです
		displayfunc2keycon();
		//各パラメータの描画
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
		//視点変更描画パラメータの表示
		char buff6[100], buff7[100], buff8[100], buff9[100];
		SetColor(0, 0, 1);
		sprintf_s(buff6, "ox=%3.0f oy=%3.0f l=%3.1f", ox, oy, l);
		DrawString(400, 200, buff6);
		sprintf_s(buff7, "viewa=%3.2f view=%3.2f", viewa, viewb);
		DrawString(400, 170, buff7);
		sprintf_s(buff8, "viewcubea=%3.2f viewcubeb=%3.2f viewcubec=%3.2f", viewcubea, viewcubeb, viewcubec);
		DrawString(400, 140, buff8);
		sprintf_s(buff9, "ea=%3.0f     eb=%3.0f     ec=%3.0f", ea, eb, ec);
		DrawString(400, 110, buff9);


		cleanup();
	}
}
void displayfunc3(){
	if (displayflag == 2){
		using namespace SDGLibF;
		char buff[100], buff1[100];
		SetColor(0, 0, 0);
		sprintf_s(buff, "GAME OVER");
		DrawString(100, 300, buff);
		sprintf_s(buff1, "your score :::   linepoint : %d minosum=%d", linepoint, minosum);
		DrawString(130, 120, buff1);
		//署名的な
		char buff3[100];
		SetColor(0, 0, 0);
		sprintf_s(buff3, "KEIO SD-Engineer");
		DrawString(130, 90, buff3);
		char buff6[100];
		SetColor(0.99, 0.54, 0);
		sprintf_s(buff6, "Tadanori Matsui");
		DrawString(340, 90, buff6);
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
		sprintf_s(buff7, "                                             r       q");
		DrawString(150, 30, buff7);
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
				z--;
				if (z <= 0) z = 1;
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


/////ここからがvisual studio2008ではできない randomクラスの実装を早くもとむ
void randombyrandom(){
	for (int j(0); j <= 200; j++){
		std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution(1, 5);
		for (int i(0); i <= 4; i++){
			minosequence[i + j * 5] = distribution(generator);
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_int_distribution<int> minominomino(1, 5);
			minosequence[i + j * 5] = minominomino(mt);
		}
	}
}


void randombypi(){//randomライブラリが入っていない学校用の乱数生成関数
	using namespace std;
	std::string s;
	int len(0);
	s = "1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412";//種の生成のために円周率を使ってみる。
	len = s.length();
	for (int i(0); i < len; i++){
		srand(s[i] * 31);
		minosequence[i] = rand() % 5 + 1;
	}

}

void cheating(){
	if (cheatingflag == 1){
		for (int i(0); i < 10; i++){
			minosequence[i + minosum] = 1;
		}
		cheatingflag = 0;
	}
}
void simulation2(void){
	using namespace SDGLibF;
	ReDraw();
}
void simulation(void){//実際に動かす関数
	using namespace SDGLibF;
	down();
	minoselect(x, y, z, pf, pr, kind, tt, harddropflag, kv, v);
	harddrop(z, harddropflag, kv, v);
	brockshadow(x, y, z, pf, pr, kind, tt, harddropflag, kv, v);
	cheating();
	overview();
	overviewxz();
	overviewyz();

	//床の定義
	for (int ix(0); ix <= 5; ix++){
		for (int iy(0); iy <= 5; iy++){
			f[ix][iy][0] = 1;
		}
	}
	//壁の定義
	for (int j(0); j <= 21; j++){
		//yz壁
		for (int iy(0); iy <= 5; iy++){
			f[0][iy][0] = 1;
		}
		//xz壁	
		for (int ix(0); ix <= 5; ix++){
			f[ix][0][0] = 1;
		}
	}
	//画面切り替え
	if (f[3][3][10] == 1){
		displayflag = 2; IdleFunc(simulation2);
	}
	ReDraw();
}


void definition(){//初期値定義関数
	//中身の初期値の定義
	for (int j(1); j <= 20; j++){
		for (int ix(0); ix <= 10; ix++){
			for (int iy(0); iy <= 10; iy++){
				f[ix][iy][j] = 0;
			}
		}
	}
	//床の定義
	for (int ix(0); ix <= 10; ix++){
		for (int iy(0); iy <= 10; iy++){
			f[ix][iy][0] = 1;
		}
	}
	//壁の定義
	for (int j(0); j <= 21; j++){
		//yz壁
		for (int iy(0); iy <= 10; iy++){
			f[0][iy][0] = 1;
		}
		//xz壁	
		for (int ix(0); ix <= 10; ix++){
			f[ix][0][0] = 1;
		}
	}
	randombyrandom();
	//randombypi();

	//overview
	for (int i(0); i < 12; i++){
		for (int j(0); j < 12; j++){
			of[i][j] = 0;
			ofj[i][j] = 0;
		}
	}

	//overviewxz
	for (int i(0); i < 6; i++){
		for (int j(0); j < 25; j++){
			ofxz[i][j] = 0;
			ofxzj[i][j] = 0;
		}
	}
	//overviewxz
	for (int i(0); i < 6; i++){
		for (int j(0); j < 25; j++){
			ofyz[i][j] = 0;
			ofyzj[i][j] = 0;
		}
	}
}

void keyboardfunc(unsigned char k, int xx, int yy){
	using namespace SDGLibF;
	switch (k) {
	case 27:  exit(0);
	case 'q': displayflag = 1; IdleFunc(simulation); break;
		//移動
		//テンキーがあるとき
	case '4': x++; break;
	case '8': y--; break;
	case '6': x--; break;
	case '2': y++; break;
	case '5': z--; break;
	case '0':harddropflag = 1; break;
		//テンキーがないとき
	case 'a': x++; break;
	case 'w': y--; break;
	case 'd': x--; break;
	case 'x': y++; break;
	case 's': z--; break;
	case VK_SPACE: harddropflag = 1; break;
	case VK_RETURN: pf++; if (pf >= 3) pf %= 3; break; //pfの変更　正面の図形の変更
	case '[': pr++; if (pr >= 4) pr %= 4; break; //z軸まわり回転
	case 'v': if (v <= 100)v -= 10; if (v > 100 && v <= 1000)v -= 100; if (v > 1000) v -= 1000; if (v <= 0) v = 10; break;//速度あげる
	case 'V': if (v <= 1000)v += 100; if (v > 1000) v += 1000; if (v > 10000) v = 10000; break;//速度下げる
	case 'S': downflag += 1; break;//時を止める
	case VK_BACK: swap(kind, holdkind);
	case 'r':if (displayflag == 2){
				 displayflag = 0;
				 tt = 0;
				 kind = minosequence[minosum], holdkind = 1;
				 x = 3, y = 3, z = 10, pf = 0, pr = 0;
				 v = 500, downflag = 1;
				 linepoint = 0;
				 minosum = 0;
				 stagecolorr = 1, stagecolorg = 1, stagecolorb = 1;//ブロックの色とか
				 stagepoint = 1;
				 definition();
				 harddropflag = 0;
	}break;
	case 'C':cheatingflag = 1; break;
		//視点変更系
	case 'u':ox += 10; break;
	case 'j':oy += 10; break;
		//case 'm':viewcubec += 0.1; break;
		//case 'i':viewa += 0.1; break;
		//case 'k':viewb += 0.1; break;
		//case 'o':ox += 10; break;
	case 'l':l += 0.1; break;//ズーム
	case 'U':ox -= 10; break;
	case 'J':oy -= 10; break;
		//case 'M':viewcubec -= 0.1; break;
		//case 'I':viewa -= 0.1; break;
		//case 'K':viewb -= 0.1; break;
		//case 'O':o -= 10; break;
	case 'L':l -= 0.1; break;
	case 'y':ea += 10; break;
	case 'h':eb += 10; break;
	case 'g':ec += 10; break;
	case 'Y':ea -= 10; break;
	case 'H':eb -= 10; break;
	case 'G':ec -= 10; break;
	case 'R':ox = 150, oy = 350, l = 20, viewa = 1, viewb = 1, viewcubea = 1, viewcubeb = 1.5, viewcubec = 2, ea = 0, eb = 0, ec = 0; break;//視点変更リセット
		//壁のon off
	case 'W':wallflag++; if (wallflag >= 2) wallflag %= 2;
	default: break;
	}
}

int main(void){
	using namespace SDGLibF;
	definition();
	kind = kindrandom(tt);
	SDGLib mygraphic(1300, 700, "- Fake TETRiS 3D- (61218525 松井忠宗)", -100, 1200, -100, 650);
	mygraphic.SetCursor(GLUT_CURSOR_WAIT);
	mygraphic.Display(displayfunc);
	mygraphic.Keyboard(keyboardfunc);
	mygraphic.Start();
	return 0;
}

/*
・座標変換関数の定義3次元配列から2次元のウィンドウ上に表示させる
・ブロックの色を座標によって濃淡をつけることによって遠近感を作る
・overviewと対応が取れてるようになっています。
・オイラー角を用いて視点の回転ができるようにしました。

・視点の変更ができたらよいですな…ctx,ctyをいじれば行けるかもしれない
ctx,ctyは座標系を変更してるわけじゃないので平行移動気味になる。
回転行列を考えて座標を変更してdrawcubeの中を変えてみる。
*/
