#pragma once
#include <iostream>
#include <tchar.h>
#include <opencv2/imgcodecs.hpp>

#define PRO_NAME "切图-九宫格"
#define PRO_NAME_T _T("切图-九宫格")
#define NUM_WIN_WIDTH 60
#define NUM_WIN_HEIGH 30
#define NUM_SPACE_OP 20
#define NUM_SECTION_SIZE_DEFAULT 3 // 切片规格默认值
#define NUM_SECTION_SIZE_MIN 2 // 切片规格最小值
#define NUM_SECTION_SIZE_MAX 30 // 切片规格最大值
#define CHAR_ENDL '\n'
#define CHAR_DIR_SEP '/'
#define TEXT_PRO_DESC "该程序主要将指定图片等分为指定数量的切片图, 并保存到指定文件夹下, 供用户使用. 目录分隔符统一采用 '/'. 主要有以下操作(输入序号执行):"

enum OPSigalSets /* 操作信号集合 枚举类型 */
{
	OP_EMPTY = 0, OP_VIEW_CONF = 1, OP_SET_PATH_STORAGE=2, OP_SECTION_SIZE=3, OP_SELECT_TARGET_PIC=4, OP_VIEW_TARGET_PIC=5, OP_SECTION_MAKER=6, OP_ESC=7
};

enum FileInfo
{
	FI_INIT = 0, FI_NAME = 1, FI_EXT = 2
};

struct State
{
	bool FlagExited; // 退出标识
	unsigned long TimeProStart; // 程序启动时间
	unsigned long TimeFrameStart; // 帧-启动时间
};

class PicGridSplit
{
private:
	std::string PathStorageMain; // 存储主路径
	std::string PicNameCustom;   // 用户输入图片路径
	cv::Mat TmpTargetPic; // 目标图片临时
	unsigned short NumSectionSize; // 切片规格 n*n
	OPSigalSets CurrentOPSigalCustom; // 当前用户信号
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

	void InitWindow() const; /* 初始化-窗口 */
	void RenderMenu() const; /* 渲染-菜单 */
	void RenderWindow() const; /* 渲染-窗口 */
	void UpdaterData(); /* 更新-数据 */
	bool CheckerExit() const; /* 检测器-退出 */ 
};