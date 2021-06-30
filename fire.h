#ifndef FIRE_H
#define FIRE_H
#include"parameter.h"
#include"robot.h"

class Fire{
    int currentPoint;
    QList<double> Ilist;
    int type; // 0-Затухающий, 1-постоянный, 2-Развивающийся
    int counterChangedType; // через counterChangedType тактов менять type
    
public:
	QList<Robot> Rlist;
	void removeRobot(Robot r);
    Fire(int p);
	Fire();
	int getIn();
    void update();
	bool isNeedMoreRobot(int time);
};

#endif // FIRE_H
