#include "celluralautomaton.h"

CelluralAutomaton::CelluralAutomaton(QWidget *parent) : QWidget(parent)
{
    customPlot = new QCustomPlot(this);
    mainLayout = new QGridLayout;
    hl = new QHBoxLayout;
    gl = new QGridLayout;

    btn = new QPushButton;
    btn->setText("Построить график");

    grp_bx = new QGroupBox;

    ls = new QLabel;
    ls->setText("Размер массива:");
    ls->setAlignment(Qt::AlignHCenter);
    ls->setWordWrap(true);
    ld = new QLabel;
    ld->setText("Размерность пространства:");
    ld->setAlignment(Qt::AlignHCenter);
    ld->setWordWrap(true);
    lN = new QLabel;
    lN->setText("Количество случ.чисел:");
    lN->setAlignment(Qt::AlignHCenter);
    lN->setWordWrap(true);
    lm = new QLabel;
    lm->setText("Число ячеек гистограммы:");
    lm->setAlignment(Qt::AlignHCenter);
    lm->setWordWrap(true);

    sb_s = new QSpinBox;
    sb_s->setMaximum(250);
    sb_s->setMinimum(0);
    sb_s->setValue(100);
    sb_s->setSingleStep(1);

    sb_d = new QSpinBox;
    sb_d->setMaximum(100);
    sb_d->setMinimum(0);
    sb_d->setValue(1);
    sb_d->setSingleStep(1);
    sb_d->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    sb_N = new QSpinBox;
    sb_N->setMaximum(5000);
    sb_N->setMinimum(0);
    sb_N->setValue(500);

    sb_m = new QSpinBox;
    sb_m->setMaximum(100);
    sb_m->setMinimum(0);
    sb_m->setValue(20);

    gl->addWidget(ls, 0, 0);
    gl->addWidget(ld, 0, 1);
    gl->addWidget(lN, 0, 2);
    gl->addWidget(lm, 0, 3);

    gl->addWidget(sb_s, 1, 0);
    gl->addWidget(sb_d, 1, 1);
    gl->addWidget(sb_N, 1, 2);
    gl->addWidget(sb_m, 1, 3);

    grp_bx->setLayout(gl);
    grp_bx->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    gl2 = new QGridLayout;
    lChi = new QLabel;
    lChi->setText("Хи-квадрат:");
    lChi->setAlignment(Qt::AlignHCenter);
    lChi->setWordWrap(true);
    ld2 = new QLabel;
    ld2->setText("Размерность пространства: ");
    ld2->setAlignment(Qt::AlignHCenter);
    ld2->setWordWrap(true);
    lChicr = new QLabel;
    lChicr->setText("Критерий хи-квадрат:");
    lChicr->setAlignment(Qt::AlignHCenter);
    lChicr->setWordWrap(true);

    le_Chi = new QLineEdit;
    le_Chi->setReadOnly(true);
    le_d = new QLineEdit;
    le_d->setReadOnly(true);
    le_Chicr = new QLineEdit;
    le_Chicr->setReadOnly(true);

    gl2->addWidget(lChi, 0, 0);
    gl2->addWidget(ld2, 0, 1);
    gl2->addWidget(lChicr, 0, 2);

    gl2->addWidget(le_Chi, 1, 0);
    gl2->addWidget(le_d, 1, 1);
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
    customPlot->xAxis->setRange(-1, 35);
    customPlot->yAxis->setRange(-1, 10);
    customPlot->xAxis->setLabel("X");
    customPlot->yAxis->setLabel("Y");

    CreatorConnections();
}

void CelluralAutomaton::CreatorConnections()
{
    connect(customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(ChartBuilding()));
}

void CelluralAutomaton::clearPlot()
{
    customPlot->clearGraphs();
    customPlot->clearItems();
    customPlot->clearPlottables();
    customPlot->replot();
}

double CelluralAutomaton::KA_random(int s, int A[])
{

    int B[s]; //Вспомогательный массив для временного хранения массива A

    for (int j = 1; j < s; j++)
    {
        int S = A[j - 1] + A[j] + A[j + 1];
        if (S % 2 == 0)
            B[j] = 0;
        else
            B[j] = 1;
    }

    for (int j = 1; j < s; j++)
        A[j] = B[j];

    A[0] = A[s];
    A[s + 1] = A[1];

    double x1 = 0;
    for (int j = 1; j < s; j++)
        x1 = x1 + 2*A[j] * ( exp((j - 1 - s)*(log(2.0)) ) );

    return x1;
}

void CelluralAutomaton::mousePress()
{
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void CelluralAutomaton::mouseWheel()
{
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void CelluralAutomaton::ChartBuilding()
{
    int s = sb_s->value();
    int N = sb_N->value();
    int m = sb_m->value();
    int d = sb_d->value();

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

    //const int N;     //Количество случайных точек
    //const int s;//Размер массива КА (без учета двух замыкающих ячеек)
    //const int m;     //Количество ячеек гистограммы
    int h[m + 1];    //Массив гистограммы
    int A[s + 2];//Массив КА
    //const int d;//Размерность пространства
    double x[d];//d-мерный массив случайных точек

    for (int j = 1; j <= s; j++)
    {
        double r = qrand() / (double)RAND_MAX;
        if (r > 0.5) A[j] = 1;
        else A[j] = 0;
    }

    for (int k = 0; k < m + 1; k++){
        h[k] = 0;
    }

    A[0] = A[s];
    A[s + 1] = A[1];

    for (int i = 0; i < N; i++)
    {
        double x_max = 0;

        for (int j = 0; j < d; j++)
        {
            x[j] = KA_random(s, A);

            if (x_max < x[j]) x_max = x[j];
        }

        double xmaxlog = log(x_max);
        double maxexp = exp(d * xmaxlog);

        double k = m * maxexp;

        h[(int)k] = h[(int)k] + 1;
    }

    double Pk = 1.0 / m;

    double Hi = 0;

    for (int k = 0; k < m; k++)
    {
        double temp = h[k] - N*Pk;
        Hi = Hi + temp*temp / (N*Pk);

        /*qDebug() << "==========================================";
        qDebug() << "temp =" << temp;
        qDebug() << "Pk[" << k << "] =" << Pk;
        qDebug() << "h[" << k << "] =" << h[k];
        qDebug() << "N*Pk[" << k << "] =" << N*Pk;
        qDebug() << "N = " << N << "; Pk[" << k << "] =" << Pk;
        qDebug() << "k = " << k << "; Hi = " << Hi;*/
    }

    for (int i = 0; i < m; i++){
        mapData->operator [](i) = QCPData(i, h[i]);
        arrX.insert(i, h[i]);
        arrY.insert(i, i);
    }

    le_Chi->setText(QString::number(Hi));
    le_d->setText(QString::number(d));
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








