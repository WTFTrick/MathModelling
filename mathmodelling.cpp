#include "mathmodelling.h"
#include "ui_mathmodelling.h"

MathModelling::MathModelling(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MathModelling)
{
    ui->setupUi(this);

    setWindowTitle("Mathematic Modelling");
    setCentralWidget(ui->TabWidget);

    Setups();
    CreatorConnections();
}

MathModelling::~MathModelling()
{
    delete ui;
}

void MathModelling::BinomialDistribution(double p, int N, int m)
{
    clearPlot();

    QCPDataMap  *mapData;
    QVector<double> arrX;
    QVector<double> arrY;
    QCPGraph *graph1;

    QCPBars *Bars = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    ui->customPlot->addPlottable(Bars);

    graph1 = ui->customPlot->addGraph();
    mapData = ui->customPlot->graph(0)->data(); //customPlot->graph(0)->setData(arrX, arrY);

    QPen graphPen;
    graphPen.setColor(QColor(0, 0, 0));
    graphPen.setWidthF(1);
    ui->customPlot->graph(0)->setPen(graphPen);
    ui->customPlot->graph(0)->setLineStyle((QCPGraph::LineStyle)(1));

    //const int N = 1000;                 //Количество случайных точек
    //const int m = 12;                 //Количество испытаний
    //double p = 0.4;                    //Вероятность удачного исхода
    double p_est;                //Оценка вероятности удачного исхода
    int Ki[N];                   //Массив количества испытаний, закончившихся удачно
    double h[m + 1];                //Массив гистограммы
    double Pk[m + 1];            //Массив вероятностей попадания в k-ю ячейку гистограммы
    srand((unsigned)time(0));    //Инициализация генератора псевдослучайных чисел

    arrX.resize(m + 1);
    arrY.resize(m + 1);

    for (int k = 0; k <= m; k++) h[k] = 0;        //Обнуление гистограммы
    p_est = 0;

    for (int i = 1; i < N; i++) //Основной цикл генерации N случайных чисел
    {
        Ki[i] = 0;
        for (int j = 0; j < m; j++) //Цикл моделирования из m испытаний
        {
            double r = rand()/(double)RAND_MAX;
            if (r < p)
                Ki[i] = Ki[i] + 1;
        }

        h[Ki[i]] = h[Ki[i]] + 1;
        p_est =p_est + Ki[i];
    }

    p_est = p_est/double(m*N);
    double q_est = 1 - p_est;
    Pk[0] = exp(m*log(q_est));      //Вероятность того, что не было ни одного удачного исхода

    for (int k = 0; k < m; k++)
        Pk[k + 1] = Pk[k]*(m - k)*p_est/((k + 1)*q_est);

    double Hi = 0;                                                     //Расчет критерия хи-квадрат
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

    QPen pen;
    pen.setWidthF(1.2);
    Bars->setName("Bars");
    pen.setColor(QColor(255, 131, 0));
    Bars->setPen(pen);
    Bars->setBrush(QColor(255, 131, 0, 50));
    Bars->setData(arrY, arrX);

    ui->customPlot->replot();
}

void MathModelling::mousePress()
{
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MathModelling::mouseWheel()
{
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MathModelling::BinomialChartBuilding()
{
    double p = ui->sb_prob->value();
    qDebug() << p;
    int N = ui->le_nmb_points->text().toInt();
    int m = ui->le_nmb_tests->text().toInt();

    BinomialDistribution(p, N, m);
}

void MathModelling::CreatorConnections()
{
    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    connect(ui->btn_build, SIGNAL(clicked(bool)), this, SLOT(BinomialChartBuilding()));
}

void MathModelling::clearPlot()
{
    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();
    ui->customPlot->clearPlottables();
    ui->customPlot->replot();
}

void MathModelling::Setups()
{
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );
    //ui->customPlot->xAxis->setLabel("x");
    //ui->customPlot->yAxis->setLabel("y");
    ui->customPlot->xAxis->setRange(-5, 14);
    ui->customPlot->yAxis->setRange(-5, 250);

    ui->le_nmb_points->setValidator( new QIntValidator(0, 2500, this) );
    ui->le_nmb_tests->setValidator( new QIntValidator(0, 100, this) );
}
