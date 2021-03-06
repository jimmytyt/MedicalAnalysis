
// MFCDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "MFCDemoDlg.h"
#include "afxdialogex.h"
#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace SYY;
using namespace SYY::MedicalAnalysis;
using namespace SYY::Inpainting;


// CMFCDemoDlg 对话框

CMFCDemoDlg::CMFCDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FOLDER, &CMFCDemoDlg::OnBnClickedButtonOpenFolder)
	ON_BN_CLICKED(IDC_BUTTON_ANALYSIS, &CMFCDemoDlg::OnBnClickedButtonAnalysis)
	ON_LBN_SELCHANGE(IDC_LIST_FILELIST, &CMFCDemoDlg::OnSelchangeListFilelist)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_INPAINT, &CMFCDemoDlg::OnBnClickedButtonInpaint)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CMFCDemoDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_INPAINT_V2, &CMFCDemoDlg::OnBnClickedButtonInpaintV2)
	ON_BN_CLICKED(IDC_BUTTON_INPAINT_V3, &CMFCDemoDlg::OnBnClickedButtonInpaintV3)
	ON_BN_CLICKED(IDC_BUTTON_INPAINT_V4, &CMFCDemoDlg::OnBnClickedButtonInpaintV4)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMFCDemoDlg::OnBnClickedButtonSave)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMFCDemoDlg::OnTcnSelchangeTab1)
//	ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR_THRESHOLD, &CMFCDemoDlg::OnThemechangedScrollbarThreshold)
//ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER_THRESHOLD, &CMFCDemoDlg::OnThumbposchangingSliderThreshold)
ON_WM_HSCROLL()
ON_BN_CLICKED(IDC_BUTTON_ANALYSIS_2, &CMFCDemoDlg::OnBnClickedButtonAnalysis2)
ON_BN_CLICKED(IDC_BUTTON_ANALYSIS_3, &CMFCDemoDlg::OnBnClickedButtonAnalysis3)
ON_BN_CLICKED(IDC_BUTTON_ANALYSIS_4, &CMFCDemoDlg::OnBnClickedButtonAnalysis4)
END_MESSAGE_MAP()


// CMFCDemoDlg 消息处理程序

