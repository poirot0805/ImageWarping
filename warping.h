#ifndef WARPING_H
#define WARPING_H
#include <vector>
#include <line.h>
#include <Eigen/Dense>
#include <QImage>
#include <iostream>
using namespace Eigen;
using namespace std;

class Warping
{
public:
    Warping();
    virtual ~Warping();

    virtual MatrixXf WarpAtij(int x,int y);
    virtual int Init();
    virtual void SetU(double u);

    void AddCP(Line* cur_line,bool in);
    void DrawCP(QPainter &painter,int dw,int dh);
    void fillHole(QImage *img);
    void SetWH(int w,int h);
    void Setchanged(int i,int j){
        ischanged[i][j]=true;
    }
    QColor computeRGB(QColor a1,QColor a2);
protected:
    vector<Line*> cp;
    vector<bool> in_affineset;
    double u;
    bool **ischanged;
    int width_;
    int height_;
};

#endif // WARPING_H
