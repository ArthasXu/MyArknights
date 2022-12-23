#include "bullet.h"
#include "operator.h"
#include "mainwindow.h"
#include "utility.h"
#include "enemy.h"
#include "affix.h"

Operator::Operator(){}

Operator::Operator(int id,QTimer *q,MainWindow *m):present(false),m_game(m)
{
    this->id = id;

    switch(id){
    case 1://艾雅法拉
    {
        type = 5;                           //职业（先锋1、近卫2、重装3、狙击4、术士5、医疗6、特种7、辅助8）
        ex = 0,ey = 0;
        ImgPath = ":/resources/eyjafjalla.png";
        ImgPath_card = ":/resources/eyjafjalla_card.png";
        BulletPath = ":/resources/bullet3.png";
        DeployPath = ":/resources/eyjafjalla_deploy_matting.gif";
        //DeployPath = ":/resources/eyj-Idle.webm";
        deployTime = 1150;
        hitPoint = 1743; maxhp = 1743;
        atk = 735; baseatk = 735;
        atkType = 2;                        //伤害类型（物理1、法术2、真实3）
        atkSpeed = 1.2; baseatkSpeed = 1.2;
        range = 170;
        physicDef = 122; basephysicDef = 122;
        magicDef = 20; basemagicDef = 20;
        position = 2;
        block = 1;
        cost = 20;
        redeploy = 60;
        initialskill = 6000;curskill = 6000;maxskill = 7500;
        break;
    }
    case 2://能天使
    {
        type = 4;                           //职业（先锋1、近卫2、重装3、狙击4、术士5、医疗6、特种7、辅助8）
        ex = 0,ey = 0;
        ImgPath = ":/resources/exusiai.png";
        ImgPath_card = ":/resources/exusiai_card.png";
        BulletPath = ":/resources/bullet1.png";
        DeployPath = ":/resources/exusiai_deploy_matting.gif";
        deployTime = 550;
        hitPoint = 1833; maxhp = 1833;
        atk = 692; baseatk = 692;
        atkType = 1;                        //伤害类型（物理1、法术2、真实3）
        atkSpeed = 0.8; baseatkSpeed = 0.8;
        range = 190;
        physicDef = 161; basephysicDef = 161;
        magicDef = 0; basemagicDef = 0;
        position = 2;
        block = 1;
        cost = 13;
        redeploy = 60;
        initialskill = 2500;curskill = 2500;maxskill = 3000;
        break;
    }
    case 3://星熊
    {
        type = 3;                           //职业（先锋1、近卫2、重装3、狙击4、术士5、医疗6、特种7、辅助8）
        ex = 0,ey = 0;
        ImgPath = ":/resources/hoshiguma.png";
        ImgPath_card = ":/resources/hoshiguma_card.png";
        BulletPath = ":/resources/bullet2.png";
        DeployPath = ":/resources/hoshiguma_deploy_matting.gif";
        deployTime = 550;
        hitPoint = 3850; maxhp = 3850;
        atk = 552; baseatk = 552;
        atkType = 1;                        //伤害类型（物理1、法术2、真实3）
        atkSpeed = 1.2; baseatkSpeed = 1.2;
        range = 90;
        physicDef = 903; basephysicDef = 903;
        magicDef = 0; basemagicDef = 0;
        position = 1;
        block = 3;
        cost = 22;
        redeploy = 60;
        initialskill = 2600;curskill = 2600;maxskill = 4200;
        break;
    }
    case 4://德克萨斯
    {
        type = 1;                           //职业（先锋1、近卫2、重装3、狙击4、术士5、医疗6、特种7、辅助8）
        ex = 0,ey = 0;
        ImgPath = ":/resources/texas.png";
        ImgPath_card = ":/resources/texas_card.png";
        BulletPath = ":/resources/bullet4.png";
        DeployPath = ":/resources/texas_deploy_matting.gif";
        deployTime = 550;
        hitPoint = 1802; maxhp = 1802;
        atk = 571; baseatk = 571;
        atkType = 1;                        //伤害类型（物理1、法术2、真实3）
        atkSpeed = 1; baseatkSpeed = 1;
        range = 90;
        physicDef = 326; basephysicDef = 326;
        magicDef = 0; basemagicDef = 0;
        position = 1;
        block = 2;
        cost = 11;
        redeploy = 60;
        initialskill = 3200;curskill = 3200;maxskill = 4000;
        break;
    }
    default:
        break;
    }

    m_fireRateTime = q;
    m_chooseEnemy = NULL;
    connect(m_fireRateTime,SIGNAL(timeout()),this,SLOT(shootWeapon()));
    connect(wait_timer,SIGNAL(timeout()),this,SLOT(waitskill()));
    connect(use_timer,SIGNAL(timeout()),this,SLOT(usingskill()));
}