BOOL CMFCDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	GetInitImageRect();
	SetAnalysisThreshold(0.1f);
	m_bIsResultNeedRefresh = true;

	CSliderCtrl *sliderCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_THRESHOLD);
	sliderCtrl->SetRange(0, 100, TRUE);
	sliderCtrl->SetPos(m_fAnalysisThresHold * 100);

	auto listbox = (CListBox*)GetDlgItem(IDC_LIST_FILELIST);
	listbox->SetHorizontalExtent(1000);

	ErrorCode code;
	char msg[256];
	if (SYY_NO_ERROR != (code = InitSDK()))
	{
		sprintf_s(msg, "初始化算法失败! 错误码为%d", code);
		MessageBox(msg);
	}
	else {
		if (SYY_NO_ERROR != (code = InitBUAnalysisWithMode(m_hHandleBUAnalysis, BUAnalysisMode::Crop_V1 | BUAnalysisMode::DetectFast))) {
			sprintf_s(msg, "初始化B超分析算法1失败! 错误码为%d", code);
			MessageBox(msg);
		}
		if (SYY_NO_ERROR != (code = InitBUAnalysisWithMode(m_hHandleBUAnalysis2, BUAnalysisMode::Crop_V2 | BUAnalysisMode::DetectFast))) {
			sprintf_s(msg, "初始化B超分析算法2失败! 错误码为%d", code);
			MessageBox(msg);
		}
		if (SYY_NO_ERROR != (code = InitBUAnalysisWithMode(m_hHandleBUAnalysis3, BUAnalysisMode::Crop_V1 | BUAnalysisMode::DetectAccurate))) {
			sprintf_s(msg, "初始化B超分析算法3失败! 错误码为%d", code);
			MessageBox(msg);
		}
		if (SYY_NO_ERROR != (code = InitBUAnalysisWithMode(m_hHandleBUAnalysis4, BUAnalysisMode::Crop_V2 | BUAnalysisMode::DetectAccurate))) {
			sprintf_s(msg, "初始化B超分析算法4失败! 错误码为%d", code);
			MessageBox(msg);
		}
		if (SYY_NO_ERROR != (code = InitInpaint(m_hHandleInpaintPM, InpaintMode::PatchMatch))) {
			sprintf_s(msg, "初始化图像修复算法失败! 错误码为%d", code);
			MessageBox(msg);
		}
		if (SYY_NO_ERROR != (code = InitInpaint(m_hHandleInpaintCriminisi, InpaintMode::Criminisi_P5))) {
			sprintf_s(msg, "初始化图像修复算法失败! 错误码为%d", code);
			MessageBox(msg);
		}
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		DrawAnalysisResults();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//void CMFCDemoDlg::OnLbnSelchangeList1()
//{
//	// TODO:  在此添加控件通知处理程序代码
//}


void CMFCDemoDlg::OnBnClickedButtonOpenFolder()
{
	char szPath[MAX_PATH];     //存放选择的目录路径

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = "请选择需要打开的目录：";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(lp, szPath);

	CString folderRootPath = szPath;
	
	std::vector<std::string> files;
	scanFilesUseRecursive(folderRootPath.GetString(), files);

	ResetAll();

	for (auto file : files)
	{
		int idx = file.find_last_of('.');
		if (idx == -1)
			continue;

		auto ext = file.substr(idx + 1);
		if (ext != "jpg" && ext != "bmp" && ext != "png")
			continue;

		m_vFiles.push_back(file);
	}

	if (m_vFiles.size() == 0)
	{
		MessageBox("该目录下没有图片文件！");
		return;
	}

	RefreshFileListBox();
}


void CMFCDemoDlg::OnBnClickedButtonAnalysis()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_mCurImg.empty())
	{
		MessageBox("请先选择图片!");
		return;
	}

	auto img = m_mCurImg;

	auto& result = m_BUresult;

	ErrorCode code;
	bool bIsCrop = true;
	if (SYY_NO_ERROR != (code = 
		ExecuteBUAnalysis(m_hHandleBUAnalysis, (char*)img.data, img.cols, img.rows, &result)))
	{
		char msg[256];
		sprintf_s(msg, "b超分析算法失败！ 错误码为： %d", code);
		MessageBox(msg);
		return;
	}

	m_bIsResultNeedRefresh = false;

	DrawAnalysisResults();
}

void CMFCDemoDlg::OnSelchangeListFilelist()
{
	
	CListBox* listbox = (CListBox*)GetDlgItem(IDC_LIST_FILELIST);
	m_nCurFileIdx = listbox->GetCurSel();

	ShowImgFromFileIdx(m_nCurFileIdx);

	m_bIsResultNeedRefresh = true;
}

void CMFCDemoDlg::ResetAll()
{
	m_vFiles.clear();
}

void CMFCDemoDlg::RefreshFileListBox()
{
	CListBox* listbox_filepath = (CListBox*)GetDlgItem(IDC_LIST_FILELIST);

	listbox_filepath->ResetContent();
	int nFilesLen = m_vFiles.size();
	for (int i = 0; i < nFilesLen; i++)
	{
		CString file = CString(m_vFiles[i].c_str());

		CString line;
		line.Format(" (%d) %s", i + 1, file);

		listbox_filepath->InsertString(i, line);
	}
}

void CMFCDemoDlg::GetInitImageRect()
{
	CWnd *pWnd = GetDlgItem(IDC_IMAGE); //获得pictrue控件窗口的句柄   
	CRect rect, rect_windows;
	pWnd->GetClientRect(&rect);//获得pictrue控件所在的矩形区域 
	rect_windows = rect;
	ScreenToClient(&rect_windows);
	m_rImageInit = rect_windows;
	m_mBgMat = cv::Mat(rect.Height(), rect.Width(), CV_8UC3, cv::Scalar(255, 255, 255));
}


