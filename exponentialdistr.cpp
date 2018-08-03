#include "exponentialdistr.h"

ExponentialDistr::ExponentialDistr(QWidget *parent) : QWidget(parent)
{
    customPlot = new QCustomPlot(this);
    mainLayout = new QGridLayout;
    hl = new QHBoxLayout;
    gl = new QGridLayout;

    btn = new QPushButton;
    btn->setText("Построить график");

    grp_bx = new QGroupBox;

    lp = new QLabel;
    lp->setText("Лямбда:");
    lp->setAlignment(Qt::AlignHCenter);
    lN = new QLabel;
    lN->setText("Количество случ.чисел:");
    lN->setWordWrap(true);
    lN->setAlignment(Qt::AlignHCenter);
    lm = new QLabel;
    lm->setText("Число ячеек гистограммы:");
    lm->setAlignment(Qt::AlignHCenter);
    lm->setWordWrap(true);

    sb_p = new QDoubleSpinBox;
    sb_p->setMaximum(100);
    sb_p->setValue(2);
    sb_p->setSingleStep(1);

    sb_N = new QSpinBox;
    sb_N->setMaximum(2500);
    sb_N->setMinimum(0);
    sb_N->setValue(1250);

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
    lChi->setText("Значение Колмогорова-Смирнова:");
    lChi->setAlignment(Qt::AlignHCenter);
    lChi->setWordWrap(true);
    lSest = new QLabel;
    lSest->setText("Оценка лямбда:");
    lSest->setAlignment(Qt::AlignHCenter);
    lSest->setWordWrap(true);
    lChicr = new QLabel;
    lChicr->setText("Критерий Колмогорова-Смирнова:");
    lChicr->setAlignment(Qt::AlignHCenter);
    lChicr->setWordWrap(true);

    le_Chi = new QLineEdit;
    le_Chi->setReadOnly(true);
    le_Sest = new QLineEdit;
    le_Sest->setReadOnly(true);
    le_Chicr = new QLineEdit;
    le_Chicr->setReadOnly(true);

    gl2->addWidget(lChi, 0, 0);
    gl2->addWidget(lSest, 0, 1);
    gl2->addWidget(lChicr, 0, 2);

    gl2->addWidget(le_Chi, 1, 0);
    gl2->addWidget(le_Sest, 1, 1);
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
    customPlot->yAxis->setRange(-5, 700);
    customPlot->xAxis->setLabel("X");
    customPlot->yAxis->setLabel("Y");

    CreatorConnections();
}


void ExponentialDistr::ExpoChartBuilding()
{
    double lambda = sb_p->value();
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

    //const int N;       //Количество случайных точек
    //const int m;      //Количество ячеек гистограммы
    //double lambda;//Параметр экспоненциального распределения
    double lambda_est;  //Оценка параметра экспоненциального распределения
    int h[m + 1];//Массив гистограммы
    //double Pk[m + 1]; //Массив вероятностей попадания в k-ю ячейку гистограммы
    double x[N];  //Массив полученных случайных чисел

    for (int k = 0; k <= m; k++)
    {
        h[k] = 0;
    }

    double dx = 5.0/m;

    lambda_est = 0;

    for (int i = 1; i < N; i++)
    {
        double r = qrand()/(double)RAND_MAX;
        x[i] = -log(r)/lambda;
        if (x[i] <= 5)
        {
            int k = int(x[i]/dx);
            h[k] = h[k] + 1;
        }
        else
        {
            h[m] = h[m] + 1;
        }

        lambda_est = lambda_est + x[i];
    }

    lambda_est = N/lambda_est;

    double x_sort[N];

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

    double d_max = 0;

    for (int i = 0; i < N; i++)
    {
        double Fi = 1 - exp(-lambda_est*x_sort[i]);
        double d = fabs(i/(double)N - Fi);
        if (d_max < d)
        {
            d_max = d;
        }

        d = fabs((i + 1)/(double)N - Fi);
        if (d_max < d)
        {
            d_max = d;
        }
    }

    double Dn = sqrt((double)N)*d_max;

    for (int i = 0; i < m + 1; i++){
        mapData->operator [](i) = QCPData(i, h[i]);
        arrX.insert(i, h[i]);
        arrY.insert(i, i);
    }

    le_Chi->setText(QString::number(Dn));
    le_Sest->setText(QString::number(lambda_est));
    le_Chicr->setText(QString::number(1.36));

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

void ExponentialDistr::CreatorConnections()
{
    connect(customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(ExpoChartBuilding()));
}

void ExponentialDistr::mousePress()
{
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void ExponentialDistr::mouseWheel()
{
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void ExponentialDistr::clearPlot()
{
    customPlot->clearGraphs();
    customPlot->clearItems();
    customPlot->clearPlottables();
    customPlot->replot();
}
