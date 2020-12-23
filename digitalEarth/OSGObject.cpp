#include "StdAfx.h"
#include "OSGObject.h"


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
}
void COSGObject::InitSceneGraph()
{
	mRoot = new osg::Group;
	//std::string strFileName = "E:\\tutorial\\osg\\000.ISO\\35\\builder\\data\\glider.osg";
	std::string strFileName = "E:\\tutorial\\osg\\000.ISO\\35\\builder\\data\\heightfield\\tt.ive";
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(strFileName);
	mRoot->addChild(node);
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
	mViewer->setCameraManipulator(new osgGA::TrackballManipulator );
	mViewer->setSceneData(mRoot);
	mViewer->realize();

}
void COSGObject::PreFrameUpdate()
{
}
void COSGObject::PostFrameUpdate()
{
}

//äÖÈ¾Ïß³Ì
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