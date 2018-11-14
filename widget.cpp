#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->widget_playBackMode->hide();

    //设置窗口透明度、框架
    setWindowOpacity(0.95);     // 1是完全不透明
    setWindowFlags(Qt::FramelessWindowHint);

    //给是否移动的标志初始化为false.
    m_move = false;

    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);

    connect(ui->button_play, SIGNAL(clicked(bool)), this, SLOT(on_button_play_toggled(bool)));
    connect(ui->button_stop, SIGNAL(clicked(bool)), this, SLOT(on_button_stop_toggled(bool)));
    connect(ui->button_next, SIGNAL(clicked(bool)), this, SLOT(on_button_next_toggled(bool)));
    connect(ui->button_last, SIGNAL(clicked(bool)), this, SLOT(on_button_last_toggled(bool)));
    connect(ui->button_addMusic, SIGNAL(clicked(bool)), this, SLOT(on_button_addMusic_toggled(bool)));
    connect(player, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(on_label_musicInfoUpdate()));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(updatePosition(qint64)));
    connect(ui->slider_music, SIGNAL(sliderMoved(int)), this, SLOT(setPosition(int)));
    connect(ui->button_close, SIGNAL(clicked(bool)), this, SLOT(on_button_close_toggled(bool)));
    connect(ui->button_playBackMode, SIGNAL(clicked(bool)), this, SLOT(on_button_playBackMode_toggled(bool)));
    connect(ui->button_currentItemInLoop, SIGNAL(clicked(bool)), this, SLOT(on_button_currentItemInLoop_clicked()));
    connect(ui->button_loop, SIGNAL(clicked(bool)), this, SLOT(on_button_loop_clicked()));
    connect(ui->button_random, SIGNAL(clicked(bool)), this, SLOT(on_button_random_clicked()));
    connect(ui->button_sequential, SIGNAL(clicked(bool)), this, SLOT(on_button_sequential_clicked()));

    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player ->setPlaylist(playlist);
    player->setVolume(50);

    // 显示播放时间的标签
    ui->label_time->setToolTip(tr("当前时间 / 总时间"));
    ui->label_time->setAlignment(Qt::AlignCenter);
    ui->label_time->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->slider_volume->setSliderPosition(50);

    //label们的字体大小
    QFont font("MUSIC",24);
    ui->label_musictitle->setFont(font);

    //建立系统托盘
    createSystemTrayIcon();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::createSystemTrayIcon()
{
    QSystemTrayIcon *system_tray = new QSystemTrayIcon();
    //放在托盘提示信息、托盘图标
    system_tray ->setToolTip(QString("MusicPlayer"));
    system_tray ->setIcon(QIcon(":/img/tray"));
    connect(system_tray , SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
    system_tray->show();
    //托盘显示提示信息
    system_tray->showMessage(QString("托盘标题"), QString("托盘显示内容"));
}

void Widget::iconIsActived(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    //点击托盘显示窗口
    case QSystemTrayIcon::Trigger:
    {
        showNormal();
        break;
    }
    //双击托盘显示窗口
    case QSystemTrayIcon::DoubleClick:
    {
        showNormal();
        break;
    }
    default:
    break;
    }
}

void Widget::on_button_play_toggled(bool checked)
{
    musictitle = player->metaData(QMediaMetaData::Title);
    ui->label_musictitle->setText(musictitle.toString());

    if(player->state() == QMediaPlayer::StoppedState || player->state() == QMediaPlayer::PausedState){
        player -> play();
        ui->button_play->setStyleSheet("QPushButton{background-image: url(:/img/pause.png);"
                                                   "background-color:transparent;}"
                                                  " QPushButton:hover{background-image: url(:/img/pause_ho.png);"
                                                   "background-color:transparent;}");
        qDebug() << player->state();
    }else{
        player->pause();
        ui->button_play->setStyleSheet("QPushButton{background-image: url(:/img/play.png);"
                                                   "background-color:transparent;}"
                                                  " QPushButton:hover{background-image: url(:/img/play_ho.png);"
                                                   "background-color:transparent;}");
        qDebug() << player->state();
    }
}

void Widget::on_button_stop_toggled(bool checked)
{
    player->stop();     //没有加对player状态的验证，但可以正常运行
    ui->button_play->setStyleSheet("QPushButton{background-image: url(:/img/play.png);"
                                               "background-color:transparent;}"
                                              " QPushButton:hover{background-image: url(:/img/play_ho.png);"
                                               "background-color:transparent;}");
}

