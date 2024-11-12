#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <weathertool.h>

#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QPainter>

#define INCREMENT 3 // 温度每升高或降低1度y轴坐标的增量
#define POINT_RADIUS 3 // 曲线描点的大小
#define TEXT_OFFSET_X 9  // 文本X轴偏移量
#define TEXT_OFFSET_Y 9  // 文本Y轴偏移量

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    // 1. 设置窗口属性
    setWindowFlag( Qt::FramelessWindowHint ); // 设置窗口无边框
    setFixedSize( width(), height() ); // 设置固定窗口大小

    // 2. 构建右键菜单，设置退出属性
    mExitMenu = new QMenu( this );
    mExitAct = new QAction();

    mExitAct -> setText( "退出" );
    mExitAct -> setIcon( QIcon( ":/new/prefix1/Figure/close.png" ) );

    mExitMenu -> addAction( mExitAct );

    connect( mExitAct , &QAction::triggered, this, [=]{ qApp -> exit( 0 ); }); // lambda函数实现退出

    // 将控件添加到控件数组中
    mWeekList << ui -> week0 << ui -> week1 << ui-> week2 << ui-> week3 << ui-> week4 << ui-> week5; // 星期数组
    mDateList << ui -> date0 << ui -> date1 << ui-> date2 << ui-> date3 << ui-> date4 << ui-> date5; // 日期数组
    mTypeList << ui -> type0 << ui -> type1 << ui-> type2 << ui-> type3 << ui-> type4 << ui-> type5; // 天气类型数组
    mTypeIconList << ui -> icon0 << ui -> icon1 << ui-> icon2 << ui-> icon3 << ui-> icon4 << ui-> icon5; // 天气图标数组
    mAqiList << ui -> aqi0 << ui -> aqi1 << ui-> aqi2 << ui-> aqi3 << ui-> aqi4 << ui-> aqi5; // 污染指数数组
    mFxList << ui -> fx0 << ui -> fx1 << ui-> fx2 << ui-> fx3 << ui-> fx4 << ui-> fx5; // 风向数组
    mFlList << ui -> fl0 << ui -> fl1 << ui-> fl2 << ui-> fl3 << ui-> fl4 << ui-> fl5; // 风力数组

    // 图标路径与天气类型对应
    mTypeMap.insert( "暴雪" , ":/new/prefix1/Figure/type/BaoXue.png" );
    mTypeMap.insert( "暴雨" , ":/new/prefix1/Figure/type/BaoYu.png" );
    mTypeMap.insert( "暴雨到大暴雨" , ":/new/prefix1/Figure/type/BaoYuDaoDaBaoYu.png" );
    mTypeMap.insert( "大暴雨" , ":/new/prefix1/Figure/type/DaBaoYu.png" );
    mTypeMap.insert( "大暴雨到特大暴雨" , ":/new/prefix1/Figure/type/DaBaoYuDaoTeDaBaoYu.png" );
    mTypeMap.insert( "大到暴雪" , ":/new/prefix1/Figure/type/DaDaoBaoXue.png" );
    mTypeMap.insert( "大到暴雨" , ":/new/prefix1/Figure/type/DaDaoBaoYu.png" );
    mTypeMap.insert( "大雪" , ":/new/prefix1/Figure/type/DaXue.png" );
    mTypeMap.insert( "大雨" , ":/new/prefix1/Figure/type/DaYu.png" );
    mTypeMap.insert( "冻雨" , ":/new/prefix1/Figure/type/DongYu.png" );
    mTypeMap.insert( "多云" , ":/new/prefix1/Figure/type/DuoYun.png" );
    mTypeMap.insert( "浮尘" , ":/new/prefix1/Figure/type/FuChen.png" );
    mTypeMap.insert( "雷阵雨" , ":/new/prefix1/Figure/type/LeiZhenYu.png" );
    mTypeMap.insert( "雷阵雨伴有冰雹" , ":/new/prefix1/Figure/type/LeiZhenYuBanYouBingBao.png" );
    mTypeMap.insert( "霾" , ":/new/prefix1/Figure/type/Mai.png" );
    mTypeMap.insert( "强沙尘暴" , ":/new/prefix1/Figure/type/QiangShaChenBao.png" );
    mTypeMap.insert( "晴" , ":/new/prefix1/Figure/type/Qing.png" );
    mTypeMap.insert( "沙尘暴" , ":/new/prefix1/Figure/type/ShaChenBao.png" );
    mTypeMap.insert( "特大暴雨" , ":/new/prefix1/Figure/type/TeDaBaoYu.png" );
    mTypeMap.insert( "无数据" , ":/new/prefix1/Figure/type/undefined.png" );
    mTypeMap.insert( "雾" , ":/new/prefix1/Figure/type/Wu.png" );
    mTypeMap.insert( "小到中雪" , ":/new/prefix1/Figure/type/XiaoDaoZhongXue.png" );
    mTypeMap.insert( "小到中雨" , ":/new/prefix1/Figure/type/XiaoDaoZhongYu.png" );
    mTypeMap.insert( "小雪" , ":/new/prefix1/Figure/type/XiaoXue.png" );
    mTypeMap.insert( "小雨" , ":/new/prefix1/Figure/type/XiaoYu.png" );
    mTypeMap.insert( "雪" , ":/new/prefix1/Figure/type/Xue.png" );
    mTypeMap.insert( "扬沙" , ":/new/prefix1/Figure/type/YangSha.png" );
    mTypeMap.insert( "阴" , ":/new/prefix1/Figure/type/Yin.png" );
    mTypeMap.insert( "雨" , ":/new/prefix1/Figure/type/Yu.png" );
    mTypeMap.insert( "雨夹雪" , ":/new/prefix1/Figure/type/YuJiaXue.png" );
    mTypeMap.insert( "阵雨" , ":/new/prefix1/Figure/type/ZhenXue.png" );
    mTypeMap.insert( "阵雪" , ":/new/prefix1/Figure/type/ZhenXue.png" );
    mTypeMap.insert( "中到大雪" , ":/new/prefix1/Figure/type/ZhongDaoDaXue.png" );
    mTypeMap.insert( "中到大雨" , ":/new/prefix1/Figure/type/ZhongDaoDaYu.png" );
    mTypeMap.insert( "中雪" , ":/new/prefix1/Figure/type/ZhongXue.png" );
    mTypeMap.insert( "中雨" , ":/new/prefix1/Figure/type/ZhongYu.png" );

    // 3. 网络请求，获取数据
    mNetAccessManager = new QNetworkAccessManager( this );
    connect( mNetAccessManager, &QNetworkAccessManager::finished, this, &MainWindow::onReplied ); //实现自动调用槽函数

    // 直接在构造中请求天气数据
    // “101010100” 北京城市天气数据编码
    // getWeatherInfo( "101010100" );
    getWeatherInfo( "西安" );

    // 4. 给标签添加事件处理器
    // 参数指定为this,也就是当前窗口对象MainWindow
    ui -> lblHighCurve -> installEventFilter( this );
    ui -> lblLowCurve -> installEventFilter( this );

}

