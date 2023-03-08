#pragma once
#include <QtWidgets/QMainWindow>
#include<qstandarditemmodel.h>
#include "ui_Import_TSX.h"
#include"MyThread.h"

class Import_TSX : public QWidget
{
    Q_OBJECT
public:
    explicit Import_TSX(QWidget* parent = Q_NULLPTR);
    ~Import_TSX();
public slots:
    void ShowProjectList(QStandardItemModel* );
    /*�������������ļ���*/
    bool generate_name(QListWidget* imageslist, vector<QString>& original_nameslist,  vector<QString>& import_nameslist);
    void ChangeVision(bool Editable);
private:
    Ui::Import_TSX* ui;
    QString cos_path;
    QString geo_path;
    QString xml_path;
    QString save_path;
    QStandardItemModel* copy;
    MyThread* import_TSX_thread, *import_TSX_thread2;
    void saveSystemSettings();
signals:
    void sendPath(QString, QString, QString);
    void operate(QString, QString, QString, QString, QString, QString, QStandardItemModel*);
    void operate2(QString, QString, vector<QString>, vector<QString>, QString, QString, QStandardItemModel*);
    void sendCopy(QStandardItemModel*);
private slots:
    void on_comboBox_dst_project_currentIndexChanged();
    /*��������Ŀ�깤�̰�ť��Ӧ����*/
    void on_comboBox_dst_project_2_currentIndexChanged();
    void on_button_xml_browse_pressed();
    /*����������Ӱ�ť��Ӧ����*/
    void on_pushButton_add_pressed();
    /*���������Ƴ���ť��Ӧ����*/
    void on_pushButton_remove_pressed();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    /*��������ȷ����ť��Ӧ����*/
    void on_buttonBox_2_accepted();
    /*��������ȡ����ť��Ӧ����*/
    void on_buttonBox_2_rejected();

    void updateProcess(int, QString);
    void endProcess();
    void endThread();
    void StopThread();
    void TransitModel(QStandardItemModel*);
};