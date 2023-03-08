
#pragma once
#include <QtGui> 
#include<qtreeview.h>
#include<QtWidgets/qmainwindow.h>

class TreeView :public QTreeView
{
    Q_OBJECT;
public:
     TreeView(QWidget* parent = Q_NULLPTR);
     void iterateOverItems();
     void NewProject(QString, QString);
     void init_tree();
     void init_mould();
     QList<QStandardItem*> returnTheItems();
     QStandardItemModel* model;
     //QItemSelectionModel* modelSelection;
     //File_Path file_path[10];
     void mouseDoubleClickEvent(QMouseEvent* event);
public slots:
    void Import(QString, QString, QString);
    void slotCustomContextMenu(const QPoint&);
    /*ж�ع�����Ӧ����*/
    void Unload();
private:
    int num_pro;
    int type;//1�����Ϲ�������2������ģ��
signals:
    void sendindex(QModelIndex);
    /*����treeview*/
    void update();

 }; 
