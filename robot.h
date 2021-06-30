#ifndef ROBOT_H
#define ROBOT_H
#include"parameter.h"
#include"graph.h"
class Robot{
    //QPointF currentPoint;
    int currentVertex;
    action actionType;
    type typeRobot;
    int timeOut;
    int counter;// counter for timeOut
    double Ps; // вероятность выхода из строя
    int currentResources;
    int maxResources;
    //int T;//время работы с номинальной производительностью
    int Z;//время восстановления ресурса
    bool needSupport;
	bool turn;
public:
	QVector<int> nextPoints;
	Graph graph;
    int P;//производительность
	int V;//скорость
    Robot(type t, int p,Graph g);
    type getType();
    //QPointF getCurrentPoint();
	int getCurrentVertex();
    void setNextPoint(QVector<int> lP,action act);
    void setAction(action ac);
    action getAction();
    bool errorRunning();
    int getCurrentResourses();
    void support();
    void extinguish();
    void move();
    bool update();// return true if movingToFire -> extinguishing
    bool isNeedSupport();
	bool isOnBlockWay();
	int getGoal();
	int getTimeOut();
	void subResource();
	int getAddTime();
	void updateGraph();
};
#endif // ROBOT_H
