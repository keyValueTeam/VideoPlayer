#include "Player.h"
#include <QtWidgets/QApplication>
#include "sqltest.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	int* para = new int[2];
	AVFormatContext* fmt = avformat_alloc_context();
	MediaLoader::load("test.mp3", para, fmt);
	//SQLTest q;
	Player p(nullptr);
	p.show();
	return a.exec();
}