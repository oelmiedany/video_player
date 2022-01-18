#ifndef THE_VIDEOWIDGET_H
#define THE_VIDEOWIDGET_H


#include <QVideoWidget>

class VideoWidget : public QVideoWidget
{
    Q_OBJECT

public:
    explicit VideoWidget(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};


#endif // THE_VIDEOWIDGET_H
