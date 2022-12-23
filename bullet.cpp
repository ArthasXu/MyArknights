#include "bullet.h"
#include "operator.h"
#include "mainwindow.h"
#include "utility.h"
#include "enemy.h"
#include "affix.h"


#include <QPoint>
#include <QPainter>
#include <QString>
#include <QPropertyAnimation>

Bullet::Bullet(){}

const QSize Bullet::m_fixedSize(24,24);

Bullet::Bullet(QPoint startPos,QPoint targetPos,int damage,Enemy* targetEnemy,QString path,MainWindow *m):
    m_startPos(startPos),
    m_targetPos(targetPos),
    m_damage(damage),
    m_path(path),
    m_targetEnemy(targetEnemy),
    m_game(m)
    {}

QString Bullet::get_path(){
    return m_path;
}

QPoint Bullet::get_currentPos()
{
    return m_currentPos;
}

void Bullet::set_currentPos(QPoint pos)
{
    m_currentPos = pos;
}

void Bullet::set_type1(int t){
    m_type1 = t;
}

void Bullet::set_type2(int t){
    m_type2 = t;
}

void Bullet::move()
{
    static int duration=100;                                                    //子弹飞行的时间，经过100ms击中敌人
    QPropertyAnimation* animation=new QPropertyAnimation(this,"m_currentPos");
    animation->setDuration(duration);                                           //设置持续时间
    animation->setStartValue(m_startPos);                                       //设置起始位置
    animation->setEndValue(m_targetPos);                                        //设置目标位置
    connect(animation,SIGNAL(finished()),this,SLOT(hitTarget()));               //连接信号槽，击中敌人后，子弹动态运动结束
    animation->start();
}

void Bullet::hitTarget()
{
    if(m_targetEnemy != NULL){
        m_targetEnemy->set_health(m_targetEnemy->get_health()-m_damage);
        if(m_targetEnemy->get_health()<0)
            m_targetEnemy->set_health(0);

        if(m_type1 == 2 || m_type2 == 2){
        //冰冻的
        if(!m_targetEnemy->get_isboss()){
            m_targetEnemy->set_icestate(1);
            m_targetEnemy->set_atkTimer(-500);
            m_targetEnemy->set_moveTimer(-500);
        }
    }

        if(m_type1 == 3 || m_type2 == 3){
        //群伤的
        QVector<Enemy*> cure = m_game->get_enemyVec();
        for(int i = 0;i<cure.size();++i){
            if(cure[i] != m_targetEnemy && collisionWithCircle(QPoint(m_targetEnemy->get_x(),m_targetEnemy->get_y()),80,QPoint(cure[i]->get_x(),cure[i]->get_y()),0)){
                cure[i]->set_health(cure[i]->get_health()-m_damage/2);
                if(cure[i]->get_health()<0)
                    cure[i]->set_health(0);
            }
        }
    }

        if(m_type1 == 4 || m_type2 == 4){
        //流血的
        if(!m_targetEnemy->get_isboss()){
            m_targetEnemy->set_bloodstate(1);
            m_targetEnemy->bloodbullet();
        }
    }
    }
    m_game->removeBullet(this);
}

