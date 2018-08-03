#include "gaussdistr.h"

GaussDistr::GaussDistr(QWidget *parent) : QWidget(parent)
{
    customPlot = new QCustomPlot(this);
    mainLayout = new QGridLayout;
    hl = new QHBoxLayout;
    gl = new QGridLayout;

    btn = new QPushButton;
    btn->setText("Построить график");

    grp_bx = new QGroupBox;

    la = new QLabel;
    la->setText("А:");
    la->setAlignment(Qt::AlignHCenter);
    ls = new QLabel;
    ls->setText("Сигма:");
    ls->setAlignment(Qt::AlignHCenter);
    lN = new QLabel;
    lN->setText("Количество случ.чисел:");
    lN->setAlignment(Qt::AlignHCenter);
    lN->setWordWrap(true);
    lm = new QLabel;
    lm->setText("Число ячеек гистограммы:");
    lm->setAlignment(Qt::AlignHCenter);
    lm->setWordWrap(true);

    sb_a = new QDoubleSpinBox;
    sb_a->setMaximum(1000);
    sb_a->setMinimum(-1000);
    sb_a->setValue(5);
    sb_a->setSingleStep(1);

    sb_s = new QDoubleSpinBox;
    sb_s->setMaximum(1000);
    sb_s->setMinimum(-1000);
    sb_s->setValue(1);
    sb_s->setSingleStep(1);

    sb_N = new QSpinBox;
    sb_N->setMaximum(5000);
    sb_N->setMinimum(0);
    sb_N->setValue(3000);

    sb_m = new QSpinBox;
    sb_m->setMaximum(100);
    sb_m->setMinimum(0);
    sb_m->setValue(20);

    rb_rejection = new QRadioButton;
    rb_rejection->setChecked(true);
    rb_rejection->setText("Метод Браковки");
    rb_clt = new QRadioButton;
    rb_clt->setChecked(false);
    rb_clt->setText("ЦПТ");

    gl->addWidget(la, 0, 2);
    gl->addWidget(ls, 0, 3);
    gl->addWidget(lN, 0, 4);
    gl->addWidget(lm, 0, 5);

    gl->addWidget(rb_rejection, 1, 0);
    gl->addWidget(rb_clt, 1, 1);
    gl->addWidget(sb_a, 1, 2);
    gl->addWidget(sb_s, 1, 3);
    gl->addWidget(sb_N, 1, 4);
    gl->addWidget(sb_m, 1, 5);

    grp_bx->setLayout(gl);
    grp_bx->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    gl2 = new QGridLayout;
    lAest = new QLabel;
    lAest->setText("Оценка А:");
    lAest->setWordWrap(true);
    lAest->setAlignment(Qt::AlignHCenter);
    lSest = new QLabel;
    lSest->setText("Оценка сигма:");
    lSest->setAlignment(Qt::AlignHCenter);
    lSest->setWordWrap(true);
    lChi = new QLabel;
    lChi->setText("Хи-квадрат:");
    lChi->setAlignment(Qt::AlignHCenter);
    lChi->setWordWrap(true);
    lChicr = new QLabel;
    lChicr->setText("Оценка хи-квадрат:");
    lChicr->setAlignment(Qt::AlignHCenter);
    lChicr->setWordWrap(true);

    le_Aest = new QLineEdit;
    le_Aest->setReadOnly(true);
    le_Sest = new QLineEdit;
    le_Chi = new QLineEdit;
    le_Chi->setReadOnly(true);
    le_Sest->setReadOnly(true);
    le_Chicr = new QLineEdit;
    le_Chicr->setReadOnly(true);

    gl2->addWidget(lAest, 0, 0);
    gl2->addWidget(lSest, 0, 1);
    gl2->addWidget(lChi, 0, 2);
    gl2->addWidget(lChicr, 0, 3);

    gl2->addWidget(le_Aest, 1, 0);
    gl2->addWidget(le_Sest, 1, 1);
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
    customPlot->yAxis->setRange(-5, 650);
    customPlot->xAxis->setLabel("X");
    customPlot->yAxis->setLabel("Y");

    CreatorConnections();
}

