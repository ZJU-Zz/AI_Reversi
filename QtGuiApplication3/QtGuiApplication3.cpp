#include "QtGuiApplication3.h"

#define TIMER_TIMEOUT   1000


QtGuiApplication3::QtGuiApplication3(QWidget *parent)
	: QMainWindow(parent)
{
	test = new ReversiBoard();
	test->reset();

	setStatusBar(0);
	scene = new QGraphicsScene;
	scene->setSceneRect(-200, -200, 600, 600);//设置场景大小  
	drawCrossLine();//画棋盘  

	view = new QGraphicsView;
	view->setBackgroundBrush(QColor(135, 206, 235));//设置背景颜色  
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去除QGraphicsView的滚动条  
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setScene(scene);
	view->setRenderHint(QPainter::Antialiasing);//设置反走样(抗锯齿)  
	setCentralWidget(view);

	/*固定大小*/
	setMaximumSize(800, 800);
	setMinimumSize(800, 800);
	setWindowTitle("Chess");//设置窗口标题
	showView();


	m_pTimer = new QTimer(this);

	QObject::connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
	QObject::connect(this, &QtGuiApplication3::viewChanged, this, &QtGuiApplication3::showView);
	QObject::connect(this, &QtGuiApplication3::gameOver, this, &QtGuiApplication3::endMessage);
	QObject::connect(this, &QtGuiApplication3::timingStart, this, &QtGuiApplication3::startTiming);
	QObject::connect(this, &QtGuiApplication3::timingEnd, this, &QtGuiApplication3::endTiming);

	//temptest();
	//startTiming();
}


QGraphicsTextItem* totalTimeItem = NULL;
QGraphicsTextItem* oneStepTimeItem = NULL;

void QtGuiApplication3::handleTimeout()
{
	if (totalTimeItem) {
		scene->removeItem(totalTimeItem);
		delete totalTimeItem;
	}
	totalTimeItem = new QGraphicsTextItem("TotalTime:" + QString::number(++totalTime, 10));
	totalTimeItem->setPos(QPointF(200,-250));//设置要放置的的位置
	totalTimeItem->setScale(1.5);
	scene->addItem(totalTimeItem);//添加item到scene上


	if (oneStepTimeItem) {
		scene->removeItem(oneStepTimeItem);
		delete oneStepTimeItem;
	}
	oneStepTimeItem = new QGraphicsTextItem("OneStepTime:" + QString::number(++oneStepTime, 10));
	oneStepTimeItem->setPos(QPointF(200, -200));//设置要放置的的位置
	oneStepTimeItem->setScale(1.5);
	scene->addItem(oneStepTimeItem);//添加item到scene上
}



