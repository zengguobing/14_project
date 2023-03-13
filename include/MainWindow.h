#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include<ColorBar.h>
#include<qgraphicsscene.h>
#include"qprogressdialog.h"
#include"MyThread.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    MyThread* thread;
public:
    MainWindow(QWidget* parent = Q_NULLPTR);
    MainWindow(QString str, QWidget* parent = Q_NULLPTR);
    ~MainWindow();
    void Addproject(QString,QString);
    void resizeEvent(QResizeEvent* event);
public slots:
    void ShowImage(QModelIndex);
    void updateProcess(int, QString);
    void endProcess();
    void endThread();
    void StopThread();
    void Loading(QString Data_path, QString ImageType);
    /*用于双击工程文件打开程序并加载该工程*/
    void open_from_project_file(QString str);
    /*更新treeview*/
    void update_treeview();
protected:
    bool eventFilter(QObject*, QEvent*);
private:
    Ui::MainWindow ui;
    QLabel* mColorbar_Layer;
    QProgressDialog *Process;
    QString bmp_path;
    QString bmp_name;

    /*用于显示图例*/
    QString mData_path;
    QString mType;
    QList<ColorBar*> mColors;
    QList<bool> mExist_Color;
    int ColorBar_Before = -1;
    int TabCount_Before = -1;

    QTimer *t;
    XMLFile* project;
    QStandardItemModel* model;
    /*用于双击工程文件打开程序并加载该工程*/
    QString double_click_open_project_file;
    bool b_open_throug_dbclk = false;
signals:
    void sendModel(QStandardItemModel*);
    void operate(QString, QString, QString);
private slots:
    bool CheckTab(QModelIndex);
    //void OpenMould(QModelIndex);
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionRepeatPass_triggered();
    /*导入sentinel数据响应函数*/
    void on_actionSingleTransDoubleRecv_triggered();
    void on_actionCut_triggered();
    void on_actionRegistration_triggered();
    void on_actionS1_TOPS_BackGeocoding_triggered();
    void on_actionS1_Deburst_triggered();
    void on_actionSBAS_deformation_triggered();
    void on_actionDeformation_Preview_triggered();
    void on_actionreference_re_selection_triggered();
    void on_actionExport_KML_triggered();
    void on_actionBaseline_Preview_triggered();
    void on_actionSLC_deramp_triggered();
    void on_actionqucankao_triggered();
    void on_actionBaseline_Formation_triggered();
    void on_actionInterferometric_Formation_triggered();
    void on_actionDenoise_triggered();
    void on_actionUnwrap_triggered();
    void on_actionDEM_triggered();
    void on_actiongeocode_triggered();
    void on_actionS1_swath_merge_triggered();
    void on_actionS1_frame_merge_triggered();
    void on_actionPingPong_triggered();
    void on_actionDualFreqPingPong_triggered();
    void on_tabWidget_tabCloseRequested(int);
    void RenewTree(QStandardItemModel*);
    /*切换ColorBar*/
    void ShowColorBar(int index);
};