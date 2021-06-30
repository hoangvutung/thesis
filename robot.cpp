#include"robot.h"
Robot::Robot(type t, int p,Graph g){
    typeRobot=t;
	turn = false;
    counter=timeOut=0;
    graph= Graph(g);
    if(t==A){
        V=Va;
        P=Pa;
        Z=Za;
        currentResources=maxResources=Maxa;
    }
    else if (t==B){
        V=Vb;
        P=Pb;
        Z=Zb;
        currentResources=maxResources=Maxb;
    }
    else{
        V=Vc;
        P=Pc;
        Z=Zc;
        currentResources=maxResources=Maxc;
    }
    Ps= 0.01*(double)rand() / RAND_MAX;
    //currentPoint= graph.lPoint[p]->pos();
    currentVertex=p;
    needSupport=false;
    actionType=waiting;
	updateGraph();
	//minPath = graph.getMatrixFloyd();
}
type Robot::getType(){
    return typeRobot;
}
action Robot::getAction(){
    return actionType;
}
/*QPointF Robot::getCurrentPoint(){
    return currentPoint;
}*/
int Robot::getCurrentVertex()
{
	return currentVertex;
}
void Robot::move(){
	if (nextPoints.size() == 1) {
		//currentPoint = graph.lPoint[nextPoints[0]]->pos();
		nextPoints.clear();
	}
	if (timeOut == 0) {
		turn = false;
		nextPoints.pop_front();
		currentVertex = nextPoints.front();
		if (nextPoints.size() > 1 && graph.C[nextPoints[0]][nextPoints[1]] < notWay)
			counter = timeOut = graph.C[nextPoints[0]][nextPoints[1]] / V;
		else if (nextPoints.size() == 1) {
			counter = 0;
			nextPoints.pop_front();
		}
	}
	if (nextPoints.size() > 1 && (nextPoints[0] == nextPoints[1])) {
		currentVertex = nextPoints[0];
		timeOut = counter = 0;
		nextPoints.clear();
	}
	if (timeOut >= 1 && nextPoints.size() > 1&& graph.C[nextPoints[0]][nextPoints[1]] < notWay) {
		timeOut--;
		/*QPointF start = graph.lPoint[nextPoints[0]]->pos();
		QPointF end = graph.lPoint[nextPoints[1]]->pos();
		currentPoint.setX(start.x() + (end.x() - start.x())*(counter - timeOut) / counter);
		currentPoint.setY(start.y() + (end.y() - start.y())*(counter - timeOut) / counter);*/
	}
}
void Robot::setNextPoint(QVector<int> lP,action act){
	if ( lP.size()>1 && nextPoints.size()==0) {
		nextPoints = lP;
		nextPoints.push_front(currentVertex);
	}
	else if(lP.size()>1 && nextPoints.size() >1 && nextPoints[1] == lP[1]) nextPoints = lP;
	else if (timeOut && !turn) {
			if(nextPoints.size()>1)lP.push_front(nextPoints[1]);
			nextPoints = lP;
			currentVertex = nextPoints.first();
			turn = true;
	}
	if (lP.size() == 1 && actionType == extinguishing) return;
    actionType=act;
}
void Robot::setAction(action ac){
    actionType=ac;
}
bool Robot::errorRunning(){
    if((double)rand() / RAND_MAX <Ps) return true;
    else return false;
}
int Robot::getCurrentResourses(){
    return currentResources;
}
void Robot::support(){
    currentResources=maxResources;
    timeOut--;
	if (timeOut == 0) {
		actionType = waiting;
		needSupport = false;
	}
}
void Robot::extinguish(){
    currentResources -= P;
    if(currentResources<P) needSupport=true;
}
bool Robot::update(){
    if((actionType==movingToFire ||actionType==movingToSupport || actionType == movingToStart) && nextPoints.size() ) move();
    if(actionType==movingToSupport && nextPoints.size()<1){
        actionType=supporting;
        timeOut=Z;
        support();
    }
    else if (actionType==movingToFire && nextPoints.size()<1) {
        actionType=extinguishing;
        //extinguish();
		return true;
    }
	else if (actionType == movingToStart && nextPoints.size() < 1) {
		currentResources = maxResources;
	}
	else if(actionType == extinguishing) extinguish();
	else if (actionType == supporting) support();
    else if(actionType==waiting && currentResources<(maxResources*0.2)) needSupport=true;
	else if (actionType == waiting && timeOut) move();
	return false;
}
bool Robot::isNeedSupport(){
    return needSupport;
}

bool Robot::isOnBlockWay()
{
	if(nextPoints.size()>1) return (graph.C[nextPoints[0]][nextPoints[1]] == notWay);
	return false;
}

int Robot::getGoal()
{
	if (!nextPoints.isEmpty()) return (nextPoints.last()+1);
	return (1+currentVertex);
}

int Robot::getTimeOut()
{
	return timeOut;
}

void Robot::subResource()
{
	currentResources -= P;
}

int Robot::getAddTime()
{
	return counter-timeOut;
}

void Robot::updateGraph()
{
	for (int i = 0; i < numV; i++) {
		for (int j = 0; j < numV; j++) {
			if (graph.B[i][j] < typeRobot) graph.C[i][j] = notWay;
		}
	}
	//if(changed) minPath = graph.getMatrixFloyd();
}

