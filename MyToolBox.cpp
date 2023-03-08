#include<MyToolBox.h>
#include<MyPage.h>
#include<qevent.h>
#include<QScrollBar>

MyToolBox::MyToolBox(QWidget* parent) :
	QWidget(parent)
{
	initVar();
	createScrollArea();
	initLayout();
}

MyToolBox::~MyToolBox()
{

}

int MyToolBox::count() const
{
    return PageBar.count();
}

MyPage* MyToolBox::getItem(int index)
{
    if (index >= 0 && index < PageBar.count())
    {
        return PageBar[index];
    }
    else
    {
        return NULL;
    }
}

void MyToolBox::addItem(MyPage* page)
{
    if (page == NULL) return;

    PageBar << page;
    Page_wnd->layout()->addWidget(page);
    adjustScrollAreaWidgetSize();
}

void MyToolBox::setBarHeight(int index, int height)
{
    int iCount = PageBar.count();
    if (index < 0 || index >= iCount) return;

    PageBar[index]->setBarHeight(height);
    adjustScrollAreaWidgetSize();
}

void MyToolBox::setContentLayout(int index, QLayout* layout)
{
    int iCount = PageBar.count();
    if (index < 0 || index >= iCount) return;

    PageBar[index]->setContentLayout(layout);
}

void MyToolBox::Wnd_Resize()
{
    if (Page_wnd) {
        Area->setFixedSize(this->size());
        //renewLayout();
        for (int i = 0; i < PageBar.size(); i++)
            PageBar.at(i)->ResizePage();
        Page_wnd->adjustSize();
        Page_wnd->setFixedWidth(this->width());
    }
}

void MyToolBox::initVar()
{
	Area = NULL;
	Page_wnd = NULL;
}

void MyToolBox::initLayout()
{
    QVBoxLayout* Main_Layout = new QVBoxLayout;
    Main_Layout->setSpacing(4);
    Main_Layout->setContentsMargins(2, 2, 4, 2);
    for(int i=0;i<PageBar.size();i++)
    {
        Main_Layout->addWidget(PageBar.at(i));
    }
    //Main_Layout->addStretch();

    Page_wnd->setLayout(Main_Layout);
    adjustScrollAreaWidgetSize();
}

void MyToolBox::renewLayout()
{
    QVBoxLayout* Main_Layout = new QVBoxLayout;
    Main_Layout->setSpacing(4);
    Main_Layout->setContentsMargins(2, 2, 4, 2);
    for (int i = 0; i < PageBar.size(); i++)
    {
        Main_Layout->addWidget(PageBar.at(i));
    }
    //Main_Layout->addStretch();

    Page_wnd->setLayout(Main_Layout);
}

void MyToolBox::createScrollArea()
{
    //����
    Page_wnd = new QWidget;
    Area = new QScrollArea(this);
    Area->setWidget(Page_wnd);
    Area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);    //�������������
    Area->verticalScrollBar()->installEventFilter(this);           //�����������װ����������Ϊ�ڹ��������ֻ�����ʧ��ʱ����Ҫ����m_pScrollAreaWidget��С
}

bool MyToolBox::eventFilter(QObject* watched, QEvent* event)
{
    //���¸ı�ؼ�����ʾ��С
    switch ((int)event->type())
    {
    case QEvent::Resize:
        if (watched->inherits("MyToolBox"))
        {
            //���ݵ�ǰ�Ƿ��й��������֣��������Ҫ��ȥ�������Ŀ�����⵲סBar��ͼ��
            bool b = Area->verticalScrollBar()->isVisible();
            int iScrollBarWidth = Area->verticalScrollBar()->width();

            if (Area) Area->setFixedSize(size());
            if (Page_wnd)
                Page_wnd->setFixedWidth(width() - (b ? iScrollBarWidth : 0));
        }
        break;

    case QEvent::Show:
    case QEvent::Hide:
        if (watched->inherits("QScrollBar"))
        {
            //�ڵ��Bar��ʾ/����Contents ���ݵ�ǰ�Ƿ��й��������֣��������Ҫ��ȥ�������Ŀ�����⵲סBar��ͼ��
            int iScrollBarWidth = Area->verticalScrollBar()->width();
            int iWidth = width() - ((event->type() == QEvent::Show) ? iScrollBarWidth : 0);
            if (Page_wnd) Page_wnd->setFixedWidth(iWidth);
        }
        break;
    }

    return QWidget::eventFilter(watched, event);
}