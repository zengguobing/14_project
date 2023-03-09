#include"MainWindow.h"
#include"import_SingleTransDoubleRecv.h"
#include"icon_source.h"
#include"qfiledialog.h"
#include<opencv2/highgui.hpp>
#include<qmessagebox.h>
import_SingleTransDoubleRecv::import_SingleTransDoubleRecv(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::import_SingleTransDoubleRecv)
{
    ui->setupUi(this);
    import_SingleTransDoubleRecv_thread = NULL;
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
import_SingleTransDoubleRecv::~import_SingleTransDoubleRecv()
{
    import_SingleTransDoubleRecv_thread = NULL;
    if (copy)
    {
        for (int i = 0; i < ui->comboBox_dst_project->count(); i++)
        {
            if (!copy->findItems(ui->comboBox_dst_project->itemText(i)).isEmpty())
                copy->findItems(ui->comboBox_dst_project->itemText(i))[0]->setStatusTip(NOT_IN_PROCESS);
        }
    }
}

void import_SingleTransDoubleRecv::ChangeVision(bool Editable)
{
    if (Editable)
    {
        ui->comboBox_dst_project->setDisabled(0);
        ui->lineEdit_dst_node->setDisabled(0);
        ui->lineEdit_master->setDisabled(0);
        ui->lineEdit_slave->setDisabled(0);
        ui->buttonBox->buttons().at(0)->setDisabled(0);
        ui->pushButton_master->setDisabled(0);
        ui->pushButton_slave->setDisabled(0);
    }
    else
    {
        ui->comboBox_dst_project->setDisabled(1);
        ui->lineEdit_dst_node->setDisabled(1);
        ui->lineEdit_master ->setDisabled(1);
        ui->lineEdit_slave->setDisabled(1);
        ui->buttonBox->buttons().at(0)->setDisabled(1);
        ui->pushButton_master->setDisabled(1);
        ui->pushButton_slave->setDisabled(1);
    }
}

void import_SingleTransDoubleRecv::updateProcess(int value, QString information)
{
    if (!ui->progressBar->isHidden())
    {
        ui->progressBar->setValue(value);
        ui->progressBar->setFormat(QString::fromLocal8Bit("%1：%2%").arg(information).arg(value));
        ui->progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }
}
void import_SingleTransDoubleRecv::endProcess()
{
    if (import_SingleTransDoubleRecv_thread)
    {
        import_SingleTransDoubleRecv_thread->thread()->quit();
        import_SingleTransDoubleRecv_thread->thread()->wait();

    }
    ui->progressBar->hide();
    this->close();
}
void import_SingleTransDoubleRecv::endThread()
{
    if (import_SingleTransDoubleRecv_thread)
    {
        import_SingleTransDoubleRecv_thread->thread()->quit();
        import_SingleTransDoubleRecv_thread->thread()->wait();
    }
}
void import_SingleTransDoubleRecv::StopThread()
{
    if (import_SingleTransDoubleRecv_thread != NULL)
        if (import_SingleTransDoubleRecv_thread->thread()->isRunning())
        {
            import_SingleTransDoubleRecv_thread->thread()->requestInterruption();
            import_SingleTransDoubleRecv_thread->thread()->quit();
            import_SingleTransDoubleRecv_thread->thread()->wait();
        }
}
void import_SingleTransDoubleRecv::TransitModel(QStandardItemModel* model)
{
    emit sendCopy(model);
}

void import_SingleTransDoubleRecv::on_comboBox_dst_project_currentIndexChanged()
{
    QStandardItem* project = this->copy->findItems(ui->comboBox_dst_project->currentText())[0];
    QModelIndex pro_index = this->copy->indexFromItem(project);
    QModelIndex pro_path_index = pro_index.siblingAtColumn(1);
    this->save_path = this->copy->itemFromIndex(pro_path_index)->text();
}
void import_SingleTransDoubleRecv::ShowProjectList(QStandardItemModel* model)
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

void import_SingleTransDoubleRecv::on_pushButton_master_pressed()
{
    QString filename = QFileDialog::getOpenFileName(this,
        QString::fromLocal8Bit("导入单发双收主星数据"),
        this->xml_path,
        "*.h5");
    if (QFile::exists(filename))
    {
        ui->lineEdit_master->setText(filename);
    }
}

void import_SingleTransDoubleRecv::saveSystemSettings()
{
    int count = 0;
    QSettings settings(QString("Config.ini"), QSettings::IniFormat);
    settings.beginGroup("TSX");
    settings.setValue("xml_Path", ui->lineEdit_master->text());
    settings.endGroup();
}
void import_SingleTransDoubleRecv::on_pushButton_slave_pressed()
{
    QString filename = QFileDialog::getOpenFileName(this,
        QString::fromLocal8Bit("导入单发双收辅星数据"),
        this->xml_path,
        "*.h5");
    if (QFile::exists(filename))
    {
        ui->lineEdit_slave->setText(filename);
    }
}

void import_SingleTransDoubleRecv::on_buttonBox_accepted()
{
    if (ui->lineEdit_master->text().isEmpty() || ui->lineEdit_slave->text().isEmpty())
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("未选择.h5文件！"));
        return;
    }
    bool bFlag = (ui->lineEdit_master->text().contains(QRegularExpression("^[\\n\\w:.\\()-/]+$")))&&
        (ui->lineEdit_slave->text().contains(QRegularExpression("^[\\n\\w:.\\()-/]+$")));
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
        if (ui->lineEdit_dst_node->text() == project->child(i)->text() && project->child(i, 1)->text() != "2-complex-0.0")
        {
            same_name_node = true;
        }
    }
    if (same_name_node)
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("目标节点已存在，且和导入数据级别不同，请重命名！"));
        return;
    }

    import_SingleTransDoubleRecv_thread = new MyThread;
    import_SingleTransDoubleRecv_thread->moveToThread(new QThread(this));
    ui->progressBar->setValue(0);
    ui->progressBar->show();
    connect(this, &import_SingleTransDoubleRecv::operate, import_SingleTransDoubleRecv_thread, &MyThread::import_SingleTransDoubleRecv, Qt::QueuedConnection);
    connect(import_SingleTransDoubleRecv_thread, &MyThread::updateProcess, this, &import_SingleTransDoubleRecv::updateProcess);
    connect(import_SingleTransDoubleRecv_thread->thread(), &QThread::finished, import_SingleTransDoubleRecv_thread, &MyThread::deleteLater);
    connect(import_SingleTransDoubleRecv_thread, &MyThread::endProcess, this, &import_SingleTransDoubleRecv::endProcess);
    connect(this, &QWidget::destroyed, this, &import_SingleTransDoubleRecv::StopThread);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &import_SingleTransDoubleRecv::StopThread);// , Qt::QueuedConnection);
    connect(import_SingleTransDoubleRecv_thread, &MyThread::sendModel, this, &import_SingleTransDoubleRecv::TransitModel);
    import_SingleTransDoubleRecv_thread->thread()->start();
    emit operate(
        this->save_path,
        ui->lineEdit_master->text(),
        ui->lineEdit_slave->text(),
        ui->lineEdit_dst_node->text(),
        ui->comboBox_dst_project->currentText(),
        this->copy
    );
    ChangeVision(false);

}
void import_SingleTransDoubleRecv::on_buttonBox_rejected()
{
    close();
}

