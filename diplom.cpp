#include "diplom.h"
#include "ui_diplom.h"
#include "hungarian.h"
using namespace std;
using namespace std::chrono;
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	readFromFile();
	counterFire = fireNum =timeCounter= 0;
	timeType1 = T1;
	typeChanged = false;
	qsrand(static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));
	ui->setupUi(this);
	graph = Graph(Csource,Bsource);
	//supportPoints = graph.setStartPoints(4);
	//supportPoints.push_back(427);
	//supportPoints.push_back(208);
	//supportPoints.push_back(276);
	supportPoints.push_back(14);
	supportPoints.push_back(338);
	for (int i = 0; i < numSupport; i++) {
		/*QColor color("blue");
		QBrush brush(color);
		QPen pen(brush, 2);
		graph.lPoint[supportPoints[i]]->setPen(pen);*/
		graph.probPoint[supportPoints[i]] = 0;
	}
	QObject::connect(ui->nextTak, SIGNAL(clicked()), this, SLOT(NextTak()));
	scene = new QGraphicsScene();
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	ui->view->setScene(scene);
	ui->textEdit->setReadOnly(true);
	ui->label_2->setNum(0);
	//groupSetting(supportPoints);
	//drawGraph();
	researchNumPoint();
	timer=new QTimer(this);
	connect(timer, SIGNAL(timeout()),this, SLOT(NextTak()));
	timer->start(100);
}
void MainWindow::groupSetting(QVector<int> startPoint) {
	int nA = numRobotA,nB=numRobotB ,nC= numRobotC;
	int nSum = nA + nB + nC;
	/*Robots.push_back(Robot(A, startPoint[0], graph));
	Robots.push_back(Robot(A, startPoint[1], graph));
	Robots.push_back(Robot(B, startPoint[2], graph));
	Robots.push_back(Robot(B, startPoint[2], graph));
	Robots.push_back(Robot(C, startPoint[0], graph));
	Robots.push_back(Robot(C, startPoint[1], graph));*/
	while (nA || nB || nC) {
		for (int i = 0; i < startPoint.size(); i++) {
			if (nA > 0) {
				Robots.push_back(Robot(A, startPoint[i], graph));
				nA--;
			}
			if (nB > 0) {
				Robots.push_back(Robot(B, startPoint[i], graph));
				nB--;
			}
			if (nC > 0) {
				Robots.push_back(Robot(C, startPoint[i], graph));
				nC--;
			}
		}
	}
	//make plan: waiting
	lastPlan = QVector<int>(Robots.size(), -1);
	//setMinPaths();
}
void MainWindow::researchNumPoint()
{
	if (count--) {
		Robots.clear();
		graph = Graph(Csource,Bsource);
		groupSetting(supportPoints);
		//int n = 1;
		while (Fires.size() < n) {
			for (int i = 0; i < numV; i++) {
				double ran = (double)rand() / RAND_MAX;
				if (graph.probPoint[i] < 0.99 && ran < graph.probPoint[i] && !supportPoints.contains(i) && Fires.size() < n) {
					graph.probPoint[i] = 1.0;
					Fires[i] = Fire(i);
				}
			}
		}
	}
	else {
		int _time = std::accumulate(timeReseach.begin(), timeReseach.end(), 0) / timeReseach.size();
		timeReseach.clear();
		time.push_back(_time);
		count = 10;
		n+=1;
		if(n<15) researchNumPoint();
		else {
			QFile file("C:/Users/pc/Documents/r10_3.txt");
			if (file.open(QIODevice::ReadWrite)) {
				QTextStream stream(&file);
				for (int i = 0; i < time.size(); i++) {
					stream << time[i] << endl;
				}
				file.close();
			}
			exit(0);
		}
	}
	
}
/*void MainWindow::makeLine(QGraphicsItem *item1, QGraphicsItem *item2, int  type, int c) {
	QPointF start = item1->pos() - QPointF(15, 15);
	QPointF end = item2->pos() - QPointF(15, 15);
	QLineF polyLine = QLineF(start, end);
	QGraphicsLineItem* line = new QGraphicsLineItem(polyLine);
	QColor* color;
	if (type == 1)  color = new QColor("yellow");
	else if (type == 2) color = new QColor("green");
	else if (type == 3)color = new QColor("cyan");
	else color = new QColor("black");
	QBrush brush(*color);
	QPen pen(brush, 2);
	line->setPen(pen);
	scene->addItem(line);
	QGraphicsTextItem * text = new QGraphicsTextItem(QString::number(c));
	text->setPos((end.x() + start.x()) / 2, (end.y() + start.y()) / 2);
	scene->addItem(text);
}

void MainWindow::drawGraph() {
	for (int i = 0; i < Fires.size(); i++) {
		QColor color("red");
		QBrush brush(color);
		graph.lPoint[(Fires.begin() + i).key()]->setBrush(brush);
	}
	for (int i = 0; i < numV; i++) {
		for (int j = i; j < numV; j++) {
			if (graph.B[i][j] != 0)makeLine(graph.lPoint[i], graph.lPoint[j], graph.B[i][j], graph.C[i][j]);
		}
	}
	for (int i = 0; i < numV; i++) {
		QGraphicsTextItem * text = new QGraphicsTextItem(QString::number(i + 1));
		text->setPos(graph.lPoint[i]->pos() - QPointF(20, 20));
		scene->addItem(graph.lPoint[i]);
		scene->addItem(text);
	}
	for (int i = 0; i < Robots.size(); i++) {
		//QGraphicsTextItem * text = new QGraphicsTextItem(QString::number(Robots[i].getGoal()));
		QGraphicsTextItem * text = new QGraphicsTextItem(QString::number(i) + "," + QString::number(Robots[i].getGoal()));
		text->setPos(Robots[i].getCurrentPoint() - QPointF(15, 15));
		QGraphicsRectItem* item = new QGraphicsRectItem(-20, -20, 20, 20);
		QColor* color;
		if (Robots[i].getType() == A) {
			color = new QColor("yellow");
			item = new QGraphicsRectItem(-12, -12, 12, 12);
		}
		else if (Robots[i].getType() == B) {
			color = new QColor("green");
			item = new QGraphicsRectItem(-15, -15, 15, 15);
		}
		else color = new QColor("cyan");
		if (Robots[i].isNeedSupport()) color = new QColor("black");
		QBrush brush(*color);
		item->setBrush(brush);
		item->setPos(Robots[i].getCurrentPoint());
		scene->addItem(item);
		scene->addItem(text);
	}

	QString text = "Число очагов: " + QString::number(counterFire + Fires.size()) + "\n";
	text += "Число активных очагов: " + QString::number(Fires.size()) + "\n";
	text += "Тущили " + QString::number(counterFire) + " очагов \n";
	if (type == 1) {
		text += "целевая функция: уменьшение числа очагов за 20 тактов\n";
		text += "Осталось: " + QString::number(timeType1);
	}

	else text += "целевая функция: уменьшение числа участвующих роботов";
	text += "\nЧисло участвующих роботов в ликвидации аварий: " + QString::number(rNum);
	ui->textEdit->setText(text);

}*/
void MainWindow::NextTak() {
	timeCounter++;
	QList<QGraphicsItem*> all = scene->items();
	for (int i = 0; i < all.size(); i++) {
		scene->removeItem(all[i]);
	}
	rNum = 0;
	update();
	makePlan();
	/*auto start = high_resolution_clock::now();
	makePlan();
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	time.push_back(duration.count());*/
	//drawGraph();

	/*researchIn.push_back(sumIn());
	researchRobot.push_back(rNum);*/
;	ui->label_2->setNum(timeCounter);
	if (Fires.size() == 0) {
		//int _time = std::accumulate(time.begin(),time.end(),0)/time.size();
		timeReseach.push_back(timeCounter);
		timeCounter = 0;
		researchNumPoint();
		/*QFile file("C:/Users/pc/Documents/sit2_2.txt");
		if (file.open(QIODevice::ReadWrite)) {
			QTextStream stream(&file);
			for (int i = 0; i < researchIn.size(); i++) {
				stream << researchIn[i] << "\t" << researchRobot[i] << endl;
			}
			file.close();
		}
		exit(0);*/
	}
}
void MainWindow::update() {
	/*if (graph.update()) {
		for (int i = 0; i < Robots.size(); i++)
			Robots[i].updateGraph();
		setMinPaths();
	}*/
	// update old fires
	for (int i = 0; i < Fires.size(); i++) {
		(Fires.begin() + i).value().update();
		if ((Fires.begin() + i).value().getIn() <= 0) {
			QColor color("white");
			QBrush brush(color);
			int v = (Fires.begin() + i).key();
			//graph.lPoint[v]->setBrush(brush);
			graph.probPoint[v] = 0.05*(double)rand() / RAND_MAX;
			for (int k = 0; k < Robots.size(); k++) {
				if (Robots[k].getCurrentVertex() == v && Robots[k].getAction() == extinguishing)
					Robots[k].setAction(waiting);
			}
			Fires.remove((Fires.begin() + i).key());
			counterFire++;
			i--;
			//int _time = std::accumulate(time.begin(), time.end(), 0) / time.size();
		}
	}
	// check new fires
	for (int i = 0; i < numV; i++) {
		double ran = (double)rand() / RAND_MAX;
		if (graph.probPoint[i] < 0.99 && ran < graph.probPoint[i] && !supportPoints.contains(i) && Fires.size()>0) {
			bool flag = false;
			for (int j = 0; j < numV; j++) {
				if (i != j && graph.B[i][j] > 0 && Fires.contains(j))
					flag = true;
			}
			if (flag) {
				graph.probPoint[i] = 1.0;
				Fires[i] = Fire(i);
			}
			else graph.probPoint[i] = 0;
		}
	}
	// update probPoint
	/*for (int i = 0; i < numV; i++) {
		if (!Fires.contains(i)) {
			for (int j = 0; j < numV; j++) {
				if (i != j && graph.B[i][j] > 0 && Fires.contains(j)) {
					graph.probPoint[i] += 0.000009* Fires[j].getIn();
				}
			}
		}
	}*/

}
void MainWindow::makePlan() {
	QVector<int> lRobot;
	pathsFromRobots.clear();
	pathFromNextPoints.clear();
	for (int i = 0; i < Robots.size(); i++) {
		if (Robots[i].update()) {
			if (Fires.contains(Robots[i].getCurrentVertex())) {
				Fires[Robots[i].getCurrentVertex()].Rlist.push_back(Robots[i]);
				Fires[Robots[i].getCurrentVertex()].update();
				Robots[i].extinguish();
			}
			else Robots[i].setAction(waiting);
		}
		QVector<int> paths = getMinPathFromV(Robots[i].getCurrentVertex(), Robots[i].getType());
		pathsFromRobots.push_back(paths);
		if (Robots[i].isNeedSupport() && Robots[i].getAction() != movingToSupport && Robots[i].getAction() != supporting)
			Robots[i].setNextPoint(getNearestSupport(paths, Robots[i]), movingToSupport);
		else if (Robots[i].getAction() == supporting || Robots[i].getAction() == movingToSupport || Robots[i].isOnBlockWay()) {

		}
		else if (Robots[i].getAction() == extinguishing) rNum++;
		else lRobot.push_back(i);
	}
	for (int i = 0; i < lRobot.size(); i++) {
		if (Robots[lRobot[i]].nextPoints.size() > 1 && Robots[lRobot[i]].getAddTime()) {
			QVector<int> paths = getMinPathFromV(Robots[lRobot[i]].nextPoints[1], Robots[lRobot[i]].getType()) ;
			pathFromNextPoints[lRobot[i]] = paths;
		}
	}
	fires = Fires.keys();
	for (int i = Fires.size() - 1; i >= 0; i--) {
		if (!(Fires.begin() + i).value().isNeedMoreRobot(0)) fires.removeAt(i);
	}

	if (fires.size() > 0 && lRobot.size() > 1) {
		controlRobot(lRobot);
	}
	else if (fires.size() > 0 && lRobot.size() == 1) {
		QVector<QPair<int, int>> robotToFires = sortPath(lRobot[0], fires);
		if (robotToFires[0].first < 200) {
			QVector<int> nextPoints = Robots[lRobot[0]].graph.getMinPath(pathsFromRobots[lRobot[0]], Robots[lRobot[0]].getCurrentVertex(), robotToFires[0].second);
			Robots[lRobot[0]].setNextPoint(nextPoints, movingToFire);
		}
		timeType1--;
		if (timeType1 <= T1 && timeType1 > 0) {
			typeChanged = false;
		}
		else if (timeType1 <= 0) timeType1 = T1;
	}
	else if (fires.size() > 0 && lRobot.size() == 0) timeType1--;
	else {
		for (int i = 0; i < lRobot.size(); i++)
			Robots[lRobot[i]].setNextPoint(getNearestSupport(pathsFromRobots[lRobot[i]], Robots[lRobot[i]]), movingToStart);
	}
}

