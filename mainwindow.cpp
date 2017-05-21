#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)

{
    //this->setWindowFlags(Qt :: Window | Qt::FramelessWindowHint);
    //this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setMinimumSize(1280,720);
    videoManager = new VideoManager(500);

    QString url = QFileInfo("H:/test.mp4").absoluteFilePath();
    videoManager->changeVideo(QUrl::fromLocalFile(url));

    VideoWidget *outputVideoWidget = new VideoWidget();

    QBoxLayout *controlLayout = new QVBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(outputVideoWidget);

    QWidget *window = new QWidget();
    window->setLayout(controlLayout);
    setCentralWidget(window);
    //this->layout()->setContentsMargins(0,0,0,0);
    //this->layout()->addWidget(controlLayout);

    //video->setOutput(output);

    VideoFrameSurface* videoSurface = outputVideoWidget->getVideoSurface();

    videoManager->setOutput(videoSurface);
    outputVideoWidget->show();

    imageViewer = new QLabel();
    controlLayout->addWidget(imageViewer);
    imageViewer->setAlignment(Qt::AlignCenter);
    imageViewer->show();
    connect(videoManager,SIGNAL(imageIsReady(QImage)),this,SLOT(setImage(QImage)));

    videoManager->startTimer();

}

MainWindow::~MainWindow()
{

}

void MainWindow::play()
{
    // should print 1: NoMedia but is 0: UnknownMediaStatus
    videoManager->play();





}

void MainWindow::setImage(QImage value)
{
    QSize size =value.size();
    imageViewer->setPixmap(QPixmap::fromImage(value));
    if (imageViewer->size()!= size)
        imageViewer->resize(size);

}
