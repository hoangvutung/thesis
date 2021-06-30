#ifndef PARAMETER_H
#define PARAMETER_H
#include<iostream>
#include <QMainWindow>
#include<QGraphicsEllipseItem>
#include<QGraphicsRectItem>
#include<QBrush>
#include<QColor>
#include<QGraphicsScene>
#include<QList>
#include<QGraphicsItem>
#include<QPointF>
#include<QtMath>
#include<QVector>
#include<QTime>
#include <stdlib.h>
#include<time.h>
#include<QLineF>
#include<QHBoxLayout>
#include<QTextStream>
#include<QFile>
#include<QPushButton>
#include<QSet>
#include<QMap>
#include <algorithm>
#include<QPair>
#include<QTimer>
#include<QStringList>
#include<QLatin1Char>
#define numV 504
#define notWay 10000
#define numSupport 2
#define numRobotA 4
#define numRobotB 3
#define numRobotC 3

#define Va 200
#define Vb 170
#define Vc 150

#define Pa 5
#define Pb 7
#define Pc 9

#define Za 3
#define Zb 4
#define Zc 5

#define Maxa 80
#define Maxb 100
#define Maxc 120

enum type{A=1,B,C};
enum action{waiting=1, movingToFire,movingToSupport, supporting,extinguishing, movingToStart};


// priority coefficient
#define K 0.2
#define M 0.2
#define L 0.1
#define N 0.1
#define H 0.2

//genetic algorithm
#define T1 20
#define PC 0.8
#define PM 1

#define K0 100
#define K1 3
#define K2 2
#endif // PARAMETER_H