MainWindow::~MainWindow()
{
    delete ui;
}

// 重写父类的虚函数
// 父类中默认实现的是忽略右键菜单事件
// 重写后可以处理右键菜单事件
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    // 弹出右键菜单
    mExitMenu -> exec( QCursor::pos() );

    event -> accept();
}

// 改写父类虚函数
// 实现窗口随鼠标移动
void MainWindow::mousePressEvent( QMouseEvent *event )
{
    mOffset = event -> globalPos() - this -> pos();
}

void MainWindow::mouseMoveEvent( QMouseEvent *event )
{
    this -> move( event -> globalPos() - mOffset );
}

// 重写父类函数
// 显示图像
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{

    if( watched == ui -> lblHighCurve && event -> type() == QEvent::Paint )
    {
        paintHighCurve();
    }

    if( watched == ui -> lblLowCurve && event -> type() == QEvent::Paint )
    {
        paintLowCurve();
    }

    return QWidget::eventFilter( watched, event );

}

// 获取天气数据
void MainWindow::getWeatherInfo( QString cityName )
{
    QString cityCode = WeatherTool::getCityCode( cityName );
    if( cityCode.isEmpty() )
    {
        QMessageBox::warning( this, "天气", "输入错误！", QMessageBox::Ok );
        return;
    }

    QUrl url( "http://t.weather.itboy.net/api/weather/city/" + cityCode );
    mNetAccessManager -> get( QNetworkRequest( url ) );
}

