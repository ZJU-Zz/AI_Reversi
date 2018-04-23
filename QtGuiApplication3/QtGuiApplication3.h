#pragma once
#include <qapplication.h>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMainWindow>  
#include <QGraphicsScene>  
#include <QGraphicsView>  
#include <QGraphicsItem>  
#include <QMouseEvent> 
#include <QMessageBox>  
#include "ReversiBoard.h"
#include <time.h>
#include <QTime>
#include <math.h>
#include <QThread>
#include <QMutex>
#include <QTimer>
typedef struct point Point;
struct point
{
	int x;
	int y;
};

class QtGuiApplication3 : public QMainWindow ,public QThread , public QObject

{
	Q_OBJECT


public:
	QtGuiApplication3(QWidget *parent = Q_NULLPTR);
	virtual void run();


	void mousePressEvent(QMouseEvent *);//��갴���¼�
	void keyPressEvent(QKeyEvent *event);
	void drawCrossLine();//������


	void clear();
	void addChess(int m,int n,Type type);
	void reset();
	void showView();
	void sleep(unsigned int msec);

	void temptest();
	void insertRoot();

	Point UctSearch();
	void Search(TreeNode next);
	void backUp(TreeNode next,Type win,int count);
	Point getBestChild(unsigned int total, double value[][8],unsigned int playTime[][8]);
	ReversiBoard* test;
	void endMessage();



	void startTiming();
	void endTiming();

public slots:
	void handleTimeout();  //��ʱ������  

signals:
	void viewChanged();
signals:
	void gameOver();

signals:
	void timingStart(); 
signals:
	void timingEnd();


private:
	double C = sqrt(2);
	QGraphicsScene * scene;
	QGraphicsView * view;
	QGraphicsItem * item;
	LoadWriteNodes Tree;
	qreal dx = 40;
	qreal dy = 40;
	QMutex mutex;
	int oneStepTime = 0;
	int totalTime = 0;
	QTimer *m_pTimer;
	char keyPressed;
};

//static const double C = sqrt(2);