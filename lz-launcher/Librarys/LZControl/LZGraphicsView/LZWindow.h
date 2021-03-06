#ifndef LZWINDOW_H
#define LZWINDOW_H

#include <QMainWindow>
#include "LZControl_global.h"
#include "LZGraphicsView.h"
#include "LTreeMode.h"
#include "UnitTreeMode.h"
#include <QComboBox>

namespace Ui {
class LZWindow;
}

class LZCONTROL_EXPORT LZWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LZWindow(QWidget *parent = nullptr);
    ~LZWindow();

    void init(LProduct *p);
    void init(LUnit *unit, LProduct *product);
private slots:
    void on_sceneZoom(const int value);

private:
    void createDockWindows();
    void createMenus();
    void createToolbars();
protected:
    virtual void closeEvent(QCloseEvent* event) override;
signals:
    void destroyWindow();
private:
    Ui::LZWindow *ui;
    LZGraphicsView *m_pView = nullptr;
    LTreeMode *m_pTreeMode = nullptr;
    UnitTreeMode *m_pUnitTreeMode = nullptr;

    QMenu *m_fileMenu = nullptr;
    QMenu *m_itemMenu = nullptr;
    QMenu *m_aboutMenu = nullptr;

    QComboBox *m_sceneScaleCombo = nullptr;

    QToolBar *m_editToolBar = nullptr;
    QToolBar *m_pointerToolbar = nullptr;

    LProduct* m_pProduct = nullptr;
    LUnit* m_pUnit = nullptr;
};

#endif // LZWINDOW_H
