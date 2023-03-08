
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
    /*卸载工程响应函数*/
    void Unload();
private:
    int num_pro;
    int type;//1：左上工程树，2：左下模板
signals:
    void sendindex(QModelIndex);
    /*更新treeview*/
    void update();

 }; 