bool CMFCDemoDlg::ShowImg(const cv::Mat& src)
{
	cv::Mat resize_img;

	CWnd *pWnd = GetDlgItem(IDC_IMAGE); //获得pictrue控件窗口的句柄   
	pWnd->UpdateWindow();

	pWnd->SetWindowPos(NULL, m_rImageInit.left, m_rImageInit.top, m_rImageInit.Width(), m_rImageInit.Height(),
		SWP_NOZORDER | SWP_NOMOVE);

	CRect rect(0, 0, src.cols, src.rows);
	pWnd->GetClientRect(&rect);//获得pictrue控件所在的矩形区域   
	AdjustInputMatSizeAndRect(src, resize_img, rect);

	m_resizeImgSize.width = rect.Width();
	m_resizeImgSize.height = rect.Height();

	CImage c_mat;
	MatToCImage(resize_img, c_mat);

	CDC *pDC = pWnd->GetDC();//获得pictrue控件的DC   

	c_mat.Draw(pDC->m_hDC, rect); //将图片画到Picture控件表示的矩形区域   

	ReleaseDC(pDC);

	return true;
}

bool CMFCDemoDlg::AdjustInputMatSizeAndRect(const cv::Mat& src, cv::Mat&dst, CRect& rect)
{
	double m_rate = double(src.cols) / double(src.rows);
	double cr_rate = double(rect.Width()) / double(rect.Height());
	int width = 0, height = 0;
	if (cr_rate >= m_rate)
	{
		height = rect.Height();
		width = height*m_rate;
	}
	else
	{
		width = rect.Width();
		height = width / m_rate;
	}

	rect.bottom = rect.top + height;
	rect.right = rect.left + width;

	resize(src, dst, cv::Size(width, height), cv::INTER_LANCZOS4);

	return true;
}

void CMFCDemoDlg::MatToCImage(const cv::Mat& mat, CImage& cImage)
{
	int width = mat.cols;
	int height = mat.rows;
	int channels = mat.channels();

	cImage.Destroy();//这一步是防止重复利用造成内存问题  
	cImage.Create(width, height, 8 * channels);

	const uchar* ps;
	uchar* pimg = (uchar*)cImage.GetBits(); //获取CImage的像素存贮区的指针  
	int step = cImage.GetPitch();//每行的字节数,注意这个返回值有正有负  

	// 如果是1个通道的图像(灰度图像) DIB格式才需要对调色板设置    
	// CImage中内置了调色板，我们要对他进行赋值：  
	if (1 == channels)
	{
		RGBQUAD* ColorTable;
		int MaxColors = 256;
		//这里可以通过CI.GetMaxColorTableEntries()得到大小(如果你是CI.Load读入图像的话)    
		ColorTable = new RGBQUAD[MaxColors];
		cImage.GetColorTable(0, MaxColors, ColorTable);//这里是取得指针    
		for (int i = 0; i<MaxColors; i++)
		{
			ColorTable[i].rgbBlue = (BYTE)i;
			//BYTE和uchar一回事，但MFC中都用它    
			ColorTable[i].rgbGreen = (BYTE)i;
			ColorTable[i].rgbRed = (BYTE)i;
		}
		cImage.SetColorTable(0, MaxColors, ColorTable);
		delete[]ColorTable;
	}


	for (int i = 0; i < height; i++)
	{
		ps = mat.ptr<uchar>(i);
		for (int j = 0; j < width; j++)
		{
			if (1 == channels)
			{
				*(pimg + i*step + j) = ps[j];
				//*(pimg + i*step + j) = 105;  
			}
			else if (3 == channels)
			{
				*(pimg + i*step + j * 3) = ps[j * 3];
				*(pimg + i*step + j * 3 + 1) = ps[j * 3 + 1];
				*(pimg + i*step + j * 3 + 2) = ps[j * 3 + 2];
			}
		}
	}
	//string str = CString2StdString(_T("C:\\sample1020.bmp"));  
	//imwrite(str,mat);  
	//这句话就是用来测试cimage有没有被赋值  
	//cImage.Save(_T("C:\\sample1024.bmp"));  
	//return;
}

void CMFCDemoDlg::ShowImgFromFileIdx(int idx)
{
	if (idx >= m_vFiles.size())
		return;

	m_nCurFileIdx = idx;
	CListBox* listbox = (CListBox*)GetDlgItem(IDC_LIST_FILELIST);
	listbox->SetCurSel(m_nCurFileIdx);
	listbox->UpdateData();

	auto file = m_vFiles[idx];
	auto img = cv::imread(file);
	if (img.empty())
	{
		CString line;
		line.Format("不能读取图片： %s", file.c_str());
		MessageBox(line);
		return;
	}

	m_mOriImg = img;
	m_mMaskImg = cv::Mat::zeros(img.size(), CV_8UC1);

	ShowImg(m_mBgMat);

	Redraw();
}

