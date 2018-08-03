#include "uniformdistr.h"

UniformDistr::UniformDistr(QWidget *parent) : QWidget(parent)
{
    customPlot = new QCustomPlot(this);
    mainLayout = new QGridLayout;
    hl = new QHBoxLayout;
    gl = new QGridLayout;

    btn = new QPushButton;
    btn->setText("Построить график");

    grp_bx = new QGroupBox;

    la = new QLabel;
    la->setText("A:");
    la->setAlignment(Qt::AlignHCenter);
    lb = new QLabel;
    lb->setText("B:");
    lb->setAlignment(Qt::AlignHCenter);
    lN = new QLabel;
    lN->setText("Количество случ.чисел:");
    lN->setAlignment(Qt::AlignHCenter);
    lN->setWordWrap(true);
    lm = new QLabel;
    lm->setText("Число ячеек гистограммы:");
    lm->setWordWrap(true);
    lm->setAlignment(Qt::AlignHCenter);

    sb_a = new QDoubleSpinBox;
    sb_a->setMaximum(500);

    sb_a->setValue(1);
    sb_a->setSingleStep(1);

    sb_b = new QDoubleSpinBox;
    sb_b->setMaximum(500);
    sb_b->setValue(7);
    sb_b->setSingleStep(1);

    sb_N = new QSpinBox;
    sb_N->setMaximum(5000);
    sb_N->setMinimum(0);
    sb_N->setValue(5000);

    sb_m = new QSpinBox;
    sb_m->setMaximum(100);
    sb_m->setMinimum(0);
    sb_m->setValue(20);

    gl->addWidget(la, 0, 0);
    gl->addWidget(lb, 0, 1);
    gl->addWidget(lN, 0, 2);
    gl->addWidget(lm, 0, 3);

    gl->addWidget(sb_a, 1, 0);
    gl->addWidget(sb_b, 1, 1);
    gl->addWidget(sb_N, 1, 2);
    gl->addWidget(sb_m, 1, 3);

    grp_bx->setLayout(gl);
    grp_bx->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    gl2 = new QGridLayout;
    lAest = new QLabel;
    lAest->setText("Оценка А:");
    lAest->setAlignment(Qt::AlignHCenter);
    lBest = new QLabel;
    lBest->setText("Оценка Б:");
    lBest->setAlignment(Qt::AlignHCenter);
    lChi = new QLabel;
    lChi->setText("Хи-квадрат:");
    lChi->setAlignment(Qt::AlignHCenter);
    lChicr = new QLabel;
    lChicr->setText("Критерий хи-квадрат:");
    lChicr->setWordWrap(true);
    lChicr->setAlignment(Qt::AlignHCenter);

    le_Aest = new QLineEdit;
    le_Aest->setReadOnly(true);
    le_Best = new QLineEdit;
    le_Chi = new QLineEdit;
    le_Chi->setReadOnly(true);
    le_Best->setReadOnly(true);
    le_Chicr = new QLineEdit;
    le_Chicr->setReadOnly(true);

    gl2->addWidget(lAest, 0, 0);
    gl2->addWidget(lBest, 0, 1);
    gl2->addWidget(lChi, 0, 2);
    gl2->addWidget(lChicr, 0, 3);

    gl2->addWidget(le_Aest, 1, 0);
    gl2->addWidget(le_Best, 1, 1);
    gl2->addWidget(le_Chi, 1, 2);
    gl2->addWidget(le_Chicr, 1, 3);

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
    customPlot->yAxis->setRange(-5, 475);
    customPlot->xAxis->setLabel("X");
    customPlot->yAxis->setLabel("Y");

    CreatorConnections();
}


void UniformDistr::UniformlChartBuilding()
{
    double a = sb_a->value();
    double b = sb_b->value();
    int N = sb_N->value();
    int m = sb_m->value();
    //qDebug() << "m =" << m;

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

    //const int N;  //Количество случайных точек
    //const int m;  //Количество ячеек гистограммы
    //double a, b;  //Границы интервала
    double a_est, b_est;
    int h[m];
    double x[N];
    double x_sort[N];

    for (int k = 0; k < m; k++)
    {
        h[k] = 0;
        //mapData->operator [](k) = QCPData(0, 0);
    }

    for (int i = 1; i < N; i++)
    {
        double r = qrand() /(double)RAND_MAX;
        x[i] = r * (b - a) + a;
    }

    for (int i = 0; i < N; i++)
    {
        x_sort[i] = x[i];
    }

    for (int k = 0; k < N; k++)
    {
        for (int i = N; i >= k; i--)
        {
            if (x_sort[i] < x_sort[i - 1])
            {
                double t = x_sort [i];
                x_sort [i] = x_sort [i - 1];
                x_sort [i - 1] = t;
            }
        }
    }

    for (int i = 0; i < N; i++){
        //qDebug() << "x_sort[" << i << "] =" << x_sort[i];
    }

    a_est = x_sort[2];
    b_est = x_sort[N - 1];

    double dx = (b_est - a_est) / m;

    for (int i = 1; i < N; i++)
    {
        int k = int( (x[i] - a_est) /dx );
        h[k] = h[k] + 1;
        //qDebug() << "h[" << k << "] =" << h[k];
    }

    double Pk = 1.0/m;
    double Hi = 0;
    for (int k = 0; k < m; k++)
    {
        double temp = h[k] - N*Pk;
        Hi = Hi + temp*temp/(N*Pk);
    }

    for (int i = 0; i < m; i++){
        mapData->operator [](i) = QCPData(i, h[i]);
        arrX.insert(i, h[i]);
        arrY.insert(i, i);
    }

    le_Chi->setText(QString::number(Hi));
    le_Aest->setText(QString::number(a_est));
    le_Best->setText(QString::number(b_est));
    le_Chicr->setText(QString::number(m+3*sqrt(2*m)));

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

void UniformDistr::CreatorConnections()
{
    connect(customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(UniformlChartBuilding()));
}

void UniformDistr::mousePress()
{
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void UniformDistr::mouseWheel()
{
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void UniformDistr::clearPlot()
{
    customPlot->clearGraphs();
    customPlot->clearItems();
    customPlot->clearPlottables();
    customPlot->replot();
}

