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
	SetConsoleTitle(PRO_NAME_T); // ���ô�������
	system("COLOR B1"); // color ����ɫ����ɫ ǳ��|��ɫ
	system("mode con cols=60 lines=30"); // cols ������ lines ������
	/* ����opencv��־��ʾ���� */
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);
}

void PicGridSplit::RenderMenu() const
{
	system("cls");
	std::cout << std::setw((NUM_WIN_WIDTH >> 1) + (NUM_WIN_HEIGH >> 2)) << PRO_NAME << '\n'
		<< TEXT_PRO_DESC << '\n'
		<< std::setw(17) << "1.�鿴��ǰ����" << std::setw(NUM_SPACE_OP) << "2.���ñ���·��" << std::setw(NUM_SPACE_OP) << "3.������Ƭ���\n"
		<< std::setw(17) << "4.ѡ��Ŀ��ͼƬ" << std::setw(NUM_SPACE_OP) << "5.�鿴Ŀ��ͼƬ" << std::setw(NUM_SPACE_OP) << "6.������Ƭ����\n"
		<< std::setw(17) << "7.�˳���ǰ����\n";
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
		std::cout << "������������(Ĭ��0ˢ��): ";
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
			std::cout << "������ŷǷ�" << std::endl;
		}
		this->CurrentOPSigalCustom = OP_EMPTY;
		//Sleep(1000); 
	} while (!this->State.FlagExited);
	
}

void PicGridSplit::OPCall_1() const
{
	std::cout << "�洢·��: " << (!this->PathStorageMain.empty() ? this->PathStorageMain : "./")
		<< "\nĿ��ͼƬ: " << (!this->PicNameCustom.empty() ? this->PicNameCustom : "��δ����")
		<< "\n��Ƭ���: " << this->NumSectionSize << "x" << this->NumSectionSize << '\n';
}

void PicGridSplit::OPCall_2()
{
	std::string tmpPath;
	unsigned short tmpPathLen = 0;
	char tmpDirPath[256] = { 0 };
	std::cout << "��������Ƭ����·��: ";
	std::cin >> tmpPath;
	tmpPathLen = (unsigned short)tmpPath.length();
	if (tmpPathLen > 0) {
		if (tmpPath.find(CHAR_DIR_SEP) == tmpPath.npos) {
			std::cout << "��ǰ·����Ч\n";
		} else {
			if (tmpPath[tmpPathLen - 1] != CHAR_DIR_SEP) { // ����
				tmpPath += CHAR_DIR_SEP;
				tmpPathLen++;
			}
			for (unsigned short i = 0; i < tmpPathLen; i++)
			{
				tmpDirPath[i] = tmpPath[i];
				if (tmpDirPath[i] == CHAR_DIR_SEP) {
					if (_access(tmpDirPath, 6) == -1) {
						if (_mkdir(tmpDirPath) == -1) {
							std::cout << "��ǰ·���޷�����: " << tmpDirPath << '\n';
						}
						else {
							std::cout << "·�������ɹ�: " << tmpDirPath << '\n';
							this->PathStorageMain = tmpDirPath;
						}
					} else {
						this->PathStorageMain = tmpDirPath;
					}
				}
			}
		}
	}
	if (!this->PathStorageMain.empty()) std::cout << "��ǰ�洢·��: " << this->PathStorageMain << '\n';
}

void PicGridSplit::OPCall_3()
{
	std::cout << "��������Ƭ���(2~30): ";
	std::cin >> this->NumSectionSize;
	if (this->NumSectionSize < NUM_SECTION_SIZE_MIN || this->NumSectionSize > NUM_SECTION_SIZE_MAX) {
		std::cout << "��Ч���: " << this->NumSectionSize << ", ����ΪĬ��ֵ: " << NUM_SECTION_SIZE_DEFAULT << '\n';
		this->NumSectionSize = NUM_SECTION_SIZE_DEFAULT;
	} else {
		std::cout << "������óɹ�. ��Ƭ���: " << this->NumSectionSize << '\n';
	}
}

