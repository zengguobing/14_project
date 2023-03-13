#include"interferogram_generation.h"
#include"icon_source.h"
#include<qdialog.h>
#include<qcheckbox.h>
#include<qscrollarea.h>
#include<qmessagebox.h>
#include<QFile>
#include<QDir>
interferogram_generation::interferogram_generation(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::interferogram_generation)
{
    ui->setupUi(this);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setHidden(1);
    ui->spinBox_multilook_az->setValue(1);
    ui->spinBox_multilook_rg->setValue(1);
}
interferogram_generation::~interferogram_generation()
{
    interferogram_generation_thread = NULL;
    if (copy)
    {
        for (int i = 0; i < ui->comboBox_src_project->count(); i++)
        {
            if (!copy->findItems(ui->comboBox_src_project->itemText(i)).isEmpty())
                copy->findItems(ui->comboBox_src_project->itemText(i))[0]->setStatusTip(NOT_IN_PROCESS);
        }
    }
}

void interferogram_generation::updateProcess(int value, QString information)
{
    ui->progressBar->setValue(value);
    ui->progressBar->setFormat(QString::fromLocal8Bit("%1：%2%").arg(information).arg(value));
    ui->progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
}
void interferogram_generation::endProcess()
{
    interferogram_generation_thread->thread()->quit();
    interferogram_generation_thread->thread()->wait();
    ui->progressBar->hide();
    this->close();
}
void interferogram_generation::endThread()
{
    interferogram_generation_thread->thread()->quit();
    interferogram_generation_thread->thread()->wait();
}
void interferogram_generation::StopThread()
{
    if (interferogram_generation_thread != NULL)
        if (interferogram_generation_thread->thread()->isRunning())
        {
            interferogram_generation_thread->thread()->requestInterruption();
            interferogram_generation_thread->thread()->quit();
            interferogram_generation_thread->thread()->wait();
        }

}
void interferogram_generation::TransitModel(QStandardItemModel* model)
{
    emit sendCopy(model);
}

void interferogram_generation::ChangeVision(bool Editable)
{
    if (Editable)
    {
        ui->comboBox_master_image->setDisabled(0);
        ui->comboBox_src_node->setDisabled(0);
        ui->comboBox_src_project->setDisabled(0);
        ui->spinBox_multilook_az->setDisabled(0);
        ui->spinBox_multilook_rg->setDisabled(0);
        ui->buttonBox->buttons().at(0)->setDisabled(0);
    }
    else
    {
        ui->comboBox_master_image->setDisabled(1);
        ui->comboBox_src_node->setDisabled(1);
        ui->comboBox_src_project->setDisabled(1);
        ui->spinBox_multilook_az->setDisabled(1);
        ui->spinBox_multilook_rg->setDisabled(1);
        ui->buttonBox->buttons().at(0)->setDisabled(1);
        ui->buttonBox->buttons().at(0)->setDisabled(1);
    }


}

void interferogram_generation::ShowProjectList(QStandardItemModel* model)
{
    this->copy = model;
    for (int i = 0; i < model->rowCount(); i++)
    {
        ui->comboBox_src_project->addItem(model->item(i, 0)->text());
        model->item(i, 0)->setStatusTip(IN_PROCESS);
    }
    this->save_path = copy->item(0, 1)->text();
    QStandardItem* project = NULL;
    QStandardItem* node = NULL;
    int count = 0;
    for (int i = 0; i < model->rowCount(); i++)
    {
        if (model->item(i, 0)->rowCount() != 0)
        {
            count = model->item(i, 0)->rowCount();
            project = model->item(i, 0);
            ui->comboBox_src_project->setCurrentIndex(i);
            break;
        }

    }
    if (count == 0)
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("无可处理数据，请先导入数据！"));
        this->deleteLater();
    }
    QModelIndex pro_index = model->indexFromItem(project);
    ui->comboBox_src_node->clear();
    for (int i = 0; i < count; i++)
    {
        string rank = project->child(i, 1)->text().toStdString();
        int ret, mode; double level;
        ret = sscanf(rank.c_str(), "%d-complex-%lf", &mode, &level);
        if (ret == 2 && level >= 2.0)
        {
            ui->comboBox_src_node->addItem(project->child(i, 0)->text());
            if (!node)
                node = project->child(i, 0);
        }
    }
    if (ui->comboBox_src_node->count() == 0)
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("未检测到可处理数据，请先进行配准！"));
        this->deleteLater();
    }
    ui->comboBox_src_node->setCurrentIndex(0);
    ui->comboBox_master_image->clear();
    if (node != NULL)
    {
        int children = node->rowCount();
        for (int i = 0; i < children; i++)
        {
            ui->comboBox_master_image->addItem(node->child(i, 0)->text());
        }
    }
    else
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("无配准后图像用于生成干涉相位，请先进行配准!"));
        this->deleteLater();
    }
    ui->comboBox_master_image->setCurrentIndex(0);
}

