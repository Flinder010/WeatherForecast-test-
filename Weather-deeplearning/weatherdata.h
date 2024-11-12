#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QString>

class Today
{

public:
    Today()
    {
        date = "2024-11-10";
        city = "西安";

        ganmao = "感冒指数";

        wendu = 0;
        shidu = "0%";
        pm25 = 0;
        quality = "无数据";

        type = "多云";

        fx = "南风";
        fl = "2级";

        high = 30;
        low = 18;
    }

    QString date;
    QString city;

    QString ganmao;

    int wendu;
    QString shidu;
    int pm25;
    QString quality;

    QString type;

    QString fx;
    QString fl;

    int high;
    int low;

};

class Day
{

public:
    Day()
    {
        date = "2024-11-10";
        week = "周日";

        type = "多云";

        high = 0;
        low = 0;

        fx = "南风";
        fl = "1级";

        aqi = 0;
    }

    QString date;
    QString week;

    QString type;

    int high;
    int low;

    QString fx;
    QString fl;

    int aqi;

};

#endif // WEATHERDATA_H
