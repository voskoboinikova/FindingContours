#include "canvaswidget.h"




CanvasWidget::CanvasWidget(QWidget *parent) :
    QWidget(parent), m_btnLoadImg(0), m_btnProcessImgBW(0), m_btnFindImgContours(0), m_btnProcessImgBlobs(0), m_lblImgField(0), rightLayout(0),
    leftLayout(0), mainLayout(0)
{
    setupGUI();
    //setupGUI2();
}



void CanvasWidget::setupGUI(){
    std::cout << "setupGUI! " << std::endl;

    //define mouse-tracking
    setMouseTracking(true);

    m_lblImgField = new QLabel();
    m_lblImgField->setBackgroundRole(QPalette::Base);
    m_lblImgField->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored); //without it, image cant be scaled smaller than it is
    //This property holds whether the label will scale its contents to fill all available space.
    m_lblImgField->setScaledContents(true);

    m_scrollArea = new QScrollArea();
    m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setWidget(m_lblImgField);

    checkBoxFitToWindow = new QCheckBox("Fit To Window");
    updateZoomControls();


    inputImage = NULL;
    outputImage = NULL;


    m_btnLoadImg = new QPushButton("Open File...");
    m_btnProcessImgBW = new QPushButton("Do EdgeDetection");
    m_btnFindImgContours = new QPushButton("Find Contours");
    m_btnProcessImgBlobs = new QPushButton("Process Blobs");
    m_btnSaveImg = new QPushButton("Save Result");
    lblImageSize = new QLabel();
    lblMouseClick = new QLabel();
    lblContours = new QLabel();
    lblCurrentContour = new QLabel();


    m_btnLoadImg->setFixedWidth(150);
    m_btnProcessImgBW->setFixedWidth(150);
    m_btnFindImgContours->setFixedWidth(150);
    m_btnProcessImgBlobs->setFixedWidth(150);
    m_btnSaveImg->setFixedWidth(150);
    lblImageSize->setFixedWidth(200);
    lblMouseClick->setFixedWidth(200);
    lblContours->setFixedWidth(200);
    lblCurrentContour->setFixedWidth(200);

    //link between the signal and slot
    connect(m_btnLoadImg, SIGNAL(clicked()), this, SLOT(onBtnLoadImgFromFileClicked()) );
    connect(m_btnProcessImgBW, SIGNAL(clicked()), this, SLOT(onBtnProcessImgBWClicked()) );
    connect(m_btnFindImgContours, SIGNAL(clicked()), this, SLOT(onBtnFindImgContoursClicked()) );
    connect(m_btnProcessImgBlobs, SIGNAL(clicked()), this, SLOT(onBtnProcessImgBlobsClicked()));
    connect(m_btnSaveImg, SIGNAL(clicked()), this, SLOT(onBtnSaveImgClicked()) );
    connect(checkBoxFitToWindow, SIGNAL(stateChanged(int)), SLOT(updateZoomControls()));



    /*QGridLayout* gridLayout = new QGridLayout();
    gridLayout->addWidget(m_lblImgField, 0,0);
    gridLayout->addWidget(m_btnLoadImg, 0, 1);
    gridLayout->addWidget(m_btnProcessImgBW, 1,1);
    gridLayout->addWidget(m_btnProcessImgContours, 2,1);
    gridLayout->addWidget(m_btnSaveImg, 3,1);
    setLayout(gridLayout);
    */



    rightLayout = new QVBoxLayout();
    rightLayout->addWidget(m_btnLoadImg);
    rightLayout->addWidget(checkBoxFitToWindow);
    rightLayout->addSpacing(20);

    rightLayout->addWidget(m_btnProcessImgBW);
    rightLayout->addWidget(m_btnFindImgContours);
    rightLayout->addWidget(m_btnProcessImgBlobs);
    rightLayout->addWidget(m_btnSaveImg);
    rightLayout->addSpacing(40);


    rightLayout->addWidget(lblImageSize);
    rightLayout->addWidget(lblMouseClick);
    rightLayout->addWidget(lblContours);
    rightLayout->addWidget(lblCurrentContour);
    rightLayout->addStretch(1);

    leftLayout = new QVBoxLayout();
    leftLayout->addWidget(m_scrollArea);
    //leftLayout->addWidget(m_lblImgField);

    mainLayout = new QHBoxLayout();
    mainLayout->setMargin(21);
    mainLayout->setSpacing(6);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);


    setLayout(mainLayout);

}



