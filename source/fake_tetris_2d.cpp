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

static int f[12][25];//������ŕςȃo�O���N�����Ȃ��悤�ɑ傫�߂ɍ���Ă���B
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
static double stagecolorr(1), stagecolorg(1), stagecolorb(1);//�u���b�N�̐F�Ƃ�
static int stagepoint(1);
static int harddropflag(0);
static int kv(0);
static int cheatingflag(0);
//�n�C�X�R�A����͂��邽�߂̕ϐ������܂��B
std::string name = "";
static int keyboardflag(0);
static int hslinepoint[300], hsminosum[300], hstt[300];
std::string hsname[300];
static int playertotal(0);

//�Ƃ肠����x�������͈̔͌���͂��ꂼ���mino�̊֐��ł���Ă��� //fmino�ɂ��邱�Ƃɂ���Ă����ȃu���b�N�ɑΉ��ł���悤�ɂ����B
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
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//���ɒ�����
	case 1: f[x][y] = fmino; f[x][y + 1] = fmino; f[x + 1][y + 1] = fmino; f[x + 1][y + 2] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//�c�ɒ�����
	default: break;
	}
}
void ms(int &x, int &y, int &p, int &fmino){//s
	p %= 2;
	switch (p){
	case 0: f[x][y] = fmino; f[x + 1][y] = fmino; f[x + 1][y + 1] = fmino; f[x + 2][y + 1] = fmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//���ɒ�����
	case 1: f[x][y + 1] = fmino; f[x][y + 2] = fmino; f[x + 1][y + 1] = fmino; f[x + 1][y] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//�c�ɒ�����
	default: break;
	}
}
void mi(int &x, int &y, int &p, int &fmino){//i�@�_
	p %= 2;
	switch (p){
	case 0: f[x][y] = fmino; f[x + 1][y] = fmino; f[x + 2][y] = fmino; f[x + 3][y] = fmino;
		if (x == 0) x = 1; if (x + 3 >= 11)x = 10 - 3; break;//���ɒ�����
	case 1: f[x + 1][y - 1] = fmino; f[x + 1][y] = fmino; f[x + 1][y + 1] = fmino; f[x + 1][y + 2] = fmino;
		if (x + 1 == 0) x = 0; if (x + 1 == 11) x = 9; break;//�c�ɒ�����
	default: break;
	}
}
void ml(int &x, int &y, int &p, int &fmino){// L
	switch (p){
	case 0: f[x][y] = fmino; f[x + 1][y] = fmino; f[x + 2][y] = fmino; f[x + 2][y + 1] = fmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//���ɒ�����
	case 1: f[x][y] = fmino; f[x][y + 1] = fmino; f[x][y + 2] = fmino; f[x + 1][y] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//�c�ɒ�����
	case 2: f[x][y] = fmino; f[x][y + 1] = fmino; f[x + 1][y + 1] = fmino; f[x + 2][y + 1] = fmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//�t���ɒ�����
	case 3: f[x][y + 2] = fmino; f[x + 1][y + 2] = fmino; f[x + 1][y + 1] = fmino; f[x + 1][y] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//�t�c�ɒ�����
	default: break;
	}
}
void mj(int &x, int &y, int &p, int &fmino){// J
	switch (p){
	case 0: f[x][y] = fmino; f[x + 1][y] = fmino; f[x + 2][y] = fmino; f[x][y + 1] = fmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//���ɒ�����
	case 1: f[x][y] = fmino; f[x + 1][y] = fmino; f[x + 1][y + 1] = fmino; f[x + 1][y + 2] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//�c�ɒ�����
	case 2: f[x][y + 1] = fmino; f[x + 1][y + 1] = fmino; f[x + 2][y + 1] = fmino; f[x + 2][y] = fmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//�t���ɒ�����
	case 3: f[x][y] = fmino; f[x][y + 1] = fmino; f[x][y + 2] = fmino; f[x + 1][y + 2] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//�t�c�ɒ�����
	default: break;
	}
}
void mt(int &x, int &y, int &p, int &fmino){// T
	switch (p){
	case 0: f[x][y] = fmino; f[x + 1][y] = fmino; f[x + 2][y] = fmino; f[x + 1][y + 1] = fmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//���
	case 1: f[x][y] = fmino; f[x][y + 1] = fmino; f[x][y + 2] = fmino; f[x + 1][y + 1] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//�E��
	case 2: f[x][y + 1] = fmino; f[x + 1][y + 1] = fmino; f[x + 2][y + 1] = fmino; f[x + 1][y] = fmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//����
	case 3: f[x][y + 1] = fmino; f[x + 1][y] = fmino; f[x + 1][y + 1] = fmino; f[x + 1][y + 2] = fmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//����
	default: break;
	}
}

