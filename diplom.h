#pragma once

#include"parameter.h"
#include"robot.h"
#include"fire.h"
#include"graph.h"
/*bool sortByVal(const QPair<int, int> &a,
	const QPair<int, int> &b)
{
	return (a.second < b.second);
}*/
namespace Ui {
	class MainWindow;
	class Robot;
	class Fire;
	class Graph;
}
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void NextTak();
private:
	//void makeLine(QGraphicsItem *item1, QGraphicsItem *item2, int type, int c);
	//void drawGraph();
	void groupSetting(QVector<int> startPoint);
	void GeneticAlgorithm(QVector<int> lRobot);
	void controlRobot(QVector<int> lRobot);
	int finessFuction(QVector<int> lRobot, QVector<int> goals, int type);
	int sumIn();
	QVector<QPair<int, int>> sortPath(int robot, QList<int> listV);
	QVector<int> getNearestSupport(QVector<int> path, Robot r);
	int timeToGoal(int nRobot, int goal);
	void update();
	void makePlan();
	void readFromFile();
	QVector<int> getMinPathFromV(int v, int typeRobot);
	void setMinPaths();
	//research

	void researchNumPoint();
	QVector<int> timeReseach;
	QVector<int> researchIn;
	QVector<int> researchRobot;
	Ui::MainWindow *ui;
	int  timeCounter;
	QTimer* timer;
	QGraphicsScene *scene;
	Graph graph;
	QVector<int> supportPoints;
	QList<Robot> Robots;
	QMap<int, Fire> Fires;
	QList<int> fires;
	QVector<QVector<int>> pathsFromRobots;
	QMap<int, QVector<int>> pathFromNextPoints;
	int counterFire;
	int fireNum;
	int rNum = 0; // число free роботов
	int type;
	int timeType1;
	bool typeChanged;
	QVector<int> lastPlan;
	int count = 15;
	
	QVector<QVector<int>> Csource;
	QVector<QVector<int>> Bsource;
	QVector<QVector<int>> minPathA;
	QVector<QVector<int>> minPathB;
	QVector<QVector<int>> minPathC;
	QVector<int> time;
	int n = 10;
};
