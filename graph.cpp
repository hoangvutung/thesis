#include"graph.h"
#include <iostream>
#include<algorithm>
QVector<int> Graph::setStartPoints(int k)
{
	QVector<QVector<int>>D = getMatrixFloyd();
	QVector<int> centers;
	if (k == 1) {
		int min = 0;
		int s_min = std::accumulate(D[0].begin(), D[0].end(), 0);
		for (int j = 1; j < numV; j++) {
			int sum = std::accumulate(D[j].begin(), D[j].end(), 0);
			if (sum < s_min) {
				s_min = sum;
				min = j;
			}
		}
		centers.push_back(min);
		return centers;
	}
	int maxIter = 20;
	QVector<QVector<int>> clusters;
	QVector<int> points(k,-1);
	for (int i = 0; i < k; i++) {
		int randNum = rand() % numV;
		if (centers.contains(randNum)) i--;
		else if (centers.size() > 0) {
			bool flag = true;
			/*for (int k = 0; k < centers.size(); k++) {
				if (D[randNum][centers[k]] < 50) {
					flag = false;
					break;
				}
			}*/
			if(flag) centers.push_back(randNum);
			else	i--;
		}
		else centers.push_back(randNum);
	}
	QVector<int> minD(k);
	while (maxIter>0) {
		maxIter--;
		points = centers;
		centers.clear();
		clusters.clear();
		clusters.resize(k);
		// определить кластер для каждой точки
		for (int i = 0; i < numV; i++) {
			int min = 0;
			for (int j = 1; j < k; j++) {
				if (D[i][points[j]] < D[i][points[min]]) min = j;
			}
			clusters[min].push_back(i);
		}
		//перечислить центр
		for (int i = 0; i < k; i++) {
			int size = clusters[i].size();
			QVector<QVector<int>> Dcl = D;
			for (int j = 0; j < numV; j++) {
				for (int k = j; k < numV; k++) {
					if (!clusters[i].contains(j) || !clusters[i].contains(k)) Dcl[j][k] = Dcl[k][j] = 0;
				}
			}
			int min = 0; 
			int s_min = std::accumulate(Dcl[clusters[i][min]].begin(), Dcl[clusters[i][min]].end(), 0);
			for (int j = 1; j < size; j++) {
				int sum= std::accumulate(Dcl[clusters[i][j]].begin(), Dcl[clusters[i][j]].end(), 0);
				if (sum < s_min) {
					s_min = sum;
					min = j;
				}
			}
			centers.push_back(clusters[i][min]);
			minD[i] = s_min;
		}
	}
	return points;
}
QVector<QVector<int>> Graph::getMatrixFloyd()
{
	QVector<QVector<int>> matrix = C;
	for (int i = 0; i < numV; i++) {
		matrix[i][i] = 0;
	}
	for (int k = 0; k < numV; k++)
	{
		for (int i = 0; i < numV; ++i)
		{
			for (int j = 0; j < numV; ++j)
			{
				matrix[i][j] = std::min(matrix[i][j], matrix[i][k] + matrix[k][j]);
			}
		}
	}
	return matrix;
}

Graph::Graph()
{
}

Graph::Graph(QVector<QVector<int>> Cs, QVector<QVector<int>> Bs){
	
	C = Csource = Cs;
	B = Bsource = Bs;
	PB.resize(numV);
	for (int i = 0; i < numV; i++) {
		probPoint.push_back(0.04*(double)rand() / RAND_MAX);
		PB[i].resize(numV);
		for (int j = 0; j < numV; j++) {
			if(B[i][j]>0) PB[i][j]= 0.001*(double)rand() / RAND_MAX;
		}
	}
	
   
	/*int min = (numV - 1)*(numV - 2) / 2 + 1;
	int m = min + qrand() % 2;
	for (int k = 0; k < m; k++) {
		int i = qrand() % numV;
		int j = qrand() % numV;
		if (i != j && i < j && C[i][j] == 0) C[i][j] = 1;
		else if (i != j && i > j && C[j][i] == 0) C[j][i] = 1;
		else k--;
	}
    for(int i=0;i<numV;i++){
        for(int j=i;j<numV;j++){
            if(C[i][j]>0 && qrand() % 3<2) {
				Csource[i][j] = Csource[j][i] = C[j][i] = C[i][j] = qrand() % 10 + 15;
                Bsource[i][j]=Bsource[j][i]=B[j][i]=B[i][j]= qrand()%3+1;
                PB[i][j]= 0.004*(double)rand() / RAND_MAX;
            }
			else {
				C[j][i] = C[i][j]=Csource[i][j] = Csource[j][i]  = notWay;
				PB[i][j] = Bsource[i][j] = Bsource[j][i] = 0;
			}
        }
    }
	QFile file("C:/Users/pc/Documents/data50.txt");
	if (file.open(QIODevice::ReadWrite)) {
		QTextStream stream(&file);
		for (int i = 0; i < numV; i++) {
			for (int j = 0; j < numV; j++) {
				stream << C[i][j] << endl;
				stream << B[i][j] << endl;
			}
		}
		file.close();
	}*/
}

Graph::Graph(const Graph & g)
{
	C = Csource = g.Csource;
	B = Bsource = g.Bsource;
	probPoint = g.probPoint;
	PB = g.PB;
	blockEdges = g.blockEdges;
}

bool Graph::update(){
    //update block edges
	bool changed = false;
    for(int i=0;i<numV;i++){
        for(int j=0;j<numV;j++){
            double r= (double)rand() / RAND_MAX;
            if(B[i][j] >0 && r<PB[i][j]){
                QVector<int> newBlock{i,j,rand()%4+9};
                blockEdges.push_back(newBlock);
                B[i][j]= B[j][i] = -1;
                C[i][j]= C[j][i] = notWay;
				changed = true;
            }
        }
    }
	if (blockEdges.size()) {
		for (int i = 0; i < blockEdges.size(); i++) {
			blockEdges[i][2]--;
			if (blockEdges[i][2] == 0) {
				int v1 = blockEdges[i][0];
				int v2 = blockEdges[i][1];
				B[v2][v1] = B[v1][v2] = Bsource[v1][v2];
				C[v2][v1] = C[v1][v2] = Csource[v1][v2];
				blockEdges.remove(i);
				changed = true;
				i--;
			}
		}
	}
	return changed;
}
/*QVector<int> Graph::minPaths(int robot) {
	QVector<int> minPaths(numV,notWay);
	minPaths[robot] = 0;
	for (int k = 0; k < numV; k++)
	{
		for (int i = 0; i < numV; ++i)
		{
			for (int j = 0; j < numV; ++j)
			{
				if (minPaths[j] > minPaths[i] + C[i][j]) {
					minPaths[j] = minPaths[i] + C[i][j];
				}
			}
		}
	}
	return minPaths;
}*/

QVector<int> Graph::getMinPath(QVector<int> minPath, int s, int t)
{
	int counter = 0;
	if (minPath[t] > notWay) {
		return QVector<int>();
	}
	QVector<int> Path;
	Path.push_front(t);
	while (t != s) {
		counter++;
		if (C[s][t] <100) {
			Path.push_front(s);
			t = s;
		}
		else {
			for (int i = 0; i < numV; i++) {
				if (minPath[t] == C[i][t] + minPath[i]) {
					Path.push_front(i);
					t = i;
					break;
				}
			}
		}
	}
	return Path;
}
