#pragma once
#include <QtWidgets/QMainWindow>
#include<qstandarditemmodel.h>
#include "ui_Registration_ui.h"
#include"MyThread.h"

class Registration_ui : public QWidget
{
    Q_OBJECT
public:
    explicit Registration_ui(QWidget* parent = Q_NULLPTR);
    ~Registration_ui();
public slots:
    void ShowProjectList(QStandardItemModel*);
    void updateProcess(int, QString);
    void endProcess();
    void endThread();
    void StopThread();
    void TransitModel(QStandardItemModel*);
private:
    Ui::Registration_ui* ui;
    QStandardItemModel* copy;
    MyThread* Registration_thread;
    QString save_path;
    QString projectFile;
    int image_number;
    void ChangeVision(bool Editable);
signals:
    void operate(QList<int>, QString, QString, QString, QString, QStandardItemModel*);
    void operate2(int, QString, QString, QString, QString, QStandardItemModel*);
    void sendCopy(QStandardItemModel*);
private slots:
    void on_comboBox_currentIndexChanged();
    /*DEM������׼����ѡ����Ӧ����*/
    void on_comboBox_project_currentIndexChanged();
    /*DEM�������ݽڵ㹤��ѡ����Ӧ����*/
    void on_comboBox_node_currentIndexChanged();
    void on_comboBox_2_currentIndexChanged();
    // void on_masterpushButton_pressed();
     //void on_slavepushButton_pressed()
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_buttonBox_2_accepted();
    void on_buttonBox_2_rejected();
};