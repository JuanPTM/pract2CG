#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	/// osgview
	osgw = new OsgView(ui->widget);
	/// manipulator
        osg::Vec3 eye(osg::Vec3d(0,-25,0));
        osg::Vec3 front(osg::Vec3d(0,0,0));
        osg::Vec3 up(osg::Vec3d(0,0,10));
        osgGA::TrackballManipulator *man = new osgGA::TrackballManipulator;
        man->setHomePosition(eye,front,up,false);
        osgw->setCameraManipulator(man,true);

	init3D();

	timerOSG.start(20);
	connect(&timerOSG, SIGNAL(timeout()), this, SLOT(computeOSG()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initCamera()
{
	scene = new QGraphicsScene;
}

void MainWindow::addBox(osg::Vec3 pos, float x, float y, float z, osg::Vec4 color)
{
	osg::Box *box = new osg::Box( pos, x, y, z);
	osg::ShapeDrawable *boxDrawable = new osg::ShapeDrawable(box);
	boxDrawable->setColor(color);
	osg::Geode* shapeGeode = new osg::Geode();
	shapeGeode->addDrawable(boxDrawable);
	auto pat = new osg::PositionAttitudeTransform;
	pat->addChild(shapeGeode);
	osgw->getRootGroup()->addChild(pat);
}

void MainWindow::init3D()
{
 loadMesh();

addBox(osg::Vec3(10, 1, 1), 5, 5, 5,osg::Vec4(1,0,0,1));
addBox(osg::Vec3(1, 10, 1), 5, 5, 5,osg::Vec4(0,1,0,1));
addBox(osg::Vec3(1, 1, 10), 5, 5, 5,osg::Vec4(0,0,1,1));


}

osg::PositionAttitudeTransform* MainWindow::getBox(osg::Vec3 pos, float x, float y, float z, osg::Vec4 color)
{
	osg::Box *box = new osg::Box( pos, x, y, z);
	osg::ShapeDrawable *boxDrawable = new osg::ShapeDrawable(box);
	boxDrawable->setColor(color);
	osg::Geode* shapeGeode = new osg::Geode();
	shapeGeode->addDrawable(boxDrawable);
	osg::PositionAttitudeTransform *pat = new osg::PositionAttitudeTransform;
	pat->addChild(shapeGeode);
	return pat;
}

void MainWindow::loadMesh()
{
	osg::Node *osgMesh = osgDB::readNodeFile("../gualzru.osg");
	if (osgMesh==NULL)
	{
	      printf("Could not open osg file.\n");
	}
	else
	{
	     float sc = 0.05; float rx = 0.3;
	     smt = new osg::PositionAttitudeTransform;
	     smt->setPosition(osg::Vec3(0.0f,-10.0f,0.0f));
	     smt->setScale(osg::Vec3(sc,sc,sc));
	     smt->setAttitude(osg::Quat(rx,osg::Vec3(1,1,1)));
	     //smt->setMatrix(osg::Matrix::scale(sc,sc,sc) * osg::Matrix::rotate(rx, osg::Vec3(1,1,1))*osg::Matrix::translate(0.0f,-10.0f,0.0f));
	     osgw->getRootGroup()->addChild(smt);
	     smt->addChild(osgMesh);

	     osg::PositionAttitudeTransform *pat_bb = new osg::PositionAttitudeTransform;
	     pat_bb->setPosition(osg::Vec3(0.0f/sc,0.0f/sc,5.0f/sc));


			osg::Box *box = new osg::Box( osg::Vec3(0,0,0), 2/sc, 0.5/sc, 2/sc);
			osg::ShapeDrawable *boxDrawable = new osg::ShapeDrawable(box);
			boxDrawable->setColor(osg::Vec4(1,0,1,1));

			osg::Billboard *bb = new osg::Billboard();
			bb->setMode(osg::Billboard::POINT_ROT_EYE);
			bb->addDrawable(boxDrawable);

			pat_bb->addChild(bb);
			smt->addChild(pat_bb);


			osg::ref_ptr<osg::LightSource> lightSource1 = addLight(0, osg::Vec4(  0.,  10.,  40.,  1.0), osg::Vec4(  1., 1., 1.,  1.0),osg::Vec4(  1., 1., 1.,  1.0),osg::Vec4(  0, 0, 0,  1.0));

			osg::PositionAttitudeTransform *patAux = getBox(osg::Vec3(0.,  10.,  40.), 1, 1, 1,osg::Vec4(1,0,0,1));
			lightSource1->addChild(patAux);

			osgw->getRootGroup()->addChild(lightSource1.get() );
			osg::StateSet *rootStateSet = osgw->getRootGroup()->getOrCreateStateSet();
			rootStateSet->setMode(GL_LIGHT0, osg::StateAttribute::ON);

			osg::ref_ptr<osg::LightSource> lightSource2 = addLight(1, osg::Vec4(  0., 30.,  0.,  1.0), osg::Vec4(  1., 0., 0.,  1.0),osg::Vec4(  1., 0, 0,  1.0),osg::Vec4(  0.1, 0, 0,  1.0));
			pat_bbRotatelight = new osg::PositionAttitudeTransform;
			pat_bbRotatelight->setPosition(osg::Vec3(0.0f,0.0f,0.0f));
			pat_bbRotatelight->addChild(lightSource2);
			osgw->getRootGroup()->addChild(pat_bbRotatelight );
			rootStateSet->setMode(GL_LIGHT1, osg::StateAttribute::ON);





	}


}


osg::ref_ptr<osg::LightSource>  MainWindow::addLight(int lightnum, osg::Vec4 pos, osg::Vec4 specular, osg::Vec4 diffuse, osg::Vec4 ambient)
{
	osg::Light *light1 = new osg::Light;
	light1->setLightNum(lightnum);
	light1->setPosition( pos);
	light1->setSpecular( specular);
	light1->setDiffuse(  diffuse);
	light1->setAmbient( ambient);
	osg::ref_ptr<osg::LightSource> lightSource1 = new osg::LightSource;
	lightSource1->setLight(light1);
	return lightSource1;



}
void MainWindow::computeOSG()
{
	rx += 0.05;
	smt->setAttitude(osg::Quat(rx,osg::Vec3(1,0,0)));
	pat_bbRotatelight->setAttitude(osg::Quat(rx,osg::Vec3(0,0,1)));
	osgw->frame();
}
