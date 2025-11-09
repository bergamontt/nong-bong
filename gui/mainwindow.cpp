#include "mainwindow.h"

#include <iostream>

#include "ui_mainwindow.h"
#include "BankCardList.h"
#include "TransactionListWidget.h"
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
        const User user = userService.getUserByPhone(enteredPhone.toStdString()).value();
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
                            const QPixmap design1(QString::fromStdString(
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
            const QPixmap design1(QString::fromStdString(
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

void MainWindow::on_B_changePin_clicked() {
    setupPinChangeScreen();
    animateTransition(ui->cardScreen, ui->changePinScreen);
}

void MainWindow::on_B_enterNewPin_clicked() {
    const QString oldPin = ui->LE_oldPin->text();
    const QString newPin = ui->LE_newPin_1->text();
    const QString newPinConfirm = ui->LE_newPin_2->text();
    if (newPin.toStdString() == newPinConfirm.toStdString() 
        && newPin.size() == 4 && newPinConfirm.size() == 4)
    {
        const std::string& oldPinStr = oldPin.toStdString();
        const std::string& newPinStr = newPin.toStdString();
        const int id = ui->W_currentCard->getCardId();
        const bool success = context.cardService().changeCardPin(id, oldPinStr, newPinStr);
        if (success) 
        {
            on_B_cancelChange_clicked();
            return;
        }
    }
    ui->L_newPinInvalid->show();
    shakeLabel(ui->L_newPinInvalid);
}

void MainWindow::on_B_cancelChange_clicked() {
    ui->LE_oldPin->clear();
    ui->LE_newPin_1->clear();
    ui->LE_newPin_2->clear();
    ui->L_newPinInvalid->hide();
    animateTransition(ui->changePinScreen, ui->cardScreen);
}

void MainWindow::on_B_withdraw_clicked() {
    setupWithdrawScreen();
    animateTransition(ui->cardScreen, ui->withdrawScreen);
}

void MainWindow::on_B_100_clicked() const {
    ui->LE_enteredAmount->clear();
    ui->LE_enteredAmount->insert("100");
}

void MainWindow::on_B_200_clicked() const {
    ui->LE_enteredAmount->clear();
    ui->LE_enteredAmount->insert("200");
}

void MainWindow::on_B_500_clicked() const {
    ui->LE_enteredAmount->clear();
    ui->LE_enteredAmount->insert("500");
}

void MainWindow::on_B_1000_clicked() const {
    ui->LE_enteredAmount->clear();
    ui->LE_enteredAmount->insert("1000");
}

void MainWindow::on_B_cancelWithdraw_clicked() {
    ui->L_failWithdrawal->hide();
    animateTransition(ui->withdrawScreen, ui->cardScreen);
}

void MainWindow::on_B_enterWithdraw_clicked() const {
    const int enteredAmount = ui->LE_enteredAmount->text().toInt();
    auto newTransaction = BankTransaction();
    newTransaction.type = "withdrawal";
    newTransaction.fromCardId = ui->W_currentCardwd->getCardId();
    newTransaction.amount = enteredAmount;
    newTransaction.currencyCode = "UAH";
    newTransaction.description = "withdrawal";
    newTransaction.comment = "";
    newTransaction.status = "completed";

    if (context.bankTransactionService().createBankTransaction(newTransaction)) {
        qDebug() << "Transaction SUCCESS";
        ui->L_failWithdrawal->hide();
        ui->W_currentCardwd->setCard(context.cardService().getCardById(ui->W_currentCardwd->getCardId()).value());
        ui->W_currentCard->setCard(context.cardService().getCardById(ui->W_currentCardwd->getCardId()).value());
        ui->W_currentCardOnScreen->setCard(context.cardService().getCardById(ui->W_currentCardwd->getCardId()).value());
        ui->W_currentCardd->setCard(context.cardService().getCardById(ui->W_currentCardwd->getCardId()).value());
        ui->LE_enteredAmount->clear();
    } else {
        qDebug() << "Transaction FAILURE";
        ui->L_failWithdrawal->show();
        shakeLabel(ui->L_failWithdrawal);
    }
}

void MainWindow::on_B_deposit_clicked() {
    setupDepositScreen();
    animateTransition(ui->cardScreen, ui->depositScreen);
}

void MainWindow::on_B_D20_clicked() const {
    const int currentAmount = ui->L_amountDeposit->text().toInt()+20;
    ui->L_amountDeposit->clear();
    ui->L_amountDeposit->setText(QString::fromStdString(std::to_string(currentAmount)));
    ui->B_cancelDeposit->setDisabled(true);
}

void MainWindow::on_B_D50_clicked() const {
    const int currentAmount = ui->L_amountDeposit->text().toInt()+50;
    ui->L_amountDeposit->clear();
    ui->L_amountDeposit->setText(QString::fromStdString(std::to_string(currentAmount)));
    ui->B_cancelDeposit->setDisabled(true);
}

void MainWindow::on_B_D100_clicked() const {
    const int currentAmount = ui->L_amountDeposit->text().toInt()+100;
    ui->L_amountDeposit->setDisabled(true);
    ui->L_amountDeposit->setText(QString::fromStdString(std::to_string(currentAmount)));
    ui->B_cancelDeposit->setDisabled(true);
}

void MainWindow::on_B_D200_clicked() const {
    const int currentAmount = ui->L_amountDeposit->text().toInt()+200;
    ui->L_amountDeposit->clear();
    ui->L_amountDeposit->setText(QString::fromStdString(std::to_string(currentAmount)));
    ui->B_cancelDeposit->setDisabled(true);
}

void MainWindow::on_B_D500_clicked() const {
    const int currentAmount = ui->L_amountDeposit->text().toInt()+500;
    ui->L_amountDeposit->clear();
    ui->L_amountDeposit->setText(QString::fromStdString(std::to_string(currentAmount)));
    ui->B_cancelDeposit->setDisabled(true);
}

void MainWindow::on_B_D1000_clicked() const {
    const int currentAmount = ui->L_amountDeposit->text().toInt()+1000;
    ui->L_amountDeposit->clear();
    ui->L_amountDeposit->setText(QString::fromStdString(std::to_string(currentAmount)));
    ui->B_cancelDeposit->setDisabled(true);
}

void MainWindow::on_B_cancelDeposit_clicked() {
    ui->B_D20->hide();
    ui->B_D50->hide();
    ui->B_D100->hide();
    ui->B_D200->hide();
    ui->B_D500->hide();
    ui->B_D1000->hide();
    animateTransition(ui->depositScreen, ui->cardScreen);
}

void MainWindow::on_B_enterDeposit_clicked() const {
    const int enteredAmount = ui->L_amountDeposit->text().toInt();
    auto newTransaction = BankTransaction();
    newTransaction.type = "deposit";
    newTransaction.toCardId = ui->W_currentCardd->getCardId();
    newTransaction.amount = enteredAmount;
    newTransaction.currencyCode = "UAH";
    newTransaction.description = "deposit";
    newTransaction.comment = "";
    newTransaction.status = "completed";

    if (context.bankTransactionService().createBankTransaction(newTransaction)) {
        qDebug() << "Deposit SUCCESS";
        ui->W_currentCardd->setCard(context.cardService().getCardById(ui->W_currentCardd->getCardId()).value());
        ui->W_currentCardwd->setCard(context.cardService().getCardById(ui->W_currentCardd->getCardId()).value());
        ui->W_currentCard->setCard(context.cardService().getCardById(ui->W_currentCardd->getCardId()).value());
        ui->W_currentCardOnScreen->setCard(context.cardService().getCardById(ui->W_currentCardd->getCardId()).value());

        ui->B_cancelDeposit->setDisabled(false);
        ui->L_amountDeposit->clear();
        ui->L_amountDeposit->setText("0");
    } else {
        qDebug() << "Deposit FAILURE";
    }

}

void MainWindow::setupPinScreen() {
    ui->LE_pin->setEchoMode(QLineEdit::Password);
    ui->LE_pin->setMaxLength(4);
    ui->LE_pin->setValidator(new QIntValidator(0, 9999, this));
    ui->L_accessDenied->hide();
}

void MainWindow::setupPinChangeScreen() {
    ui->LE_oldPin->setEchoMode(QLineEdit::Password);
    ui->LE_oldPin->setMaxLength(4);
    ui->LE_oldPin->setValidator(new QIntValidator(0, 9999, this));
    ui->LE_oldPin->setPlaceholderText("Old PIN");

    ui->LE_newPin_1->setEchoMode(QLineEdit::Password);
    ui->LE_newPin_1->setMaxLength(4);
    ui->LE_newPin_1->setValidator(new QIntValidator(0, 9999, this));
    ui->LE_newPin_1->setPlaceholderText("New PIN");

    ui->LE_newPin_2->setEchoMode(QLineEdit::Password);
    ui->LE_newPin_2->setMaxLength(4);
    ui->LE_newPin_2->setValidator(new QIntValidator(0, 9999, this));
    ui->LE_newPin_2->setPlaceholderText("Repeat");

    ui->L_newPinInvalid->setStyleSheet("color: #800000;");
    QSizePolicy sp = ui->L_newPinInvalid->sizePolicy();
    sp.setRetainSizeWhenHidden(true);
    ui->L_newPinInvalid->setSizePolicy(sp);
    ui->L_newPinInvalid->hide();
}

void MainWindow::setupWithdrawScreen() {
    ui->W_currentCardwd->setCard(context.cardService().getCardById(ui->W_currentCardOnScreen->getCardId()).value());
    if (context.cardService().getCardById(ui->W_currentCardOnScreen->getCardId()).value().designId.has_value()) {
        const QPixmap design1(QString::fromStdString(
            context.cardDesignService().getCardDesignById(context.cardService().getCardById(ui->W_currentCardOnScreen->getCardId()).value().designId.value()).
            value().imageRef));
        ui->W_currentCardwd->setDesignPixmap(design1);
    } else {
        ui->W_currentCardwd->setDesignPixmap();
    }
    ui->LE_enteredAmount->clear();
    ui->LE_enteredAmount->setValidator(new QIntValidator(0, 10000, this));
    ui->L_failWithdrawal->hide();
}

void MainWindow::on_B_transactionHistory_clicked() {
    setupTransHistoryScreen();
    animateTransition(ui->cardScreen, ui->transHistoryScreen);
}

void MainWindow::on_B_backToCard_clicked() {
    QLayout* layout = ui->transHistoryContainer->layout();
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    animateTransition(ui->transHistoryScreen, ui->cardScreen);
}

void MainWindow::setupTransHistoryScreen() {
    int cardId = ui->W_currentCard->getCardId();
    QWidget* listWidget = new TransactionListWidget(context, cardId);
    listWidget->setMouseTracking(true);
    ui->transHistoryContainer->layout()->addWidget(listWidget);
}

void MainWindow::setupDepositScreen() const {
    ui->W_currentCardd->setCard(context.cardService().getCardById(ui->W_currentCardOnScreen->getCardId()).value());
    if (context.cardService().getCardById(ui->W_currentCardOnScreen->getCardId()).value().designId.has_value()) {
        const QPixmap design1(QString::fromStdString(
            context.cardDesignService().getCardDesignById(context.cardService().getCardById(ui->W_currentCardOnScreen->getCardId()).value().designId.value()).
            value().imageRef));
        ui->W_currentCardd->setDesignPixmap(design1);
    } else {
        ui->W_currentCardd->setDesignPixmap();
    }
    ui->B_cancelDeposit->setDisabled(false);
    ui->L_amountDeposit->clear();
    ui->L_amountDeposit->setText("0");
    ui->B_D20->show();
    ui->B_D50->show();
    ui->B_D100->show();
    ui->B_D200->show();
    ui->B_D500->show();
    ui->B_D1000->show();
    ui->B_D20->setIcon(QIcon(":uah/resources/uah/20_uah.png"));
    ui->B_D20->setIconSize(QSize(90, 174));
    ui->B_D50->setIcon(QIcon(":uah/resources/uah/50_uah.png"));
    ui->B_D50->setIconSize(QSize(90, 174));
    ui->B_D100->setIcon(QIcon(":uah/resources/uah/100_uah.png"));
    ui->B_D100->setIconSize(QSize(90, 174));
    ui->B_D200->setIcon(QIcon(":uah/resources/uah/200_uah.png"));
    ui->B_D200->setIconSize(QSize(90, 174));
    ui->B_D500->setIcon(QIcon(":uah/resources/uah/500_uah.png"));
    ui->B_D500->setIconSize(QSize(90, 174));
    ui->B_D1000->setIcon(QIcon(":uah/resources/uah/1000_uah.png"));
    ui->B_D1000->setIconSize(QSize(90, 174));

}

bool MainWindow::authenticate(const std::string &phone, const std::string &password) const {
    return context.userService().accessToUser(phone, password);
}

void MainWindow::animateTransition(QWidget *from, QWidget *to, int initY, std::function<void()> onFinished) {
    const int h = ui->stackedWidget->height();

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
    const QPoint originalPos = label->pos();

    const auto animation = new QPropertyAnimation(label, "pos");
    animation->setDuration(400);

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
    ui->L_cashWithdrawal->setObjectName("cashWithdrawalLabel");
    ui->L_failWithdrawal->setObjectName("failWithdrawalLabel");
    ui->L_cashDepositing->setObjectName("cashDepositingLabel");

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
        QLabel#cashWithdrawalLabel {
            font-size: 20px;
            font-weight: bold;
            qproperty-alignment: 'AlignCenter';
        }
        QLabel#cashDepositingLabel {
            font-size: 20px;
            font-weight: bold;
            qproperty-alignment: 'AlignCenter';
        }
        QLabel#accessDeniedLabel {
            color: #800000;
            font-size: 15px;
            font-weight: bold;
            qproperty-alignment: 'AlignCenter';
        }
        QLabel#failWithdrawalLabel {
            color: #800000;
            font-size: 15px;
            font-weight: bold;
            qproperty-alignment: 'AlignCenter';
        }
        QLineEdit { background-color: #222222; color: #eae3d9; border: 3px solid #805535; font-weight: bold; font-size: 10pt; border-radius: 5px; padding: 5px; }

        QPushButton { background-color: #805535; color: #eae3d8; border-radius: 10px; font-weight: bold; padding: 5px 10px; border: 3px solid #eae3d8; }
        QPushButton:hover { background-color: #734d30; }
        QPushButton:pressed { background-color: #5a3b25; }
        QPushButton:disabled { background-color: #a69c91; color: #d0c8bf; border: 3px solid #cfc6bb; }

        QTableView {
            background-color: #57735d;
            color: #eae3d8;
            gridline-color: #4a5b4f;
            selection-background-color: #375b4f;
            selection-color: #eae3d8;
            alternate-background-color: #506853;
        }

        QHeaderView::section {
            background-color: #445a4a;
            color: #eae3d8;
            padding: 4px;
            border: none;
        }
    )");
}

void MainWindow::initDesigns() {
    std::cout << QPixmap(":uah/resources/uah/20_uah.png").isNull();
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