void CanvasWidget::onBtnLoadImgFromFileClicked(){

    //1: predecessor, 2: name of the window, 3: working directory, 4: filter for file extensions
    QString qstr = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.jpg *.png *.bmp");

    delete(inputImage);
    delete(outputImage);

    /*********read to Qt variable****************/
    inputImage = new QImage(qstr);

    if (inputImage->isNull()) {
        QMessageBox::information(this, tr("Contours"),
                                 tr("Cannot load %1.").arg(qstr));
        return;
    }

    lblImageSize->setText(tr(" Image Size: %1 x %2 ").arg(inputImage->width()).arg(inputImage->height()));

    this->m_lblImgField->setPixmap(QPixmap::fromImage(*inputImage));
    this->m_lblImgField->resize(m_lblImgField->pixmap()->size());
    //fitToWindow();



    /**********read to the opencv variable*******/
    image = cv::imread(qstr.toStdString());
    if(! image.data){
        std::cout << "Problem loading the image" << std::endl;
    }

    // get image dimensions
    w = image.cols;         // width
    h = image.rows;         // height
    nc = image.channels();  // number of channels
}




void CanvasWidget::resizeEvent(QResizeEvent *resizeEvent) {
    if (checkBoxFitToWindow->isChecked()) {
        fitToWindow();
    }
}



void CanvasWidget::fitToWindow() {

    if (! m_lblImgField->pixmap()) {
        std::cout << "return!" << std::endl;
        return;
    }

    double a = m_scrollArea->width()/(double)inputImage->width();
    double b = m_scrollArea->height()/(double)inputImage->height();
    double factor = qMin(a,b);

    if (factor>1){
        factor = 1;
    }

     scaleImage(factor);
}



