#include "the_controls.h"//imports the widgets we'll be using

#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>


Controls::Controls(QWidget *parent):QWidget(parent), playerState(QMediaPlayer::StoppedState){

    themeValue = true;

    playButton = new QToolButton(this);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));


    stopButton = new QToolButton(this);
    stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    stopButton->setEnabled(false);

    connect(stopButton, SIGNAL(clicked()), this, SIGNAL(stop()));
    connect(playButton, SIGNAL(clicked()), this, SLOT(playClicked()));


    timerLabel = new QLabel(this);
    timerLabel->setText("0s/0s");
    timerLabel->setStyleSheet("color: white");

    muteButton = new SoundButton(this);
    muteButton->setMaximumWidth(50);
    slider = new SoundSlider(this);
    slider->setMaximumWidth(150);

    progressBar=new QSlider(Qt::Horizontal, this);
    progressBar->setTracking(true);
    progressBar->setRange(0,1000);

    fullScreenButton = new QPushButton(tr("Full screen"), this);
    fullScreenButton->setCheckable(true);
    fullScreenButton->setStyleSheet("background: lightpink");

    themeButton = new QPushButton(tr("Light Theme"), this);
    themeButton->setStyleSheet("QPushButton { background-color : black; color : white; }");


    QBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(playButton);
    layout->addWidget(stopButton);
    layout->addWidget(progressBar);
    layout->addWidget(timerLabel);
    layout->addWidget(muteButton);
    layout->addWidget(slider);
    layout->addWidget(fullScreenButton);
    layout->addWidget(themeButton);



    playButton->setStyleSheet("background-color: gray");
    stopButton->setStyleSheet("background-color: gray");

    setLayout(layout);

}

QMediaPlayer::State Controls::state() const
{
    return playerState;
}


void Controls::setState(QMediaPlayer::State ms)
{
    if (ms != playerState) {
        playerState = ms;

        switch (ms) {
        case QMediaPlayer::StoppedState:
            stopButton->setEnabled(false);
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case QMediaPlayer::PlayingState:
            stopButton->setEnabled(true);
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case QMediaPlayer::PausedState:
            stopButton->setEnabled(true);
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
}

void Controls::playClicked()
{
    switch (playerState) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        emit play();
        break;
    case QMediaPlayer::PlayingState:
        emit pause();
        break;
    }
}

void Controls::progress()
{
    emit position();
}

void Controls::newLength(qint64 change)//sets the seek bar's length according to the video's length
{
    vidLength=change/1000;
    progressBar->setMaximum(vidLength);
    timerLabel->setText("0s/"+QString::number(vidLength)+"s");

}

void Controls::progressChange (qint64 progress){//keeps track of the video's position and moves the slider accordingly

    progressBar->setValue(progress/1000);
    timerLabel->setText(QString::number(progress/1000)+"s/"+QString::number(vidLength)+"s");


}

void Controls::changeTheme()
{
    themeValue = !themeValue;
    if(themeValue){
        this->setStyleSheet("background-color: black");
        themeButton->setStyleSheet("QPushButton { background-color : black; color : white; }");
        themeButton->setText("Light Theme");
        timerLabel->setStyleSheet("QLabel { background-color : black; color : white; }");
    }
    else{
        this->setStyleSheet("background-color: white");
        themeButton->setStyleSheet("QPushButton { background-color : white; color : black; }");
        themeButton->setText("Dark Theme");
        timerLabel->setStyleSheet("QLabel { background-color : white; color : black; }");
    }
}



