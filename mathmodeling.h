#ifndef MATHMODELING_H
#define MATHMODELING_H

#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
QT_END_NAMESPACE

class MathModeling : public QDialog
{
    Q_OBJECT

public:
    explicit MathModeling(QWidget *parent = 0);
    ~MathModeling();

private:
    QTabWidget *tabWidget;
    QVBoxLayout *mainLayout;
};

#endif // MATHMODELING_H
