#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QNetworkAccessManager;
class QNetworkReply;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnGo_clicked();

    void on_btnShowDetails_clicked();

private:
    Ui::MainWindow *ui;

    bool detailsShowed;

    QStringList countryListWithCode;

    QScopedPointer<QString> jsonString;

    QNetworkAccessManager* networkAccessManager;
    QNetworkReply* networkReply;

    void initCountryList();

    QStringList& getCountryList();

    void populateCmbCountry();

    void onWeatherDataReceived();
};
#endif // MAINWINDOW_H
