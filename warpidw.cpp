#include "warpidw.h"

WarpIDW::WarpIDW()
{

}

WarpIDW::~WarpIDW(){
    for(auto&& it: cp){
        if(it) {delete it; it=NULL;}
    }
}
void WarpIDW::SetU(double u){
    v=2;
}
int WarpIDW::Init(){
    // 初始化：记录Pi,Qi, =>初始化Ti =>计算Ti
    // 能量最小化：for Ti
    //      for each j!=i:
    //          wi(pj)*(qi+Ti(pj-pi)-qj)^2
    //       求偏导：

    int n=cp.size();
    if(n<3) return n;
    for(int i=0;i<n;i++){
        MatrixXf temp_p=MatrixXf::Zero(2,1);
        MatrixXf temp_q=MatrixXf::Zero(2,1);
        MatrixXf temp_t=MatrixXf::Zero(2,2);
        temp_p(0,0)=cp[i]->getStartPoint().x();
        temp_p(1,0)=cp[i]->getStartPoint().y();
        temp_q(0,0)=cp[i]->getEndPoint().x();
        temp_q(1,0)=cp[i]->getEndPoint().y();
        cpP.push_back(temp_p);
        cpQ.push_back(temp_q);
        T.push_back(temp_t);
    }
    for(int i=0;i<n;i++){
        MatrixXf A=MatrixXf::Zero(2,2);
        MatrixXf B=MatrixXf::Zero(2,2);
        MatrixXf dp=MatrixXf::Zero(2,1);
        MatrixXf dq=MatrixXf::Zero(2,1);
        for(int j=0;j<n;j++){
            if(j==i) continue;
            dp=cpP[j]-cpP[i];
            dq=cpQ[j]-cpQ[i];
            float weight=1.0f/((dp.transpose()*dp)(0,0));
            A+=weight*dp*dp.transpose();
            B+=weight*dp*dq.transpose();
        }
        T[i]=B.transpose()*A.inverse();
    }
    return n;
}

MatrixXf WarpIDW::WarpAtij(int x, int y){
// 计算wi(X)和fi(X) =>累加
// wi(x)=theta_i(x)/sum(theta_j(x))
// fi(x)=qi+T(x-pi)
    int n=cp.size();
    vector<float> theta;
    theta.resize(n);
    float sum=0;

    MatrixXf old=MatrixXf::Zero(2,1);
    MatrixXf newpos=MatrixXf::Zero(2,1);
    old(0,0)=x; old(1,0)=y;
    bool isatcp=false;
    int i;
    for(i=0;i<n;i++){
        theta[i]=((old-cpP[i]).transpose()*(old-cpP[i]))(0,0);
        if(theta[i]<1E-6){
            isatcp=true;
            break;
        }
        sum+=1.0f/theta[i];
    }
    if(isatcp){
        newpos=cpQ[i];
        return newpos;
    }
    for(i=0;i<n;i++){
        float weight=(1.0f/theta[i])/sum;
        newpos+=weight* (cpQ[i]+T[i]*(old-cpP[i]));
    }
    return newpos;

}

