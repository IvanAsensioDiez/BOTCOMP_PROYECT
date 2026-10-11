#include "inclinacionwidget.h"

#include <QPainter>
#include <QtMath>

InclinacionWidget::InclinacionWidget(QWidget *parent): QWidget(parent){}

void InclinacionWidget::setAngulo(float angulo){m_angulo = angulo; update();}

void InclinacionWidget::paintEvent(QPaintEvent *){
    QPainter p(this);

    p.setRenderHint(QPainter::Antialiasing);

    int r = qMin(width(), height()) / 2 - 10;

    QPoint centro(width()/2, height()/2);

    p.setPen(QPen(Qt::white, 2));

    p.drawEllipse(centro, r, r);
    p.drawLine(centro.x(), centro.y(), centro.x(), centro.y() - r);

    double rad = qDegreesToRadians(m_angulo - 90.0);
    int x = centro.x() + r * cos(rad);
    int y = centro.y() + r * sin(rad);

    p.setBrush(Qt::red);

    p.drawEllipse(QPoint(x, y), 8, 8);
}