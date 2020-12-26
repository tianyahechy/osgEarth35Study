#pragma once
#include <osgViewer/viewer>
#include <osgDB/ReadFile>
#include <osgViewer/api/Win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
#include <osgEarth/mapNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/SkyNode>
#include <osgEarth/ImageLayer>
#include <osgEarthAnnotation/PlaceNode>
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

	//渲染线程
	static void Render(void* ptr);

	void InitOSGEarth();

	void setChinaBoundaryOpacity(double opt);
	double getChinaBoundaryOpacity();
	
	//移除国界线层
	void removeChinaBoundary();
	//加上国界线层
	void addChinaBoundary();
	//新增地标
	void addLabel();
private:
	//句柄
	HWND	m_hWnd;
	osg::ref_ptr<osgViewer::Viewer> mViewer;
	osg::ref_ptr<osg::Group> mRoot;
	osg::ref_ptr<osgEarth::MapNode> theMapNode;
	osg::ref_ptr<osgEarth::Util::EarthManipulator> em;

	//国界线图层
	osg::ref_ptr<osgEarth::ImageLayer> china_boundary;
	//地标
	osg::ref_ptr<osg::Group> _earthLabel;
};

