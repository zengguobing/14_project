#include<iostream>
/*���溯����*/
#include"Baseline.h"
#include<Deformation_Average.h>
#include "MainWindow.h"
#include"OpenProject.h"
#include"NewProject.h"
#include"Import_TSX.h"
#include"import_sentinel.h"
#include"Cut.h"
#include"Registration_ui.h"
#include"S1_TOPS_BackGeocoding.h"
#include"S1_Deburst.h"
#include"Coordinate.h"
#include"Interferometric_Formation.h"
#include"Filter_ui.h"
#include"treeview.h"
#include"ImageView.h"
#include"MyThread.h"
#include"Unwrap_ui.h"
#include"Dem_ui.h"
#include"SLC_deramp.h"
#include"Baseline_Formation.h"
#include"SBAS_time_series_analysis.h"
#include"SBAS_reference_reselection.h"
#include<Export_KML.h>
#include"Geocoding.h"
#include"S1_swath_merge.h"
#include"S1_frame_merge.h"
#include"import_CSK.h"
#include"import_ALOS2.h"
#include"import_RepeatPass.h"
#include"import_SingleTransDoubleRecv.h"
#include"import_PingPong.h"
#include"import_DualFreqPingPong.h"
#include"icon_source.h"
//#include<Mould.h>

/*Qt������*/
#include<QtGui>
#include<qfiledialog.h>
#include<qdialog.h>
#include<qsplitter.h>
#include<qstring.h>
#include<qgraphicsitem.h>
#include<qmessagebox.h>
#include<qdialogbuttonbox.h>
/*���溯����*/
//#include<FormatConversion.h>
//#include<Utils.h>
/*opencv������*/
#include<opencv2/highgui.hpp>

//#ifdef _DEBUG
//#pragma comment(lib, "Utils_d.lib")
//#pragma comment(lib, "FormatConversion_d.lib")
//#pragma comment(lib, "ComplexMat_d.lib")
//#endif
using namespace cv;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    if (!this->Process)
    {
        Process = NULL;
    }
    ui.setupUi(this);
    this->project = new XMLFile;
    this->double_click_open_project_file = "";
    this->b_open_throug_dbclk = false;

    /*���APPͼ��*/
    this->setWindowTitle("SatExplorer");
    this->setWindowIcon(QIcon(APP_ICON));
    ui.Process->setDisabled(1);
    ui.menuSAR->setDisabled(1);
    ui.menuInSAR->setDisabled(1);
    ui.menuDInSAR->setDisabled(1);
    ui.performance_evaluation->setDisabled(1);
    ui.system_modelling->setDisabled(1);

    ui.treeView->init_tree();
    ui.tool->init_mould();
    
    ui.tabWidget->setTabsClosable(true);
    connect(ui.treeView, SIGNAL(sendindex(QModelIndex)), this, SLOT(ShowImage(QModelIndex)));
    //connect(ui.tool, &TreeView::sendindex, this, &MainWindow::OpenMould);
    connect(ui.treeView, &TreeView::update, this, &MainWindow::update_treeview);
    connect(ui.tabWidget, &QTabWidget::currentChanged, this, &MainWindow::ShowColorBar);
}
MainWindow::MainWindow(QString str, QWidget* parent) : QMainWindow(parent)
{
    if (!this->Process)
    {
        Process = NULL;
    }
    ui.setupUi(this);
    this->project = new XMLFile;
    this->double_click_open_project_file = "";
    

    /*���APPͼ��*/
    this->setWindowTitle("SatExplorer");
    this->setWindowIcon(QIcon(APP_ICON));
    ui.treeView->init_tree();
    ui.tool->init_mould();

    ui.tabWidget->setTabsClosable(true);
    cout << ui.tabWidget->count();
    connect(ui.treeView, SIGNAL(sendindex(QModelIndex)), this, SLOT(ShowImage(QModelIndex)));
    //connect(ui.tool, &TreeView::sendindex, this, &MainWindow::OpenMould);
    connect(ui.treeView, &TreeView::update, this, &MainWindow::update_treeview);
    this->open_from_project_file(str);
}
MainWindow::~MainWindow()

