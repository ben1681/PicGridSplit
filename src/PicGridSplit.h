#pragma once
#include <iostream>
#include <tchar.h>
#include <opencv2/imgcodecs.hpp>

#define PRO_NAME "��ͼ-�Ź���"
#define PRO_NAME_T _T("��ͼ-�Ź���")
#define NUM_WIN_WIDTH 60
#define NUM_WIN_HEIGH 30
#define NUM_SPACE_OP 20
#define NUM_SECTION_SIZE_DEFAULT 3 // ��Ƭ���Ĭ��ֵ
#define NUM_SECTION_SIZE_MIN 2 // ��Ƭ�����Сֵ
#define NUM_SECTION_SIZE_MAX 30 // ��Ƭ������ֵ
#define CHAR_ENDL '\n'
#define CHAR_DIR_SEP '/'
#define TEXT_PRO_DESC "�ó�����Ҫ��ָ��ͼƬ�ȷ�Ϊָ����������Ƭͼ, �����浽ָ���ļ�����, ���û�ʹ��. Ŀ¼�ָ���ͳһ���� '/'. ��Ҫ�����²���(�������ִ��):"

enum OPSigalSets /* �����źż��� ö������ */
{
	OP_EMPTY = 0, OP_VIEW_CONF = 1, OP_SET_PATH_STORAGE=2, OP_SECTION_SIZE=3, OP_SELECT_TARGET_PIC=4, OP_VIEW_TARGET_PIC=5, OP_SECTION_MAKER=6, OP_ESC=7
};

enum FileInfo
{
	FI_INIT = 0, FI_NAME = 1, FI_EXT = 2
};

struct State
{
	bool FlagExited; // �˳���ʶ
	unsigned long TimeProStart; // ��������ʱ��
	unsigned long TimeFrameStart; // ֡-����ʱ��
};

class PicGridSplit
{
private:
	std::string PathStorageMain; // �洢��·��
	std::string PicNameCustom;   // �û�����ͼƬ·��
	cv::Mat TmpTargetPic; // Ŀ��ͼƬ��ʱ
	unsigned short NumSectionSize; // ��Ƭ��� n*n
	OPSigalSets CurrentOPSigalCustom; // ��ǰ�û��ź�
	State State;
private:
	void OPCall_1() const;
	void OPCall_2();
	void OPCall_3();
	void OPCall_4();
	void OPCall_5() const;
	void OPCall_6();
public:
	PicGridSplit() : NumSectionSize(NUM_SECTION_SIZE_DEFAULT), CurrentOPSigalCustom(OP_EMPTY), State({}) {}
	~PicGridSplit() {}

	void InitWindow() const; /* ��ʼ��-���� */
	void RenderMenu() const; /* ��Ⱦ-�˵� */
	void RenderWindow() const; /* ��Ⱦ-���� */
	void UpdaterData(); /* ����-���� */
	bool CheckerExit() const; /* �����-�˳� */ 
};