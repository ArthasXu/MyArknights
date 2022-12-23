#include "bullet.h"
#include "operator.h"
#include "mainwindow.h"
#include "utility.h"
#include "enemy.h"
#include "affix.h"

Enemy::Enemy(){}

Enemy::Enemy(int id,QVector<pair<int,int>> wayPoint,int ex,int ey,MainWindow* m_game,Affix* a1,Affix* a2)
    :m_game(m_game),
     aff1(a1),
     aff2(a2)
{
    this->id = id;
    for(int i = 0;i<wayPoint.size();++i){
        this->wayPoint.push_back(wayPoint[i]);
    }
    this->ex = ex;
    this->ey = ey;

    switch(id){
    case 1://源石虫
    {
        hitPoint = 1050; maxhp = 1050;
        atk = 385;atkSpeed = 800;atkType = 1;
        physicDef = 0;magicDef = 0;
        speed = 3;
        ewidth = 64,eheight = 64;
        value = 1; isboss = false;
        affDrop = 1; probability = 20;
        ImgPath = ":/resources/enemy1.png";
        break;
    }
    case 2://暴徒
    {
        hitPoint = 1700; maxhp = 1700;
        atk = 450;atkSpeed = 1000;atkType = 1;
        physicDef = 800;magicDef = 0;
        speed = 3;
        ewidth = 90,eheight = 117;
        value = 1; isboss = false;
        affDrop = 2; probability = 20;
        ImgPath = ":/resources/enemy2.png";
        break;
    }
    case 3://士兵
    {
        hitPoint = 3250; maxhp = 3250;
        atk = 500;atkSpeed = 1000;atkType = 1;
        physicDef = 100;magicDef = 0;
        speed = 3;
        ewidth = 130,eheight = 105;
        value = 1; isboss = false;
        affDrop = 3; probability = 20;
        ImgPath = ":/resources/enemy3.png";
        break;
    }
    case 4://拾荒者
    {
        hitPoint = 5000; maxhp = 5000;
        atk = 1000;atkSpeed = 1500;atkType = 1;
        physicDef = 50;magicDef = 20;
        speed = 2;
        ewidth = 104,eheight = 128;
        value = 1; isboss = false;
        affDrop = 4; probability = 30;
        ImgPath = ":/resources/enemy4.png";
        break;
    }
    case 5://庞贝
    {
        hitPoint = 40000; maxhp = 40000;
        atk = 1200;atkSpeed = 1300;atkType = 2;
        physicDef = 220;magicDef = 70;
        speed = 2;
        ewidth = 156,eheight = 156;
        value = 3; isboss = true;
        affDrop = 3; probability = 100;
        ImgPath = ":/resources/enemy5.png";
        break;
    }
    case 6://弑君者
    {
        hitPoint = 20000; maxhp = 20000;
        atk = 800;atkSpeed = 500;atkType = 1;
        physicDef = 120;magicDef = 10;
        speed = 5;
        ewidth = 100,eheight = 100;
        value = 2; isboss = true;
        affDrop = 2; probability = 100;
        ImgPath = ":/resources/enemy6.png";
        break;
    }
    case 7://雪怪小队
    {
        hitPoint = 2650; maxhp = 2650;
        atk = 600;atkSpeed = 1000;atkType = 1;
        physicDef = 100;magicDef = 10;
        speed = 3;
        ewidth = 125,eheight = 125;
        value = 1; isboss = false;
        affDrop = 2; probability = 10;
        ImgPath = ":/resources/enemy7.png";
        break;
    }
    case 8://重装防御组长
    {
        hitPoint = 10000; maxhp = 10000;
        atk = 650;atkSpeed = 1000;atkType = 1;
        physicDef = 1200;magicDef = 0;
        speed = 2;
        ewidth = 130,eheight = 130;
        value = 1; isboss = false;
        affDrop = 3; probability = 20;
        ImgPath = ":/resources/enemy8.png";
        break;
    }
    case 9://霜锐
    {
        hitPoint = 2550; maxhp = 2550;
        atk = 450;atkSpeed = 800;atkType = 1;
        physicDef = 0;magicDef = 30;
        speed = 5;
        ewidth = 100,eheight = 67;
        value = 1; isboss = false;
        affDrop = 1; probability = 10;
        ImgPath = ":/resources/enemy9.png";
        break;
    }
    case 10://冰爆源石虫
    {
        hitPoint = 3250; maxhp = 3250;
        atk = 300;atkSpeed = 1500;atkType = 1;
        physicDef = 0;magicDef = 0;
        speed = 3;
        ewidth = 80,eheight = 55;
        value = 1; isboss = false;
        affDrop = 2; probability = 10;
        ImgPath = ":/resources/enemy10.png";
        break;
    }
    case 11://雪怪小队凿冰人
    {
        hitPoint = 16000; maxhp = 16000;
        atk = 1030;atkSpeed = 2000;atkType = 1;
        physicDef = 500;magicDef = 20;
        speed = 2;
        ewidth = 150,eheight = 112;
        value = 1; isboss = false;
        affDrop = 4; probability = 20;
        ImgPath = ":/resources/enemy11.png";
        break;
    }
    case 12://霜星，“冬痕”
    {
        hitPoint = 45000; maxhp = 45000;
        atk = 530;atkSpeed = 1000;atkType = 2;
        physicDef = 440;magicDef = 50;
        speed = 2;
        ewidth = 140,eheight = 140;
        value = 5; isboss = true;
        affDrop = 2; probability = 100;
        needskill = 10000;
        ImgPath = ":/resources/enemy12.png";
        break;
    }
    case 13://乌萨斯裂兽族母
    {
        hitPoint = 2000; maxhp = 2000;
        atk = 430;atkSpeed = 1400;atkType = 1;
        physicDef = 200;magicDef = 20;
        speed = 5;
        ewidth = 103,eheight = 70;
        value = 1; isboss = false;
        affDrop = 1; probability = 10;
        ImgPath = ":/resources/enemy13.png";
        break;
    }
    case 14://感染者高级纠察官
    {
        hitPoint = 3000; maxhp = 3000;
        atk = 530;atkSpeed = 1800;atkType = 1;
        physicDef = 300;magicDef = 20;
        speed = 3;
        ewidth = 120,eheight = 110;
        value = 1; isboss = false;
        affDrop = 2; probability = 20;
        ImgPath = ":/resources/enemy14.png";
        break;
    }
    case 15://帝国炮火中枢先兆者
    {
        hitPoint = 10000; maxhp = 10000;
        atk = 1200;atkSpeed = 3000;atkType = 1;
        physicDef = 500;magicDef = 50;
        speed = 2;
        ewidth = 154,eheight = 89;
        value = 2; isboss = false; isfly = true;
        affDrop = 3; probability = 30;
        needskill = 4000;
        ImgPath = ":/resources/enemy15.png";
        break;
    }
    case 16://"不死的黑蛇"
    {
        hitPoint = 60000; maxhp = 60000;
        atk = 1000;atkSpeed = 1500;atkType = 2;
        physicDef = 600;magicDef = 50;
        speed = 2;
        ewidth = 170,eheight = 130;
        value = 10; isboss = true;
        affDrop = 1; probability = 100;
        ImgPath = ":/resources/enemy16.png";
        break;
    }

    default:
        break;
    }

    blocked = false;
    blocker = NULL;

    if((aff1 != NULL && aff1->get_id() == 6) || (aff2 != NULL && aff2->get_id() == 6)){
        speed *= 2;
    }

    connect(blood_timer,SIGNAL(timeout()),this,SLOT(bleedstate()));
    connect(flash_timer,SIGNAL(timeout()),this,SLOT(flashcd()));
}

