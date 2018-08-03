#include "rileydistr.h"

RileyDistr::RileyDistr(QWidget *parent) : QWidget(parent)
{
    customPlot = new QCustomPlot(this);
    mainLayout = new QGridLayout;
    hl = new QHBoxLayout;
    gl = new QGridLayout;

    btn = new QPushButton;
    btn->setText("Построить график");

    grp_bx = new QGroupBox;

    lp = new QLabel;
    lp->setText("Сигма:");
    lp->setAlignment(Qt::AlignHCenter);
    lN = new QLabel;
    lN->setText("Количество случ.чисел:");
    lN->setAlignment(Qt::AlignHCenter);
    lN->setWordWrap(true);
    lm = new QLabel;
    lm->setText("Число ячеек гистограммы:");
    lm->setAlignment(Qt::AlignHCenter);
    lm->setWordWrap(true);
    ld = new QLabel;
    ld->setText("Ширина гистограммы:");
    ld->setAlignment(Qt::AlignHCenter);
    ld->setWordWrap(true);


    sb_sigma = new QDoubleSpinBox;
    sb_sigma->setMaximum(100);
    sb_sigma->setMinimum(0);
    sb_sigma->setValue(1);
    sb_sigma->setSingleStep(1);

    sb_N = new QSpinBox;
    sb_N->setMaximum(5000);
    sb_N->setMinimum(0);
    sb_N->setValue(1000);

    sb_m = new QSpinBox;
    sb_m->setMaximum(100);
    sb_m->setMinimum(0);
    sb_m->setValue(20);

    sb_d = new QDoubleSpinBox;
    sb_d->setMaximum(100);
    sb_d->setMinimum(0);
    sb_d->setValue(3);
    sb_d->setSingleStep(1);

    gl->addWidget(lp, 0, 0);
    gl->addWidget(lN, 0, 1);
    gl->addWidget(lm, 0, 2);
    gl->addWidget(ld, 0, 3);

    gl->addWidget(sb_sigma, 1, 0);
    gl->addWidget(sb_N, 1, 1);
    gl->addWidget(sb_m, 1, 2);
    gl->addWidget(sb_d, 1, 3);

    grp_bx->setLayout(gl);
    grp_bx->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    gl2 = new QGridLayout;
    lChi = new QLabel;
    lChi->setText("Хи-квадрат:");
    lChi->setAlignment(Qt::AlignHCenter);
    lSest = new QLabel;
    lSest->setText("Оценка сигма:");
    lSest->setAlignment(Qt::AlignHCenter);
    lChicr = new QLabel;
    lChicr->setText("Критерий хи-квадрат:");
    lChicr->setAlignment(Qt::AlignHCenter);

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
    customPlot->yAxis->setRange(-5, 500);
    customPlot->xAxis->setLabel("X");
    customPlot->yAxis->setLabel("Y");

    CreatorConnections();
}

void RileyDistr::RileyChartBuilding()
{
    double sigma = sb_sigma->value();
    int N = sb_N->value();
    int m = sb_m->value();
    double wd = sb_d->value();

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

    //qDebug() << "==========================================";

    double sigma_est = 0;
    int h[m + 1];
    double x[N];
    double Pk[m];
    double dx =  (float) wd /m;
    double SumX2 = 0;
    double tk[m + 1];
    double Hi = 0;

    for (int k = 0; k <= m; k++)
    {
        h[k] = 0;
    }

    for (int i = 0; i < N; i++)
    {
        double r = qrand()/(double)RAND_MAX;
        x[i] = sigma * sqrt(-2*log(r));
        SumX2 = SumX2 + x[i]*x[i];

        if (x[i] > (float) wd)
        {
            h[m] = h[m] + 1;
        }
        else
        {
            int k =  trunc ( (x[i])/dx );
            //int k_int32 = (int) x[i]/dx;
            // Convert to INT 32:
            const float recip = 1.0 / (32768.0*65536.0);
            float temp_val = k * recip;
            int k_int32 = temp_val * (32768.0*65536.0);

            h[k_int32] = h[k_int32] + 1;
        }
    }

    sigma_est = sqrt(0.5*SumX2/N);

    for (int k = 0; k <= m; k++)
    {
        tk[k] = k*dx;
    }

    for (int k = 0; k < m; k++)
    {
        Pk[k] = exp(-0.5*tk[k]*tk[k]/(sigma_est*sigma_est)) - exp(-0.5*tk[k + 1]*tk[k + 1]/(sigma_est*sigma_est)    );
    }

    Pk[m] = 1 - exp(-0.5*tk[m]*tk[m]/sigma_est*sigma_est);


    for (int k = 0; k < m ; k++)
    {
        double temp = h[k] - (N*Pk[k]);
        Hi = Hi + temp*temp/(N*Pk[k]);


        //qDebug() << "==========================================";
        //qDebug() << "temp =" << temp;
        //qDebug() << "Pk[" << k << "] =" << Pk[k];
        //qDebug() << "h[" << k << "] =" << h[k];
        //qDebug() << "N*Pk[" << k << "] =" << N*Pk[k];
        //qDebug() << "N = " << N << "; Pk[" << k << "] =" << Pk[k];
        //qDebug() << "k = " << k << "; Hi = " << Hi;
    }

    double Hiaverage = m + 3 * sqrt(2 * m);

    //qDebug() << "Sigma = " << sigma;
    //qDebug() << "sigma_est = " << sigma_est;
    //qDebug() << "Hi = " << Hi;

    le_Chi->setText(QString::number(Hi));
    le_Sest->setText(QString::number(sigma_est));
    le_Chicr->setText(QString::number(Hiaverage));

    for (int i = 0; i < m; i++){
        mapData->operator [](i) = QCPData(i, h[i]);
        arrX.insert(i, h[i]);
        arrY.insert(i, i);
    }

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

void RileyDistr::CreatorConnections()
{
    connect(customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(RileyChartBuilding()));
}

void RileyDistr::mousePress()
{
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void RileyDistr::mouseWheel()
{
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void RileyDistr::clearPlot()
{
    customPlot->clearGraphs();
    customPlot->clearItems();
    customPlot->clearPlottables();
    customPlot->replot();
}

