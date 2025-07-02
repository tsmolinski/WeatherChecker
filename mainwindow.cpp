#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QScopedPointer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), jsonString(new QString())
{
    ui->setupUi(this);

    initCountryList();
    populateCmbCountry();

    detailsShowed = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initCountryList()
{
    countryListWithCode =
        {
            "AM - Armenia",
            "AU - Australia",
            "BS - Bahamas",
            "CH - Switzerland",
            "CZ - Czech Republic",
            "DE - Germany",
            "FR - France",
            "NO - Norway",
            "PL - Poland",
            "SE - Sweden",
            "UA - Ukraine",
            "US - United States of America"
        };
}

QStringList& MainWindow::getCountryList()
{
    return countryListWithCode;
}

void MainWindow::populateCmbCountry()
{
    ui->cmbCountry->addItems(getCountryList());
}

void MainWindow::onWeatherDataReceived()
{
    networkReply = qobject_cast<QNetworkReply*>(sender());

    if(networkReply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error: " << networkReply->error();
    }

    QByteArray responseData = networkReply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = jsonDoc.object();
    *jsonString = jsonDoc.toJson(QJsonDocument::Indented);

    QString weatherDescription = jsonObj["weather"].toArray()[0].toObject()["description"].toString();
    int weatherConditionCode = jsonObj["weather"].toArray()[0].toObject()["id"].toInt();
    double windSpeed = jsonObj["wind"].toObject()["speed"].toDouble();
    QString city = jsonObj["name"].toString();
    QString country = jsonObj["sys"].toObject()["country"].toString();
    double temp = jsonObj["main"].toObject()["temp"].toDouble() - 273.15;
    double tempMax = jsonObj["main"].toObject()["temp_max"].toDouble() - 273.15;
    double tempMin = jsonObj["main"].toObject()["temp_min"].toDouble() - 273.15;
    double feelsLike = jsonObj["main"].toObject()["feels_like"].toDouble() - 273.15;
    double humidity = jsonObj["main"].toObject()["humidity"].toDouble();

    ui->lblWeatherDescription->setText(weatherDescription);
    ui->lblWindSpeed->setText(QString::number(windSpeed));
    ui->lblTemp->setText(QString::number(std::round(temp)));
    ui->lblCity->setText(city);
    ui->lblCountry->setText(country);
    ui->lblTempMax->setText(QString::number(tempMax));
    ui->lblTempMin->setText(QString::number(tempMin));
    ui->lblFeelsLike->setText(QString::number(feelsLike));
    ui->lblHumidity->setText(QString::number(humidity));
}

void MainWindow::on_btnGo_clicked()
{
    QString countryCode, zipCode, apiKey;

    apiKey = "5c6b62689aaad2eb882fe67b54431ae0";
    countryCode = ui->cmbCountry->currentText().split("-")[0].trimmed();
    zipCode = ui->txtZipCode->text();

    QString url = QString("https://api.openweathermap.org/data/2.5/weather?zip=%1,%2&appid=%3").arg(zipCode).arg(countryCode).arg(apiKey);

    //qDebug() << url;

    networkAccessManager = new QNetworkAccessManager(this);
    QNetworkRequest request((QUrl(url)));

    networkReply = networkAccessManager->get(request);

    connect(networkReply, &QNetworkReply::finished, this, &MainWindow::onWeatherDataReceived);

    if(networkReply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Network Error: " << networkReply->errorString();
    }
}


void MainWindow::on_btnShowDetails_clicked()
{
    if(jsonString)
    {
        if(!detailsShowed)
        {
            ui->txtJsonString->setText(*jsonString);
            detailsShowed = !detailsShowed;
        }
        else
        {
            ui->txtJsonString->clear();
            detailsShowed = !detailsShowed;
        }
    }
}