{
    if (!this->Process)
    {
        delete(Process);
        Process = NULL;
    }
    this->thread = NULL;
    if (this->project)
    {
        delete this->project;
        this->project = NULL;
    }
}
//void MainWindow::OpenMould(QModelIndex index)
//{
//    if (ui.tool->model->itemFromIndex(index)->toolTip() == "DEM")
//    {
//        Mould* mould = new Mould;
//        connect(this, &MainWindow::sendModel, mould, &Mould::ReceiveModel);
//        emit sendModel(ui.treeView->model);
//        mould->show();
//        connect(mould, &Mould::sendCopy, this, &MainWindow::RenewTree);
//        mould->setAttribute(Qt::WA_DeleteOnClose, true);
//    }
//}
void MainWindow::Addproject(QString name, QString save_path)
{
    ui.treeView->NewProject(name, save_path);
    RenewTree(ui.treeView->model);
}
void MainWindow::resizeEvent(QResizeEvent* event)
{
    if (ui.tabWidget->count())
    {
        int index = ui.tabWidget->currentIndex();
        if (mExist_Color.at(index))
        {
            mColors.at(index)->resize(ui.tabWidget->currentWidget()->width() / 10, ui.tabWidget->currentWidget()->height() / 5);
            mColors.at(index)->move(ui.tabWidget->currentWidget()->mapToGlobal(QPoint(0,0)));
        }
    }
}
void MainWindow::updateProcess(int value, QString information)
{
    this->Process->setValue(value);
    this->Process->setLabelText(information);
    QThread::currentThread()->msleep(1);

}
void MainWindow::endProcess()
{
        Process->setValue(100);
        waitKey(100);
        if (!this->Process)
        {
            delete(Process);
            Process = NULL;
        }
        Loading(mData_path, mType);
        thread->thread()->quit();
        thread->thread()->wait();
}
void MainWindow::endThread()
{
    thread->thread()->quit();
    thread->thread()->wait();
    //thread->thread()->deleteLater();
}
void MainWindow::StopThread()
{
    //qDebug() << "Close thread id: " << thread->thread()->isFinished();
   // qDebug() << "Status: " << thread->thread()->isInterruptionRequested();
    if(this->thread != NULL)
        if (this->thread->thread()->isRunning())
    {
        thread->thread()->requestInterruption();
        thread->thread()->quit();
        thread->thread()->wait();
    }
   
}
void MainWindow::Loading(QString Data_path, QString ImageType)
{
    QGridLayout* TabLayout = new QGridLayout;
    QWidget* TabChild = new QWidget;
    int index = ui.tabWidget->addTab(TabChild, bmp_name);
    ui.tabWidget->setCurrentWidget(TabChild);
    
    
    ImageView* graph = new ImageView(TabChild);
    TabLayout->addWidget(graph);
    TabLayout->setContentsMargins(0, 0, 0, 0);
    QGraphicsScene* scene = new QGraphicsScene;
    graph->setScene(scene);
    graph->setInteractive(true);
    graph->setDragMode(QGraphicsView::RubberBandDrag);
    graph->setRubberBandSelectionMode(Qt::ContainsItemShape);
    QImage Qimg = QImage(this->bmp_path);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(Qimg));
    item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    item->setAcceptedMouseButtons(Qt::LeftButton);
    scene->addItem(item);
    TabChild->setLayout(TabLayout);
    TabChild->setAttribute(Qt::WA_DeleteOnClose);
    /// progressdialog.setValue(100);
    // progressdialog.autoClose();
    graph->show();
    ColorBar* Color_Label = new ColorBar(TabChild);
    int ret = Color_Label->SetData(Data_path, ImageType);
    if(ret == 0)
    {
        QPoint globalpos = TabChild->mapToGlobal(QPoint(0, 0));
        Color_Label->move(globalpos.x(), globalpos.y());
        Color_Label->show();
        mExist_Color.append(true);
    }
    else
    {
        mExist_Color.append(false);
    }
    mColors.append(Color_Label);
}
void MainWindow::open_from_project_file(QString str)
{
    
    QString filename = str;
    QFileInfo fileinfo = QFileInfo(filename);
    QString abs_path = fileinfo.absolutePath();
    model = ui.treeView->model;
    int ret = this->project->XMLFile_load(filename.toStdString().c_str());
    if (ret < 0)
    {
        return;
    }
    TiXmlElement* root;
    ret = this->project->get_root(root);
    TiXmlElement* p, * q, * j;
    QList<QString> origin_name;
    if (!root->NoChildren())
    {
        p = root->FirstChildElement();
        if (!strcmp(p->Value(), "project_info"))
        {
            q = p->FirstChildElement();
            QString project_name = q->Value();
            QStandardItem* Project = new QStandardItem;
            QStandardItem* Project_Path = new QStandardItem;
            Project->setIcon(QIcon(PROJECT_ICON));
            Project->setStatusTip(NOT_IN_PROCESS);
            if (!strcmp(q->Value(), "project_name"))
            {
                String x(q->GetText());
                Project->setText(x.c_str());
            }
                
            q = q->NextSiblingElement();
            if (!strcmp(q->Value(), "project_path"))
                if (!strcmp(abs_path.toStdString().c_str(), q->GetText()))
                    Project_Path->setText(q->GetText());
                else
                {
                    Project_Path->setText(abs_path.toStdString().c_str());
                    q->Clear(); q->LinkEndChild(new TiXmlText(abs_path.toStdString().c_str()));//���¾���·��
                }
            model->appendRow(Project);
            model->setItem(model->rowCount() - 1, 1, Project_Path);
            for (p = p->NextSiblingElement(); p != NULL; p = p->NextSiblingElement())
            {
                QStandardItem* Data_Node = new QStandardItem;
                Data_Node->setText(p->Attribute("name"));
                Data_Node->setToolTip(Project->text());
                Data_Node->setIcon(QIcon(FOLDER_ICON));
                Project->appendRow(Data_Node);
                QStandardItem* Rank = new QStandardItem(p->Attribute("rank"));
                Project->setChild(Project->rowCount() - 1, 1, Rank);
                int i = 0;
                int count = 0;
                for (q = p->FirstChildElement(); q != NULL && strcmp(q->Value(), "Data") == 0; q = q->NextSiblingElement(), i++)
                {
                    QStandardItem* Data = new QStandardItem;
                    QStandardItem* Data_Path = new QStandardItem;

                    for (j = q->FirstChildElement(); j != NULL; j = j->NextSiblingElement())
                    {

                        if (!strcmp(j->Value(), "Data_Name"))
                        {
                            Data->setText(j->GetText());
                        }
                        else if (!strcmp(j->Value(), "Data_Rank"))
                        {
                            int mode, ret;
                            double level;
                            ret = sscanf(j->GetText(), "%d-complex-%lf", &mode, &level);
                            if (ret == 2)
                            {
                                Data->setToolTip("complex");
                            }
                            ret = sscanf(j->GetText(), "%d-phase-%lf", &mode, &level);
                            if (ret == 2)
                            {
                                Data->setToolTip("phase");
                            }
                            ret = sscanf(j->GetText(), "%d-coherence-%lf", &mode, &level);
                            if (ret == 2)
                            {
                                Data->setToolTip("coherence");
                            }
                            ret = sscanf(j->GetText(), "%d-dem-%lf", &mode, &level);
                            if (ret == 2)
                            {
                                Data->setToolTip("dem");
                            }

                        }
                        else if (!strcmp(j->Value(), "Data_Path"))
                        {
                            Data_Path->setText(abs_path + QString(j->GetText()));
                        }

                    }
                    Data_Node->appendRow(Data);
                    Data->setIcon(QIcon(IMAGEDATA_ICON));
                    Data_Node->setChild(i, 1, Data_Path);
                }

            }
            //model->setHeaderData(0, Qt::Horizontal, tr("workspace"));
            this->RenewTree(model);
        }
        this->project->XMLFile_save(str.toStdString().c_str());
    }
    else
        QMessageBox::warning(NULL, "Warning!", "*.Insar is empty!");
    close();
}
void MainWindow::update_treeview()
{
    if (ui.treeView->model)
    {
        if (ui.treeView->model->rowCount() < 1)
        {
            ui.system_modelling->setDisabled(1);
            ui.performance_evaluation->setDisabled(1);
            ui.menuSAR->setDisabled(1);
            ui.Process->setDisabled(1);
            ui.menuInSAR->setDisabled(1);
            ui.menuDInSAR->setDisabled(1);
        }
        else
        {
            ui.system_modelling->setDisabled(0);
            ui.performance_evaluation->setDisabled(0);
            ui.menuSAR->setDisabled(0);
            ui.Process->setDisabled(0);
            ui.menuInSAR->setDisabled(0);
            ui.menuDInSAR->setDisabled(0);
        }
    }
}

