#pragma once
#include <QtWidgets/QMainWindow>
#include<FormatConversion.h>
#include "ui_Deformation_Average.h"
#include"MyThread.h"
#include "Deformation_Preview_Window.h"
#include"ImageView.h"
#include<Qtgui>

class Deformation_Average : public QWidget
{
    Q_OBJECT
public:
    explicit Deformation_Average(QWidget* parent = Q_NULLPTR);
    ~Deformation_Average();
    QStandardItemModel* copy;
public slots:
    void ShowProjectList(QStandardItemModel*);
    void TransitModel(QStandardItemModel*);
    void cancelled();

private:
    double h5_left, h5_right, h5_top, h5_bottom;
    /*���Ѿ������Ԥ�������ֹ�ٴε��*/
    bool isPreviewPressed;
    /*�Ƿ����ڲü�*/
    bool isDeformation_Averageting;
    Ui::Deformation_Average* ui;
    QString save_path;
    MyThread* Deformation_Average_thread;
    int image_number;
    void ChangeVision(bool Editable);
signals:
    void sendCopy(QStandardItemModel*);
private slots:
    /*��γ�Ȳü�������*/
    void on_comboBox_currentIndexChanged();
    void on_comboBox_2_currentIndexChanged();

    void on_Preview_pressed();

};