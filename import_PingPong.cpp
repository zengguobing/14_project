#include"MainWindow.h"
#include"import_PingPong.h"
#include"icon_source.h"
#include"qfiledialog.h"
#include<opencv2/highgui.hpp>
#include<qmessagebox.h>
import_PingPong::import_PingPong(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::import_PingPong)
{
    ui->setupUi(this);
    import_PingPong_thread = NULL;
    //import_TSX_thread2 = NULL;
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setHidden(1);
    //ui->progressBar_2->setMinimum(0);
    //ui->progressBar_2->setMaximum(100);
    //ui->progressBar_2->setHidden(1);
    //ui->comboBox_pol->addItem(QString("HH"));
    //ui->comboBox_pol->addItem(QString("VV"));
    //ui->comboBox_pol2->addItem(QString("HH"));
    //ui->comboBox_pol2->addItem(QString("VV"));
    QSettings settings(QString("Config.ini"), QSettings::IniFormat);
    settings.beginGroup("TSX");
    this->xml_path = settings.value("xml_Path").toString();
    settings.endGroup();
}
import_PingPong::~import_PingPong()
{
    import_PingPong_thread = NULL;
    if (copy)
    {
        for (int i = 0; i < ui->comboBox_dst_project->count(); i++)
        {
            if (!copy->findItems(ui->comboBox_dst_project->itemText(i)).isEmpty())
                copy->findItems(ui->comboBox_dst_project->itemText(i))[0]->setStatusTip(NOT_IN_PROCESS);
        }
    }
}

void import_PingPong::ChangeVision(bool Editable)
{
    if (Editable)
    {
        ui->comboBox_dst_project->setDisabled(0);
        ui->lineEdit_dst_node->setDisabled(0);
        ui->lineEdit_master_master->setDisabled(0);
        ui->lineEdit_slave_master->setDisabled(0);
        ui->lineEdit_slave_slave->setDisabled(0);
        ui->lineEdit_master_slave->setDisabled(0);
        ui->buttonBox->buttons().at(0)->setDisabled(0);
        ui->pushButton_master_master->setDisabled(0);
        ui->pushButton_slave_master->setDisabled(0);
        ui->pushButton_slave_slave->setDisabled(0);
        ui->pushButton_master_slave->setDisabled(0);
    }
    else
    {
        ui->comboBox_dst_project->setDisabled(1);
        ui->lineEdit_dst_node->setDisabled(1);
        ui->lineEdit_master_master->setDisabled(1);
        ui->lineEdit_slave_master->setDisabled(1);
        ui->lineEdit_slave_slave->setDisabled(1);
        ui->lineEdit_master_slave->setDisabled(1);
        ui->buttonBox->buttons().at(0)->setDisabled(1);
        ui->pushButton_master_master->setDisabled(1);
        ui->pushButton_slave_master->setDisabled(1);
        ui->pushButton_slave_slave->setDisabled(1);
        ui->pushButton_master_slave->setDisabled(1);
    }
}

void import_PingPong::updateProcess(int value, QString information)
{
    if (!ui->progressBar->isHidden())
    {
        ui->progressBar->setValue(value);
        ui->progressBar->setFormat(QString::fromLocal8Bit("%1：%2%").arg(information).arg(value));
        ui->progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }
}
void import_PingPong::endProcess()
{
    if (import_PingPong_thread)
    {
        import_PingPong_thread->thread()->quit();
        import_PingPong_thread->thread()->wait();

    }
    ui->progressBar->hide();
    this->close();
}
void import_PingPong::endThread()
{
    if (import_PingPong_thread)
    {
        import_PingPong_thread->thread()->quit();
        import_PingPong_thread->thread()->wait();
    }
}
void import_PingPong::StopThread()
{
    if (import_PingPong_thread != NULL)
        if (import_PingPong_thread->thread()->isRunning())
        {
            import_PingPong_thread->thread()->requestInterruption();
            import_PingPong_thread->thread()->quit();
            import_PingPong_thread->thread()->wait();
        }
}
void import_PingPong::TransitModel(QStandardItemModel* model)
{
    emit sendCopy(model);
}

void import_PingPong::on_comboBox_dst_project_currentIndexChanged()
{
    QStandardItem* project = this->copy->findItems(ui->comboBox_dst_project->currentText())[0];
    QModelIndex pro_index = this->copy->indexFromItem(project);
    QModelIndex pro_path_index = pro_index.siblingAtColumn(1);
    this->save_path = this->copy->itemFromIndex(pro_path_index)->text();
}
void import_PingPong::ShowProjectList(QStandardItemModel* model)
{
    this->copy = model;
    for (int i = 0; i < model->rowCount(); i++)
    {
        ui->comboBox_dst_project->addItem(model->item(i, 0)->text());
        model->item(i, 0)->setStatusTip(IN_PROCESS);
    }
    ui->comboBox_dst_project->setCurrentIndex(0);
    this->save_path = model->item(0, 1)->text();
}

