#pragma once
#include <QtWidgets/QMainWindow>
#include<qstandarditemmodel.h>
#include "ui_interferogram_generation.h"
#include"MyThread.h"

class interferogram_generation : public QWidget
{
    Q_OBJECT
public:
    explicit interferogram_generation(QWidget* parent = Q_NULLPTR);
    ~interferogram_generation();
public slots:
    void ShowProjectList(QStandardItemModel*);
    void updateProcess(int, QString);
    void endProcess();
    void endThread();
    void StopThread();
    void TransitModel(QStandardItemModel*);
private:
    Ui::interferogram_generation* ui;
    QStandardItemModel* copy;
    MyThread* interferogram_generation_thread;
    QString save_path;
    int image_number;
    void ChangeVision(bool Editable);
signals:
    void operate(int, int, int, QString, QString, QString, QString, QStandardItemModel* model);
    void sendCopy(QStandardItemModel*);
private slots:
    void ChangeSetting();
    //void on_iscoherence_Checked();
    void on_comboBox_src_project_currentIndexChanged();
    void on_comboBox_src_node_currentIndexChanged();

    // void on_masterpushButton_pressed();
     //void on_slavepushButton_pressed()
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};