void CanvasWidget::scaleImage(double factor) {
    if (!m_lblImgField->pixmap()) {
        return;
    }

    double scaleFactor =factor;
    m_lblImgField->resize(scaleFactor * m_lblImgField->pixmap()->size());

    adjustScrollBar(m_scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(m_scrollArea->verticalScrollBar(), factor);

    //ui->btnZoomIn->setEnabled(scaleFactor < 1.0);
    //ui->btnZoomOut->setEnabled(scaleFactor > 0.3);
}


void CanvasWidget::adjustScrollBar(QScrollBar *scrollBar, double factor) {
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}



void CanvasWidget::updateZoomControls() {
    bool fitChecked = checkBoxFitToWindow->isChecked();

    m_scrollArea->setVerticalScrollBarPolicy(!fitChecked ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
    m_scrollArea->setHorizontalScrollBarPolicy(!fitChecked ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);

    if (fitChecked) {
        fitToWindow();
    }
}


void CanvasWidget::onBtnProcessImgBWClicked(){
    //the following code is copied from the onBtnProcessImgFindContoursClicked() function
    //step1: convert into gray (CV_8UC1)
    cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);
    //cv::medianBlur(imageGray, imageGray, 5);

    //step2: convert to binary (CV_8UC1)
    cv::Mat bw(image.size(), CV_8U, cv::Scalar(255));
    int thresh = 100;
    //cv::Canny( imageGray, bw, thresh, thresh*2, 3 );
    cv::threshold(imageGray, bw, thresh, 255, cv::THRESH_BINARY_INV);

    //stepx: convert to Qt format
    // only create our color table once
    if ( sColorTable.isEmpty() ){
          for ( int i = 0; i < 256; ++i )
              sColorTable.push_back( qRgb( i, i, i ) );
    }

    QImage img2 = QImage( bw.data, bw.cols, bw.rows, bw.step, QImage::Format_Indexed8 );
    img2.setColorTable( sColorTable );

    this->m_lblImgField->setPixmap(QPixmap::fromImage(img2));
    //this->m_lblImgField->resize(m_lblImgField->pixmap()->size());
    //fitToWindow();
}


void CanvasWidget::onBtnFindImgContoursClicked(){
    //step1: convert into gray (CV_8UC1)
    cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);

    //step2: convert to binary (CV_8UC1)
    cv::Mat bw(image.size(), CV_8U, cv::Scalar(255));

    int thresh = 100;
    //cv::Canny( imageGray, bw, thresh, thresh*2, 3 );
    cv::threshold(imageGray, bw, thresh, 255, cv::THRESH_BINARY_INV);
    // dilate canny output to remove potential holes between edge segments
    //cv::dilate(bw, bw, cv::Mat(), cv::Point(-1,-1));

    //step3: find contours in binary
    //cv::RETR_EXTERNAL: retrieves only the extreme outer contours.
    //cv::CHAIN_APPROX_NONE:  stores absolutely all the contour points.
    cv::findContours(bw, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    numOfContours = contours.size();
    lblContours->setText(tr("Number of Contours: \n %1").arg(numOfContours));

    cv::Mat result(image.size(), CV_8U, cv::Scalar(255));
    //void drawContours(InputOutputArray image, InputArrayOfArrays contours, int contourIdx, const Scalar& color, int thickness=1,
    //int lineType=8, InputArray hierarchy=noArray(), int maxLevel=INT_MAX, Point offset=Point() )
    //if contourIdx par-r is negative, all contours are drawn
    cv::drawContours(result, contours, -1, cv::Scalar(0), 1);

    //result is the vector containing information about the contours
    imageOut = result;


    //stepx: convert to Qt format 
    if ( sColorTable.isEmpty() ){
          for ( int i = 0; i < 256; ++i )
              sColorTable.push_back( qRgb( i, i, i ) );
    }

    QImage img3 = QImage( result.data, result.cols, result.rows, result.step, QImage::Format_Indexed8 );
    img3.setColorTable( sColorTable );

    this->m_lblImgField->setPixmap(QPixmap::fromImage(img3));
    //this->m_lblImgField->resize(m_lblImgField->pixmap()->size());




    //writing labels to cclMap
    cv::Mat empty(image.size(), CV_8U, cv::Scalar(0));
    cclMap = empty;

    for(int x=0; x<w; x++){
        for(int y=0; y<h; y++){

            cv::Point pt(x,y);
            for(int i=0; i<numOfContours; i++){
                //the function determines whether the point is inside(+1), outside(-1) or on (0) the contour
                int io = cv::pointPolygonTest(contours[i], pt, false);
                if(io==0 || io==1)
                    cclMap.at<uchar>(pt) = i+1;
            }

        }
    }


    //writing cclMap to a file
    std::ofstream output("cclMap.txt");
    if(output.is_open()){
        for(int y=0; y<h; y++){
            for(int x=0; x<w; x++){
                cv::Point2f pt(x,y);
                int a = cclMap.at<uchar>(pt);
                output << a;
            }
           output << std::endl;
        }
    }
    output.close();

}


void CanvasWidget::onBtnProcessImgBlobsClicked(){

    cv::Mat empty(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
    imageOutProcessed = empty;

    for(int x=0; x<w; x++){
          for(int y=0; y<h; y++){
                for(int i=1; i<=numOfContours; i++){
                    int a = cclMap.at<uchar>(y,x);
                    if(a==i){
                        imageOutProcessed.at<cv::Vec3b>(y,x)[0] = 180;
                        imageOutProcessed.at<cv::Vec3b>(y,x)[1] = 0;
                        imageOutProcessed.at<cv::Vec3b>(y,x)[2] = 100;
                        break;
                     }
                 }
           }
     }



    QImage img4 = QImage((const unsigned char*) (imageOutProcessed.data), imageOutProcessed.cols, imageOutProcessed.rows,imageOutProcessed.step, QImage::Format_RGB888);

    this->m_lblImgField->setPixmap(QPixmap::fromImage(img4));
    //this->m_lblImgField->resize(m_lblImgField->pixmap()->size());
}



void CanvasWidget::mousePressEvent(QMouseEvent* me){
    int margin = this->mainLayout->margin();

    //(x,y) is the position w.r.t. the canvas widget (global coordinates)
    x=me->x() - margin;
    y=me->y() - margin;

    int io;
    cv::Point2f pt(x,y);
    if(!contours.empty()){
        for(int i=0; i<numOfContours; i++){
            //the function determines whether the point is inside(+1), outside(-1) or on (0) the contour
            io = cv::pointPolygonTest(contours[i], pt, false);
            if(io==1 || io==0){
                //std::cout << "you clicked on the contour " << i+1 << std::endl;
                currentFigure = i+1;
                lblCurrentContour->setText(tr("Current Contour: %1 ").arg(currentFigure));
                break;
            }
            if(i==numOfContours-1){
                 lblCurrentContour->setText(tr("Current Contour: %1 ").arg(0));
            }
        }
    }

    lblMouseClick->setText(tr("Click on (x, y) =  %1, %2").arg(x).arg(y));


    /*
    cv::Mat imageOutCopy = imageOut;
    cv::drawContours(imageOutCopy, contours, currentFigure-1, cv::Scalar(0), 5);
    QImage img4 = QImage( imageOutCopy.data, imageOutCopy.cols, imageOutCopy.rows, imageOutCopy.step, QImage::Format_Indexed8 );
    img4.setColorTable( sColorTable );

    this->m_lblImgField->setPixmap(QPixmap::fromImage(img4));
    this->m_lblImgField->resize(m_lblImgField->pixmap()->size());
    */

}


void CanvasWidget::onBtnSaveImgClicked(){
    // save input and result
    cv::imwrite("image_input.png",image);  // "imwrite" assumes channel range [0,255]
    cv::imwrite("image_output.png",imageOut);

}

