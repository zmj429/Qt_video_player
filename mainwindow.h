#ifndef MAINWINDOW_H              // 防止头文件重复包含的宏定义
#define MAINWINDOW_H              // 定义头文件保护宏

#include <QMainWindow>           // 包含QMainWindow类，作为主窗口基类
#include <QMediaPlayer>          // 包含QMediaPlayer类，用于媒体播放控制
#include <QVideoWidget>          // 包含QVideoWidget类，用于视频显示
#include <QAudioOutput>          // 包含QAudioOutput类，用于音频输出控制
#include <QStringList>           // 包含QStringList类，用于存储播放列表

QT_BEGIN_NAMESPACE               // Qt命名空间开始
namespace Ui { class MainWindow; } // 声明UI命名空间中的MainWindow类
QT_END_NAMESPACE                 // Qt命名空间结束

// 主窗口类，继承自QMainWindow
class MainWindow : public QMainWindow
{
    Q_OBJECT                     // Qt的宏，用于启用信号槽机制

public:
    MainWindow(QWidget *parent = nullptr); // 构造函数，parent为父窗口指针
    ~MainWindow();               // 析构函数

private slots:
    void on_btnOpen_clicked();   // 打开文件按钮点击槽函数
    void on_btnPlay_clicked();   // 播放按钮点击槽函数
    void on_btnPause_clicked();  // 暂停按钮点击槽函数
    void on_btnStop_clicked();   // 停止按钮点击槽函数
    void on_btnPrev_clicked();   // 上一首按钮点击槽函数
    void on_btnNext_clicked();   // 下一首按钮点击槽函数
    void on_sliderProgress_sliderMoved(int pos); // 进度条拖动槽函数
    void on_sliderVolume_sliderMoved(int vol);   // 音量滑块拖动槽函数
    void updateProgress(qint64 pos); // 更新播放进度槽函数
    void onMediaLoaded();        // 媒体加载完成槽函数
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status); // 媒体状态改变槽函数

private:
    void playNext();             // 播放下一首私有方法
    void playPrev();             // 播放上一首私有方法
    void updateVolumeLabel(int vol); // 更新音量显示标签私有方法
    bool isSupportedFormat(const QString& fileName); // 判断文件格式是否支持私有方法

    Ui::MainWindow *ui;          // UI对象指针，用于访问界面控件
    QMediaPlayer* m_player;      // 媒体播放器对象指针
    QAudioOutput* m_audioOut;    // 音频输出对象指针
    QStringList m_playlist;      // 播放列表，存储媒体文件路径
    int m_currentIndex;          // 当前播放索引
    float m_lastVolume;          // 当前音量值
};

#endif // MAINWINDOW_H           // 头文件保护宏结束