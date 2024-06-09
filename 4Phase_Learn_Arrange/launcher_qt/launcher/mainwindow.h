#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QDesktopServices>

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
    void on_pushButton_clicked();
    void proc_login_reply();

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager m_access_mng;
    QNetworkReply* m_reply = nullptr;

    enum login_opt {
        create_room,
        follow_room,
        change_password
    } m_cur_opt;

};
#endif // MAINWINDOW_H
