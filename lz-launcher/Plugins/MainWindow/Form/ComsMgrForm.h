#ifndef COMSMGRFORM_H
#define COMSMGRFORM_H

#include <QWidget>
#include <QPushButton>
#include <QMap>

namespace Ui {
class ComsMgrForm;
}

class ComsMgrForm : public QWidget
{
    Q_OBJECT

public:
    explicit ComsMgrForm(QWidget *parent = nullptr);
    ~ComsMgrForm();

    void init();
private slots:
    void on_clicked();
private:
    Ui::ComsMgrForm *ui;

    QMap<int, QPushButton*> m_buttonList;
};

#endif // COMSMGRFORM_H
