#pragma once
#include <osgViewer/viewer>
#include <osgDB/ReadFile>
#include <osgViewer/api/Win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
#include <osgEarth/mapNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/SkyNode>
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

	//äÖÈ¾Ïß³Ì
	static void Render(void* ptr);

	void InitOSGEarth();

private:
	//¾ä±ú
	HWND	m_hWnd;
	osg::ref_ptr<osgViewer::Viewer> mViewer;
	osg::ref_ptr<osg::Group> mRoot;
	osg::ref_ptr<osgEarth::MapNode> theMapNode;
	osg::ref_ptr<osgEarth::Util::EarthManipulator> em;
};

