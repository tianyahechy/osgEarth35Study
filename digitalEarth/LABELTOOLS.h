#pragma once
#include "resource.h"
#include <string>
// CLABELTOOLS 对话框
class COSGObject;
class CLABELTOOLS : public CDialogEx
{
	DECLARE_DYNAMIC(CLABELTOOLS)

public:
	CLABELTOOLS(CWnd* pParent = NULL,COSGObject * cosg = NULL);   // 标准构造函数
	virtual ~CLABELTOOLS();

public:


// 对话框数据
	enum { IDD = IDD_DIALOG_TOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLabelHandle();

	void setOSG(COSGObject * cosg)
	{
		_cosg = cosg;
	}
public:
	//参数0
	COSGObject * _cosg;
	//参数1
	std::string _inputLabelPath;
	//参数2，
	int _count;
	//参数3
	std::string _outputLabelPath;

	//传入参数
	unsigned int iParam[4];
	afx_msg void OnBnClickedLabelInput();
	afx_msg void OnBnClickedLabelButton();
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