bool Enemy::Move(){

    if(needskill != 0 && !ice_state){
        curskill += 30;
        if(curskill >= needskill){
            skill();
            curskill = 0;
            //cout<<1<<endl;
        }
    }

    if(moveTimer >= 30){
    moveTimer = 0;
    ice_state = 0;      //冰冻状态恢复正常

    if(wayPoint.empty()){
        return true;
    }

    if(wayPoint[0].first < ex) //left
    {
        ex -= speed;
        if(ex < wayPoint[0].first)
            ex = wayPoint[0].first;
        return false;
    }

    if(wayPoint[0].first > ex) //right
    {
        ex += speed;
        if(ex > wayPoint[0].first)
            ex = wayPoint[0].first;
        return false;
    }

    if(wayPoint[0].second < ey) //up
    {
        ey -= speed;
        if(ey < wayPoint[0].second)
            ey = wayPoint[0].second;
        return false;
    }

    if(wayPoint[0].second > ey) //down
    {
        ey += speed;
        if(ey > wayPoint[0].second)
            ey = wayPoint[0].second;
        return false;
    }

    if (wayPoint[0].second == ey && wayPoint[0].first == ex)
    {
        if(id == 12)                            //霜星
        {
            if(wayPoint[0].first == 600 && wayPoint[0].second == 80){
                ex = 360;
                ey = 80;
            }
            else if(wayPoint[0].first == 40 && wayPoint[0].second == 160){
                ex = 840;
                ey = 320;
            }
            else if(wayPoint[0].first == 120 && wayPoint[0].second == 320){
                ex = 840;
                ey = 480;
            }
            else if(wayPoint[0].first == 840 && wayPoint[0].second == 160){
                moveTimer = -10000;
            }
            else if(wayPoint[0].first == 360 && wayPoint[0].second == 160){
                moveTimer = -10000;
            }
            else if(wayPoint[0].first == 760 && wayPoint[0].second == 320){
                moveTimer = -20000;
            }
            else if(wayPoint[0].first == 760 && wayPoint[0].second == 480){
                moveTimer = -20000;
            }
        }
        else if(id == 16)                       //塔露拉
        {
            if(wayPoint[0].first == 920 && wayPoint[0].second == 80){
                moveTimer = -10000;
            }
            else if(wayPoint[0].first == 840 && wayPoint[0].second == 400){
                moveTimer = -10000;
            }
            else if(wayPoint[0].first == 520 && wayPoint[0].second == 160){
                moveTimer = -10000;
            }
            else if(wayPoint[0].first == 520 && wayPoint[0].second == 400){
                moveTimer = -10000;
            }
        }
        else if(id == 15)                       //帝国炮火中枢先兆者
        {
            if(wayPoint[0].first == 920 && wayPoint[0].second == 240){
                moveTimer = -10000;
            }
            else if(wayPoint[0].first == 680 && wayPoint[0].second == 240){
                moveTimer = -20000;
            }
            else if(wayPoint[0].first == 440 && wayPoint[0].second == 240){
                moveTimer = -20000;
            }
            else if(wayPoint[0].first == 920 && wayPoint[0].second == 320){
                moveTimer = -10000;
            }
            else if(wayPoint[0].first == 680 && wayPoint[0].second == 320){
                moveTimer = -20000;
            }
            else if(wayPoint[0].first == 440 && wayPoint[0].second == 320){
                moveTimer = -20000;
            }
        }
        wayPoint.erase(wayPoint.begin());       //从数组中删除
        return false;
    }
    }

    return false;
}