void MainWindow::readFromFile()
{
	Csource.resize(numV);
	Bsource.resize(numV);
	for (int i = 0; i < numV; i++) {
		Csource[i].resize(numV);
		Bsource[i].resize(numV);
	}
	QFile file("C:/Users/pc/Documents/data504.txt");
	QFile file1("C:/Users/pc/Documents/Bdata504.txt");
	if (file.open(QIODevice::ReadWrite) && file1.open(QIODevice::ReadWrite)) {
		QTextStream in(&file);
		QTextStream in1(&file1);
		while (!in.atEnd() && !in1.atEnd()) {
			for (int i = 0; i < numV; i++) {
				QStringList s = in.readLine().split(QLatin1Char(','));
				QStringList s1 = in1.readLine().split(QLatin1Char(','));
				for (int j = 0; j < numV; j++) {
					Csource[i][j] = Csource[j][i]  = s[j].toDouble();
					Bsource[i][j] = Bsource[j][i]  = s1[j].toInt();
				}
			}
		}
	}

	// read minPaths
	minPathA.resize(numV);
	minPathB.resize(numV);
	minPathC.resize(numV);
	for (int i = 0; i < numV; i++) {
		minPathA[i].resize(numV);
		minPathB[i].resize(numV);
		minPathC[i].resize(numV);
	}
	QFile fileA("C:/Users/pc/Documents/minPathA.txt");
	QFile fileB("C:/Users/pc/Documents/minPathB.txt");
	QFile fileC("C:/Users/pc/Documents/minPathC.txt");
	if (fileA.open(QIODevice::ReadWrite) && fileB.open(QIODevice::ReadWrite) && fileC.open(QIODevice::ReadWrite)) {
		QTextStream inA(&fileA);
		QTextStream inB(&fileB);
		QTextStream inC(&fileC);
		while (!inA.atEnd() && !inB.atEnd() && !inC.atEnd()) {
			for (int i = 0; i < numV; i++) {
				QStringList s = inA.readLine().split(QLatin1Char(','));
				QStringList s1 = inB.readLine().split(QLatin1Char(','));
				QStringList s2 = inC.readLine().split(QLatin1Char(','));
				for (int j = 0; j < numV; j++) {
					minPathA[i][j] = s[j].toInt();
					minPathB[i][j] = s1[j].toInt();
					minPathC[i][j] = s2[j].toInt();
				}
			}
		}
	}
}