void CMFCDemoDlg::Redraw()
{
	if (m_mOriImg.empty())
		return;
	
	auto img = m_mOriImg.clone();
	m_mCurImg = img;

	ShowImg(img);
}


void CMFCDemoDlg::ScreenPt2ImgPt(CPoint snPt, cv::Point& imgPt)
{
	CRect Prect1;          //定义图片的矩形

	GetDlgItem(IDC_IMAGE)->GetWindowRect(&Prect1);    //得到图片的矩//形大小
	ScreenToClient(&Prect1);   //将图片框的绝对矩形大小

	CPoint point = snPt;

	//判断是否在图片框内，不处理不在图片框内的点击
	//if (point.x<Prect1.left || point.x>Prect1.right || point.y<Prect1.top || point.y>Prect1.bottom) return;
	imgPt.x = snPt.x - Prect1.left;
	imgPt.y = snPt.y - Prect1.top;

	imgPt.x = std::max(0, std::min(imgPt.x, int(Prect1.right)));
	imgPt.y = std::max(0, std::min(imgPt.y, int(Prect1.bottom)));
}

void CMFCDemoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	m_bIsLButtonDown = true;

	if (false == m_mCurImg.empty() && m_bIsLButtonDown)
	{
		cv::Point pt;
		ScreenPt2ImgPt(point, pt);

		auto oriSize = m_mOriImg.size();
		float rx = (float)oriSize.width / (float)m_resizeImgSize.width;
		float ry = (float)oriSize.height / (float)m_resizeImgSize.height;

		pt.x *= rx;
		pt.y *= ry;

		cv::circle(m_mCurImg, pt, 5, cv::Scalar(255, 255, 255), -1);
		cv::circle(m_mMaskImg, pt, 5, cv::Scalar(255), -1);

		ShowImg(m_mCurImg);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMFCDemoDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	m_bIsLButtonDown = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMFCDemoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if (false == m_mCurImg.empty() && m_bIsLButtonDown)
	{
		cv::Point pt;
		ScreenPt2ImgPt(point, pt);

		auto oriSize = m_mOriImg.size();
		float rx = (float)oriSize.width / (float)m_resizeImgSize.width;
		float ry = (float)oriSize.height / (float)m_resizeImgSize.height;

		pt.x *= rx;
		pt.y *= ry;

		cv::circle(m_mCurImg, pt, 5, cv::Scalar(255, 255, 255), -1);
		cv::circle(m_mMaskImg, pt, 5, cv::Scalar(255), -1);

		ShowImg(m_mCurImg);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CMFCDemoDlg::OnBnClickedButtonInpaint()
{
	if (m_mCurImg.empty())
	{
		MessageBox("请先选择图片!");
		return;
	}

	Inpaint_v1();
	m_bIsResultNeedRefresh = true;

	ShowImg(m_mCurImg);
}


void CMFCDemoDlg::OnBnClickedButtonReset()
{
	ShowImgFromFileIdx(m_nCurFileIdx);
}

void CMFCDemoDlg::Inpaint_v1()
{
	ErrorCode code;
	Image inpaint,
		src((char*)m_mOriImg.data, m_mOriImg.cols, m_mOriImg.rows, m_mOriImg.channels()),
		mask((char*)m_mMaskImg.data, m_mMaskImg.cols, m_mMaskImg.rows, m_mMaskImg.channels());

	if (SYY_NO_ERROR != (code =
		ExecuteInpaint(m_hHandleInpaintPM, src, mask, inpaint)))
	{
		char msg[256];
		sprintf_s(msg, "图像修复算法失败！ 错误码为： %d", code);
		MessageBox(msg);
		return;
	}

	m_mCurImg = cv::Mat(inpaint.nHeight, inpaint.nWidth, inpaint.nChannels == 3 ? CV_8UC3 : CV_8UC1, inpaint.pData).clone();
	m_mOriImg = m_mCurImg.clone();
	m_mMaskImg = cv::Mat::zeros(m_mMaskImg.size(), m_mMaskImg.type());
}

void CMFCDemoDlg::Inpaint_v2()
{
	ErrorCode code;
	Image inpaint,
		src((char*)m_mOriImg.data, m_mOriImg.cols, m_mOriImg.rows, m_mOriImg.channels()),
		mask((char*)m_mMaskImg.data, m_mMaskImg.cols, m_mMaskImg.rows, m_mMaskImg.channels());

	if (SYY_NO_ERROR != (code =
		ExecuteInpaint(m_hHandleInpaintCriminisi, src, mask, inpaint)))
	{
		char msg[256];
		sprintf_s(msg, "图像修复算法失败！ 错误码为： %d", code);
		MessageBox(msg);
		return;
	}

	m_mCurImg = cv::Mat(inpaint.nHeight, inpaint.nWidth, inpaint.nChannels == 3 ? CV_8UC3 : CV_8UC1, inpaint.pData).clone();
	m_mOriImg = m_mCurImg.clone();
	m_mMaskImg = cv::Mat::zeros(m_mMaskImg.size(), m_mMaskImg.type());



}


void CMFCDemoDlg::OnBnClickedButtonInpaintV2()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_mCurImg.empty())
	{
		MessageBox("请先选择图片!");
		return;
	}

	Inpaint_v2();

	m_bIsResultNeedRefresh = true;

	ShowImg(m_mCurImg);
}


