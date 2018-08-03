#ifndef CELLURALAUTOMATON_H
#define CELLURALAUTOMATON_H

#include <QWidget>
#include <QDialog>
#include "qcustomplot.h"

class CelluralAutomaton : public QWidget
{
    Q_OBJECT
public:
    explicit CelluralAutomaton(QWidget *parent = 0);
    QCustomPlot *customPlot;
    QCPGraph * graph1;
    QCPDataMap  *mapData;
    QGridLayout *mainLayout;
    QPushButton * btn;
    QGroupBox * grp_bx;
    QHBoxLayout *hl;
    QGridLayout *gl;
    QLabel * ls;
    QLabel * ld;
    QLabel * lN;
    QLabel * lm;
    QSpinBox * sb_s;
    QSpinBox * sb_d;
    QSpinBox * sb_N;
    QSpinBox * sb_m;
    QGridLayout *gl2;
    QLabel * lChi;
    QLabel * ld2;
    QLabel * lChicr;
    QLineEdit *le_Chi;
    QLineEdit *le_d;
    QLineEdit *le_Chicr;
    QGroupBox * grp_bx_2;
    void CreatorConnections();
    void clearPlot();
    double KA_random(int s, int A[]);

private slots:
    void mousePress();
    void mouseWheel();
    void ChartBuilding();
};

#endif // CELLURALAUTOMATON_H
