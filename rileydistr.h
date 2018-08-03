#ifndef RILEYDISTR_H
#define RILEYDISTR_H

#include <QWidget>
#include <QDialog>
#include "qcustomplot.h"

class RileyDistr : public QWidget
{
    Q_OBJECT
public:
    explicit RileyDistr(QWidget *parent = 0);
    QCustomPlot *customPlot;
    QCPDataMap  *mapData;
    QCPGraph * graph1;
    QGridLayout *mainLayout;
    QPushButton * btn;
    QGroupBox * grp_bx;
    QHBoxLayout *hl;
    QGridLayout *gl;
    QLabel * lp;
    QLabel * lN;
    QLabel * lm;
    QLabel * ld;
    QDoubleSpinBox * sb_sigma;
    QSpinBox * sb_N;
    QSpinBox * sb_m;
    QDoubleSpinBox * sb_d;
    QGridLayout *gl2;
    QLabel * lChi;
    QLabel * lSest;
    QLabel * lChicr;
    QLineEdit *le_Chi;
    QLineEdit *le_Sest;
    QLineEdit *le_Chicr;
    QGroupBox * grp_bx_2;
    void CreatorConnections();
    void clearPlot();

private slots:
    void mousePress();
    void mouseWheel();
    void RileyChartBuilding();
};

#endif // RILEYDISTR_H