void QtGuiApplication3::run()
{
	if (keyPressed == 'M') {
		if (mutex.tryLock(5000) == true)
		{
			while (1)
			{
				Point BestChild = UctSearch();
				cout << "(" << BestChild.x << "," << BestChild.y << ")" << endl;
				if (BestChild.x != -1) {
					vector<int> CanPlayVector = test->canPlay(BestChild.x, BestChild.y);
					test->doPlay(CanPlayVector, BestChild.x, BestChild.y);
					emit viewChanged();
				}
				else
				{
					if (test->isLeaf()) {
						emit gameOver();
						emit viewChanged();
						break;
					}
					else {
						test->reversePlaying();
						emit viewChanged();
					}
				}
			}
			mutex.unlock();
			cout << "Training Ok!" << endl;
		}
		else
		{
			cout << "Lock error" << endl;
		}
	}
	else if (keyPressed == 'A') {
		if (mutex.tryLock(1000)) {
			Point BestChild = UctSearch();
			cout << "(" << BestChild.x << "," << BestChild.y << ")" << endl;
			if (BestChild.x != -1) {
				vector<int> canPlayVector = test->canPlay(BestChild.x, BestChild.y);
				test->doPlay(canPlayVector, BestChild.x, BestChild.y);
				emit viewChanged();
			}
			else
			{
				if (test->isLeaf()) {
					emit gameOver();
				}
				else {
					test->reversePlaying();
					emit viewChanged();
				}
			}
			mutex.unlock();
		}
		else
		{
			cout << "locked! Try again later!" << endl;
		}
		emit timingEnd();
	}
	else if (keyPressed == 'H')
	{
		int PlayTime = 13;
		while (PlayTime--)
		{
			if (mutex.tryLock(5000) == true)
			{
				while (1)
				{
					Point BestChild = UctSearch();
					cout << "(" << BestChild.x << "," << BestChild.y << ")" << endl;
					if (BestChild.x != -1) {
						vector<int> CanPlayVector = test->canPlay(BestChild.x, BestChild.y);
						test->doPlay(CanPlayVector, BestChild.x, BestChild.y);
						emit viewChanged();
					}
					else
					{
						if (test->isLeaf()) {
							test->reset();
							emit viewChanged();
							break;
						}
						else {
							test->reversePlaying();
							emit viewChanged();
						}
					}
				}
				mutex.unlock();
				cout << "Training Ok!" << endl;
			}
			else
			{
				cout << "Lock error" << endl;
			}
		}
	}
	else if (keyPressed == 'K')
	{
		if (mutex.tryLock(1000)) {
			Point BestChild = UctSearch();
			cout << "(" << BestChild.x << "," << BestChild.y << ")" << endl;
			mutex.unlock();
		}
		else
		{
			cout << "locked! Try again later!" << endl;
		}
	}
}

void QtGuiApplication3::mousePressEvent(QMouseEvent *e)
{
	int m, n;

	int mx = e->x() - dx;
	int my = e->y() - dy;
	/*鼠标事件的坐标系是view,而不是scene*/

	if (mx >= 0 && mx < 320){
		n = mx / 40;
	}
	else {
		return;
	}

	if (my >= 0 && my < 320) {
		m = my / 40;
	}
	else {
		return;
	}
	/*如果当前落点有棋子,弹出警告*/
	if (test->getType(m,n) != EMPTY)
	{
		QMessageBox::warning(this, "warning", "This Position has Already had chess!", QMessageBox::Ok);
	}
	else
	{
		if (test->getPlaying() == BLACK)
		{
			vector<int> CanPlayVector = test->canPlay(m, n);
			if (CanPlayVector.size() != 0) 
			{
				test->doPlay(CanPlayVector, m, n);
				showView();
			}
			//添加黑棋  
		}
		else
		{
			vector<int> CanPlayVector = test->canPlay(m, n);
			if (CanPlayVector.size() != 0)
			{
				test->doPlay(CanPlayVector, m, n);
				showView();
			}
			//添加白棋  
		}
	} 
}

void QtGuiApplication3::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_P)
	{
		if (mutex.tryLock(1000)) {
			if (test->isEnd()) {
				emit gameOver();
			}
			else
			{
				test->autoRandomPlayOneStep();
				showView();
			}
			mutex.unlock();
		}
		else
		{
			cout << "locked! Try again later!" << endl;
		}

	}
	if (event->key() == Qt::Key_L)
	{
		if (mutex.tryLock(1000)) {
			test->takeBack();
			showView();
			mutex.unlock();
		}
		else
		{
			cout << "locked! Try again later!" << endl;
		}
	}
	if (event->key() == Qt::Key_A)
	{
		emit timingStart();
		keyPressed = 'A';
		start();
	}
	if (event->key() == Qt::Key_R)
	{
		if (mutex.tryLock(1000)) {
			test->reversePlaying();
			showView();
			mutex.unlock();
		}
		else
		{
			cout << "locked! Try again later!" << endl;
		}
	}
	if (event->key() == Qt::Key_S)
	{
		cout << "---------------------------------" << endl;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				vector<int> temp = test->canPlay(i, j);
				cout << temp.size() << " ";
			}
			cout << endl;
		}
		showView();
	}
	if (event->key() == Qt::Key_M)
	{
		keyPressed = 'M';
		start();
	}
	if (event->key() == Qt::Key_I)
	{
		if (mutex.tryLock(1000)) {
			reset();
			mutex.unlock();
		}
		else
		{
			cout << "locked! Try again later!" << endl;
		}
	}
	if (event->key() == Qt::Key_K)
	{
		keyPressed = 'K';
		start();
	}
	if (event->key() == Qt::Key_H)
	{
		keyPressed = 'H';
		start();
	}
}

