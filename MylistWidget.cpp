#include"MylistWidget.h"
#include<QMenu>
#include<QString>
#include <QDebug>
#include<QAction>
MyListWidget::MyListWidget()
{
	finfo = new QAction(this);
	
	finfo->setText(QString::fromLocal8Bit("查看文件信息"));

	
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
	
	rMenu->exec(QCursor::pos());//菜单出现在当前鼠标的位置。
}
