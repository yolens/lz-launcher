#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "LZGraphicsView/LZGraphicsView.h"
#include "LZGraphicsView/LZWindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();
private:
    void switchPage(QWidget *w);
private slots:
    void on_action(const int type);


    void on_pushButton_back_clicked();

private:
    Ui::MainWindow *ui;

    LZWindow *m_pWindow = nullptr;
};

#endif // MAINWINDOW_H
