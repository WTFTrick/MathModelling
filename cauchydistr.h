#ifndef CAUCHYDISTR_H
#define CAUCHYDISTR_H

#include <QWidget>
#include <QDialog>
#include "qcustomplot.h"

class CauchyDistr : public QWidget
{
    Q_OBJECT
public:
    explicit CauchyDistr(QWidget *parent = 0);
    QCustomPlot *customPlot;
    QCPGraph * graph1;
    QCPDataMap  *mapData;
    QGridLayout *mainLayout;
    QPushButton * btn;
    QGroupBox * grp_bx;
    QHBoxLayout *hl;
    QGridLayout *gl;
    QLabel * lp;
    QLabel * lN;
    QLabel * lm;
    QDoubleSpinBox * sb_p;
    QSpinBox * sb_N;
    QSpinBox * sb_m;
    QGridLayout *gl2;
    QLabel * lChi;
    QLabel * lPest;
    QLabel * lChicr;
    QLineEdit *le_Chi;
    QLineEdit *le_Pest;
    QLineEdit *le_Chicr;
    QGroupBox * grp_bx_2;
    void CreatorConnections();
    void clearPlot();

private slots:
    void mousePress();
    void mouseWheel();
    void CauchyChartBuilding();

signals:

public slots:
};

#endif // CAUCHYDISTR_H
