#include "mainwindow.h"

#include <iostream>

#include "ui_mainwindow.h"
#include "BankCardList.h"
#include <QMessageBox>
#include <QSizePolicy>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDir>
#include <qvalidator.h>

#include "feature/user/IUserService.h"
#include "feature/user/User.h"

MainWindow::MainWindow(IContext &context, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
                                                             context(context) {
    ui->setupUi(this);

    setStyles();
    initDesigns();

    ui->stackedWidget->setCurrentWidget(ui->loginScreen);

    ui->LE_phone->setInputMask(R"(\+3\8\0 (99) 999-9999;_)");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_B_enter_clicked() {
    const QString enteredPhone = ui->LE_phone->text();

    const QString enteredPassword = ui->LE_password->text();
    if (authenticate(enteredPhone.toStdString(), enteredPassword.toStdString())) {
        const IUserService &userService = context.userService();
        User user = userService.getUserByPhone(enteredPhone.toStdString()).value();
        ui->L_welcomeUser->setText("Welcome!");
        animateTransition(ui->loginScreen, ui->dashboardScreen);

        auto cardList = new BankCardList(context, user.id,
                                         ui->B_prevCard, ui->B_nextCard);
        ui->cardList->addWidget(cardList);
        connect(cardList, &BankCardList::selectedCardClicked,
                this, [this, cardList](const Card &card) {
                    if (card.status == Card::active) {
                        ui->W_currentCard->setCard(card);
                        if (card.designId.has_value()) {
                            QPixmap design1(QString::fromStdString(
                                context.cardDesignService().getCardDesignById(card.designId.value()).
                                value().imageRef));
                            ui->W_currentCard->setDesignPixmap(design1);
                        } else {
                            ui->W_currentCard->setDesignPixmap();
                        }
                        ui->B_nextCard->hide();
                        ui->B_prevCard->hide();
                        setupPinScreen();
                        cardList->hideLeftRight();
                        animateTransition(ui->dashboardScreen, ui->pinScreen, 340, [this] {
                            ui->B_nextCard->show();
                            ui->B_prevCard->show();
                        });
                    }
                });
    } else {
        QMessageBox::critical(this, "Wrong credentials", "No such user");
    }
}

void MainWindow::on_B_enterPin_clicked() {
    const QString enteredPin = ui->LE_pin->text();
    if (context.cardService().accessToCard(ui->W_currentCard->getCardId(), enteredPin.toStdString())) {
        qDebug() << "SUCCESS";
        ui->L_accessDenied->hide();
        ui->W_currentCardOnScreen->setCard(context.cardService().getCardById(ui->W_currentCard->getCardId()).value());
        if (context.cardService().getCardById(ui->W_currentCard->getCardId()).value().designId.has_value()) {
            QPixmap design1(QString::fromStdString(
                context.cardDesignService().getCardDesignById(context.cardService().getCardById(ui->W_currentCard->getCardId()).value().designId.value()).
                value().imageRef));
            ui->W_currentCardOnScreen->setDesignPixmap(design1);
        } else {
            ui->W_currentCardOnScreen->setDesignPixmap();
        }
        animateTransition(ui->pinScreen, ui->cardScreen);
    } else {
        qDebug() << "FAILURE";
        ui->L_accessDenied->show();
        ui->LE_pin->clear();
        shakeLabel(ui->L_accessDenied);
        if (context.cardService().getCardById(ui->W_currentCard->getCardId()).value().status == Card::blocked) {
            animateTransition(ui->pinScreen, ui->dashboardScreen);
        }
    }
}

void MainWindow::on_B_logout_clicked() {
    ui->LE_phone->clear();
    ui->LE_password->clear();

    QLayoutItem *item;
    while ((item = ui->cardList->takeAt(0)) != nullptr) {
        if (item->widget())
            delete item->widget();
        delete item;
    }

    animateTransition(ui->dashboardScreen, ui->loginScreen);
}

void MainWindow::setupPinScreen() {
    ui->LE_pin->setEchoMode(QLineEdit::Password);
    ui->LE_pin->setMaxLength(4);
    ui->LE_pin->setValidator(new QIntValidator(0, 9999, this));
    ui->L_accessDenied->hide();
}

bool MainWindow::authenticate(const std::string &phone, const std::string &password) const {
    return context.userService().accessToUser(phone, password);
}


void MainWindow::animateTransition(QWidget *from, QWidget *to, int initY, std::function<void()> onFinished) {
    int h = ui->stackedWidget->height();

    to->move(0, h);
    to->show();

    auto *group = new QParallelAnimationGroup;

    auto *animFrom = new QPropertyAnimation(from, "pos", group);
    animFrom->setDuration(400);
    animFrom->setStartValue(from->pos());
    animFrom->setEndValue(QPoint(0, -h + initY));

    auto *animTo = new QPropertyAnimation(to, "pos", group);
    animTo->setDuration(400);
    animTo->setStartValue(QPoint(to->pos().x(), to->pos().y() - initY));
    animTo->setEndValue(QPoint(0, 0));

    group->addAnimation(animFrom);
    group->addAnimation(animTo);
    group->start(QAbstractAnimation::DeleteWhenStopped);

    connect(group, &QParallelAnimationGroup::finished, this, [this, to, from]() {
        ui->stackedWidget->setCurrentWidget(to);
        from->hide();
        from->move(0, 0);
    });
}

void MainWindow::shakeLabel(QLabel *label) {
    QPoint originalPos = label->pos();

    QPropertyAnimation *animation = new QPropertyAnimation(label, "pos");
    animation->setDuration(400); // тривалість (мс)

    animation->setKeyValueAt(0, originalPos);
    animation->setKeyValueAt(0.1, originalPos + QPoint(-5, 0));
    animation->setKeyValueAt(0.2, originalPos + QPoint(5, 0));
    animation->setKeyValueAt(0.3, originalPos + QPoint(-5, 0));
    animation->setKeyValueAt(0.4, originalPos + QPoint(5, 0));
    animation->setKeyValueAt(0.5, originalPos + QPoint(-3, 0));
    animation->setKeyValueAt(0.6, originalPos + QPoint(3, 0));
    animation->setKeyValueAt(1, originalPos);

    animation->setEasingCurve(QEasingCurve::OutElastic);

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}


void MainWindow::setStyles() const {
    ui->L_welcome->setObjectName("welcomeLabel");
    ui->L_welcomeUser->setObjectName("welcomeUserLabel");
    ui->L_accessDenied->setObjectName("accessDeniedLabel");

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
        QLabel#accessDeniedLabel {
            color: #800000;
            font-size: 15px;
            font-weight: bold;
            qproperty-alignment: 'AlignCenter';
        }
        QLineEdit { background-color: #222222; color: #eae3d9; border: 3px solid #805535; font-weight: bold; font-size: 10pt; border-radius: 5px; padding: 5px; }

        QPushButton { background-color: #805535; color: #eae3d8; border-radius: 10px; font-weight: bold; padding: 5px 10px; border: 3px solid #eae3d8; }
        QPushButton:hover { background-color: #734d30; }
        QPushButton:pressed { background-color: #5a3b25; }
    )");
}

void MainWindow::initDesigns() {

    context.cardDesignService().deleteAll();
    CardDesign newDesign;
    newDesign.name = "Church";
    newDesign.author = "iryna";
    newDesign.imageRef = ":/designs/resources/designs/church1.jpg";
    context.cardDesignService().createCardDesign(newDesign);

    CardDesign newDesign1;
    newDesign1.name = "Spiral staircase";
    newDesign1.author = "iryna";
    newDesign1.imageRef = ":/designs/resources/designs/spiral_staircase.jpg";
    context.cardDesignService().createCardDesign(newDesign1);

    auto allDesigns = context.cardDesignService().getAllCardDesigns();

    if (allDesigns.size() >= 2) {
        Card card = context.cardService().getCardById(2).value();
        card.designId = allDesigns[0].id;
        context.cardService().updateCard(card);

        card = context.cardService().getCardById(4).value();
        card.designId = allDesigns[1].id;
        context.cardService().updateCard(card);

    }
}