int Operator::get_x() const{
    return ex;
}

int Operator::get_y() const{
    return ey;
}

QString Operator::get_imgPath() const{
    return ImgPath;
}

QString Operator::get_imgPath_card() const{
    return ImgPath_card;
}

QString Operator::get_deployPath() const{
    return DeployPath;
}

int Operator::get_dtime() const{
    return deployTime;
}

int Operator::get_id() const{
    return id;
}

int Operator::get_atk() const{
    return atk;
}

int Operator::get_baseatk() const{
    return baseatk;
}

int Operator::get_atkType() const{
    return atkType;
}

int Operator::get_range() const{
    return range;
}

double Operator::get_atkSpeed() const{
    return atkSpeed;
}

double Operator::get_baseatkSpeed() const{
    return baseatkSpeed;
}

int Operator::get_physicDef() const{
    return physicDef;
}

int Operator::get_basephysicDef() const{
    return basephysicDef;
}

int Operator::get_magicDef() const{
    return magicDef;
}

int Operator::get_basemagicDef() const{
    return basemagicDef;
}

int Operator::get_type() const{
    return type;
}

int Operator::get_position() const{
    return position;
}

int Operator::get_block() const{
    return block;
}

int Operator::get_curBlock() const{
    return curBlock;
}

int Operator::get_cost() const{
    return cost;
}

int Operator::get_redeploy() const{
    return redeploy;
}

int Operator::get_health() const{
    return hitPoint;
}

int Operator::get_maxhp() const{
    return maxhp;
}

bool Operator::get_present() const{
    return present;
}

int Operator::get_initialskill() const{
    return initialskill;
}

int Operator::get_curskill() const{
    return curskill;
}

int Operator::get_maxskill() const{
    return maxskill;
}

bool Operator::get_isskill() const{
    return isskill;
}

bool Operator::get_isready() const{
    return isready;
}

bool Operator::get_isattack() const{
    return isattack;
}

Enemy* Operator::get_AttackedEnemy() const{
    return m_chooseEnemy;
}

Affix* Operator::get_aff1() const{
    return aff1;
}

Affix* Operator::get_aff2() const{
    return aff2;
}

QTimer* Operator::get_wait_timer() const{
    return wait_timer;
}

QTimer* Operator::get_use_timer() const{
    return use_timer;
}

QTimer* Operator::get_m_fireRateTime() const{
    return m_fireRateTime;
}

void Operator::set_health(int h){
    hitPoint = h;
}

void Operator::set_atk(int a){
    atk = a;
}

void Operator::set_atkSpeed(double as){
    atkSpeed = as;
}

void Operator::set_physicDef(int pd){
    physicDef = pd;
}

void Operator::set_magicDef(int md){
    magicDef = md;
}

void Operator::set_curBlock(int cb){
    curBlock = cb;
}

void Operator::set_x(int x){
    ex = x;
}

void Operator::set_y(int y){
    ey = y;
}

void Operator::set_present(bool p){
    this->present = p;
}

int Operator::set_aff(Affix* a){
    //添加词缀，函数内自动判断是否已有该词缀（返回1）、是否词缀已满（返回2）、不匹配（返回3）、添加成功（返回4）
    if(aff1 != NULL && aff2 != NULL){
        return 2;
    }
    else{
        if(a->get_type() != this->position){
            return 3;
        }
        else{
            if(aff1 != NULL){
                if(aff1->get_id() == a->get_id()){
                    return 1;
                }
                else{
                    aff2 = a;
                    return 4;
                }
            }
            else{
                aff1 = a;
                return 4;
            }
        }
    }
}

void Operator::set_aff1(Affix* a){
    aff1 = a;
}

void Operator::set_aff2(Affix* a){
    aff2 = a;
}

void Operator::targetKilled(){
    m_chooseEnemy=NULL;
    if(m_fireRateTime->isActive()){
        m_fireRateTime->stop(); //敌人死亡，停止开火
        isattack = false;
    }
}