void PicGridSplit::OPCall_4()
{ 
	do {
		std::cout << "������ͼƬ����·��(q�˳���ǰ����): ";
		std::cin >> this->PicNameCustom;
		if (this->PicNameCustom.empty())  continue;
		if (this->PicNameCustom.find(CHAR_DIR_SEP) == this->PicNameCustom.npos) {
			std::cout << "��ǰĿ��ͼƬ·����Ч." << std::endl;
			continue;
		}
		if (this->PicNameCustom == "q") {
			this->PicNameCustom = "";
			this->TmpTargetPic = NULL;
			break;
		}
		this->TmpTargetPic = cv::imread(this->PicNameCustom);
		if (this->TmpTargetPic.empty()) {
			std::cout << "��ǰĿ��ͼƬ·����Ч." << std::endl;
		}
	} while (!this->PicNameCustom.empty() && this->TmpTargetPic.empty());

	if (!this->PicNameCustom.empty()) std::cout << "��ǰĿ��ͼƬ·��: " << this->PicNameCustom << std::endl;
}

void PicGridSplit::OPCall_5() const
{
	if (this->PicNameCustom.empty() || this->TmpTargetPic.empty()) { /* ���ͼƬ�Ƿ��ѽ��� */
		std::cout << "��δ����Ŀ��ͼƬ" << std::endl;
		return;
	}
	cv::String winName = "Ŀ��ͼƬ";
	cv::namedWindow(winName, cv::WINDOW_AUTOSIZE); // ��������ͼƬ�Ĵ���
	cv::imshow(winName, this->TmpTargetPic); // ��ʾͼƬ��ָ������
	cv::waitKey(0); // ���Զ��ر�, ���û����д���
	//cv::destroyWindow(winName); // ����ᴥ���쳣, ��δ����
}

void PicGridSplit::OPCall_6()
{
	if (this->PicNameCustom.empty() || this->TmpTargetPic.empty()) { /* ���ͼƬ�Ƿ��ѽ��� */
		std::cout << "��δ����Ŀ��ͼƬ\n";
		return;
	}
	std::string fileName = GetFileInfoInPath(this->PicNameCustom, FI_NAME);
	std::string fileExt = GetFileInfoInPath(this->PicNameCustom, FI_EXT);
	std::string sectionStorage = this->PathStorageMain + fileName;
	if (_access(sectionStorage.c_str(), 6) != -1) _rmdir(sectionStorage.c_str());
	if (_access(sectionStorage.c_str(), 6) == -1) {
		if (_mkdir(sectionStorage.c_str()) == -1) {
			std::cout << "��ǰ��Ƭ����Ŀ¼����ʧ��: " << sectionStorage << '\n';
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
			std::cout << (x + y * this->NumSectionSize + 1) << "." << tmpName << "��Ƭ����";
			if (!cv::imwrite(tmpName, dst, compressionParams)) {
				CounterFail++;
				std::cout << "ʧ��'\n";
			} else {
				CounterSuccess++;
				std::cout << "�ɹ�\n";
			}

		}
	}
	if (CounterSuccess == (this->NumSectionSize * this->NumSectionSize)) {
		this->PicNameCustom = "";
		this->TmpTargetPic = NULL;
		std::cout << "��Ƭ����ɹ�, Ŀ��ͼƬ����������\n";
	} else if (CounterFail == (this->NumSectionSize * this->NumSectionSize)) {
		std::cout << " ��Ƭ����ʧ��, ������ƬĿ¼";
		if (_rmdir(sectionStorage.c_str()) == -1) {
			std::cout << "ʧ��\n";
		} else {
			std::cout << "�ɹ�\n";
		}
	} else {
		std::cout << "��Ƭ����, �ɹ�: " << CounterSuccess << ", ʧ��: " << CounterFail << ", ���ٴ�ִ�е�ǰ����\n";
	}

}
// E:/Tmp/
// E:/�ӻ���/a.jpg
// E:/�ӻ���/wx.jpg