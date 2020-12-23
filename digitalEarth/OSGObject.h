#pragma once
#include <osgViewer/viewer>
#include <osgDB/ReadFile>
#include <osgViewer/api/Win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
class COSGObject
{
public:
	COSGObject(HWND hWnd);
	~COSGObject(void);

	void InitOSG();
	void InitSceneGraph();
	void InitCameraConfig();
	void PreFrameUpdate();
	void PostFrameUpdate();

	osgViewer::Viewer* getViewer();

	//��Ⱦ�߳�
	static void Render(void* ptr);


private:
	//���
	HWND	m_hWnd;
	osg::ref_ptr<osgViewer::Viewer> mViewer;
	osg::ref_ptr<osg::Group> mRoot;
};

