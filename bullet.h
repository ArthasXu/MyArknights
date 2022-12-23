#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QPoint>
#include <QPainter>
#include <QString>
#include <QSize>
#include <QPropertyAnimation>

#include "enemy.h"

class MainWindow;
class Bullet : public QObject
{
        Q_OBJECT
        Q_PROPERTY(QPoint m_currentPos READ get_currentPos WRITE set_currentPos)//子弹动态移动

    private:
        QPoint m_startPos;
        QPoint m_targetPos;
        QPoint m_currentPos;

        int m_type1 = 0;                              //子弹的类型（1为正常，2为冰冻,3为群伤，4为放血）
        int m_type2 = 0;                              //子弹的类型（1为正常，2为冰冻,3为群伤，4为放血）
        int m_damage;
        QString m_path;

        Enemy * m_targetEnemy;
        MainWindow * m_game;

        static const QSize m_fixedSize;

    public:
        Bullet();
        Bullet(QPoint startPos,QPoint targetPos,int damage,Enemy * targetEnemy,QString path,MainWindow *m);

        QString get_path();
        QPoint get_currentPos();                    //得到子弹的当前位置

        void set_currentPos(QPoint pos);            //设置子弹的当前位置
        void set_type1(int t);
        void set_type2(int t);

        void move();                                //子弹的移动

    private slots:
        void hitTarget();                           //私有信号槽，击中敌人的时候触发

};

#endif // BULLET_H
