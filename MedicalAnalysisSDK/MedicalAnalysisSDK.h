/*!
 * \file MedicalAnalysisSDK.h
 * \date 2017/05/09 16:20
 *
 * \author blue
 * Contact: yang.wang@shangyiyun.com
 *
 * \brief 
 *
 * description: sdk header
 *
 * \note
*/

#pragma once

#include "ErrorCode.h"

#ifdef MEDICAL_ANALYSIS_SDK_EXPORTS
#define MEDICAL_ANALYSIS_SDK_API __declspec(dllexport) 
#else
#define MEDICAL_ANALYSIS_SDK_API __declspec(dllimport) 
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

namespace SYY { 

	struct Rect { 
		int x, y, w, h; 
	};

	struct Image
	{
		Image() : pData(nullptr), nWidth(0), nHeight(0) {}
		Image(char* p, int w, int h) : pData(p), nWidth(w), nHeight(h) {}

		char* pData;
		int nWidth;
		int nHeight;
	};

	typedef unsigned long long HANDLE;

namespace MedicalAnalysis {


	struct BUAnalysisResult {
		BUAnalysisResult() : pLessionRects(nullptr), nLessionsCount(0) {}

		Rect rCropRect;				// ��ЧͼƬ����

		int nLessionsCount;			// ��������
		Rect* pLessionRects;		// ��������
	};

	enum BUAnalysisMode{
		None = 0x1,
		Crop_V1 = 0x2,
		Crop_V2 = 0x4,
	};

	MEDICAL_ANALYSIS_SDK_API ErrorCode InitSDK();

	MEDICAL_ANALYSIS_SDK_API ErrorCode ReleaseSDK();

	MEDICAL_ANALYSIS_SDK_API ErrorCode InitBUAnalysis(
		OUT HANDLE& hHandle,
		IN unsigned long nMode
		);

	MEDICAL_ANALYSIS_SDK_API ErrorCode ReleaseBUAnalysis(
		INOUT HANDLE& hHandle
		);

	MEDICAL_ANALYSIS_SDK_API ErrorCode ExecuteBUAnalysis(
		IN HANDLE hHandle, 
		IN char* pImg, 
		IN int nImgWidth, 
		IN int nImgHeight,
		OUT BUAnalysisResult* pResult
		);

	MEDICAL_ANALYSIS_SDK_API ErrorCode InitInpaint(
		OUT HANDLE& hHandle
		);
	MEDICAL_ANALYSIS_SDK_API ErrorCode ReleaseInpaint(
		INOUT HANDLE& hHandle
		);
	MEDICAL_ANALYSIS_SDK_API ErrorCode ExecuteInpaint(
		IN HANDLE hHandle,
		IN Image srcImg,
		IN Image maskImg,
		OUT Image& inpaintImg
		);

}
}
