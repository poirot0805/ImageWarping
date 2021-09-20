#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QFileDialog>
#include <QMouseEvent>
#include <iostream>
#include <vector>
#include <warping.h>
#include <warpidw.h>
#include <warprbf.h>

using namespace std;


class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWidget(QWidget *parent = nullptr);
    ~ImageWidget(void);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *paintevent);
public slots:
    // File IO
    void Open();												// Open an image file, support ".bmp, .png, .jpg" format
    void Save();												// Save image to current file
    void SaveAs();												// Save image to another file

    // Image processing
    void Invert();												// Invert pixel value in image
    void Mirror(bool horizontal=false, bool vertical=true);		// Mirror image vertically or horizontally
    void TurnGray();											// Turn image to gray-scale map
    void Restore();												// Restore image to origin

    // Warp
    void SetControlPoints();
    void SetMode_IDW();
    void SetMode_RBF();
    void StartWarp();
private:
    bool is_inaff;
    bool isSetCP;
    bool isWarpMode_idw;
    QImage		*ptr_image_;				// image
    QImage		*ptr_image_backup_;
    Warping     *img_warp;

    Line* cur_line; //cp
    QPoint start;
    QPoint end;
};

#endif // IMAGEWIDGET_H
