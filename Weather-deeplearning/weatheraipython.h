#ifndef WEATHERAIPYTHON_H
#define WEATHERAIPYTHON_H

#include <Python.h>
#include <QDialog>

namespace Ui {
class WeatherAIPython;
}

class WeatherAIPython : public QDialog
{
    Q_OBJECT

public:
    explicit WeatherAIPython(QWidget *parent = nullptr);
    ~WeatherAIPython();

private slots:
    void on_pushButton_clicked();

private:
    Ui::WeatherAIPython *ui;
};

#endif // WEATHERAIPYTHON_H
