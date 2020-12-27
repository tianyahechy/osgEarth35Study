#include "StdAfx.h"
#include "OSGObject.h"
#include "digitalEarth.h"


static std::wstring s2ws(const std::string &s)
{
    size_t i;
    std::string curLocale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "chs");
    const char* _source = s.c_str();
    size_t _dsize = s.size() + 1;
    wchar_t* _dest = new wchar_t[_dsize];
    wmemset(_dest, 0x0, _dsize);
    mbstowcs_s(&i, _dest, _dsize, _source, _dsize);
    std::wstring result = _dest;
    delete[] _dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}

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
	std::string strFileName = "E:\\tutorial\\osg\\000.ISO\\35\\builder\\earthfile\\china-simple.earth";
	//std::string strFileName = "E:\\test\\osg\\earthFile\\china-simple6.earth";
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(strFileName);
	mRoot->addChild(node);
	theMapNode = dynamic_cast<osgEarth::MapNode*> ( node.get() );
	//地标初始化
	_earthLabel = new osg::Group;
	mRoot->addChild(_earthLabel);
	this->addLabel();
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

//新增地标
void COSGObject::addLabel()
{
	
	osgEarth::Symbology::TextSymbol* textStyle = theStyle.getOrCreateSymbol<osgEarth::Symbology::TextSymbol> ();
	//设置颜色
	textStyle->fill()->color() = osg::Vec4f(1.0, 1.0, 1.0, 0.5 );
	//设置边框
	textStyle->halo()->color() = osg::Vec4f(0.0, 0.0, 0.0, 1.0 );
	textStyle->font() = "simsun.ttc";
	textStyle->size() = 20.0;

	//图标
	strChinaICON = "E:\\tutorial\\osg\\000.ISO\\35\\builder\\data\\image\\label\\chinaIcon.jpg";
	strCityCenterICON = "E:\\tutorial\\osg\\000.ISO\\35\\builder\\data\\image\\label\\icon25.png";
	strCityICON = "E:\\tutorial\\osg\\000.ISO\\35\\builder\\data\\image\\label\\icon26.png";
	strCountryCityICON = "E:\\tutorial\\osg\\000.ISO\\35\\builder\\data\\image\\label\\icon25.png";
	strCountryICON = "E:\\tutorial\\osg\\000.ISO\\35\\builder\\data\\image\\label\\icon25.png";
	strTownICON = "E:\\tutorial\\osg\\000.ISO\\35\\builder\\data\\image\\label\\icon10.png";
	strVIICON = "E:\\tutorial\\osg\\000.ISO\\35\\builder\\data\\image\\label\\icon4.png";
	chinaImage = osgDB::readImageFile(strChinaICON);
	CityCenterImage = osgDB::readImageFile(strCityCenterICON);
	CityImage = osgDB::readImageFile(strCityICON);
	CountryCityImage = osgDB::readImageFile(strCountryCityICON);
	CountryImage = osgDB::readImageFile(strCountryICON);
	TownImage = osgDB::readImageFile(strTownICON);
	VIImage = osgDB::readImageFile(strVIICON);
	
	//文字
	wstrChinaTxt = L"中国";
	wstrCityCenterTxt = L"省会";
	wstrCityTxt = L"市";
	wstrCountryCityTxt = L"区";
	wstrCountryTxt = L"县";
	wstrTownTxt = L"镇";
	wstrVITxt = L"村";
	
	osg::Vec3d posChina = osg::Vec3d(110, 34, 0 );
	osg::ref_ptr<osgEarth::Annotation::PlaceNode> pn = new osgEarth::Annotation::PlaceNode(theMapNode, posChina, chinaImage, wstrChinaTxt,theStyle );
	_earthLabel->addChild(pn);

	//添加陕西地名
	strShaanxiText = "E:\\tutorial\\osg\\000.ISO\\35\\builder\\data\\label\\txt\\shaanxi.txt";
	shaanxiParam[0] = (unsigned int) this;
	shaanxiParam[1] = (unsigned int)& strShaanxiText;
	shaanxiParam[2] = 37937;
	(HANDLE)_beginthread(&COSGObject::ReadLabelThread, 0, (void*) shaanxiParam);
	


}
void COSGObject::ReadLabelThread(void* ptr)
{
	unsigned int * tempArray = (unsigned int*) ptr;
	//参数0
	COSGObject* cOsg = (COSGObject*) tempArray[0];
	//参数1
	std::string* fileFullPath = (std::string*) tempArray[1];
	//参数2
	unsigned int count = tempArray[2];

	std::fstream f( fileFullPath->c_str(), std::ios::in);
	char name[128];
	wchar_t wname[128];
	char area[256];
	int level;
	float lon;
	float lat;
	
	osg::ref_ptr<osg::Image> imageUse = 0;
	
	std::wstring wstrTxtUse;
	
	for(int i = 0; i < count;i = i+100)
	{
		int j = 0;
		osg::ref_ptr<osg::Group> gp = new osg::Group;
		while(j<100 && i < count)
		{
			j++;
			i++;
			f>>name>>area>>level>>lon>>lat;
	
			osg::Vec3d center;
			cOsg->theMapNode->getMap()->mapPointToWorldPoint(osg::Vec3(lon, lat, 0), center );
			osg::ref_ptr<osg::LOD> lod = new osg::LOD;
			lod->setCenterMode(osg::LOD::USER_DEFINED_CENTER);
			lod->setCenter(center);
		
			long dist;
			switch( level )
			{
			case 16:
				{
					dist = 500000;
					imageUse = cOsg->CityCenterImage;
					wstrTxtUse = cOsg->wstrCityCenterTxt;
				}
				break;

			case 64:
				{
					dist = 100000;
					imageUse = cOsg->CityImage;
					wstrTxtUse = cOsg->wstrCityTxt;
				}
				break;

			case 256:
				{
					dist = 50000;
					imageUse = cOsg->CountryCityImage;
					wstrTxtUse = cOsg->wstrCountryCityTxt;
				}
				break;

			case 512:
				{
					dist = 25000;
					imageUse = cOsg->CountryImage;
					wstrTxtUse = cOsg->wstrCountryTxt;
				
				}
				break;

			case 1024:
				{
					dist = 12000;
					imageUse = cOsg->TownImage;
					wstrTxtUse = cOsg->wstrTownTxt;
				}
				break;
			case 4096:
				{
					dist = 6000;
					imageUse = cOsg->VIImage;
					wstrTxtUse = cOsg->wstrVITxt;
				}
				break;
			
			default:
				{
					dist = 10;
					imageUse = cOsg->CityCenterImage;
					wstrTxtUse = cOsg->wstrCityCenterTxt;
				}
				break;
			}
			//将地名取出，转换成宽字符
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, name, 128, wname, 128 );
			osg::ref_ptr<osgEarth::Annotation::PlaceNode> pn2 = new osgEarth::Annotation::PlaceNode(cOsg->theMapNode,osg::Vec3d(lon, lat, 0) , imageUse, wstrTxtUse, cOsg->theStyle );
			lod->addChild(pn2, 0, dist);
			gp->addChild(lod);
		}


		theApp._bNeedModify = true;
		while(!theApp._bCanModify)
		{
			Sleep(1);
		}
		cOsg->_earthLabel->addChild(gp);
		theApp._bNeedModify = false;
		Sleep(10);
	}
	f.close();
	_endthread();
}