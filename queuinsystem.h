#ifndef QUEUINSYSTEM_H
#define QUEUINSYSTEM_H

#include <QWidget>
#include <QDialog>
#include "qcustomplot.h"

class QueuinSystem : public QWidget
{
    Q_OBJECT
public:
    explicit QueuinSystem(QWidget *parent = 0);
    QGridLayout *mainLayout;
    QVBoxLayout *vl;
    QHBoxLayout *hl;
    QGridLayout *gl;
    QGridLayout *gl2;
    QPushButton * btn;
    QGroupBox * grp_bx;
    QLabel * Name;
    QLabel * ls;
    QLabel * ld;
    QLabel * lN;
    QLabel * lm;
    QDoubleSpinBox * sb_lambda;
    QSpinBox * sb_t_serv;
    QSpinBox * sb_N;
    QSpinBox * sb_m;
    QGroupBox * grp_bx_2;
    QLabel * l_T_line;
    QLabel * l_T_sys;
    QLabel * l_N_vac;
    QLabel * l_A;
    QLabel * l_Q;
    QLineEdit *le_T_line;
    QLineEdit *le_T_sys;
    QLineEdit *le_vac;
    QLineEdit *le_Q;
    QLineEdit *le_A;

    void CreatorConnections();
    int Factorial(int number);

private slots:
    void Count();
};

#endif // QUEUINSYSTEM_H
