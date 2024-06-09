#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QStringList>
#include <QJsonDocument>
// #include <QJsonArray> // 应该不涉及
#include <QJsonObject>
#include <QDebug>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QDesktopServices>
#include <QProcess>

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

void MainWindow::on_createroom_clicked()
{
    /* 取出用户名和密码 */
    QString username = ui->username->text();
    QString password = ui->password->text();


    /* 创建密码的签名值 (QCryptographicHash) */
    QCryptographicHash md5_hash(QCryptographicHash::Md5);
    md5_hash.addData(password.toUtf8());
    QString md5_password(md5_hash.result().toHex());


    /* 拼登录请求 (应该是 post 请求) 使用Json格式 */

    // 构建一个json对象, 填入用户名和密码的签名值
    QJsonObject json;
    json["username"] = username;
    json["password"] = md5_password;

    // 将json对象放到JsonDocument中
    QJsonDocument json_doc(json);
    // qDebug() << json_doc.toJson();
    QString output = json_doc.toJson();


    /* 发给登录服务器 (发 http 请求 ---> Qt网络访问库(要异步处理)) */
    QNetworkRequest req(QUrl("http://47.113.177.68/login/"));
    // QNetworkRequest req(QUrl("http://127.0.0.1:4321/login"));
    req.setHeader(QNetworkRequest::ContentLengthHeader, output.size());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 接收发来的信号
    m_reply = m_access_mng.post(req, output.toUtf8());


    /* 异步处理服务器的回复 (也就是信号和槽, 有事件才处理: connect 服务器回复与回复的处理) */
    connect(m_reply, SIGNAL(finished()), this, SLOT(proc_login_reply()));

}

// 处理回复
void MainWindow::proc_login_reply()
{
    // 接收服务器发回来的数据
    // QString reply = m_reply->readAll();


    // 判断登录结果
    QJsonDocument json_doc;
    json_doc = QJsonDocument::fromJson(m_reply->readAll());

    QJsonObject jsonObject = json_doc.object();

    if (jsonObject["login_result"].toString() == "OK")
    {
        // qDebug() << "认证成功";
        QProcess proc;
        QStringList argv;
        argv.push_back("47.113.177.68");
        argv.push_back("8899");
        proc.startDetached("client.exe", argv, ".");
        qApp->exit();
    }
    else
    {
        QMessageBox::information(this, "登录失败", "用户名或密码错误");
    }

}


void MainWindow::on_register_2_clicked()
{
    // 启动浏览器并访问界面
    QDesktopServices::openUrl(QUrl("http://47.113.177.68"));
}

