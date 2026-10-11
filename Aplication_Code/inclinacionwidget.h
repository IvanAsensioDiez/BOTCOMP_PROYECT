#ifndef INCLINACIONWIDGET_H
#define INCLINACIONWIDGET_H

#include <QWidget>

class InclinacionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InclinacionWidget(QWidget *parent = nullptr);
    void setAngulo(float angulo);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    float m_angulo = 0.0f;
};

#endif
