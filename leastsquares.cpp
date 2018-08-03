#include "leastsquares.h"

LeastSquares::LeastSquares(QWidget *parent) : QWidget(parent)
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
    lSigma->setWordWrap(true);
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
    lAs = new QLabel;
    lAs->setText("A сигма:");
    lAs->setAlignment(Qt::AlignHCenter);
    lBs = new QLabel;
    lBs->setText("B сигма:");
    lBs->setAlignment(Qt::AlignHCenter);
    lss = new QLabel;
    lss->setText("Сигма:");
    lss->setAlignment(Qt::AlignHCenter);

    gl2->addWidget(lA0, 0, 0);
    gl2->addWidget(lB0, 0, 1);
    gl2->addWidget(la, 0, 2);
    gl2->addWidget(lb, 0, 3);
    gl2->addWidget(lAs, 0, 4);
    gl2->addWidget(lBs, 0, 5);
    gl2->addWidget(lss, 0, 6);

    le_a0 = new QLineEdit;
    le_a0->setReadOnly(true);
    le_b0 = new QLineEdit;
    le_b0->setReadOnly(true);
    le_A = new QLineEdit;
    le_A->setReadOnly(true);
    le_B = new QLineEdit;
    le_B->setReadOnly(true);
    le_As = new QLineEdit;
    le_As->setReadOnly(true);
    le_Bs = new QLineEdit;
    le_Bs->setReadOnly(true);
    le_ss = new QLineEdit;
    le_ss->setReadOnly(true);

    gl2->addWidget(le_a0, 1, 0);
    gl2->addWidget(le_b0, 1, 1);
    gl2->addWidget(le_A, 1, 2);
    gl2->addWidget(le_B, 1, 3);
    gl2->addWidget(le_As, 1, 4);
    gl2->addWidget(le_Bs, 1, 5);
    gl2->addWidget(le_ss, 1, 6);

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
    customPlot->xAxis->setRange(-25, 200);
    customPlot->yAxis->setRange(-25, 175);
    customPlot->xAxis->setLabel("X");
    customPlot->yAxis->setLabel("Y");

    yAxisRangeDiff = 0;

    CreatorConnections();
}

void LeastSquares::LeastSquaresChartBuilding()
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

    double A = 0;
    double B = 0;
    double x[N], y[N]; // - Массивы координат точек
    double var = 6;
    //sigma_noise - Параметр уровня нормального шума
    QVector<double> arrY;
    QVector<double> arrX;

    for (int i = 0; i < N; i++)
    {
        double error = 0;

        for (int j = 0; j < var * 2; j++)
        {
            error = error + qrand()/(double)RAND_MAX;
        }
        error = (error - var) * sigma_noise;

        x[i] = i;
        y[i] = A0*x[i] + B0 + error;

        CreateNewDot(x[i], y[i]);
    }

    /*for (int i=0; i<N ; ++i) {
        x[i] = i;
        y[i] =  (int) round(noise(engine));
    }*/

    graph1 = customPlot->addGraph();
    mapData = graph1->data();
    graph1->setPen(graphPen);
    graph1->setLineStyle((QCPGraph::LineStyle)(1));

    /*
    double sx, sy, sxx, syy, sxy;
    sx = sy = sxx = syy = sxy = 0;

    for (int i = 0; i < N; i++) //Расчет сумм, необходимых для оценки параметров прямой
    {
        sx  = sx  + x[i];
        sy  = sx  + y[i];
        sxx = sxx + x[i]*x[i];
        syy = syy + y[i]*y[i];
        sxy = sxy + x[i]*y[i];
    }

    double q = N * sxx - sx*sx; //Знаменатель

    if (q == 0) //Если знаменатель равен нулю…
    {
        qDebug() << "Error: q = 0. Line is vertical!"; //выводится сообщение об ошибке,
        exit(0); //и программа завершается
    }
    else
    {
        A = (N*sxy - sx*sy)/q;
        B = (sy*sxx - sx*sxy) / q;
        qDebug() << "B = " << B;
    }*/

    double xsum=0, x2sum=0, ysum=0, xysum=0, syy=0;

    for (int i = 0; i< N; i++)
    {
        xsum = xsum + x[i];                        //calculate sigma(xi)
        ysum = ysum + y[i];                        //calculate sigma(yi)
        x2sum = x2sum + pow (x[i], 2);                //calculate sigma(x^2i)
        xysum = xysum + x[i]*y[i];                    //calculate sigma(xi*yi)
        syy = syy + y[i]*y[i];
    }

    double q = N * x2sum - xsum*xsum;

    A = (N*xysum-xsum*ysum) / (N*x2sum-xsum*xsum);            //calculate slope
    B = (x2sum*ysum-xsum*xysum) / (x2sum*N-xsum*xsum);            //calculate intercept

    for (int i = 0; i < N; i++){
        double y = A*x[i] + B;
        mapData->operator [](i) = QCPData(x[i], y);
        qDebug() << "x= " << x[i];
        qDebug() << "y= " << y;
        arrX.append(x[i]);
        arrY.append(y);
    }

    double F_min = (N*syy - ysum*ysum - q*A*A)/N; //Минимальное значение функционала   //МНК
    qDebug() << "F_min" << F_min;

    //Среднеквадратичная ошибка
    // TODO: добавить вывод в интерфейс
    double sigma = sqrt(F_min/(N - 2));

    double sigma_A = sigma*sqrt(N/q); //Погрешность расчета A
    double sigma_B = sigma*sqrt(x2sum/q); //Погрешность расчета B

    le_A->setText(QString::number(A));
    le_B->setText(QString::number(B));
    le_a0->setText(QString::number(A0));
    le_b0->setText(QString::number(B0));
    le_As->setText(QString::number(sigma_A));
    le_Bs->setText(QString::number(sigma_B));
    le_ss->setText(QString::number(sigma));

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

void LeastSquares::CreatorConnections()
{
    connect(customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(LeastSquaresChartBuilding()));
}

void LeastSquares::mousePress()
{
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void LeastSquares::mouseWheel()
{
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void LeastSquares::clearPlot()
{
    customPlot->clearGraphs();
    customPlot->clearItems();
    customPlot->clearPlottables();
    customPlot->replot();
}

void LeastSquares::CreateNewDot(double x, double y)
{
    QCPGraph *dot = customPlot->addGraph();
    QCPDataMap  *dotData;
    dotData = dot->data();
    dot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    dotData->operator [](x) = QCPData(x, y);
    //customPlot->replot();
}
