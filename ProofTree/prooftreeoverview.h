#ifndef PROOFTREEOVERVIEW_H
#define PROOFTREEOVERVIEW_H

#include <QWidget>
#include <QVBoxLayout>

namespace Ui {
class ProofTreeOverview;
}

class ProofTreeOverview : public QWidget
{
    Q_OBJECT

public:
    explicit ProofTreeOverview(QWidget *parent = 0);
    ~ProofTreeOverview();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ProofTreeOverview *ui;
    QVBoxLayout* layout;
};

#endif // PROOFTREEOVERVIEW_H
