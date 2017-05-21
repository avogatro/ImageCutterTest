#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "video.h"
#include "videoframesurface.h"
#include "videowidget.h"
#include <QFileInfo>
#include <QDebug>
#include <QBoxLayout>
#include <QLabel>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void play();
private:
    Ui::MainWindow *ui;
    VideoManager *videoManager;
    QLabel *imageViewer;
public slots:
    void setImage (QImage value);
};

#endif // MAINWINDOW_H
