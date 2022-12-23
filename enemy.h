#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QVector>
#include <QString>
#include <Qtimer>
#include <iostream>
using namespace std;

#include "affix.h"

class Operator;
class MainWindow;
class Enemy : public QObject
{
    Q_OBJECT
private:
    int id;                                 //编号
    QVector<pair<int,int>> wayPoint;        //路径
    int ex,ey;                              //坐标
    int ewidth,eheight;                     //宽高
    QString ImgPath;                        //图片路径
    int hitPoint;                           //生命值
    int maxhp;                              //最大生命值
    int speed;                              //速度
    int atk;                                //攻击
    int atkType;                            //伤害类型（物理1、法术2、真实3）
    int atkSpeed;                           //攻速
    int physicDef;                          //物抗
    int magicDef;                           //法抗
    int value;                              //目标价值
    bool isboss;                            //是否为boss
    bool isfly = false;                     //是否为空中单位
    bool nearDeath = false;                 //弥留时刻

    int curskill = 0;                       //当前技力值
    int needskill = 0;                      //需求技力值

    Affix* aff1 = NULL;                     //词缀一
    Affix* aff2 = NULL;                     //词缀二
    int affDrop = 0;                        //死亡后概率掉落的词缀编号
    int probability = 0;                    //掉落率（以100为总概率）

    bool blocked;                           //是否被阻挡
    Operator* blocker;                      //阻挡的干员

    int atkTimer = 0;                       //用于判断是否攻击
    int moveTimer = 0;                      //用于判断是否移动
    int bloodTimer = 0;                     //用于判断是否失血

    int ice_state = 0;                      //怪物的冰冻状态（0正常、1冰冻）
    int blood_state = 0;                    //怪物的失血状态（0正常、1失血）

    QTimer * blood_timer = new QTimer;      //失血计时器
    QTimer * flash_timer = new QTimer;      //闪现计时器

    MainWindow* m_game;

public:
    Enemy();
    Enemy(int id,QVector<pair<int,int>> wayPoint,int ex,int ey,MainWindow* m_game,Affix* a1 = NULL,Affix* a2 = NULL);

    bool Move();
    void Flash();

    int get_x() const;
    int get_y() const;
    int get_width() const;
    int get_height() const;
    QString get_imgPath() const;
    int get_id() const;
    int get_atk() const;
    int get_atkType() const;
    int get_atkSpeed() const;
    int get_physicDef() const;
    int get_magicDef() const;
    int get_value() const;
    int get_speed() const;
    int get_health() const;
    int get_maxhp() const;
    bool get_blocked() const;
    Operator* get_blocker() const;
    int get_atkTimer() const;
    int get_moveTimer() const;
    bool get_isboss() const;
    bool get_isfly() const;
    bool get_nearDeath() const;
    int get_icestate() const;
    int get_bloodstate() const;

    Affix* get_aff1() const;                    //返回词缀一
    Affix* get_aff2() const;                    //返回词缀二
    int get_affdrop() const;                    //返回掉落词缀的编号
    int get_probability() const;
    bool get_flashcd() const;

    void set_health(int);
    void set_blocked(bool);
    void set_blocker(Operator*);
    void set_atkTimer(int);
    void set_moveTimer(int);
    void set_icestate(int);
    void set_bloodstate(int);
    void set_nearDeath(bool);

    void attackBlocker();
    void bloodbullet();
    void skill();
    void death();

private slots:
    void bleedstate();                          //私有信号槽，实现流血状态
    void flashcd();                             //闪现的cd (10s)
};

#endif // ENEMY_H
