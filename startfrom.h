#ifndef STARTFROM_H
#define STARTFROM_H

#include <QWidget>
#include <QMediaPlayer> //音乐播放器
#include <QIODevice>
#include <QAudioOutput>
#include <QFileDialog>

namespace Ui {
class startFrom;
}

class startFrom : public QWidget
{
    Q_OBJECT

public:
    explicit startFrom(QWidget *parent = nullptr);
    ~startFrom();
    void paintEvent(QPaintEvent *event);

private:
    Ui::startFrom *ui;
};

#endif // STARTFROM_H