QVector<int> MainWindow::getMinPathFromV(int v, int typeRobot)
{
	if (typeRobot == A) return minPathA[v];
	else if (typeRobot == B) return minPathB[v];
	else return minPathC[v];
}

void MainWindow::setMinPaths()
{
	bool a = true, b = true, c = true;
	int counter = 0;
	while (a || b || c) {
		if (a && Robots[counter].getType() == A) {
			minPathA = Robots[counter].graph.getMatrixFloyd();
			/*QFile file("C:/Users/pc/Documents/minPathA.txt");
			if (file.open(QIODevice::ReadWrite)) {
				QTextStream stream(&file);
				for (int i = 0; i < numV; i++) {
					for (int j = 0; j < numV; j++) {
						stream << minPathA[i][j] << ",";
					}
					stream << endl;
				}
			}*/
			a = false;
		}
		else if (b && Robots[counter].getType() == B) {
			minPathB = Robots[counter].graph.getMatrixFloyd();
			/*QFile file("C:/Users/pc/Documents/minPathB.txt");
			if (file.open(QIODevice::ReadWrite)) {
				QTextStream stream(&file);
				for (int i = 0; i < numV; i++) {
					for (int j = 0; j < numV; j++) {
						stream << minPathB[i][j] << ",";
					}
					stream << endl;
				}
			}*/
			b = false;
		}
		else if (c && Robots[counter].getType() == C) {
			minPathC = Robots[counter].graph.getMatrixFloyd();
			/*QFile file("C:/Users/pc/Documents/minPathC.txt");
			if (file.open(QIODevice::ReadWrite)) {
				QTextStream stream(&file);
				for (int i = 0; i < numV; i++) {
					for (int j = 0; j < numV; j++) {
						stream << minPathC[i][j] << ",";
					}
					stream << endl;
				}
			}*/
			c = false;
		}
		counter++;
	}

}

