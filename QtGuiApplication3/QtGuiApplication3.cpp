#include "QtGuiApplication3.h"




QtGuiApplication3::QtGuiApplication3(QWidget *parent)
	: QMainWindow(parent)
{
	test = new ReversiBoard();
	test->reset();



	setStatusBar(0);
	scene = new QGraphicsScene;
	scene->setSceneRect(-200, -200, 600, 600);//���ó�����С  

	drawCrossLine();//������  

	view = new QGraphicsView;
	view->setBackgroundBrush(QColor(135, 206, 235));//���ñ�����ɫ  
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//ȥ��QGraphicsView�Ĺ�����  
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setScene(scene);
	view->setRenderHint(QPainter::Antialiasing);//���÷�����(�����)  
	setCentralWidget(view);

	/*�̶���С*/
	setMaximumSize(800, 800);
	setMinimumSize(800, 800);
	setWindowTitle("Chess");//���ô��ڱ���
	showView();

	//Type win = test->autoRandomPlay();
	//showView();
	//addChess(10, 10, win);
	//cout << "white" << test->getCount(WHITE) << endl;
	//cout << "black" << test->getCount(BLACK) << endl;

	//test->autoRandomPlayOneStep();
	//showView();
	//sleep(1000);
	//temptest();
}


void QtGuiApplication3::run()
{
	while (1)
	{
		Point nextPlay = UctSearch();
		cout << "(" << nextPlay.x << "," << nextPlay.y << ")" << endl;
		if (nextPlay.x != -1) {
			vector<int> tt = test->canPlay(nextPlay.x, nextPlay.y);
			test->doPlay(tt, nextPlay.x, nextPlay.y);
			//showView();
		}
		else
		{
			if (test->isLeaf()) {
				//QMessageBox::information(this, "Title", "End");
				//test->reset();
				break;
			}
			else {
				test->reversePlaying();
				//showView();
			}
		}
	}
	cout << "Training Ok!" << endl;
}

void QtGuiApplication3::mousePressEvent(QMouseEvent *e)
{
	int m, n;

	//cout << e->x() << " " << e->y() << endl;
	int mx = e->x() - dx;
	int my = e->y() - dy;
	/*����¼�������ϵ��view,������scene*/

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
	/*�����ǰ���������,��������*/
	if (test->getType(m,n) != EMPTY)
	{
		QMessageBox::warning(this, "warning", "This Position has Already had chess!", QMessageBox::Ok);
	}
	else
	{
		if (test->getPlaying() == BLACK)
		{
			vector<int> temp = test->canPlay(m, n);
			if (temp.size() != 0) 
			{
				test->doPlay(temp, m, n);
				showView();
				//sleep(1000);
				//test->autoRandomPlayOneStep();
				//showView();
			}
			//��Ӻ���  
		}
		else
		{
			vector<int> temp = test->canPlay(m, n);
			if (temp.size() != 0)
			{
				test->doPlay(temp, m, n);
				showView();
				//sleep(1000);
				//test->autoRandomPlayOneStep();
				//showView();
			}
			//��Ӱ���  
		}
	}
	//Sleep(1000);
//	palyer = !palyer;//ת���ڰ�����  
//	Win();//�ж���Ӯ  
}

