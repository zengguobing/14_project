#pragma once
#include <QtGui> 
#include <QDebug> 
#include <QtWidgets>
#include <QVector>
#include <QPainterPath>
#include <qgraphicssceneevent.h>
class QPixmap;
class QPaintEvent;
/*�����жϲ���״̬��ö����*/
enum State
{
    UnChecked,
    Move_State,
    Scaled_State,
    Cut_State,
    Check_State,
    GCPs_state,
    GCPs_delete
};


/*Ԥ��ͼ��ʾ����*/
class Preview :public QGraphicsView
{
    Q_OBJECT
public:
    explicit Preview(QWidget* parent = Q_NULLPTR);
    ~Preview();
    /** @brief ����Ԥ��ͼ
    * @param path                       ͼƬ·��
    */
    void setPixmap(const QString& path);
    /** @brief ����Ŀǰ״̬���ƶ������š��ü���
    * @param s                          ״̬
    */
    void SetState(State s);

    QGraphicsScene* mScene;             //�ӳ���
    QGraphicsRectItem* mRect;           //�ü���
    QGraphicsPixmapItem* mPixmap;       //Ԥ��ͼItem
    QPixmap Cursor_up, Cursor_down;
public slots:
    /** @brief ��ȡ�ü����ı߱���
    */
    void GetOffset();
signals:
    void SendPos(double left, double right, double top, double bottom);
    void Parent_Close();
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
private:

    QPointF sceneMousePos;//scene��껬�ֹ���ʱ���������꣬���������������
    QPointF posAnchor;//view������꣬������ק��������ק�������ж������ʲ�ת��Ϊscene����
    bool isMousePressed;                //�����ж��ƶ�ʱ����Ƿ���
    State mState;                       //����״̬
    QPointF mStart, mEnd;               //��¼��ʼ������ֹ��                
};

/*���崰��*/
class Preview_Window : public QWidget
{
    Q_OBJECT
public:
    explicit Preview_Window(QWidget* parent = 0);
    ~Preview_Window();
    void resizeEvent(QResizeEvent* event);
    QDialogButtonBox* ButtonBox;        //ȷ����ȡ����ť
    Preview* View;                      //������ʾԤ��ͼ���Զ���QGraphicsView
public slots:
    void Close();
protected:
    bool eventFilter(QObject*, QEvent*);
private:
    QVBoxLayout* MainLayout;
    QHBoxLayout* SubLayout1;
    QHBoxLayout* SubLayout2;
    QPushButton* MoveButton;
    QPushButton* ScaledButton;
    QPushButton* CutButton;
    QPixmap pixmap;
private slots:
};

