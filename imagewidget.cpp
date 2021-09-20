#include "imagewidget.h"

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent)
{
    isSetCP=false;  // 不能添加控制点对
    isWarpMode_idw=false;   // 默认使用RBF
    is_inaff=false; // 不在仿射集中
    ptr_image_ = new QImage();
    ptr_image_backup_ = new QImage();
    //img_warp=new Warping();
    img_warp=NULL;
    cur_line=NULL;

}

ImageWidget::~ImageWidget(void)
{
    if(img_warp!=NULL) {delete img_warp;    img_warp=NULL;}
    if(cur_line!=NULL) {delete cur_line;    cur_line=NULL;}
    if(ptr_image_!=NULL){
        delete ptr_image_;
        ptr_image_=NULL;
    }
    if(ptr_image_backup_!=NULL){
        delete ptr_image_backup_;
        ptr_image_backup_=NULL;
    }

}
void ImageWidget::mousePressEvent(QMouseEvent *event){
    if(isSetCP){
        if(Qt::LeftButton==event->button()){
            is_inaff=false;
            start = end = event->pos();
        }
        else if(Qt::RightButton==event->button()){
            is_inaff=true;
            start = end = event->pos();
        }
        update();
    }
}
void ImageWidget::mouseMoveEvent(QMouseEvent *event){
    if(isSetCP){
        end = event->pos();
        update();
    }

}
void ImageWidget::mouseReleaseEvent(QMouseEvent *event){
    if(isSetCP){
        end = event->pos();
        int dw = (width()-ptr_image_->width())/2;
        int dh = (height()-ptr_image_->height())/2;
        if(start.x()-dw<0 || start.y()-dh<0 ||end.x()-dw<0 || end.y()-dh<0){

        }
        else{
            cur_line=new Line(start.x()-dw,start.y()-dh,end.x()-dw,end.y()-dh);
            //cout<<cur_line<<" :cur_line"<<endl;
            //cout<<"add cp"<<endl;
            img_warp->AddCP(cur_line,is_inaff);
            cur_line=NULL;
        }
        update();
    }
}
void ImageWidget::paintEvent(QPaintEvent *paintevent)
{
    QPainter painter;
    painter.begin(this);

    // Draw background
    painter.setBrush(Qt::lightGray);
    QRect back_rect(0, 0, width(), height());
    painter.drawRect(back_rect);

    int dw = (width()-ptr_image_->width())/2;
    int dh = (height()-ptr_image_->height())/2;
    // Draw image
    QRect rect = QRect( dw, dh, ptr_image_->width(), ptr_image_->height());
    painter.drawImage(rect, *ptr_image_);

    if(isSetCP){
        //cout<<"draw cp"<<endl;
        img_warp->DrawCP(painter,dw,dh);
        painter.setPen(QPen(Qt::green,10,Qt::SolidLine,Qt::RoundCap));
        painter.drawPoint(start);
        painter.setPen(QPen(Qt::blue,10,Qt::SolidLine,Qt::RoundCap));
        painter.drawPoint(end);
        painter.setPen(Qt::red);
        painter.drawLine(start,end);
    }
    painter.end();
}

void ImageWidget::Open()
{
    // Open file
    QString fileName = QFileDialog::getOpenFileName(this, tr("Read Image"), ".", tr("Images(*.bmp *.png *.jpg)"));

    // Load file
    if (!fileName.isEmpty())
    {
        ptr_image_->load(fileName);
        *(ptr_image_backup_) = *(ptr_image_);
    }

    //ptr_image_->invertPixels(QImage::InvertRgb);
    //*(ptr_image_) = ptr_image_->mirrored(true, true);
    //*(ptr_image_) = ptr_image_->rgbSwapped();
    cout<<"image size: "<<ptr_image_->width()<<' '<<ptr_image_->height()<<endl;
    update();
}

void ImageWidget::Save()
{
    SaveAs();
}

void ImageWidget::SaveAs()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), ".", tr("Images(*.bmp *.png *.jpg)"));
    if (filename.isNull())
    {
        return;
    }

    ptr_image_->save(filename);
}

