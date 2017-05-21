#include "video.h"

quint64 VideoManager::getInterval() const
{
    return interval;
}

void VideoManager::setInterval(const quint64 &interval)
{
    if (interval>0)
        this->interval = interval;
    else
        throw std::out_of_range ("video interval <= 0");
}

//void VideoManager::setMyQVideoFrame(const QVideoFrame &value)
//{
//    if (myQVideoFrame != NULL){
//        *myQVideoFrame = value;
//        myVideoFrame->setMyQVideoFrame(*myQVideoFrame);
//    }
//    else{
//        myQVideoFrame = new QVideoFrame(value);
//        myVideoFrame = new VideoFrame(*myQVideoFrame);
//    }
//}

VideoManager::VideoManager(quint64 interval)
{
    myQMediaPlayer = new QMediaPlayer();
    myQMediaPlaylist = new QMediaPlaylist(myQMediaPlayer);
    myQMediaPlayer->setPlaylist(myQMediaPlaylist);
    setInterval(interval);

    myQMediaPlayer->setVolume(100);
    myQMediaPlayer->setMuted(false);
    timer = NULL;
}

VideoManager::~VideoManager()
{
    delete myQMediaPlayer;
    delete myQMediaPlaylist;

}

void VideoManager::changeVideo(QUrl url)
{
    stop();
    myQMediaPlaylist->clear();
    myQMediaPlaylist->addMedia(url);
    myQMediaPlaylist->setCurrentIndex(1);
}

void VideoManager::addpictures(QList<QUrl> pictures)
{


    stop();
    myQMediaPlaylist->clear();
    for (QUrl url : pictures){
        myQMediaPlaylist->addMedia(url);
    }
    myQMediaPlaylist->setCurrentIndex(1);
}



void VideoManager::setOutput(VideoFrameSurface *output)
{
    myQMediaPlayer->setVideoOutput(output);
    output->setInterval(interval);
    connect(output, SIGNAL(imageIsReady(QImage)),
                         this, SLOT(setImage(QImage)));

    connect(this, SIGNAL(requestImage()),
                         output, SLOT(createImage()));
}



void VideoManager::play()
{
    myQMediaPlayer->play();
    //qDebug() << " myQMediaPlaylist mediastatus "<<myQMediaPlaylist->mediaCount();
}
void VideoManager::stop()
{
    myQMediaPlayer->stop();
}

void VideoManager::setPosition(qint64 position)
{
    position = position/interval*interval;
    if (myQMediaPlayer->duration()<=0)
        std::out_of_range ("video duration <=0");
    else if(position>=0 && position<myQMediaPlayer->duration()){
        myQMediaPlayer->setPosition(position);
        emit requestImage();
    }else
        throw std::out_of_range ("video position out of range");
}

void VideoManager::pause()
{
    this->myQMediaPlayer->pause();
    setPosition( (this->myQMediaPlayer->position()/interval)
                 * interval);
}

void VideoManager::forward()
{
    this->myQMediaPlayer->pause();
    this->setPosition( this->myQMediaPlayer->position() + interval);
}

void VideoManager::backward()
{
    this->myQMediaPlayer->pause();
    this->setPosition( this->myQMediaPlayer->position() - interval);
}

QImage VideoManager::crop(const int x,const int y, const int width,const int height)
{
    this->pause();
    return this->image.copy(x,y,width,height);
}

void VideoManager::startTimer()
{
    if (timer != NULL){
        timer->stop();
        delete timer;
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(doTimerEvent()));
    timer->start(interval);
}

void VideoManager::setImage(QImage image)
{
    this->image = image;
    emit imageIsReady(this->image);
}

void VideoManager::doTimerEvent()
{
    this->forward();
}
