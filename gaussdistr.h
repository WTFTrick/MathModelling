#ifndef GAUSSDISTR_H
#define GAUSSDISTR_H

#include <QWidget>
#include <QDialog>
#include "qcustomplot.h"
#include <math.h>

class GaussDistr : public QWidget
{
    Q_OBJECT
public:
    explicit GaussDistr(QWidget *parent = 0);
    QCustomPlot *customPlot;
    QCPDataMap  *mapData;
    QCPGraph * graph1;
    QGridLayout *mainLayout;
    QPushButton * btn;
    QGroupBox * grp_bx;
    QHBoxLayout *hl;
    QGridLayout *gl;
    QLabel * la;
    QLabel * ls;
    QLabel * lN;
    QLabel * lm;
    QDoubleSpinBox * sb_a;
    QDoubleSpinBox * sb_s;
    QRadioButton * rb_rejection;
    QRadioButton * rb_clt;
    QSpinBox * sb_N;
    QSpinBox * sb_m;
    QGridLayout *gl2;
    QLabel * lChi;
    QLabel * lAest;
    QLabel * lSest;
    QLabel * lChicr;
    QLineEdit *le_Chi;
    QLineEdit *le_Aest;
    QLineEdit *le_Sest;
    QLineEdit *le_Chicr;
    QGroupBox * grp_bx_2;
    void CreatorConnections();
    void clearPlot();

private slots:
    void mousePress();
    void mouseWheel();
    void GaussChartBuilding();
};

#endif // GAUSSDISTR_H