void MainWindow::GeneticAlgorithm(QVector<int> lRobot) {

	int num = 100;
	int maxIteration = 50;
	QVector<int> f_min;
	if (fires.size() < lRobot.size()) {
		type = 2;
		typeChanged = true;
		timeType1 = T1;
	}
	else {
		type = 1;
		//maxIteration = 150;
		if (timeType1 <= T1 && timeType1 > 0) {
			timeType1--;
			typeChanged = false;
		}
		else if (timeType1 <= 0) timeType1 = T1;
	}
	int numRobot = lRobot.size();
	QVector<QVector<int>> population(num);
	QVector<QVector<int>> population1(num / 2);
	QVector<int> f(num);
	for (int i = 0; i < num; i++) {
		population[i].resize(numRobot);
		population1[i / 2].resize(numRobot);
		for (int j = 0; j < numRobot; j++) {
			population[i][j] = -1;
		}
	}
	//начальная популяция
	for (int i = 0; i < numRobot; i++) {
		if (fires.contains(lastPlan[lRobot[i]]))population[0][i] = lastPlan[lRobot[i]];
	}
	if (type == 1) {
		for (int k = 0; k < numRobot; k++)
			if (population[0][k] < 0) population[0][k] = fires[rand() % fires.size()];
	}
	f[0] = finessFuction(lRobot, population[0], type);
	for (int i = 1; i < num; i++) {
		if (type == 2) {
			for (int k = 0; k < fires.size(); k++) {
				if (k >= lRobot.size()) break;
				int randRobot = rand() % lRobot.size();
				while (population[i][randRobot] >= 0) {
					randRobot = rand() % lRobot.size();
				}
				population[i][randRobot] = fires[k];
			}
			/*for (int k = 0; k < numRobot; k++) {
				if (population[i][k] < 0 && (type == 1 || (type == 2 && rand() % 2))) population[i][k] = fires[rand() % fires.size()];
			}*/
		}
		else {
			for (int j = 0; j < numRobot; j++) {
				QVector<QPair<int, int>> robotToFires = sortPath(lRobot[j], fires);
				if (robotToFires.size() > 1)population[i][j] = robotToFires[rand() % 2].second;
				else population[i][j] = robotToFires[0].second;
			}
		}
		f[i] = finessFuction(lRobot, population[i], type);
	}
	while (maxIteration) {
		//reprodution
		int counter = 0, index = 0;
		while (counter < num) {
			if (type == 2) {
				if (f[counter] < f[counter + 1])
					population1[index] = population[counter];
				else population1[index] = population[counter + 1];
			}
			else {
				if (f[counter] > f[counter + 1])
					population1[index] = population[counter];
				else population1[index] = population[counter + 1];
			}
			index++;
			counter = counter + 2;
		}

		// кроссинговер
		for (int i = 0; i < population1.size() - 1; i++) {
			if (rand() / RAND_MAX < PC) {
				QVector<int> chrom1 = population1[i];
				QVector<int> chrom2 = population1[i + 1];
				int randomNum = rand() % numRobot;
				for (int j = 0; j < numRobot; j++) {
					if (j < randomNum) population1[i][j] = population1[i + 1][j];
					else population1[i + 1][j] = population1[i][j];
				}
			}
		}
		//мутация
		for (int i = 0; i < population1.size(); i++) {
			if (rand() / RAND_MAX < PM) {
				int randomNum1 = rand() % numRobot;
				if (type == 2) {
					int randomNum2 = rand() % numRobot;
					while (randomNum1 == randomNum2)
						randomNum2 = rand() % numRobot;
					population1[i][randomNum1] = population1[i][randomNum2];
				}
				else population1[i][randomNum1] = fires[rand() % (fires.size())];
			}
		}
		//reduction
		QVector<QVector<int>> tempPopulation = population;
		population.clear();
		QVector<int> fit = f;
		f.clear();
		for (int i = 0; i < population1.size(); i++) {
			tempPopulation.push_back(population1[i]);
			fit.push_back(finessFuction(lRobot, population1[i], type));
		}
		for (int i = 0; i < num; i++) {
			int t;
			if (type == 2) t = *std::min_element(fit.begin(), fit.end());
			else t = *std::max_element(fit.begin(), fit.end());
			int ind;
			for (int k = 0; k < fit.size(); k++) {
				if (fit[k] == t) {
					ind = k;
					break;
				}
			}
			population.push_back(tempPopulation[ind]);
			f.push_back(fit[ind]);
			for (int k = 0; k < fit.size(); k++) {
				if (fit.at(k) == t && type == 2)fit[k] = INT16_MAX;
				else if (fit.at(k) == t && type == 1)fit[k] = 0;
			}
		}
		maxIteration--;
		f_min.push_back(f[0]);
	}
	QVector<int> goals = population[0];
	int fMin = finessFuction(lRobot, goals, type);
	if (fMin <= 0) {
		for (int i = 0; i < numRobot; i++) {
			QVector<QPair<int, int>> robotToFires = sortPath(lRobot[i], fires);
			QVector<int> nextPoints = Robots[lRobot[i]].graph.getMinPath(pathsFromRobots[lRobot[i]], Robots[lRobot[i]].getCurrentVertex(), robotToFires[0].second);
			Robots[lRobot[i]].setNextPoint(nextPoints, movingToFire);
		}
	}
	else {
		for (int i = 0; i < numRobot; i++) {
			if (goals[i] >= 0) {
				QVector<int> nextPoints = Robots[lRobot[i]].graph.getMinPath(pathsFromRobots[lRobot[i]], Robots[lRobot[i]].getCurrentVertex(), goals[i]);
				Robots[lRobot[i]].setNextPoint(nextPoints, movingToFire);
				rNum++;
			}
			else Robots[lRobot[i]].setNextPoint(getNearestSupport(pathsFromRobots[lRobot[i]], Robots[lRobot[i]]), movingToStart);
			lastPlan[lRobot[i]] = goals[i];
		}
	}

}
void MainWindow::controlRobot(QVector<int> lRobot) {
	int result;
	int numRobot = lRobot.size();
	int numFire = fires.size();
	QVector<int> chrom(numRobot);
	if (fires.size() < numRobot) {
		type = 2;
		typeChanged = true;
		timeType1 = T1;
	}
	else {
		type = 1;
		if (timeType1 <= T1 && timeType1 > 0) {
			timeType1--;
			typeChanged = false;
		}
		else if (timeType1 <= 0) timeType1 = T1;
	}
	if (type == 2) {
		/*QVector<int> arr(numRobot);
		for (int i = 0; i < numRobot; i++) arr[i] = i;
		QVector<int> robotToFire(numFire);
		for (int i = 0; i < numFire; i++) robotToFire[i] = arr[i];
		for (int i = 0; i < numRobot; i++) {
			chrom[i] = robotToFire.indexOf(i) >= 0 ? fires[robotToFire.indexOf(i)] : -1;
		}
		result = finessFuction(lRobot, chrom, type);
		// make combination
		bool flag = true;
		while (flag) {
			int j;
			do  // повторяем пока не будет найдено следующее размещение
			{
				j = numRobot - 2;
				while (j != -1 && arr[j] >= arr[j + 1]) j--;
				if (j == -1) {
					flag = false;
					break;
					// больше размещений нет
				}
				int k = numRobot - 1;
				while (arr[j] >= arr[k]) k--;
				std::iter_swap(arr.begin() + j, arr.begin() + k);
				int l = j + 1, r = numRobot - 1; // сортируем оставшуюся часть последовательности
				while (l < r) std::iter_swap(arr.begin() + (l++), arr.begin() + (r--));
			} while (j > numFire - 1);
			QVector<int> chrom1(numRobot);
			for (int i = 0; i < numFire; i++) robotToFire[i] = arr[i];
			for (int i = 0; i < numRobot; i++) {
				chrom1[i] = robotToFire.indexOf(i) >= 0 ? fires[robotToFire.indexOf(i)] : -1;
			}
			int result1 = finessFuction(lRobot, chrom1, type);
			if (result > result1) {
				result = result1;
				chrom = chrom1;
			}
		}*/
		QVector<QVector<double>> costMatrix(numRobot);
		for (int i = 0; i < numRobot; i++) {
			costMatrix[i].resize(numRobot);
			//compute cost of i-robot for each fire
			for (int j = 0; j < numFire; j++) {
				costMatrix[i][j] = timeToGoal(lRobot[i], fires[j]);
				int delay = Fires[fires[j]].getIn() - Robots[lRobot[i]].getCurrentResourses();
				if (delay > 0) costMatrix[i][j] += delay * 0.1;
			}
		}
		HungarianAlgorithm HungAlgo;
		QVector<int> assignment;
		double cost = HungAlgo.Solve(costMatrix, assignment);
		for (int i = 0; i < numRobot; i++) {
			if (assignment[i] < numFire) chrom[i] = fires[assignment[i]];
			else chrom[i] = -1;
		}
	}
	else {
		int length = fires.size()>3?3:fires.size() ;
		QVector<QVector<int>> nearest2(numRobot);
		for (int i = 0; i < numRobot; i++) {
			QVector<QPair<int, int>> robotToFires = sortPath(lRobot[i], fires);
			for (int j = 0; j < length; j++) {
				nearest2[i].push_back(robotToFires[j].second);
			}
			chrom[i] = nearest2[i][0];
		}
		QVector<int> coder(numRobot);
		result = finessFuction(lRobot, chrom, type);
		while (coder[0] != length)
		{
			QVector<int> ch(numRobot);
			for (int i = 0; i < numRobot; i++) {
				ch[i] = nearest2[i][coder[i]];
			}
			int res = finessFuction(lRobot, ch, type);
			if (res > result) {
				result = res;
				chrom = ch;
			}
			bool next = true;
			int index = numRobot - 1;
			while (next)
			{
				coder[index] += 1;
				if (coder[index] < length || coder[0] == length) next = false;
				else {
					coder[index] = 0;
					index--;
				}
			}
		}
	}
	for (int i = 0; i < numRobot; i++) {
		if (chrom[i] >= 0) {
			QVector<int> nextPoints = Robots[lRobot[i]].graph.getMinPath(pathsFromRobots[lRobot[i]], Robots[lRobot[i]].getCurrentVertex(), chrom[i]);
			Robots[lRobot[i]].setNextPoint(nextPoints, movingToFire);
			rNum++;
		}
		else Robots[lRobot[i]].setNextPoint(getNearestSupport(pathsFromRobots[lRobot[i]], Robots[lRobot[i]]), movingToStart);
		lastPlan[lRobot[i]] = chrom[i];
	}
}

