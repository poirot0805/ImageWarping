#ifndef WARPIDW_H
#define WARPIDW_H

#include "warping.h"
class WarpIDW : public Warping
{
public:
    WarpIDW();
    ~WarpIDW();

    int Init() override;   // 初始化：记录Pi,Qi, =>初始化Ti =>计算Ti
    MatrixXf WarpAtij(int x,int y) override;   // 计算wi(X)和fi(X) 累加
    void SetU(double u) override;
private:
    vector<MatrixXf> cpP;
    vector<MatrixXf> cpQ;
    vector<MatrixXf> T;
    int v;
};

#endif // WARPIDW_H
