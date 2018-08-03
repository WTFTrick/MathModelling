#ifndef LEASTSQUARES_H
#define LEASTSQUARES_H

#include <QWidget>
#include <QDialog>
#include "qcustomplot.h"

class LeastSquares : public QWidget
{
    Q_OBJECT
public:
    explicit LeastSquares(QWidget *parent = 0);
    int yAxisRangeDiff;
    QCustomPlot *customPlot;
    QCPDataMap  *mapData;
    QCPGraph * graph1;
    QGridLayout *mainLayout;
    QPushButton * btn;
    QGroupBox * grp_bx;
    QHBoxLayout *hl;
    QGridLayout *gl;
    QLabel * la0;
    QLabel * lb0;
    QLabel * lN;
    QLabel * lSigma;
    QDoubleSpinBox * sb_a0;
    QDoubleSpinBox * sb_b0;
    QSpinBox * sb_N;
    QDoubleSpinBox * sb_Sigma;
    QGridLayout *gl2;
    QLabel * lA0;
    QLabel * lB0;
    QLabel * la;
    QLabel * lb;
    QLabel * lAs;
    QLabel * lBs;
    QLabel * lss;
    QLineEdit *le_a0;
    QLineEdit *le_b0;
    QLineEdit *le_A;
    QLineEdit *le_B;
    QLineEdit *le_As;
    QLineEdit *le_Bs;
    QLineEdit *le_ss;
    QGroupBox * grp_bx_2;
    void CreatorConnections();
    void clearPlot();
    void CreateNewDot(double x, double y);

private slots:
    void mousePress();
    void mouseWheel();
    void LeastSquaresChartBuilding();
};

#endif // LEASTSQUARES_H
