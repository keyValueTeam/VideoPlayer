#include "Player.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	Player p(NULL);
	p.show();
	return a.exec();
}