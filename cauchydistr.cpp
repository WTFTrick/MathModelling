#include "cauchydistr.h"

CauchyDistr::CauchyDistr(QWidget *parent) : QWidget(parent)
{
    customPlot = new QCustomPlot(this);
    mainLayout = new QGridLayout;
    hl = new QHBoxLayout;
    gl = new QGridLayout;

    btn = new QPushButton;
    btn->setText("Построить график");

    grp_bx = new QGroupBox;

    lp = new QLabel;
    lp->setText("A:");
    lp->setAlignment(Qt::AlignHCenter);
    lN = new QLabel;
    lN->setText("Количество случ.чисел:");
    lN->setAlignment(Qt::AlignHCenter);
    lN->setWordWrap(true);
    lm = new QLabel;
    lm->setText("Число ячеек гистограммы:");
    lm->setAlignment(Qt::AlignHCenter);
    lm->setWordWrap(true);

    sb_p = new QDoubleSpinBox;
    sb_p->setValue(1);
    sb_p->setSingleStep(1);

    sb_N = new QSpinBox;
    sb_N->setMaximum(5000);
    sb_N->setMinimum(0);
    sb_N->setValue(2000);

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
    lPest->setText("Оценка А");
    lPest->setAlignment(Qt::AlignHCenter);
    lPest->setWordWrap(true);
    lChicr = new QLabel;
    lChicr->setText("Критерий хи-квадрат:");
    lChicr->setAlignment(Qt::AlignHCenter);
    lChicr->setWordWrap(true);

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
    customPlot->yAxis->setRange(-5, 400);
    customPlot->xAxis->setLabel("X");
    customPlot->yAxis->setLabel("Y");

    CreatorConnections();
}


void CauchyDistr::CauchyChartBuilding()
{
    double a = sb_p->value();
    int N = sb_N->value();
    int m = sb_m->value();

    clearPlot();

    QVector<double> arrX;
    QVector<double> arrY;

    QCPBars *Bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(Bars);

    graph1 = customPlot->addGraph();
    mapData =  graph1->data();

    QPen graphPen;
    graphPen.setColor(QColor(0, 0, 0));
    graphPen.setWidthF(1);
    graph1->setPen(graphPen);
    graph1->setLineStyle((QCPGraph::LineStyle)(1));

    //const int N;      //Количество случайных точек
    //const int m;  //Количество ячеек гистограммы
    //double a;//Параметр распределения Коши
    double a_est;//Оценка параметра распределения Коши

    int h[m + 2];
    double Pk[m + 2];
    double x[N];
    double x_sort[N];
    double gamma = 1;

    for (int k = 0; k <= m + 2; k++)
    {
        h[k] = 0;
    }

    for (int i = 1; i < N; i++)
    {
        double r = qrand()/(double)RAND_MAX;
        double tan_c = tan(3.141593*(r - 0.5)) + a;
        x[i] = gamma * tan_c;
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

    a_est = x_sort[N/2];

    double dx = 10.0/m;

    for (int i = 0; i < N; i++)
    {
        if (x[i] < a_est - 5)
        {
            h[0] = h[0] + 1;
        }
        else
        {
            if (x[i] > a_est + 5)
            {
                h[m + 1] = h[m + 1] + 1;
            }
            else
            {
                int k = int((x[i] - a_est + 5)/dx) + 1;
                h[k] = h[k] + 1;
            }
        }
    }

    double tk[m + 1];

    for (int k = 1; k <= m + 1; k++)
    {
        tk[k] = a_est - 5 + (k - 1)*dx;
    }

    for (int k = 1; k <= m + 1; k++)
    {
        Pk[k] = (atan((tk[k + 1] - a_est)/gamma) - atan((tk[k] - a_est)/gamma))/3.141593;
    }

    Pk[0] = Pk[m + 1] = (atan((tk[1] - a_est)/gamma))/3.141593 + 0.5;

    double Hi = 0;
    for (int k = 1; k <= m + 1; k++)
    {
        double temp = h[k] - (N*Pk[k]);
        qDebug() << "==========================================";
        qDebug() << "h[" << k << "] =" << h[k];
        qDebug() << "N*Pk[" << k << "] =" << N*Pk[k];
        qDebug() << "N = " << N << "; Pk[" << k << "] =" << Pk[k];
        Hi = Hi + temp*temp/(N*Pk[k]);
        qDebug() << "k = " << k << "; Hi = " << Hi;
    }

    for (int i = 0; i < m + 2; i++){
        mapData->operator [](i) = QCPData(i, h[i]);
        arrX.insert(i, h[i]);
        arrY.insert(i, i);
    }

    le_Chi->setText(QString::number(Hi));
    le_Pest->setText(QString::number(a_est));
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

void CauchyDistr::CreatorConnections()
{
    connect(customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(CauchyChartBuilding()));
}

void CauchyDistr::mousePress()
{
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void CauchyDistr::mouseWheel()
{
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void CauchyDistr::clearPlot()
{
    customPlot->clearGraphs();
    customPlot->clearItems();
    customPlot->clearPlottables();
    customPlot->replot();
}
