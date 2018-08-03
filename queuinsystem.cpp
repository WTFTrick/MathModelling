#include "queuinsystem.h"

QueuinSystem::QueuinSystem(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QGridLayout;
    vl = new QVBoxLayout;
    hl = new QHBoxLayout;
    gl = new QGridLayout;
    gl2 = new QGridLayout;
    btn = new QPushButton;
    btn->setText("Вычислить");

    Name = new QLabel;
    Name->setText("Система массового обслуживания:");
    Name->setAlignment(Qt::AlignHCenter);
    QFont f( "Arial", 18, QFont::Bold);
    Name->setFont(f);

    ls = new QLabel;
    ls->setText("Лямбда:");
    ls->setAlignment(Qt::AlignHCenter);

    ld = new QLabel;
    ld->setText("Ср. время обслуживания 1 заявки:");
    ld->setWordWrap(true);
    ld->setAlignment(Qt::AlignHCenter);
    //ld->setWordWrap(true);

    lN = new QLabel;
    lN->setText("Количество случ.чисел:");
    lN->setAlignment(Qt::AlignHCenter);
    lN->setWordWrap(true);
    lm = new QLabel;
    lm->setText("Число ячеек гистограммы:");
    lm->setAlignment(Qt::AlignHCenter);
    lm->setWordWrap(true);

    sb_lambda = new QDoubleSpinBox;
    sb_lambda->setMaximum(100);
    sb_lambda->setMinimum(0);
    sb_lambda->setValue(2);
    sb_lambda->setSingleStep(1);

    sb_t_serv = new QSpinBox;
    sb_t_serv->setMaximum(100);
    sb_t_serv->setMinimum(0);
    sb_t_serv->setValue(2);
    sb_t_serv->setSingleStep(1);
    //sb_t_serv->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    sb_N = new QSpinBox;
    sb_N->setMaximum(99);
    sb_N->setMinimum(0);
    sb_N->setValue(10);

    sb_m = new QSpinBox;
    sb_m->setMaximum(100);
    sb_m->setMinimum(0);
    sb_m->setValue(6);

    gl->addWidget(ls, 0, 0);
    gl->addWidget(ld, 0, 1);
    gl->addWidget(lN, 0, 2);
    gl->addWidget(lm, 0, 3);

    gl->addWidget(sb_lambda, 1, 0);
    gl->addWidget(sb_t_serv, 1, 1);
    gl->addWidget(sb_N, 1, 2);
    gl->addWidget(sb_m, 1, 3);

    grp_bx = new QGroupBox;
    grp_bx_2 = new QGroupBox;

    grp_bx->setLayout(gl);
    grp_bx->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    grp_bx_2->setLayout(gl2);
    grp_bx_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    l_T_line = new QLabel;
    l_T_sys = new QLabel;
    l_N_vac = new QLabel;
    l_A = new QLabel;
    l_Q = new QLabel;

    l_T_line->setText("Ср. время ожидания заявки в очереди:");
    l_T_line->setWordWrap(true);
    l_T_sys->setText("Ср. время пребывания заявки в СМО: ");
    l_T_sys->setWordWrap(true);
    l_N_vac->setText("Ср. число свободных каналов:");
    l_N_vac->setWordWrap(true);
    l_A->setText("Абсолютная пропускная способность:");
    l_A->setWordWrap(true);
    l_Q->setText("Относительная пропускная способность:");
    l_Q->setWordWrap(true);

    l_T_line->setAlignment(Qt::AlignHCenter);
    l_T_sys->setAlignment(Qt::AlignHCenter);
    l_N_vac->setAlignment(Qt::AlignHCenter);
    l_A->setAlignment(Qt::AlignHCenter);
    l_Q->setAlignment(Qt::AlignHCenter);

    gl2->addWidget(l_T_line, 0, 0);
    gl2->addWidget(l_T_sys, 0, 1);
    gl2->addWidget(l_N_vac, 0, 2);
    gl2->addWidget(l_A, 0, 3);
    gl2->addWidget(l_Q, 0, 4);

    le_T_line = new QLineEdit;
    le_T_sys = new QLineEdit;
    le_vac = new QLineEdit;
    le_Q = new QLineEdit;
    le_A = new QLineEdit;

    le_T_line->setReadOnly(true);
    le_T_sys->setReadOnly(true);
    le_vac->setReadOnly(true);
    le_Q->setReadOnly(true);
    le_A->setReadOnly(true);

    gl2->addWidget(le_T_line, 1, 0);
    gl2->addWidget(le_T_sys, 1, 1);
    gl2->addWidget(le_vac, 1, 2);
    gl2->addWidget(le_Q, 1, 3);
    gl2->addWidget(le_A, 1, 4);

    vl->addWidget(Name);
    hl->addWidget(btn);
    hl->addWidget(grp_bx);
    hl->addWidget(grp_bx_2);

    mainLayout->addLayout(vl, 1, 0);
    mainLayout->addLayout(hl, 2, 0);
    mainLayout->setAlignment(Qt::AlignTop);
    setLayout(mainLayout);

    CreatorConnections();
}


