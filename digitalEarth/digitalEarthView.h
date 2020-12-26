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

// digitalEarthView.h : CdigitalEarthView 类的接口
//

#pragma once
#include "digitalEarthDoc.h"
#include "OSGObject.h"

class CdigitalEarthView : public CView
{
protected: // 仅从序列化创建
	CdigitalEarthView();
	DECLARE_DYNCREATE(CdigitalEarthView)

// 特性
public:
	CdigitalEarthDoc* GetDocument() const;

public:
	COSGObject* mOSG;
	HANDLE mThreadHandle;
	double mChinaBoundariesOpt;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CdigitalEarthView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();
	afx_msg void OnChinaBound();
	afx_msg void OnSetChinaBound();
	afx_msg void OnShowChinabound();
	afx_msg void Onsetchinabound();
	afx_msg void Onszgjx();
	afx_msg void Onxianshi();
	afx_msg void Ontoumingdu();

public:
	//是否显示国界线
	bool _bShowChinaBoundary;
	afx_msg void OnUpdatexianshi(CCmdUI *pCmdUI);
	
};

#ifndef _DEBUG  // digitalEarthView.cpp 中的调试版本
inline CdigitalEarthDoc* CdigitalEarthView::GetDocument() const
   { return reinterpret_cast<CdigitalEarthDoc*>(m_pDocument); }
#endif

