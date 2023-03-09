#include"MainWindow.h"
#include"import_RepeatPass.h"
#include"icon_source.h"
#include"qfiledialog.h"
#include<opencv2/highgui.hpp>
#include<qmessagebox.h>
import_RepeatPass::import_RepeatPass(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::import_RepeatPass)
{
    ui->setupUi(this);
    import_RepeatPass_thread = NULL;
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setHidden(1);
}
import_RepeatPass::~import_RepeatPass()
{
    import_RepeatPass_thread = NULL;
    if (copy)
    {
        for (int i = 0; i < ui->comboBox_dst_project->count(); i++)
        {
            if (!copy->findItems(ui->comboBox_dst_project->itemText(i)).isEmpty())
                copy->findItems(ui->comboBox_dst_project->itemText(i))[0]->setStatusTip(NOT_IN_PROCESS);
        }
    }
}

bool import_RepeatPass::generate_name(QListWidget* imageslist, vector<QString>& original_nameslist, vector<QString>& import_nameslist)
{
    if (!imageslist) return false;
    import_nameslist.clear();
    original_nameslist.clear();
    for (int i = 0; i < imageslist->count(); i++)
    {
        original_nameslist.push_back(imageslist->item(i)->text());
        QFileInfo fileinfo = QFileInfo(imageslist->item(i)->text());
        import_nameslist.push_back(fileinfo.baseName());
    }
    return true;
}

void import_RepeatPass::ChangeVision(bool Editable)
{
    if (Editable)
    {
        ui->comboBox_dst_project->setDisabled(0);
        ui->lineEdit_dst_node->setDisabled(0);
        ui->buttonBox->buttons().at(0)->setDisabled(0);
        ui->pushButton_add->setDisabled(0);
        ui->pushButton_remove->setDisabled(0);
    }
    else
    {
        ui->comboBox_dst_project->setDisabled(1);
        ui->lineEdit_dst_node->setDisabled(1);
        ui->buttonBox->buttons().at(0)->setDisabled(1);
        ui->pushButton_add->setDisabled(1);
        ui->pushButton_remove->setDisabled(1);
    }
}

void import_RepeatPass::updateProcess(int value, QString information)
{
    if (!ui->progressBar->isHidden())
    {
        ui->progressBar->setValue(value);
        ui->progressBar->setFormat(QString::fromLocal8Bit("%1��%2%").arg(information).arg(value));
        ui->progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }
}
void import_RepeatPass::endProcess()
{
    if (import_RepeatPass_thread)
    {
        import_RepeatPass_thread->thread()->quit();
        import_RepeatPass_thread->thread()->wait();

    }
    ui->progressBar->hide();
    this->close();
}

void import_RepeatPass::errorProcess(QString error_msg)
{
    QMessageBox::warning(NULL, "Error", error_msg);
    if (import_RepeatPass_thread)
    {
        import_RepeatPass_thread->thread()->quit();
        import_RepeatPass_thread->thread()->wait();
        import_RepeatPass_thread = NULL;
    }
    ui->progressBar->hide();
    ChangeVision(true);
}

void import_RepeatPass::endThread()
{
    if (import_RepeatPass_thread)
    {
        import_RepeatPass_thread->thread()->quit();
        import_RepeatPass_thread->thread()->wait();
        import_RepeatPass_thread = NULL;
    }
}
void import_RepeatPass::StopThread()
{
    if (import_RepeatPass_thread != NULL)
    {
        if (import_RepeatPass_thread->thread()->isRunning())
        {
            import_RepeatPass_thread->thread()->requestInterruption();
            import_RepeatPass_thread->thread()->quit();
            import_RepeatPass_thread->thread()->wait();
        }
        import_RepeatPass_thread = NULL;
    }
}
void import_RepeatPass::TransitModel(QStandardItemModel* model)
{
    emit sendCopy(model);
}

void import_RepeatPass::ShowProjectList(QStandardItemModel* model)
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

