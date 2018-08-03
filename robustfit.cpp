#include "robustfit.h"

RobustFit::RobustFit(QWidget *parent) : QWidget(parent)
{
    customPlot = new QCustomPlot(this);
    mainLayout = new QGridLayout;
    hl = new QHBoxLayout;
    gl = new QGridLayout;

    btn = new QPushButton;
    btn->setText("Построить график");

    grp_bx = new QGroupBox;

    la0 = new QLabel;
    la0->setText("A0:");
    la0->setAlignment(Qt::AlignHCenter);
    lb0 = new QLabel;
    lb0->setText("B0:");
    lb0->setAlignment(Qt::AlignHCenter);
    lN = new QLabel;
    lN->setText("Количество случ.чисел:");
    lN->setWordWrap(true);
    lN->setAlignment(Qt::AlignHCenter);
    lSigma = new QLabel;
    lSigma->setText("Шум:");
    lSigma->setAlignment(Qt::AlignHCenter);

    sb_a0 = new QDoubleSpinBox;
    sb_a0->setMaximum(500);
    sb_a0->setMinimum(-500);
    sb_a0->setValue(1);
    sb_a0->setSingleStep(1);

    sb_b0 = new QDoubleSpinBox;
    sb_b0->setMaximum(500);
    sb_b0->setMinimum(-500);
    sb_b0->setValue(4);
    sb_b0->setSingleStep(1);

    sb_N = new QSpinBox;
    sb_N->setMaximum(2500);
    sb_N->setMinimum(0);
    sb_N->setValue(150);

    sb_Sigma = new QDoubleSpinBox;
    sb_Sigma->setMaximum(1000);
    sb_Sigma->setMinimum(0);
    sb_Sigma->setValue(0.5);

    gl->addWidget(la0, 0, 0);
    gl->addWidget(lb0, 0, 1);
    gl->addWidget(lN, 0, 2);
    gl->addWidget(lSigma, 0, 3);

    gl->addWidget(sb_a0, 1, 0);
    gl->addWidget(sb_b0, 1, 1);
    gl->addWidget(sb_N, 1, 2);
    gl->addWidget(sb_Sigma, 1, 3);

    grp_bx->setLayout(gl);
    grp_bx->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    gl2 = new QGridLayout;

    lA0 = new QLabel;
    lA0->setText("A0:");
    lA0->setAlignment(Qt::AlignHCenter);
    lB0 = new QLabel;
    lB0->setText("B0:");
    lB0->setAlignment(Qt::AlignHCenter);
    la = new QLabel;
    la->setText("A:");
    la->setAlignment(Qt::AlignHCenter);
    lb = new QLabel;
    lb->setText("B:");
    lb->setAlignment(Qt::AlignHCenter);
    lK = new QLabel;
    lK->setText("Число итераций K:");
    lK->setAlignment(Qt::AlignHCenter);

    gl2->addWidget(lA0, 0, 0);
    gl2->addWidget(lB0, 0, 1);
    gl2->addWidget(la, 0, 2);
    gl2->addWidget(lb, 0, 3);
    gl2->addWidget(lK, 0, 4);

    le_a0 = new QLineEdit;
    le_a0->setReadOnly(true);
    le_b0 = new QLineEdit;
    le_b0->setReadOnly(true);
    le_A = new QLineEdit;
    le_A->setReadOnly(true);
    le_B = new QLineEdit;
    le_B->setReadOnly(true);
    le_K = new QLineEdit;
    le_K->setReadOnly(true);

    gl2->addWidget(le_a0, 1, 0);
    gl2->addWidget(le_b0, 1, 1);
    gl2->addWidget(le_A, 1, 2);
    gl2->addWidget(le_B, 1, 3);
    gl2->addWidget(le_K, 1, 4);

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
    customPlot->xAxis->setRange(-5, 200);
    customPlot->yAxis->setRange(-5, 200);
    customPlot->xAxis->setLabel("X");
    customPlot->yAxis->setLabel("Y");

    yAxisRangeDiff = 0;

    CreatorConnections();
}


