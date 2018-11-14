#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QApplication>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QDebug>
#include <QString>
#include <QMouseEvent>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QMediaPlayer* player;
    QMediaPlaylist* playlist;
    QMediaPlaylist* songslist;
    QVariant musictitle;
    QVariant author;
    QVariant albumtitle;
    QVariant musictitleForTable;
    QVariant authorForTable;
    QVariant albumtitleForTable;
    qint64 totalTimeValue;
    qint64 time;
    bool sig = false;

    
private slots:
    void on_button_play_toggled(bool checked);

    void on_button_stop_toggled(bool checked);

    void on_button_next_toggled(bool checked);

    void on_button_last_toggled(bool checked);

    void on_label_musicInfoUpdate();

    void updatePosition(qint64 posi);

    void on_button_addMusic_toggled(bool checked);

    void setPosition(int t);

    void on_slider_volume_valueChanged(int n);

    void on_button_close_toggled(bool checked);

    void on_button_playBackMode_toggled(bool checked);

    void on_button_currentItemInLoop_clicked();

    void on_button_loop_clicked();

    void on_button_random_clicked();

    void on_button_sequential_clicked();

    void iconIsActived(QSystemTrayIcon::ActivationReason reason);


    void on_button_hide_clicked();

//    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void createSystemTrayIcon();

private:
    Ui::Widget *ui;
    bool m_move;
        QPoint m_startPoint;
        QPoint m_windowPoint;

};

#endif // WIDGET_H
