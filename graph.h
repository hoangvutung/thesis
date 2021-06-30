#ifndef GRAPH_H
#define GRAPH_H
#include"parameter.h"

class Graph{
	
	
public:
	QVector<QVector<int>> getMatrixFloyd();
	QVector<int> setStartPoints(int k);
    //QList<QGraphicsEllipseItem*> lPoint;
    QList<double> probPoint; //вероятность появления очагов
    QVector<QVector<int>> blockEdges;// list of block Edges
    QVector<QVector<int>> C;//матрица стоимости в момент t
    QVector<QVector<int>> B;//матрица пропускной способности в момент t
    //int Csource[numV][numV];//матрица стоимости
    //int Bsource[numV][numV];//матрица пропускной способности
    //double PB[numV][numV]; //Матрица вероятностей блокировок ребер
	QVector<QVector<int>> Csource;
	QVector<QVector<int>> Bsource;
	QVector<QVector<double>> PB;
	Graph();
    Graph(QVector<QVector<int>> Cs, QVector<QVector<int>> Bs);
	Graph(const Graph& g);
    bool update();
	QVector<int> getMinPath(QVector<int> minPath,int s, int t);
};

#endif // GRAPH_H
