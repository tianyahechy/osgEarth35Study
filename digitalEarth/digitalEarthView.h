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

// digitalEarthView.h : CdigitalEarthView ��Ľӿ�
//

#pragma once
#include "digitalEarthDoc.h"
#include "OSGObject.h"

class CdigitalEarthView : public CView
{
protected: // �������л�����
	CdigitalEarthView();
	DECLARE_DYNCREATE(CdigitalEarthView)

// ����
public:
	CdigitalEarthDoc* GetDocument() const;

public:
	COSGObject* mOSG;
	HANDLE mThreadHandle;
	double mChinaBoundariesOpt;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CdigitalEarthView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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
	//�Ƿ���ʾ������
	bool _bShowChinaBoundary;
	afx_msg void OnUpdatexianshi(CCmdUI *pCmdUI);
	
};

#ifndef _DEBUG  // digitalEarthView.cpp �еĵ��԰汾
inline CdigitalEarthDoc* CdigitalEarthView::GetDocument() const
   { return reinterpret_cast<CdigitalEarthDoc*>(m_pDocument); }
#endif

