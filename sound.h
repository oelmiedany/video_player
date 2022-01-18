#ifndef SOUND_H
#define SOUND_H


#include <QPushButton>
#include <QUrl>
#include <QStyle>
#include <QAudio>
#include <QWidget>
#include <QSlider>
#include <iostream>

class SoundButton : public QPushButton {
    Q_OBJECT

public:
     SoundButton(QWidget *parent) :  QPushButton(parent) {
         setIconSize(QSize(60,60));
         setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
         connect(this, SIGNAL(released()), this, SLOT (clicked()));
     }

    void setMute(bool flipMute);

private slots:
    void clicked();
    void changeIcon(int volVal);

signals:
    void mute(bool muted);
    void moveSlider(int volume);

private:
    bool muteValue;
};




class SoundSlider : public QSlider {
    Q_OBJECT
public:
    SoundSlider(QWidget *parent) :  QSlider(Qt::Horizontal, parent) {
        // set the range and tracking of the slider
        setTracking(true);
        setRange(0,100);
   }

private slots:
    //Sets the position of the slider when it gets unmuted
    void moveSlider(int volume);
};

#endif // SOUND_H
