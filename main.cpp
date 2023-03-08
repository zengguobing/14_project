#include"QtGui"
#include<ColorBar.h>
#include <QtWidgets/QApplication>
#include"MainWindow.h"
#include"qheaderview.h"
#include <QPixmap>
#include <QSplashScreen>
#include<string>
#include<icon_source.h>
//#include<QStyleFactory>
int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    QPixmap* k = new QPixmap(QString(CURSOR_UP_ICON));
    /*������������*/
    char szFilePath[MAX_PATH + 1] = { 0 };
    GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
    (strrchr(szFilePath, '\\'))[0] = 0;
    string exe_path(szFilePath);
    exe_path = exe_path.append("\\icon\\guide.png");
    QPixmap pixmap(exe_path.c_str());
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage(QStringLiteral("�������������Ժ�......"), Qt::AlignHCenter | Qt::AlignBottom, Qt::white);
    QDateTime n = QDateTime::currentDateTime();
    QDateTime now;
    do {
        now = QDateTime::currentDateTime();
    } while (n.secsTo(now) <= 1);//3Ϊ��Ҫ��ʱ������
    /*ʹ��������ʾ���������ͬʱ������Ӧ���������¼�*/
    a.processEvents();
    //if (argc == 3)
    //{
    //    MainWindow b(QString(argv[1]), nullptr);
    //    b.show();
    //    splash.finish(&b);
    //}
    //else
    //{
    //    MainWindow b;
    //    b.show();
    //    splash.finish(&b);
    //}
    MainWindow* b = NULL;
    if (argc == 2)
    {
        b = new MainWindow(argv[1], nullptr);
    }
    else
    {
        b = new MainWindow(nullptr);
    }
    //b->setAttribute(Qt::WA_QuitOnClose);
    b->show();
    splash.finish(b);
    return a.exec();
}