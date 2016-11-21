#ifndef RESIZABLELABEL_H
#define RESIZABLELABEL_H

//#include <QObject>
#include <iostream>
#include <fstream>
#include <QLabel>
#include <QFileDialog>
#include <QPixmap>
#include <QPainter>

class ResizableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ResizableLabel(QWidget *parent = 0);

signals:

public slots:
    void loadPixmapFromPath();

public:
    void setScaledPixmap(const QPixmap& pixmap);

protected:
    void paintEvent(QPaintEvent*);

    QPixmap m_pixmap;

};

#endif // RESIZABLELABEL_H
