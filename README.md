# Qt音视频播放器

基于Qt6框架开发的跨平台音视频播放器，支持多种媒体格式播放，具备完整的播放控制功能。

## 技术栈

- Qt 6.x
- QMediaPlayer
- QVideoWidget
- QAudioOutput
- C++17

## 功能特性

### 播放控制
- 播放、暂停、停止
- 上一首/下一首切换
- 播放列表循环播放

### 进度控制
- 进度条拖动跳转
- 实时显示当前时间/总时长

### 音量控制
- 音量滑块调节
- 音量百分比显示

### 文件管理
- 支持多选打开媒体文件
- 支持格式：mp4、avi、mov、mkv、flv、wmv、mp3、wav、flac、ogg、aac

### 菜单操作
- 文件菜单：打开文件(Ctrl+O)、退出(Ctrl+Q)
- 播放菜单：播放(Space)、暂停、停止、上一首、下一首

## 项目结构

```
video_player/
├── main.cpp              # 程序入口
├── mainwindow.h          # 主窗口头文件
├── mainwindow.cpp        # 主窗口实现
├── mainwindow.ui         # UI界面文件
├── video_player.pro      # Qt项目配置文件
└── README.md             # 项目说明文档
```

## 编译运行

### 环境要求
- Qt 6.0+
- C++17编译器
- Qt Multimedia模块

### 编译步骤

1. 使用Qt Creator打开项目文件 `video_player.pro`
2. 配置构建套件（建议使用Qt自带的MinGW或MSVC工具链）
3. 点击构建按钮编译项目
4. 点击运行按钮启动程序

### 命令行编译

```bash
# 创建构建目录
mkdir build && cd build

# 生成Makefile
qmake ..\video_player.pro

# 编译
make -j4

# 运行
./video_player
```

## 使用说明

1. 点击「打开」按钮选择要播放的媒体文件
2. 使用播放控制按钮进行播放、暂停、停止操作
3. 拖动进度条可跳转到指定位置
4. 使用音量滑块调节音量大小
5. 使用上一首/下一首按钮切换播放文件

## 核心代码说明

### 媒体播放核心

```cpp
// 创建媒体播放器和音频输出
m_player = new QMediaPlayer(this);
m_audioOut = new QAudioOutput(this);
m_player->setAudioOutput(m_audioOut);
m_player->setVideoOutput(ui->videoWidget);

// 信号槽绑定
connect(m_player, &QMediaPlayer::positionChanged, this, &MainWindow::updateProgress);
connect(m_player, &QMediaPlayer::durationChanged, this, &MainWindow::onMediaLoaded);
connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);
```

### 播放列表管理

播放列表使用 `QStringList` 存储，支持多文件选择和循环播放。播放结束后自动切换到下一首。

## 许可证

MIT License