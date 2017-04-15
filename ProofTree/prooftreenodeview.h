#ifndef PROOFTREENODEVIEW_H
#define PROOFTREENODEVIEW_H

#include <QWidget>

namespace Ui {
class ProofTreeNodeView;
}

class ProofTreeNodeView : public QWidget
{
    Q_OBJECT

public:
    explicit ProofTreeNodeView(QWidget *parent = 0);
    ~ProofTreeNodeView();

private:
    Ui::ProofTreeNodeView *ui;
};

#endif // PROOFTREENODEVIEW_H