int MainWindow::finessFuction(QVector<int> lRobot, QVector<int> goals, int type)
{
	int timeOnPath = 0;
	if (type == 1) {
		int counter = 0;
		int notOptimal = 0;
		QMap<int, int> fireIn;
		for (int i = 0; i < goals.size(); i++) {
			fireIn[goals[i]] = Fires[goals[i]].getIn();
		}
		for (int i = 0; i < lRobot.size(); i++) {
			int time = timeToGoal(lRobot[i], goals[i]);
			timeOnPath += time;
			int r = (time < timeType1) ? timeType1 - time : 0;
			int maxRes = (r*Robots[lRobot[i]].P < Robots[lRobot[i]].getCurrentResourses()) ? r * Robots[lRobot[i]].P : Robots[lRobot[i]].getCurrentResourses();
			if (fireIn[goals[i]] > 0) fireIn[goals[i]] -= maxRes;
			else notOptimal += 1;

		}
		for (int i = 0; i < fireIn.size(); i++) {
			if ((fireIn.begin() + i).value() < 0) counter++;
		}
		return 70 * counter - 30 * notOptimal - timeOnPath;
	}
	else {
		for (int i = 0; i < lRobot.size(); i++) {
			if (goals[i] >= 0) {
				timeOnPath += timeToGoal(lRobot[i], goals[i]);
			}
		}
		int timeOnFire = 0;
		int subIn = 0;
		QMap<int, int> pairs;
		QMap<int, int> resource;
		int sum = 0;
		for (int i = 0; i < goals.size(); i++) {
			if (goals[i] >= 0) {
				if (pairs.contains(goals[i])) {
					pairs[goals[i]] += Robots[lRobot[i]].P;
					resource[goals[i]] += Robots[lRobot[i]].getCurrentResourses();
				}
				else {
					pairs[goals[i]] = Robots[lRobot[i]].P;
					resource[goals[i]] = Robots[lRobot[i]].getCurrentResourses();
				}
			}
		}
		for (int i = 0; i < pairs.size(); i++) {
			timeOnFire += Fires[(pairs.begin() + i).key()].getIn() / (pairs.begin() + i).value();
			subIn += Fires[(resource.begin() + i).key()].getIn() - (resource.begin() + i).value();

		}
		if (subIn < 0) subIn = 0;
		QSet<int> myset;
		for (int i = 0; i < goals.size(); i++) {
			if (goals[i] >= 0)myset.insert(goals[i]);
		}
		return 20 * timeOnPath + 8 * subIn + 2 * timeOnFire + 800 * (fires.size() - myset.size());
	}
}