void Operator::checkEnemyInRange(){
    if(present){
            if(m_chooseEnemy != NULL) //如果有了攻击的敌人
            {
                //QVector2D normalized(m_chooseEnemy->getPos()-m_pos);
                //normalized.normalize();
                if(!collisionWithCircle(QPoint(ex,ey),range,QPoint(m_chooseEnemy->get_x(),m_chooseEnemy->get_y()),0)) //当敌人不在范围内的时候
                {
                    lostSightOfEnemy();
                }
            }
            else //如果没有攻击的敌人，就遍历enemylist，找到在攻击范围内的敌人
            {
                QVector<Enemy*> enemyList = m_game->get_enemyVec();
                for(int i = 0;i<enemyList.size();++i){
                    if(!enemyList[i]->get_nearDeath() && collisionWithCircle(QPoint(ex,ey),range,QPoint(enemyList[i]->get_x(),enemyList[i]->get_y()),0))
                    {
                        chooseEnemyFromAttack(enemyList[i]);
                        break;
                    }
                    //cout<<ex<<' '<<ey<<' '<<range<<' '<<enemyList[i]->get_x()<<' '<<enemyList[i]->get_y()<<endl;
                }
            }
    }
}

void Operator::lostSightOfEnemy(){
    m_chooseEnemy=NULL;
    if(m_fireRateTime->isActive()){
        m_fireRateTime->stop();
        isattack = false;
    }
}

void Operator::chooseEnemyFromAttack(Enemy * enemy){
    m_chooseEnemy = enemy;
    attackEnemy();
}

void Operator::attackEnemy(){
    if(!m_fireRateTime->isActive()){
        m_fireRateTime->start(atkSpeed*500);
        isattack = true;
    }
}

void Operator::shootWeapon()
{
    //cout<<1<<endl;
    int damage = 0;
    if(atkType == 1){
        damage = max((int)0.05*atk,atk - m_chooseEnemy->get_physicDef());
    }
    else if(atkType == 2){
        damage = max((int)0.05*atk,atk *(1- m_chooseEnemy->get_magicDef()/100));
    }
    else{
        damage = atk;
    }
    Bullet * bullet = new Bullet(QPoint(ex,ey),QPoint(m_chooseEnemy->get_x(),m_chooseEnemy->get_y()),damage,m_chooseEnemy,BulletPath,m_game);
    if(aff1 != NULL)
        bullet->set_type1(aff1->get_id());
    if(aff2 != NULL)
        bullet->set_type2(aff2->get_id());

    m_game->addBullet(bullet);//将该子弹添加到mainwindow中
    bullet->move();
}

void Operator::deploy(){
    wait_timer->start(50);
}

int Operator::useskill(){
    if(curskill < maxskill)
        return -1;

    use_timer->start(50);
    isskill = true;
    isready = false;

    switch(id){
    case 1:{
        atk = 2*atk;
        break;
    }
    case 2:{
        atkSpeed = 0.2*atkSpeed;
        if(m_fireRateTime->isActive()){
            m_fireRateTime->stop();
            m_fireRateTime->start(atkSpeed*500);
        }
        break;
    }
    case 3:{
        physicDef = 2*physicDef;
        break;
    }
    case 4:{
        m_game->addcost(10);
        break;
    }
    default:{
        break;
    }
    }
    return 0;
}

void Operator::endskill(){
    switch(id){
    case 1:{
        atk = atk/2;
        break;
    }
    case 2:{
        atkSpeed = atkSpeed*5;
        if(m_fireRateTime->isActive()){
            m_fireRateTime->stop();
            m_fireRateTime->start(atkSpeed*500);
        }
        break;
    }
    case 3:{
        physicDef = physicDef/2;
        break;
    }
    case 4:{
        break;
    }
    default:{
        break;
    }
    }
    wait_timer->start(50);
}

void Operator::withdraw(){
    m_chooseEnemy=NULL;
    m_fireRateTime->stop();
    present = false;

    //属性恢复初值
    curBlock = 0;
    hitPoint = maxhp;
    atk = baseatk;
    atkSpeed = baseatkSpeed;
    physicDef = basephysicDef;
    magicDef = basemagicDef;
    curskill = initialskill;
    isskill = false;

    if(aff1 != NULL){
        m_game->addAffix(aff1);
        aff1 = NULL;
    }
    if(aff2 != NULL){
        m_game->addAffix(aff2);
        aff2 = NULL;
    }

    if(wait_timer->isActive()){
        wait_timer->stop();
    }
    if(use_timer->isActive()){
        use_timer->stop();
    }
}

void Operator::waitskill(){
    curskill += 30;
    if(curskill >= maxskill){
        curskill = maxskill;
        isready = true;
        wait_timer->stop();
    }
}

void Operator::usingskill(){
    switch(id){
    case 1:{
        curskill -= 35;
        break;
    }
    case 2:{
        curskill -= 18;
        break;
    }
    case 3:{
        curskill -= 12;
        break;
    }
    case 4:{
        curskill -= 160;
        break;
    }
    default:{
        break;
    }
    }
    if(curskill <= 0){
        curskill = 0;
        isskill = false;
        use_timer->stop();
        endskill();
    }
}
