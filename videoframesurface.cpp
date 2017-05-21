#include "videoframesurface.h"

VideoFrameSurface::VideoFrameSurface(QWidget *widget,int interval, QObject *parent)
{

    this->widget= widget;
    lastFrameImage = new QImage();
    timestamp = QDateTime::currentMSecsSinceEpoch();
    this->setInterval(interval);
    ifCreateImage = false;
}

bool VideoFrameSurface::present(const QVideoFrame& frame)
{

    if (QAbstractVideoSurface::surfaceFormat().pixelFormat() != frame.pixelFormat()
                || QAbstractVideoSurface::surfaceFormat().frameSize() != frame.size()) {
            setError(IncorrectFormatError);
            stop();

            return false;
        } else {
            currentFrame = frame;

            widget->repaint(targetRect);
            //if (ifCreateImage && InvervalIsOver()){
            if (ifCreateImage){
                if(currentFrame.map(QAbstractVideoBuffer::ReadOnly))
                {
                    copyFrame(currentFrame);
                }
                currentFrame.unmap();
                ifCreateImage = false;
                //timestamp = QDateTime::currentMSecsSinceEpoch();
                emit imageIsReady(QImage(*lastFrameImage));
            }

            return true;
        }
}

QList<QVideoFrame::PixelFormat> VideoFrameSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB32
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB565
                << QVideoFrame::Format_RGB555;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}

bool VideoFrameSurface::isFormatSupported(const QVideoSurfaceFormat &format, QVideoSurfaceFormat *similar) const
{
    Q_UNUSED(similar);

    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    return imageFormat != QImage::Format_Invalid
            && !size.isEmpty()
            && format.handleType() == QAbstractVideoBuffer::NoHandle;
}

VideoFrameSurface::~VideoFrameSurface()
{

}

QImage VideoFrameSurface::getImage(){

    readWriteLock.lockForRead();
    QImage res = QImage(*lastFrameImage);
    readWriteLock.unlock();
    return res;
}

int VideoFrameSurface::getInterval() const
{
    return interval;
}

void VideoFrameSurface::setInterval(int value)
{
    if (value >0)
        interval = value;
}

void VideoFrameSurface::createImage()
{
    ifCreateImage = true;
}

bool VideoFrameSurface::InvervalIsOver()
{
    int a = QDateTime::currentMSecsSinceEpoch();
    int b = timestamp + interval;
    bool c = b>a;
    if (timestamp + interval > a){
        return false;
    }else if (timestamp +2*interval < a)
    {
        timestamp = a;
    }
    return true;
}

void VideoFrameSurface::copyFrame(const QVideoFrame& frame)
{
    readWriteLock.lockForWrite();
    delete lastFrameImage;

    this->lastFrameImage = new QImage(  frame.bits(),
                                        frame.width(),
                                        frame.height(),
                                        frame.bytesPerLine(),
                                        QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat())
                                        );
    readWriteLock.unlock();
}

bool VideoFrameSurface::start(const QVideoSurfaceFormat &format)
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) {
        this->imageFormat = imageFormat;
        imageSize = size;
        sourceRect = format.viewport();

        QAbstractVideoSurface::start(format);

        widget->updateGeometry();
        updateVideoRect();

        return true;
    } else {
        return false;
    }
}

void VideoFrameSurface::updateVideoRect()
{
    QSize size = surfaceFormat().sizeHint();
    size.scale(widget->size(), Qt::KeepAspectRatio);
   // size.scale(widget->size().boundedTo(size), Qt::KeepAspectRatio);
//
    targetRect = QRect(QPoint(0, 0), size);
    targetRect.moveCenter(widget->rect().center());
}

void VideoFrameSurface::paint(QPainter *painter)
{
    if (currentFrame.map(QAbstractVideoBuffer::ReadOnly)) {
        const QTransform oldTransform = painter->transform();

        if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop) {
           painter->scale(1, -1);
           painter->translate(0, -widget->height());
        }

        QImage image(
                currentFrame.bits(),
                currentFrame.width(),
                currentFrame.height(),
                currentFrame.bytesPerLine(),
                imageFormat);

        painter->drawImage(targetRect, image, sourceRect);

        painter->setTransform(oldTransform);

        currentFrame.unmap();
    }
}

void VideoFrameSurface::stop()
{
    currentFrame = QVideoFrame();
    targetRect = QRect();

    QAbstractVideoSurface::stop();

    widget->update();
}
