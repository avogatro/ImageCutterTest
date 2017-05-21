#ifndef VIDEOFRAMESURFACE_H
#define VIDEOFRAMESURFACE_H

#include <thread>
#include <QMutex>

#include <QPainter>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QOpenGLWidget>
#include <QDateTime>

#include <QReadWriteLock>

#include <QList>

///
/// \brief The VideoFrameSurface class output for MediaManager
/// input for VideoWidget,
///

class VideoFrameSurface : public QAbstractVideoSurface
{

    Q_OBJECT

public:
    VideoFrameSurface(QWidget *widget, int interval = 100, QObject *parent = NULL);
    ~VideoFrameSurface();
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const;
    bool isFormatSupported(const QVideoSurfaceFormat &format, QVideoSurfaceFormat *similar) const;


    bool present(const QVideoFrame& frame);

    bool start(const QVideoSurfaceFormat &format);
    void stop();


    QRect videoRect() const { return targetRect; }
    void updateVideoRect();

    void paint(QPainter *painter);

    QImage getImage();
    int getInterval() const;
    void setInterval(int value);

signals:
    void imageIsReady(QImage image);

public slots:
    void createImage();
private:
    QReadWriteLock readWriteLock;
    QWidget *widget;
    QImage* lastFrameImage;
    QVideoFrame currentFrame;
    QImage::Format imageFormat;
    QRect targetRect;
    QSize imageSize;
    QRect sourceRect;

    int interval;
    int timestamp;
    bool ifCreateImage;
    bool InvervalIsOver();
    void copyFrame(const QVideoFrame& frame);
};

#endif // VIDEOFRAMESURFACE_H
