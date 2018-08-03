#include "binomialdistr.h"

BinomialDistr::BinomialDistr()
{
    customPlot = new QCustomPlot(this);
    mainLayout = new QGridLayout;
    hl = new QHBoxLayout;
    gl = new QGridLayout;

    btn = new QPushButton;
    btn->setText("Построить график");

    grp_bx = new QGroupBox;

    lp = new QLabel;
    lp->setText("Вероятность:");
    lp->setAlignment(Qt::AlignHCenter);
    lN = new QLabel;
    lN->setText("Количество случ.чисел:");
    lN->setWordWrap(true);
    lN->setAlignment(Qt::AlignHCenter);
    lm = new QLabel;
    lm->setText("Число ячеек гистограммы:");
    lm->setWordWrap(true);
    lm->setAlignment(Qt::AlignHCenter);

    sb_p = new QDoubleSpinBox;
    sb_p->setMaximum(0.999);
    sb_p->setMinimum(0);
    sb_p->setValue(0.4);
    sb_p->setSingleStep(0.01);

    sb_N = new QSpinBox;
    sb_N->setMaximum(2500);
    sb_N->setMinimum(0);
    sb_N->setValue(1000);

    sb_m = new QSpinBox;
    sb_m->setMaximum(100);
    sb_m->setMinimum(0);
    sb_m->setValue(20);

    gl->addWidget(lp, 0, 0);
    gl->addWidget(lN, 0, 1);
    gl->addWidget(lm, 0, 2);

    gl->addWidget(sb_p, 1, 0);
    gl->addWidget(sb_N, 1, 1);
    gl->addWidget(sb_m, 1, 2);

    grp_bx->setLayout(gl);
    grp_bx->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    gl2 = new QGridLayout;
    lChi = new QLabel;
    lChi->setText("Хи-квадрат:");
    lChi->setAlignment(Qt::AlignHCenter);
    lChi->setWordWrap(true);
    lPest = new QLabel;
    lPest->setText("Оценка вероятности удачного исхода:");
    lPest->setAlignment(Qt::AlignHCenter);
    lPest->setWordWrap(true);
    lChicr = new QLabel;
    lChicr->setText("Критерий хи-квадрат:");
    lChicr->setWordWrap(true);
    lChicr->setAlignment(Qt::AlignHCenter);

    le_Chi = new QLineEdit;
    le_Chi->setReadOnly(true);
    le_Pest = new QLineEdit;
    le_Pest->setReadOnly(true);
    le_Chicr = new QLineEdit;
    le_Chicr->setReadOnly(true);

    gl2->addWidget(lChi, 0, 0);
    gl2->addWidget(lPest, 0, 1);
    gl2->addWidget(lChicr, 0, 2);

    gl2->addWidget(le_Chi, 1, 0);
    gl2->addWidget(le_Pest, 1, 1);
    gl2->addWidget(le_Chicr, 1, 2);

    grp_bx_2 = new QGroupBox;
    grp_bx_2->setLayout(gl2);
    grp_bx_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    hl->addWidget(btn);
    hl->addWidget(grp_bx);
    hl->addWidget(grp_bx_2);

    mainLayout->addWidget(customPlot, 0, 0);
    mainLayout->addLayout(hl, 1, 0);
    setLayout(mainLayout);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );
    customPlot->xAxis->setRange(-5, 35);
    customPlot->yAxis->setRange(-5, 250);
    customPlot->xAxis->setLabel("X");
    customPlot->yAxis->setLabel("Y");

    CreatorConnections();
}

BinomialDistr::~BinomialDistr()
{
}

void BinomialDistr::BinomialChartBuilding()
{
    double p = sb_p->value();
    int N = sb_N->value();
    int m = sb_m->value();

    clearPlot();

    QVector<double> arrX;
    QVector<double> arrY;

    QCPBars *Bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(Bars);

    graph1 = customPlot->addGraph();
    mapData = graph1->data();

    QPen graphPen;
    graphPen.setColor(QColor(0, 0, 0));
    graphPen.setWidthF(1);
    graph1->setPen(graphPen);
    graph1->setLineStyle((QCPGraph::LineStyle)(1));

    double p_est;
    int Ki[N];
    double h[m + 1];
    double Pk[m + 1];
    srand((unsigned)time(0));

    arrX.resize(m + 1);
    arrY.resize(m + 1);

    for (int k = 0; k <= m; k++) h[k] = 0;
    p_est = 0;

    for (int i = 1; i < N; i++)
    {
        Ki[i] = 0;
        for (int j = 0; j < m; j++)
        {
            double r = qrand()/(double)RAND_MAX;
            if (r < p)
                Ki[i] = Ki[i] + 1;
        }

        h[Ki[i]] = h[Ki[i]] + 1;
        p_est =p_est + Ki[i];
    }

    p_est = p_est/double(m*N);
    double q_est = 1 - p_est;
    Pk[0] = exp(m*log(q_est));

    for (int k = 0; k < m; k++)
        Pk[k + 1] = Pk[k]*(m - k)*p_est/((k + 1)*q_est);

    double Hi = 0;
    for (int k = 0; k <= m; k++)
    {
        double temp = h[k] - N*Pk[k];
        Hi = Hi + temp*temp/(N*Pk[k]);
    }

    for (int i = 0; i < m + 1; i++){
        mapData->operator [](i) = QCPData(i, h[i]);
        arrX.insert(i, h[i]);
        arrY.insert(i, i);
    }

    le_Chi->setText(QString::number(Hi));
    le_Pest->setText(QString::number(p_est));
    le_Chicr->setText(QString::number(m+1+3*sqrt(2*(m+1))));

    QPen pen;
    pen.setWidthF(1.2);
    Bars->setName("Bars");
    pen.setColor(QColor(255, 131, 0));
    Bars->setPen(pen);
    Bars->setBrush(QColor(255, 131, 0, 50));
    Bars->setData(arrY, arrX);

    double maxY = arrX.at(0);
    double minY = arrX.at(0);

    for (int i = 0; i < m; i++)
    {
        if (arrX[i] > maxY)
        {
            maxY = arrX[i];
        }
        else if (arrX[i] < minY)
        {
            if (arrX.at(i) != 0)
                minY = arrX[i];
        }
    }

    customPlot->xAxis->setRange(-1, m + m * 0.1 );
    customPlot->yAxis->setRange(-1, maxY + maxY * 0.3  );

    customPlot->replot();
}

void BinomialDistr::CreatorConnections()
{
    connect(customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(BinomialChartBuilding()));
}

void BinomialDistr::mousePress()
{
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void BinomialDistr::mouseWheel()
{
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void BinomialDistr::clearPlot()
{
    customPlot->clearGraphs();
    customPlot->clearItems();
    customPlot->clearPlottables();
    customPlot->replot();
}

