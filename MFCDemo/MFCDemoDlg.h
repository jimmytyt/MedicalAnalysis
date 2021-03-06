
// MFCDemoDlg.h : 头文件
//

#pragma once

#include <vector>
#include <string>

#include <opencv2/opencv.hpp>
#include "MedicalAnalysisSDK.h"

// CMFCDemoDlg 对话框
class CMFCDemoDlg : public CDialogEx
{
// 构造
public:
	CMFCDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButtonOpenFolder();
	afx_msg void OnBnClickedButtonAnalysis();
	afx_msg void OnSelchangeListFilelist();


	////////////////////////////////////
protected:
	void ResetAll();
	void RefreshFileListBox();
	void GetInitImageRect();
	bool ShowImg(const cv::Mat& src);
	bool AdjustInputMatSizeAndRect(const cv::Mat& src, cv::Mat&dst, CRect& rect);
	void MatToCImage(const cv::Mat& mat, CImage& cImage);
	void ShowImgFromFileIdx(int idx);
	void SaveCurImg(const std::string& save_path);
	void Redraw();
	void ScreenPt2ImgPt(CPoint snPt, cv::Point& imgPt);
	void Inpaint_v1();
	void Inpaint_v2();
	void Inpaint_v3();
	void Inpaint_v4();
	void SetAnalysisThreshold(float thres);
	cv::Mat DrawAnalysisResults();

private:
	std::vector<std::string> m_vFiles;
	CRect m_rImageInit;
	cv::Mat m_mBgMat;
	int m_nCurFileIdx;
	cv::Mat m_mOriImg;
	cv::Mat m_mCurImg;
	cv::Mat m_mMaskImg;
	cv::Size m_resizeImgSize;

	SYY::MedicalAnalysis::BUAnalysisResult m_BUresult;
	bool m_bIsResultNeedRefresh;

	SYY::HANDLE m_hHandleBUAnalysis;
	SYY::HANDLE m_hHandleBUAnalysis2;
	SYY::HANDLE m_hHandleBUAnalysis3;
	SYY::HANDLE m_hHandleBUAnalysis4;
	SYY::HANDLE m_hHandleInpaintPM;
	SYY::HANDLE m_hHandleInpaintCriminisi;
	bool m_bIsLButtonDown;

	float m_fAnalysisThresHold;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonInpaint();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonInpaintV2();
	afx_msg void OnBnClickedButtonInpaintV3();
	afx_msg void OnBnClickedButtonInpaintV4();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnThemechangedScrollbarThreshold(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnThumbposchangingSliderThreshold(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonAnalysis2();
	afx_msg void OnBnClickedButtonAnalysis3();
	afx_msg void OnBnClickedButtonAnalysis4();
};
