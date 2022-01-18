#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QWidget>

class MainWindow: public QWidget
{
    Q_OBJECT
public:
    MainWindow();

public slots:

    void changeTheme();
private:

    bool themeValue;
};

#endif // MAINWINDOW_H
