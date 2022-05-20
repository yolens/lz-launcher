#ifndef LZWINDOW_H
#define LZWINDOW_H

#include <QMainWindow>
#include "LZControl_global.h"
#include "LZGraphicsView.h"
#include "LTreeMode.h"

namespace Ui {
class LZWindow;
}

class LZCONTROL_EXPORT LZWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LZWindow(QWidget *parent = nullptr);
    ~LZWindow();

    void init();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    void createDockWindows();
private:
    Ui::LZWindow *ui;
    LZGraphicsView *m_pView = nullptr;
    LTreeMode *m_pTreeMode = nullptr;
};

#endif // LZWINDOW_H
