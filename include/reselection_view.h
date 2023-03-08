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
QT_CHARTS_USE_NAMESPACE
#ifdef DEBUG
#pragma comment(lib, "Qt5Chartsd.lib")
#else
#pragma comment(lib, "Qt5Charts.lib")
#endif // DEBUG

using namespace cv;


/*Ԥ��ͼ��ʾ����*/
class reselection_view :public QGraphicsView
{
    Q_OBJECT
public:
    explicit reselection_view(QWidget* parent = Q_NULLPTR);
    ~reselection_view();
    /** @brief ����Ԥ��ͼ
    * @param path                       ͼƬ·��
    */
    void setPixmap(const QString& path);
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

    QGraphicsScene* mScene;             //�ӳ���
    QGraphicsRectItem* mFirstRect;      //��ѡ���
    QGraphicsRectItem* mSecondRect;     //��ѡ���
    QList<QGraphicsRectItem*> GCPs_rect;
    QList<QPointF> GCPs_point;
    QGraphicsPixmapItem* mPixmap;       //Ԥ��ͼItem
    QPixmap Cursor_up, Cursor_down;
    QList<QPoint> MatPos;
    bool mFirstIsChecked, mSecondIsChecked;  //����Ƿ�ѡ��
signals:
    //���Ͳο������꣨��������0��ʼ��
    void send_coordinate(int rows, int cols, QList<QPoint>);
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
    State mState;                       //����״̬
    bool mPointFlag;
    QPointF mFirst, mSecond;            //��¼�۲��Ͳο���  

};

/*���崰��*/
class reselection_view_Window : public QWidget
{
    Q_OBJECT
public:
    explicit reselection_view_Window(QWidget* parent = 0);
    ~reselection_view_Window();
    void resizeEvent(QResizeEvent* event);
    QDialogButtonBox* ButtonBox;        //ȷ����ȡ����ť
    reselection_view* View;                      //������ʾԤ��ͼ���Զ���QGraphicsView
signals:
    void send_coordinate(int, int, QList<QPoint>);
public slots:
    void Close();
    void receive_coordinate(int, int, QList<QPoint>);
protected:
    bool eventFilter(QObject*, QEvent*);
private:
    QVBoxLayout* MainLayout;
    QHBoxLayout* SubLayout2;
    QHBoxLayout* SubLayout3;
    QPushButton* MoveButton;
    QPushButton* ScaledButton;
    QPushButton* SelectButton;
    QPushButton* GCPsButton;
    QPushButton* GCPsDeleteButton;
    QLabel* status_label;
    QPixmap pixmap;
};