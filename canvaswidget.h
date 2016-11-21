#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QMouseEvent>
#include <QFileDialog>
#include <QResizeEvent>
#include <QScrollBar>
#include <QCheckBox>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>

#include <QFileDialog>
#include <QScrollArea>
#include <QMessageBox>

//#include "opencv2/imgcodecs.hpp"
//#include <opencv2/core/utility.hpp>

class CanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CanvasWidget(QWidget *parent = 0);

private:
    void setupGUI();
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    //to declare input widgets as input variables
    QPushButton* m_btnLoadImg;
    QPushButton* m_btnProcessImgBW;
    QPushButton* m_btnFindImgContours;
    QPushButton* m_btnProcessImgBlobs;
    QPushButton* m_btnSaveImg;

    QLabel* m_lblImgField;
    QScrollArea* m_scrollArea;
    QLabel* lblImageSize; //
    QCheckBox* checkBoxFitToWindow;
    QLabel* lblMouseClick;
    QLabel* lblContours;
    QLabel* lblCurrentContour;

    QVBoxLayout* rightLayout;
    QVBoxLayout* leftLayout;
    QHBoxLayout* mainLayout;

    QVector<QRgb>  sColorTable;
    cv::Mat image;
    cv::Mat imageGray;   
    cv::Mat imageOut;
    cv::Mat imageOutProcessed;
    int w, h, nc;

    std::vector< std::vector<cv::Point> > contours;
    int numOfContours;
    cv::Mat cclMap;

    //(x,y) - mouse coordinates w.r.t. the canvaswidget coord.system
    int x, y;
    //the id of the figure that user clicked on
    int currentFigure;

    QImage *inputImage; //
    QImage *outputImage; //


protected:
    //automatically grabs the mouse when a mouse button is pressed inside a widget
    void mousePressEvent(QMouseEvent* me);
    void resizeEvent(QResizeEvent *resizeEvent);

private slots:
    //slot is a function definition
    void onBtnLoadImgFromFileClicked();
    void onBtnProcessImgBWClicked();
    void onBtnFindImgContoursClicked();
    void onBtnProcessImgBlobsClicked();
    void onBtnSaveImgClicked();

    void fitToWindow(); //*
    void scaleImage(double factor); //*
    void updateZoomControls();




};

#endif // CANVASWIDGET_H
