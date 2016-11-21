#include "resizablelabel.h"

ResizableLabel::ResizableLabel(QWidget *parent) :
    QLabel(parent)
{
    std::cout << "new ResizableLabel" << std::endl;
}


void ResizableLabel::setScaledPixmap(const QPixmap& pixmap){
    std::cout << "setScaledPixmap()" << std::endl;
    m_pixmap = pixmap;
    QSize m_originalSize = pixmap.size();
    update();
}


void ResizableLabel::loadPixmapFromPath(){
    std::cout << "loadPixmapFromPath()" << std::endl;
    QString qstr = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.jpg *.png *.bmp");
    std::cout << qstr.toStdString() << std::endl;

    QPixmap pixmap(qstr);
    if (!pixmap.isNull())
        setScaledPixmap(pixmap);
}

void ResizableLabel::paintEvent(QPaintEvent * event){
    QPainter painter(this);
    QPoint centerPoint(0,0);
    QPixmap scaledPixmap = m_pixmap;
    painter.drawPixmap(centerPoint, scaledPixmap);

}



/*
void ScaledPixmap::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    if (!m_pixmap.isNull())
    {
        QPoint centerPoint(0, 0);
        QSize scaledSize = overscaleEnabled() ? size() : (fitsToScreen(size()) ? m_originalSize : size());

        QPixmap scaledPixmap = m_pixmap.scaled(scaledSize, m_keepAspectRatio ? Qt::KeepAspectRatio : Qt::IgnoreAspectRatio);

        centerPoint.setX((size().width() - scaledPixmap.width()) / 2);
        centerPoint.setY((size().height() - scaledPixmap.height()) / 2);

        painter.drawPixmap(centerPoint, scaledPixmap);
    }

    QLabel::paintEvent(event);
}
*/
