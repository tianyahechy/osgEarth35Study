// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// digitalEarthView.cpp : CdigitalEarthView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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

// CdigitalEarthView 构造/析构

CdigitalEarthView::CdigitalEarthView()
{
	// TODO: 在此处添加构造代码
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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CdigitalEarthView 绘制

void CdigitalEarthView::OnDraw(CDC* /*pDC*/)
{
	CdigitalEarthDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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


// CdigitalEarthView 诊断

#ifdef _DEBUG
void CdigitalEarthView::AssertValid() const
{
	CView::AssertValid();
}

void CdigitalEarthView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdigitalEarthDoc* CdigitalEarthView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdigitalEarthDoc)));
	return (CdigitalEarthDoc*)m_pDocument;
}
#endif //_DEBUG


// CdigitalEarthView 消息处理程序


int CdigitalEarthView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	mOSG = new COSGObject(m_hWnd);
	return 0;
}


BOOL CdigitalEarthView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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

	// TODO: 在此处添加消息处理程序代码
	if(mOSG != 0 )
	{
		delete mOSG;
	}
	WaitForSingleObject(mThreadHandle, 1000 );

}


void CdigitalEarthView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	mOSG->InitOSG();
	mThreadHandle = (HANDLE) _beginthread(&COSGObject::Render, 0, mOSG );
}


void CdigitalEarthView::Onszgjx()
{
	// TODO: 在此添加命令处理程序代码
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
		MessageBox("错误", "透明度必须为数字且为浮点值", MB_OK | MB_ICONERROR);
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
	
	// TODO: 在此添加命令处理程序代码
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
	// TODO: 在此添加命令处理程序代码

}


void CdigitalEarthView::OnUpdatexianshi(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(_bShowChinaBoundary);
}


void CdigitalEarthView::Ondbycl()
{
	// TODO: 在此添加命令处理程序代码
	if(!ct)
	{
		ct = new CLABELTOOLS(0, mOSG);
	}
	ct->DoModal();
}