void QtGuiApplication3::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_P)
	{
		if (test->isEnd()) {
			QMessageBox::information(this, "Title", "End");
			reset();
		}
		else
		{
			test->autoRandomPlayOneStep();
			showView();
		}
	}

	if (event->key() == Qt::Key_A)
	{
		Point nextPlay = UctSearch();
		cout << "(" << nextPlay.x << "," << nextPlay.y << ")" << endl;
		if (nextPlay.x != -1) {
			vector<int> tt = test->canPlay(nextPlay.x, nextPlay.y);
			test->doPlay(tt, nextPlay.x, nextPlay.y);
			showView();
		}
		else
		{
			if (test->isLeaf()) {
				QMessageBox::information(this, "Title", "End");
				reset();
			}
			else {
				test->reversePlaying();
				showView();
			}
		}
	}
	if (event->key() == Qt::Key_R)
	{
		test->reversePlaying();
		showView();
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
		start();
	}
	if (event->key() == Qt::Key_I)
	{
		reset();
	}
	if (event->key() == Qt::Key_K)
	{
		Point nextPlay = UctSearch();
		cout << "(" << nextPlay.x << "," << nextPlay.y << ")" << endl;
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
			txtitem1->setPos(QPointF(-300 + dx - 30, -300 + 40 * i + dy + 5));//����Ҫ���õĵ�λ��
			scene->addItem(txtitem1);//���item��scene��
			txtitem2->setPos(QPointF(-300 + 40 * i + dx + 10, -300 + dy - 40));//����Ҫ���õĵ�λ��
			scene->addItem(txtitem2);//���item��scene��
		}
		scene->addLine(-300 + dx, -300 + 40 * i + dy, 20 + dx, -300 + 40 * i + dy);
		scene->addLine(-300 + 40 * i + dx, -300 + dy, -300 + 40 * i + dx, 20 + dy);
	}
	//QGraphicsTextItem *txtitem = new QGraphicsTextItem("hello");
	//txtitem->setPos(QPointF(100,100));//����Ҫ���õĵ�λ��
	//scene->addItem(txtitem);//���item��scene��
}

void QtGuiApplication3::clear()
{
	/*�������*/
	QList<QGraphicsItem*> list = scene->items();
	while (!list.empty())
	{
		scene->removeItem(list.at(0));
		list.removeAt(0);
	}
	drawCrossLine();
}

void QtGuiApplication3::addChess(int m, int n, Type type)
{
	if (type == BLACK)
	{
		scene->addEllipse(40 * m + dx - 300 + 2, 40 * n + dy - 300 + 2, 35, 35, QPen(Qt::NoPen), QBrush(Qt::black));//��Ӻ���  
	}else if(type == WHITE)
	{
		scene->addEllipse(40 * m + dx - 300 + 2, 40 * n + dy - 300 + 2, 35, 35, QPen(Qt::NoPen), QBrush(Qt::white));//��Ӱ���  
	}
}

void QtGuiApplication3::reset()
{
	test->reset();
	showView();
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
	test->setType(WHITE, 2, 2);
	test->setType(WHITE, 2, 3);
	test->setType(WHITE, 2, 4);
	test->setType(WHITE, 3, 3);
	test->setType(WHITE, 3, 4);
	test->setType(WHITE, 4, 4);

	test->setType(BLACK, 3, 1);
	test->setType(BLACK, 4, 2);
	test->setType(BLACK, 4, 3);
	test->setType(BLACK, 5, 3);


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
		//cout << count++ << endl;
		//QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

	}
	cout << count << endl;
	double value[8][8] = { 0 };
	unsigned int playTime[8][8] = { 0 };
	unsigned int n = 0;
	//test->printBoard();
	//cout << test->getPlaying() << endl;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			vector<int> temp = test->canPlay(i, j);
			if (temp.size() != 0)
			{
				//cout << i << " " << j << "can play" << endl;
				TreeNode a = test->getLeaf(temp, i, j);
			Try:
				Tree.iter = Tree.Nodes.find(a);
				if (Tree.iter != Tree.Nodes.end())
				{
					for (int t = 0; t < 8; t++)
					{
						if (Tree.iter->parentBoard[i] != test->getBoard()[i])
						{
							a.parentIndex = Tree.iter->parentIndex + 1;
							goto Try;
						}
					}
					n += Tree.iter->n;
					if (Tree.iter->n != 0) {
						value[i][j] = Tree.iter->w / (double)Tree.iter->n;
						playTime[i][j] = Tree.iter->n;
					}
				}
				else
				{
					cout << "Error----------------------------------------------------" << endl;
					return Point();
				}
			}
		}
	}

	//unsigned int maxVisit = 0;
	Point ret;
	ret.x = ret.y = -1;
	cout << n << endl;
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
	ret = getBestChild(n, value, playTime);
	return ret;
}

