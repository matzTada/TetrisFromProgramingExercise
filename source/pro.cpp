/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////3�����e�g���X//////////////////////////////////////////////////////////
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

//3Dfake tetris�̕�
static int f[12][12][25];//x,y,z
static int fmino(0);
static int tt(0);
static int kind(0), holdkind(4);
static int x(3), y(3), z(10), pf(0), pr(0);
static int v(1000), downflag(1);
static int stagecount(3);//������������X�e�[�W��ύX���邩�̃J�E���g�B
static int displayflag(0);
static int linepoint(0);
static int minosum(0), minosequence[100000];
static double stagecolorr(1), stagecolorg(1), stagecolorb(1);//�u���b�N�̐F�Ƃ�
static int stagepoint(1);
static int harddropflag(0);
static int kv(0);
static int cheatingflag(0);
static double d = 0.125;
//wall��on off
static int wallflag(0);

//���_�ύX�̌W��
static double ox(150), oy(350), l(20);//���W�n�̒�`
static double viewa(1), viewb(1);
static double viewcubea(1), viewcubeb(1.5), viewcubec(2);
//�I�C���[�p�Ȃ���̂�����悤���B
static double pi = 3.14;
static double ea(0), eb(0), ec(0);//ea,eb,ec�̒P�ʂ͓x�Ƃ���

