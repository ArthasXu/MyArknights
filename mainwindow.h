#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>     //绘制地图
#include <QMouseEvent>  //鼠标事件
#include <Qtimer>       //定时器
#include <QMediaPlayer> //音乐播放器
#include <QIODevice>
#include <QAudioOutput>
#include <QSoundEffect>
//#include <QSound>
#include <QLabel>
#include <QMovie>
#include <random>
#include <iostream>
using namespace std;

#include "operator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    const int LevelNumber;                              //标识关卡
    int winflag = 0;                                    //成功标志
    int loseflag = 0;                                   //失败标志

    int mapHeight;                                      //地图高
    int mapWidth;                                       //地图宽
    int row = 0;                                        //地图行
    int col = 0;                                        //地图列

    int map_1_7[5][11] = {
        4,2,2,2,1,1,1,1,1,3,6,
        4,2,1,1,1,2,2,2,2,4,4,
        5,1,1,7,1,1,1,1,1,3,6,
        4,2,1,1,1,2,2,2,2,4,4,
        4,2,2,2,1,1,1,1,1,3,6,
    };                                          //1近战位，2高台位，3不可部署近战位，4不可部署高台位，5蓝门（不可部署），6红门（不可部署），7震撼装置

    int map_6_16[7][12] = {
        10,10,8,8,13,10,10,13,8,9,10,10,
        13,8,8,8,3,10,10,8,8,8,3,6,
        10,10,9,9,9,9,9,9,9,9,10,10,
        5,13,8,8,8,8,8,8,8,3,13,6,
        10,9,9,12,9,9,9,9,12,9,9,10,
        5,8,8,8,8,8,8,8,8,3,13,6,
        15,15,15,15,10,15,10,15,15,10,15,15,
    };

    int map_h_8_4[6][13] = {
        6,3,17,17,16,16,16,17,16,18,16,3,6,
        20,3,17,17,16,18,16,19,19,18,16,18,20,
        20,5,17,5,16,16,16,18,19,16,16,3,6,
        20,5,17,5,16,17,16,16,19,18,16,3,6,
        20,3,17,17,16,16,16,17,19,19,19,18,20,
        6,3,17,17,18,18,16,16,16,18,19,3,6,
    };

    QVector<Operator*> OperatorVec;                     //干员数组
    QVector<pair<Operator*,pair<int,int>>> Ops;         //干员摆放位置,格式为<干员,<横坐标(1)，纵坐标(1)>>
    QVector<pair<Operator*,pair<int,int>>> Opc;         //当前选中干员
    QVector<Bullet*> BulletVec;                         //子弹数组
    QVector<pair<Enemy*,vector<int>>> EnemyBulletVec;   //敌人子弹数组，现横，现纵，目标横，目标纵
    QVector<pair<int,pair<int,int>>> EffectVec;         //特效数组，编号，横，纵
    QVector<Enemy*> EnemyVec;                           //敌人数组
    QVector<Affix*> AffixVec;                           //词缀数组
    QVector<QVector<int>> MapVec;                       //地图函数

    pair<Operator*,pair<int,int>> opcur;                //即将被摆放的干员

    int operatorChosed = -1;                            //当前选中卡牌or词缀，底栏从左到右

    int cost = 50;                                      //部署费用
    int life = 10;                                      //生命点
    int countSum = 0;                                   //关卡的总怪物数
    int count = 0;                                      //用于产生怪物的计数器
    int countTimer = 0;                                 //用于出怪
    QLabel *costlabel = new QLabel(this);               //显示费用标签控件
    QLabel *lifelabel = new QLabel(this);               //显示费用标签控件
    QLabel *countlabel = new QLabel(this);              //显示波次标签控件
    QLabel *tipslabel = new QLabel(this);               //提示标签控件
    QLabel *giflabel = new QLabel(this);                //显示动画控件
    QMovie *movie = new QMovie;                         //动画

    QTimer* timer1;                                     //插入怪物、回复费用的计时器
    QTimer* timer2;                                     //怪物移动、怪物攻击的计时器
    QTimer* timer3;                                     //干员攻击的计时器
    QTimer* timer4;                                     //停止动画的计时器
    QTimer* timer5;                                     //判断胜负的计时器
    QTimer* timer6;                                     //播报胜利动画的计时器
    QTimer* timer7;                                     //播报boss信息的计时器

    vector<vector<int>> fireball;
    QTimer* timer8 = new QTimer(this);                  //火雨移动计时器
    QTimer* timer9 = new QTimer(this);                  //boss死亡计时器
    QTimer* timer10 = new QTimer(this);                 //创造火雨计时器

    QTimer* timer11 = new QTimer(this);                 //轰炸机计时器

    int fireNum = 0;
    int deathTimer = 0;

    int reverse7 = 0;
    int infopos = 0;

    int winpos = 0;

    QPixmap qm[301];                                    //提前加载图片

    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QSoundEffect *effect;

    bool isTimeout = false;                             //暂停
    bool istimer8 = false;
    bool istimer9 = false;
    bool istimer10 = false;
    vector<vector<int>> opetimer;

public:
    void loadImg();                                     //加载图片
    void paintEvent(QPaintEvent *);                     //绘图
    void mousePressEvent(QMouseEvent *);                //鼠标点击事件

    void DrawMap(QPainter&);                            //画地图
    void DrawEnemy(QPainter&);                          //画敌人
    void DrawCard(QPainter&);                           //画卡牌
    void DrawOperator(QPainter&);                       //画干员
    void DrawControlOpe(QPainter&);                     //画干员操作（开技能、撤退）
    void DrawBullet(QPainter&);                         //画子弹
    void DrawEnemyBullet(QPainter&);                    //画敌人子弹
    void DrawIcon(QPainter&);                           //画插件
    void DrawResult(QPainter&);                         //画结果
    void DrawInfo(QPainter&);                           //画信息
    void DrawFire(QPainter&);                           //画火雨
    void DrawPause(QPainter&);                          //画暂停

    inline bool DeductionCost(int cost);                //判断费用

    void EnemyCreator();                                //出怪

    QVector<Enemy*> get_enemyVec();
    QVector<pair<Operator*,pair<int,int>>> get_opsVec();

    void addOperatorVec(Operator* o);
    void removeOps(Operator* o);

    void addBullet(Bullet* b);
    void removeBullet(Bullet* b);

    void addAffix(Affix* a);
    void addcost(int i);

    void fireRain();
    void sealFloor(int x,int y);
    void bomber(Enemy* en,int ex,int ey);

    void setMapWidth(int mw);
    void setMapHeight(int mh);
    void setrow(int r);
    void setcol(int c);
    void setcountSum(int cs);
    void setwinpos(int wp);
    void setinfopos(int ip);
    void setMapVec(QVector<QVector<int>> mv);

    void reinit();

};
#endif // MAINWINDOW_H
