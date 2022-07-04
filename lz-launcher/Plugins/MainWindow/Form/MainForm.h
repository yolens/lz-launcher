#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>

namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT
public:
    enum PageType {
        Views = 0,
        Coms,
    };
public:
    explicit MainForm(QWidget *parent = nullptr);
    ~MainForm();

    void init();
signals:
    void action(const int type);
private slots:
    void on_pushButton_views_clicked();

    void on_pushButton_coms_clicked();

private:
    Ui::MainForm *ui;
};

#endif // MAINFORM_H
