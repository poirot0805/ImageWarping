#include "warprbf.h"

WarpRBF::WarpRBF()
{

}

WarpRBF::~WarpRBF(){
    for(auto&& it: cp){
        if(it) {delete it; it=NULL;}
    }
}

void WarpRBF::SetU(double u){

}
int WarpRBF::Init(){
    int n=cp.size();
    int a_num=0;
    int r_num=0;
    MatrixXf temp_p=MatrixXf::Zero(2,1);
    MatrixXf temp_q=MatrixXf::Zero(2,1);
    for(int i=0;i<n;i++){
        temp_p(0,0)=cp[i]->getStartPoint().x();
        temp_p(1,0)=cp[i]->getStartPoint().y();
        temp_q(0,0)=cp[i]->getEndPoint().x();
        temp_q(1,0)=cp[i]->getEndPoint().y();
        if(in_affineset[i]){
            cpAP.push_back(temp_p);
            cpAQ.push_back(temp_q);
            a_num++;
        }
        cpRP.push_back(temp_p);
        cpRQ.push_back(temp_q);

    }
    M = MatrixXf::Zero(2, 2);
    b = MatrixXf::Zero(2, 1);
    if(a_num==0){
        M<<1,0,0,1;
        //cout<<"anum==0"<<endl;
    }
    else if(a_num==1){
        M<<1,0,0,1;
        b=cpAQ[0]-cpAP[0];
    }
    else if(a_num==2){//平移+缩放
        M << 0, -1,  1, 0;
        float s, a1, a2;

        s = ((cpAP[0] - cpAP[1]).transpose() * (cpAP[0] - cpAP[1]))(0, 0);
        a1 = ((cpAP[0] - cpAP[1]).transpose() * (cpAQ[0] - cpAQ[1]))(0, 0);
        a2 = ((cpAP[0] - cpAP[1]).transpose() * (M * (cpAQ[0] - cpAQ[1])))(0, 0);
        M << a1 / s, a2 / s,
           -a2 / s, a1 / s;
        b = cpAQ[0] - M * cpAP[0];
    }
    else{
        // 最小二乘
        MatrixXf tempx=MatrixXf::Zero(a_num,3);
        MatrixXf tempy=MatrixXf::Zero(a_num,2);
        for(int i=0;i<a_num;i++){
            tempx(i,0)=1;
            tempx(i,1)=cpAP[i](0,0);tempx(i,2)=cpAP[i](1,0);
            tempy(i,0)=cpAQ[i](0,0);tempy(i,1)=cpAQ[i](1,0);
        }
        MatrixXf para=MatrixXf::Zero(3,2);
        para=(tempx.transpose() * tempx).inverse() * tempx.transpose() * tempy;
        b <<para(0,0),para(0,1);
        M <<para(1,0),para(1,1),
            para(2,0),para(2,1);

    }
    // r(i):min(pi,pj)
    r.resize(n);
    MatrixXf dp=MatrixXf::Zero(2,1);
    for(int i=0;i<n;i++){
        r[i]=FLT_MAX;
        for(int j=0;j<n;j++){
            if(j==i) continue;
            dp=cpRP[j]-cpRP[i];
            float w=(dp.transpose()*dp)(0,0);
            if(r[i]>w){
                r[i]=w;
            }
        }
    }
    // a: =>sum(ai*R(d))=F-A(X)
    MatrixXf G=MatrixXf::Zero(n,n);
    MatrixXf temp_a=MatrixXf::Zero(n,2);
    MatrixXf temp_f=MatrixXf::Zero(n,2);
    theta.resize(n);
    for(int j=0;j<n;j++){
        //G(j,i)=> pj在aiRi(d)
        for(int i=0;i<n;i++){
            dp=cpRP[j]-cpRP[i];
            float dist=(dp.transpose()*dp)(0,0);
            G(j,i) = getBF(i,dist);
        }
        dp=cpRQ[j]-M*cpRP[j]-b;
        temp_f(j,0)=dp(0,0);
        temp_f(j,1)=dp(1,0);
    }
    temp_a = G.colPivHouseholderQr().solve(temp_f);
    for(int i=0;i<n;i++){
        dp(0,0)=temp_a(i,0);
        dp(1,0)=temp_a(i,1);
        theta[i]=dp;    // bug1: 之前用的push_back，但我事先已经resize过了
    }
    return n;
}

double WarpRBF::getBF(int index, float dist){
    return sqrt(dist+r[index]); // bug2: dist是一个平方
}
MatrixXf WarpRBF::WarpAtij(int x, int y){
    int n = cpRP.size();
    //cout<<"this is warp at ij"<<endl;
    MatrixXf oldpos = MatrixXf::Zero(2, 1);
    oldpos << x,y;
    MatrixXf new_ij = MatrixXf::Zero(2, 1);
    MatrixXf dp=MatrixXf::Zero(2,1);
    for(int i=0;i<n;i++){
        dp=oldpos-cpRP[i];
        float dist=(dp.transpose()*dp)(0,0);
        float Rd=getBF(i,dist);
        //cout<<"before newid"<<endl;
        new_ij+=Rd * theta[i];
    }
    new_ij += M * oldpos + b;
//    SetChangedIJ(new_ij);
    return new_ij;
}


