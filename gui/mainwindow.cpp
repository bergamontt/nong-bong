#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "BankCardWidget.h"
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    users["+380 (67) 111-1111"] = "12345";
    users["+380 (67) 222-2222"] = "qwerty2";
    users["+380 (67) 333-3333"] = "qwerty";
    ui->setupUi(this);

    setStyles();

    ui->stackedWidget->setCurrentWidget(ui->loginScreen);

    ui->LE_phone->setInputMask(R"(\+3\8\0 (99) 999-9999;_)");

    Card myBlockedCard;
    myBlockedCard.id = 1;
    myBlockedCard.userId = 42;
    myBlockedCard.cardNumber = "4444333322221111";
    myBlockedCard.allowCredit = 1;
    myBlockedCard.creditLimit.reset();
    myBlockedCard.currencyCode = "USD";
    myBlockedCard.balance = 20000;
    myBlockedCard.dailyLimit = 100000;
    myBlockedCard.pinHash = "sha256$xxxxx";
    myBlockedCard.status = "blocked";

    {
        std::time_t now = std::time(nullptr);
        std::time_t later = now + 20;
        std::tm tm_later = *std::localtime(&later);
        myBlockedCard.blockedUntil = tm_later;
    }

    BankCardWidget* w = new BankCardWidget;
    w->setCard(myBlockedCard);
    w->setDailyUsageRatio(0.8);
    w->setFixedSize(300, 180);
    ui->cardList->addWidget(w);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_B_enter_clicked()
{
    const QString enteredPhone = ui->LE_phone->text();

    const QString enteredPassword = ui->LE_password->text();
    if(authenticate(enteredPhone.toStdString(), enteredPassword.toStdString())){
        ui->L_welcomeUser->setText("Welcome!");
        animateTransition(ui->loginScreen, ui->dashboardScreen);
    } else{
        QMessageBox::critical(this,"Wrong credentials","No such user");
    }

}

bool MainWindow::authenticate(const std::string& phone, const std::string& password){
    if(users.contains(phone)){
        if(users[phone]==password){
            return true;
        }
    }
    return false;
}

void MainWindow::animateTransition(QWidget* from, QWidget* to)
{
    int h = ui->stackedWidget->height();

    to->move(0, h);
    to->show();

    auto *group = new QParallelAnimationGroup;

    auto *animFrom = new QPropertyAnimation(from, "pos",group);
    animFrom->setDuration(400);
    animFrom->setStartValue(from->pos());
    animFrom->setEndValue(QPoint(0, -h));

    auto *animTo = new QPropertyAnimation(to, "pos",group);
    animTo->setDuration(400);
    animTo->setStartValue(to->pos());
    animTo->setEndValue(QPoint(0, 0));

    group->addAnimation(animFrom);
    group->addAnimation(animTo);
    group->start(QAbstractAnimation::DeleteWhenStopped);

    connect(group, &QParallelAnimationGroup::finished, this, [this, to, from]() {
        ui->stackedWidget->setCurrentWidget(to);
        from->hide();
        from->move(0,0);
    });
}

void MainWindow::on_B_logout_clicked()
{
    ui->LE_phone->clear();
    ui->LE_password->clear();

    animateTransition(ui->dashboardScreen, ui->loginScreen);
}

void MainWindow::setStyles() const {

    ui->L_welcome->setObjectName("welcomeLabel");
    ui->L_welcomeUser->setObjectName("welcomeUserLabel");

    qApp->setStyleSheet(R"(
        QMainWindow { background-color: #57735d; }
        QStackedWidget { background-color: #57735d; }


        QLabel { color: #eae3d8; font-size: 16px; font-weight: bold; }
        QLabel#welcomeLabel {
            color: #222220;
            font-size: 20px;
            font-weight: bold;
            background-color: #eae4d9;
            border-radius: 20px;
            padding: 10px 20px;
            qproperty-alignment: 'AlignCenter';
        }
        QLabel#welcomeUserLabel {
            color: #805535;
            font-size: 15px;
            font-weight: bold;
            background-color: #eae4d9;
            border-radius: 20px;
            padding: 10px 20px;
            qproperty-alignment: 'AlignCenter';
        }
        QLineEdit { background-color: #222222; color: #eae3d9; border: 3px solid #805535; font-weight: bold; font-size: 10pt; border-radius: 5px; padding: 5px; }

        QPushButton { background-color: #805535; color: #eae3d8; border-radius: 10px; font-weight: bold; padding: 5px 15px; border: 3px solid #eae3d8; }
        QPushButton:hover { background-color: #734d30; }
        QPushButton:pressed { background-color: #5a3b25; }
    )");
}

