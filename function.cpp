#include "function.h"
bool isMP3(QString filePath)   //�ж��Ƿ���MP3
{
	QFileInfo fileInfo = QFileInfo(filePath);
	if (fileInfo.suffix() == "mp3")
	{
		return true;
	}
	else return false;
}
