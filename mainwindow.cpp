#include "mainwindow.h"           // 包含主窗口头文件
#include "ui_mainwindow.h"        // 包含UI头文件，由Qt Designer生成
#include <QTime>                  // 包含QTime类，用于时间格式化
#include <QFileDialog>            // 包含QFileDialog类，用于文件选择对话框
#include <QFileInfo>              // 包含QFileInfo类，用于文件信息获取

// 主窗口构造函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) // 初始化父类并创建UI对象
{
    ui->setupUi(this);            // 初始化UI界面
    this->setWindowTitle("Qt音视频播放器"); // 设置窗口标题

    m_player = new QMediaPlayer(this); // 创建媒体播放器对象
    m_audioOut = new QAudioOutput(this); // 创建音频输出对象
    m_player->setAudioOutput(m_audioOut); // 将音频输出绑定到播放器
    m_player->setVideoOutput(ui->videoWidget); // 将视频输出绑定到视频显示控件

    m_currentIndex = -1;          // 初始化当前播放索引为-1，表示无播放
    m_lastVolume = 0.5f;          // 初始化上次音量为50%

    ui->sliderVolume->setValue(50); // 设置音量滑块初始值为50
    m_audioOut->setVolume(0.5f);   // 设置音频输出音量为50%
    updateVolumeLabel(50);        // 更新音量显示标签

    // 绑定播放进度变化信号到更新进度槽函数
    connect(m_player, &QMediaPlayer::positionChanged, this, &MainWindow::updateProgress);
    // 绑定媒体时长变化信号到媒体加载完成槽函数
    connect(m_player, &QMediaPlayer::durationChanged, this, &MainWindow::onMediaLoaded);
    // 绑定媒体状态变化信号到状态变化处理槽函数
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);
}

// 主窗口析构函数
MainWindow::~MainWindow()
{
    delete ui;                    // 删除UI对象，释放资源
}

// 打开文件按钮点击事件处理
void MainWindow::on_btnOpen_clicked()
{
    // 弹出文件选择对话框，支持多选媒体文件
    QStringList filePaths = QFileDialog::getOpenFileNames(this,
                                                          "选择音视频文件",
                                                          "./",
                                                          "媒体文件 (*.mp4 *.avi *.mov *.mkv *.flv *.wmv *.mp3 *.wav *.flac *.ogg *.aac)");

    if (!filePaths.isEmpty())     // 判断是否选择了文件
    {
        m_playlist = filePaths;   // 更新播放列表
        m_currentIndex = 0;       // 设置当前播放索引为第一个文件
        m_player->setSource(QUrl::fromLocalFile(m_playlist[m_currentIndex])); // 设置播放源
        m_player->play();         // 开始播放
        // 在状态栏显示当前播放文件名
        statusBar()->showMessage("正在播放: " + QFileInfo(m_playlist[m_currentIndex]).fileName());
    }
}

// 播放按钮点击事件处理
void MainWindow::on_btnPlay_clicked()
{
    // 判断是否有有效的播放列表索引
    if (m_currentIndex >= 0 && m_currentIndex < m_playlist.size())
    {
        m_player->play();         // 开始播放
    }
}

// 暂停按钮点击事件处理
void MainWindow::on_btnPause_clicked()
{
    m_player->pause();            // 暂停播放
}

// 停止按钮点击事件处理
void MainWindow::on_btnStop_clicked()
{
    m_player->stop();             // 停止播放
    ui->sliderProgress->setValue(0); // 重置进度条到起始位置
    ui->labelTime->setText("00:00 / 00:00"); // 重置时间显示
}

// 上一首按钮点击事件处理
void MainWindow::on_btnPrev_clicked()
{
    playPrev();                   // 调用播放上一首方法
}

// 下一首按钮点击事件处理
void MainWindow::on_btnNext_clicked()
{
    playNext();                   // 调用播放下一首方法
}

// 进度条拖动事件处理
void MainWindow::on_sliderProgress_sliderMoved(int pos)
{
    m_player->setPosition(pos);   // 设置播放位置到拖动的位置
}

// 音量滑块拖动事件处理
void MainWindow::on_sliderVolume_sliderMoved(int vol)
{
    float volRate = vol / 100.0f; // 将0-100的音量值转换为0.0-1.0的浮点数
    m_audioOut->setVolume(volRate); // 设置音频输出音量
    m_lastVolume = volRate;       // 保存当前音量值
    updateVolumeLabel(vol);       // 更新音量显示标签
}

// 更新播放进度槽函数
void MainWindow::updateProgress(qint64 pos)
{
    qint64 total = m_player->duration(); // 获取媒体总时长
    ui->sliderProgress->setRange(0, total); // 设置进度条范围
    ui->sliderProgress->setValue(pos);     // 设置进度条当前值

    // 格式化当前时间
    QTime currentTime(0, 0, 0);
    currentTime = currentTime.addMSecs(pos);
    // 格式化总时长
    QTime totalTime(0, 0, 0);
    totalTime = totalTime.addMSecs(total);
    // 更新时间显示标签
    ui->labelTime->setText(currentTime.toString("mm:ss") + " / " + totalTime.toString("mm:ss"));
}

// 媒体加载完成槽函数
void MainWindow::onMediaLoaded()
{
    ui->sliderProgress->setMaximum(m_player->duration()); // 设置进度条最大值为媒体时长
}

// 媒体状态变化槽函数
void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) // 判断是否播放结束
    {
        playNext();               // 自动播放下一首
    }
}

// 播放下一首私有方法
void MainWindow::playNext()
{
    if (m_playlist.isEmpty())     // 判断播放列表是否为空
        return;

    m_currentIndex++;             // 索引加1
    if (m_currentIndex >= m_playlist.size()) // 判断是否超出列表范围
        m_currentIndex = 0;       // 循环回到第一个

    m_player->setSource(QUrl::fromLocalFile(m_playlist[m_currentIndex])); // 设置新的播放源
    m_player->play();             // 开始播放
    // 在状态栏显示当前播放文件名
    statusBar()->showMessage("正在播放: " + QFileInfo(m_playlist[m_currentIndex]).fileName());
}

// 播放上一首私有方法
void MainWindow::playPrev()
{
    if (m_playlist.isEmpty())     // 判断播放列表是否为空
        return;

    m_currentIndex--;             // 索引减1
    if (m_currentIndex < 0)       // 判断是否小于0
        m_currentIndex = m_playlist.size() - 1; // 循环到最后一个

    m_player->setSource(QUrl::fromLocalFile(m_playlist[m_currentIndex])); // 设置新的播放源
    m_player->play();             // 开始播放
    // 在状态栏显示当前播放文件名
    statusBar()->showMessage("正在播放: " + QFileInfo(m_playlist[m_currentIndex]).fileName());
}

// 更新音量标签私有方法
void MainWindow::updateVolumeLabel(int vol)
{
    ui->labelVolume->setText(QString::number(vol) + "%"); // 设置音量百分比显示
}

// 判断文件格式是否支持私有方法
bool MainWindow::isSupportedFormat(const QString& fileName)
{
    QString ext = QFileInfo(fileName).suffix().toLower(); // 获取文件扩展名并转换为小写
    // 定义支持的媒体文件扩展名列表
    QStringList supportedExts = {"mp4", "avi", "mov", "mkv", "flv", "wmv", "mp3", "wav", "flac", "ogg", "aac"};
    return supportedExts.contains(ext); // 判断扩展名是否在支持列表中
}