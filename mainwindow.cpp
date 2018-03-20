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

	timerOSG.start(2);
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
osg::Node *osgMesh = osgDB::readNodeFile("../gualzru.osg");
if (osgMesh==NULL)
{
      printf("Could not open osg file.\n");
}
else
{
     float sc = 0.05; float rx = 0.3;
     osg::MatrixTransform *smt = new osg::MatrixTransform;
     smt->setMatrix(osg::Matrix::scale(sc,sc,sc) * osg::Matrix::rotate(rx, osg::Vec3(1,1,1)));
     osgw->getRootGroup()->addChild(smt);
     smt->addChild(osgMesh);
}


osg::Box *box = new osg::Box( osg::Vec3(10, 1, 1), 10, 5, 5);
osg::ShapeDrawable *boxDrawable = new osg::ShapeDrawable(box);
boxDrawable->setColor(osg::Vec4(1,0,0,1));
osg::Geode* shapeGeode = new osg::Geode();
shapeGeode->addDrawable(boxDrawable);
auto pat = new osg::PositionAttitudeTransform;
pat->addChild(shapeGeode);
osgw->getRootGroup()->addChild(pat);


}

void MainWindow::computeOSG()
{

	osgw->frame();
}
