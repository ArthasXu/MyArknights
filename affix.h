#ifndef AFFIX_H
#define AFFIX_H

#include <QObject>
#include <QVector>
#include <QString>
#include <Qtimer>
#include <iostream>
using namespace std;

class Affix
{
private:
    int id;
    int type;                           //0不可使用，1给近战，2给高台，3通用
    QString ImgPath;                    //显示图片路径
    QString ImgPath_card;               //卡牌图片路径
public:
    Affix();
    Affix(int id);

    int get_id() const;
    int get_type() const;
    QString get_imgPath() const;
    QString get_imgPath_card() const;
};

#endif // AFFIX_H
