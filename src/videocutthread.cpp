#include"videocutthread.h"
#include<QThread>
#include<QProcess>
#include<QStringList>
#include<QDir>
#include<QTime>
#include<cstdlib>
#include<Windows.h>
#include<string>

void VideoCutThread::closeThread()
{
	isStop = true;
}


VideoCutThread::VideoCutThread(QObject* parent) :
	QObject(parent),
	isStop(false)
{

}

//QVector<pair<QString, QString>> time
void VideoCutThread::startWoker() {
	//��������Ŀ¼
	QString data_path = project_path + "/" + "data" + "/";
	QDir dir(data_path);
	if (!dir.exists()) {
		dir.mkdir(data_path);
	}
	QString program = "E:/learnEnglishWithFilms/bin/ffmpeg.exe";

	int i = 1;
	for (QVector<std::pair<QString, QString>>::iterator it = time.begin(); it != time.end(); it++, i++) {
		//�������ʱ��
		int s = QTime().fromString(it->first, "hh:mm:ss").secsTo(QTime().fromString(it->second, "hh:mm:ss"));
		QTime temp = QTime(0, 0, 0).addSecs(s+2);
		QString duration = temp.toString("hh:mm:ss");

		//����ļ���
		QString first = it->first;
		first = first.replace(QString(":"), QString(""));
		QString second = it->second;
		second = second.replace(QString(":"), QString(""));
		QString out = data_path + first + "-" + second + ".mp4";
		

		QString para = QString("/c ffmpeg -y -ss %1 -t %2 -i %3 -codec copy %4").arg(it->first,duration,video_path,out);

		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = (LPCWSTR)L"open";
		ShExecInfo.lpFile = (LPCWSTR)L"cmd";
		ShExecInfo.lpParameters = (LPCWSTR)para.unicode();
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_HIDE;
		ShExecInfo.hInstApp = NULL;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

		emit progress(i);
	}
}