void QtGuiApplication3::drawCrossLine()
{
	for (int i = 0; i < 9; i++)
	{
		if (i != 8)
		{
			QGraphicsTextItem *txtitem1 = new QGraphicsTextItem(QString::number(i, 10));
			QGraphicsTextItem *txtitem2 = new QGraphicsTextItem(QString::number(i, 10));
			txtitem1->setPos(QPointF(-300 + dx - 30, -300 + 40 * i + dy + 5));//设置要放置的的位置
			scene->addItem(txtitem1);//添加item到scene上
			txtitem2->setPos(QPointF(-300 + 40 * i + dx + 10, -300 + dy - 40));//设置要放置的的位置
			scene->addItem(txtitem2);//添加item到scene上
		}
		scene->addLine(-300 + dx, -300 + 40 * i + dy, 20 + dx, -300 + 40 * i + dy);
		scene->addLine(-300 + 40 * i + dx, -300 + dy, -300 + 40 * i + dx, 20 + dy);
	}
}

void QtGuiApplication3::clear()
{
	/*清除棋盘*/
	QList<QGraphicsItem*> list = scene->items();
	while (!list.empty())
	{
		if(list.at(0) != totalTimeItem && list.at(0) != oneStepTimeItem)
			scene->removeItem(list.at(0));
		list.removeAt(0);
	}
	drawCrossLine();
}

void QtGuiApplication3::addChess(int m, int n, Type type)
{
	if (type == BLACK)
	{
		scene->addEllipse(40 * m + dx - 300 + 2, 40 * n + dy - 300 + 2, 35, 35, QPen(Qt::NoPen), QBrush(Qt::black));//添加黑棋  
	}else if(type == WHITE)
	{
		scene->addEllipse(40 * m + dx - 300 + 2, 40 * n + dy - 300 + 2, 35, 35, QPen(Qt::NoPen), QBrush(Qt::white));//添加白棋  
	}
}

void QtGuiApplication3::reset()
{
	totalTime = oneStepTime = 0;
	test->reset();
	emit viewChanged();
}

void QtGuiApplication3::showView()
{
	clear();
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (test->getType(i, j) == BLACK) {
				addChess(j, i, BLACK);
			}
			else if (test->getType(i, j) == WHITE) {
				addChess(j, i, WHITE);
			}
		}
	}
	if (test->getPlaying() == BLACK) 
	{
		addChess(8, 8, BLACK);
	}
	else
	{
		addChess(8, 8, WHITE);
	}

	addChess(1, 9, WHITE);
	QGraphicsTextItem *txtitemWHITE = new QGraphicsTextItem(QString::number( test->getCount(WHITE), 10));
	txtitemWHITE->setPos(QPointF(-170,93));//设置要放置的的位置
	txtitemWHITE->setScale(1.5);
	scene->addItem(txtitemWHITE);//添加item到scene上

	addChess(1, 11, BLACK);
	QGraphicsTextItem *txtitemBLACK = new QGraphicsTextItem(QString::number(test->getCount(BLACK), 10));
	txtitemBLACK->setPos(QPointF(-170, 173));//设置要放置的的位置
	txtitemBLACK->setScale(1.5);
	scene->addItem(txtitemBLACK);//添加item到scene上

}

void QtGuiApplication3::sleep(unsigned int msec)
{
	QTime reachTime = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < reachTime)
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}

void QtGuiApplication3::temptest()
{
	reset();

	test->getBoard()[0] = 0x0000;
	test->getBoard()[1] = 0x0000;
	test->getBoard()[2] = 0xaa80;
	test->getBoard()[3] = 0x0240;
	test->getBoard()[4] = 0x0250;
	test->getBoard()[5] = 0x0254;
	test->getBoard()[6] = 0x0000;
	test->getBoard()[7] = 0x0000;

	//test->reversePlaying();
	showView();


}

