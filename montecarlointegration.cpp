#include "montecarlointegration.h"

MonteCarloIntegration::MonteCarloIntegration(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QGridLayout;
    vl = new QVBoxLayout;
    hl = new QHBoxLayout;
    gl = new QGridLayout;

    btn = new QPushButton;
    btn->setText("Вычислить");

    grp_bx = new QGroupBox;

    Name = new QLabel;
    Name->setText("Метод Монте-Карло:");
    Name->setAlignment(Qt::AlignHCenter);
    QFont f( "Arial", 18, QFont::Bold);
    Name->setFont(f);

    lN = new QLabel;
    lN->setText("Количество случайных чисел:");
    lN->setAlignment(Qt::AlignHCenter);

    sb_N = new QSpinBox;
    sb_N->setMaximum(5000);
    sb_N->setMinimum(0);
    sb_N->setValue(5000);
    sb_N->setSingleStep(1);

    gl->addWidget(lN, 0, 0);

    gl->addWidget(sb_N, 1, 0);

    grp_bx->setLayout(gl);
    grp_bx->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    gl2 = new QGridLayout;
    lV = new QLabel;
    lV->setText("Объем:");
    lV->setAlignment(Qt::AlignHCenter);
    lPi_est = new QLabel;
    lPi_est->setText("Оценка Pi:");
    lPi_est->setAlignment(Qt::AlignHCenter);
    lEpsilon = new QLabel;
    lEpsilon->setText("Эпсилон:");
    lEpsilon->setAlignment(Qt::AlignHCenter);

    le_V = new QLineEdit;
    le_V->setReadOnly(true);
    le_Pi_est = new QLineEdit;
    le_Pi_est->setReadOnly(true);
    le_E_epsilon = new QLineEdit;
    le_E_epsilon->setReadOnly(true);

    gl2->addWidget(lV, 0, 0);
    gl2->addWidget(lPi_est, 0, 1);
    gl2->addWidget(lEpsilon, 0, 2);

    gl2->addWidget(le_V, 1, 0);
    gl2->addWidget(le_Pi_est, 1, 1);
    gl2->addWidget(le_E_epsilon, 1, 2);

    grp_bx_2 = new QGroupBox;
    grp_bx_2->setLayout(gl2);
    //grp_bx_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    vl->addWidget(Name);
    hl->addWidget(btn);
    hl->addWidget(grp_bx);
    hl->addWidget(grp_bx_2);

    mainLayout->addLayout(vl, 1, 0);
    mainLayout->addLayout(hl, 2, 0);
    mainLayout->setAlignment(Qt::AlignTop);
    setLayout(mainLayout);

    CreatorConnections();
}

void MonteCarloIntegration::CreatorConnections()
{
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(MonteCarloCounting()));
}

void MonteCarloIntegration::MonteCarloCounting()
{
    int N = sb_N->value();
    //const int N;
    double V = 0;
    double xin;
    double yin;
    double zin;
    double xout;
    double yout;
    double zout;

    for (int i = 0; i < N; i++)           //Цикл генерации точек
    {
        double x = qrand()/(double)RAND_MAX;
        double y = qrand()/(double)RAND_MAX;
        double z = qrand()/(double)RAND_MAX;

        if (x*x + y*y + z*z < 1)
        {
            V = V + 1;
            xin = x;
            yin = y;
            zin = z;
        }
        else
        {
            xout = x;
            yout = y;
            zout = z;
        }
    }

    V = V/N;

    double Pi_est = 6*V;

    double epsilon = 2.6*sqrt(V*(1 - V)/N);

    le_V->setText(QString::number(V));
    le_Pi_est->setText(QString::number(Pi_est));
    le_E_epsilon->setText(QString::number(epsilon));
}


