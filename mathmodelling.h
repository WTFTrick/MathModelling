#ifndef MATHMODELLING_H
#define MATHMODELLING_H

#include <QMainWindow>
#include "qcustomplot.h"

namespace Ui {
class MathModelling;
}

class MathModelling : public QMainWindow
{
    Q_OBJECT

public:
    explicit MathModelling(QWidget *parent = 0);
    ~MathModelling();

private:
    Ui::MathModelling *ui;
    void BinomialDistribution(double p, int N, int m);
    void CreatorConnections();
    void clearPlot();
    void Setups();

private slots:
    void mousePress();
    void mouseWheel();
    void BinomialChartBuilding();
};

#endif // MATHMODELLING_H
