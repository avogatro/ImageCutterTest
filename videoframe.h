#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H

#include <QVideoFrame>
#include <QImage>
#include <QGraphicsVideoItem>
///\class VideoFrame
/// \brief Buffer for multiple crop of the same videoframe.
///
class VideoFrame : public QObject
{
public:
    VideoFrame(const QVideoFrame &videoFrame);
    VideoFrame(const VideoFrame &videoFrame);
    ///
    /// \brief crop Videoframe at giving position and size
    /// \param x X
    /// \param y Y
    /// \param width width
    /// \param height height
    /// \return
    ///
    QImage crop(const int x,const int y, const int width,const int height) const;
    QImage getImage() const;

    ~VideoFrame();
    QVideoFrame getMyQVideoFrame() const;
    void setMyQVideoFrame(const QVideoFrame &value);

private:
    QVideoFrame myQVideoFrame;
    QImage myQImage;
    void generateImage();
};

#endif // VIDEOFRAME_H
