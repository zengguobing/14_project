#pragma once
#include<QGraphicsView>

class ImageView : public QGraphicsView
{
	Q_OBJECT;
public:
	explicit ImageView(QWidget* parent = Q_NULLPTR);
	~ImageView();
protected:
	void wheelEvent(QWheelEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
private:
	QPointF sceneMousePos;//scene��껬�ֹ���ʱ���������꣬���������������
	QPointF posAnchor;//view������꣬������ק��������ק�������ж������ʲ�ת��Ϊscene����
	bool isMousePressed;
};