void interferogram_generation::on_comboBox_src_project_currentIndexChanged()
{
    if (ui->comboBox_src_project->count() != 0)
    {
        QStandardItem* project = copy->findItems(ui->comboBox_src_project->currentText())[0];
        this->save_path = copy->item(project->row(), 1)->text();
        int count = project->rowCount();
        if (count != 0)
        {
            ui->comboBox_src_node->clear();
            QStandardItem* node = NULL;
            for (int i = 0; i < count; i++)
            {
                string rank = project->child(i, 1)->text().toStdString();
                int ret, mode; double level;
                ret = sscanf(rank.c_str(), "%d-complex-%lf", &mode, &level);
                if (ret == 2 && level >= 2.0)
                {
                    ui->comboBox_src_node->addItem(project->child(i, 0)->text());
                    if (!node)
                        node = project->child(i, 0);
                }
            }
            if (node != NULL)
            {
                int children = node->rowCount();
                ui->comboBox_master_image->clear();
                for (int i = 0; i < children; i++)
                {
                    ui->comboBox_master_image->addItem(node->child(i, 0)->text());
                }
            }
            else
            {
                ui->comboBox_master_image->clear();
            }

        }
        else
        {
            ui->comboBox_src_node->clear();
            ui->comboBox_master_image->clear();
        }
    }
}

void interferogram_generation::on_comboBox_src_node_currentIndexChanged()
{
    if (ui->comboBox_src_node->count() != 0)
    {
        QStandardItem* project = copy->findItems(ui->comboBox_src_project->currentText())[0];
        QStandardItem* node = NULL;
        int count = project->rowCount();
        int i = 0;
        for (i = 0; i < count; i++)
        {
            if (project->child(i, 0)->text() == ui->comboBox_src_node->currentText())
            {
                this->image_number = project->child(i, 0)->rowCount();
                node = project->child(i, 0);
                break;
            }
        }
        if (node != NULL)
        {
            ui->comboBox_master_image->clear();
            for (int j = 0; j < this->image_number; j++)
            {
                ui->comboBox_master_image->addItem(node->child(j, 0)->text());
            }
        }
        else
        {
            ui->comboBox_master_image->clear();
        }
    }
}

void interferogram_generation::on_buttonBox_accepted()
{
    if (ui->comboBox_src_node->count() == 0)
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("该工程无可处理数据，请先进行配准或更换工程！"));
        return;
    }
    if (ui->comboBox_master_image->count() == 0)
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("该节点下无可处理数据，请先进行配准或更换节点！"));
        return;
    }
    int multi_az = ui->spinBox_multilook_az->value();
    int multi_rg = ui->spinBox_multilook_rg->value();
    if (ui->lineEdit_dst_node->text().isEmpty())
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("请输入保存干涉相位的文件夹名称！"));
        return;
    }
    bool bFlag = ui->lineEdit_dst_node->text().contains(QRegularExpression("^\\w+$"));
    if (!bFlag)
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("请注意文件夹名称应当为数字、字母及下划线的组合！"));
        return;
    }
    int master_index = ui->comboBox_master_image->currentIndex() + 1;
    interferogram_generation_thread = new MyThread;
    interferogram_generation_thread->moveToThread(new QThread(this));
    ui->progressBar->setValue(0);
    ui->progressBar->show();
    QList<int> para;
    connect(this, &interferogram_generation::operate, interferogram_generation_thread, &MyThread::interferogram_generation_14, Qt::QueuedConnection);
    connect(interferogram_generation_thread, &MyThread::updateProcess, this, &interferogram_generation::updateProcess);
    connect(interferogram_generation_thread->thread(), &QThread::finished, interferogram_generation_thread, &MyThread::deleteLater);
    connect(interferogram_generation_thread, &MyThread::endProcess, this, &interferogram_generation::endProcess);
    connect(this, &QWidget::destroyed, this, &interferogram_generation::StopThread);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &interferogram_generation::StopThread);// , Qt::QueuedConnection);
    connect(interferogram_generation_thread, &MyThread::sendModel, this, &interferogram_generation::TransitModel);
    interferogram_generation_thread->thread()->start();
    ChangeVision(false);
    emit operate(
        ui->comboBox_master_image->currentIndex() + 1,
        ui->spinBox_multilook_rg->value(),
        ui->spinBox_multilook_az->value(),
        this->save_path, 
        ui->comboBox_src_project->currentText(),
        ui->comboBox_src_node->currentText(),
        ui->lineEdit_dst_node->text(),
        this->copy);
}

void interferogram_generation::on_buttonBox_rejected()
{
    this->close();
}