#include<Coordinate.h>
#include<cstdlib>
#include<ctime>
#include"opencv2\opencv.hpp"
#define inf 0x3f3f3f3f
Coordinate::Coordinate(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Coordinate)
{
    ui->setupUi(this);
    image = QImage(wnd_width, wnd_height, QImage::Format_RGB32);  //初始化画布
    QColor backColor = qRgb(255, 255, 255);  
    image.fill(backColor);//对画布进行填充
    this->setFixedSize(wnd_width,wnd_height);
}

void Coordinate::Paint(QList<double> temporal_baseline, QList<double> spatial_baseline, int index)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);//设置反锯齿模式


    int pointx = 50, pointy = /*1400*/700;//确定坐标轴起点坐标（Qt坐标系y轴方向向下，这里需要反过来）
    double width = /*1900*/900 - pointx, height = /*1350*/650;//确定坐标轴宽度跟高度

    //painter.drawRect(5, 5, wnd_width - 40, wnd_width - 40);//外围的矩形，从(5,5)起

    painter.drawLine(pointx, pointy, int(width) + pointx, pointy);//坐标轴x宽度为width
    painter.drawLine(pointx, pointy - int(height), pointx, pointy);//坐标轴y高度为height

    int number = temporal_baseline.size();//number为数据个数
    /*获取基线最大最小值以确定坐标轴上的间隔*/
    double time_min = 1e15, time_max = -1e15;
    double spatial_min = 1e15, spatial_max = -1e15;
    for (int i = 0; i < number; i++)
    {
        if (temporal_baseline.at(i) <= time_min)
        {
            time_min = temporal_baseline.at(i);
        }
        if (temporal_baseline.at(i) >= time_max)
        {
            time_max = temporal_baseline.at(i);
        }
        if (spatial_baseline.at(i) <= spatial_min)
        {
            spatial_min = spatial_baseline.at(i);
        }
        if (spatial_baseline.at(i) >= spatial_max)
        {
            spatial_max = spatial_baseline.at(i);
        }
    }
    double time_span = time_max - time_min;//时间基线范围
    double spatial_span = spatial_max - spatial_min;//空间基线范围

    QPen pen, penPoint, penOrigin;
    /*线条笔：黑色*/
    pen.setColor(Qt::black);
    pen.setWidth(5);
    /*其他点：黑色*/
    penPoint.setColor(Qt::black);
    penPoint.setWidth(20);
    /*原点：红色*/
    penOrigin.setColor(Qt::red);
    penOrigin.setWidth(20);

    painter.setPen(penOrigin);//基点标红
    /*原点坐标*/
    int Origin_x = pointx + (temporal_baseline.at(index - 1) - time_min) / time_span * width;
    int Origin_y = pointy - (spatial_baseline.at(index - 1) - spatial_min) / spatial_span * height;
    painter.drawPoint(Origin_x, Origin_y);
    for (int i = 0; i < number; i++)
    {
        if (i == index - 1)
        {
        }
        else
        {
            painter.setPen(pen);
            int Point_x = pointx + (temporal_baseline.at(i) - time_min) / time_span * width;
            int Point_y = pointy - (spatial_baseline.at(i) - spatial_min) / spatial_span * height;
            painter.drawLine(Point_x, Point_y, Origin_x, Origin_y);//连线

            painter.setPen(penPoint);
            painter.drawPoint(Point_x, Point_y);//标记各个点
        }
        
    }
    //绘制刻度线
    QPen penDegree;
    penDegree.setColor(Qt::black);
    penDegree.setWidth(2);
    painter.setPen(penDegree);
    //画上x轴刻度线
    int x_count = 50;
    for (int i = 0; i < x_count; i++)
    {
        double deltaX = time_span / x_count;//x轴刻度间隔
        painter.drawLine(pointx + i * width / x_count, pointy, pointx + i * width / x_count, pointy + 10);
        if ((i) % 5 == 0)//每五个点标数据
        {
            painter.drawText(pointx + i * width / x_count,
                pointy + 40, QString::number(int(time_min + i * deltaX)));
        }
        
        
        
    }
    painter.drawText(pointx + width, pointy - 10, QString::fromLocal8Bit("时间(天)"));
    painter.drawText(pointx + width /2, pointy + 80, QString::fromLocal8Bit("时空基线分布"));
    //y轴刻度线
    int y_count = 30;//y轴刻度间隔
    for (int i = 0; i < y_count; i++)
    {
        double deltaY = spatial_span / y_count;
        painter.drawLine(pointx, pointy - i * height / y_count,
            pointx - 4, pointy - i * height / y_count);
        if ((i) % 2 == 0)//每两个点标数据
        painter.drawText(pointx - 40, pointy - i * height / y_count,
            QString::number(int(spatial_min+i*deltaY)));
    }
    painter.drawText(pointx -20, pointy - height - 10, QString::fromLocal8Bit("空间基线(米)"));
}

