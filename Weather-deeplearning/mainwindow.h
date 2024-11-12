#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QContextMenuEvent>
#include <QMainWindow>
#include <QMenu>
#include <QCursor>
#include <QAction>

#include <QMouseEvent>
#include <QLabel>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <weatherdata.h>

#include <weatherai.h>
#include <weatheraipython.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

    //重写父类虚函数
    void contextMenuEvent( QContextMenuEvent* event );
    void mousePressEvent( QMouseEvent* event );
    void mouseMoveEvent( QMouseEvent* event );
    bool eventFilter( QObject* watched, QEvent* event );

    void getWeatherInfo( QString cityCode ); // 获取天气数据

    void parseJson( QByteArray& byteArray); // 解析Json数据

    void updateUI(); // 更新UI

    // 绘制高低温曲线
    void paintHighCurve();
    void paintLowCurve();

private slots:
    void onReplied( QNetworkReply* reply);

    void on_search_clicked();

    void on_closeButton_clicked();

    void on_actionWeatherAI_triggered();

    void on_actionWeatherAI_Python_triggered();

private:
    Ui::MainWindow *ui;

    QMenu* mExitMenu; // 右键退出菜单
    QAction* mExitAct; // 退出的行为

    QPoint mOffset; // 窗口随鼠标移动

    QNetworkAccessManager* mNetAccessManager; // Http请求

    // 需要解析的数据
    Today mToday;
    Day mDay[6];

    // 星期和日期数组
    QList<QLabel*> mWeekList;
    QList<QLabel*> mDateList;

    // 天气和图标数组
    QList<QLabel*> mTypeList;
    QList<QLabel*> mTypeIconList;

    // 污染指数数组
    QList<QLabel*> mAqiList;

    // 风向和风力数组
    QList<QLabel*> mFxList;
    QList<QLabel*> mFlList;

    QMap<QString, QString> mTypeMap;

};
#endif // MAINWINDOW_H
