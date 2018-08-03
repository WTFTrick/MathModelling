#include "mathmodeling.h"
#include "binomialdistr.h"
#include "poissondistr.h"
#include "uniformdistr.h"
#include "exponentialdistr.h"
#include "rileydistr.h"
#include "gaussdistr.h"
#include "cauchydistr.h"
#include "leastsquares.h"
#include "robustfit.h"
#include "montecarlointegration.h"
#include "celluralautomaton.h"
#include "queuinsystem.h"

MathModeling::MathModeling(QWidget *parent) : QDialog(parent)
{
    tabWidget = new QTabWidget;
    tabWidget->addTab(new RobustFit(), tr("Робастная подгонка прямой"));
    tabWidget->addTab(new LeastSquares(), tr("Подгонка прямой МНК"));
    tabWidget->addTab(new UniformDistr, tr("Равномерное распределение"));
    tabWidget->addTab(new CauchyDistr(), tr("Распределение Коши"));
    tabWidget->addTab(new GaussDistr(), tr("Распределение Гаусса"));
    tabWidget->addTab(new CelluralAutomaton(), tr("Клеточный автомат"));
    tabWidget->addTab(new BinomialDistr(), tr("Биномиальное распределение"));
    tabWidget->addTab(new ExponentialDistr, tr("Экспоненциальное распределение"));
    tabWidget->addTab(new PoissonDistr(), tr("Распределение Пуассона"));
    tabWidget->addTab(new RileyDistr(), tr("Распределение Рилея"));
    tabWidget->addTab(new MonteCarloIntegration(), tr("Метод Монте-Карло"));
    tabWidget->addTab(new QueuinSystem, tr("СМО"));

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);

    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
}

MathModeling::~MathModeling()
{
    delete tabWidget/*, mainLayout*/;
}
