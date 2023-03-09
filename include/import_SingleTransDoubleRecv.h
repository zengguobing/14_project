#pragma once
#include <QtWidgets/QMainWindow>
#include<qstandarditemmodel.h>
#include "ui_import_SingleTransDoubleRecv.h"
#include"MyThread.h"

class import_SingleTransDoubleRecv : public QWidget
{
    Q_OBJECT
public:
    explicit import_SingleTransDoubleRecv(QWidget* parent = Q_NULLPTR);
    ~import_SingleTransDoubleRecv();
public slots:
    void ShowProjectList(QStandardItemModel*);
    /*生成批量导入文件名*/
    //bool generate_name(QListWidget* imageslist, vector<QString>& original_nameslist, vector<QString>& import_nameslist);
    void ChangeVision(bool Editable);
private:
    Ui::import_SingleTransDoubleRecv* ui;
    QString cos_path;
    QString geo_path;
    QString xml_path;
    QString save_path;
    QStandardItemModel* copy;
    MyThread* import_SingleTransDoubleRecv_thread;
    void saveSystemSettings();
signals:
    void sendPath(QString, QString, QString);
    void operate(QString, QString, QString, QString, QString, QStandardItemModel*);
    void sendCopy(QStandardItemModel*);
private slots:
    void on_comboBox_dst_project_currentIndexChanged();
    void on_pushButton_master_pressed();
    void on_pushButton_slave_pressed();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void updateProcess(int, QString);
    void endProcess();
    void endThread();
    void StopThread();
    void TransitModel(QStandardItemModel*);
};