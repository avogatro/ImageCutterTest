#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include "videoframesurface.h"

#include <QWidget>

class QAbstractVideoSurface;

class VideoFrameSurface;

class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    VideoWidget(QWidget *parent = 0);
    ~VideoWidget();

    VideoFrameSurface *getVideoSurface() const { return surface; }

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    VideoFrameSurface *surface;
};

#endif
