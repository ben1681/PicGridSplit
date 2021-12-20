#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <io.h>
#include <direct.h>
#include <windows.h>
#include <tchar.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "PicGridSplit.h"

static std::string GetFileInfoInPath(const std::string& filePath, FileInfo flag)
{
	std::string::size_type pos = filePath.find_last_of(CHAR_DIR_SEP) + 1;
	std::string fileName = filePath.substr(pos, filePath.length() - pos);
	if (flag == FI_NAME) {
		return fileName.substr(0, fileName.rfind("."));
	} else if (flag == FI_EXT) {
		return fileName.substr(fileName.find_last_of('.') + 1);
	}
	return fileName;
}

bool PicGridSplit::CheckerExit() const
{
	return this->CurrentOPSigalCustom == OPSigalSets::OP_ESC;
}

void PicGridSplit::InitWindow() const
{
	SetConsoleTitle(PRO_NAME_T); // 设置窗口名称
	system("COLOR B1"); // color 背景色字体色 浅绿|蓝色
	system("mode con cols=60 lines=30"); // cols 宽列数 lines 高行数
	/* 限制opencv日志显示级别 */
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);
}

void PicGridSplit::RenderMenu() const
{
	system("cls");
	std::cout << std::setw((NUM_WIN_WIDTH >> 1) + (NUM_WIN_HEIGH >> 2)) << PRO_NAME << '\n'
		<< TEXT_PRO_DESC << '\n'
		<< std::setw(17) << "1.查看当前配置" << std::setw(NUM_SPACE_OP) << "2.设置保存路径" << std::setw(NUM_SPACE_OP) << "3.设置切片规格\n"
		<< std::setw(17) << "4.选择目标图片" << std::setw(NUM_SPACE_OP) << "5.查看目标图片" << std::setw(NUM_SPACE_OP) << "6.制作切片保存\n"
		<< std::setw(17) << "7.退出当前程序\n";
}

void PicGridSplit::RenderWindow() const
{
	this->RenderMenu();
}

void PicGridSplit::UpdaterData()
{
	std::string tmpStrBuff;
	int tmpOP = 0;
	do {
		tmpStrBuff.clear();
		std::cout << "请输入操作序号(默认0刷屏): ";
		std::cin >> tmpStrBuff;
		tmpOP = atoi(tmpStrBuff.c_str());
		if (tmpOP >= OP_EMPTY && tmpOP <= OP_ESC) {
			this->CurrentOPSigalCustom = (OPSigalSets)tmpOP;
			switch (this->CurrentOPSigalCustom)
			{
			case OP_VIEW_CONF:
				this->OPCall_1(); break;
			case OP_SET_PATH_STORAGE:
				this->OPCall_2(); break;
			case OP_SECTION_SIZE:
				this->OPCall_3(); break;
			case OP_SELECT_TARGET_PIC:
				this->OPCall_4(); break;
			case OP_VIEW_TARGET_PIC:
				this->OPCall_5(); break;
			case OP_SECTION_MAKER:
				this->OPCall_6(); break;
			case OP_ESC:
				this->State.FlagExited = 1; break;
			default:
				this->RenderWindow(); break;
			}
		} else {
			std::cout << "操作序号非法" << std::endl;
		}
		this->CurrentOPSigalCustom = OP_EMPTY;
		//Sleep(1000); 
	} while (!this->State.FlagExited);
	
}

void PicGridSplit::OPCall_1() const
{
	std::cout << "存储路径: " << (!this->PathStorageMain.empty() ? this->PathStorageMain : "./")
		<< "\n目标图片: " << (!this->PicNameCustom.empty() ? this->PicNameCustom : "暂未设置")
		<< "\n切片规格: " << this->NumSectionSize << "x" << this->NumSectionSize << '\n';
}

void PicGridSplit::OPCall_2()
{
	std::string tmpPath;
	unsigned short tmpPathLen = 0;
	char tmpDirPath[256] = { 0 };
	std::cout << "请输入切片保存路径: ";
	std::cin >> tmpPath;
	tmpPathLen = (unsigned short)tmpPath.length();
	if (tmpPathLen > 0) {
		if (tmpPath.find(CHAR_DIR_SEP) == tmpPath.npos) {
			std::cout << "当前路径无效\n";
		} else {
			if (tmpPath[tmpPathLen - 1] != CHAR_DIR_SEP) { // 补充
				tmpPath += CHAR_DIR_SEP;
				tmpPathLen++;
			}
			for (unsigned short i = 0; i < tmpPathLen; i++)
			{
				tmpDirPath[i] = tmpPath[i];
				if (tmpDirPath[i] == CHAR_DIR_SEP) {
					if (_access(tmpDirPath, 6) == -1) {
						if (_mkdir(tmpDirPath) == -1) {
							std::cout << "当前路径无法创建: " << tmpDirPath << '\n';
						}
						else {
							std::cout << "路径创建成功: " << tmpDirPath << '\n';
							this->PathStorageMain = tmpDirPath;
						}
					} else {
						this->PathStorageMain = tmpDirPath;
					}
				}
			}
		}
	}
	if (!this->PathStorageMain.empty()) std::cout << "当前存储路径: " << this->PathStorageMain << '\n';
}

