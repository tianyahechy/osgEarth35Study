#pragma once
#include "resource.h"
#include <string>
// CLABELTOOLS �Ի���
class COSGObject;
class CLABELTOOLS : public CDialogEx
{
	DECLARE_DYNAMIC(CLABELTOOLS)

public:
	CLABELTOOLS(CWnd* pParent = NULL,COSGObject * cosg = NULL);   // ��׼���캯��
	virtual ~CLABELTOOLS();

public:


// �Ի�������
	enum { IDD = IDD_DIALOG_TOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLabelHandle();

	void setOSG(COSGObject * cosg)
	{
		_cosg = cosg;
	}
public:
	//����0
	COSGObject * _cosg;
	//����1
	std::string _inputLabelPath;
	//����2��
	int _count;
	//����3
	std::string _outputLabelPath;

	//�������
	unsigned int iParam[4];
	afx_msg void OnBnClickedLabelInput();
	afx_msg void OnBnClickedLabelButton();
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
