// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// digitalEarthView.cpp : CdigitalEarthView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "digitalEarth.h"
#endif

#include "digitalEarthDoc.h"
#include "digitalEarthView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdigitalEarthView

IMPLEMENT_DYNCREATE(CdigitalEarthView, CView)

BEGIN_MESSAGE_MAP(CdigitalEarthView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_COMMAND(ID_szgjx, &CdigitalEarthView::Onszgjx)
	ON_COMMAND(ID_xianshi, &CdigitalEarthView::Onxianshi)
	ON_COMMAND(ID_toumingdu, &CdigitalEarthView::Ontoumingdu)
	ON_UPDATE_COMMAND_UI(ID_xianshi, &CdigitalEarthView::OnUpdatexianshi)
	ON_COMMAND(ID_dbycl, &CdigitalEarthView::Ondbycl)
END_MESSAGE_MAP()

// CdigitalEarthView ����/����

CdigitalEarthView::CdigitalEarthView()
{
	// TODO: �ڴ˴���ӹ������
	mOSG = 0;
	mThreadHandle = 0;
	_bShowChinaBoundary = true;
	mChinaBoundariesOpt = 1.0;
	ct = 0;
}

CdigitalEarthView::~CdigitalEarthView()
{
}

BOOL CdigitalEarthView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CdigitalEarthView ����

void CdigitalEarthView::OnDraw(CDC* /*pDC*/)
{
	CdigitalEarthDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

void CdigitalEarthView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CdigitalEarthView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CdigitalEarthView ���

#ifdef _DEBUG
void CdigitalEarthView::AssertValid() const
{
	CView::AssertValid();
}

void CdigitalEarthView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdigitalEarthDoc* CdigitalEarthView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdigitalEarthDoc)));
	return (CdigitalEarthDoc*)m_pDocument;
}
#endif //_DEBUG


// CdigitalEarthView ��Ϣ�������


int CdigitalEarthView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	mOSG = new COSGObject(m_hWnd);
	return 0;
}


BOOL CdigitalEarthView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if( 0 == mOSG )
	{
		return CView::OnEraseBkgnd(pDC);
	}
	else
	{
		return FALSE;
	}
	return CView::OnEraseBkgnd(pDC);
}


void CdigitalEarthView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if(mOSG != 0 )
	{
		delete mOSG;
	}
	WaitForSingleObject(mThreadHandle, 1000 );

}


void CdigitalEarthView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	mOSG->InitOSG();
	mThreadHandle = (HANDLE) _beginthread(&COSGObject::Render, 0, mOSG );
}


void CdigitalEarthView::Onszgjx()
{
	// TODO: �ڴ���������������
	CdigitalEarthApp* pApp = ( CdigitalEarthApp*) AfxGetApp();
	CMainFrame* pWnd = ( CMainFrame*) pApp->GetMainWnd();
	CMFCRibbonEdit* editt = dynamic_cast<CMFCRibbonEdit*> (pWnd->GetRibbonBar()->FindByID(ID_toumingdu));
	if(!editt)
	{
		return;
	}
	
	CString cstr = editt->GetEditText();
	std::string str(cstr.GetBuffer());
	double opt = std::atof(str.c_str());
	if(opt < 0)
	{
		MessageBox("����", "͸���ȱ���Ϊ������Ϊ����ֵ", MB_OK | MB_ICONERROR);
		cstr.Format("%f", mChinaBoundariesOpt);
		editt->SetEditText(cstr);
	}
	else
	{
		mChinaBoundariesOpt = opt;
		mOSG->setChinaBoundaryOpacity(opt);
	}
	
}


void CdigitalEarthView::Onxianshi()
{
	theApp._bNeedModify = TRUE;
	while(!theApp._bCanModify)
	{
		Sleep(1);
	}
	
	// TODO: �ڴ���������������
	_bShowChinaBoundary = !_bShowChinaBoundary;
	if(_bShowChinaBoundary)
	{
		mOSG->removeChinaBoundary();
	}
	else
	{
		mOSG->addChinaBoundary();
	}
	theApp._bNeedModify = false;
}


void CdigitalEarthView::Ontoumingdu()
{
	// TODO: �ڴ���������������

}


void CdigitalEarthView::OnUpdatexianshi(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(_bShowChinaBoundary);
}


void CdigitalEarthView::Ondbycl()
{
	// TODO: �ڴ���������������
	if(!ct)
	{
		ct = new CLABELTOOLS(0, mOSG);
	}
	ct->DoModal();
}
