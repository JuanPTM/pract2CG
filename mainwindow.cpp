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
}


}

void MainWindow::computeOSG()
{
	rx += 0.1;
	smt->setAttitude(osg::Quat(rx,osg::Vec3(1,0,1)));
	osgw->frame();
}
