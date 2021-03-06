/*!
 * \file AlgorithmManager.h
 * \date 2017/05/09 17:40
 *
 * \author blue
 * Contact: yang.wang@shangyiyun.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#pragma once

#include "ErrorCode.h"
#include "MedicalAnalysisSDK.h"

namespace SYY {

	class AlgorithmManager {
	public:
		ErrorCode Init();
		ErrorCode Release();

		ErrorCode InitBUAnalysis( HANDLE& hHandle, unsigned long nMode);

		ErrorCode ReleaseBUAnalysis( HANDLE& hHandle );

		ErrorCode ExecuteBUAnalysis(HANDLE hHandle,
			char* pImg, int nImgWidth, int nImgHeight,
			MedicalAnalysis::BUAnalysisResult* pResult);

		ErrorCode ExecuteBUAnalysisFromImage(
			HANDLE hHandle,
			Image* pImage,
			MedicalAnalysis::BUAnalysisResult* pResult);

		ErrorCode InitInpaint(HANDLE& hHandle, unsigned long nMode);
		ErrorCode ReleaseInpaint(HANDLE hHandle);

		ErrorCode ExecuteInpaint(HANDLE hHandle, 
			Image srcImg, Image maskImg, 
			Image& inpaintImg);

		ErrorCode DrawResult2Image(
			Image* pImage,
			MedicalAnalysis::BUAnalysisResult* pResult);	

		ErrorCode LoadVideo(
			const char* pVideoFile,
			const int nVideoFileStrLength,
			HANDLE& hHandle,
			Image* pFrameDesc
			);

		ErrorCode GetVideoFrame(
			HANDLE hHandle,
			Image* pImage );

		ErrorCode ReleaseVideo( HANDLE& hHandle );

	private:

	};

}