void QtGuiApplication3::insertRoot()
{
	TreeNode root;
	root.n = root.w = root.parentIndex = 0;
	root.playing = BLACK;
	for (int i = 0; i < 8; i++)
	{
		root.Board[i] = test->getBoard()[i];
		cout << hex << test->getBoard()[i] << endl;
	}
	root.parentBoard[0] = 0xFFFF;
	Tree.Nodes.insert(root);
}

Point QtGuiApplication3::UctSearch()
{
	TreeNode current(test->getPlaying(), test->getBoard(), test->getBoard(), 0, 0, 0);
	QTime reachTime = QTime::currentTime().addMSecs(1000 * 10);
	int count = 0;
	while (QTime::currentTime() < reachTime)
	{
		count++;
		Search(current);

	}
	cout << count << endl;
	double value[8][8] = { 0 };
	unsigned int playTime[8][8] = { 0 };
	unsigned int TotalPlayTime = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			vector<int> temp = test->canPlay(i, j);
			if (temp.size() > 0)
			{
				TreeNode Child = test->getLeaf(temp, i, j);
			Try:
				Tree.iter = Tree.Nodes.find(Child);
				if (Tree.iter != Tree.Nodes.end())
				{
					for (int t = 0; t < 8; t++)
					{
						if (Tree.iter->parentBoard[t] != test->getBoard()[t])
						{
							Child.parentIndex = Tree.iter->parentIndex + 1;
							goto Try;
						}
					}
					TotalPlayTime += Tree.iter->n;
					if (Tree.iter->n != 0) {
						value[i][j] = Tree.iter->w / (double)Tree.iter->n;
						playTime[i][j] = Tree.iter->n;
					}
				}
				else
				{
					cout << "Error----------------------------------------------------" << endl;
					cout << i  << " " << j << endl;
					return Point();
				}
			}
		}
	}

	Point ret;
	cout << TotalPlayTime << endl;
	cout << "----------------------------------------" << endl;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			cout << playTime[i][j] << " ";
		}
		cout << endl;
	}
	cout << "----------------------------------------" << endl;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			cout << value[i][j] << " ";
		}
		cout << endl;
	}
	cout << "----------------------------------------" << endl;
	ret = getBestChild(TotalPlayTime, value, playTime);
	return ret;
}

void QtGuiApplication3::Search(TreeNode next)
{
	ReversiBoard TempBoard(next);
	double value[8][8] = { 0 };
	unsigned int playTime[8][8] = { 0 };
	unsigned int TotalPlayTime = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			vector<int> temp = TempBoard.canPlay(i, j);
			if (temp.size() > 0)
			{
				TreeNode Child = TempBoard.getLeaf(temp, i, j);
			Try:
				Tree.iter = Tree.Nodes.find(Child);
				if (Tree.iter != Tree.Nodes.end())
				{
					for (int t = 0; t < 8; t++)
					{
						if (Tree.iter->parentBoard[t] != TempBoard.getBoard()[t])
						{
							Child.parentIndex = Tree.iter->parentIndex + 1;
							goto Try;
						}
					}
					TotalPlayTime += Tree.iter->n;
					if (Tree.iter->n != 0) {
						value[i][j] = Tree.iter->w / (double)Tree.iter->n;
						playTime[i][j] = Tree.iter->n;
					}
				}
				else
				{
					Tree.Nodes.insert(Child);
					TempBoard.doPlay(temp, i, j);
					int tempcount = TempBoard.count;
					Type win = TempBoard.autoRandomPlay();
					backUp(Child, win , tempcount);
					return;
				}
			}
		}
	}
	
	Point bestChild = getBestChild(TotalPlayTime, value,playTime);

	if (bestChild.x != -1)
	{
		vector<int> playVector = TempBoard.canPlay(bestChild.x, bestChild.y);
		Search(TempBoard.getLeaf(playVector, bestChild.x, bestChild.y));
	}
	else
	{
		if (TempBoard.isLeaf()) {
			int bcount = TempBoard.getCount(BLACK);
			int wcount = TempBoard.getCount(WHITE);
			if (bcount > wcount) {
				backUp(next, BLACK,TempBoard.count);
			}
			else if (bcount < wcount) {
				backUp(next, WHITE,TempBoard.count);
			}
			else
			{
				backUp(next, EMPTY,TempBoard.count);
			}
			return;
		}
		TempBoard.reversePlaying();
		TreeNode pass(TempBoard.getPlaying() , TempBoard.getBoard(), TempBoard.getBoard(), 0, 0, 0);
		Try1:
		Tree.iter = Tree.Nodes.find(pass);
		if (Tree.iter != Tree.Nodes.end())
		{
			for (int t = 0; t < 8; t++)
			{
				if (Tree.iter->parentBoard[t] != TempBoard.getBoard()[t])
				{
					pass.parentIndex = Tree.iter->parentIndex + 1;
					goto Try1;
				}
			}
		}
		else
		{
			Tree.Nodes.insert(pass);
		}
		Search(pass);
	}
}