void ImageWidget::Invert()
{
    for (int i=0; i<ptr_image_->width(); i++)
    {
        for (int j=0; j<ptr_image_->height(); j++)
        {
            QRgb color = ptr_image_->pixel(i, j);
            ptr_image_->setPixel(i, j, qRgb(255-qRed(color), 255-qGreen(color), 255-qBlue(color)) );
        }
    }

    // equivalent member function of class QImage
    // ptr_image_->invertPixels(QImage::InvertRgb);
    update();
}

void ImageWidget::Mirror(bool ishorizontal, bool isvertical)
{
    QImage image_tmp(*(ptr_image_));
    int width = ptr_image_->width();
    int height = ptr_image_->height();

    if (ishorizontal)
    {
        if (isvertical)
        {
            for (int i=0; i<width; i++)
            {
                for (int j=0; j<height; j++)
                {
                    ptr_image_->setPixel(i, j, image_tmp.pixel(width-1-i, height-1-j));
                }
            }
        }
        else			//仅水平翻转
        {
            for (int i=0; i<width; i++)
            {
                for (int j=0; j<height; j++)
                {
                    ptr_image_->setPixel(i, j, image_tmp.pixel(width-1-i, j));
                }
            }
        }

    }
    else
    {
        if (isvertical)		//仅垂直翻转
        {
            for (int i=0; i<width; i++)
            {
                for (int j=0; j<height; j++)
                {
                    ptr_image_->setPixel(i, j, image_tmp.pixel(i, height-1-j));
                }
            }
        }
    }

    // equivalent member function of class QImage
    //*(ptr_image_) = ptr_image_->mirrored(true, true);
    update();
}

void ImageWidget::TurnGray()
{
    for (int i=0; i<ptr_image_->width(); i++)
    {
        for (int j=0; j<ptr_image_->height(); j++)
        {
            QRgb color = ptr_image_->pixel(i, j);
            int gray_value = (qRed(color)+qGreen(color)+qBlue(color))/3;
            ptr_image_->setPixel(i, j, qRgb(gray_value, gray_value, gray_value) );
        }
    }

    update();
}

void ImageWidget::Restore()
{
    *(ptr_image_) = *(ptr_image_backup_);
    if(img_warp!=NULL){
        delete img_warp;
        img_warp=NULL;
    }
    update();
}

void ImageWidget::SetControlPoints(){
    isSetCP=!isSetCP;
}
void ImageWidget::SetMode_IDW(){
    isWarpMode_idw=true;
    isSetCP=true;
    if(img_warp!=NULL) {delete img_warp;    img_warp=NULL;}
    img_warp=new WarpIDW();
}
void ImageWidget::SetMode_RBF(){
    isWarpMode_idw=false;
    isSetCP=true;
    if(img_warp!=NULL) {delete img_warp;    img_warp=NULL;}
    img_warp=new WarpRBF();
    cout<<"SetRBF"<<endl;
}
void ImageWidget::StartWarp(){
    if(img_warp!=NULL){
        int n=img_warp->Init();
//        cout<< "out of init"<<endl;
        if(isWarpMode_idw && n<3) return;
        int width=ptr_image_->width();
        int height=ptr_image_->height();
        QImage image_tmp(*(ptr_image_));    // 用于变换的img
        ptr_image_->fill(QColor(255, 255, 255));    // 原图先填上空白
//        cout<<"into-for"<<endl;
        img_warp->SetWH(width,height);
        for(int i=0;i<width;i++){
            for(int j=0;j<height;j++){
                // cout<<"["<<i<<", "<<j<<"] =>";
                MatrixXf newpos=img_warp->WarpAtij(i,j);
                // cout<<"["<<newpos(0,0)<<", "<<newpos(1,0)<<"]"<<endl;
                if(newpos(0,0)>=0&&newpos(0,0)<width && newpos(1,0)>=0&&newpos(1,0)<height){
                    ptr_image_->setPixel(newpos(0,0),newpos(1,0),image_tmp.pixel(i, j));
                    // 新位置在范围内
                    img_warp->Setchanged(newpos(0,0),newpos(1,0));
                }
            }
        }
        isSetCP=false;
        img_warp->fillHole(ptr_image_);
        update();
    }
}
