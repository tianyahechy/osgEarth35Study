// LABELTOOLS.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "digitalEarth.h"
#include "LABELTOOLS.h"
#include "afxdialogex.h"

#include "OSGObject.h"



// CLABELTOOLS �Ի���

IMPLEMENT_DYNAMIC(CLABELTOOLS, CDialogEx)

CLABELTOOLS::CLABELTOOLS(CWnd* pParent /*=NULL*/,COSGObject * cosg)
	: CDialogEx(CLABELTOOLS::IDD, pParent)
	, _cosg(cosg)
{
}

CLABELTOOLS::~CLABELTOOLS()
{
}

void CLABELTOOLS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLABELTOOLS, CDialogEx)
	ON_BN_CLICKED(IDC_LABEL_HANDLE, &CLABELTOOLS::OnBnClickedLabelHandle)
	ON_BN_CLICKED(IDC_LABEL_INPUT, &CLABELTOOLS::OnBnClickedLabelInput)
	ON_BN_CLICKED(IDC_LABEL_BUTTON, &CLABELTOOLS::OnBnClickedLabelButton)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CLABELTOOLS::OnNMCustomdrawProgress1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLABELTOOLS ��Ϣ�������


void CLABELTOOLS::OnBnClickedLabelHandle()
{
	_count = 37937;
	iParam[0] = (unsigned int) _cosg;
	iParam[1] = (unsigned int)& _inputLabelPath;
	iParam[2] = _count;
	iParam[3] = (unsigned int)& _outputLabelPath;

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	(HANDLE)_beginthread(&COSGObject::CreateLabelThread, 0, (void*) iParam);
	SetTimer(0, 1000, 0);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetRange(0, _count);
}

void CLABELTOOLS::OnBnClickedLabelInput()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�����Ի���ѡ��ر��ļ�
	static char szFilter[] = "LabelFile (*.txt) | *.txt|";
	CFileDialog fd(TRUE, "txt", "", 0, szFilter, 0, 0, 1);
	if(fd.DoModal() == IDOK)
	{
		GetDlgItem(IDC_TXT_LABEL_INPUT)->SetWindowTextA(fd.GetPathName());
		_inputLabelPath =fd.GetPathName();
	}
}


void CLABELTOOLS::OnBnClickedLabelButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	static char szFilter[] = "LabelFile (*.ive) | *.ive|";
	CFileDialog fd(TRUE, "ive", "", 0, szFilter, 0, 0, 1);
	if(fd.DoModal() == IDOK)
	{
		GetDlgItem(IDC_TXT_LABEL_OUTPUT)->SetWindowTextA(fd.GetPathName());
		_outputLabelPath =fd.GetPathName();
	}
}


void CLABELTOOLS::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CLABELTOOLS::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent == 0 )
	{
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetPos(_cosg->_processI);
	}

	CDialogEx::OnTimer(nIDEvent);
}
