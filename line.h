#ifndef LINE_H
#define LINE_H

#include <QPoint>
#include <QPainter>
#include <iostream>
using namespace std;
class Line
{
public:
    Line();
    ~Line();
    Line(QPoint s,QPoint e);
    Line(int start_x,int start_y,int end_x,int end_y);
    QPoint getStartPoint();
    QPoint getEndPoint();
    void Draw(QPainter &painter,int dw,int dh);

private:
    QPoint start;
    QPoint end;
};

#endif // LINE_H
