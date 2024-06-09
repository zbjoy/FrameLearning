#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

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
    void on_createroom_clicked();
    void proc_login_reply();

    void on_register_2_clicked();

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager m_access_mng;
    QNetworkReply* m_reply = nullptr;
};
#endif // MAINWINDOW_H
