#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>
#include <QCryptographicHash>
#include <QJsonObject>
#include <QJsonDocument>
#include <QProcess>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // 取出用户名和密码
    QString username = ui->username->text();
    QString password = ui->password->text();

    // qDebug() << username;
    // qDebug() << password;

    // 创建密码的签名值
    QCryptographicHash md5_hash(QCryptographicHash::Md5);
    md5_hash.addData(password.toUtf8());
    QString md5_password(md5_hash.result().toHex());

    // 拼登录请求(Json格式)
    // 构建一个json对象, 填入用户名和密码的签名值
    QJsonObject json;
    json["username"] = username;
    json["password"] = password;

    // 将json对象放到doc中
    QJsonDocument json_doc(json);
    QString output = json_doc.toJson();

    // 发送给登录服务器 (发送http ---> qt网络访问库)
    QNetworkRequest req(QUrl("http://47.113.177.68/login/"));
    req.setHeader(QNetworkRequest::ContentLengthHeader, output.size());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    m_reply = m_access_mng.post(req, output.toUtf8());
    // 异步处理服务器的回复 (connect 服务器回复和回复的处理函数)
    connect(m_reply, SIGNAL(finished()), this, SLOT(proc_login_reply()));
}

void MainWindow::proc_login_reply()
{
    // 判断登录结果
    QJsonDocument json_doc = QJsonDocument::fromJson(m_reply->readAll());

    QJsonObject json = json_doc.object();

    if ("OK" == json["login_result"].toString())
    {
        if (create_room == m_cur_opt)
        {
            QProcess pro;
            QStringList argv;
            QString room_no = json["room_no"].toString();

            QMessageBox::information(this, "房间号", "房间号: " + room_no);

            argv.push_back("47.113.177.68");
            argv.push_back(room_no);
            pro.startDetached("client.exe", argv, ".");
            qApp->exit();
        }
        else
        {

        }
    }
    else
    {
        QMessageBox::information(this, "登陆失败", "用户名或密码错误");
    }
}

