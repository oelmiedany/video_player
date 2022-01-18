#ifndef THE_CONTROLS_H
#define THE_CONTROLS_H

#include <QMediaPlayer>
#include <QWidget>
#include <QAbstractButton>
#include <QProgressBar>
#include <QLabel>
#include "sound.h"



class Controls: public QWidget
{
    Q_OBJECT

public:
    Controls(QWidget *parent = 0);
    QMediaPlayer::State state() const;
    SoundButton *muteButton;
    SoundSlider *slider;
    QSlider *progressBar;
    QLabel *timerLabel;
    QPushButton *fullScreenButton = nullptr;
    QPushButton *themeButton = nullptr;

signals:
    void play();
    void pause();
    void stop();
    void position();
    void seek ();
    void fullScreenChanged(bool fullScreen);
    void theme();


public slots:
    void setState(QMediaPlayer::State state);
    void newLength(qint64 change);
    void progressChange (qint64 progress);
    void changeTheme ();

private slots:
    void playClicked();
    void progress();



private:
    QMediaPlayer::State playerState;
    QAbstractButton *playButton;
    QAbstractButton *stopButton;
    int vidLength;
    bool themeValue;

};


#endif // THE_CONTROLS_H
