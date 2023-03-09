#include"OpenProject.h"
#include"icon_source.h"
#include<QFileDialog>
#include<QMessageBox>
OpenProject::OpenProject(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::OpenProject)
{
    ui->setupUi(this);
    this->project = new XMLFile;
}

OpenProject::~OpenProject()
{
    if (!this->project)
    {
        delete(this->project);
        this->project = NULL;
    }     
}

void OpenProject::LoadModel(QStandardItemModel* treeview)
{
    this->model = treeview;
}

void OpenProject::on_buttonBox_accepted()
{
    QString filename = ui->PathLine->text();
    QFileInfo fileinfo = QFileInfo(filename);
    QString abs_path = fileinfo.absolutePath();
    int ret = this->project->XMLFile_load(filename.toStdString().c_str());
    if (ret < 0)
    {
        return;
    }
    TiXmlElement* root;
    ret = this->project->get_root(root);
    TiXmlElement* p, * q, * j;
    QList<QString> origin_name;
    if (!root->NoChildren())
    {
        p = root->FirstChildElement();
        if (!strcmp(p->Value(), "project_info"))
        {
            q = p->FirstChildElement();
            QString project_name = q->Value();
            QStandardItem* Project = new QStandardItem;
            QStandardItem* Project_Path = new QStandardItem;
            Project->setIcon(QIcon(PROJECT_ICON));
            Project->setStatusTip(NOT_IN_PROCESS);
            if (!strcmp(q->Value(), "project_name"))
                Project->setText(q->GetText());
            q = q->NextSiblingElement();
            if (!strcmp(q->Value(), "project_path"))
                if (!strcmp(abs_path.toStdString().c_str(), q->GetText()))
                    Project_Path->setText(q->GetText());
                else
                {
                    Project_Path->setText(abs_path.toStdString().c_str());
                    q->Clear(); q->LinkEndChild(new TiXmlText(abs_path.toStdString().c_str()));//更新绝对路径
                }
            this->model->appendRow(Project);
            this->model->setItem(this->model->rowCount()-1, 1, Project_Path);
            for (p = p->NextSiblingElement(); p != NULL; p = p->NextSiblingElement())
            {
                QStandardItem* Data_Node = new QStandardItem;
                Data_Node->setText(p->Attribute("name"));
                Data_Node->setToolTip(Project->text());
                Data_Node->setIcon(QIcon(FOLDER_ICON));
                Project->appendRow(Data_Node);
                QStandardItem* Rank = new QStandardItem(p->Attribute("rank"));
                Project->setChild(Project->rowCount() - 1, 1, Rank);
                int i = 0;
                int count = 0;
                for (q = p->FirstChildElement(); q != NULL && strcmp(q->Value(), "Data") == 0; q = q->NextSiblingElement(), i++)
                {
                    QStandardItem* Data = new QStandardItem;
                    QStandardItem* Data_Path = new QStandardItem;
                    
                    for (j = q->FirstChildElement(); j != NULL; j = j->NextSiblingElement())
                    {
                 
                        if (!strcmp(j->Value(), "Data_Name"))
                        {
                            Data->setText(j->GetText());
                        }
                        else if (!strcmp(j->Value(), "Data_Rank"))
                        {
                            int mode, ret;
                            double level;
                            ret = sscanf(j->GetText(), "%d-complex-%lf", &mode, &level);
                            if (ret == 2)
                            {
                                Data->setToolTip("complex");
                            }
                            ret = sscanf(j->GetText(), "%d-phase-%lf", &mode, &level);
                            if (ret == 2)
                            {
                                Data->setToolTip("phase");
                            }
                            ret = sscanf(j->GetText(), "%d-coherence-%lf", &mode, &level);
                            if (ret == 2)
                            {
                                Data->setToolTip("coherence");
                            }
                            ret = sscanf(j->GetText(), "%d-dem-%lf", &mode, &level);
                            if (ret == 2)
                            {
                                Data->setToolTip("dem");
                            }
                        }
                        else if (!strcmp(j->Value(), "Data_Path"))
                        {
                            Data_Path->setText(abs_path + QString(j->GetText()));
                        }
                        
                    }
                    Data_Node->appendRow(Data);
                    Data->setIcon(QIcon(IMAGEDATA_ICON));
                    Data_Node->setChild(i, 1, Data_Path);
                }

            }
            emit sendModel(this->model);

        }

        this->project->XMLFile_save(filename.toStdString().c_str());
    }
    else
        QMessageBox::warning(NULL, "Warning!", "*.Insar is empty!");
    close();
}

void OpenProject::on_buttonBox_rejected()
{
    close();
}

void OpenProject::on_BrowseButton_pressed()
{
    QString filename = QFileDialog::getOpenFileName(this,
        "Path of project",
        "",
        "*.insar");
    ui->PathLine->setText(filename);
}
