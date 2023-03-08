#include "treeview.h"  
#include"icon_source.h"
#include<qmessagebox.h>
#include <QMenu>  
#include <QMenuBar>  
#include <QStatusBar> 
#include<FormatConversion.h>
#ifdef _DEBUG
#pragma comment(lib, "Utils_d.lib")
#pragma comment(lib, "FormatConversion_d.lib")
#else
#pragma comment(lib, "Utils.lib")
#pragma comment(lib, "FormatConversion.lib")
#endif
TreeView::TreeView(QWidget* parent) : QTreeView(parent)
{
    this->num_pro =0;
    type = 2;
     model = new QStandardItemModel(0, 2);
     //modelSelection = new QItemSelectionModel(model);
     //this->setSelectionModel(modelSelection);
     this->setModel(model);
     //this->setSelectionBehavior(QAbstractItemView::SelectItems);
     //this->setSelectionMode(QAbstractItemView::ExtendedSelection);
     this->setColumnHidden(1, true);
     this->setContextMenuPolicy(Qt::CustomContextMenu);
     connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slotCustomContextMenu(const QPoint&)));
}

void TreeView::init_tree()
{
    model->setHeaderData(0, Qt::Horizontal, tr("workspace"));
    model->setHeaderData(1, Qt::Horizontal, tr("Path"));
    type = 1;
}

void TreeView::init_mould()
{
    model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("template"));
    QStandardItem* InSAR = new QStandardItem("InSAR");
    InSAR->setIcon(QIcon(TEMPLATE_FOLDER));
    QStandardItem* DInSAR = new QStandardItem("DInSAR");
    DInSAR->setIcon(QIcon(TEMPLATE_FOLDER));
    model->appendRow(InSAR);
    model->appendRow(DInSAR);
    QStandardItem* ToDEM = new QStandardItem("DEM");
    ToDEM->setIcon(QIcon(TEMPLATE_TOOL));
    ToDEM->setToolTip("DEM");
    InSAR->appendRow(ToDEM);
    type = 2;
}

void TreeView::NewProject(QString name, QString save_path)
{
    
    QStandardItem* item0 = new QStandardItem(name + ".insar");
    item0->setIcon(QIcon(PROJECT_ICON));
    item0->setStatusTip(NOT_IN_PROCESS);
    QStandardItem* item1_path = new QStandardItem(save_path);
    model->setRowCount(model->rowCount() + 1);
    model->setItem(model->rowCount()-1, 0, item0);
    model->setItem(model->rowCount() - 1, 1, item1_path);
    XMLFile xml;
    QFileInfo info = QFileInfo(save_path);
    QString path = info.absoluteFilePath();
    xml.XMLFile_creat_new_project(path.toStdString().c_str(),QString("%1.insar").arg(name).toStdString().c_str(), "1.0");
    
}

void TreeView::Import(QString Project, QString name, QString path)
{
       
}
 QList<QStandardItem*> TreeView::returnTheItems()
 {
    return model->findItems("*", Qt::MatchWildcard | Qt::MatchRecursive);
 }

void TreeView::iterateOverItems()
 {
    QList<QStandardItem*> list = returnTheItems();

     foreach(QStandardItem * item, list) {
         qDebug() << item->text();

	}
 }

void TreeView::mouseDoubleClickEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        if (model->rowCount() >= 1)
        {
            emit sendindex(currentIndex());
        }
	}
    else
    {
        
    }
 }

void TreeView::slotCustomContextMenu(const QPoint& point) //槽函数定义
{
    if (type == 1)
    {
        if (!model->itemFromIndex(this->currentIndex())->parent())//工程节点菜单栏
        {
            QMenu* menu = new QMenu(this);
            QAction* unload = new QAction(QString::fromLocal8Bit("卸载工程"));
            menu->addAction(unload);
            connect(unload, SIGNAL(triggered()), this, SLOT(Unload()));
            menu->exec(this->mapToGlobal(point));
        }
        //图像数据节点菜单栏
        else if (!model->itemFromIndex(this->currentIndex())->hasChildren() && model->itemFromIndex(this->currentIndex())->parent())
        {
            QMenu* menu = new QMenu(this);
            QAction* image_saveas = new QAction(QString::fromLocal8Bit("另存为"));
            image_saveas->setIcon(QIcon(EXPORT_ICON));
            menu->addAction(image_saveas);
            //connect(image_saveas, SIGNAL(triggered()), this, SLOT(image_saveas()));
            menu->exec(this->mapToGlobal(point));
        }
        
    }
}

void TreeView::Unload()
{
    if (model->itemFromIndex(this->currentIndex())->statusTip() == NOT_IN_PROCESS)
    {
        model->removeRow(model->itemFromIndex(this->currentIndex())->row());
        emit update();
    }
    else
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("该工程正在处理中，无法卸载！"));
    }
}
