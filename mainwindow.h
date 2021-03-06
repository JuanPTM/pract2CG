#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QGraphicsPixmapItem>

#include <osgview.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>


namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void computeOSG();

private:
	void initCamera();
	void init3D();
	void addBox(osg::Vec3 pos, float x, float y, float z, osg::Vec4 color);
	osg::ref_ptr<osg::LightSource> addLight(int lightnum, osg::Vec4 pos, osg::Vec4 specular, osg::Vec4 diffuse, osg::Vec4 ambient);
	osg::PositionAttitudeTransform* getBox(osg::Vec3 pos, float x, float y, float z, osg::Vec4 color);
	void updateImage();
	void updateImageInWindow();
	void loadMesh();

private:
	QTimer timerOSG;
	Ui::MainWindow *ui;
	QGraphicsScene *scene;

	osg::Matrix mat;
	float rx=0;
	osg::PositionAttitudeTransform *smt,*pat_bbRotatelight;
	OsgView *osgw;
	osg::PositionAttitudeTransform *pats[4];
	osg::Geode * geodes[4];

};

#endif // MAINWINDOW_H