bool MainWindow::eventFilter(QObject* target, QEvent* event)
{
    if (target == ui.tabWidget)
    {
        if (event->type() == QEvent::Resize || event->type() == QEvent::Move)
        {
            if (mColors.size())
                mColors.at(ui.tabWidget->currentIndex())->move(ui.tabWidget->currentWidget()->mapToGlobal(QPoint(0, 0)));
        }
    }
    if (target == this)
    {
        if (event->type() == QEvent::Move)
        {
            if (mColors.size())
                mColors.at(ui.tabWidget->currentIndex())->move(ui.tabWidget->currentWidget()->mapToGlobal(QPoint(0, 0)));
        }
    }
    return false;
}

void MainWindow::ShowImage(QModelIndex image)
{
    
    QString name = ui.treeView->model->index(image.row(), 0, image.parent()).data().toString();
    QString path = ui.treeView->model->index(image.row(), 1, image.parent()).data().toString();
    QString type = ui.treeView->model->itemFromIndex(ui.treeView->model->index(image.row(),0,image.parent()))->toolTip();
    if (!path.isEmpty())
    {
        QFileInfo fileinfo = QFileInfo(path);
        QString bmp = QString("%1/%2.jpg").arg(fileinfo.absolutePath()).arg(fileinfo.baseName());
        QString path_abs = QString("%1%2%3%4").arg(fileinfo.absolutePath()).arg("/").arg(name).arg(".jpg");
        this->bmp_name = name;
        this->bmp_path = path_abs;

        QFileInfo fileinfo1 = QFileInfo(path_abs);
        QDir dir(fileinfo1.absolutePath());
        if (dir.exists(fileinfo1.baseName() + ".jpg"))
        {
            mData_path = path;
            mType = type;
            Loading(mData_path, mType);
        }
        else
        {
            if(!image.child(0,0).isValid() && image.parent().isValid())
            {
                mData_path = path;
                mType = type;
                thread = new MyThread;
                thread->moveToThread(new QThread(this));
                this->Process = new QProgressDialog("Loading Image...", "Cancel", 0, 100);
                //this->Process->setAutoClose(true);
                this->Process->setValue(0);
                this->Process->show();
                waitKey(100);
                connect(this, &MainWindow::operate, thread, &MyThread::ShowImage);
                connect(thread, &MyThread::updateProcess, this, &MainWindow::updateProcess);
                connect(thread->thread(), &QThread::finished, thread, &MyThread::deleteLater);
                connect(thread, &MyThread::endProcess, this, &MainWindow::endProcess);
                connect(this->Process, &QProgressDialog::destroyed, this, &MainWindow::StopThread);
                connect(this->Process, &QProgressDialog::canceled, this, &MainWindow::StopThread);// , Qt::QueuedConnection);
                thread->thread()->start();
                emit operate(path, path_abs, type);

            }
            
        }
    }
}
void MainWindow::on_actionNew_triggered()
{
    NewProject* newpro = new NewProject;
    connect(this, &MainWindow::sendModel, newpro, &NewProject::ReceiveModel);
    emit sendModel(ui.treeView->model);
    newpro->show();
    connect(newpro, &NewProject::sendPath, this, &MainWindow::Addproject);
    newpro->setAttribute(Qt::WA_DeleteOnClose, true);
    
}
void MainWindow::on_actionOpen_triggered()
{
    OpenProject* open_Window = new OpenProject;
    open_Window->show();
    connect(this, &MainWindow::sendModel, open_Window, &OpenProject::LoadModel);
    emit sendModel(ui.treeView->model);
    connect(open_Window, &OpenProject::sendModel, this, &MainWindow::RenewTree);
    open_Window->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actionRepeatPass_triggered()
{
   
    import_RepeatPass* RepeatPass = new import_RepeatPass();
    connect(this, &MainWindow::sendModel, RepeatPass, &import_RepeatPass::ShowProjectList);
    emit sendModel(ui.treeView->model);
    RepeatPass->show();
    
    connect(RepeatPass, &import_RepeatPass::sendCopy, this, &MainWindow::RenewTree);
    RepeatPass->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actionSingleTransDoubleRecv_triggered()
{
    import_SingleTransDoubleRecv* SingleTransDoubleRecv = new import_SingleTransDoubleRecv;
    connect(this, &MainWindow::sendModel, SingleTransDoubleRecv, &import_SingleTransDoubleRecv::ShowProjectList);
    emit sendModel(ui.treeView->model);
    SingleTransDoubleRecv->show();

    connect(SingleTransDoubleRecv, &import_SingleTransDoubleRecv::sendCopy, this, &MainWindow::RenewTree);
    SingleTransDoubleRecv->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actionCut_triggered()
{
    Cut* cut = new Cut();
    connect(this, &MainWindow::sendModel, cut, &Cut::ShowProjectList);
    emit sendModel(ui.treeView->model);
    cut->show();
    connect(cut, &Cut::sendCopy, this, &MainWindow::RenewTree);
    cut->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actionRegistration_triggered()
{
    Registration_ui* regis = new Registration_ui();
    connect(this, &MainWindow::sendModel, regis, &Registration_ui::ShowProjectList);
    emit sendModel(ui.treeView->model);
    regis->show();
    connect(regis, &Registration_ui::sendCopy, this, &MainWindow::RenewTree);
    regis->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actionS1_TOPS_BackGeocoding_triggered()
{
    S1_TOPS_BackGeocoding* backGeocoding = new S1_TOPS_BackGeocoding();
    connect(this, &MainWindow::sendModel, backGeocoding, &S1_TOPS_BackGeocoding::ShowProjectList);
    emit sendModel(ui.treeView->model);
    backGeocoding->show();
    connect(backGeocoding, &S1_TOPS_BackGeocoding::sendCopy, this, &MainWindow::RenewTree);
    backGeocoding->setAttribute(Qt::WA_DeleteOnClose, true);
}

void MainWindow::on_actionS1_Deburst_triggered()
{
    S1_Deburst* deburst = new S1_Deburst();
    connect(this, &MainWindow::sendModel, deburst, &S1_Deburst::ShowProjectList);
    emit sendModel(ui.treeView->model);
    deburst->show();
    connect(deburst, &S1_Deburst::sendCopy, this, &MainWindow::RenewTree);
    deburst->setAttribute(Qt::WA_DeleteOnClose, true);
}

void MainWindow::on_actionSBAS_deformation_triggered()
{
    SBAS_time_series_analysis* SBAS_time_series = new SBAS_time_series_analysis();
    connect(this, &MainWindow::sendModel, SBAS_time_series, &SBAS_time_series_analysis::ShowProjectList);
    emit sendModel(ui.treeView->model);
    SBAS_time_series->show();
    connect(SBAS_time_series, &SBAS_time_series_analysis::sendCopy, this, &MainWindow::RenewTree);
    SBAS_time_series->setAttribute(Qt::WA_DeleteOnClose, true);
}

void MainWindow::on_actionDeformation_Preview_triggered()
{
    Deformation_Average* bl = new Deformation_Average();
    connect(this, &MainWindow::sendModel, bl, &Deformation_Average::ShowProjectList);
    emit sendModel(ui.treeView->model);
    bl->show();
    bl->setAttribute(Qt::WA_DeleteOnClose, true);
}

void MainWindow::on_actionreference_re_selection_triggered()
{
    SBAS_reference_reselection* bl = new SBAS_reference_reselection();
    connect(this, &MainWindow::sendModel, bl, &SBAS_reference_reselection::ShowProjectList);
    emit sendModel(ui.treeView->model);
    bl->show();
    bl->setAttribute(Qt::WA_DeleteOnClose, true);
}

void MainWindow::on_actionExport_KML_triggered()
{
    Export_KML* bl = new Export_KML();
    connect(this, &MainWindow::sendModel, bl, &Export_KML::ShowProjectList);
    emit sendModel(ui.treeView->model);
    bl->show();
    bl->setAttribute(Qt::WA_DeleteOnClose, true);
}

void MainWindow::on_actionBaseline_Preview_triggered()
{
    Baseline* bl = new Baseline();
    connect(this, &MainWindow::sendModel, bl, &Baseline::ShowProjectList);
    emit sendModel(ui.treeView->model);
    bl->show();
    bl->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actionSLC_deramp_triggered()
{
    SLC_deramp* deramp = new SLC_deramp();
    connect(this, &MainWindow::sendModel, deramp, &SLC_deramp::ShowProjectList);
    emit sendModel(ui.treeView->model);
    deramp->show();
    deramp->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actionBaseline_Formation_triggered()
{
    Baseline_Formation* BF = new Baseline_Formation();
    connect(this, &MainWindow::sendModel, BF, &Baseline_Formation::ShowProjectList);
    emit sendModel(ui.treeView->model);
    BF->show();
    BF->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actionInterferometric_Formation_triggered()
{
    Interferometric_Formation* IF = new Interferometric_Formation();
    connect(this, &MainWindow::sendModel, IF, &Interferometric_Formation::ShowProjectList);
    emit sendModel(ui.treeView->model);
    IF->show();
    connect(IF, &Interferometric_Formation::sendCopy, this, &MainWindow::RenewTree);
    IF->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actionDenoise_triggered()
{
    Filter_ui *Denoise = new Filter_ui();
    connect(this, &MainWindow::sendModel, Denoise, &Filter_ui::ShowProjectList);
    emit sendModel(ui.treeView->model);
    Denoise->show();
    connect(Denoise, &Filter_ui::sendCopy, this, &MainWindow::RenewTree);
    Denoise->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actionUnwrap_triggered()
{
    Unwrap_ui* unwrap = new Unwrap_ui();
    connect(this, &MainWindow::sendModel, unwrap, &Unwrap_ui::ShowProjectList);
    emit sendModel(ui.treeView->model);
    unwrap->show();
    connect(unwrap, &Unwrap_ui::sendCopy, this, &MainWindow::RenewTree);
    unwrap->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actionDEM_triggered()
{
    Dem_ui* Dem = new Dem_ui();
    connect(this, &MainWindow::sendModel, Dem, &Dem_ui::ShowProjectList);
    emit sendModel(ui.treeView->model);
    Dem->show();
    connect(Dem, &Dem_ui::sendCopy, this, &MainWindow::RenewTree);
    Dem->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actiongeocode_triggered()
{
    Geocoding* geocode = new Geocoding();
    connect(this, &MainWindow::sendModel, geocode, &Geocoding::ShowProjectList);
    emit sendModel(ui.treeView->model);
    geocode->show();
    connect(geocode, &Geocoding::sendCopy, this, &MainWindow::RenewTree);
    geocode->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actionS1_swath_merge_triggered()
{
    S1_swath_merge* swath_merge = new S1_swath_merge();
    connect(this, &MainWindow::sendModel, swath_merge, &S1_swath_merge::ShowProjectList);
    emit sendModel(ui.treeView->model);
    swath_merge->show();
    connect(swath_merge, &S1_swath_merge::sendCopy, this, &MainWindow::RenewTree);
    swath_merge->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actionS1_frame_merge_triggered()
{
    S1_frame_merge* frame_merge = new S1_frame_merge();
    connect(this, &MainWindow::sendModel, frame_merge, &S1_frame_merge::ShowProjectList);
    emit sendModel(ui.treeView->model);
    frame_merge->show();
    connect(frame_merge, &S1_frame_merge::sendCopy, this, &MainWindow::RenewTree);
    frame_merge->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actionPingPong_triggered()
{
    import_PingPong* pingpong = new import_PingPong;
    connect(this, &MainWindow::sendModel, pingpong, &import_PingPong::ShowProjectList);
    emit sendModel(ui.treeView->model);
    pingpong->show();

    connect(pingpong, &import_PingPong::sendCopy, this, &MainWindow::RenewTree);
    pingpong->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::on_actionDualFreqPingPong_triggered()
{
    import_DualFreqPingPong* dualpingpong = new import_DualFreqPingPong;
    connect(this, &MainWindow::sendModel, dualpingpong, &import_DualFreqPingPong::ShowProjectList);
    emit sendModel(ui.treeView->model);
    dualpingpong->show();

    connect(dualpingpong, &import_DualFreqPingPong::sendCopy, this, &MainWindow::RenewTree);
    dualpingpong->setAttribute(Qt::WA_DeleteOnClose, true);
}
void MainWindow::RenewTree(QStandardItemModel* copy)
{
    
    if (ui.treeView->isHidden())
    {
        ui.treeView->setHidden(0);
        ui.tool->setHidden(0);
        ui.tabWidget->setHidden(0);
    }
    if (!ui.system_modelling->isEnabled())
        ui.system_modelling->setDisabled(0);
    if (!ui.performance_evaluation->isEnabled())
        ui.performance_evaluation->setDisabled(0);
    if(!ui.Process->isEnabled())
        ui.Process->setDisabled(0);
    if (!ui.menuInSAR->isEnabled())
        ui.menuInSAR->setDisabled(0);
    if (!ui.menuDInSAR->isEnabled())
        ui.menuDInSAR->setDisabled(0);
    ui.treeView->setColumnHidden(1, 1);
    ui.treeView->setModel(copy);
}
void MainWindow::ShowColorBar(int index)
{
    if ( ui.tabWidget->count()== mExist_Color.size())
    {
        if (ui.tabWidget->count() - 1 >= index)
        {
            cout << ui.tabWidget->count();
            cout << "\n" << "new";
            if (mExist_Color.at(index))
            {

                mColors.at(index)->resize(ui.tabWidget->currentWidget()->width() / 5, ui.tabWidget->currentWidget()->height() / 3);
                mColors.at(index)->move(ui.tabWidget->currentWidget()->mapToGlobal(QPoint(0, 0)));
                mColors.at(index)->raise();
                mColors.at(index)->show();
            }
        }
       
        if (ColorBar_Before >= 0)
        {
            if (mColors.size())
            {
                cout << ui.tabWidget->count();
                cout << "\n" << "hide";
                mColors.at(ColorBar_Before)->hide();
            }

        }
        cout << ui.tabWidget->count();
        cout << "\n" << "change";
        
    }
    if (TabCount_Before >= 0 && TabCount_Before< ui.tabWidget->count())
    {
        if (mColors.size())
        {
            cout << ui.tabWidget->count();
            cout << "\n" << "hide";
            mColors.at(ColorBar_Before)->hide();
        }
    }
    TabCount_Before = ui.tabWidget->count();
    ColorBar_Before = index;
}

bool MainWindow::CheckTab(QModelIndex image)
{
    int n = ui.tabWidget->count();
    int i = 0;
    QString name = ui.treeView->model->index(image.row(), 0, image.parent()).data().toString();
    for (i = 0; i < n; i++)
    {
        if (!QString::compare(ui.tabWidget->tabText(i), name))
        {
            ui.tabWidget->setCurrentIndex(i);
            return true;
        }
    }
       
    return false;
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    if (ui.tabWidget->widget(index))
    {
        cout << ui.tabWidget->count();
        cout << "\n" << "close";
       // ui.tabWidget->removeTab(index);
        delete(ui.tabWidget->widget(index));
        mColors.removeAt(index);
        mExist_Color.removeAt(index);
        if (ui.tabWidget->currentIndex() >= 0)
        {
            if (mExist_Color.at(ui.tabWidget->currentIndex()))
            {
                mColors.at(ui.tabWidget->currentIndex())->resize(ui.tabWidget->currentWidget()->width() / 5, ui.tabWidget->currentWidget()->height() / 3);
                mColors.at(ui.tabWidget->currentIndex())->move(ui.tabWidget->currentWidget()->mapToGlobal(QPoint(0, 0)));
                mColors.at(ui.tabWidget->currentIndex())->raise();
                mColors.at(ui.tabWidget->currentIndex())->show();
            }
            
        }
        
    }
}