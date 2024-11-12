#include "weatheraipython.h"
#include "ui_weatheraipython.h"

WeatherAIPython::WeatherAIPython(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WeatherAIPython)
{
    ui->setupUi(this);
}

WeatherAIPython::~WeatherAIPython()
{
    delete ui;
}

void WeatherAIPython::on_pushButton_clicked()
{
    QPixmap pixmap( ":/new/prefix1/Figure/PythonModel.png" );
    ui -> label -> setPixmap( pixmap );
}