void QtGuiApplication3::Search(TreeNode next)
{
	ReversiBoard r(next);
	double value[8][8] = { 0 };
	unsigned int playTime[8][8] = { 0 };
	unsigned int n = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			vector<int> temp = r.canPlay(i, j);
			if (temp.size() > 0)
			{
				//cout << "OK" << endl;
				TreeNode a = r.getLeaf(temp, i, j);
			Try:
				Tree.iter = Tree.Nodes.find(a);
				if (Tree.iter != Tree.Nodes.end())
				{
					for (int t = 0; t < 8; t++)
					{
						if (Tree.iter->parentBoard[i] != r.getBoard()[i])
						{
							a.parentIndex = Tree.iter->parentIndex + 1;
							goto Try;
						}
					}
					n += Tree.iter->n;
					if (Tree.iter->n != 0) {
						value[i][j] = Tree.iter->w / (double)Tree.iter->n;
						playTime[i][j] = Tree.iter->n;
					}
				}
				else
				{
					Tree.Nodes.insert(a);
					r.doPlay(temp, i, j);
					Type win = r.autoRandomPlay();
					backUp(a, win);
					//cout << "Play:"  << next.playing << " "<< i << " " << j << " " << win << endl;
					return;
				}
			}
		}
	}
	/*
	cout << n << endl;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			cout << playTime[i][j] << " ";
		}
		cout << endl;
	}
	for (int i = 0; i < 8; i++)
	{
	for (int j = 0; j < 8; j++)
	{
		cout << value[i][j] << " ";
	}
	cout << endl;
	}
	//Tree.printHash();
	*/
	
	Point bestChild = getBestChild(n, value,playTime);

	if (bestChild.x != -1)
	{
		//cout << "bestchild::" << bestChild.x << " " << bestChild.y << endl;
		//system("pause");
		vector<int> playVector = r.canPlay(bestChild.x, bestChild.y);
		Search(r.getLeaf(playVector, bestChild.x, bestChild.y));
	}
	else
	{
		//cout << "Leaf---------------------------" << endl;
		//cout << "nobestchild::" << bestChild.x << " " << bestChild.y << endl;
		if (r.isLeaf()) {
			//cout << "this is ture Leaf---------------------------" << endl;
			int bcount = r.getCount(BLACK);
			int wcount = r.getCount(WHITE);
			if (bcount > wcount) {
				backUp(next, BLACK);
			}
			else if (bcount < wcount) {
				backUp(next, WHITE);
			}
			else
			{
				backUp(next, EMPTY);
			}
			return;
		}
		r.reversePlaying();
		TreeNode pass(r.getPlaying() , r.getBoard(), r.getBoard(), 0, 0, 0);
		Try1:
		Tree.iter = Tree.Nodes.find(pass);
		if (Tree.iter != Tree.Nodes.end())
		{
			for (int t = 0; t < 8; t++)
			{
				if (Tree.iter->parentBoard[t] != r.getBoard()[t])
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

void QtGuiApplication3::backUp(TreeNode next, Type win)
{
	//if (Tree.iter == Tree.Nodes.end()) return;
	for (int i = 0;; i++)
	{
		TreeNode temp = TreeNode(next.playing, next.Board, next.Board , 0, 0, i);
		Tree.iter = Tree.Nodes.find(temp);
		if (Tree.iter != Tree.Nodes.end())
		{
			//Tree.Nodes.erase(Tree.iter);
			TreeNode newNode = TreeNode(next.playing, next.Board, (unsigned short int*)Tree.iter->parentBoard, Tree.iter->w , Tree.iter->n + 1 , i);
			if (win == next.playing)
			{
				newNode.w++;
			}
			//cout <<"ceshi ::" <<  newNode.w  << " "<< newNode.n << endl;
			Tree.Nodes.erase(temp);
			Tree.Nodes.insert(newNode);
			if (newNode.parentBoard[0] == 0xFFFF) return;
			Type pType = (next.playing == BLACK) ? WHITE : BLACK;
			TreeNode parentNode = TreeNode(pType, newNode.parentBoard , newNode.parentBoard, 0 ,0 , 0);
			backUp(parentNode, win);
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

