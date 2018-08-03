#ifndef UNIFORMDISTR_H
#define UNIFORMDISTR_H

#include <QWidget>
#include <QDialog>
#include "qcustomplot.h"

class UniformDistr : public QWidget
{
    Q_OBJECT
public:
    explicit UniformDistr(QWidget *parent = 0);
    QCustomPlot *customPlot;
    QCPDataMap  *mapData;
    QCPGraph * graph1;
    QGridLayout *mainLayout;
    QPushButton * btn;
    QGroupBox * grp_bx;
    QHBoxLayout *hl;
    QGridLayout *gl;
    QLabel * la;
    QLabel * lb;
    QLabel * lN;
    QLabel * lm;
    QDoubleSpinBox * sb_a;
    QDoubleSpinBox * sb_b;
    QSpinBox * sb_N;
    QSpinBox * sb_m;
    QGridLayout *gl2;
    QLabel * lChi;
    QLabel * lAest;
    QLabel * lBest;
    QLabel * lChicr;
    QLineEdit *le_Chi;
    QLineEdit *le_Aest;
    QLineEdit *le_Best;
    QLineEdit *le_Chicr;
    QGroupBox * grp_bx_2;
    void CreatorConnections();
    void clearPlot();

private slots:
    void mousePress();
    void mouseWheel();
    void UniformlChartBuilding();
};

#endif // UNIFORMDISTR_H