void RobustFit::RobustFitChartBuilding()
{
    // N - Количество случайных точек
    // A0, B0, A, B - Параметры смоделированной и подогнанной прямых линий
    double A0 = sb_a0->value();
    double B0 = sb_b0->value();
    int N = sb_N->value();
    double sigma_noise = sb_Sigma->value();

    clearPlot();

    QCPBars *Bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(Bars);

    QPen graphPen;
    graphPen.setColor(QColor(255, 0, 0));
    graphPen.setWidthF(2);


    double A, B;
    double x[N], y[N]; // - Массивы координат точек
    //sigma_noise - Параметр уровня нормального шума
    double w[N]; //Массив робастных весов
    double d[N]; //Массив расстояний от точек до подгоняемой прямой
    QVector<double> arrY;
    QVector<double> arrX;

    for (int i = 0; i < N; i++) //Цикл генерации точек измерений
    {
        double error = 0; //Моделирование ошибки измерения на основании ЦПТ
        for (int j = 0; j < 12; j++)
            error = error + qrand()/(double)RAND_MAX;
        error = (error - 6)*sigma_noise;

        x[i] = i;
        y[i] = A0*x[i] + B0 + error;
        CreateNewDot(x[i], y[i]);
    }

    graph1 = customPlot->addGraph();
    mapData = graph1->data();
    graph1->setPen(graphPen);
    graph1->setLineStyle((QCPGraph::LineStyle)(1));

    double r = 6*(rand()/(double)RAND_MAX - 0.5);
    A = A0 + r; //Начальное приближение параметра A
    r = 4*(rand()/(double)RAND_MAX - 0.5);
    B = B0 + r; //Начальное приближение параметра B

    for (int i = 0; i < N; i++)
    {
        w[i] = 1; //Начальные веса равны единице
    }

    double sw = N; //Начальная сумма всех весов
    const double C = 5; //Константа весовой функции Тьюки
    double S = 100000;

    for (int k = 0; S > 0.0001; k++)       //Цикл итерационного перерасчета параметров прямой
    {
        if (k > 100) break;             //Если количество итераций очень большое, цикл прекращается
        double a = A, b = B;         //Временные переменные для хранения параметров прямой

        double swd, sx, sy, sxy, sxx;
        swd = sx = sy = sxy = sxx = 0;

        for (int i = 0; i < N; i++)
        {
            d[i] = y[i] - a*x[i] - b; //Расчет расстояний от точек до подгоняемой линии
            swd += w[i]*d[i]*d[i];
        }

        double Sigma = C*sqrt(swd/sw); //Расчет параметра весовой функции Тьюки

        for (int i = 0; i < N; i++) //Цикл пересчета весов для точек
        {
            if (fabs(d[i]) > Sigma)
            {
                w[i] = 0;
            }
            else
            {
                double temp = d[i]/Sigma;
                temp = 1 - temp*temp;
                w[i] = temp*temp;
            }
        }
        sw = 0;

        for (int i = 0; i < N; i++)
        {
            sw  += w[i];
            sx  += w[i]*x[i];
            sxx += w[i]*x[i]*x[i];
            sy  += w[i]*y[i];
            sxy += w[i]*x[i]*y[i];
        }

        double q = sw*sxx - sx*sx; //Знаменатель

        if (q == 0) //Если знаменатель равен нулю…
        {
            qDebug()<<"Error: q = 0. Line is vertical!"; //…выводится сообщение об ошибке,
            exit(0); //и программа завершается
        }
        else
        {
            A = (sw*sxy - sx*sy)/q;
            B = (sy*sxx - sx*sxy)/q;
        }

        S = (A - a)*(A - a) + (B - b)*(B - b);  //Расчет условия сходимости параметров

        for (int i = 0; i < N; i++){
            double y = A*x[i] + B;
            mapData->operator [](i) = QCPData(x[i], y);
            arrX.append(x[i]);
            arrY.append(y);
        }

        le_A->setText(QString::number(A));
        le_B->setText(QString::number(B));
        le_a0->setText(QString::number(A0));
        le_b0->setText(QString::number(B0));
        le_K->setText(QString::number(k));
    }

    yAxisRangeDiff = abs( arrY.last() - arrY.first() );
    //qDebug() << "=============================";
    //qDebug() << "yAxisRangeDiff =" << yAxisRangeDiff;

    customPlot->xAxis->setRange( -( N * 0.1 ) , N + N * 0.1 );

    if (A0 >= 0){
        if (B0 >= 0){
            if (arrY.first() >= 0)
                customPlot->yAxis->setRange( arrY.first()  - yAxisRangeDiff * 0.3, arrY.last() + yAxisRangeDiff * 0.3);
            if (arrY.first() < 0 )
                customPlot->yAxis->setRange( arrY.first()  + yAxisRangeDiff * 0.3, arrY.last() - yAxisRangeDiff * 0.3);
        }
        else if (B0 < 0){
            if (arrY.first() >= 0)
                customPlot->yAxis->setRange( arrY.first() + yAxisRangeDiff * 0.3, arrY.last() - yAxisRangeDiff * 0.3);
            if (arrY.first() < 0 ){

                customPlot->yAxis->setRange( arrY.first() - yAxisRangeDiff * 0.3, arrY.last() + yAxisRangeDiff * 0.3);
            }
        }
    }

    if (A0 < 0){
        if (B0 >= 0){
            if (arrY.first() >= 0)
                customPlot->yAxis->setRange( arrY.last() - yAxisRangeDiff * 0.3, arrY.first() + yAxisRangeDiff * 0.3);
            if (arrY.first() < 0 )
                customPlot->yAxis->setRange( arrY.last() - yAxisRangeDiff * 0.3, arrY.first() - yAxisRangeDiff * 0.3);
        }
        else if (B0 < 0){
            if (arrY.first() >= 0){
                customPlot->yAxis->setRange( arrY.last() - yAxisRangeDiff * 0.3, arrY.first() - yAxisRangeDiff * 0.3);
            }
            if (arrY.first() < 0 )
                customPlot->yAxis->setRange( arrY.last() - yAxisRangeDiff * 0.3, arrY.first() + yAxisRangeDiff * 0.3);
        }
    }

    //qDebug() << "Y AXIS > lower: " << customPlot->yAxis->range().lower << "; upper:" << customPlot->yAxis->range().upper;
    //qDebug() << "ARR Y > first: " << arrY.first() << "; last:" << arrY.last();

    customPlot->replot();
}

void RobustFit::CreatorConnections()
{
    connect(customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(RobustFitChartBuilding()));
}

void RobustFit::mousePress()
{
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void RobustFit::mouseWheel()
{
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void RobustFit::clearPlot()
{
    customPlot->clearGraphs();
    customPlot->clearItems();
    customPlot->clearPlottables();
    customPlot->replot();
}

void RobustFit::CreateNewDot(double x, double y)
{
    QCPGraph *dot = customPlot->addGraph();
    QCPDataMap  *dotData;
    dotData = dot->data();
    dot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    dotData->operator [](x) = QCPData(x, y);
}