void import_RepeatPass::on_comboBox_dst_project_currentIndexChanged()
{
    QStandardItem* project = this->copy->findItems(ui->comboBox_dst_project->currentText())[0];
    QModelIndex pro_index = this->copy->indexFromItem(project);
    QModelIndex pro_path_index = pro_index.siblingAtColumn(1);
    this->save_path = this->copy->itemFromIndex(pro_path_index)->text();
}

void import_RepeatPass::on_pushButton_add_pressed()
{
    QString filename = QFileDialog::getOpenFileName(this,
        QString::fromLocal8Bit("���뵥����������"),
        "",
        "*.h5");
    ui->listWidget->addItem(filename);
    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        if (ui->listWidget->item(i)->text().isEmpty())
        {
            ui->listWidget->takeItem(i);
        }
    }
}
void import_RepeatPass::on_pushButton_remove_pressed()
{
    if (ui->listWidget->count() >= 1)
    {
        ui->listWidget->takeItem(ui->listWidget->currentRow());
    }
    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        if (ui->listWidget->item(i)->text().isEmpty())
        {
            ui->listWidget->takeItem(i);
        }
    }
}

void import_RepeatPass::on_buttonBox_rejected()
{
    close();
}

void import_RepeatPass::on_buttonBox_accepted()
{
    //��鵼���ļ�list�Ƿ�Ϊ��
    if (ui->listWidget->count() < 1)
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("����ͼ���ļ�Ϊ�գ�"));
        return;
    }
    //���Ŀ��ڵ���
    if (ui->lineEdit_dst_node->text().isEmpty())
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("Ŀ��ڵ���Ϊ�գ�"));
        return;
    }


    //���������
    QStandardItem* project = this->copy->findItems(ui->comboBox_dst_project->currentText())[0];
    if (!project) {
        return;
    }
    bool same_name_node = false;
    for (int i = 0; i < project->rowCount(); i++)
    {
        if (ui->lineEdit_dst_node->text() == project->child(i, 0)->text() && project->child(i, 1)->text() != "1-complex-0.0")
        {
            same_name_node = true;
        }
    }
    if (same_name_node)
    {
        QMessageBox::warning(NULL, "Warning!", QString::fromLocal8Bit("Ŀ��ڵ��Ѵ��ڣ��Һ͵������ݼ���ͬ������������"));
        return;
    }

    //����ԭʼ�ļ��������ɵ����ļ�����
    vector<QString> original_namelist;
    vector<QString> import_namelist;
    if (!generate_name(ui->listWidget, original_namelist, import_namelist)) return;


    import_RepeatPass_thread = new MyThread;
    import_RepeatPass_thread->moveToThread(new QThread(this));
    ui->progressBar->setValue(0);
    ui->progressBar->show();
    connect(this, &import_RepeatPass::operate2, import_RepeatPass_thread, &MyThread::import_RepeatPass, Qt::QueuedConnection);
    connect(import_RepeatPass_thread, &MyThread::updateProcess, this, &import_RepeatPass::updateProcess);
    connect(import_RepeatPass_thread->thread(), &QThread::finished, import_RepeatPass_thread, &MyThread::deleteLater);
    connect(import_RepeatPass_thread, &MyThread::endProcess, this, &import_RepeatPass::endProcess);
    connect(import_RepeatPass_thread, &MyThread::errorProcess, this, &import_RepeatPass::errorProcess);
    connect(this, &QWidget::destroyed, this, &import_RepeatPass::StopThread);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &import_RepeatPass::StopThread);// , Qt::QueuedConnection);
    connect(import_RepeatPass_thread, &MyThread::sendModel, this, &import_RepeatPass::TransitModel);
    import_RepeatPass_thread->thread()->start();
    emit operate2(
        this->save_path, //����·��
        original_namelist,//ԭʼ�ļ���
        import_namelist, //�����ļ���
        ui->lineEdit_dst_node->text(), //����ڵ���
        ui->comboBox_dst_project->currentText(), //���빤����
        this->copy);
    ChangeVision(false);
}