void QueuinSystem::CreatorConnections()
{
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(Count()));
}

int QueuinSystem::Factorial(int number)
{
    if (number < 0) //Ошибка. Факториал может быть только у неотрицательного числа
    {
        qDebug() << "Factorial: given number is negative!";   //Вывод сообщения на экран
        exit(0); //Аварийное завершение программы
    }

    if (number < 2)
        return 1; //Факториалы нуля и единицы по определению равны единице
    else
        return number*Factorial(number - 1);
}

void QueuinSystem::Count()
{
    double lambda = sb_lambda->value();
    int n = sb_N->value();
    int m = sb_m->value();
    double t_serv = sb_t_serv->value();

    /*qDebug() << "Lambda =" << lambda;
    qDebug() << "n =" << n;
    qDebug() << "m =" << m;
    qDebug() << "t_serv =" << t_serv;*/

    //double lambda;               //Интенсивность поступления требований
    //double t_serv;//Среднее время обслуживания одной заявки
    //const int n;       //Количество каналов обслуживания
    //const int m;     //Количество мест в очереди
    double p[n + m + 1];//Массив вероятностей

    double alpha = lambda*t_serv;//Коэффициент загрузки
    //qDebug() << "Коэффициент загрузки alpha =" << alpha;

    double temp1 = 0;//Первая временная переменная, необходимая для расчета p[0]
    for (int k = 1; k <= n; k++){
        temp1 = temp1 + exp(k*log(alpha)) / Factorial(k);
        //qDebug() << "temp1 =" << temp1;
    }

    double temp2 = 0;               //Вторая временная переменная, необходимая для расчета p[0]
    for (int s = 1; s <= m; s++){
        temp2 = temp2 + exp(s*log(alpha / n));
        //qDebug() << "temp2 =" << temp2;
    }

    p[0] = 1 / (1 + temp1 + temp2*exp(n*log(alpha)) / Factorial(n)); //Вероятность простоя СМО

    for (int k = 1; k <= n; k++)
    {
        p[k] = p[0] * exp(k*log(alpha)) / Factorial(k);
        //qDebug() << "p[" << k << "] =" << p[k];
    }

    for (int s = 1; s <= m; s++){
        p[n + s] = p[0] * exp(n*log(alpha))*exp(s*log(alpha / n)) / Factorial(n);
        //qDebug() << "p[" << n << "+ " << s << "] =" << p[n+s];
    }

    double N_buzy = 0;                               //Среднее число каналов, занятых обслуживанием
    for (int k = 1; k <= n; k++){
        N_buzy = N_buzy + k*p[k];
        //qDebug() << "N_buzy =" << N_buzy;
    }
    for (int s = 1; s <= m; s++)
    {
        N_buzy = N_buzy + n*p[n + s];
        //qDebug() << "N_buzy =" << N_buzy;
    }

    double N_vac = n - N_buzy;                //Среднее число свободных каналов

    //qDebug() << "N_vac" << N_vac;

    double N_req = 0;                                //Среднее число заявок в очереди
    for (int s = 1; s <= m; s++) {
        N_req = N_req + s*p[n + s];
        //qDebug() << "N_req" << N_req;
    }

    double T_line = N_req*t_serv;             //Среднее время ожидания заявки в очереди
    double T_sys = T_line + t_serv;          //Среднее время обслуживания заявки в СМО

    double Q = 1 - p[n + m];                      //Относительная пропускная способность СМО
    double A = lambda*Q;

    //qDebug() << "N_Vac =" << N_vac;

    le_T_line->setText(QString::number(T_line));
    le_T_sys->setText(QString::number(T_sys));
    le_vac->setText(QString::number(N_vac));
    le_Q->setText(QString::number(Q));
    le_A->setText(QString::number(A));
}



