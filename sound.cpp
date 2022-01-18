#include "sound.h"

void SoundButton::setMute(bool flipMute)
{
    muteValue = !flipMute;
}

void SoundButton::clicked(){

    if (muteValue){
        setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        emit moveSlider(0);
        emit mute(1);
    }
    else{
        setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        emit(moveSlider(15));
        emit mute(0);
    }
}

void SoundButton::changeIcon(int volumeValue)
{
    if (volumeValue == 0  && muteValue == 1)
    {
        setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        muteValue = 0;
        emit mute(1);
    }

    if (volumeValue != 0)
    {
        setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        muteValue = 1;
        emit mute(0);
    }
}

void SoundSlider::moveSlider(int volume)
{
    setSliderPosition(volume);
}