void import_PingPong::on_pushButton_master_master_pressed()
{
    QString filename = QFileDialog::getOpenFileName(this,
        QString::fromLocal8Bit("导入主发主收数据"),
        this->xml_path,
        "*.h5");
    if (QFile::exists(filename))
    {
        ui->lineEdit_master_master->setText(filename);
    }
}

void import_PingPong::on_pushButton_slave_master_pressed()
{
    QString filename = QFileDialog::getOpenFileName(this,
        QString::fromLocal8Bit("导入辅发主收数据"),
        this->xml_path,
        "*.h5");
    if (QFile::exists(filename))
    {
        ui->lineEdit_slave_master->setText(filename);
    }
}

void import_PingPong::saveSystemSettings()
{
    int count = 0;
    QSettings settings(QString("Config.ini"), QSettings::IniFormat);
    settings.beginGroup("TSX");
    settings.setValue("xml_Path", ui->lineEdit_master_master->text());
    settings.endGroup();
}
void import_PingPong::on_pushButton_master_slave_pressed()
{
    QString filename = QFileDialog::getOpenFileName(this,
        QString::fromLocal8Bit("导入主发辅收数据"),
        this->xml_path,
        "*.h5");
    if (QFile::exists(filename))
    {
        ui->lineEdit_master_slave->setText(filename);
    }
}

void import_PingPong::on_pushButton_slave_slave_pressed()
{
    QString filename = QFileDialog::getOpenFileName(this,
        QString::fromLocal8Bit("导入辅发辅收数据"),
        this->xml_path,
        "*.h5");
    if (QFile::exists(filename))
    {
        ui->lineEdit_slave_slave->setText(filename);
    }
}

void import_PingPong::on_buttonBox_accepted()
{
    if (ui->lineEdit_master_master->text().isEmpty() || 
        ui->lineEdit_master_slave->text().isEmpty() ||
        ui->lineEdit_slave_master->text().isEmpty() ||
        ui->lineEdit_slave_slave->text().isEmpty()
        )
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("未选择.h5文件！"));
        return;
    }
    bool bFlag = (ui->lineEdit_master_master->text().contains(QRegularExpression("^[\\n\\w:.\\()-/]+$"))) &&
        (ui->lineEdit_master_slave->text().contains(QRegularExpression("^[\\n\\w:.\\()-/]+$"))) && 
        (ui->lineEdit_slave_master->text().contains(QRegularExpression("^[\\n\\w:.\\()-/]+$"))) &&
        (ui->lineEdit_slave_slave->text().contains(QRegularExpression("^[\\n\\w:.\\()-/]+$")));
    if (!bFlag)
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("请注意路径中应当仅包含数字、字母及下划线！"));
        return;
    }


    if (ui->lineEdit_dst_node->text().isEmpty())
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("目标节点名为空！"));
        return;
    }

    //防重名检查

    QStandardItem* project = this->copy->findItems(ui->comboBox_dst_project->currentText())[0];
    if (!project) {
        return;
    }
    bool same_name_node = false;
    for (int i = 0; i < project->rowCount(); i++)
    {
        if (ui->lineEdit_dst_node->text() == project->child(i)->text() && project->child(i, 1)->text() != "3-complex-0.0")
        {
            same_name_node = true;
        }
    }
    if (same_name_node)
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("目标节点已存在，且和导入数据级别不同，请重命名！"));
        return;
    }

    import_PingPong_thread = new MyThread;
    import_PingPong_thread->moveToThread(new QThread(this));
    ui->progressBar->setValue(0);
    ui->progressBar->show();
    connect(this, &import_PingPong::operate, import_PingPong_thread, &MyThread::import_PingPong, Qt::QueuedConnection);
    connect(import_PingPong_thread, &MyThread::updateProcess, this, &import_PingPong::updateProcess);
    connect(import_PingPong_thread->thread(), &QThread::finished, import_PingPong_thread, &MyThread::deleteLater);
    connect(import_PingPong_thread, &MyThread::endProcess, this, &import_PingPong::endProcess);
    connect(this, &QWidget::destroyed, this, &import_PingPong::StopThread);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &import_PingPong::StopThread);// , Qt::QueuedConnection);
    connect(import_PingPong_thread, &MyThread::sendModel, this, &import_PingPong::TransitModel);
    import_PingPong_thread->thread()->start();
    vector<QString> import_file_list;
    import_file_list.push_back(ui->lineEdit_master_master->text());
    import_file_list.push_back(ui->lineEdit_slave_master->text());
    import_file_list.push_back(ui->lineEdit_master_slave->text());
    import_file_list.push_back(ui->lineEdit_slave_slave->text());
    emit operate(
        import_file_list,
        this->save_path,
        ui->lineEdit_dst_node->text(),
        ui->comboBox_dst_project->currentText(),
        this->copy
    );
    ChangeVision(false);

}
void import_PingPong::on_buttonBox_rejected()
{
    close();
}

