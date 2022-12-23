#ifndef OPERATOR_H
#define OPERATOR_H

#include <QObject>
#include <QVector>
#include <QString>
#include <Qtimer>
#include <iostream>
using namespace std;

#include "enemy.h"
#include "bullet.h"
#include "affix.h"

class MainWindow;
class Operator : public QObject
{
    Q_OBJECT
private:
    int id;                             //编号
    int type;                           //职业（先锋1、近卫2、重装3、狙击4、术士5、医疗6、特种7、辅助8）
    int ex,ey;                          //坐标
    QString ImgPath;                    //战斗图片路径
    QString ImgPath_card;               //卡牌图片路径    
    QString BulletPath;                 //子弹图片路径
    QString DeployPath;                 //部署动画路径
    int deployTime;                     //部署动画时长
    int hitPoint;                       //当前生命值
    int maxhp;                          //最大生命值
    int atk;                            //当前攻击力
    int baseatk;                        //基础攻击力
    int atkType;                        //伤害类型（物理1、法术2、真实3）
    int range;                          //攻击范围（简化为圆形）
    double atkSpeed;                    //当前攻速
    double baseatkSpeed;                //基础攻速
    int physicDef;                      //当前物抗
    int basephysicDef;                  //基础物抗
    int magicDef;                       //当前法抗
    int basemagicDef;                   //基础法抗
    int position;                       //部署位置（地面1、高台2）
    int block;                          //阻挡单位数
    int curBlock = 0;                   //已阻挡单位数
    int cost;                           //部署费用
    int redeploy;                       //再部署时间
    bool present;                       //在场与否

    int initialskill;                   //初始技力值
    int maxskill;                       //所需技力值
    int curskill;                       //当前技力值

    bool isskill = false;               //是否处于开技能状态
    bool isready = false;               //是否可以开技能
    bool isattack = false;              //是否处于攻击状态

    QTimer* wait_timer = new QTimer;    //恢复技力计时器
    QTimer* use_timer = new QTimer;     //开技能计时器

    Affix* aff1 = NULL;                 //词缀一
    Affix* aff2 = NULL;                 //词缀二

    Enemy* m_chooseEnemy;               //正在攻击的敌人

    QTimer* m_fireRateTime;

    MainWindow* m_game;
public:
    Operator();
    Operator(int id ,QTimer *q,MainWindow *m);

    int     get_x() const;
    int     get_y() const;
    QString get_imgPath() const;
    QString get_imgPath_card() const;
    QString get_deployPath() const;
    int     get_dtime() const;
    int     get_id() const;
    int     get_atk() const;
    int     get_baseatk() const;
    int     get_atkType() const;
    int     get_range() const;
    double  get_atkSpeed() const;
    double  get_baseatkSpeed() const;
    int     get_physicDef() const;
    int     get_basephysicDef() const;
    int     get_magicDef() const;
    int     get_basemagicDef() const;
    int     get_type() const;
    int     get_position() const;
    int     get_block() const;
    int     get_curBlock() const;
    int     get_cost() const;
    int     get_redeploy() const;
    int     get_health() const;
    int     get_maxhp() const;
    bool    get_present() const;
    int     get_initialskill() const;
    int     get_curskill() const;
    int     get_maxskill() const;
    bool    get_isskill() const;
    bool    get_isready() const;
    bool    get_isattack() const;
    Enemy*  get_AttackedEnemy() const;

    QTimer* get_wait_timer() const;
    QTimer* get_use_timer() const;
    QTimer* get_m_fireRateTime() const;

    Affix*  get_aff1() const;                   //返回词缀一
    Affix*  get_aff2() const;                   //返回词缀二

    void set_health(int);
    void set_atk(int);
    void set_atkSpeed(double);
    void set_physicDef(int);
    void set_magicDef(int);
    void set_curBlock(int);
    void set_x(int);
    void set_y(int);
    void set_present(bool);

    int  set_aff(Affix* a);                     //添加词缀，函数内自动判断是否已有该词缀（返回1）、是否词缀已满（返回2）、添加成功（返回3）
    void set_aff1(Affix* a);
    void set_aff2(Affix* a);

    void attackEnemy();                         //攻击敌人
    void targetKilled();                        //当目标被击杀后
    void lostSightOfEnemy();                    //丢失视野
    void checkEnemyInRange();                   //检查敌人是否在攻击范围内
    void chooseEnemyFromAttack(Enemy * enemy);  //从可以攻击的敌人中，选出攻击的敌人
    void withdraw();                            //撤退或被击倒
    void deploy();                              //技力开始计算
    int  useskill();                            //开技能
    void endskill();                            //技能结束



    //void createBullet();                      //生成子弹
    //void removeBullet();                      //移除子弹

private slots:
    void shootWeapon();                         //私有信号槽，实现和子弹类的连接
    void waitskill();                           //私有信号槽，实现技力恢复
    void usingskill();                          //私有信号槽，实现开技能
};

#endif // OPERATOR_H