void MainWindow::onReplied( QNetworkReply *reply )
{

    // 相应状态为200表示成功，404表示失败
    //qDebug() << "onReply success";

    int status_code = reply -> attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();

    //qDebug() << "operation: " << reply -> operation();
    //qDebug() << "status code:  " << status_code;
    //qDebug() << "url: " << reply -> url();
    //qDebug() << "raw header: " << reply -> rawHeaderList();

    if( reply ->error() != QNetworkReply::NoError || status_code != 200 )
    {
        //qDebug() << reply -> errorString().toLatin1().data();
        QMessageBox::warning( this, "天气", "请求数据失败", QMessageBox::Ok );
    }
    else
    {
        QByteArray byteArray = reply -> readAll();
        //qDebug() << "read all: " << byteArray.data();
        parseJson( byteArray );
    }

    reply -> deleteLater(); // 释放内存，防止内存泄露

}

// 解析获取的天气数据
void MainWindow::parseJson(QByteArray &byteArray)
{

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson( byteArray, &err );
    if( err.error != QJsonParseError::NoError )
    {
        return;
    }

    QJsonObject rootObj = doc.object();
    //qDebug() << rootObj.value( "message" ).toString();

    // 1. 解析日期和城市
    mToday.date = rootObj.value( "date" ).toString();
    mToday.city = rootObj.value( "cityInfo" ).toObject().value( "city" ).toString();

    // 2. 解析昨天的数据
    QJsonObject objData = rootObj.value( "data" ).toObject();

    QJsonObject objYesterday = objData.value( "yesterday" ).toObject();

    mDay[0].week = objYesterday.value( "week" ).toString();
    mDay[0].date = objYesterday.value( "ymd" ).toString();

    mDay[0].type = objYesterday.value( "type" ).toString();

    QString temp_high, temp_low;
    temp_high = objYesterday.value( "high" ).toString().split(" ").at( 1 ); // 取到高温数据
    temp_high = temp_high.left( temp_high.length() -1 );
    mDay[0].high = temp_high.toInt();

    temp_low = objYesterday.value( "low" ).toString().split(" ").at( 1 ); // 取到低温数据
    temp_low = temp_low.left( temp_low.length() -1 );
    mDay[0].low = temp_low.toInt();

    // 风向和风力
    mDay[0].fx = objYesterday.value( "fx" ).toString();
    mDay[0].fl = objYesterday.value( "fl" ).toString();

    // 污染指数
    mDay[0].aqi = objYesterday.value( "aqi" ).toDouble();

    // 3. 解析forecast中5天的数据
    QJsonArray forecastArr = objData.value( "forecast" ).toArray();

    for(int i = 0; i < 5; i++ )
    {

        QJsonObject ObjForecast = forecastArr[i].toObject();

        // 日期
        mDay[ i + 1 ].week = ObjForecast.value( "week" ).toString();
        mDay[ i + 1 ].date = ObjForecast.value( "ymd" ).toString();

        // 天气类型
        mDay[ i + 1 ].type = ObjForecast.value( "type" ).toString();

        // 高温和低温
        QString temp_high, temp_low;
        temp_high = ObjForecast.value( "high" ).toString().split(" ").at( 1 ); // 取到高温数据
        temp_high = temp_high.left( temp_high.length() -1 );
        mDay[ i + 1 ].high = temp_high.toInt();

        temp_low = ObjForecast.value( "low" ).toString().split(" ").at( 1 ); // 取到低温数据
        temp_low = temp_low.left( temp_low.length() -1 );
        mDay[ i + 1 ].low = temp_low.toInt();

        // 风向和风力
        mDay[ i + 1 ].fx = ObjForecast.value( "fx" ).toString();
        mDay[ i + 1 ].fl = ObjForecast.value( "fl" ).toString();

        // 污染指数
        mDay[ i + 1 ].aqi = ObjForecast.value( "aqi" ).toDouble();

    }

    // 4. 解析今天的数据
    mToday.ganmao = objData.value( "ganmao" ).toString();
    mToday.wendu = objData.value( "wendu" ).toString().toDouble();
    mToday.shidu = objData.value( "shidu" ).toString();
    mToday.pm25 = objData.value( "pm25" ).toDouble();
    mToday.quality = objData.value( "quality" ).toString();

    // 5. forecast中第一个数组的元素是今天的数据
    mToday.type = mDay[1].type;
    mToday.fx = mDay[1].fx;
    mToday.fl = mDay[1].fl;
    mToday.high = mDay[1].high;
    mToday.low = mDay[1].low;

    // 6. 更新UI

    // 6.1 文本更新
    updateUI();

    // 6.2 曲线更新
    ui -> lblHighCurve -> update();
    ui -> lblLowCurve -> update();

}

