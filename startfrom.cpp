#include "startfrom.h"
#include "ui_startfrom.h"
#include "mainwindow.h"
#include "importmap.h"

startFrom::startFrom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::startFrom)
{
    ui->setupUi(this);
    this->setFixedSize(750,425);
    setWindowTitle("终端");
    //第一关
    connect(ui->pushButton,&QPushButton::clicked,[=]()
    {
        MainWindow* mw = new MainWindow(1);
        mw->move((1600 - mw->width()) / 2, (800 - mw->height()) / 2);
        mw->show();
    });
    //第二关
    connect(ui->pushButton_2,&QPushButton::clicked,[=]()
    {
        MainWindow* mw2 = new MainWindow(2);
        mw2->move((1600 - mw2->width()) / 2, (800 - mw2->height()) / 2);
        mw2->show();
     });
    //第三关
    connect(ui->pushButton_3,&QPushButton::clicked,[=]()
    {
        MainWindow* mw3 = new MainWindow(3);
        mw3->move((1600 - mw3->width()) / 2, (800 - mw3->height()) / 2);
        mw3->show();
     });    
    //导入地图
    connect(ui->pushButton_4,&QPushButton::clicked,[=](){
       importMap* im = new importMap;
       im->move((1600 - im->width()) / 2, (800 - im->height()) / 2);
       im->show();
    });
}

startFrom::~startFrom()
{
    delete ui;
}

void startFrom::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QString path(":/resources/background.png");
    painter.drawPixmap(0,0,750,425,path);
}
