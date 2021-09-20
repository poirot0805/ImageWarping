#include "warping.h"

Warping::Warping()
{

}

Warping::~Warping(){
    for(auto&& it: cp){
        if(it) {delete it; it=NULL;}
    }
    for(int i=0;i<width_;i++){
        delete []ischanged[i];
    }
    delete []ischanged;
}

void Warping::SetU(double u){

}

int Warping::Init(){
    return 0;
}
MatrixXf Warping::WarpAtij(int x, int y){
    return MatrixXf::Zero(2,1);
}
void Warping::AddCP(Line* cur_line,bool in){
    cp.push_back(cur_line);
    in_affineset.push_back(in);
}

void Warping::DrawCP(QPainter& painter,int dw,int dh){
    //cout<<cp.size()<<endl;
    for(size_t i=0;i<cp.size();i++){
        if(in_affineset[i]){
            painter.setPen(QPen(Qt::darkYellow,10,Qt::SolidLine,Qt::RoundCap));
            painter.drawPoint(cp[i]->getStartPoint().x()+dw,cp[i]->getStartPoint().y()+dh);
            painter.setPen(QPen(Qt::cyan,10,Qt::SolidLine,Qt::RoundCap));
            painter.drawPoint(cp[i]->getEndPoint().x()+dw,cp[i]->getEndPoint().y()+dh);
            painter.setPen(Qt::magenta);
        }
        else{
            painter.setPen(QPen(Qt::green,10,Qt::SolidLine,Qt::RoundCap));
            painter.drawPoint(cp[i]->getStartPoint().x()+dw,cp[i]->getStartPoint().y()+dh);
            painter.setPen(QPen(Qt::blue,10,Qt::SolidLine,Qt::RoundCap));
            painter.drawPoint(cp[i]->getEndPoint().x()+dw,cp[i]->getEndPoint().y()+dh);
            painter.setPen(Qt::red);
        }
        painter.setPen(Qt::red);
        cp[i]->Draw(painter,dw,dh);
    }
}
QColor Warping::computeRGB(QColor a1,QColor a2){
    int r=a1.red()+a2.green();
    int g=a1.green()+a2.green();
    int b=a1.blue()+a2.blue();
    r=r/2; g=g/2; b=b/2;
    if(r>255) r=255;
    if(g>255) g=255;
    if(b>255) b=255;
    return QColor(r,g,b);

}
void Warping::fillHole(QImage *img){
    int width=img->width();
    int height=img->height();
    QColor temp1,temp2;
    for(int i=1;i<height-1;i++){
        if(!ischanged[0][i]){
            temp1=computeRGB(img->pixelColor(0,i-1),img->pixelColor(0,i+1));
            img->setPixelColor(0,i,temp1);
        }
        if(!ischanged[width-1][i]){
            temp2=computeRGB(img->pixelColor(width-1,i-1),img->pixelColor(width-1,i+1));
            img->setPixelColor(width-1,i,temp2);
        }
    }
//    cout<<"ROW -1"<<endl;
    for(int i=1;i<width-1;i++){
        if(!ischanged[i][0]){
            temp1=computeRGB(img->pixelColor(i-1,0),img->pixelColor(i+1,0));
            img->setPixelColor(i,0,temp1);
        }
        if(!ischanged[i][height-1]){
            temp2=computeRGB(img->pixelColor(i-1,height-1),img->pixelColor(i+1,height-1));
            img->setPixelColor(i,height-1,temp2);
        }

    }
//    cout<<"COL -1"<<endl;
    QColor temp;
    for(int i=1;i<width-1;i++){
        for(int j=1;j<height-1;j++){
            if(ischanged[i][j]) continue;
            temp1=computeRGB(img->pixelColor(i-1,j-1),img->pixelColor(i+1,j-1));
            temp2=computeRGB(img->pixelColor(i-1,j+1),img->pixelColor(i+1,j+1));
            temp=computeRGB(temp1,temp2);
            img->setPixelColor(i,j,temp);
        }
    }
}
void Warping::SetWH(int w, int h){
    width_=w;
    height_=h;
    ischanged=new bool* [w];
    for(int i=0;i<w;i++){
        ischanged[i]=new bool[h];
    }
    for(int i=0;i<w;i++){
        for(int j=0;j<h;j++){
            ischanged[i][j]=false;
        }
    }
}