int kindrandom(int tt){//7��1�u���b�N�Ƃ��u���b�N1���őS��ނ��o��悤�ɂ������ˁ[
	//�Q�[�����n�܂����i�K�Ŕz���������������y�Ȃ̂�������Ȃ��B
	int randmino;
	srand(tt);
	randmino = 1 + (int)(rand() * 7 - 1 + 1) / (1 + RAND_MAX);
	if (minosequence[minosum] == 0)return randmino;//�z�����������ɂȂ���0���ł��Ă��܂��̂ł���̉����p�ł��B���Ԃ�rd�͈͎̔w�肪�����Ƃ��ĂȂ����炾�Ǝv���B
	else return minosequence[minosum];
}

void colorrandom(){//�o�b�N�̐F���ς��悤�ɂ��悤���ȂƂ������B
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
void ghostminoselect(int &x, int &y, int &p, int &kind, int &tt, int &harddropflag, int &kv, int &v){//���ݓ������Ă���e�g���~�m�̔����kind�Ƃ����t���O�����ɍs���Ă���B
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
void ghostbrock(int &x, int &y, int &p, int &kind, int &tt, int &harddropflag, int &kv, int &v){//���ݓ������Ă���e�g���~�m�̔����kind�Ƃ����t���O�����ɍs���Ă���B
//�܂������Ă��̈�ԉ������o���A�ۑ�����
int yminu[4], ymaxd[4], yminmax[4], yminflag(0);
for (int i(0); i < 4; i++){ yminu[i] = 0; ymaxd[i]=0; }
for (int i(x); i <= 10; i++){
int flag1(0);
for (int j(1); j <= y + 2; j++){
if (flag1 == 0){
if (f[i][j] == 2){//�����瓮���Ă��𒲂ׂ�����
flag1 = 1;
yminu[yminflag] = j;//����x�̂Ƃ����
int flag2(0);
for (int k(j - 1); k >= 0; k--){
if (flag2 == 0){
if (f[i][k] == 1){//�ォ��~�܂��Ă���z�𒲂ׂ�����
flag2 = 1;
ymaxd[yminflag] = k;
yminmax[yminflag] = yminu[yminflag] - ymaxd[yminflag];//2��1�̊Ԃ��o���Ă���
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

void brockshadow(int &x, int &y, int &p, int &kind, int tt, int &harddropflag, int &kv, int &v){//���ɂ��A�܂��͂ق��̃~�m�ɂ��������Ƃ��Ƀ��C���������邩�̔��ʂ��s���A����ю����J�n����
	for (int i(x); i <= 10; i++){//���ꂾ���ł����͂��ꂢ�������肷��B�����Ă�����~�܂��Ă��܂œh��ׂ�
		int flag1(0);
		for (int j(1); j <= y + 2; j++){
			if (flag1 == 0){
				if (f[i][j] == 2){//�����瓮���Ă��𒲂ׂ�����
					flag1 = 1;
					int flag2(0);
					for (int k(j - 1); k >= 0; k--){
						if (flag2 == 0){
							if (f[i][k] == 1){//�ォ��~�܂��Ă���z�𒲂ׂ�����
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

void linedelete(int &x, int &y, int &p){//line���������߂̔��f�@count�͂����Ƀu���b�N�����݂��邩�ǂ����̃t���O�̂悤�Ȃ���
	using namespace SDGLibF;
	//int counttetris(0);
	for (int j(20); j >= 1; j--){//j���ォ�痎�Ƃ��悤�ɂ������C�ɏ�����悤�ɂȂ����B
		int count(0);
		for (int i(1); i <= 10; i++){
			if (f[i][j] == 1) count++;//
		}
		if (count == 10){//�����Ƃ���̖{�́B����j�i�̍��v��10��������A��̒i�����̒i�Ɉڂ��̂��J��Ԃ��B
			linepoint++;
			//counttetris++;
			for (int ja(j); ja <= 19; ja++){
				for (int i(1); i <= 10; i++){
					f[i][ja] = f[i][ja + 1];
				}
			}
			if (linepoint % 3 == 0){//����΃X�e�[�W�I�ȁ@�z�j���������������Ƃɔw�i�F���ς�葬�x���オ��B
				colorrandom();
				if (v <= 30)v -= 1; if (v > 30 && v <= 100)v -= 10; if (v > 100 && v <= 1000)v -= 100; if (v > 1000) v -= 1000; if (v <= 0) v = 10;
				stagepoint++;
			}
		}
	}
	//if (counttetris == 4){} //�e�g���X���܂������̓��ʂȃG�t�F�N�g��p�ӂ�����
}

void stopnext(int &x, int &y, int &p, int &kind, int tt, int &harddropflag, int &kv, int &v){//���ɂ��A�܂��͂ق��̃~�m�ɂ��������Ƃ��Ƀ��C���������邩�̔��ʂ��s���A����ю����J�n����
	for (int j(0); j <= 20; j++){
		for (int i(1); i <= 10; i++){
			if (f[i][j] == 1 && f[i][j + 1] == 2){//�����~�܂�����c�B
				harddropflag = 0;
				for (int j(1); j <= 20; j++){//�����Ă��z��1�ɏ��������Ă���
					for (int i(1); i <= 10; i++){
						if (f[i][j] == 2) f[i][j] = 1;
					}
				}
				v = kv;
				linedelete(x, y, p);//�s�������邩�ǂ����𔻒f���āA�����n�߂�B��������͐V�����̂��n�߂�
				/*��邽�тɍ���Ă��o�[�W����
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

void minoselect(int &x, int &y, int &p, int &kind, int &tt, int &harddropflag, int &kv, int &v){//���ݓ������Ă���e�g���~�m�̔����kind�Ƃ����t���O�����ɍs���Ă���B
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

void cleanup(){//�����Ă������ꂢ�ɂ���
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
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//���ɒ�����
	case 1:ghostf[x][y] = ghostfmino; ghostf[x][y + 1] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 1][y + 2] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//�c�ɒ�����
	default: break;
	}
}
void ghostms(int &x, int &y, int &p, int &ghostfmino){//s
	p %= 2;
	switch (p){
	case 0:ghostf[x][y] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 2][y + 1] = ghostfmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//���ɒ�����
	case 1:ghostf[x][y + 1] = ghostfmino; ghostf[x][y + 2] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 1][y] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//�c�ɒ�����
	default: break;
	}
}
void ghostmi(int &x, int &y, int &p, int &ghostfmino){//i�@�_
	p %= 2;
	switch (p){
	case 0:ghostf[x][y] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 2][y] = ghostfmino; ghostf[x + 3][y] = ghostfmino;
		if (x == 0) x = 1; if (x + 3 >= 11)x = 10 - 3; break;//���ɒ�����
	case 1:ghostf[x + 1][y - 1] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 1][y + 2] = ghostfmino;
		if (x + 1 == 0) x = 0; if (x + 1 == 11) x = 9; break;//�c�ɒ�����
	default: break;
	}
}
void ghostml(int &x, int &y, int &p, int &ghostfmino){// L
	switch (p){
	case 0:ghostf[x][y] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 2][y] = ghostfmino; ghostf[x + 2][y + 1] = ghostfmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//���ɒ�����
	case 1:ghostf[x][y] = ghostfmino; ghostf[x][y + 1] = ghostfmino; ghostf[x][y + 2] = ghostfmino; ghostf[x + 1][y] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//�c�ɒ�����
	case 2:ghostf[x][y] = ghostfmino; ghostf[x][y + 1] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 2][y + 1] = ghostfmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//�t���ɒ�����
	case 3:ghostf[x][y + 2] = ghostfmino; ghostf[x + 1][y + 2] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 1][y] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//�t�c�ɒ�����
	default: break;
	}
}
void ghostmj(int &x, int &y, int &p, int &ghostfmino){// J
	switch (p){
	case 0:ghostf[x][y] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 2][y] = ghostfmino; ghostf[x][y + 1] = ghostfmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//���ɒ�����
	case 1:ghostf[x][y] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 1][y + 2] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//�c�ɒ�����
	case 2:ghostf[x][y + 1] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 2][y + 1] = ghostfmino; ghostf[x + 2][y] = ghostfmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//�t���ɒ�����
	case 3:ghostf[x][y] = ghostfmino; ghostf[x][y + 1] = ghostfmino; ghostf[x][y + 2] = ghostfmino; ghostf[x + 1][y + 2] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//�t�c�ɒ�����
	default: break;
	}
}
void ghostmt(int &x, int &y, int &p, int &ghostfmino){// T
	switch (p){
	case 0:ghostf[x][y] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 2][y] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//���
	case 1:ghostf[x][y] = ghostfmino; ghostf[x][y + 1] = ghostfmino; ghostf[x][y + 2] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//�E��
	case 2:ghostf[x][y + 1] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 2][y + 1] = ghostfmino; ghostf[x + 1][y] = ghostfmino;
		if (x == 0) x = 1; if (x + 2 == 11)x = 10 - 2; break;//����
	case 3:ghostf[x][y + 1] = ghostfmino; ghostf[x + 1][y] = ghostfmino; ghostf[x + 1][y + 1] = ghostfmino; ghostf[x + 1][y + 2] = ghostfmino;
		if (x == 0) x = 1; if (x + 1 == 11)x = 10 - 1; break;//����
	default: break;
	}
}
void ghostcleanup(){//�����Ă������ꂢ�ɂ���
	for (int j(1); j <= 25; j++){
		for (int i(1); i <= 10; i++){
			if (ghostf[i][j] == 2 || ghostf[i][j] == 4)ghostf[i][j] = 0;
		}
	}
}
void ghostcleanup2(){//�����Ă������ꂢ�ɂ���
	for (int j(1); j <= 25; j++){
		for (int i(1); i <= 10; i++){
			if (ghostf[i][j] == 2)ghostf[i][j] = 0;
		}
	}
}
void ghostbox(int &x, int &y, int &p, int &kind, int &tt, int &harddropflag, int &kv, int &v){//����΂낤
	//ghostf��f��1�̕�����������ǂݍ���
	for (int j(0); j < 25; j++){
		for (int i(0); i < 12; i++){
			if (f[i][j] == 2) ghostf[i][j] = f[i][j];
		}
	}
	//1����ԏゾ���ǂ܂����邩�ȁE�E�E
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
					if (ghostf[i][j] == 1 && ghostf[i][j + 1] == 2){//�����~�܂�����c�B
						for (int j(1); j <= 20; j++){//�����Ă��z��1�ɏ��������Ă���
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
	//���ݎ����̎擾
	struct tm now;
	time_t longtime;
	longtime = time(NULL);
	char date[32];
	longtime = time(NULL);
	localtime_s(&now, &longtime);
	asctime_s(date, sizeof(date), &now);
	//�t�@�C���ւ̏�������
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

	//���̓t�@�C���̓��e��ǂ݂���
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
	//linepoint���ɕ��ёւ���
	cout << "���ёւ��܂��B" << endl;
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

void drawbox(int x, int y, double r, double g, double b){//25�~25��box����邽�߂̊֐������܂����B
	//3D���[�h���I�I�I
	using namespace SDGLibF;//drawcircle������̂�drawbox���Ȃ����܂ŕs�ւ������̂ŊȈՔłł�������Ă݂܂����B
	SetColor(r, g, b);
	DrawLine(2, x * 25 + 1.5, y * 25 + 1.5, x * 25 + 1.5, y * 25 + 23.5);//left
	DrawLine(2, x * 25 + 1.5, y * 25 + 23.5, x * 25 + 23.5, y * 25 + 23.5);//top
	DrawLine(2, x * 25 + 23.5, y * 25 + 1.5, x * 25 + 23.5, y * 25 + 23.5);//right
	DrawLine(2, x * 25 + 1.5, y * 25 + 1.5, x * 25 + 23.5, y * 25 + 1.5);//botom
	//�����̕`��
	DrawLine(2, x * 25 + 1.5 + 10, y * 25 + 1.5 + 10, x * 25 + 1.5 + 10, y * 25 + 23.5 + 10);//left
	DrawLine(2, x * 25 + 1.5 + 10, y * 25 + 23.5 + 10, x * 25 + 23.5 + 10, y * 25 + 23.5 + 10);//top
	DrawLine(2, x * 25 + 23.5 + 10, y * 25 + 1.5 + 10, x * 25 + 23.5 + 10, y * 25 + 23.5 + 10);//right
	DrawLine(2, x * 25 + 1.5 + 10, y * 25 + 1.5 + 10, x * 25 + 23.5 + 10, y * 25 + 1.5 + 10);//botom
	//�O�Ɖ�������
	DrawLine(2, x * 25 + 1.5, y * 25 + 1.5, x * 25 + 1.5 + 10, y * 25 + 1.5 + 10);//left
	DrawLine(2, x * 25 + 1.5, y * 25 + 23.5, x * 25 + 1.5 + 10, y * 25 + 23.5 + 10);//top
	DrawLine(2, x * 25 + 23.5, y * 25 + 1.5, x * 25 + 23.5 + 10, y * 25 + 1.5 + 10);//right
	DrawLine(2, x * 25 + 23.5, y * 25 + 23.5, x * 25 + 23.5 + 10, y * 25 + 23.5 + 10);//botom
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
	//���̕`��
	for (int i(0); i <= 11; i++){
		if (f[i][0] == 1)	{
			drawbox(i, 0, 0, 1, 0);
			drawboxpaint(i, 0, 0, 0.7 + stagecolorg*0.3, 0);
		}
	}
	//�ǂ̕`��
	for (int j(0); j <= 21; j++){
		drawbox(0, j, 0, 1, 0);
		drawboxpaint(0, j, 0, 0.7 + stagecolorg*0.3, 0);
		drawbox(11, j, 0, 1, 0);
		drawboxpaint(11, j, 0, 0.7 + stagecolorg*0.3, 0);
	}
	//�u���b�N�̕`��
	for (int j(1); j <= 21; j++){
		for (int i(1); i <= 10; i++){
			switch (f[i][j]){
				//case 0: drawbox(i, j, 0.5 + stagecolorr*0.5, 0.5 + stagecolorg*0.5, 0.5 + stagecolorb*0.5); break;
			case 1: drawbox(i, j, 0, 0, 0.7 + stagecolorb*0.3); drawboxpaint(i, j, 0.5 + stagecolorr*0.5, 0.5 + stagecolorg*0.5, 1); break;//�����ĂȂ��ق��͐����܂����B
			case 2: drawbox(i, j, 0.7 + stagecolorr*0.3, 0, 0); drawboxpaint(i, j, 1, 0.5 + stagecolorg*0.5, 0.5 + stagecolorb*0.5); break;//�����Ă���͐Ԃ����܂����B
			case 3: drawbox(i, j, 0.75, 0.75, 0.75); drawboxpaint(i, j, 0.99, 0.99, 0.99); break;//�u���b�N�V���h�[�̂܂������̕`��D�F
			case 4: drawbox(i, j, 0.5 + stagecolorr*0.5, 0.5 + stagecolorg*0.5, 0.5 + stagecolorb*0.5); drawboxpaint(i, j, 0.75, 0.75, 0.75); break;//�S�[�X�g�u���b�N�̕�
			default: break;
			}
		}
	}
	for (int j(22); j <= 23; j++){//�݂͂����Ă��镔���̂��
		for (int i(1); i <= 10; i++){
			switch (f[i][j]){
			case 1: drawbox(i, j, 0, 0, 0.7 + stagecolorb*0.3); drawboxpaint(i, j, 0.5 + stagecolorr*0.5, 0.5 + stagecolorg*0.5, 1); break;//�����ĂȂ��ق��͐����܂����B
			case 2: drawbox(i, j, 0.7 + stagecolorr*0.3, 0, 0); drawboxpaint(i, j, 1, 0.5 + stagecolorg*0.5, 0.5 + stagecolorb*0.5); break;//�����Ă���͐Ԃ����܂����B
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
void nextmi(int &x, int &y, int &p){//i�@�_
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
void nextm0(int &x, int &y, int &p){// nextbox�p�Ȃ񂾂��킩��Ȃ��z�p�ł��B 0���o��������Ƃ��ɂǂ����悤���Ă���
	p = 0;
	for (int i(0); i < 5; i++){
		for (int j(0); j < 4; j++){
			nextf[x + i - 1][y + j - 1] = 2;
		}
	}
}
void displayfunc2nextboxes(){//���ɏo�Ă���z�̕\��
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
		//�d�؂����낤
		for (int j(0); j < 5; j++){
			nextf[j][18 - i * 5 - 2] = 3;
		}
	}
	for (int j(0); j < 25; j++){
		for (int i(0); i < 12; i++){
			if (nextf[i][j] == 2)drawbox(i + 13, j, 0, 1, 1);//���F
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
void holdmi(int &x, int &y, int &p){//i�@�_
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
void displayfunc2holdboxes(){//���ɏo�Ă���z�̕\��
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
	//hold�g�̒�`
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
			if (holdf[i][j] == 2)drawbox(i - 7, j + 16, 0.5, 0, 0.5);//��
			if (holdf[i][j] == 3)drawbox(i - 7, j + 16, 1, 0.5, 0);//�I�����W
		}
	}
}
void displayfunc2ghostboxes(){
	using namespace SDGLibF;
	//�u���b�N�̕`��
	for (int j(21); j >= 0; j--){
		for (int i(1); i <= 10; i++){
			switch (ghostf[i][j]){
			case 4: drawbox(i, j, 0.5 + stagecolorr*0.5, 0.5 + stagecolorg*0.5, 0.5 + stagecolorb*0.5); drawboxpaint(i, j, 0.75, 0.75, 0.75); break;//�S�[�X�g�u���b�N�̕�
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
		displayfunc2boxes();//�����邱�ƂŃv���O�������������肩����悤�ɂ�������ł�
		displayfunc2nextboxes();//���ɏo�Ă���z��̕\��
		displayfunc2holdboxes();
		displayfunc2keycon();
		//displayfunc2ghostboxes();

		//���Ԃ̕\���A���x�������̕\��
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
		//���ݎ����̎擾
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

		//���O�̓���
		SetColor(0, 0, 0);
		DrawString(110, 240, name.c_str());

		char buff01[100];
		if (keyboardflag == 1){
			DrawLine(2, 80, 235, 260, 235);
			sprintf_s(buff01, "please input your name => ENTER");
			DrawString(10, 210, buff01);
		}

		//highscore�̕\��
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


		//�����I�Ȃ���
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
				y--;
				if (y <= 0) {
					y = 1;
					v = kv;
				}
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

void simulation(void){//���ۂɓ������֐�
	using namespace SDGLibF;
	down();
	minoselect(x, y, p, kind, tt, harddropflag, kv, v);
	harddrop(y, harddropflag, kv, v);
	ghostbox(x, y, p, kind, tt, harddropflag, kv, v);
	brockshadow(x, y, p, kind, tt, harddropflag, kv, v);
	cheating();

	//���̒�` //���ƕǂ������Ȃ��悤�ɂ�����ɂ����ꂽ
	for (int i(1); i <= 10; i++){
		f[i][0] = 1;
	}
	//�ǂ̒�`
	for (int j(0); j <= 24; j++){
		f[0][j] = 1;
		f[11][j] = 1;
	}
	//�Q�[���I�[�o�[�̔��ʁA��ʐ؂�ւ��A�t�@�C���o��
	if (f[5][20] == 1 || f[6][20] == 1){
		displayflag = 2; IdleFunc(simulationnameinput); keyboardflag = 1;
	}
	//uncrashable(x);
	ReDraw();
}


void tanaka(){//���܂��@�����u���b�N�̐ݒ�@�c��
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
void taichi(){//���܂��@�����u���b�N�̐ݒ�@����
	f[5][18] = 1;
	f[2][17] = 1; f[3][17] = 1; f[4][17] = 1; f[5][17] = 1; f[6][17] = 1; f[7][17] = 1; f[8][17] = 1;
	f[5][16] = 1;
	f[4][15] = 1; f[6][15] = 1;
	f[3][14] = 1;  f[7][14] = 1;
	f[2][13] = 1;  f[8][13] = 1;
	f[5][13] = 1;
	f[2][10] = 1; f[3][10] = 1; f[4][10] = 1; f[5][10] = 1; f[6][10] = 1; f[7][10] = 1; f[8][10] = 1;
}
void randombypi(){//random���C�u�����������Ă��Ȃ��w�Z�p�̗��������֐�
	using namespace std;
	std::string s;
	int len(0);
	s = "1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412";//��̐����̂��߂ɉ~�������g���Ă݂�B
	len = s.length();
	for (int i(0); i < len; i++){
		srand(s[i] * 31);
		minosequence[i] = rand() % 7 + 1;
	}

}

/////�������炪visual studio2008�ł͂ł��Ȃ� random�N���X�̎����𑁂����Ƃ�
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
}					 //�ق�Ƃ�7�o���ău���b�N�ɂ������񂾂��ǂˁ[
}
}
////

void definition(){//�����l��`�֐�
	//���g�̏����l�̒�`
	for (int i(1); i <= 10; i++){
		for (int j(1); j <= 24; j++){
			f[i][j] = 0;
		}
	}
	//���̒�`
	for (int i(1); i <= 10; i++){
		f[i][0] = 1;
	}
	//�ǂ̒�`
	for (int j(0); j <= 24; j++){
		f[0][j] = 1;
		f[11][j] = 1;
	}

	//tanaka();
	//taichi();

	//randombypi();
	randombyrandom();

	//tanaka();//�����u���b�N�̐ݒ�̂��܂��ł�
	//taichi();//�����u���b�N�̐ݒ�̂��܂��ł�

	//hold�̏����l�̒�`
	for (int i(0); i < 8; i++){
		for (int j(0); j <= 8; j++){
			holdf[i][j] = 0;
		}
	}
	//next�̏����l�̒�`
	for (int i(0); i < 12; i++){
		for (int j(0); j < 25; j++){
			nextf[i][j] = 0;
		}
	}
	//ghost�̏����l�̒�`
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
		case VK_RETURN: p++; if (p >= 4) p %= 4; break; //���v��]
		case 'l': p--; if (p <= -1) p += 4; break; //�����v��]
		case 'v': if (v <= 100)v -= 10; if (v > 100 && v <= 1000)v -= 100; if (v > 1000) v -= 1000; if (v <= 0) v = 100; break;//���x������
		case 'V': if (v <= 1000)v += 100; if (v > 1000) v += 1000; if (v > 10000) v = 10000; break;//���x������
		case 'S': downflag += 1; break;//�����~�߂�
		case VK_BACK: swap(kind, holdkind);
		case 'r':if (displayflag == 2){//���X�^�[�g
					 displayflag = 0;
					 tt = 0;
					 kind = minosequence[minosum], holdkind = 4;
					 x = 5, y = 20, p = 0;
					 v = 500, downflag = 1;
					 linepoint = 0;
					 minosum = 0;
					 stagecolorr = 1, stagecolorg = 1, stagecolorb = 1;//�u���b�N�̐F�Ƃ�
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
	if (keyboardflag == 1){//�܂�����������I�ȋɂ݂ł����A�ق��ɕ��@���킩��܂���ł����B
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
	SDGLib mygraphic(800, 600, "- Fake TETRiS - (61218525 ���䒉�@)", -200, 600, 0, 600.0);
	mygraphic.SetCursor(GLUT_CURSOR_WAIT);
	mygraphic.Keyboard(keyboardfunc);
	mygraphic.Display(displayfunc);
	mygraphic.Start();
	return 0;
}

/*
�����_
�Enext�̕\��
�E�֐��̑��p�ɂ��g���̂��₷��
�E�z��𖳗����f�J���g���W�n�̕����ɍ��킹�邱�Ƃɂ���ăO���t�B�b�N�����₷�������B
�E�A���S���Y�������ׂ�1����l�������ƁA�؋��Ƃ��Ď菑���̎�������B
�E�z�[���h�@�\��ǉ����܂����B
�E�z�[���h���g���Ă��ʒu�Ə�Ԃ͕ۑ������B
�E�z�[���h�A�l�N�X�g���ꂼ��Ŕz���V���ɍ쐬���邱�Ƃɂ���Ĕz��ŏd������Ȃǂ̃o�O���Ȃ��悤�ɍH�v�B
�Elinepoint�ŃJ�E���g���邱�Ƃɂ���ėႦ��10�i��������X�s�[�h�����A�w�i�F��ύX�����B�X�e�[�W�̂悤�Ȃ��́B
�E�ŏ�����i���Z�b�g�j��t���܂����B�Ō�̉�ʂ���݈̂ڂ��悤�ɂ��܂����B
�E���̂���ł�悤�ɂȂ����B
�E��C�ɗ�����i�n�[�h�h���b�v���^���I�Ɏ������܂����B�i���x���ŏ��ɂ��邱�ƂŎ����j
�E�����ɏ������Ƃ��Ɉ�C�ɏ����̂����˂΃e�g���X�ɂȂ�˂��ォ��񂷂��Ƃŉ���
�E�Ȃ񂩏����������Ⴄ���Ƃ����܂ɋN���遨���ƕǂ���ɒ�`�������邱�Ƃŉ���
�Evisual studio��ver�ɂ����random���C�u�����̎g�p�ۈقȂ邽�߂���ɑΉ��ł���悤�ɂ����B
�E���ׂĒ����_�ɂȂ�R�}���h��ǉ����܂����B�w�肵���񐔂����o������͌��̐���ɖ߂�܂��B

�v���P
�E�ςݏd�Ȃ��Ă�z�ɉ�����Ԃ���Ȃ��悤�ɂ���

���悭�Ȃ邽�߂�
�E�A�������ɂ��X�R�A�A�b�v
�E�e�g���~�m�̏o������7��1�u���b�N�ɂ���1�u���b�N�őS���o��悤�ɂ������B
�E�������c�ł����炢����
�E�����X�^�[�g��ʂł������̃Q�[�����I�ׂ�悤�ɂ�����
�E�܂��͐ςݏd�Ȃ��Ă���Ƃ���̒��ŉ~�����˕Ԃ��Ă�Ɩʔ�����������Ȃ��ł��ˁ@�h�肾���B�����ςݏd�Ȃ��Ă��邾���ł��y���߂�B
�E���n����ꏊ�̕\���@�S�[�X�g�u���b�N
�E�n�C�X�R�A������Ƃ����ˁ[�Atxt�t�@�C���܂���excel�t�@�C���ւ̏����o������ѓǂݏo���̎���
�E�A�C�e��������Ɩʔ����񂾂��ǂȁ[
�E�e�g���~�m���ƂɐF�ς��āA�����ɂ������Ă�����F�ς�����悤�ɂ������B����͂��ꂩ�A�N���X���x�N�g�����g���Əo����̂��ȁH�z���
�@1���Ƃɂ�������̏�����������C���[�W
 �E�ΐ�^�ɂ���
 �E3D�������ł���΂����Ȃ��Ċ����B
 �E��������̉�ʂŃO���t�B�b�N�̕ύX�̉ۂ�Q�[���̑I����₢�����B
 �E�����u���b�N�̐ݒ�Ɋւ��Ă�Excel�t�@�C������ǂݍ���ł��̂܂܏o�͂ł���Ζʔ����̂ł͂Ȃ����낤���B
 �E�Ղ�Ղ�ƃe�g���X�A�������킹�Q�[���ƃe�g���X�݂����ɁA�F��4��������������A�܂���1�i��������������Ƃ����ʔ����������


 �C�Â�������
 �E7�܂ł�1-7�ŏ��ԂɊ����Ă������]����̗p����ƕ΂肪�����Ă��܂��̂�7��1�u���b�N�̍쐬������B
 �Erandombypi��randombyrandom�ɔ�ׂĕ΂肪����B�Ȃ��Ȃ�0-9�܂ł̐�����7�Ŋ������Ƃ��̂��܂�ōl���邽��
 �@1,2,3,4���ق��̐��������o�₷���d�l�ɂȂ��Ă���B
  �E�������͂�cmd�����点�悤�Ƃ���Ɠr���Ŏ~�܂��Ă��܂��ăE�B���h�E��ς�����ʓ|�Ȃ��Ƃ������̂�
  �@keyboardfunc�̒����t���O�ł킯�ĕ������͂��ł���悤�ɂ��܂����B
   �EReDraw������ꏊ�̊֌W�ŁA�����Ȃ�������ReDraw����������֐�������Ă��������������悤��
   */

