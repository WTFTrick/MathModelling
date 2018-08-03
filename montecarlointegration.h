#ifndef MONTECARLOINTEGRATION_H
#define MONTECARLOINTEGRATION_H

#include <QWidget>
#include <QDialog>
#include "qcustomplot.h"

#include <QtCore/qmath.h>
#include <QtWidgets/QComboBox>

class MonteCarloIntegration : public QWidget
{
    Q_OBJECT
public:
    explicit MonteCarloIntegration(QWidget *parent = 0);

private:
    QGridLayout *mainLayout;
    QPushButton * btn;
    QGroupBox * grp_bx;
    QVBoxLayout *vl;
    QHBoxLayout *hl;
    QGridLayout *gl;
    QLabel * Name;
    QLabel * lN;
    QSpinBox * sb_N;
    QGridLayout *gl2;
    QLabel * lV;
    QLabel * lPi_est;
    QLabel * lEpsilon;
    QLineEdit *le_V;
    QLineEdit *le_Pi_est;
    QLineEdit *le_E_epsilon;
    QGroupBox * grp_bx_2;
    void CreatorConnections();

private slots:
    void MonteCarloCounting();
};

#endif // MONTECARLOINTEGRATION_H