void Widget::on_button_last_toggled(bool checked)
{
    player->stop();
    playlist->setCurrentIndex(playlist->previousIndex());
    qDebug() << player->mediaStatus();
    player->play();
    ui->button_play->setStyleSheet("QPushButton{background-image: url(:/img/pause.png);"
                                               "background-color:transparent;}"
                                              " QPushButton:hover{background-image: url(:/img/pause_ho.png);"
                                               "background-color:transparent;}");
}

void Widget::on_button_next_toggled(bool checked)
{
    player->stop();
    playlist->setCurrentIndex(playlist->nextIndex());
    qDebug() << player->mediaStatus();
    player->play();
    ui->button_play->setStyleSheet("QPushButton{background-image: url(:/img/pause.png);"
                                               "background-color:transparent;}"
                                              " QPushButton:hover{background-image: url(:/img/pause_ho.png);"
                                               "background-color:transparent;}");
}

void Widget::on_label_musicInfoUpdate()
{
    qDebug() << "musicInfoUpdate";
    musictitle = player->metaData(QMediaMetaData::Title);
    author = player->metaData(QMediaMetaData::Author);
    albumtitle = player->metaData(QMediaMetaData::AlbumTitle);

    QFont font_label;
    font_label.setBold(true);

    ui->label_musictitle->setText(musictitle.toString());
}

void Widget::updatePosition(qint64 posi)
{
    //时间的更新
    qint64 totalTimeValue = player->duration();
    qint64 time = player->position();
    QTime totalTime(0, (totalTimeValue / 60000) % 60, (totalTimeValue / 1000) % 60);
    QTime currentTime(0, (time / 60000) % 60, (time / 1000) % 60);
    QString str = currentTime.toString("mm:ss") + " / " + totalTime.toString("mm:ss");
    ui->label_time->setText(str);

    //进度条的更新
    ui->slider_music->setMaximum(totalTimeValue);
    ui->slider_music->setSliderPosition(time);
}

void Widget::on_button_addMusic_toggled(bool checked)
{
    const QStringList musicPaths = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
    const QString filePath =
    QFileDialog::getOpenFileName(this, tr("Open File"),
                                         musicPaths.isEmpty() ? QDir::homePath() : musicPaths.first(),
                                         tr("MP3 files (*.mp3);;All files (*.*)"));
    playlist->addMedia(QUrl::fromLocalFile(filePath));

    int first = filePath.lastIndexOf("/"); //从后面查找"/"位置
    QString title = filePath.right (filePath.length() - first - 1); //从右边截取
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(title);
    QFont list_font;        //设置listWidget中字体的大小，加粗。
    list_font.setPointSize(20);
    list_font.setBold(true);
    list_font.setWeight(50);
    ui->listWidget->setFont(list_font);
    ui->listWidget->addItem(item);
}

void Widget::setPosition(int t)
{
    player->setPosition(t);
}

void Widget::on_slider_volume_valueChanged(int n)
{
    this->player->setVolume(n);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    //当鼠标左键点击时.
    if(event->button() == Qt::LeftButton)
    {
        m_startPoint = event->globalPos() - frameGeometry().topLeft();
        m_move = true;
        event->accept();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_move)
    {
        move(event->globalPos() - m_startPoint);
        event->accept();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *)
{
    m_move = false;
}

void Widget::on_button_close_toggled(bool checked)
{
    this->close();
}

void Widget::on_button_playBackMode_toggled(bool checked)
{
    qDebug() << "test_button_playBackMode";
    if(sig == false){
        ui->widget_playBackMode->show();
        sig = true;
    }else{
        ui->widget_playBackMode->hide();
        sig = false;
    }
}

void Widget::on_button_currentItemInLoop_clicked()
{
    qDebug() << "CurrentItemOnce";
    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    ui->button_playBackMode->setStyleSheet("background-image: url(:/img/currentItemInLoop.png);"
                                           "background-color: transparent;");
}

void Widget::on_button_loop_clicked()
{
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    ui->button_playBackMode->setStyleSheet("background-image: url(:/img/loop.png);"
                                           "background-color: transparent;");
}

void Widget::on_button_random_clicked()
{
    //Random模式下按下一曲并不能随机播放
    playlist->setPlaybackMode(QMediaPlaylist::Random);
    ui->button_playBackMode->setStyleSheet("background-image: url(:/img/random.png);"
                                           "background-color: transparent;");
}

void Widget::on_button_sequential_clicked()
{
    qDebug() << "testSequential";
    playlist->setPlaybackMode(QMediaPlaylist::Sequential);
    ui->button_playBackMode->setStyleSheet("background-image: url(:/img/sequenti.png);"
                                           "background-color: transparent;");
}

void Widget::on_button_hide_clicked()
{
    this->hide();
}




