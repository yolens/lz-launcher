#ifndef CARD_H
#define CARD_H

#include <QRadioButton>
#include "LZControl_global.h"
#include <QStyleOption>
#include <QPainter>

class LZCONTROL_EXPORT Card : public QRadioButton
{
    Q_OBJECT

public:
    explicit Card(QWidget *parent = nullptr);
    virtual ~Card();

    int id(){return m_id;};
    void setId(const int id){m_id = id;};
    virtual void setName(const QString& text){Q_UNUSED(text);};


signals:
    void start_action();
    void name_change_action(const QString& text);
    void selected_action();
    void add_action();
    void close_action();
private:
    int m_id;
};

#endif // CARD_H
