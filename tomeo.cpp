/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include "the_player.h"
#include "the_button.h"
#include <QScrollBar>
#include <QScrollArea>
#include <QAbstractScrollArea>
#include <QSize>
#include "the_controls.h"
#include "the_videowidget.h"
#include "the_mainwindow.h"

using namespace std;

// read in videos and thumbnails to this directory
vector<TheButtonInfo> getInfoIn (string loc) {

    vector<TheButtonInfo> out =  vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }

    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::question(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                    QMessageBox::Yes |
                    QMessageBox::No );

        switch( result )
        {
        case QMessageBox::Yes:
          QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
          break;
        default:
            break;
        }
        exit(-1);
    }

    // the widget that will show the video
    QVideoWidget *videoWidget = new VideoWidget;


    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer();
    player->setVideoOutput(videoWidget);

    // a list of the buttons
    vector<TheButton*> buttons;

    // horizontal scroll bar area
    QScrollArea *sc = new QScrollArea();
    sc->setWidgetResizable(true);
    sc->horizontalScrollBarPolicy();
    sc->setBackgroundRole(QPalette::Dark);
    sc->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // hide scrollbar
    sc->verticalScrollBar()->setDisabled(true);              // disable vertical, we only want horizontal
    sc->setMinimumHeight(140);


    // container for layout of buttons
    QWidget *buttonscontainer = new QWidget();


    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonscontainer->setLayout(layout);
    buttonscontainer->setMinimumHeight(80);

    // scroll bar area becomes parent of buttonscontainer
    sc->setWidget(buttonscontainer);

    // create the four buttons
    for ( int i = 0; i < int(videos.size()); i++ ) {
        TheButton *button = new TheButton(buttonscontainer);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }

    // maximum size of the scrollArea is the same as for the layout
    sc->setMaximumHeight(layout->maximumSize().height());

    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    // create the main window and layout
    MainWindow* window=new MainWindow;
    QVBoxLayout *top = new QVBoxLayout();
    window->setLayout(top);
    window->setWindowTitle("tomeo");
    window->setMinimumSize(800, 680);
    window->setStyleSheet("background-color: black");



    // controls of video
    Controls *controls = new Controls();
    controls->setMaximumHeight(40);
    controls->setState(player->state());
    controls->connect(controls, SIGNAL(play()), player, SLOT(play()));
    controls->connect(controls, SIGNAL(pause()), player, SLOT(pause()));
    controls->connect(controls, SIGNAL(stop()), player, SLOT(stop()));
    controls->connect(controls, SIGNAL(stop()), videoWidget, SLOT(update()));
    controls->setStyleSheet("background-color: black");



    player->connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), controls, SLOT(setState(QMediaPlayer::State)));

    // sound control
    controls->muteButton->setMute(true);
    controls->slider->connect(controls->slider, SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));
    controls->slider->connect(controls->slider, SIGNAL(valueChanged(int)), controls->muteButton, SLOT (changeIcon(int)));

    controls->muteButton->connect(controls->muteButton, SIGNAL(mute(bool)), player, SLOT(setMuted(bool)));
    controls->muteButton->connect(controls->muteButton, SIGNAL(moveSlider(int)), controls->slider, SLOT (moveSlider(int)));

    //progress bar
    //allows the user to navigate through the video
    controls->connect(controls->progressBar, &QSlider::sliderMoved, player, &ThePlayer::seek);

    //determines the length of the video
    controls->connect(player, &QMediaPlayer::durationChanged, controls, &Controls::newLength);
    //allows the progress slider to reflect the video's position
    controls->connect(player, &QMediaPlayer::positionChanged, controls, &Controls::progressChange);

    controls->connect(controls->fullScreenButton, &QPushButton::clicked, videoWidget, &QVideoWidget::setFullScreen);
    controls->connect(videoWidget, &QVideoWidget::fullScreenChanged, controls->fullScreenButton, &QPushButton::setChecked);

    // theme control
    controls->connect(controls->themeButton, &QPushButton::clicked, controls, &Controls::changeTheme);
    controls->connect(controls->themeButton, &QPushButton::clicked, window, &MainWindow::changeTheme);

    // add the video and the buttons to the top level widget

    top->addWidget(videoWidget);
    top->addWidget(controls);
    top->addWidget(sc);

    // showtime!

    window->show();

    // wait for the app to terminate
    return app.exec();
}


