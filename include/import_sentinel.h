#pragma once
#include <QtWidgets/QMainWindow>
#include<qstandarditemmodel.h>
#include"ui_import_sentinel.h"
#include"MyThread.h"

class import_sentinel : public QWidget
{
    Q_OBJECT
public:
    explicit import_sentinel(QWidget* parent = Q_NULLPTR);
    ~import_sentinel();
public slots:
    void ShowProjectList(QStandardItemModel*);
    /*���������ļ�������*/
    bool generate_name(QListWidget* imageslist, vector<QString>& original_nameslist, vector<QString>& import_nameslist);
    void ChangeVision(bool Editable);
private:
    Ui::import_sentinel* ui;
    QString sentinel_manifest_file;
    QString old_path;
    QString date, subswath, polarization;
    QString save_path;
    QStandardItemModel* copy;
    MyThread* import_sentinel_thread, *import_sentinel_thread_2;
signals:
    void sendPath(QString, QString, QString);
    void operate(QString, QString , QString , QString , QString ,QString, QString, QString, QStandardItemModel*);
    void operate2(vector<QString>, vector<QString>, QString, QString, QString, QString, QString, QStandardItemModel*);
    void sendCopy(QStandardItemModel*);
private slots:
    void on_comboBox_dst_project_currentIndexChanged();
    /*��������Ŀ�깤�̰�ť��Ӧ����*/
    void on_ComboBox_dst_project_2_currentIndexChanged();
    /*����������Ӱ�ť��Ӧ����*/
    void on_pushButton_add_pressed();
    /*���������Ƴ���ť��Ӧ����*/
    void on_pushButton_remove_pressed();
    void on_browse_Button_pressed();
    void on_pushButton_POD_pressed();
    /*��������ȷ����ť��Ӧ����*/
    void on_buttonBox_2_accepted();
    void on_buttonBox_accepted();
    /*��������ȡ����ť��Ӧ����*/
    void on_buttonBox_2_rejected();
    void on_buttonBox_rejected();
    void on_ComboBox_subswath_currentIndexChanged();
    /*���������Ӵ�ѡ����Ӧ����*/
    void on_ComboBox_subswath_2_currentIndexChanged();
    void on_ComboBox_polarization_currentIndexChanged();
    /*�������뼫����ʽѡ����Ӧ����*/
    void on_ComboBox_polarization_2_currentIndexChanged();

    void updateProcess(int, QString);
    void endProcess();
    void endThread();
    void StopThread();
    void TransitModel(QStandardItemModel*);

    
};