#ifndef WEATHERAI_H
#define WEATHERAI_H

#include <QDialog>

namespace Ui {
class WeatherAI;
}

class WeatherAI : public QDialog
{
    Q_OBJECT

public:
    explicit WeatherAI(QWidget *parent = nullptr);
    ~WeatherAI();

private slots:
    void on_Button1_clicked();

    void on_Button1_2_clicked();

private:
    Ui::WeatherAI *ui;
};

#endif // WEATHERAI_H