void MI(int &x, int &y, int&z, int &pf, int &pr, int &fmino){//�_�ł��ˁBkind1
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
void MB(int &x, int &y, int&z, int &pf, int &pr, int &fmino){//���ł��ˁBkind2
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
void MZ(int &x, int &y, int&z, int &pf, int &pr, int &fmino){//�r�Ƃ��y�Ƃ��ł��ˁBkind3
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
void MT(int &x, int &y, int&z, int &pf, int &pr, int &fmino){//�r�Ƃ��y�Ƃ��ł��ˁBkind4
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
void ML(int &x, int &y, int&z, int &pf, int &pr, int &fmino){//L�Ƃ�J�Ƃ��ł��ˁBkind4
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

int kindrandom(int tt){//7��1�u���b�N�Ƃ��u���b�N1���őS��ނ��o��悤�ɂ������ˁ[
	//�Q�[�����n�܂����i�K�Ŕz���������������y�Ȃ̂�������Ȃ��B
	int randmino;
	srand(tt);
	randmino = 1 + (int)(rand() * 5 - 1 + 1) / (1 + RAND_MAX);
	if (minosequence[minosum] == 0)return randmino;//�z�����������ɂȂ���0���ł��Ă��܂��̂ł���̉����p�ł��B���Ԃ�rd�͈͎̔w�肪�����Ƃ��ĂȂ����炾�Ǝv���B
	else return minosequence[minosum];
}

void colorrandom(int tt, int minosum){//�o�b�N�̐F���ς��悤�ɂ��悤���ȂƂ������B
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

void brockshadow(int &x, int &y, int &z, int &pf, int &pr, int &kind, int tt, int &harddropflag, int &kv, int &v){//���ɂ��A�܂��͂ق��̃~�m�ɂ��������Ƃ��Ƀ��C���������邩�̔��ʂ��s���A����ю����J�n����
	for (int iy(y); iy <= 10; iy++){
		for (int ix(x); ix <= 10; ix++){//���ꂾ���ł����͂��ꂢ�������肷��B�����Ă�����~�܂��Ă��܂œh��ׂ�
			int flag1(0);
			for (int j(1); j <= z + 2; j++){
				if (flag1 == 0){
					if (f[ix][iy][j] == 2){//�����瓮���Ă��𒲂ׂ�����
						flag1 = 1;
						int flag2(0);
						for (int k(j - 1); k >= 0; k--){
							if (flag2 == 0){
								if (f[ix][iy][k] == 1){//�ォ��~�܂��Ă���z�𒲂ׂ�����
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

void linedelete(int &x, int &y, int&z, int &pf, int &pr, int &fmino, int tt){//yz���ʁAxz���ʂ��ꂼ��ɂ��čl����
	using namespace SDGLibF;
	//�N���X�ɂ������̂��Ƃ��l���ăt���O��p����B
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
	//yz�����ɂ��Ă̔���
	for (int ix(1); ix <= 5; ix++){
		for (int j(20); j >= 1; j--){//j���ォ�痎�Ƃ��悤�ɂ������C�ɏ�����悤�ɂȂ����B
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
	//xz�����ɂ��Ă̔���
	for (int iy(1); iy <= 5; iy++){
		for (int j(20); j >= 1; j--){//j���ォ�痎�Ƃ��悤�ɂ������C�ɏ�����悤�ɂȂ����B
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
	//���������@���Ԃ�����������������E�E�E�B���₻���ł��Ȃ��B�Ȃ��Ȃ炩�Ԃ����ꏊ��z�����ɂ͉����Ȃ��͂��Ȃ̂ŁB
	for (int i(0); i < 4; i++){
		if (deleetflagyz[i] == 1){//�����Ƃ���̖{�́B����j�i�̍��v��10��������A��̒i�����̒i�Ɉڂ��̂��J��Ԃ��B
			linepoint++;
			for (int ja(yzj[i]); ja <= 19; ja++){
				for (int k(1); k <= 10; k++){
					f[yzix[i]][k][ja] = f[yzix[i]][k][ja + 1];
				}
			}
			if (linepoint % stagecount == 0){//����΃X�e�[�W�I�ȁ@�z�j���������������Ƃɔw�i�F���ς�葬�x���オ��B
				colorrandom(tt, minosum);
				if (v <= 30)v -= 1; if (v > 30 && v <= 100)v -= 10; if (v > 100 && v <= 1000)v -= 100; if (v > 1000) v -= 1000; if (v <= 0) v = 10;
				stagepoint++;
			}
		}
		if (deleetflagxz[i] == 1){//�����Ƃ���̖{�́B����j�i�̍��v��10��������A��̒i�����̒i�Ɉڂ��̂��J��Ԃ��B
			linepoint++;
			for (int ja(xzj[i]); ja <= 19; ja++){
				for (int k(1); k <= 5; k++){
					f[k][xziy[i]][ja] = f[k][xziy[i]][ja + 1];
				}
			}
			if (linepoint % stagecount == 0){//����΃X�e�[�W�I�ȁ@�z�j���������������Ƃɔw�i�F���ς�葬�x���オ��B
				colorrandom(tt, minosum);
				if (v <= 30)v -= 1; if (v > 30 && v <= 100)v -= 10; if (v > 100 && v <= 1000)v -= 100; if (v > 1000) v -= 1000; if (v <= 0) v = 10;
				stagepoint++;
			}
		}

	}
}

void stopnext(int &x, int &y, int&z, int &pf, int &pr, int &kind, int tt, int &harddropflag, int &kv, int &v){//���ɂ��A�܂��͂ق��̃~�m�ɂ��������Ƃ��Ƀ��C���������邩�̔��ʂ��s���A����ю����J�n����
	for (int j(0); j <= 20; j++){
		for (int ix(1); ix <= 5; ix++){
			for (int iy(1); iy <= 5; iy++){
				if (f[ix][iy][j] == 1 && f[ix][iy][j + 1] == 2){//�����~�܂�����c�B
					harddropflag = 0;
					v = kv;
					for (int j(1); j <= 20; j++){//�����Ă��z��1�ɏ���������
						for (int ix(1); ix <= 5; ix++){
							for (int iy(1); iy <= 5; iy++){
								if (f[ix][iy][j] == 2) f[ix][iy][j] = 1;
							}
						}
					}
					linedelete(x, y, z, pf, pr, fmino, tt);//�s�������邩�ǂ����𔻒f���āA�����n�߂�B��������͐V�����̂��n�߂�
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

void cleanup(){//�����Ă������ꂢ�ɂ���
	for (int j(1); j <= 25; j++){
		for (int ix(1); ix <= 10; ix++){
			for (int iy(1); iy <= 10; iy++){
				if (f[ix][iy][j] == 2 || f[ix][iy][j] == 3 || f[ix][iy][j] == 4) f[ix][iy][j] = 0;
			}
		}
	}
}

double ctx(double x, double y, double z){//coridinate transformation���W�ϊ��֐�
	//��]�s�񂩂�̉�]�̂��
	x -= 3;
	y -= 3;
	z -= 13;
	double a = (cos(ea *pi / 180)*cos(eb *pi / 180)*cos(ec *pi / 180) - sin(ea *pi / 180)*sin(ec *pi / 180))*x + (-cos(ea *pi / 180)*cos(eb *pi / 180)*sin(ec *pi / 180) - sin(ea *pi / 180)*cos(ec *pi / 180))*y + cos(ea *pi / 180)*sin(eb *pi / 180)*z;
	double b = (sin(ea *pi / 180)*cos(eb *pi / 180)*cos(ec *pi / 180) + cos(ea *pi / 180)*sin(ec *pi / 180))*x + (-sin(ea *pi / 180)*cos(eb *pi / 180)*sin(ec *pi / 180) + cos(ea *pi / 180)*cos(ec *pi / 180))*y + sin(ea *pi / 180)*sin(eb *pi / 180)*z;
	double c = (-sin(eb *pi / 180)*cos(ec *pi / 180))*x + (sin(eb *pi / 180)*sin(ec *pi / 180))*y + (cos(eb *pi / 180))*z;
	return (ox - viewcubec * l*a + viewcubeb * l*b)*viewa;
}

double cty(double x, double y, double z){//coridinate transformation���W�ϊ��֐�
	//��]�s�񂩂�̉�]�̂��
	x -= 3;
	y -= 3;
	z -= 13;
	double a = (cos(ea *pi / 180)*cos(eb *pi / 180)*cos(ec *pi / 180) - sin(ea *pi / 180)*sin(ec *pi / 180))*x + (-cos(ea *pi / 180)*cos(eb *pi / 180)*sin(ec *pi / 180) - sin(ea *pi / 180)*cos(ec *pi / 180))*y + cos(ea *pi / 180)*sin(eb *pi / 180)*z;
	double b = (sin(ea *pi / 180)*cos(eb *pi / 180)*cos(ec *pi / 180) + cos(ea *pi / 180)*sin(ec *pi / 180))*x + (-sin(ea *pi / 180)*cos(eb *pi / 180)*sin(ec *pi / 180) + cos(ea *pi / 180)*cos(ec *pi / 180))*y + sin(ea *pi / 180)*sin(eb *pi / 180)*z;
	double c = (-sin(eb *pi / 180)*cos(ec *pi / 180))*x + (sin(eb *pi / 180)*sin(ec *pi / 180))*y + (cos(eb *pi / 180))*z;
	return (oy - viewcubea * l*a - viewcubeb*l*b + viewcubec * l*c)*viewb + 200;
}

void drawcube(int x, int y, int z, double r, double g, double b){//25�~25��box����邽�߂̊֐��c�@�ォ�猩�Ă��܂������̂��͐����킩��܂���B
	using namespace SDGLibF;//drawcircle������̂�drawbox���Ȃ����܂ŕs�ւ������̂ŊȈՔłł�������Ă݂܂����B
	SetColor(r, g, b);
	//�㑤�̕`��
	//(x,y,z)�̈ʒu����ʏ�̃s�N�Z���P�ʂɕ\���Ɓc�@[x,y,z]��(x,y)=(o-2*l*x+1.5*l*y,o-1*l*x-1.5*l*y+2*l*z)
	//���W�ϊ��̊֐����`���Ă������B
	//��
	DrawLine(2, ctx(x + 1 - d, y + 1 - d, z + 0 + d), cty(x + 1 - d, y + 1 - d, z + 0 + d), ctx(x + 0 + d, y + 1 - d, z + 0 + d), cty(x + 0 + d, y + 1 - d, z + 0 + d));//�@1
	DrawLine(2, ctx(x + 0 + d, y + 0 + d, z + 0 + d), cty(x + 0 + d, y + 0 + d, z + 0 + d), ctx(x + 0 + d, y + 1 - d, z + 0 + d), cty(x + 0 + d, y + 1 - d, z + 0 + d));//�@2
	DrawLine(2, ctx(x + 1 - d, y + 0 + d, z + 0 + d), cty(x + 1 - d, y + 0 + d, z + 0 + d), ctx(x + 0 + d, y + 0 + d, z + 0 + d), cty(x + 0 + d, y + 0 + d, z + 0 + d));//�@3
	DrawLine(2, ctx(x + 1 - d, y + 0 + d, z + 0 + d), cty(x + 1 - d, y + 0 + d, z + 0 + d), ctx(x + 1 - d, y + 1 - d, z + 0 + d), cty(x + 1 - d, y + 1 - d, z + 0 + d));//�@4
	//�c��
	DrawLine(2, ctx(x + 0 + d, y + 1 - d, z + 0 + d), cty(x + 0 + d, y + 1 - d, z + 0 + d), ctx(x + 0 + d, y + 1 - d, z + 1 + d), cty(x + 0 + d, y + 1 - d, z + 1 + d));//�@5
	DrawLine(2, ctx(x + 0 + d, y + 0 + d, z + 0 + d), cty(x + 0 + d, y + 0 + d, z + 0 + d), ctx(x + 0 + d, y + 0 + d, z + 1 + d), cty(x + 0 + d, y + 0 + d, z + 1 + d));//�@6
	DrawLine(2, ctx(x + 1 - d, y + 0 + d, z + 0 + d), cty(x + 1 - d, y + 0 + d, z + 0 + d), ctx(x + 1 - d, y + 0 + d, z + 1 + d), cty(x + 1 - d, y + 0 + d, z + 1 + d));//�@7
	DrawLine(2, ctx(x + 1 - d, y + 1 - d, z + 0 + d), cty(x + 1 - d, y + 1 - d, z + 0 + d), ctx(x + 1 - d, y + 1 - d, z + 1 + d), cty(x + 1 - d, y + 1 - d, z + 1 + d));//�@8
	//��
	DrawLine(2, ctx(x + 1 - d, y + 1 - d, z + 1 + d), cty(x + 1 - d, y + 1 - d, z + 1 + d), ctx(x + 0 + d, y + 1 - d, z + 1 + d), cty(x + 0 + d, y + 1 - d, z + 1 + d));//�@9
	DrawLine(2, ctx(x + 0 + d, y + 0 + d, z + 1 + d), cty(x + 0 + d, y + 0 + d, z + 1 + d), ctx(x + 0 + d, y + 1 - d, z + 1 + d), cty(x + 0 + d, y + 1 - d, z + 1 + d));//�@10
	DrawLine(2, ctx(x + 1 - d, y + 0 + d, z + 1 + d), cty(x + 1 - d, y + 0 + d, z + 1 + d), ctx(x + 0 + d, y + 0 + d, z + 1 + d), cty(x + 0 + d, y + 0 + d, z + 1 + d));//�@11
	DrawLine(2, ctx(x + 1 - d, y + 0 + d, z + 1 + d), cty(x + 1 - d, y + 0 + d, z + 1 + d), ctx(x + 1 - d, y + 1 - d, z + 1 + d), cty(x + 1 - d, y + 1 - d, z + 1 + d));//�@12
}

void paintcubetop(int x, int y, int z, double r, double g, double b){//�㑤�̓h��Ԃ�
	using namespace SDGLibF;
	SetColor(r, g, b);
	//��
	for (double i(1.625); i <= 4; i += 0.250){
		DrawLine(2, ctx(x + 1 - d * (i), y + 1 - d * (i), z + 1 + 0.125), cty(x + 1 - d * (i), y + 1 - d * (i), z + 1 + 0.125), ctx(x + 0 + d * (i), y + 1 - d * (i), z + 1 + 0.125), cty(x + 0 + d * (i), y + 1 - d * (i), z + 1 + 0.125));//�@9
		DrawLine(2, ctx(x + 0 + d * (i), y + 0 + d * (i), z + 1 + 0.125), cty(x + 0 + d * (i), y + 0 + d * (i), z + 1 + 0.125), ctx(x + 0 + d * (i), y + 1 - d * (i), z + 1 + 0.125), cty(x + 0 + d * (i), y + 1 - d * (i), z + 1 + 0.125));//�@10
		DrawLine(2, ctx(x + 1 - d * (i), y + 0 + d * (i), z + 1 + 0.125), cty(x + 1 - d * (i), y + 0 + d * (i), z + 1 + 0.125), ctx(x + 0 + d * (i), y + 0 + d * (i), z + 1 + 0.125), cty(x + 0 + d * (i), y + 0 + d * (i), z + 1 + 0.125));//�@11
		DrawLine(2, ctx(x + 1 - d * (i), y + 0 + d * (i), z + 1 + 0.125), cty(x + 1 - d * (i), y + 0 + d * (i), z + 1 + 0.125), ctx(x + 1 - d * (i), y + 1 - d * (i), z + 1 + 0.125), cty(x + 1 - d * (i), y + 1 - d * (i), z + 1 + 0.125));//�@12
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




void drawbox(int x, int y, double r, double g, double b){//25�~25��box����邽�߂̊֐������܂����B
	using namespace SDGLibF;//drawcircle������̂�drawbox���Ȃ����܂ŕs�ւ������̂ŊȈՔłł�������Ă݂܂����B
	SetColor(r, g, b);
	DrawLine(2, x * 25 + 1.5, y * 25 + 1.5, x * 25 + 1.5, y * 25 + 23.5);//left
	DrawLine(2, x * 25 + 1.5, y * 25 + 23.5, x * 25 + 23.5, y * 25 + 23.5);//top
	DrawLine(2, x * 25 + 23.5, y * 25 + 1.5, x * 25 + 23.5, y * 25 + 23.5);//right
	DrawLine(2, x * 25 + 1.5, y * 25 + 1.5, x * 25 + 23.5, y * 25 + 1.5);//botom
}
void drawboxpaint(int x, int y, double r, double g, double b){//box��h��ׂ����߂ɍ�����Bnextbox�ł͒������ɂ������������ߕʊ֐��Œ��g������`
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
	//�u���b�N�̕`��
	for (int j(1); j <= 23; j++){
		for (int ix(1); ix <= 5; ix++){
			for (int iy(1); iy <= 5; iy++){
				int depth(j);
				if (depth >= 21)depth -= 10;
				if (depth >= 11)depth -= 10;
				switch (f[ix][iy][j]){
				case 1: drawcube(ix, iy, j, 0, 0, 0.7 + stagecolorb*0.3); paintcubetop(ix, iy, j, 0.3 + 0.7*stagecolorr*0.1*depth, 0.3 + 0.7*stagecolorg*0.1*depth, 1); break;//�����ĂȂ��ق��͐����܂����B
				case 2: drawcube(ix, iy, j, 0.7 + stagecolorr*0.3, 0, 0); paintcubetop(ix, iy, j, 1, 0.3 + 0.7*stagecolorg*0.1*depth, 0.3 + 0.7*stagecolorb*0.1*depth); break;//�����Ă���͐Ԃ����܂����B
				case 3: drawcube(ix, iy, j, 0.85, 0.85, 0.85); break;//�u���b�N�V���h�[�̂܂������̕`��D�F
				default: break;
				}
			}
		}
	}
}
void displayfunc2wall(){
	using namespace SDGLibF;
	if (wallflag == 1){
		//�ǂ̕`��
		for (int j(0); j <= 10; j++){
			//yz��
			for (int iy(0); iy <= 5; iy++){
				drawcube(0, iy, j, 0, 1, 0);
				paintcubetop(0, iy, j, 0, 0.7 + 0.3*stagecolorg*0.05*j, 0);
			}
			//xz��	
			for (int ix(0); ix <= 5; ix++){
				drawcube(ix, 0, j, 0, 1, 0);
				paintcubetop(ix, 0, j, 0, 0.7 + 0.3*stagecolorg*0.05*j, 0);
			}
		}
	}
}
void displayfunc2floor(){
	using namespace SDGLibF;
	//���̕`��
	for (int ix(1); ix <= 5; ix++){
		for (int iy(1); iy <= 5; iy++){
			drawcube(ix, iy, 0, 0, 0.7 + stagecolorg*0.3, 0);
			paintcubetop(ix, iy, 0, 0, 0.7 + stagecolorg*0.3, 0);
		}
	}
}

//overview�̕�
static int of[12][12];//overview�̕�
static double ofj[12][12];
//overview�̍쐬z���������猩������x,y�̎ˉe�Hx,y���C���[�W�ɍ��킹���x��y���ɂȂ��Ă�̂œ���c
void overview(){
	for (int iy(1); iy <= 5; iy++){
		for (int ix(1); ix <= 5; ix++){
			int flag2(0);//�ォ��~�܂��Ă��𒲂ׂ�
			for (int k(23); k >= 1; k--){
				if (flag2 == 0){
					if (f[ix][iy][k] == 1){//�ォ��~�܂��Ă���z�𒲂ׂ�����
						flag2 = 1;
						of[ix][iy] = 1;
						ofj[ix][iy] = k;
					}
				}
			}
			int flag3(0);//�����瓮���Ă��𒲂ׂ�
			for (int k(1); k <= 23; k++){
				if (flag3 == 0){
					if (f[ix][iy][k] == 2){//�ォ��~�܂��Ă���z�𒲂ׂ�����
						flag3 = 1;
						of[ix][iy] = 2;
						ofj[ix][iy] = k;
					}
				}
			}
		}
	}

}
void ofcleanup(){//�����Ă������ꂢ�ɂ���
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
	//�g
	for (int k(0); k < 6; k++){
		drawbox(-k + 25, 0 + 20, 0, 1, 0);//
		drawbox(0 + 25, -k + 20, 0, 1, 0);//
	}

	SetColor(1, 0, 0);//x��
	DrawLine(3, 500 - 5, 525 + 2, 650 + 2, 525 + 2);
	SetColor(0, 0, 1);//y��
	DrawLine(3, 650 + 2, 375 - 2, 650 + 2, 525 + 2);
	ofcleanup();
}


//xz����
static int ofxz[6][25];//overview�̕�
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
				int flag2(0);//�ォ��~�܂��Ă��𒲂ׂ�
				for (int iy(5); iy >= 1; iy--){
				if (flag2 == 0){
				if (f[ix][iy][k] == 1){//�ォ��~�܂��Ă���z�𒲂ׂ�����
				flag2 = 1;
				ofxz[ix][k] = 1;
				ofxzj[ix][k] = y;
				}
				}
				}
				int flag3(0);//�����瓮���Ă��𒲂ׂ�
				for (int iy(1); iy <= 5; iy++){
				if (flag3 == 0){
				if (f[ix][iy][k] == 2){//�ォ��~�܂��Ă���z�𒲂ׂ�����
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
void ofcleanupxz(){//�����Ă������ꂢ�ɂ���
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
	//�g
	for (int k(0); k < 6; k++){
		drawbox(-k + 25 + 7, 0 + 10, 0, 1, 0);//
	}
	for (int k(0); k < 11; k++){
		drawbox(0 + 25 + 7, k + 10, 0, 1, 0);//
	}

	SetColor(1, 0, 0);//x��
	DrawLine(3, 500 - 5 + 175, 375 - 2 - 125, 650 + 2 + 175, 375 - 2 - 125);
	SetColor(0, 1, 0);//z��
	DrawLine(3, 650 + 2 + 175, 375 - 2 - 125, 650 + 2 + 175, 650 + 2 - 125);
	ofcleanupxz();
}

//yz����
static int ofyz[6][25];//overview�̕�
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
void ofcleanupyz(){//�����Ă������ꂢ�ɂ���
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
	//�g
	for (int k(0); k < 6; k++){
		drawbox(k + 25 + 7 + 2, 0 + 10, 0, 1, 0);//
	}
	for (int k(0); k < 11; k++){
		drawbox(0 + 25 + 7 + 2, k + 10, 0, 1, 0);//
	}

	SetColor(0, 0, 1);//y��
	DrawLine(3, 500 - 5 + 175 + 175, 375 - 2 - 125, 650 + 2 + 175 + 175, 375 - 2 - 125);
	SetColor(0, 1, 0);//z��
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
		//���Ԃ̕\���A���x�������̕\��
		//���W���̕`��
		SetColor(1, 0, 0);//x��
		DrawLine(3, ctx(1, 1, 1), cty(1, 1, 1), ctx(25, 1, 1), cty(25, 1, 1));
		SetColor(0, 0, 1);//y��
		DrawLine(3, ctx(1, 1, 1), cty(1, 1, 1), ctx(1, 25, 1), cty(1, 25, 1));
		SetColor(0, 0.7, 0);//z��
		DrawLine(3, ctx(1, 1, 1), cty(1, 1, 1), ctx(1, 1, 25), cty(1, 1, 25));

		displayfunc2floor();
		displayfunc2wall();
		displayfunc2overview();
		displayfunc2overviewxz();
		displayfunc2overviewyz();

		displayfunc2boxes();//�����邱�ƂŃv���O�������������肩����悤�ɂ�������ł�
		displayfunc2keycon();
		//�e�p�����[�^�̕`��
		char buff1[100];
		SetColor(0, 0, 0);
		sprintf_s(buff1, "tt=%d v=%d linepoint=%d minosum=%d", tt, v, linepoint, minosum);
		DrawString(30, 570, buff1);
		//���̓z��̕\��
		char buff3[100], buff4[100];
		SetColor(0, 0, 0);
		sprintf_s(buff3, "stage [%d]", stagepoint);
		DrawString(310, 540, buff3);
		sprintf_s(buff4, "next::%d %d %d %d %d %d %d", minosequence[minosum + 0], minosequence[minosum + 1], minosequence[minosum + 2], minosequence[minosum + 3]
			, minosequence[minosum + 4], minosequence[minosum + 5], minosequence[minosum + 6]);
		DrawString(310, 510, buff4);
		//�J���[�\��
		char buff5[100];
		SetColor(0, 0, 0);
		sprintf_s(buff5, "r=%3.2f g=%3.2f b=%3.2f", stagecolorr, stagecolorg, stagecolorb);
		DrawString(30, 540, buff5);
		//���_�ύX�`��p�����[�^�̕\��
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
		//�����I��
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
void displayfunc(){//�`��֐�
	using namespace SDGLibF;
	Before(); // Set drawing plane
	displayfunc1();//�ŏ�
	displayfunc2();//�v���C��
	displayfunc3();//�Ō�
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
	}//���Ԍo�߂ŉ��ɗ�����悤�Ȃ�
	//y�͈̔͂̐��� x�̐�����class mono�ŕ␳�ς� ����y�������͎����ŗ�����͂��Ȃ̂ł���Ȃɖ��Ȃ������B
}

void swap(int &x, int &y){
	int k;
	k = x;
	x = y;
	y = k;
}


/////�������炪visual studio2008�ł͂ł��Ȃ� random�N���X�̎����𑁂����Ƃ�
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


void randombypi(){//random���C�u�����������Ă��Ȃ��w�Z�p�̗��������֐�
	using namespace std;
	std::string s;
	int len(0);
	s = "1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412";//��̐����̂��߂ɉ~�������g���Ă݂�B
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
void simulation(void){//���ۂɓ������֐�
	using namespace SDGLibF;
	down();
	minoselect(x, y, z, pf, pr, kind, tt, harddropflag, kv, v);
	harddrop(z, harddropflag, kv, v);
	brockshadow(x, y, z, pf, pr, kind, tt, harddropflag, kv, v);
	cheating();
	overview();
	overviewxz();
	overviewyz();

	//���̒�`
	for (int ix(0); ix <= 5; ix++){
		for (int iy(0); iy <= 5; iy++){
			f[ix][iy][0] = 1;
		}
	}
	//�ǂ̒�`
	for (int j(0); j <= 21; j++){
		//yz��
		for (int iy(0); iy <= 5; iy++){
			f[0][iy][0] = 1;
		}
		//xz��	
		for (int ix(0); ix <= 5; ix++){
			f[ix][0][0] = 1;
		}
	}
	//��ʐ؂�ւ�
	if (f[3][3][10] == 1){
		displayflag = 2; IdleFunc(simulation2);
	}
	ReDraw();
}


void definition(){//�����l��`�֐�
	//���g�̏����l�̒�`
	for (int j(1); j <= 20; j++){
		for (int ix(0); ix <= 10; ix++){
			for (int iy(0); iy <= 10; iy++){
				f[ix][iy][j] = 0;
			}
		}
	}
	//���̒�`
	for (int ix(0); ix <= 10; ix++){
		for (int iy(0); iy <= 10; iy++){
			f[ix][iy][0] = 1;
		}
	}
	//�ǂ̒�`
	for (int j(0); j <= 21; j++){
		//yz��
		for (int iy(0); iy <= 10; iy++){
			f[0][iy][0] = 1;
		}
		//xz��	
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
		//�ړ�
		//�e���L�[������Ƃ�
	case '4': x++; break;
	case '8': y--; break;
	case '6': x--; break;
	case '2': y++; break;
	case '5': z--; break;
	case '0':harddropflag = 1; break;
		//�e���L�[���Ȃ��Ƃ�
	case 'a': x++; break;
	case 'w': y--; break;
	case 'd': x--; break;
	case 'x': y++; break;
	case 's': z--; break;
	case VK_SPACE: harddropflag = 1; break;
	case VK_RETURN: pf++; if (pf >= 3) pf %= 3; break; //pf�̕ύX�@���ʂ̐}�`�̕ύX
	case '[': pr++; if (pr >= 4) pr %= 4; break; //z���܂���]
	case 'v': if (v <= 100)v -= 10; if (v > 100 && v <= 1000)v -= 100; if (v > 1000) v -= 1000; if (v <= 0) v = 10; break;//���x������
	case 'V': if (v <= 1000)v += 100; if (v > 1000) v += 1000; if (v > 10000) v = 10000; break;//���x������
	case 'S': downflag += 1; break;//�����~�߂�
	case VK_BACK: swap(kind, holdkind);
	case 'r':if (displayflag == 2){
				 displayflag = 0;
				 tt = 0;
				 kind = minosequence[minosum], holdkind = 1;
				 x = 3, y = 3, z = 10, pf = 0, pr = 0;
				 v = 500, downflag = 1;
				 linepoint = 0;
				 minosum = 0;
				 stagecolorr = 1, stagecolorg = 1, stagecolorb = 1;//�u���b�N�̐F�Ƃ�
				 stagepoint = 1;
				 definition();
				 harddropflag = 0;
	}break;
	case 'C':cheatingflag = 1; break;
		//���_�ύX�n
	case 'u':ox += 10; break;
	case 'j':oy += 10; break;
		//case 'm':viewcubec += 0.1; break;
		//case 'i':viewa += 0.1; break;
		//case 'k':viewb += 0.1; break;
		//case 'o':ox += 10; break;
	case 'l':l += 0.1; break;//�Y�[��
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
	case 'R':ox = 150, oy = 350, l = 20, viewa = 1, viewb = 1, viewcubea = 1, viewcubeb = 1.5, viewcubec = 2, ea = 0, eb = 0, ec = 0; break;//���_�ύX���Z�b�g
		//�ǂ�on off
	case 'W':wallflag++; if (wallflag >= 2) wallflag %= 2;
	default: break;
	}
}

int main(void){
	using namespace SDGLibF;
	definition();
	kind = kindrandom(tt);
	SDGLib mygraphic(1300, 700, "- Fake TETRiS 3D- (61218525 ���䒉�@)", -100, 1200, -100, 650);
	mygraphic.SetCursor(GLUT_CURSOR_WAIT);
	mygraphic.Display(displayfunc);
	mygraphic.Keyboard(keyboardfunc);
	mygraphic.Start();
	return 0;
}

/*
�E���W�ϊ��֐��̒�`3�����z�񂩂�2�����̃E�B���h�E��ɕ\��������
�E�u���b�N�̐F�����W�ɂ���ĔZ�W�����邱�Ƃɂ���ĉ��ߊ������
�Eoverview�ƑΉ������Ă�悤�ɂȂ��Ă��܂��B
�E�I�C���[�p��p���Ď��_�̉�]���ł���悤�ɂ��܂����B

�E���_�̕ύX���ł�����悢�ł��ȁcctx,cty��������΍s���邩������Ȃ�
ctx,cty�͍��W�n��ύX���Ă�킯����Ȃ��̂ŕ��s�ړ��C���ɂȂ�B
��]�s����l���č��W��ύX����drawcube�̒���ς��Ă݂�B
*/
