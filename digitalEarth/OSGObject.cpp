#include "StdAfx.h"
#include "OSGObject.h"
#include "digitalEarth.h"

COSGObject::COSGObject(HWND hWnd)
{
	m_hWnd = hWnd;
}


COSGObject::~COSGObject(void)
{
}

void COSGObject::InitOSG()
{
	this->InitSceneGraph();
	this->InitCameraConfig();
	this->InitOSGEarth();
}
void COSGObject::InitSceneGraph()
{
	mRoot = new osg::Group;
	//std::string strFileName = "E:\\tutorial\\osg\\000.ISO\\35\\builder\\data\\glider.osg";
	//std::string strFileName = "E:\\tutorial\\osg\\000.ISO\\35\\builder\\data\\heightfield\\tt.ive";
	std::string strFileName = "E:\\tutorial\\osg\\000.ISO\\35\\builder\\earthfile\\china-simple7.earth";
	//std::string strFileName = "E:\\test\\osg\\earthFile\\china-simple6.earth";
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(strFileName);
	mRoot->addChild(node);
	theMapNode = dynamic_cast<osgEarth::MapNode*> ( node.get() );
}
void COSGObject::InitCameraConfig()
{
	RECT rect;
	mViewer = new osgViewer::Viewer;
	::GetWindowRect(m_hWnd, &rect );
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);
	traits->x = 0;
	traits->y = 0;
	traits->width = rect.right - rect.left;
	traits->height = rect.bottom - rect.top;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->setInheritedWindowPixelFormat = true;
	traits->inheritedWindowData = windata;
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits);

	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setGraphicsContext(gc);
	camera->setViewport(traits->x, traits->y, traits->width, traits->height );
	camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double> (traits->width) / static_cast<double> (traits->height), 1.0, 1000.0 );
	camera->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES );
	camera->setNearFarRatio(0.000003f);

	mViewer->setCamera(camera);
	//mViewer->setCameraManipulator(new osgGA::TrackballManipulator );
	mViewer->setSceneData(mRoot);
	mViewer->realize();

}
void COSGObject::PreFrameUpdate()
{
	while(theApp._bNeedModify)
	{
		Sleep(1);
	}
	theApp._bCanModify = false;
}
void COSGObject::PostFrameUpdate()
{
	if(theApp._bNeedModify)
	{
		theApp._bCanModify = true;
	}
}

//渲染线程
void COSGObject::Render(void* ptr)
{
	COSGObject * osgObj = (COSGObject* ) ptr;
	osgViewer::Viewer* viewer = osgObj->getViewer();
	while(!viewer->done())
	{
		osgObj->PreFrameUpdate();
		viewer->frame();
		osgObj->PostFrameUpdate();
	}
	_endthread();

}

osgViewer::Viewer* COSGObject::getViewer()
{
	return mViewer;
}

void COSGObject::InitOSGEarth()
{
	//初始化操作器
	em = new osgEarth::Util::EarthManipulator;
	em->getSettings()->setArcViewpointTransitions(true);
	em->setNode(theMapNode);
	mViewer->setCameraManipulator(em );
	//初始化天空
	osgEarth::Config skyConfig;
	double hours = skyConfig.value("hours", 12.0);
	osg::ref_ptr< osgEarth::Util::SkyNode> sky_node = new osgEarth::Util::SkyNode(theMapNode->getMap());
	sky_node->setDateTime(2020,12,14,hours);
	sky_node->attach(mViewer, 1 );
	sky_node->setAmbientBrightness(1.0, mViewer);
	mRoot->addChild(sky_node);

	//获取国界线图层
	china_boundary = theMapNode->getMap()->getImageLayerByName("china_boundaries");
	china_boundary->setOpacity(1.0);
}

void COSGObject::setChinaBoundaryOpacity(double opt)
{
	if(china_boundary)
	{
		china_boundary->setOpacity(opt);
	}
}

double COSGObject::getChinaBoundaryOpacity()
{
	if(china_boundary)
	{
		return china_boundary->getOpacity();
	}
	else
	{
		return -1;
	}

}

//移除国界线层
void COSGObject::removeChinaBoundary()
{
	theApp._bNeedModify = true;
	while(!theApp._bCanModify)
	{
		Sleep(1);
	}
	if(china_boundary)
	{
		theMapNode->getMap()->removeImageLayer(china_boundary);
	}
	theApp._bNeedModify = false;
}
//加上国界线层
void COSGObject::addChinaBoundary()
{
	theApp._bNeedModify = true;
	while(!theApp._bCanModify)
	{
		Sleep(1);
	}
	if(china_boundary)
	{
		theMapNode->getMap()->addImageLayer(china_boundary);
	}
	theApp._bNeedModify = false;
}