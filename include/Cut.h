#pragma once
#include <QtWidgets/QMainWindow>
#include<FormatConversion.h>
#include "ui_Cut.h"
#include"MyThread.h"
#include"Preview_Window.h"
#include"ImageView.h"
#include<Qtgui>

class Cut : public QWidget
{
    Q_OBJECT
public:
    explicit Cut(QWidget* parent = Q_NULLPTR);
    ~Cut();
    QStandardItemModel* copy;
public slots:
    void ShowProjectList(QStandardItemModel*);
    void updateProcess(int, QString);
    void endProcess();
    void endThread();
    void StopThread();
    void TransitModel(QStandardItemModel*);

    void ReceivePos(double left, double right, double top, double bottom);  //���ղü�����ͼ���е�λ��
    /*�ü��Ӵ��ڷ���ȡ����Ϣ*/
    void cancelled();
private:
    double h5_left, h5_right, h5_top, h5_bottom;
    /*���Ѿ������Ԥ�������ֹ�ٴε��*/
    bool isPreviewPressed;
    /*�Ƿ����ڲü�*/
    bool isCutting;
    Ui::Cut* ui;
    QString save_path;
    MyThread* Cut_thread;
    int image_number;
    void writeXML();
    void ChangeVision(bool Editable);
signals:
    void operate(QList<double>, QString, QString, QString, QString, QStandardItemModel*);
    void operate2(double h5_left,
        double h5_right,
        double h5_top,
        double h5_bottom,
        QString save_path,
        QString project_name,
        QString node_name,
        QString file,
        QStandardItemModel* model);
    void sendCopy(QStandardItemModel*);
private slots:
    //void on_SourcepushButton_pressed();
    /*��γ�Ȳü�������*/
    void on_comboBox_currentIndexChanged();
    void on_comboBox_2_currentIndexChanged();
    /*��ѡ�ü�������*/
    void on_comboBox_3_currentIndexChanged();
    void on_comboBox_4_currentIndexChanged();
    //void addIndex();
    //void accepted();
    /*��γ�Ȳü�ȷ����ȡ��*/
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void on_Preview_pressed();
    /*��ѡ�ü�ȷ����ȡ��*/
    void on_buttonBox_2_accepted();
    void on_buttonBox_2_rejected();

};