void MainWindow::updateUI()
{

    // 1. 更新总日期和城市
    ui -> TotalDate -> setText( QDateTime::fromString( mToday.date, "yyyyMMdd" ).toString( "yyyy/MM/dd" ) + " " + mDay[1].week);
    ui -> city -> setText( mToday.city );

    // 2. 更新今天的数据
    ui -> TotalTypeIcon -> setPixmap( mTypeMap[ mToday.type ] );
    ui -> temperature -> setText( QString::number( mToday.wendu ) + "℃" );
    ui -> ganmao -> setText( "感冒指数： " + mToday.ganmao );
    ui -> PM25 -> setText( "PM2.5： " + QString::number( mToday.pm25) );
    ui -> TotalFxFl -> setText( mToday.fx + "： " + mToday.fl );
    ui -> Totalshidu -> setText( "湿度： " + mToday.shidu );
    ui -> TotalQuality -> setText( "空气质量： " + mToday.quality );
    ui -> typelowhigh -> setText( mToday.type + "  " + QString::number( mToday.low ) + "℃" + "~" + QString::number( mToday.high ) + "℃" );

    // 3. 更新六天的数据
    for( int i = 0; i < 6; i++ )
    {

        // 3.1 更新日期
        mWeekList[i] -> setText( "周" + mDay[i].week.right(1) );
        ui -> week0 -> setText( "昨天" );
        ui -> week1 -> setText( "今天" );
        ui -> week2 -> setText( "明天" );

        QStringList ymdList = mDay[i].date.split( "-" );
        mDateList[i] -> setText( ymdList[1] + "/" + ymdList[2] );

        // 3.2 更新天气类型
        mTypeList[i] -> setText( mDay[i].type );
        mTypeIconList[i] -> setPixmap( mTypeMap[ mDay[i].type ] );

        // 3.3 更新空气质量
        if( mDay[i].aqi >=0 && mDay[i].aqi <50 )
        {
            mAqiList[i] -> setText( "优" );
            mAqiList[i] -> setStyleSheet( "background-color: rgb( 121 , 184, 0 );" );
        }
        else if( mDay[i].aqi >=50 && mDay[i].aqi <100 )
        {
            mAqiList[i] -> setText( "良" );
            mAqiList[i] -> setStyleSheet( "background-color: rgb( 255 , 187, 23 );" );
        }
        else if( mDay[i].aqi >=100 && mDay[i].aqi <150 )
        {
            mAqiList[i] -> setText( "轻度" );
            mAqiList[i] -> setStyleSheet( "background-color: rgb( 255 , 87, 97 );" );
        }
        else if( mDay[i].aqi >=150 && mDay[i].aqi <200 )
        {
            mAqiList[i] -> setText( "中度" );
            mAqiList[i] -> setStyleSheet( "background-color: rgb( 235 , 17, 27 );" );
        }
        else if( mDay[i].aqi >=200 && mDay[i].aqi <250 )
        {
            mAqiList[i] -> setText( "重度" );
            mAqiList[i] -> setStyleSheet( "background-color: rgb( 170 , 0, 0 );" );
        }
        else
        {
            mAqiList[i] -> setText( "严重" );
            mAqiList[i] -> setStyleSheet( "background-color: rgb( 110 , 0, 0 );" );
        }

        // 3.4 更新风向和风力
        mFxList[i] -> setText( mDay[i].fx );
        mFlList[i] -> setText( mDay[i].fl );

    }
}

