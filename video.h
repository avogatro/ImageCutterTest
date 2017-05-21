#ifndef VIDEO_H
#define VIDEO_H

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QDebug>
#include <QTimer>
#include "videoframe.h"
#include "videoframesurface.h"
class VideoManager : public QObject
{
    Q_OBJECT

private:
    QMediaPlayer* myQMediaPlayer;
    QMediaPlaylist* myQMediaPlaylist;
    //QVideoFrame *myQVideoFrame;
    quint64 interval;
    QImage image;
    //VideoFrame* myVideoFrame;
    QTimer * timer;
public:
    ///
    /// \brief Video Constructor
    /// \param interval interval between cutting screenshots
    ///
    VideoManager(quint64 interval = 500);
    ~VideoManager();
    ///
    /// \brief changeMedia changes played media
    /// \param url
    ///
    void changeVideo(QUrl url);
    ///
    /// \brief addpictures TODO add folder of pictures
    /// \param pictureList
    ///
    void addpictures(QList<QUrl> pictureList);
    ///
    /// \brief setOutput set the only output for the QMediaplayer
    /// \param output the surface where screenshots are made
    ///
    void setOutput(VideoFrameSurface *output);
    ///
    /// \brief play play media
    ///
    void play();
    ///
    /// \brief stop stop media
    ///
    void stop();
    ///
    /// \brief pause pause media
    ///
    void pause();
    ///
    /// \brief forward jump forward to next interval
    ///
    void forward();
    ///
    /// \brief backward jump backward to the last interval
    ///
    void backward();
    ///
    /// \brief setPosition jump to position
    /// \param position
    ///
    void setPosition(qint64 position);
    ///
    /// \brief getScreenshot
    /// \return
    ///
    QImage getScreenshot();
    ///
    /// \brief getInterval
    /// \return interval between each screenshot taken
    ///
    quint64 getInterval() const;
    ///
    /// \brief setInterval interval between each screenshot taken
    /// \param interval
    ///
    void setInterval(const quint64 &interval);
    ///
    /// \brief crop crop current frame with given data
    /// \param x x
    /// \param y y
    /// \param width width
    /// \param height height
    /// \return croped Image
    ///
    QImage crop(const int x, const int y, const int width, const int height);
    void startTimer();
public slots:
    void setImage(QImage image);
    void doTimerEvent();
signals:
    void imageIsReady(QImage image);
    void requestImage();
};

#endif // VIDEO_H
