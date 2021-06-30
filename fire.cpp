#include"fire.h"

void Fire::removeRobot(Robot r)
{
	for (int i = 0; i < Rlist.size(); i++) {
		if (Rlist[i].getType() == r.getType() && Rlist[i].isNeedSupport()) {
			Rlist.removeAt(i);
			return;
		}
	}
}

Fire::Fire(int p){
    currentPoint=p;
    Ilist.push_back(rand()%20+30);
    counterChangedType= rand() % 3 + 3;
    type= rand()%3;
}
Fire::Fire()
{
}

void Fire::update(){
    double currentI= Ilist.size()>0?Ilist.last():0;
    if(Rlist.size()==0){
        if(counterChangedType==0) {
            type= rand()%3;
            counterChangedType=rand()%3+3;
        }
        if(type==0) currentI -= (currentI*0.01);
        else if (type==2) currentI += (currentI*0.01);
        counterChangedType--;
    }
    else{
		for (int i = 0; i < Rlist.size(); i++) {
			currentI -= Rlist[i].P;
			Rlist[i].subResource();
			if (Rlist[i].getCurrentResourses()< Rlist[i].P) {
				Rlist.removeAt(i);
				i--;
			}
		}
    }
	Ilist.push_back(currentI);
}
bool Fire::isNeedMoreRobot(int time)
{
	int sum = 0;
	for (int i = 0; i < Rlist.size(); i++) {
		sum += Rlist[i].getCurrentResourses();
	}
	if(Ilist.size()) return Ilist.last() > sum;
	return false;
}
int Fire::getIn() {
	if(Ilist.size()) return Ilist.last();
	else return 0;
}