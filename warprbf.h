#ifndef WARPRBF_H
#define WARPRBF_H

#include "warping.h"
class WarpRBF : public Warping
{
public:
    WarpRBF();
    ~WarpRBF();
    int Init() override;
    MatrixXf WarpAtij(int x,int y) override;
    void SetU(double u) override;

    double getBF(int index,float dist);

private:
    vector<MatrixXf> cpAP;
    vector<MatrixXf> cpAQ;
    vector<MatrixXf> cpRP;
    vector<MatrixXf> cpRQ;
    vector<MatrixXf> theta;
    MatrixXf M;
    MatrixXf b ;//MX+b

    vector<float>r; //min:d(pi,pj) for ix

};

#endif // WARPRBF_H
