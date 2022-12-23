#ifndef MAIN_H
#define MAIN_H

#include <QVector>
#include <QString>
#include "operator.h"

extern QVector<Operator*> OperatorVec;             //干员数组
extern QVector<pair<Operator*,pair<int,int>>> Ops; //干员摆放位置,格式为<干员,<横坐标，纵坐标>>
extern QVector<Bullet*> BulletVec;                 //子弹数组
extern QVector<Enemy*> EnemyVec;                   //敌人数组
extern QVector<QVector<int>> MapVec;               //地图函数

#endif // MAIN_H
