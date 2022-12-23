#include "importmap.h"
#include "ui_importmap.h"
#include "mainwindow.h"

importMap::importMap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::importMap)
{
    ui->setupUi(this);
    setWindowTitle("选择文件");

    //选择文件
    connect(ui->pushButton,&QPushButton::clicked,[=](){
       QString filename=QFileDialog::getOpenFileName(this,"打开文件");
       ui->lineEdit->setText(filename);

       QFile file(filename);
       file.open(QIODevice::ReadWrite);
       QByteArray array = file.readAll();
       ui->textEdit->setText(array);
       file.close();

       if((array[0] <= '0' || array[0] > '9') || (array[2] <= '0' || array[2] > '9')){
           importable = false;
           std::cout<<"error1!"<<std::endl;
           return;
       }
       else{
           //读行列 1<=行<=9 1<=列<=99
           row = array[0]-'0';
           if((array[3] >= '0' && array[3] <= '9')){
               col = (array[2]-'0')*10+(array[3]-'0');
           }
           else{
               col = array[2]-'0';
           }
           //std::cout<<row<<' '<<col<<std::endl;
           //读文件 读入地块类型
           QStringList qsl;
           QString qcur = "";
           bool err = false;
           for(int j = 4;j<array.size();++j){
               if(j == 4 && col >= 10)
                    continue;
               if('0' <= array[j] && array[j] <= '9'){
                   qcur += array[j];
                   if(j == array.size()-1){
                       qsl.append(qcur);
                       qcur = "";
                   }
               }
               else if(array[j] == ' ' || array[j] == '\n'){
                   if(qcur != ""){
                       qsl.append(qcur);
                       qcur = "";
                   }
               }
               else if('0' > array[j] && array[j] > '9'){
                   std::cout<<array[j]<<std::endl;
                   err = true;
                   break;
               }
           }
           //存在地块不是数字的情况
           if(err){
               importable = false;
               std::cout<<"error2!"<<std::endl;
               return;
           }

           /**
           for(int i = 0;i<qsl.size();++i){
               std::cout<<qsl[i].toInt()<<std::endl;
           }
           **/


           //地块数量和行列不一致
           if(qsl.size() != row*col){
               importable = false;
               std::cout<<"error3!"<<' '<<qsl.size()<<' '<<row*col<<std::endl;
               return;
           }

           for(int i = 0;i<row;++i){
               QVector<int> qicur;
               for(int j = 0;j<col;++j){
                   qicur.append(qsl[i*col+j].toInt());
               }
               qvi.append(qicur);
           }

           /**
           for(int i = 0;i<qvi.size();++i){
               for(int j = 0;j<qvi[i].size();++j){
                   std::cout<<qvi[i][j]<<' ';
               }
               std::cout<<std::endl;
           }
           **/

           importable = true;
       }
    });

    connect(ui->pushButton_2,&QPushButton::clicked,[=](){
        if(!importable){
            QMessageBox::information(this, "ERROR", "尚未导入地图或者地图格式有误！",QStringLiteral("确定"));
        }
        else{
            int mapWidth = 80*col;
            int mapHeight = 40+160+80*row;
            //row = 6;
            //col = 13;
            int countSum = 1;
            int winpos = mapWidth;
            int infopos = mapWidth;

            MainWindow* mw4 = new MainWindow(4);
            mw4->move((1600 - mapWidth) / 2, (800 - mapHeight) / 2);
            mw4->show();

            mw4->setMapWidth(mapWidth);
            mw4->setMapHeight(mapHeight);
            mw4->setrow(row);
            mw4->setcol(col);
            mw4->setcountSum(countSum);
            mw4->setwinpos(winpos);
            mw4->setinfopos(infopos);
            mw4->setMapVec(qvi);
            mw4->reinit();

            this->close();
        }
    });
}

importMap::~importMap()
{
    delete ui;
}
