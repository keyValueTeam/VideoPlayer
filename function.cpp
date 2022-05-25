#include "function.h"
bool isMP3(QString filePath)   //≈–∂œ «∑Ò «MP3
{
	QFileInfo fileInfo = QFileInfo(filePath);
	if (fileInfo.suffix() == "mp3")
	{
		return true;
	}
	else return false;
}