void QtGuiApplication3::backUp(TreeNode next, Type win,int count)
{
	for (int i = 0;; i++)
	{
		TreeNode temp = TreeNode(next.playing, next.Board, next.Board , 0, 0, i);
		Tree.iter = Tree.Nodes.find(temp);
		if (Tree.iter != Tree.Nodes.end())
		{
			TreeNode newNode = TreeNode(next.playing, next.Board, (unsigned short int*)Tree.iter->parentBoard, Tree.iter->w , Tree.iter->n + 1 , i);
			if (win == next.playing)
			{
				newNode.w++;
			}
			Tree.Nodes.erase(temp);
			Tree.Nodes.insert(newNode);
			if (newNode.parentBoard[0] == 0xFFFF) return;
			Type pType = (next.playing == BLACK) ? WHITE : BLACK;
			TreeNode parentNode = TreeNode(pType, newNode.parentBoard, newNode.parentBoard, 0, 0, 0);
			/*
			int flag = 0;
			for (int t = 0; t < 8; t ++)
			{
				if (newNode.parentBoard[t] != newNode.Board[t]) {
					if(count - 1 >= test->count) backUp(parentNode, win,count - 1);
					flag = 1;
					break;
				}
			}
			if(flag == 0 && count >= test->count) 
			*/
			backUp(parentNode, win, count);
		}
		else
		{
			break;
		}
	}
}

Point QtGuiApplication3::getBestChild(unsigned int total, double value[][8],unsigned int playTime[][8])
{
	Point ret;
	ret.x = ret.y = -1;
	double bestValue = 0;
	double ln = log(total);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (playTime[i][j] > 0)
			{
				double newValue = (1-value[i][j]) + C * sqrt(ln / playTime[i][j]);
				if (newValue >= bestValue)
				{
					bestValue = newValue;
					ret.x = i;
					ret.y = j;
				}
			}
		}
	}
	return ret;
}

void QtGuiApplication3::endMessage()
{
	int bcount = test->getCount(BLACK);
	int wcount = test->getCount(WHITE);
	if (bcount > wcount)
	{
		QMessageBox::warning(this, "warning", "BLACK WIN!", QMessageBox::Ok);
	}
	else if(bcount < wcount)
	{
		QMessageBox::warning(this, "warning", "WHITE WIN!", QMessageBox::Ok);
	}
	else
	{
		QMessageBox::warning(this, "warning", "TIED!", QMessageBox::Ok);
	}
}

void QtGuiApplication3::startTiming()
{
	if (!m_pTimer->isActive()) {
		oneStepTime = 0;
		m_pTimer->start(TIMER_TIMEOUT);
	}
}

void QtGuiApplication3::endTiming()
{
	if (m_pTimer->isActive()) {
		m_pTimer->stop();
	}
}

