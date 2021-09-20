#include "line.h"

Line::Line()
{

}

Line::~Line(){
}
Line::Line(QPoint s,QPoint e){
    start=s;
    end=e;
}
Line::Line(int start_x,int start_y,int end_x,int end_y){
    start=QPoint(start_x,start_y);
    end=QPoint(end_x,end_y);
}
QPoint Line::getStartPoint(){
    return start;
}
QPoint Line::getEndPoint(){
    return end;
}
void Line::Draw(QPainter &painter,int dw,int dh){
    painter.drawLine(start.x()+dw,start.y()+dh,end.x()+dw,end.y()+dh);
}