void Enemy::Flash(){
    if(!flash_timer->isActive()){
        flash_timer->start(10000);
        if(wayPoint.empty()){
            return;
        }
        if(wayPoint[0].first < ex) //left
        {
            ex -= 120;
            if(ex < wayPoint[0].first)
                ex = wayPoint[0].first;
            return;
        }
        if(wayPoint[0].first > ex) //right
        {
            ex += 120;
            if(ex > wayPoint[0].first)
                ex = wayPoint[0].first;
            return;
        }
        if(wayPoint[0].second < ey) //up
        {
            ey -= 120;
            if(ey < wayPoint[0].second)
                ey = wayPoint[0].second;
            return;
        }

        if(wayPoint[0].second > ey) //down
        {
            ey += 120;
            if(ey > wayPoint[0].second)
                ey = wayPoint[0].second;
            return;
        }
    }
}

int Enemy::get_x() const{
    return ex;
}

int Enemy::get_y() const{
    return ey;
}

int Enemy::get_width() const{
    return ewidth;
}

int Enemy::get_height() const{
    return eheight;
}

QString Enemy::get_imgPath() const{
    return ImgPath;
}

int Enemy::get_id() const{
    return id;
}

int Enemy::get_atk() const{
    return atk;
}

int Enemy::get_atkSpeed() const{
    return atkSpeed;
}

int Enemy::get_atkType() const{
    return atkType;
}

int Enemy::get_physicDef() const{
    return physicDef;
}

int Enemy::get_magicDef() const{
    return magicDef;
}

int Enemy::get_value() const{
    return value;
}

int Enemy::get_speed() const{
    return speed;
}

int Enemy::get_health() const{
    return hitPoint;
}

int Enemy::get_maxhp() const{
    return maxhp;
}

bool Enemy::get_blocked() const{
    return blocked;
}

Operator* Enemy::get_blocker() const{
    return blocker;
}

int Enemy::get_atkTimer() const{
    return atkTimer;
}

int Enemy::get_moveTimer() const{
    return moveTimer;
}

bool Enemy::get_isboss() const{
    return isboss;
}

bool Enemy::get_isfly() const{
    return isfly;
}

bool Enemy::get_nearDeath() const{
    return nearDeath;
}

int Enemy::get_icestate() const{
    return ice_state;
}

int Enemy::get_bloodstate() const{
    return blood_state;
}

Affix* Enemy::get_aff1() const{
    return aff1;
}

Affix* Enemy::get_aff2() const{
    return aff2;
}


int Enemy::get_affdrop() const{
    return affDrop;
}

