#include "ui_mainwindow.h"
#include "qpainter.h"

#include "bullet.h"
#include "operator.h"
#include "utility.h"
#include "enemy.h"
#include "mainwindow.h"
#include "affix.h"

#define X80(X, Y) (X * 80 - 40), Y * 80

inline bool MainWindow::DeductionCost(int cost)
{
    if (this->cost - cost < 0) {
        tipslabel->setText(QString("部署费用不足！"));
        return false;
    }//判断费用是否足够
    this->cost -= cost;                                         //扣除费用
    costlabel->setText(QString("费用：%1").arg(this->cost));     //刷新标签文本
    return true;
}

MainWindow::MainWindow(int LevelNumber) : LevelNumber(LevelNumber), ui(new Ui::MainWindow)
{
    //初始化--------------------------------------------------------------------
    ui->setupUi(this);

    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(20);
    player->setLoops(-1);


    switch(LevelNumber){
        case 1:
        {
            mapWidth = 880;
            mapHeight = 40+560;
            row = 5;
            col = 11;
            countSum = 22;
            winpos = mapWidth;
            infopos = mapWidth;
            player->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/bgm1.mp3"));
            player->play();
            setWindowTitle("暴君");
            break;
        }
        case 2:
        {
            mapWidth = 960;
            mapHeight = 40+720;
            row = 7;
            col = 12;
            countSum = 24;
            winpos = mapWidth;
            infopos = mapWidth;
            player->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/bgm2.mp3"));
            player->play();
            setWindowTitle("黑兔子，白兔子");
            break;
        }
        case 3:
        {
            mapWidth = 1040;
            mapHeight = 40+640;
            row = 6;
            col = 13;
            countSum = 45;
            winpos = mapWidth;
            infopos = mapWidth;
            player->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/bgm3.mp3"));
            player->play();
            setWindowTitle("昂首，足践烈焰");
            break;
        }
        //自定义
        case 4:
        {
            setWindowTitle("自定义关卡");
            break;
        }
        default:
            break;
    }

    setFixedSize(mapWidth, mapHeight);
    loadImg();

    /*cout<<5<<endl;
    if(player->playbackState() == QMediaPlayer::PlayingState){
        cout<<10<<endl;
    }*/

    //初始化--------------------------------------------------------------------

    //创建干员------------------------------------------------------------------
    QTimer* timers[4];
    for(int i = 0;i<4;++i)
            timers[i] = new QTimer(this);
    Operator* o1 = new Operator(1,timers[0],this);
    Operator* o2 = new Operator(2,timers[1],this);
    Operator* o3 = new Operator(3,timers[2],this);
    Operator* o4 = new Operator(4,timers[3],this);
    OperatorVec.push_back(o1);
    OperatorVec.push_back(o2);
    OperatorVec.push_back(o3);
    OperatorVec.push_back(o4);

    Affix* a1 = new Affix(2);
    AffixVec.push_back(a1);

    //创建干员------------------------------------------------------------------

    //标签---------------------------------------------------------------------
    costlabel->move(mapWidth-160, mapHeight-160);
    setStyleSheet("color:black");
    costlabel->setFont(QFont("微软雅黑", 12));
    costlabel->setText(QString("费用：%1").arg(this->cost));

    lifelabel->move(mapWidth/2-100, 5);
    setStyleSheet("color:black");
    lifelabel->setFont(QFont("微软雅黑", 12));
    lifelabel->setText(QString("生命：%1").arg(this->life));

    countlabel->move(mapWidth/2+30, 5);
    setStyleSheet("color:black");
    countlabel->setFont(QFont("微软雅黑", 12));
    countlabel->setText(QString("敌人：%1 / %2").arg(this->count).arg(this->countSum));

    tipslabel->move(mapWidth-185, mapHeight-125);
    setStyleSheet("color:black");
    tipslabel->setFont(QFont("微软雅黑", 12));
    //标签---------------------------------------------------------------------

    //计时器-------------------------------------------------------------------
    timer1 = new QTimer(this);              //插入怪物、回复费用的计时器
    timer1->start(1000);
    connect(timer1,&QTimer::timeout,[=](){
    //根据关卡编号确定执行怪物的路径方案
        EnemyCreator();
        countlabel->setText(QString("敌人：%1 / %2").arg(this->count).arg(this->countSum));

        this->cost++;
        costlabel->setText(QString("费用：%1").arg(this->cost));
        update();
    });

    timer2 = new QTimer(this);              //怪物移动、怪物攻击的计时器
    timer2->start(30);
    connect(timer2,&QTimer::timeout,[=](){
        //怪物移动
        for (auto enem = EnemyVec.begin(); enem != EnemyVec.end(); enem++){
             if((*enem)->get_nearDeath()){
                continue;
             }

             (*enem)->set_moveTimer((*enem)->get_moveTimer()+30);
             if(!(*enem)->get_blocked()){             //未被阻挡
                if((*enem)->Move())                   //怪物走到终点
                {
                    life -= (*enem)->get_value();

                    if((*enem)->get_blocked()){       //若被阻挡，阻挡干员已阻挡数-1
                        (*enem)->get_blocker()->set_curBlock(max(0,(*enem)->get_blocker()->get_curBlock() - 1));
                    }
                    for(int i = 0;i<Ops.size();++i){
                        if(Ops[i].first->get_AttackedEnemy() == (*enem))
                                Ops[i].first->targetKilled();
                    }

                    if((*enem)->get_id() == 16){
                        fireRain();
                    }

                    delete *enem;
                    EnemyVec.erase(enem);             //怪物走到终点则删除这个怪物

                    lifelabel->setText(QString("生命：%1").arg(life));

                    //if (life <= 0)
                    //    this->close();              //生命值为0时关闭该窗口
                    break;
                }
                else if(!(*enem)->get_isfly())        //非飞行单位遇到干员，被阻挡
                {
                    for(int i = 0;i<Ops.size();++i){
                        if(Ops[i].first->get_position() == 1 &&
                           Ops[i].first->get_curBlock() < Ops[i].first->get_block() &&
                           collisionWithCircle(QPoint(Ops[i].first->get_x(),Ops[i].first->get_y()),50,QPoint((*enem)->get_x(),(*enem)->get_y()),0)){

                            //闪现
                            if(((*enem)->get_aff1() != NULL && (*enem)->get_aff1()->get_id() == 5)
                            || ((*enem)->get_aff2() != NULL && (*enem)->get_aff2()->get_id() == 5)){
                                if((*enem)->get_icestate() == 0){ //未被冰冻
                                    if(!(*enem)->get_flashcd())   //闪现cd转好了
                                        (*enem)->Flash();
                                    else{                         //老老实实被阻挡
                                        (*enem)->set_blocked(true);
                                        (*enem)->set_blocker(Ops[i].first);
                                        Ops[i].first->set_curBlock(Ops[i].first->get_curBlock()+1);
                                        break;
                                    }
                                }
                            }
                            else{
                                (*enem)->set_blocked(true);
                                (*enem)->set_blocker(Ops[i].first);
                                Ops[i].first->set_curBlock(Ops[i].first->get_curBlock()+1);
                                break;
                            }
                        }
                    }
                }
             }
             else{                                    //已被阻挡

                 if((*enem)->get_atkTimer() >= (*enem)->get_atkSpeed()){
                     Operator* blocker = (*enem)->get_blocker();
                     //进行一次攻击
                     (*enem)->attackBlocker();
                     (*enem)->set_atkTimer(0);

                     //干员死亡
                     if(blocker->get_health() <= 0){
                         for(int i = 0;i<Ops.size();++i){
                             if(Ops[i].first == blocker){
                                 blocker->withdraw();
                                 OperatorVec.push_back(blocker);
                                 Ops.erase(Ops.begin()+i);
                                 break;
                             }
                         }
                     }
                 }
                 else{
                     (*enem)->set_atkTimer((*enem)->get_atkTimer() + 30);
                 }
             }
             if((*enem)->get_health() <= 0 && !(*enem)->get_nearDeath())           //怪物死亡
             {
                 for(int i = 0;i<Ops.size();++i){
                     if(Ops[i].first->get_AttackedEnemy() == (*enem)){
                           Ops[i].first->targetKilled();
                           //cout<<1<<endl;
                     }
                 }

                 if((*enem)->get_blocked()){          //若被阻挡，阻挡干员已阻挡数-1
                     (*enem)->get_blocker()->set_curBlock(max(0,(*enem)->get_blocker()->get_curBlock() - 1));
                     //cout<<(*enem)->get_blocker()->get_curBlock()<<endl;
                 }

                 std::mt19937 rng;                    //掉落词缀
                 rng.seed(std::random_device()());
                 std::uniform_int_distribution<int> r(1, 100);
                 int ra = r(rng);
                 if(ra <= (*enem)->get_probability()){
                     if(AffixVec.size() <= 8){
                         Affix* aff = new Affix((*enem)->get_affdrop());
                         AffixVec.push_back(aff);
                     }
                 }

                 (*enem)->death();

                 if((*enem)->get_id() == 16){
                     (*enem)->set_nearDeath(true);
                     timer9->start(30);
                     //timer9->setSingleShot(true);
                     istimer9 = true;
                     connect(timer9,&QTimer::timeout,[=](){
                         deathTimer += 30;
                         //cout<<deathTimer<<endl;
                         if(deathTimer == 30000){
                            for(int h = 0;h<EnemyVec.size();++h){
                                if(EnemyVec[h]->get_id() == 16){
                                    EnemyVec.erase(EnemyVec.begin()+h);
                                    break;
                                 }
                            }
                            istimer9 = false;
                            timer9->stop();
                         }
                     });
                 }
                 else{
                    delete *enem;
                    EnemyVec.erase(enem);
                    break;
                 }
             }
        }
        update();
    });

    timer3 = new QTimer(this);              //干员攻击的计时器
    timer3->start(60);
    connect(timer3,&QTimer::timeout,[=](){
        //cout<<Ops.size()<<endl;
        for(int i = 0;i<Ops.size();++i){
            Ops[i].first->checkEnemyInRange();
        }
        update();
    });

    timer4 = new QTimer(this);              //停止动画的计时器
    connect(timer4,&QTimer::timeout,[=](){
        movie->stop();
        giflabel->clear();
        Ops.push_back(opcur);
        opcur.first->deploy();
        update();
    });
    timer4->setSingleShot(true);

    timer5 = new QTimer(this);              //判断胜负的计时器
    timer5->start(60);
    connect(timer5,&QTimer::timeout,[=](){
        if(life <= 0){
            loseflag = 1;
        }
        else if(EnemyVec.empty() && count >= countSum){
            winflag = 1;
            timer6->start(10);
        }
    });

    timer6 = new QTimer(this);              //播报胜利动画的计时器
    connect(timer6,&QTimer::timeout,[=](){
        winpos -= 25;
        if(winpos <= 0){
            winpos = 0;
            timer6->stop();
        }
    });

    timer7 = new QTimer(this);              //播报boss信息的计时器
    timer7->start(30);
    connect(timer7,&QTimer::timeout,[=](){
        if(reverse7 == 0){
            infopos -= 10;
            if(infopos <= mapWidth-240){
                infopos = mapWidth-240;
                reverse7 = 1;
            }
        }
        else if(reverse7 >= 1 && reverse7 < 1000){
            reverse7 += 5;
            if(reverse7 >= 1000)
                reverse7 = 1000;
        }
        else if(reverse7 == 1000){
            infopos += 10;
            if(infopos >= mapWidth){
                infopos = mapWidth;
                reverse7 = -1;
                timer7->stop();
            }
        }
    });

    timer11->start(10);
    connect(timer11,&QTimer::timeout,[=](){
        for(int i = 0;i<EnemyBulletVec.size();++i){
            EnemyBulletVec[i].second[1] += 20;
            if(EnemyBulletVec[i].second[1] >= EnemyBulletVec[i].second[3]-40){
                EnemyBulletVec[i].second[1] = EnemyBulletVec[i].second[3]-40;

                EffectVec.push_back(make_pair(1,make_pair(EnemyBulletVec[i].second[2]-40,EnemyBulletVec[i].second[3]-40)));
                QTimer *qt = new QTimer(this);
                qt->start(100);
                qt->setSingleShot(true);
                connect(qt,&QTimer::timeout,[=](){
                    EffectVec.erase(EffectVec.begin());
                });

                EnemyBulletVec.erase(EnemyBulletVec.begin()+i);
                break;
            }
        }
    });

    //计时器-------------------------------------------------------------------

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImg(){
    //icon
    qm[0] = QPixmap(":/resources/1-7.png");
    qm[1] = QPixmap(":/resources/costicon.png");
    qm[2] = QPixmap(":/resources/lifeicon.png");
    qm[3] = QPixmap(":/resources/enemyicon.png");
    qm[4] = QPixmap(":/resources/rubbish.png");
    qm[5] = QPixmap(":/resources/skill.png");
    qm[6] = QPixmap(":/resources/withdraw.png");
    qm[7] = QPixmap(":/resources/nullaffix.png");
    qm[8] = QPixmap(":/resources/win.png");
    qm[9] = QPixmap(":/resources/lose.png");
    qm[10] = QPixmap(":/resources/tips1.png");
    qm[11] = QPixmap(":/resources/tips2.png");
    qm[12] = QPixmap(":/resources/firerain.png");
    qm[13] = QPixmap(":/resources/progress_state.png");
    qm[14] = QPixmap(":/resources/pause_state.png");
    qm[15] = QPixmap(":/resources/pause_1_7.png");
    qm[16] = QPixmap(":/resources/pause_6_16.png");
    qm[17] = QPixmap(":/resources/pause_h_8_4.png");
    qm[18] = QPixmap(":/resources/pause.png");

    //enemy
    qm[21] = QPixmap(":/resources/enemy1.png");
    qm[22] = QPixmap(":/resources/enemy2.png");
    qm[23] = QPixmap(":/resources/enemy3.png");
    qm[24] = QPixmap(":/resources/enemy4.png");
    qm[25] = QPixmap(":/resources/enemy5.png");
    qm[26] = QPixmap(":/resources/enemy6.png");
    qm[27] = QPixmap(":/resources/enemy7.png");
    qm[28] = QPixmap(":/resources/enemy8.png");
    qm[29] = QPixmap(":/resources/enemy9.png");
    qm[30] = QPixmap(":/resources/enemy10.png");
    qm[31] = QPixmap(":/resources/enemy11.png");
    qm[32] = QPixmap(":/resources/enemy12.png");
    qm[33] = QPixmap(":/resources/enemy13.png");
    qm[34] = QPixmap(":/resources/enemy14.png");
    qm[35] = QPixmap(":/resources/enemy15.png");
    qm[36] = QPixmap(":/resources/enemy16.png");

    //operator
    qm[41] = QPixmap(":/resources/eyjafjalla.png");
    qm[42] = QPixmap(":/resources/exusiai.png");
    qm[43] = QPixmap(":/resources/hoshiguma.png");
    qm[44] = QPixmap(":/resources/texas.png");

    //operator_card
    qm[61] = QPixmap(":/resources/eyjafjalla_card.png");
    qm[62] = QPixmap(":/resources/exusiai_card.png");
    qm[63] = QPixmap(":/resources/hoshiguma_card.png");
    qm[64] = QPixmap(":/resources/texas_card.png");

    //affix
    qm[81] = QPixmap(":/resources/affix1.png");
    qm[82] = QPixmap(":/resources/affix2.png");
    qm[83] = QPixmap(":/resources/affix3.png");
    qm[84] = QPixmap(":/resources/affix4.png");
    qm[85] = QPixmap(":/resources/affix5.png");
    qm[86] = QPixmap(":/resources/affix6.png");

    //affix_card
    qm[101] = QPixmap(":/resources/affixcard1.png");
    qm[102] = QPixmap(":/resources/affixcard2.png");
    qm[103] = QPixmap(":/resources/affixcard3.png");
    qm[104] = QPixmap(":/resources/affixcard4.png");
    qm[105] = QPixmap(":/resources/affixcard5.png");
    qm[106] = QPixmap(":/resources/affixcard6.png");

    //enemy_ice
    qm[121] = QPixmap(":/resources/enemy1_ice.png");
    qm[122] = QPixmap(":/resources/enemy2_ice.png");
    qm[123] = QPixmap(":/resources/enemy3_ice.png");
    qm[124] = QPixmap(":/resources/enemy4_ice.png");
    qm[125] = QPixmap(":/resources/enemy5_ice.png");
    qm[126] = QPixmap(":/resources/enemy6_ice.png");
    qm[127] = QPixmap(":/resources/enemy7_ice.png");
    qm[128] = QPixmap(":/resources/enemy8_ice.png");
    qm[129] = QPixmap(":/resources/enemy9_ice.png");
    qm[130] = QPixmap(":/resources/enemy10_ice.png");
    qm[131] = QPixmap(":/resources/enemy11_ice.png");
    qm[132] = QPixmap(":/resources/enemy12_ice.png");
    qm[133] = QPixmap(":/resources/enemy13_ice.png");
    qm[134] = QPixmap(":/resources/enemy14_ice.png");
    qm[135] = QPixmap(":/resources/enemy15_ice.png");
    qm[136] = QPixmap(":/resources/enemy16_ice.png");

    //enemy_blood
    qm[141] = QPixmap(":/resources/enemy1_blood.png");
    qm[142] = QPixmap(":/resources/enemy2_blood.png");
    qm[143] = QPixmap(":/resources/enemy3_blood.png");
    qm[144] = QPixmap(":/resources/enemy4_blood.png");
    qm[145] = QPixmap(":/resources/enemy5_blood.png");
    qm[146] = QPixmap(":/resources/enemy6_blood.png");
    qm[147] = QPixmap(":/resources/enemy7_blood.png");
    qm[148] = QPixmap(":/resources/enemy8_blood.png");
    qm[149] = QPixmap(":/resources/enemy9_blood.png");
    qm[150] = QPixmap(":/resources/enemy10_blood.png");
    qm[151] = QPixmap(":/resources/enemy11_blood.png");
    qm[152] = QPixmap(":/resources/enemy12_blood.png");
    qm[153] = QPixmap(":/resources/enemy13_blood.png");
    qm[154] = QPixmap(":/resources/enemy14_blood.png");
    qm[155] = QPixmap(":/resources/enemy15_blood.png");
    qm[155] = QPixmap(":/resources/enemy16_blood.png");

    //map
    //1-7
    qm[161] = QPixmap(":/resources/ground_ok.png");
    qm[162] = QPixmap(":/resources/high_ok.png");
    qm[163] = QPixmap(":/resources/ground_no.png");
    qm[164] = QPixmap(":/resources/high_no.png");
    qm[165] = QPixmap(":/resources/blue.png");
    qm[166] = QPixmap(":/resources/red.png");
    qm[167] = QPixmap(":/resources/bomb.png");
    //6-16
    qm[168] = QPixmap(":/resources/rabbit1.png");
    qm[169] = QPixmap(":/resources/rabbit2.png");
    qm[170] = QPixmap(":/resources/rabbit3.png");
    qm[171] = QPixmap(":/resources/rabbit4.png");
    qm[172] = QPixmap(":/resources/rabbit5.png");
    qm[173] = QPixmap(":/resources/rabbit6.png");
    qm[174] = QPixmap(":/resources/rabbit7.png");
    qm[175] = QPixmap(":/resources/rabbit8.png");
    //H8-4
    qm[176] = QPixmap(":/resources/fire1.png");
    qm[177] = QPixmap(":/resources/fire2.png");
    qm[178] = QPixmap(":/resources/fire3.png");
    qm[179] = QPixmap(":/resources/fire4.png");
    qm[180] = QPixmap(":/resources/fire5.png");

    //enemy_bullet
    qm[201] = QPixmap(":/resources/enemy_bullet_1.png");

    //effect
    qm[221] = QPixmap(":/resources/bomb_effect.png");
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    DrawMap(painter);
    DrawEnemy(painter);
    DrawCard(painter);
    DrawOperator(painter);
    DrawBullet(painter);
    DrawEnemyBullet(painter);
    DrawIcon(painter);
    DrawControlOpe(painter);
    DrawResult(painter);
    DrawInfo(painter);
    DrawFire(painter);
    DrawPause(painter);
}

void MainWindow::mousePressEvent(QMouseEvent * ev){
    if(loseflag || winflag){
        this->close();
    }

    int clickx = ev->x();
    int clicky = ev->y();
    int xi = clickx/80;
    int yi = (clicky-40)/80;

    //暂停
    if(isTimeout){
        if(mapHeight-80 <= clicky && clicky <= mapHeight && mapWidth-80 <= clickx && clickx <= mapWidth){
            timer1->start(1000);
            timer2->start(30);
            timer3->start(60);
            if(istimer8){
                timer8->start(10);
            }
            if(istimer9){
                timer9->start(30);
            }
            if(istimer10){
                timer10->start(20);
            }
            for(int i = 0;i<Ops.size();++i){
                if(opetimer[i][0] == 1)
                    Ops[i].first->get_wait_timer()->start(50);
                if(opetimer[i][1] == 1)
                    Ops[i].first->get_use_timer()->start(50);
                if(opetimer[i][2] == 1)
                    Ops[i].first->get_m_fireRateTime()->start(Ops[i].first->get_atkSpeed()*500);
            }
            opetimer.clear();
            isTimeout = false;
            tipslabel->setText(QString("继续游戏！"));
            return;
        }
        else{
            return;
        }
    }
    else{
        if(mapHeight-80 <= clicky && clicky <= mapHeight && mapWidth-80 <= clickx && clickx <= mapWidth){
            timer1->stop();
            timer2->stop();
            timer3->stop();
            if(istimer8){
                timer8->stop();
            }
            if(istimer9){
                timer9->stop();
            }
            if(istimer10){
                timer10->stop();
            }
            for(int i = 0;i<Ops.size();++i){
                int iswait = 0, isskill = 0, isattack = 0;
                if(Ops[i].first->get_wait_timer()->isActive()){
                    iswait = 1;
                    Ops[i].first->get_wait_timer()->stop();
                }
                if(Ops[i].first->get_use_timer()->isActive()){
                    isskill = 1;
                    Ops[i].first->get_use_timer()->stop();
                }
                if(Ops[i].first->get_m_fireRateTime()->isActive()){
                    isattack = 1;
                    Ops[i].first->get_m_fireRateTime()->stop();
                }
                vector<int> ot = {iswait,isskill,isattack};
                opetimer.push_back(ot);
            }
            isTimeout = true;
            tipslabel->setText(QString("暂停成功！"));
            return;
        }
    }

    if(operatorChosed == -1){
        if(mapHeight-160 <= clicky && clicky <= mapHeight-80)   //选中卡牌，准备摆放
        {
            if(xi<OperatorVec.size()){
                operatorChosed = xi;
            }
            if(!Opc.empty()){
                Opc.clear();
            }
        }
        else if(mapHeight-80 <= clicky && clicky <= mapHeight)  //选中词缀，准备给干员装备
        {
            if(xi<AffixVec.size()){
                operatorChosed = xi + 100;
            }
            if(!Opc.empty()){
                Opc.clear();
            }
        }
        else                                                    //选中已摆放干员，开技能 or 准备撤退 or 卸下词缀 or 选中场地装置引爆
        {
                                                                //选中干员
            if(Opc.empty()){                                    //此时没有干员被选中
                for(int i = 0;i<Ops.size();++i){
                    if(Ops[i].second.first == xi && Ops[i].second.second == yi){
                        Opc.push_back(Ops[i]);
                        break;
                    }
                }
            }
            else{                                               //此时已有干员被选中
                int cx = Opc[0].second.first;
                int cy = Opc[0].second.second;
                if(cx*80<=clickx && clickx<=cx*80+40 && cy*80<=clicky && clicky<=cy*80+40){
                    int res = Opc[0].first->useskill();
                    if(res == -1)
                        tipslabel->setText(QString("技力不足！"));
                    else{                                       //播放开技能语音
                        effect = new QSoundEffect(this);
                        switch(Opc[0].first->get_id()){
                        case 1:
                        {
                            int rand_num = rand() % 3;
                            if(rand_num == 0)
                                effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_eyj_skill1.wav"));
                            else if(rand_num == 1)
                                effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_eyj_skill2.wav"));
                            else
                                effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_eyj_skill3.wav"));
                            effect->setLoopCount(1);
                            effect->setVolume(1);
                            effect->play();
                            break;
                        }
                        case 2:
                        {
                            int rand_num = rand() % 3;
                            //cout<<"ok"<<endl;
                            if(rand_num == 0)
                                effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_exu_skill1.wav"));
                            else if(rand_num == 1)
                                effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_exu_skill2.wav"));
                            else
                                effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_exu_skill3.wav"));
                            //cout<<"ok"<<endl;
                            effect->setLoopCount(1);
                            effect->setVolume(1);
                            //cout<<"ok"<<endl;
                            effect->play();
                            //cout<<"ok"<<endl;
                            break;
                        }
                        case 3:
                        {
                            int rand_num = rand() % 3;
                            if(rand_num == 0)
                                effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_hos_skill1.wav"));
                            else if(rand_num == 1)
                                effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_hos_skill2.wav"));
                            else
                                effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_hos_skill3.wav"));
                            effect->setLoopCount(1);
                            effect->setVolume(1);
                            effect->play();
                            break;
                        }
                        case 4:
                        {
                            int rand_num = rand() % 3;
                            if(rand_num == 0)
                                effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_tex_skill1.wav"));
                            else if(rand_num == 1)
                                effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_tex_skill2.wav"));
                            else
                                effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_tex_skill3.wav"));
                            effect->setLoopCount(1);
                            effect->setVolume(1);
                            effect->play();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                        }
                    }
                }
                else if(cx*80+40<=clickx && clickx<=cx*80+63 && cy*80+10<=clicky && clicky<=cy*80+37){
                    tipslabel->setText(QString("撤退成功！"));
                    for(int i = 0;i<Ops.size();++i){
                        if(Ops[i].first == Opc[0].first){
                            Opc[0].first->withdraw();
                            this->cost += Opc[0].first->get_cost()/2;
                            OperatorVec.push_back(Opc[0].first);
                            Ops.erase(Ops.begin()+i);
                            break;
                        }
                    }
                    costlabel->setText(QString("费用：%1").arg(this->cost));
                    Opc.clear();
                    update();
                }
                else if(cx*80-20<=clickx && clickx<=cx*80+3 && cy*80+40<=clicky && clicky<=cy*80+63){
                    if(Opc[0].first->get_aff1() != NULL){
                        if(AffixVec.size()<=8){
                            AffixVec.push_back(Opc[0].first->get_aff1());
                        }

                        //卸下之后做出的改变
                        int at = Opc[0].first->get_aff1()->get_id();
                        if(at == 1){
                            Opc[0].first->set_atk(Opc[0].first->get_atk()/2);
                            Opc[0].first->set_atkSpeed(Opc[0].first->get_atkSpeed()*2);
                        }

                        Opc[0].first->set_aff1(Opc[0].first->get_aff2());
                        Opc[0].first->set_aff2(NULL);
                        tipslabel->setText(QString("卸下成功！"));
                    }
                }
                else if(cx*80-20<=clickx && clickx<=cx*80+3 && cy*80+80<=clicky && clicky<=cy*80+103){
                    if(Opc[0].first->get_aff2() != NULL){
                        if(AffixVec.size()<=8){
                            AffixVec.push_back(Opc[0].first->get_aff2());
                        }

                        //卸下之后做出的改变
                        int at = Opc[0].first->get_aff2()->get_id();
                        if(at == 1){
                            Opc[0].first->set_atk(Opc[0].first->get_atk()/2);
                            Opc[0].first->set_atkSpeed(Opc[0].first->get_atkSpeed()*2);
                        }

                        Opc[0].first->set_aff2(NULL);
                        tipslabel->setText(QString("卸下成功！"));
                    }
                }
                else{
                    Opc.clear();
                }
                //Opc.clear();
            }
        }
    }
    else{
        if(mapHeight-160 <= clicky && clicky <= mapHeight-80)   //更换选择干员
        {
            if(xi<OperatorVec.size()){
                if(xi == operatorChosed)
                    operatorChosed = -1;
                else
                    operatorChosed = xi;
            }
        }
        else if(mapHeight-80 <= clicky && clicky <= mapHeight)  //更换选择词缀，准备给干员装备 or 丢弃词缀
        {
            if(xi<AffixVec.size()){
                if(xi == operatorChosed - 100)
                    operatorChosed = -1;
                else
                    operatorChosed = xi + 100;
            }
            else if(clickx >= mapWidth - 185 && clickx <= mapWidth - 105 && operatorChosed >= 100){
                AffixVec.erase(AffixVec.begin()+operatorChosed-100);
                operatorChosed = -1;
                tipslabel->setText(QString("丢弃词缀成功！"));
            }
        }
        else if(operatorChosed < 100 && yi >= 0)                //摆放干员
        {                                                       //1近战位，2高台位，3不可部署近战位，4不可部署高台位，5蓝门（不可部署），6红门（不可部署），7震撼装置
            //cout<<xi<<yi<<endl;
            int ti = MapVec[yi][xi];
            int opeType = OperatorVec[operatorChosed]->get_position();

            int flag = 0;
            for(int i = 0;i<Ops.size();++i){
                if(Ops[i].second.first==xi && Ops[i].second.second==yi)
                    flag = -1;
            }

            if(ti != opeType && ti != opeType + 7 && ti != opeType + 15 && (opeType != 1 || ti != 19)){
                flag = -1;
            }

            if(flag == 0){
                if(DeductionCost(OperatorVec[operatorChosed]->get_cost())){
                    OperatorVec[operatorChosed]->set_present(true);
                    OperatorVec[operatorChosed]->set_x(xi*80+40);
                    OperatorVec[operatorChosed]->set_y(yi*80+40+40);
                    //cout<<OperatorVec[operatorChosed]->get_x()<<' '<<OperatorVec[operatorChosed]->get_y()<<endl;
                    opcur = make_pair(OperatorVec[operatorChosed],make_pair(xi,yi));

                    effect = new QSoundEffect(this);
                    //部署语音
                    switch(OperatorVec[operatorChosed]->get_id()){
                    case 1:
                    {
                        effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_eyj_deploy.wav"));
                        effect->setLoopCount(1);
                        effect->setVolume(1);
                        effect->play();
                        break;
                    }
                    case 2:
                    {
                        effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_exu_deploy.wav"));
                        //cout<<"ok"<<endl;
                        effect->setLoopCount(1);
                        effect->setVolume(1);
                        //cout<<"ok"<<endl;
                        effect->play();
                        //cout<<"ok"<<endl;
                        break;
                    }
                    case 3:
                    {
                        effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_hos_deploy.wav"));
                        effect->setLoopCount(1);
                        effect->setVolume(1);
                        effect->play();
                        break;
                    }
                    case 4:
                    {
                        effect->setSource(QUrl::fromLocalFile("D:/QTprojects/22.10.27TowerDefence/TowerDefence/resources/voice_tex_deploy.wav"));
                        effect->setLoopCount(1);
                        effect->setVolume(1);
                        effect->play();
                        break;
                    }
                    default:
                    {
                        break;
                    }
                    }

                    OperatorVec.erase(OperatorVec.begin()+operatorChosed);
                    operatorChosed = -1;

                    //播放动画
                    giflabel->setGeometry(xi*80-30,yi*80-35+40,120,120);
                    movie = new QMovie;
                    movie->setFileName(opcur.first->get_deployPath());
                    movie->setCacheMode(QMovie::CacheAll);
                    QSize size0 = giflabel->size();
                    movie->setScaledSize(size0);
                    giflabel->setMovie(movie);
                    movie->start();

                    //动画计时器
                    timer4->start(opcur.first->get_dtime());
                }
            }
            else{
                tipslabel->setText(QString("不能放在那里！"));
            }
        }
        else if(yi >= 0)                                        //装备词缀
        {
            int flag = -1;
            for(int i = 0;i<Ops.size();++i){
                if(Ops[i].second.first == xi && Ops[i].second.second == yi){
                    flag = i;
                    break;
                }
            }

            if(flag == -1){
                tipslabel->setText(QString("该处没有干员！"));
            }
            else{
                //添加词缀，函数内自动判断是否已有该词缀（返回1）、是否词缀已满（返回2）、不匹配（返回3）、添加成功（返回4）
                int res = Ops[flag].first->set_aff(AffixVec[operatorChosed-100]);
                if(res == 1){
                    tipslabel->setText(QString("已装备该词缀！"));
                }
                else if(res == 2){
                    tipslabel->setText(QString("干员词缀已满！"));
                }
                else if(res == 3){
                    tipslabel->setText(QString("词缀不匹配！"));
                }
                else if(res == 4){
                    tipslabel->setText(QString("词缀装备成功！"));
                    //装备成功后的特效
                    int at = AffixVec[operatorChosed-100]->get_id();
                    if(at == 1){
                        Ops[flag].first->set_atk(Ops[flag].first->get_atk()*2);
                        Ops[flag].first->set_atkSpeed(Ops[flag].first->get_atkSpeed()/2);
                    }
                    AffixVec.erase(AffixVec.begin()+operatorChosed-100);
                    operatorChosed = -1;
                }
            }
        }
    }
}

void MainWindow::DrawMap(QPainter& painter){

    for(int i = 0;i < row;i++){
        QVector<int> cur;
        for(int  j = 0;j < col;j++){
            if(LevelNumber == 1)
                cur.push_back(map_1_7[i][j]);
            else if(LevelNumber == 2)
                cur.push_back(map_6_16[i][j]);
            else if(LevelNumber == 3)
                cur.push_back((map_h_8_4[i][j]));
        }
        this->MapVec.push_back(cur);
    }

    for (int j = 0; j < row; j++){
        for (int i = 0; i < col; i++){
            painter.drawPixmap(i * 80, j * 80+40, 80, 80,qm[MapVec[j][i]+160]);
        }
    }
}

void MainWindow::DrawEnemy(QPainter& painter){
    for (auto moni : EnemyVec){
        //画出敌人
        int w = moni->get_width();
        int b = moni->get_height();
        if(moni->get_icestate() == 1) //被冻结
            painter.drawPixmap(moni->get_x()-w/2, moni->get_y()-b/2-10, moni->get_width(), moni->get_height(), qm[moni->get_id()+120]);
        else if(moni->get_bloodstate() == 1) //被流血
            painter.drawPixmap(moni->get_x()-w/2, moni->get_y()-b/2-10, moni->get_width(), moni->get_height(), qm[moni->get_id()+140]);
        else
            painter.drawPixmap(moni->get_x()-w/2, moni->get_y()-b/2-10, moni->get_width(), moni->get_height(), qm[moni->get_id()+20]);

        //画敌人的词缀
        if(moni->get_aff1() != NULL){
            painter.drawPixmap(moni->get_x()-w/2+5, moni->get_y()-b/2-30, 23, 23, qm[moni->get_aff1()->get_id()+80]);
        }

        if(moni->get_aff2() != NULL){
            painter.drawPixmap(moni->get_x()+5, moni->get_y()-b/2-30, 23, 23, qm[moni->get_aff2()->get_id()+80]);
        }

        //画敌人的血条
        int healthBarWidth,healthBarHeight;
        QPoint healthBarPoint;
        healthBarWidth = moni->get_width();//设置血条的长度
        healthBarHeight = 5;//设置血条的宽度
        healthBarPoint = QPoint(moni->get_x()-w/2, moni->get_y()+b/2-10);//设置血条的左上点

        painter.setPen(Qt::NoPen);//画笔的颜色
        painter.setBrush(QBrush(QColor(0,0,0,80)));//刷子的颜色，用来填充内部
        QRect healthBarBackRect(healthBarPoint,QSize(healthBarWidth,healthBarHeight));
        painter.drawRect(healthBarBackRect);//画出血条

        //扣血矩形
        if(!moni->get_isboss()){
            painter.setBrush(QBrush(QColor(255,0,0,100)));
        }
        else{
            painter.setBrush(QBrush(QColor(255,255,0,100)));
        }
        double hbw = ((double)moni->get_health()/(double)moni->get_maxhp())*(double)healthBarWidth;
        QRect healthBarRect(healthBarPoint,QSize(hbw,healthBarHeight));
        painter.drawRect(healthBarRect);//画出当前血量血条

        QPoint qp;
        qp.setX(moni->get_x());
        qp.setY(moni->get_y());

        if(moni->get_id() == 15){
            painter.setBrush(QBrush(QColor(255,0,0,20)));
            painter.drawEllipse(qp,200,200);//画出帝国炮火中枢先兆者的攻击范围
        }
    }
    painter.setPen(Qt::SolidLine);//画笔的颜色
    painter.setBrush(QBrush(QColor(255,255,255,0)));
}

void MainWindow::DrawCard(QPainter& painter){
    for(int i = 0;i < OperatorVec.size();++i){//画出干员卡牌
        if(i != operatorChosed)
            painter.drawPixmap(i*80,mapHeight-160,80,80,qm[OperatorVec[i]->get_id()+60]);
        else
            painter.drawPixmap(i*80,mapHeight-180,80,80,qm[OperatorVec[i]->get_id()+60]);
    }
    for(int i = 0;i < AffixVec.size();++i){//画出词缀卡牌
        if(operatorChosed >= 100 && i == operatorChosed - 100)
            painter.drawPixmap(i*80,mapHeight-100,80,80,qm[AffixVec[i]->get_id()+100]);
        else
            painter.drawPixmap(i*80,mapHeight-80,80,80,qm[AffixVec[i]->get_id()+100]);
    }
}

void MainWindow::DrawOperator(QPainter& painter){
    for(int i = 0;i < Ops.size();++i){//画出干员
        int xi = Ops[i].second.first;
        int yi = Ops[i].second.second;
        painter.drawPixmap(xi*80-16,yi*80-24+40,104,104,qm[Ops[i].first->get_id()+40]);

        //画自己的血条
        int healthBarWidth=80;//设置血条的长度
        QPoint healthBarPoint=QPoint(xi*80, yi*80+80+40);//设置血条的左上点
        painter.setPen(Qt::NoPen);//画笔的颜色
        painter.setBrush(QBrush(QColor(0,0,0,80)));//刷子的颜色，用来填充内部
        QRect healthBarBackRect(healthBarPoint,QSize(healthBarWidth,5));
        painter.drawRect(healthBarBackRect);//画出血条
        QPoint skillBarPoint=QPoint(xi*80, yi*80+80+40+5);//设置技力条的左上点
        QRect skillBarBackRect(skillBarPoint,QSize(healthBarWidth,5));
        painter.drawRect(skillBarBackRect);//画出技力条

        //扣血矩形
        painter.setBrush(QBrush(QColor(0,0,255,100)));
        double hbw = ((double)Ops[i].first->get_health()/(double)Ops[i].first->get_maxhp())*(double)healthBarWidth;
        QRect healthBarRect(healthBarPoint,QSize(hbw,5));
        painter.drawRect(healthBarRect);//画出当前血量血条

        //技力矩形
        if(Ops[i].first->get_isskill()){
            painter.setBrush(QBrush(QColor(255,255,0,100)));
        }
        else{
            painter.setBrush(QBrush(QColor(64,224,205,100)));
        }
        double hbw2 = ((double)Ops[i].first->get_curskill()/(double)Ops[i].first->get_maxskill())*(double)healthBarWidth;
        QRect skillBarRect(skillBarPoint,QSize(hbw2,5));
        painter.drawRect(skillBarRect);//画出当前技力条

        if(Ops[i].first->get_isready()){
            painter.drawPixmap(xi*80,yi*80-40+40,40,40,qm[5]);
        }
    }
    painter.setPen(Qt::SolidLine);//画笔的颜色
    painter.setBrush(QBrush(QColor(255,255,255,0)));
}

void MainWindow::DrawControlOpe(QPainter& painter) {
    for(int i = 0;i < Opc.size();++i){
        int xi = Opc[i].second.first;
        int yi = Opc[i].second.second;
        painter.drawPixmap(xi*80,yi*80-40+40,40,40,qm[5]);
        painter.drawPixmap(xi*80+40,yi*80-30+40,23,23,qm[6]);

        QPoint qp;
        qp.setX(Opc[i].second.first*80+42);
        qp.setY(Opc[i].second.second*80+40+40);
        painter.setPen(Qt::NoPen);//画笔的颜色
        painter.setBrush(QBrush(QColor(0,0,255,20)));
        painter.drawEllipse(qp,Opc[i].first->get_range(),Opc[i].first->get_range());//画出干员的攻击范围

        //画出词缀
        if(Opc[i].first->get_aff1() == NULL){
            painter.drawPixmap(xi*80-20,yi*80+40,23,23,qm[7]);
        }
        else{
            painter.drawPixmap(xi*80-20,yi*80+40,23,23,qm[Opc[i].first->get_aff1()->get_id()+80]);
        }
        if(Opc[i].first->get_aff2() == NULL){
            painter.drawPixmap(xi*80-20,yi*80+40+40,23,23,qm[7]);
        }
        else{
            painter.drawPixmap(xi*80-20,yi*80+40+40,23,23,qm[Opc[i].first->get_aff2()->get_id()+80]);
        }
    }
    painter.setPen(Qt::SolidLine);//画笔的颜色
    painter.setBrush(QBrush(QColor(255,255,255,0)));
}

void MainWindow::DrawBullet(QPainter& painter){
    for(auto b : BulletVec){
        painter.drawPixmap(b->get_currentPos().rx(),b->get_currentPos().ry(),36,36,b->get_path());
    }
}

void MainWindow::DrawEnemyBullet(QPainter& painter){
    for(int i = 0;i<EnemyBulletVec.size();++i){
        painter.drawPixmap(EnemyBulletVec[i].second[0],EnemyBulletVec[i].second[1],65,125,qm[201]);
        //cout<<EnemyBulletVec[i].second[0]<<' '<<EnemyBulletVec[i].second[1]<<endl;
    }
    for(int i = 0;i<EffectVec.size();++i){
        painter.drawPixmap(EffectVec[i].second.first,EffectVec[i].second.second,90,80,qm[220+EffectVec[i].first]);
        //cout<<EnemyBulletVec[i].second[0]<<' '<<EnemyBulletVec[i].second[1]<<endl;
    }
}

void MainWindow::DrawIcon(QPainter& painter){
    painter.drawPixmap(mapWidth-185,mapHeight-155,20,20,qm[1]);
    painter.drawPixmap(mapWidth/2,10,20,20,qm[2]);
    painter.drawPixmap(mapWidth/2-125,10,20,20,qm[3]);
    painter.drawPixmap(mapWidth-185,mapHeight-80,80,80,qm[4]);
    if(!isTimeout)
        painter.drawPixmap(mapWidth-80,mapHeight-80,80,80,qm[13]);
    else
        painter.drawPixmap(mapWidth-80,mapHeight-80,80,80,qm[14]);
}

void MainWindow::DrawResult(QPainter& painter){
    if(loseflag){
        if(player->playbackState() == 1)
            player->stop();
        painter.drawPixmap(0,0,mapWidth,mapHeight,qm[9]);
    }
    else if(winflag){
        if(player->playbackState() == 1)
            player->stop();
        painter.drawPixmap(winpos,(mapHeight/2)-110,mapWidth,120,qm[8]);
    }
}

void MainWindow::DrawInfo(QPainter& painter){
    if(reverse7 != -1){
        switch(LevelNumber){
        case 2:{
            painter.drawPixmap(infopos,80,240,72,qm[10]);
            break;
        }
        case 3:{
            painter.drawPixmap(infopos,80,240,72,qm[11]);
            break;
        }
        default:{
            break;
        }
        }
    }
}

void MainWindow::DrawFire(QPainter& painter){
    for(int i = 0;i<fireball.size();++i){
        painter.drawPixmap(fireball[i][0],fireball[i][1],132,36,qm[12]);
    }
}

void MainWindow::DrawPause(QPainter& painter){
    /*if(isTimeout){
        if(LevelNumber == 1){
            painter.drawPixmap(0,40,mapWidth,mapHeight-200,qm[15]);
        }
        else if(LevelNumber == 2){
            painter.drawPixmap(0,40,mapWidth,mapHeight-200,qm[16]);
        }
        else{
            painter.drawPixmap(0,40,mapWidth,mapHeight-200,qm[17]);
        }
    }*/
    if(isTimeout){
        painter.drawPixmap((mapWidth/2)-205,(mapHeight/2)-150,410,180,qm[18]);
    }
    update();
}

void MainWindow::EnemyCreator(){
    switch (LevelNumber){
        case 1:
        {
            if(countTimer < 5){
                QVector<pair<int,int>> w = {make_pair(X80(5,1)),make_pair(X80(5,2)),make_pair(X80(3,2)),make_pair(X80(3,3)),make_pair(X80(1,3))};
                Affix* af5 = new Affix(5);
                //Affix* af6 = new Affix(6);
                Enemy* a;

                if(countTimer == 3)
                    a = new Enemy(6,w,X80(11,1),this,af5);
                else
                    a = new Enemy(1,w,X80(11,1),this);

                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else if(15 <= countTimer && countTimer < 20){
                QVector<pair<int,int>> w = {make_pair(X80(5,1)),make_pair(X80(5,2)),make_pair(X80(3,2)),make_pair(X80(3,3)),make_pair(X80(1,3))};
                Affix* af6 = new Affix(6);
                Enemy* a;

                if(countTimer == 16)
                    a = new Enemy(2,w,X80(11,1),this,af6);
                else
                    a = new Enemy(2,w,X80(11,1),this);

                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else if(30 <= countTimer && countTimer < 35){
                QVector<pair<int,int>> w = {make_pair(X80(5,3)),make_pair(X80(5,4)),make_pair(X80(3,4)),make_pair(X80(3,3)),make_pair(X80(1,3))};
                Affix* af5 = new Affix(5);
                //Affix* af6 = new Affix(6);
                Enemy* a;

                if(countTimer == 30)
                    a = new Enemy(3,w,X80(11,3),this,af5);
                else
                    a = new Enemy(3,w,X80(11,3),this);

                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else if(45 <= countTimer && countTimer < 50){
                QVector<pair<int,int>> w = {make_pair(X80(5,5)),make_pair(X80(5,4)),make_pair(X80(3,4)),make_pair(X80(3,3)),make_pair(X80(1,3))};
                //Affix* af5 = new Affix(5);
                Affix* af6 = new Affix(6);
                Enemy* a;

                if(countTimer == 46)
                    a = new Enemy(4,w,X80(11,5),this,af6);
                else
                    a = new Enemy(4,w,X80(11,5),this);

                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else if(countTimer == 60){
                QVector<pair<int,int>> w = {make_pair(X80(5,1)),make_pair(X80(5,2)),make_pair(X80(3,2)),make_pair(X80(3,3)),make_pair(X80(1,3))};
                Affix* af5 = new Affix(5);
                Enemy* a = new Enemy(6,w,X80(11,1),this,af5);
                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else if(countTimer == 75){
                QVector<pair<int,int>> w = {make_pair(X80(5,5)),make_pair(X80(5,4)),make_pair(X80(3,4)),make_pair(X80(3,3)),make_pair(X80(1,3))};
                Enemy* a = new Enemy(5,w,X80(11,5),this);
                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else{
                countTimer++;
            }
            break;
        }
        case 2:
        {
            if(countTimer == 1){
                QVector<pair<int,int>> w = {make_pair(X80(11,2)),make_pair(X80(8,2)),make_pair(X80(8,1)),make_pair(X80(5,2)),make_pair(X80(1,2)),
                                           make_pair(X80(10,4)),make_pair(X80(2,4)),make_pair(X80(10,6)),make_pair(X80(1,6))};
                //Affix* af5 = new Affix(5);
                //Affix* af6 = new Affix(6);
                Enemy* a = new Enemy(12,w,X80(12,2),this);

                //if(countTimer == 3)
                //    a = new Enemy(6,w,X80(11,1));
                //else
                //    a = new Enemy(1,w,X80(11,1));

                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else if(5 <= countTimer && countTimer < 10){
                QVector<pair<int,int>> w = {make_pair(X80(1,4))};
                QVector<pair<int,int>> v = {make_pair(X80(1,6))};
                //Affix* af5 = new Affix(5);
                //Affix* af6 = new Affix(6);
                Enemy* a;

                if(countTimer == 8)
                    a = new Enemy(8,v,X80(12,6),this);
                else
                    a = new Enemy(9,w,X80(12,4),this);

                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else if(25 <= countTimer && countTimer < 31){
                QVector<pair<int,int>> w = {make_pair(X80(1,4))};
                QVector<pair<int,int>> v = {make_pair(X80(1,6))};
                //Affix* af5 = new Affix(5);
                Affix* af6 = new Affix(6);
                Enemy* a;

                if(countTimer == 27)
                    a = new Enemy(10,w,X80(12,4),this,af6);
                else if(countTimer == 28)
                    a = new Enemy(10,v,X80(12,6),this);
                else if(countTimer == 29)
                    a = new Enemy(11,v,X80(12,6),this);
                else
                    a = new Enemy(7,w,X80(12,4),this);

                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else if(countTimer == 50){
                QVector<pair<int,int>> w = {make_pair(X80(1,4))};
                Enemy* a = new Enemy(8,w,X80(12,4),this);
                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else if(countTimer == 51){
                QVector<pair<int,int>> v = {make_pair(X80(1,6))};
                Enemy* a = new Enemy(8,v,X80(12,6),this);
                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else if(countTimer == 54){
                QVector<pair<int,int>> w = {make_pair(X80(1,4))};
                Enemy* a = new Enemy(10,w,X80(12,4),this);
                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else if(countTimer == 55){
                QVector<pair<int,int>> v = {make_pair(X80(1,6))};
                Enemy* a = new Enemy(10,v,X80(12,6),this);
                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else if(countTimer == 57){
                QVector<pair<int,int>> w = {make_pair(X80(1,4))};
                Enemy* a = new Enemy(11,w,X80(12,4),this);
                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else if(countTimer == 58){
                QVector<pair<int,int>> v = {make_pair(X80(1,6))};
                Enemy* a = new Enemy(11,v,X80(12,6),this);
                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else if(countTimer == 82){
                QVector<pair<int,int>> w = {make_pair(X80(1,4))};
                Enemy* a = new Enemy(8,w,X80(12,4),this);

                QVector<pair<int,int>> v = {make_pair(X80(1,6))};
                Enemy* b = new Enemy(8,v,X80(12,6),this);

                EnemyVec.push_back(a);
                EnemyVec.push_back(b);
                count+=2;
                countTimer++;
            }
            else if(countTimer == 84){
                QVector<pair<int,int>> w = {make_pair(X80(1,4))};
                Enemy* a = new Enemy(11,w,X80(12,4),this);

                QVector<pair<int,int>> v = {make_pair(X80(1,6))};
                Enemy* b = new Enemy(11,v,X80(12,6),this);

                EnemyVec.push_back(a);
                EnemyVec.push_back(b);
                count+=2;
                countTimer++;
            }
            else if(countTimer == 86){
                QVector<pair<int,int>> w = {make_pair(X80(1,4))};
                Enemy* a = new Enemy(10,w,X80(12,4),this);

                QVector<pair<int,int>> v = {make_pair(X80(1,6))};
                Enemy* b = new Enemy(10,v,X80(12,6),this);

                EnemyVec.push_back(a);
                EnemyVec.push_back(b);
                count+=2;
                countTimer++;
            }
            else{
                countTimer++;
            }
            break;
        }
        case 3:
        {
            if(countTimer == 1){
                QVector<pair<int,int>> w = {make_pair(X80(12,1)),make_pair(X80(11,1)),make_pair(X80(11,5)),make_pair(X80(9,5)),make_pair(X80(9,2)),make_pair(X80(7,2)),make_pair(X80(7,5)),make_pair(X80(5,5)),make_pair(X80(5,3)),make_pair(X80(4,3))};
                //QVector<pair<int,int>> v = {make_pair(X80(1,6))};
                //Affix* af5 = new Affix(5);
                //Affix* af6 = new Affix(6);
                Enemy* a = new Enemy(16,w,X80(13,1),this);

                EnemyVec.push_back(a);
                count++;
                countTimer++;
            }
            else if(countTimer == 5){
                QVector<pair<int,int>> w = {make_pair(X80(12,3)),make_pair(X80(9,3)),make_pair(X80(6,3)),make_pair(X80(2,3))};
                QVector<pair<int,int>> v = {make_pair(X80(12,4)),make_pair(X80(9,4)),make_pair(X80(6,4)),make_pair(X80(2,4))};
                Enemy* a = new Enemy(15,w,X80(13,3),this);
                Enemy* b = new Enemy(15,v,X80(13,4),this);

                EnemyVec.push_back(a);
                EnemyVec.push_back(b);
                count += 2;
                countTimer++;
            }
            else if(countTimer == 10){
                QVector<pair<int,int>> w = {make_pair(X80(9,3)),make_pair(X80(9,2)),make_pair(X80(7,2)),make_pair(X80(7,3)),make_pair(X80(4,3))};
                QVector<pair<int,int>> v = {make_pair(X80(11,4)),make_pair(X80(11,5)),make_pair(X80(9,5)),make_pair(X80(9,4)),make_pair(X80(7,4)),make_pair(X80(7,5)),make_pair(X80(5,5)),make_pair(X80(5,4)),make_pair(X80(4,4))};
                Enemy* a = new Enemy(13,w,X80(13,3),this);
                Enemy* b = new Enemy(13,v,X80(13,4),this);

                EnemyVec.push_back(a);
                EnemyVec.push_back(b);
                count += 2;
                countTimer++;
            }
            else if(countTimer == 15){
                QVector<pair<int,int>> w = {make_pair(X80(9,3)),make_pair(X80(9,2)),make_pair(X80(7,2)),make_pair(X80(7,3)),make_pair(X80(4,3))};
                QVector<pair<int,int>> v = {make_pair(X80(11,4)),make_pair(X80(11,5)),make_pair(X80(9,5)),make_pair(X80(9,4)),make_pair(X80(7,4)),make_pair(X80(7,5)),make_pair(X80(5,5)),make_pair(X80(5,4)),make_pair(X80(4,4))};
                Enemy* a = new Enemy(13,w,X80(13,3),this);
                Enemy* b = new Enemy(13,v,X80(13,4),this);

                EnemyVec.push_back(a);
                EnemyVec.push_back(b);
                count += 2;
                countTimer++;
            }
            else if(countTimer == 20){
                QVector<pair<int,int>> w = {make_pair(X80(11,1)),make_pair(X80(11,3)),make_pair(X80(9,3)),make_pair(X80(9,2)),make_pair(X80(7,2)),make_pair(X80(7,3)),make_pair(X80(4,3))};
                QVector<pair<int,int>> v = {make_pair(X80(11,6)),make_pair(X80(11,5)),make_pair(X80(9,5)),make_pair(X80(9,4)),make_pair(X80(7,4)),make_pair(X80(7,5)),make_pair(X80(5,5)),make_pair(X80(5,4)),make_pair(X80(4,4))};
                Enemy* a = new Enemy(14,w,X80(13,1),this);
                Enemy* b = new Enemy(14,v,X80(13,6),this);

                EnemyVec.push_back(a);
                EnemyVec.push_back(b);
                count += 2;
                countTimer++;
            }
            else if(countTimer == 25){
                QVector<pair<int,int>> w = {make_pair(X80(11,1)),make_pair(X80(11,3)),make_pair(X80(9,3)),make_pair(X80(9,2)),make_pair(X80(7,2)),make_pair(X80(7,3)),make_pair(X80(4,3))};
                QVector<pair<int,int>> v = {make_pair(X80(11,6)),make_pair(X80(11,5)),make_pair(X80(9,5)),make_pair(X80(9,4)),make_pair(X80(7,4)),make_pair(X80(7,5)),make_pair(X80(5,5)),make_pair(X80(5,4)),make_pair(X80(4,4))};
                Enemy* a = new Enemy(14,w,X80(13,1),this);
                Enemy* b = new Enemy(14,v,X80(13,6),this);

                EnemyVec.push_back(a);
                EnemyVec.push_back(b);
                count += 2;

                QVector<pair<int,int>> x = {make_pair(X80(9,3)),make_pair(X80(9,2)),make_pair(X80(7,2)),make_pair(X80(7,3)),make_pair(X80(4,3))};
                QVector<pair<int,int>> y = {make_pair(X80(11,4)),make_pair(X80(11,5)),make_pair(X80(9,5)),make_pair(X80(9,4)),make_pair(X80(7,4)),make_pair(X80(7,5)),make_pair(X80(5,5)),make_pair(X80(5,4)),make_pair(X80(4,4))};
                Enemy* c = new Enemy(13,x,X80(13,3),this);
                Enemy* d = new Enemy(13,y,X80(13,4),this);

                EnemyVec.push_back(c);
                EnemyVec.push_back(d);
                count += 2;

                countTimer++;
            }
            else if(countTimer == 35){
                QVector<pair<int,int>> w = {make_pair(X80(11,1)),make_pair(X80(11,3)),make_pair(X80(9,3)),make_pair(X80(9,2)),make_pair(X80(7,2)),make_pair(X80(7,3)),make_pair(X80(4,3))};
                QVector<pair<int,int>> v = {make_pair(X80(11,6)),make_pair(X80(11,5)),make_pair(X80(9,5)),make_pair(X80(9,4)),make_pair(X80(7,4)),make_pair(X80(7,5)),make_pair(X80(5,5)),make_pair(X80(5,4)),make_pair(X80(4,4))};
                Enemy* a = new Enemy(14,w,X80(13,1),this);
                Enemy* b = new Enemy(14,v,X80(13,6),this);

                EnemyVec.push_back(a);
                EnemyVec.push_back(b);
                count += 2;

                QVector<pair<int,int>> x = {make_pair(X80(9,3)),make_pair(X80(9,2)),make_pair(X80(7,2)),make_pair(X80(7,3)),make_pair(X80(4,3))};
                QVector<pair<int,int>> y = {make_pair(X80(11,4)),make_pair(X80(11,5)),make_pair(X80(9,5)),make_pair(X80(9,4)),make_pair(X80(7,4)),make_pair(X80(7,5)),make_pair(X80(5,5)),make_pair(X80(5,4)),make_pair(X80(4,4))};
                Enemy* c = new Enemy(13,x,X80(13,3),this);
                Enemy* d = new Enemy(13,y,X80(13,4),this);

                EnemyVec.push_back(c);
                EnemyVec.push_back(d);
                count += 2;

                countTimer++;
            }
            else if(countTimer == 45){
                QVector<pair<int,int>> w = {make_pair(X80(11,1)),make_pair(X80(11,3)),make_pair(X80(9,3)),make_pair(X80(9,2)),make_pair(X80(7,2)),make_pair(X80(7,3)),make_pair(X80(4,3))};
                QVector<pair<int,int>> v = {make_pair(X80(11,6)),make_pair(X80(11,5)),make_pair(X80(9,5)),make_pair(X80(9,4)),make_pair(X80(7,4)),make_pair(X80(7,5)),make_pair(X80(5,5)),make_pair(X80(5,4)),make_pair(X80(4,4))};
                Enemy* a = new Enemy(14,w,X80(13,1),this);
                Enemy* b = new Enemy(14,v,X80(13,6),this);

                EnemyVec.push_back(a);
                EnemyVec.push_back(b);
                count += 2;

                QVector<pair<int,int>> x = {make_pair(X80(9,3)),make_pair(X80(9,2)),make_pair(X80(7,2)),make_pair(X80(7,3)),make_pair(X80(4,3))};
                QVector<pair<int,int>> y = {make_pair(X80(11,4)),make_pair(X80(11,5)),make_pair(X80(9,5)),make_pair(X80(9,4)),make_pair(X80(7,4)),make_pair(X80(7,5)),make_pair(X80(5,5)),make_pair(X80(5,4)),make_pair(X80(4,4))};
                Enemy* c = new Enemy(13,x,X80(13,3),this);
                Enemy* d = new Enemy(13,y,X80(13,4),this);

                EnemyVec.push_back(c);
                EnemyVec.push_back(d);
                count += 2;

                countTimer++;
            }
            else if(countTimer == 50){
                QVector<pair<int,int>> w = {make_pair(X80(11,1)),make_pair(X80(11,3)),make_pair(X80(9,3)),make_pair(X80(9,2)),make_pair(X80(7,2)),make_pair(X80(7,3)),make_pair(X80(4,3))};
                QVector<pair<int,int>> v = {make_pair(X80(11,6)),make_pair(X80(11,5)),make_pair(X80(9,5)),make_pair(X80(9,4)),make_pair(X80(7,4)),make_pair(X80(7,5)),make_pair(X80(5,5)),make_pair(X80(5,4)),make_pair(X80(4,4))};
                Enemy* a = new Enemy(14,w,X80(13,1),this);
                Enemy* b = new Enemy(14,v,X80(13,6),this);

                EnemyVec.push_back(a);
                EnemyVec.push_back(b);
                count += 2;

                QVector<pair<int,int>> x = {make_pair(X80(9,3)),make_pair(X80(9,2)),make_pair(X80(7,2)),make_pair(X80(7,3)),make_pair(X80(4,3))};
                QVector<pair<int,int>> y = {make_pair(X80(11,4)),make_pair(X80(11,5)),make_pair(X80(9,5)),make_pair(X80(9,4)),make_pair(X80(7,4)),make_pair(X80(7,5)),make_pair(X80(5,5)),make_pair(X80(5,4)),make_pair(X80(4,4))};
                Enemy* c = new Enemy(13,x,X80(13,3),this);
                Enemy* d = new Enemy(13,y,X80(13,4),this);

                EnemyVec.push_back(c);
                EnemyVec.push_back(d);
                count += 2;

                countTimer++;
            }
            else{
                countTimer++;
            }
            break;
        }
        default:
            break;
    }
}

QVector<Enemy*> MainWindow::get_enemyVec(){
    return EnemyVec;
}

QVector<pair<Operator*,pair<int,int>>> MainWindow::get_opsVec(){
    return Ops;
}

void MainWindow::addOperatorVec(Operator* o){
    OperatorVec.push_back(o);
}

void MainWindow::removeOps(Operator* o){
    for(int i = 0;i<Ops.size();++i){
        if(Ops[i].first == o){
            Ops.erase(Ops.begin()+i);
            break;
        }
    }
}

void MainWindow::addBullet(Bullet* b){
    BulletVec.push_back(b);
}

void MainWindow::removeBullet(Bullet* b){
    for(int i = 0;i<BulletVec.size();++i){
        if(BulletVec[i] == b){
            BulletVec.erase(BulletVec.begin()+i);
            break;
        }
    }
}

void MainWindow::addAffix(Affix* a){
    if(AffixVec.size()<8){
        AffixVec.push_back(a);
    }
}

void MainWindow::addcost(int i){
    cost += i;
    update();
}

void MainWindow::fireRain(){
    timer10->start(20);
    timer8->start(10);
    istimer8 = true;
    istimer10 = true;
    connect(timer10,&QTimer::timeout,[=](){
        fireNum += 20;
        //cout<<fireNum<<endl;
        if(fireNum % 2000 != 0)
            return;

        fireball.push_back({1040,80});
        fireball.push_back({1040,160});
        fireball.push_back({1040,240});
        fireball.push_back({1040,320});
        fireball.push_back({1040,400});
        fireball.push_back({1040,480});

        QVector<pair<int,int>> w = {make_pair(X80(9,3)),make_pair(X80(9,2)),make_pair(X80(7,2)),make_pair(X80(7,3)),make_pair(X80(4,3))};
        QVector<pair<int,int>> v = {make_pair(X80(11,4)),make_pair(X80(11,5)),make_pair(X80(9,5)),make_pair(X80(9,4)),make_pair(X80(7,4)),make_pair(X80(7,5)),make_pair(X80(5,5)),make_pair(X80(5,4)),make_pair(X80(4,4))};
        Enemy* a = new Enemy(13,w,X80(13,3),this);
        Enemy* b = new Enemy(13,v,X80(13,4),this);

        EnemyVec.push_back(a);
        EnemyVec.push_back(b);
        count += 2;

        if(fireNum == 20000){
            timer10->stop();
            istimer10 = false;
        }
    });
    connect(timer8,&QTimer::timeout,[=](){
        for(int i = 0;i<fireball.size();++i){
            fireball[i][0] -= 2;
            if(fireball[i][0] == 0){
                fireball.erase(fireball.begin()+i);
                break;
            }
            else{
                for(int j = 0;j<Ops.size();++j){
                    if(Ops[j].first->get_x() == fireball[i][0] && Ops[j].first->get_y() == fireball[i][1]){
                         int damage = max((int)(50),(int)(1000*(100-Ops[j].first->get_magicDef())*0.01));
                         Ops[j].first->set_health(Ops[j].first->get_health() - damage);
                         if(Ops[j].first->get_health() <= 0){
                             Ops[j].first->set_health(0);
                             Ops[j].first->withdraw();
                             OperatorVec.push_back(Ops[j].first);
                             Ops.erase(Ops.begin()+j);
                         }
                         fireball.erase(fireball.begin()+i);
                         break;
                    }
                }
            }
        }
    });

}

void MainWindow::sealFloor(int x,int y){
    //cout<<x<<' '<<y<<endl;
    vector<vector<int>> floor;
    for(int i = x-2;i<=x+2;++i){
        if(i >= 1 && i <= col){
            for(int j = y-2;j<=y+2;++j){
                if(j >= 1&& j <= row){
                    if(MapVec[j-1][i-1] == 8 || MapVec[j-1][i-1] == 9){
                        vector<int> tba = {j,i};
                        //cout<<j<<' '<<i<<endl;
                        floor.push_back(tba);
                    }
                }
            }
        }
    }
    int n = floor.size();
    if(n > 0){
        for(int i = 0;i<=2;++i){
            int rn = rand() % n;
            MapVec[floor[rn][0]-1][floor[rn][1]-1] = 14;
            for(int j = 0;j < Ops.size();++j){
                if(Ops[j].second.first == floor[rn][1]-1 && Ops[j].second.second == floor[rn][0]-1){
                    Ops[j].first->set_health(0);
                    Ops[j].first->withdraw();
                    OperatorVec.push_back(Ops[j].first);
                    Ops.erase(Ops.begin()+j);
                    break;
                }
            }
        }
    }
    update();
}

void MainWindow::bomber(Enemy* en,int ex,int ey){
    int bx = ex-40,by = 0;
    vector<int> curs = {bx,by,ex,ey};
    EnemyBulletVec.push_back(make_pair(en,curs));
}

void MainWindow::setMapWidth(int mw){
    mapWidth = mw;
}

void MainWindow::setMapHeight(int mh){
    mapHeight = mh;
}

void MainWindow::setrow(int r){
    row = r;
}

void MainWindow::setcol(int c){
    col = c;
}

void MainWindow::setcountSum(int cs){
    countSum = cs;
}

void MainWindow::setwinpos(int wp){
    winpos = wp;
}

void MainWindow::setinfopos(int ip){
    infopos = ip;
}

void MainWindow::setMapVec(QVector<QVector<int>> mv){
    for(int i = 0;i<mv.size();++i){
        QVector<int> cur;
        for(int j = 0;j<mv[i].size();++j){
            cur.append(mv[i][j]);
        }
        MapVec.append(cur);
    }
}

void MainWindow::reinit(){
    setFixedSize(mapWidth, mapHeight);
    costlabel->move(mapWidth-160, mapHeight-160);
    setStyleSheet("color:black");
    costlabel->setFont(QFont("微软雅黑", 12));
    costlabel->setText(QString("费用：%1").arg(this->cost));

    lifelabel->move(mapWidth/2-100, 5);
    setStyleSheet("color:black");
    lifelabel->setFont(QFont("微软雅黑", 12));
    lifelabel->setText(QString("生命：%1").arg(this->life));

    countlabel->move(mapWidth/2+30, 5);
    setStyleSheet("color:black");
    countlabel->setFont(QFont("微软雅黑", 12));
    countlabel->setText(QString("敌人：%1 / %2").arg(this->count).arg(this->countSum));

    tipslabel->move(mapWidth-185, mapHeight-125);
    setStyleSheet("color:black");
    tipslabel->setFont(QFont("微软雅黑", 12));
}
