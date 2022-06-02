#ifndef DEVICEVIEW_H
#define DEVICEVIEW_H

#include <QWidget>

namespace Ui {
class DeviceView;
}

class DeviceCom;
class DeviceView : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceView(QWidget *parent = nullptr);
    ~DeviceView();

    void adjustView();
private:
    void initCom(DeviceCom *com);
protected:
    virtual void resizeEvent(QResizeEvent* event) override;
private slots:
    void on_updateView();
    void on_pushButton_add_clicked();

private:
    Ui::DeviceView *ui;
};

#endif // DEVICEVIEW_H