void MainWindow::paintHighCurve()
{

    QPainter painter( ui -> lblHighCurve );

    // 抗锯齿
    painter.setRenderHint( QPainter::Antialiasing , true );

    // 1. 获取x坐标
    int pointX[ 6 ] = { 0 };
    for( int i = 0; i < 6; i++ )
    {
        pointX[ i ] = mWeekList[ i ] -> pos().x() + mWeekList[ i ] -> width() / 2;
    }

    // 2. 获取y坐标
    int tempSum = 0;
    int tempAverage = 0;
    for( int i = 0; i < 6; i++ )
    {
        tempSum += mDay[i].high;
    }
    tempAverage = tempSum / 6; // 最高温的平均值

    // 计算y坐标
    int pointY[ 6 ] = { 0 };
    int yCenter = ui -> lblHighCurve -> height() / 2;
    for( int i = 0; i < 6; i++ )
    {
        pointY[ i ] = yCenter - ( mDay[ i ].high - tempAverage ) * INCREMENT;
    }

    // 3. 绘图
    // 3.1 初始化画笔
    QPen pen = painter.pen();
    pen.setWidth( 1 ); // 设置画笔大小
    pen.setColor( QColor( 255, 17, 27 ) ); // 设置画笔颜色

    painter.setPen( pen );
    painter.setBrush( QColor( 255, 17, 27 ) ); // 设置画刷内部填充的颜色

    // 3.2 画点和写文本
    for( int i = 0; i < 6; i++ )
    {
        // 显示点
        painter.drawEllipse( QPoint( pointX[ i ], pointY[ i ] ), POINT_RADIUS, POINT_RADIUS );

        // 显示温度文本
        painter.drawText( pointX[ i ] - TEXT_OFFSET_X, pointY[ i ] - TEXT_OFFSET_Y, QString::number( mDay[i].high ) + "°" );
    }

    // 3.3 绘制曲线
    for( int i = 0; i < 5; i++ )
    {
        if( i == 0 )
        {
            pen.setStyle( Qt::DotLine ); //虚线
            painter.setPen( pen );
        }
        else
        {
            pen.setStyle( Qt::SolidLine ); //虚线
            painter.setPen( pen );
        }
        painter.drawLine( pointX[ i ], pointY[ i ] , pointX[ i + 1 ], pointY[ i + 1 ] );
    }

}

void MainWindow::paintLowCurve()
{

    QPainter painter( ui -> lblLowCurve );

    // 抗锯齿
    painter.setRenderHint( QPainter::Antialiasing , true );

    // 1. 获取x坐标
    int pointX[ 6 ] = { 0 };
    for( int i = 0; i < 6; i++ )
    {
        pointX[ i ] = mWeekList[ i ] -> pos().x() + mWeekList[ i ] -> width() / 2;
    }

    // 2. 获取y坐标
    int tempSum = 0;
    int tempAverage = 0;
    for( int i = 0; i < 6; i++ )
    {
        tempSum += mDay[i].low;
    }
    tempAverage = tempSum / 6; // 最高温的平均值

    // 计算y坐标
    int pointY[ 6 ] = { 0 };
    int yCenter = ui -> lblLowCurve -> height() / 2;
    for( int i = 0; i < 6; i++ )
    {
        pointY[ i ] = yCenter - ( mDay[ i ].low - tempAverage ) * INCREMENT;
    }

    // 3. 绘图
    // 3.1 初始化画笔
    QPen pen = painter.pen();
    pen.setWidth( 1 ); // 设置画笔大小
    pen.setColor( QColor( 0, 255, 255 ) ); // 设置画笔颜色

    painter.setPen( pen );
    painter.setBrush( QColor( 0, 255, 255 ) ); // 设置画刷内部填充的颜色

    // 3.2 画点和写文本
    for( int i = 0; i < 6; i++ )
    {
        // 显示点
        painter.drawEllipse( QPoint( pointX[ i ], pointY[ i ] ), POINT_RADIUS, POINT_RADIUS );

        // 显示温度文本
        painter.drawText( pointX[ i ] - TEXT_OFFSET_X, pointY[ i ] - TEXT_OFFSET_Y, QString::number( mDay[i].low ) + "°" );
    }

    // 3.3 绘制曲线
    for( int i = 0; i < 5; i++ )
    {
        if( i == 0 )
        {
            pen.setStyle( Qt::DotLine ); //虚线
            painter.setPen( pen );
        }
        else
        {
            pen.setStyle( Qt::SolidLine ); //虚线
            painter.setPen( pen );
        }
        painter.drawLine( pointX[ i ], pointY[ i ] , pointX[ i + 1 ], pointY[ i + 1 ] );
    }
}

// 搜索按键
void MainWindow::on_search_clicked()
{
    QString cityName = ui -> searchcity -> text();
    getWeatherInfo( cityName );
}

void MainWindow::on_closeButton_clicked()
{
    this -> close();
}

void MainWindow::on_actionWeatherAI_triggered()
{
    WeatherAI WeatherAi;
    WeatherAi.setModal( true );
    WeatherAi.exec();
}

void MainWindow::on_actionWeatherAI_Python_triggered()
{
    WeatherAIPython WeatherAiPython;
    WeatherAiPython.setModal( true );
    WeatherAiPython.exec();
}
