#pragma once
#include<QtCharts/QtCharts>
#include <QtGui> 
#include <QDebug> 
#include <QtWidgets>
#include <QVector>
#include <QPainterPath>
#include <qgraphicssceneevent.h>
#include<Preview_Window.h>
#include<opencv2/highgui.hpp>
#include<FormatConversion.h>
QT_CHARTS_USE_NAMESPACE
#ifdef DEBUG
#pragma comment(lib, "FormatConversion_d.lib")
#pragma comment(lib, "Qt5Chartsd.lib")
#else
#pragma comment(lib, "FormatConversion.lib")
#pragma comment(lib, "Qt5Charts.lib")
#endif // DEBUG

using namespace cv;

/*�����жϲ���״̬��ö����*/

enum Method
{
    AbsoluteResult,
    RelativeResult
};

/*Ԥ��ͼ��ʾ����*/
class Deformation_Preview :public QGraphicsView
{
    Q_OBJECT
public:
    explicit Deformation_Preview(QWidget* parent = Q_NULLPTR);
    ~Deformation_Preview();
    /** @brief ����Ԥ��ͼ
    * @param path                       ͼƬ·��
    */
    void setPixmap(const QString& path);
    /** @brief ����ƽ������
    * @param m                          ����
    */
    void SetMethod(Method m);
    /** @brief ����Ŀǰ״̬���ƶ������š��ü���
    * @param s                          ״̬
    */
    void SetState(State s);
    /** @brief ����ѡ���־��trueΪ�۲�㣬flaseΪ�ο��㣩
    * @param flag                       ��־
    */
    void SetPointFlag(bool flag);
    /** @brief �����α��ļ�·��
    * @param path                       ·��
    */
    void SetH5Path(QString& path);

    State GetState();

    Method GetMethod();
    QGraphicsScene* mScene;             //�ӳ���
    QGraphicsRectItem* mFirstRect;      //��ѡ���
    QGraphicsRectItem* mSecondRect;     //��ѡ���
    QGraphicsPixmapItem* mPixmap;       //Ԥ��ͼItem
    QPixmap Cursor_up, Cursor_down;
    QList<QPoint> MatPos;
    bool mFirstIsChecked, mSecondIsChecked;  //����Ƿ�ѡ��
public slots:
    /** @brief ��ȡ�ü����ı߱���
    */
    void GetOffset();
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
private:
    QString mPath;                      //�α��ļ�·��
    QPointF sceneMousePos;//scene��껬�ֹ���ʱ���������꣬���������������
    QPointF posAnchor;//view������꣬������ק��������ק�������ж������ʲ�ת��Ϊscene����
    bool isMousePressed;                //�����ж��ƶ�ʱ����Ƿ���
    Method mMethod;                     //ƽ������
    State mState;                       //����״̬
    bool mPointFlag;
    QPointF mFirst, mSecond;            //��¼�۲��Ͳο���  
    
};

/*���崰��*/
class Deformation_Preview_Window : public QWidget
{
    Q_OBJECT
public:
    explicit Deformation_Preview_Window(QWidget* parent = 0);
    ~Deformation_Preview_Window();
    void resizeEvent(QResizeEvent* event);
    QDialogButtonBox* ButtonBox;        //ȷ����ȡ����ť
    Deformation_Preview* View;                      //������ʾԤ��ͼ���Զ���QGraphicsView
public slots:
    void Close();
protected:
    bool eventFilter(QObject*, QEvent*);
private:
    QVBoxLayout* MainLayout;
    QHBoxLayout* SubLayout1;
    QHBoxLayout* SubLayout2;
    QHBoxLayout* SubLayout3;
    QHBoxLayout* InsertLayout;
    QButtonGroup* Methods;
    QRadioButton* AbsoluteButton;
    QRadioButton* RelativeButton;
    QButtonGroup* ChoosePoint;
    QRadioButton* FirstPointButton;
    QRadioButton* SecondPointButton;
    QPushButton* MoveButton;
    QPushButton* ScaledButton;
    QPushButton* CutButton;
    QPixmap pixmap;
};

class Deformation_Chart : public QWidget
{
    Q_OBJECT
public:
    Deformation_Chart(QWidget* parent = Q_NULLPTR);
    Deformation_Chart(QString Title);
    ~Deformation_Chart();
    void resizeEvent(QResizeEvent* event);
    void SetData(Mat Times_series, Mat Deformation);
    void SetMinMax(double Min, double Max);
    void Paint(QList<double> temporal_baseline, QList<double> spatial_baseline);
protected:

private:
    double MinDefomation;
    double MaxDeformation;
    QChart* mChart;
    QChartView* mChartView;
    QLabel* mLabel;
    QValueAxis* mAxisX;
    QValueAxis* mAxisY;
private slots:
    void ShowData(const QPointF& point, bool state);
};