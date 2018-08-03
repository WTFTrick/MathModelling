#ifndef EXPONENTIALDISTR_H
#define EXPONENTIALDISTR_H

#include <QWidget>
#include <QDialog>
#include "qcustomplot.h"

class ExponentialDistr : public QWidget
{
    Q_OBJECT
public:
    explicit ExponentialDistr(QWidget *parent = 0);
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
    QDoubleSpinBox * sb_p;
    QSpinBox * sb_N;
    QSpinBox * sb_m;
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
    void ExpoChartBuilding();
};

#endif // EXPONENTIALDISTR_H