void GaussDistr::GaussChartBuilding()
{
    double a = sb_a->value();
    double sigma = sb_s->value();
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

    //double a, sigma;
    double a_est, sigma_est;
    int h[m + 2];
    double x[N];

    for (int k = 0; k <= m + 2; k++) h[k] = 0;

    a_est = 0;

    if (rb_rejection->isChecked() == 1){

        for (int i = 0; i<N; i++)
        {
            double y;
            do
            {
                double r = qrand()/(double)RAND_MAX;
                x[i] = 6*sigma*(r - 0.5) + a;
                y = rand()/(double)RAND_MAX;
            }
            while ( y > exp( -( (x[i] - a) * (x[i] - a) ) / (2*sigma*sigma) )  );

            a_est = a_est + x[i];

        }
    }
    else if(rb_clt->isChecked() == 1) {

        for (int i = 1; i < N; i++)
        {
            x[i] = 0;
            for (int j = 1; j <= 12; j++)
            {
                x[i] = x[i] + qrand()/(double)RAND_MAX;
            }

            x[i] = (x[i] - 6)*sigma + a;

            a_est = a_est + x[i];
        }
    }

    a_est = a_est/N;

    sigma_est = 0;

    double dx = 0;

    for (int i = 0; i<N; i++)       //Циклрасчета sigma_est
    {
        sigma_est += (x[i] - a_est)*(x[i] - a_est);
    }
    sigma_est = sqrt(sigma_est/N);

    dx = 6*sigma_est/m;                  //Ширина ячейки гистограммы

    for (int i = 0; i<N; i++)                          //Цикл заполнения гистограммы
    {
        if (x[i] <a_est - 3*sigma_est)//Если полученное число меньше левой границы
            h[0] = h[0] + 1;//прямоугольника, оно попадает в нулевую ячейку…
        else
        {
            if (x[i] > a_est + 3*sigma_est) //…а если оно больше правой границы,
            {
                h[m + 1] = h[m + 1] + 1;//оно попадает в (m+1)-ю ячейку
                //qDebug() << "1 - h[" << m + 1 << "] =" << h[m+1];
            }
            else
            {
                int k = int((x[i] - a_est + 3*sigma_est)/dx) + 1;
                h[k] = h[k] + 1;
                //qDebug() << "1 - h[" << k << "] =" << h[k];
            }
        }
    }

    double Pk[m + 2];
    double tk[m + 1];

    for (int k = 1; k <= m + 1; k++)
    {
        tk[k] = a_est - 3*sigma_est + (k - 1)*dx;
    }

    double SumPk = 0;

    for (int k = 1; k <= m; k++)
    {
        double x_m = 0.5*(tk[k] + tk[k + 1]);
        Pk[k] = dx*exp(-(x_m - a_est)*(x_m - a_est)/
                       (2*sigma_est *sigma_est))/(sqrt(2*3.141593)*sigma_est);
        SumPk = SumPk + Pk[k];
    }
    Pk[0] = Pk[m + 1] = 0.5*(1 - SumPk);


    double Hi = 0;
    for (int k = 1; k <= m; k++)
    {
        double temp = h[k] - N*Pk[k];
        Hi = Hi + temp*temp/(N*Pk[k]);
    }

    for (int i = 0; i < m + 2; i++){
        mapData->operator [](i) = QCPData(i, h[i]);
        arrX.insert(i, h[i]);
        arrY.insert(i, i);
    }

    le_Chi->setText(QString::number(Hi));
    le_Aest->setText(QString::number(a_est));
    le_Sest->setText(QString::number(sigma_est));
    le_Chicr->setText(QString::number(m+3*sqrt(2*m)));

    //qDebug() << "First item =" << arrX.first() << ";" << arrY.first();
    //qDebug() << "Last item =" << arrX.last() << ";" << arrY.last();

    QPen pen;
    pen.setWidthF(1.2);
    Bars->setName("Bars");
    pen.setColor(QColor(255, 131, 0));
    Bars->setPen(pen);
    Bars->setBrush(QColor(255, 131, 0, 50));
    Bars->setData(arrY, arrX);

    customPlot->replot();
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

void GaussDistr::CreatorConnections()
{
    connect(customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(GaussChartBuilding()));
}

void GaussDistr::mousePress()
{
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void GaussDistr::mouseWheel()
{
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void GaussDistr::clearPlot()
{
    customPlot->clearGraphs();
    customPlot->clearItems();
    customPlot->clearPlottables();
    customPlot->replot();
}
