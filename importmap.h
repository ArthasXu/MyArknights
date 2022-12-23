#ifndef IMPORTMAP_H
#define IMPORTMAP_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>
#include <QString>
#include <iostream>

namespace Ui {
class importMap;
}

class importMap : public QWidget
{
    Q_OBJECT

public:
    explicit importMap(QWidget *parent = nullptr);
    ~importMap();

private:
    Ui::importMap *ui;

    int row = 0;
    int col = 0;

    QVector<QVector<int>> qvi;

    bool importable = false;
};

#endif // IMPORTMAP_H
