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
#include "LABELTOOLS.h"

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

	//1，指针COSGObject
	//2,指针文件名
	//3,地标数量
	static void ReadLabelThread(void* ptr);
	static void CreateLabelThread(void* ptr);
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

private:
	
	osg::ref_ptr<osg::Image> chinaImage ;
	osg::ref_ptr<osg::Image> CityCenterImage ;
	osg::ref_ptr<osg::Image> CityImage ;
	osg::ref_ptr<osg::Image> CountryCityImage ;
	osg::ref_ptr<osg::Image> CountryImage ;
	osg::ref_ptr<osg::Image> TownImage;
	osg::ref_ptr<osg::Image> VIImage;
	std::string strChinaICON ;
	std::string strCityCenterICON ;
	std::string strCityICON;
	std::string strCountryCityICON ;
	std::string strCountryICON;
	std::string strTownICON;
	std::string strVIICON ;
	//文字
	std::wstring wstrChinaTxt ;
	std::wstring wstrCityCenterTxt ;
	std::wstring wstrCityTxt ;
	std::wstring wstrCountryCityTxt ;
	std::wstring wstrCountryTxt ;
	std::wstring wstrTownTxt ;
	std::wstring wstrVITxt ;

	osgEarth::Style theStyle;
	
	unsigned int shaanxiParam[3];
	std::string strShaanxiText; 

public:
	unsigned int _processI;
	
};

