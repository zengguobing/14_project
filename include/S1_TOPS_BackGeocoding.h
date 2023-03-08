#pragma once
#include <QtWidgets/QMainWindow>
#include<qstandarditemmodel.h>
#include "ui_S1_TOPS_BackGeocoding.h"
#include"MyThread.h"

class S1_TOPS_BackGeocoding : public QWidget
{
    Q_OBJECT
public:
    explicit S1_TOPS_BackGeocoding(QWidget* parent = Q_NULLPTR);
    ~S1_TOPS_BackGeocoding();
public slots:
    void ShowProjectList(QStandardItemModel*);
    void updateProcess(int, QString);
    void endProcess();
    void endThread();
    void StopThread();
    void TransitModel(QStandardItemModel*);
private:
    Ui::S1_TOPS_BackGeocoding* ui;
    QStandardItemModel* copy;
    MyThread* S1_TOPS_BackGeocoding_thread;
    QString save_path;
    QString projectFile;
    int image_number;
    void ChangeVision(bool Editable);
signals:
    void operate(int, int, QString, QString, QString, QString, QStandardItemModel*, bool);
    void sendCopy(QStandardItemModel*);
private slots:
    /*����ѡ��ť��Ӧ����*/
    void on_comboBox_currentIndexChanged();
    /*���ݽڵ�ѡ��ť��Ӧ����*/
    void on_comboBox_2_currentIndexChanged();
    /*��ͼ��ѡ��ť��Ӧ����*/
    void on_comboBox_3_currentIndexChanged();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};