void CMFCDemoDlg::OnBnClickedButtonInpaintV3()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_mCurImg.empty())
	{
		MessageBox("请先选择图片!");
		return;
	}

	Inpaint_v3();

	m_bIsResultNeedRefresh = true;

	ShowImg(m_mCurImg);
}

void CMFCDemoDlg::Inpaint_v3()
{
	cv::Mat inpaintImg;

	/*-   **INPAINT_NS** Navier-Stokes based method [Navier01]
	-   **INPAINT_TELEA** Method by Alexandru Telea @cite Telea04 .*/

	cv::inpaint(m_mCurImg, m_mMaskImg, inpaintImg, 20, cv::INPAINT_TELEA);
	m_mCurImg = inpaintImg.clone();
	m_mOriImg = inpaintImg.clone();
	m_mMaskImg = cv::Mat::zeros(m_mMaskImg.size(), m_mMaskImg.type());

}

void CMFCDemoDlg::Inpaint_v4()
{
	cv::Mat inpaintImg;

/*-   **INPAINT_NS** Navier-Stokes based method [Navier01]
-   **INPAINT_TELEA** Method by Alexandru Telea @cite Telea04 .*/

	cv::inpaint(m_mCurImg, m_mMaskImg, inpaintImg, 20, cv::INPAINT_NS);
	m_mCurImg = inpaintImg.clone();
	m_mOriImg = inpaintImg.clone();
	m_mMaskImg = cv::Mat::zeros(m_mMaskImg.size(), m_mMaskImg.type());

}


void CMFCDemoDlg::OnBnClickedButtonInpaintV4()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_mCurImg.empty())
	{
		MessageBox("请先选择图片!");
		return;
	}

	Inpaint_v4();

	m_bIsResultNeedRefresh = true;

	ShowImg(m_mCurImg);
}

void CMFCDemoDlg::SaveCurImg(const std::string& save_path)
{
	if (m_mCurImg.empty())
	{
		return;
	}
	auto file = m_vFiles[m_nCurFileIdx];

	auto filename = file.substr(file.find_last_of('\\'));

	auto filepath = save_path + filename;

	auto resImg = DrawAnalysisResults();

	cv::imwrite(filepath, resImg);
}

void CMFCDemoDlg::OnBnClickedButtonSave()
{
	// TODO:  在此添加控件通知处理程序代码
	char szPath[MAX_PATH];     //存放选择的目录路径

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = "请选择需要打开的目录：";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(lp, szPath);

	CString folderRootPath = szPath;

	SaveCurImg(folderRootPath.GetString());
}


void CMFCDemoDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


//void CMFCDemoDlg::OnThemechangedScrollbarThreshold(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// 该功能要求使用 Windows XP 或更高版本。
//	// 符号 _WIN32_WINNT 必须 >= 0x0501。
//	// TODO:  在此添加控件通知处理程序代码
//	*pResult = 0;
//}


//void CMFCDemoDlg::OnThumbposchangingSliderThreshold(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// 此功能要求 Windows Vista 或更高版本。
//	// _WIN32_WINNT 符号必须 >= 0x0600。
//	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
//	// TODO:  在此添加控件通知处理程序代码
//	*pResult = 0;
//}


void CMFCDemoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CSliderCtrl* sliderCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_THRESHOLD);
	int value = sliderCtrl->GetPos();

	value = std::max(0, value);
	value = std::min(value, 100);

	float fValue = (float)value / 100.f;

	SetAnalysisThreshold(fValue);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMFCDemoDlg::SetAnalysisThreshold(float thres)
{
	m_fAnalysisThresHold = thres;

	CString text;
	text.Format("%1.3f", m_fAnalysisThresHold);
	GetDlgItem(IDC_TEXT_THRESHOLD)->SetWindowTextA(text);

	if (m_mCurImg.empty())
	{

		return;
	}

	DrawAnalysisResults();
}

cv::Mat CMFCDemoDlg::DrawAnalysisResults()
{
	if (m_mCurImg.empty())
	{
		return m_mCurImg;
	}

	auto img = m_mCurImg.clone();

	if (false == m_bIsResultNeedRefresh)
	{
		auto result = m_BUresult;

		cv::Rect cropRect(result.rCropRect.x, result.rCropRect.y, result.rCropRect.w, result.rCropRect.h);

		cv::rectangle(img, cropRect, cv::Scalar(255, 255, 255), 1);
		auto text = result.nGrading == 0 ? "1a" : "other";
		cv::putText(img, text, cropRect.tl(), 1, 1, cv::Scalar(255, 255, 255));

		std::stringstream ss;

		for (int i = 0; i < result.nLessionsCount; i++)
		{
			if (result.pLessionConfidence[i] < m_fAnalysisThresHold)
				continue;

			auto r = result.pLessionRects[i];
			cv::Rect rect(r.x, r.y, r.w, r.h);
			cv::rectangle(img, rect, cv::Scalar(0, 0, 255), 2);
			ss.str("");
			ss.precision(3);
			ss << result.pLessionConfidence[i];
			cv::putText(img, ss.str(), (rect.br() + rect.tl()) / 2, 1, 2, cv::Scalar(255, 255, 255), 1);
		}
	}

	ShowImg(img);

	return img;
}


void CMFCDemoDlg::OnBnClickedButtonAnalysis2()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_mCurImg.empty())
	{
		MessageBox("请先选择图片!");
		return;
	}

	auto img = m_mCurImg;

	auto& result = m_BUresult;

	ErrorCode code;
	bool bIsCrop = true;
	if (SYY_NO_ERROR != (code =
		ExecuteBUAnalysis(m_hHandleBUAnalysis2, (char*)img.data, img.cols, img.rows, &result)))
	{
		char msg[256];
		sprintf_s(msg, "b超分析算法失败！ 错误码为： %d", code);
		MessageBox(msg);
		return;
	}

	m_bIsResultNeedRefresh = false;

	DrawAnalysisResults();
}


void CMFCDemoDlg::OnBnClickedButtonAnalysis3()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_mCurImg.empty())
	{
		MessageBox("请先选择图片!");
		return;
	}

	auto img = m_mCurImg;

	auto& result = m_BUresult;

	ErrorCode code;
	bool bIsCrop = true;
	if (SYY_NO_ERROR != (code =
		ExecuteBUAnalysis(m_hHandleBUAnalysis3, (char*)img.data, img.cols, img.rows, &result)))
	{
		char msg[256];
		sprintf_s(msg, "b超分析算法失败！ 错误码为： %d", code);
		MessageBox(msg);
		return;
	}

	m_bIsResultNeedRefresh = false;

	DrawAnalysisResults();
}

void CMFCDemoDlg::OnBnClickedButtonAnalysis4()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_mCurImg.empty())
	{
		MessageBox("请先选择图片!");
		return;
	}

	auto img = m_mCurImg;

	auto& result = m_BUresult;

	ErrorCode code;
	bool bIsCrop = true;
	if (SYY_NO_ERROR != (code =
		ExecuteBUAnalysis(m_hHandleBUAnalysis4, (char*)img.data, img.cols, img.rows, &result)))
	{
		char msg[256];
		sprintf_s(msg, "b超分析算法失败！ 错误码为： %d", code);
		MessageBox(msg);
		return;
	}

	m_bIsResultNeedRefresh = false;

	DrawAnalysisResults();
}
