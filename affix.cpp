#include "affix.h"

Affix::Affix() {}

Affix::Affix(int id){
    this->id = id;
    switch(id)
    {
        case 1:
        //狂暴的
        //己方使用：攻击力*2，攻击间隔/2
        //敌方使用：攻击力*2，攻击间隔/2
        {
            type = 1;
            this->ImgPath = ":/resources/affix1.png";
            this->ImgPath_card = ":/resources/affixcard1.png";
            break;
        }
        case 2:
        //冰系的
        //己方使用：攻击会停止怪物的攻击和移动0.5s
        //敌方使用：攻击会降低我方攻速（攻击间隔*2）
        {
            type = 1;
            this->ImgPath = ":/resources/affix2.png";
            this->ImgPath_card = ":/resources/affixcard2.png";
            break;
        }
        case 3:
        //群伤的
        //己方使用：溅射范围为攻击范围的1/2
        //敌方使用：溅射范围为一格
        {
            type = 1;
            this->ImgPath = ":/resources/affix3.png";
            this->ImgPath_card = ":/resources/affixcard3.png";
            break;
        }
        case 4:
        //放血的：
        //己方使用：攻击会让怪物进入流血状态，每秒损失当前生命值的10%，持续5秒
        //敌方使用：攻击会让干员进入流血状态，每秒损失当前生命值的5%，持续5秒
        {
            type = 2;
            this->ImgPath = ":/resources/affix4.png";
            this->ImgPath_card = ":/resources/affixcard4.png";
            break;
        }
        case 5:
        //闪现的：
        //敌方使用：能闪过阻挡，冷却10秒
        {
            type = 0;
            this->ImgPath = ":/resources/affix5.png";
            this->ImgPath_card = ":/resources/affixcard5.png";
            break;
        }
        case 6:
        //神速的：
        //敌方使用：移动速度*2
        {
            type = 0;
            this->ImgPath = ":/resources/affix6.png";
            this->ImgPath_card = ":/resources/affixcard6.png";
            break;
        }
        default:
            break;
    }
}

int Affix::get_id() const{
    return id;
}

int Affix::get_type() const{
    return type;
}

QString Affix::get_imgPath() const{
    return ImgPath;
}

QString Affix::get_imgPath_card() const{
    return ImgPath_card;
}
