#include"MylistWidget.h"
#include<QMenu>
#include<QString>
#include <QDebug>
#include<QAction>
MyListWidget::MyListWidget()
{
	finfo = new QAction(this);
	
	finfo->setText(QString::fromLocal8Bit("�鿴�ļ���Ϣ"));

	
}
MyListWidget::~MyListWidget()
{
}
void MyListWidget::contextMenuEvent(QContextMenuEvent* ev)
{
	QMenu* rMenu = new QMenu(this);
	if (this->itemAt(mapFromGlobal(QCursor::pos())) != NULL)
	{
		rMenu->addAction(finfo);
	    musicItem = this->itemAt(mapFromGlobal(QCursor::pos()));
		qDebug() << musicItem->text();

	}
	
	rMenu->exec(QCursor::pos());//�˵������ڵ�ǰ����λ�á�
}