void PicGridSplit::OPCall_3()
{
	std::cout << "请输入切片规格(2~30): ";
	std::cin >> this->NumSectionSize;
	if (this->NumSectionSize < NUM_SECTION_SIZE_MIN || this->NumSectionSize > NUM_SECTION_SIZE_MAX) {
		std::cout << "无效规格: " << this->NumSectionSize << ", 重置为默认值: " << NUM_SECTION_SIZE_DEFAULT << '\n';
		this->NumSectionSize = NUM_SECTION_SIZE_DEFAULT;
	} else {
		std::cout << "规格设置成功. 切片规格: " << this->NumSectionSize << '\n';
	}
}

void PicGridSplit::OPCall_4()
{ 
	do {
		std::cout << "请输入图片绝对路径(q退出当前操作): ";
		std::cin >> this->PicNameCustom;
		if (this->PicNameCustom.empty())  continue;
		if (this->PicNameCustom.find(CHAR_DIR_SEP) == this->PicNameCustom.npos) {
			std::cout << "当前目标图片路径无效." << std::endl;
			continue;
		}
		if (this->PicNameCustom == "q") {
			this->PicNameCustom = "";
			this->TmpTargetPic = NULL;
			break;
		}
		this->TmpTargetPic = cv::imread(this->PicNameCustom);
		if (this->TmpTargetPic.empty()) {
			std::cout << "当前目标图片路径无效." << std::endl;
		}
	} while (!this->PicNameCustom.empty() && this->TmpTargetPic.empty());

	if (!this->PicNameCustom.empty()) std::cout << "当前目标图片路径: " << this->PicNameCustom << std::endl;
}

void PicGridSplit::OPCall_5() const
{
	if (this->PicNameCustom.empty() || this->TmpTargetPic.empty()) { /* 检测图片是否已解析 */
		std::cout << "暂未设置目标图片" << std::endl;
		return;
	}
	cv::String winName = "目标图片";
	cv::namedWindow(winName, cv::WINDOW_AUTOSIZE); // 创建适配图片的窗口
	cv::imshow(winName, this->TmpTargetPic); // 显示图片到指定窗口
	cv::waitKey(0); // 无自动关闭, 等用户自行处理
	//cv::destroyWindow(winName); // 这里会触发异常, 暂未处理
}

void PicGridSplit::OPCall_6()
{
	if (this->PicNameCustom.empty() || this->TmpTargetPic.empty()) { /* 检测图片是否已解析 */
		std::cout << "暂未设置目标图片\n";
		return;
	}
	std::string fileName = GetFileInfoInPath(this->PicNameCustom, FI_NAME);
	std::string fileExt = GetFileInfoInPath(this->PicNameCustom, FI_EXT);
	std::string sectionStorage = this->PathStorageMain + fileName;
	if (_access(sectionStorage.c_str(), 6) != -1) _rmdir(sectionStorage.c_str());
	if (_access(sectionStorage.c_str(), 6) == -1) {
		if (_mkdir(sectionStorage.c_str()) == -1) {
			std::cout << "当前切片保存目录创建失败: " << sectionStorage << '\n';
			return;
		}
	}

	int imgWidth = this->TmpTargetPic.cols;
	int imgHigh = this->TmpTargetPic.rows;

	int colSize = imgWidth / this->NumSectionSize;
	int rowSize = imgHigh / this->NumSectionSize;

	std::vector<int> compressionParams;
	compressionParams.push_back(cv::IMWRITE_PNG_COMPRESSION);
	compressionParams.push_back(100);

	int CounterSuccess = 0;
	int CounterFail = 0;

	for (int y = 0; y < this->NumSectionSize; y++)
	{
		for (int x = 0; x < this->NumSectionSize; x++)
		{
			int tmpWidth = colSize;
			int tmpHeigh = colSize;
			if ((tmpWidth + x * colSize) > imgWidth) tmpWidth = imgWidth - x * colSize;
			if ((tmpHeigh + y * rowSize) > imgHigh) tmpHeigh = imgHigh - y * rowSize;
			cv::Rect tmpRect(x * colSize, y * rowSize, colSize, rowSize);
			cv::Mat dst = this->TmpTargetPic(tmpRect);
			std::stringstream tmpStream;
			tmpStream << y << "_" << x;
			std::string tmpName = sectionStorage + CHAR_DIR_SEP + fileName + "_" + tmpStream.str() + ".png";
			std::cout << (x + y * this->NumSectionSize + 1) << "." << tmpName << "切片创建";
			if (!cv::imwrite(tmpName, dst, compressionParams)) {
				CounterFail++;
				std::cout << "失败'\n";
			} else {
				CounterSuccess++;
				std::cout << "成功\n";
			}

		}
	}
	if (CounterSuccess == (this->NumSectionSize * this->NumSectionSize)) {
		this->PicNameCustom = "";
		this->TmpTargetPic = NULL;
		std::cout << "切片任务成功, 目标图片数据已重置\n";
	} else if (CounterFail == (this->NumSectionSize * this->NumSectionSize)) {
		std::cout << " 切片任务失败, 销毁切片目录";
		if (_rmdir(sectionStorage.c_str()) == -1) {
			std::cout << "失败\n";
		} else {
			std::cout << "成功\n";
		}
	} else {
		std::cout << "切片任务, 成功: " << CounterSuccess << ", 失败: " << CounterFail << ", 可再次执行当前操作\n";
	}

}
// E:/Tmp/
// E:/杂货店/a.jpg
// E:/杂货店/wx.jpg