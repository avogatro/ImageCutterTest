#include "videoframe.h"


VideoFrame::VideoFrame(const QVideoFrame &qVideoFrame) {
    this->myQVideoFrame = QVideoFrame (qVideoFrame);
    this->generateImage();
}

VideoFrame::VideoFrame(const VideoFrame &videoFrame):VideoFrame(videoFrame.myQVideoFrame){
}

QImage VideoFrame::crop(const int x,const int y, const int width,const int height) const{
    if (x<0
            || y<0
            || x+width > myQVideoFrame.width()
            || y+height > myQVideoFrame.height()
            ){
        QString text = QString().sprintf("crop box out of range: X %d Y %d Width %d Height %d", x,y,width,height);
        throw std::out_of_range (text.toUtf8().constData());

    }



    return myQImage.copy(x,y,width,height);

}

QImage VideoFrame::getImage() const
{
    return QImage(myQImage);
}

void VideoFrame::generateImage(){
    if (this->myQVideoFrame.map(QAbstractVideoBuffer::ReadOnly)) {
        this->myQImage = QImage(this->myQVideoFrame.bits(),
                                this->myQVideoFrame.width(),
                                this->myQVideoFrame.height(),
                                this->myQVideoFrame.bytesPerLine(),
                                QVideoFrame::imageFormatFromPixelFormat(this->myQVideoFrame.pixelFormat())
                                );
        this->myQVideoFrame.unmap();
    }
}

VideoFrame::~VideoFrame(){

}

QVideoFrame VideoFrame::getMyQVideoFrame() const
{
    return myQVideoFrame;
}

void VideoFrame::setMyQVideoFrame(const QVideoFrame &value)
{
    myQVideoFrame = value;
    this->generateImage();
}