int Enemy::get_probability() const{
    return probability;
}

bool Enemy::get_flashcd() const{
    return flash_timer->isActive();
}


void Enemy::set_health(int h){
    this->hitPoint = h;
}

void Enemy::set_blocked(bool b){
    blocked = b;
}

void Enemy::set_blocker(Operator* o){
    blocker = o;
}

void Enemy::set_atkTimer(int at) {
    atkTimer = at;
}

void Enemy::set_moveTimer(int mt) {
    moveTimer = mt;
}

void Enemy::set_icestate(int s){
    ice_state = s;
}

void Enemy::set_bloodstate(int s){
    blood_state = s;
}

void Enemy::set_nearDeath(bool n){
    nearDeath = n;
}

void Enemy::attackBlocker(){
    if(blocked){
        if(!blocker->get_present()){
            blocked = false;
            blocker = NULL;
        }
        else{
            int opedef = 0,damage = 0;
            if(atkType == 1){
                opedef = blocker->get_physicDef();
                damage = max((int)(0.05*atk),atk-opedef);
            }
            else if(atkType == 2){
                opedef = blocker->get_magicDef();
                damage = max((int)(0.05*atk),(int)(atk*(100-opedef)*0.01));
            }
            else if(atkType == 3){
                damage = atk;
            }

            int remainhp = blocker->get_health()-damage;
            if(remainhp > 0)
                blocker->set_health(remainhp);
            else{
                blocker->set_health(0);
                blocked = false;
                blocker = NULL;
            }
        }
    }
}

void Enemy::bloodbullet(){
    if(blood_timer != NULL && !blood_timer->isActive()){
        blood_timer->start(1000);
        //cout<<1<<endl;
    }
    bloodTimer = 0;
    //cout<<0<<endl;
}

void Enemy::bleedstate(){
    bloodTimer++;
    hitPoint = max(0,(int)(0.9*hitPoint));
    if(bloodTimer == 5){
        bloodTimer = 0;
        blood_state = 0;
        blood_timer->stop();
    }
}

void Enemy::flashcd(){
    flash_timer->stop();
}

void Enemy::skill(){
    switch(id){
    case 10:
    {
        QVector<pair<Operator*,pair<int,int>>> ops = m_game->get_opsVec();
        //cout<<5<<endl;
        for(int i = ops.size()-1;i>=0;--i){
            if(collisionWithCircle(QPoint(ops[i].first->get_x(),ops[i].first->get_y()),100,QPoint(ex,ey),0)){
                int damage = max((int)(30),(int)(300*(100-ops[i].first->get_magicDef())*0.01));
                ops[i].first->set_health(ops[i].first->get_health() - damage);
                //cout<<ops[i].first->get_health()<<' '<<damage<<endl;
                if(ops[i].first->get_health() < 0){
                    ops[i].first->set_health(0);
                    ops[i].first->withdraw();
                    m_game->addOperatorVec(ops[i].first);
                    m_game->removeOps(ops[i].first);
                }
            }
        }
        break;
    }
    case 12:
    {
        int x = (ex+40)/80;
        int y = ey/80;
        m_game->sealFloor(x,y);
        break;
    }
    case 15:
    {
        QVector<pair<Operator*,pair<int,int>>> ops = m_game->get_opsVec();
        //cout<<5<<endl;
        for(int i = ops.size()-1;i>=0;--i){
            if(collisionWithCircle(QPoint(ops[i].first->get_x(),ops[i].first->get_y()),200,QPoint(ex,ey),0)){
                m_game->bomber(this,ops[i].first->get_x(),ops[i].first->get_y());
                int damage = max(0,atk - ops[i].first->get_physicDef());
                ops[i].first->set_health(ops[i].first->get_health() - damage);
                //cout<<ops[i].first->get_health()<<' '<<damage<<endl;
                if(ops[i].first->get_health() < 0){
                    ops[i].first->set_health(0);
                    ops[i].first->withdraw();
                    m_game->addOperatorVec(ops[i].first);
                    m_game->removeOps(ops[i].first);
                }
                break;
            }
        }
        break;
    }
    case 16:
    {
        m_game->fireRain();
        break;
    }
    default:
    {
        break;
    }
    }
}

void Enemy::death(){
    if(id == 10){ //冰爆源石虫
        skill();
    }
    if(id == 16){ //塔露拉
        skill();
    }
    if(blood_timer->isActive())
        blood_timer->stop();
    disconnect(blood_timer,SIGNAL(timeout()),this,SLOT(bleedstate()));
    //delete blood_timer;
    //delete flash_timer;
    //cout<<bloodTimer<<endl;
}
