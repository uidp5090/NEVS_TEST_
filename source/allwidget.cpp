#include "include/allwidget.h"
#include <QGLWidget>
#include <QOpenGLWidget>

//allwidget::allwidget(QWidget *parent) : QWidget(parent)
allwidget::allwidget(QObject *parent) : QObject(parent)
{
    scene=new QGraphicsScene(-960,-360,1920,720);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    Needleitem=new NeedleItem;
    Dashitem=new DashItem;
    Backgrounditem=new BackgroundItem;
    scene->addItem(Backgrounditem);
    scene->addItem(Dashitem);
    scene->addItem(Needleitem);
    scene->setBackgroundBrush(Qt::black);
    Backgrounditem->setZValue(0);
    Dashitem->setZValue(1);
    Needleitem->setZValue(3);

    SpeedLabel=new CenterLabel;
    GraSpeedLabel=scene->addWidget(SpeedLabel);
    GraSpeedLabel->setPos(0-(SpeedLabel->width()/2),
                      0-(SpeedLabel->height()/2));
    GraSpeedLabel->setZValue(2);

    lwidget=new LWidget;
    Gralwidget=scene->addWidget(lwidget);
    Gralwidget->setPos((650-lwidget->width()-1920)/2,
                       (720-lwidget->height()-720)/2);
    Gralwidget->setZValue(2);

    rwidget=new RWidget;
    Grarwidget=scene->addWidget(rwidget);
    Grarwidget->setPos((650+650-rwidget->width())/2,
                       (720-rwidget->height()-720)/2);
    Grarwidget->setZValue(2);

    view=new QGraphicsView;
    //view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    view->setViewport(new QOpenGLWidget());
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->resize(1920,720);//设置视图大小
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //view->setSceneRect(-960,-360,1920,720);
    view->setFrameStyle(0);
    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    view->setScene(scene);

    qDebug()<<"main: "<<QThread::currentThreadId();

    //resize(1920,720);

    Lthread=new QThread(this);
    worker=new Worker;
    Lthread->start();
    worker->moveToThread(Lthread);
    qRegisterMetaType<rc_msg_t>("rc_msg_t");

    startTimer(1000);

    connect(this,SIGNAL(changLabelText(int*)),lwidget,SLOT(Update_LWiaget_data(int*)));
    connect(this,SIGNAL(changLabelText(int*)),rwidget,SLOT(Update_LWiaget_data(int*)));
    view->show();
}

void allwidget::timerEvent(QTimerEvent *event)
{
    rc_data_telltWarn_t tw_data;
    int i;
    static int AllLabel[8]={
        0,0,0,0,0,0,0,0
    };

    if(0==get_data(&tw_data))
    {
        for(i=0;i<8;i++)
        {
            if(AllLabel[i] != tw_data.telltaleWarnText.icon_status[i])
            {
                emit changLabelText(tw_data.telltaleWarnText.icon_status);
            }
        }
    }
}