Coordinate::~Coordinate()
{
    delete ui;
}

void Coordinate::Paint2(
    QList<double> temporal_baseline, 
    QList<double> spatial_baseline,
    int index, double temporal_thresh, double temporal_thresh_low,
    double spatial_thresh
)
{
    cv::Mat formation_matrix(temporal_baseline.size(), temporal_baseline.size(), CV_32S);
    formation_matrix = 0;
    for (int i = 0; i < temporal_baseline.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (fabs(spatial_baseline[j] - spatial_baseline[i]) < spatial_thresh &&
                fabs((temporal_baseline[j] - temporal_baseline[i]) / 365.0) < temporal_thresh &&
                fabs((temporal_baseline[j] - temporal_baseline[i]) / 365.0) > temporal_thresh_low / 365.0
                )
            {
                formation_matrix.at<int>(i, j) = 1;
            }
        }
    }
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);//设置反锯齿模式


    int pointx = 50, pointy = /*1400*/700;//确定坐标轴起点坐标（Qt坐标系y轴方向向下，这里需要反过来）
    double width = /*1900*/900 - pointx, height = /*1350*/650;//确定坐标轴宽度跟高度

    //painter.drawRect(5, 5, wnd_width - 40, wnd_width - 40);//外围的矩形，从(5,5)起

    painter.drawLine(pointx, pointy, int(width) + pointx, pointy);//坐标轴x宽度为width
    painter.drawLine(pointx, pointy - int(height), pointx, pointy);//坐标轴y高度为height

    int number = temporal_baseline.size();//number为数据个数
    /*获取基线最大最小值以确定坐标轴上的间隔*/
    double time_min = 1e15, time_max = -1e15;
    double spatial_min = 1e15, spatial_max = -1e15;
    for (int i = 0; i < number; i++)
    {
        if (temporal_baseline.at(i) <= time_min)
        {
            time_min = temporal_baseline.at(i);
        }
        if (temporal_baseline.at(i) >= time_max)
        {
            time_max = temporal_baseline.at(i);
        }
        if (spatial_baseline.at(i) <= spatial_min)
        {
            spatial_min = spatial_baseline.at(i);
        }
        if (spatial_baseline.at(i) >= spatial_max)
        {
            spatial_max = spatial_baseline.at(i);
        }
    }
    double time_span = time_max - time_min;//时间基线范围
    double spatial_span = spatial_max - spatial_min;//空间基线范围

    QPen pen, penPoint, penOrigin;
    /*线条笔：黑色*/
    pen.setColor(Qt::black);
    pen.setWidth(2);
    /*点：黑色*/
    penPoint.setColor(Qt::red);
    penPoint.setWidth(10);


    
    for (int i = 0; i < number; i++)
    {
        int Origin_x = pointx + (temporal_baseline.at(i) - time_min) / time_span * width;
        int Origin_y = pointy - (spatial_baseline.at(i) - spatial_min) / spatial_span * height;
        painter.setPen(penPoint);
        painter.drawPoint(Origin_x, Origin_y);
        for (int j = 0; j < i; j++)
        {
            if (formation_matrix.at<int>(i, j) == 1)
            {
                painter.setPen(pen);
                int Point_x = pointx + (temporal_baseline.at(j) - time_min) / time_span * width;
                int Point_y = pointy - (spatial_baseline.at(j) - spatial_min) / spatial_span * height;
                painter.drawLine(Point_x, Point_y, Origin_x, Origin_y);//连线
            }
        }
    }
    //绘制刻度线
    QPen penDegree;
    penDegree.setColor(Qt::black);
    penDegree.setWidth(2);
    painter.setPen(penDegree);
    //画上x轴刻度线
    int x_count = 50;
    for (int i = 0; i < x_count; i++)
    {
        double deltaX = time_span / x_count;//x轴刻度间隔
        painter.drawLine(pointx + i * width / x_count, pointy, pointx + i * width / x_count, pointy + 10);
        if ((i) % 5 == 0)//每五个点标数据
        {
            painter.drawText(pointx + i * width / x_count,
                pointy + 40, QString::number(int(time_min + i * deltaX)));
        }



    }
    painter.drawText(pointx + width, pointy - 10, QString::fromLocal8Bit("时间(天)"));
    painter.drawText(pointx + width / 2, pointy + 80, QString::fromLocal8Bit("小基线集时空基线组合"));
    //y轴刻度线
    int y_count = 30;//y轴刻度间隔
    for (int i = 0; i < y_count; i++)
    {
        double deltaY = spatial_span / y_count;
        painter.drawLine(pointx, pointy - i * height / y_count,
            pointx - 4, pointy - i * height / y_count);
        if ((i) % 2 == 0)//每两个点标数据
            painter.drawText(pointx - 40, pointy - i * height / y_count,
                QString::number(int(spatial_min + i * deltaY)));
    }
    painter.drawText(pointx - 20, pointy - height - 10, QString::fromLocal8Bit("空间基线(米)"));
}