int MainWindow::sumIn()
{
	int sum = 0;
	for (int i = 0; i < Fires.size(); i++) {
		sum += (Fires.begin() + i).value().getIn();
	}
	return sum;
}

QVector<QPair<int, int>> MainWindow::sortPath(int robot, QList<int> listV)
{
	QVector<QPair<int, int>> res;
	for (int i = 0; i < listV.size(); i++) {
		//int path = pathsFromRobots[robot][listV[i]];
		int path = timeToGoal(robot, listV[i]);
		res.push_back(qMakePair(path, listV[i]));
	}
	std::sort(res.begin(), res.end());
	return res;
}

QVector<int> MainWindow::getNearestSupport(QVector<int> path, Robot r)
{
	int min = 5000;
	int v;
	for (int j = 0; j < supportPoints.size(); j++) {
		if (min > path[supportPoints[j]]) {
			min = path[supportPoints[j]];
			v = supportPoints[j];
		}
	}
	if (min < 5000)return r.graph.getMinPath(path, r.getCurrentVertex(), v);
	else return QVector<int>();
}

int MainWindow::timeToGoal(int nRobot, int goal)
{
	int time = 0;
	if (pathFromNextPoints.contains(nRobot)) {
		int fromRobot = pathsFromRobots[nRobot][goal];
		int fromNextPoint = pathFromNextPoints[nRobot][goal];
		if (fromRobot == (fromNextPoint + graph.C[Robots[nRobot].getCurrentVertex()][Robots[nRobot].nextPoints[1]]))
			time = (pathsFromRobots[nRobot][goal] / Robots[nRobot].V) - Robots[nRobot].getAddTime();
		else time = (pathsFromRobots[nRobot][goal] / Robots[nRobot].V) + Robots[nRobot].getAddTime();
	}
	else time = (pathsFromRobots[nRobot][goal] / Robots[nRobot].V);
	return time;
}
MainWindow::~MainWindow() {}