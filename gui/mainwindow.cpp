#include "mainwindow.h"

#include <iostream>
#include <optional>

#include "ui_mainwindow.h"
#include "BankCardList.h"
#include "TransactionListWidget.h"
#include <QMessageBox>
#include <QSizePolicy>
#include <QButtonGroup>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDir>
#include <qvalidator.h>
#include <regex>

#include "feature/user/IUserService.h"
#include "feature/user/User.h"

using namespace std;

MainWindow::MainWindow(IContext &context, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
                                                             context(context) {
    ui->setupUi(this);

    setStyles();
    initDesigns();

    ui->stackedWidget->setCurrentWidget(ui->loginScreen);

    ui->LE_phone->setInputMask(R"(\+3\8\0 (99) 999-9999;_)");

    ui->W_currentCard->setContext(context);
    ui->W_currentCard_2->setContext(context);
    ui->W_currentCard_3->setContext(context);
    ui->W_currentCardd->setContext(context);
    ui->W_currentCardwd->setContext(context);
    ui->W_currentCardOnScreen->setContext(context);
    ui->W_currentCard_sp->setContext(context);
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
                        ui->W_currentCard->setCardId(card.id);
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
        ui->W_currentCardOnScreen->setCardId(ui->W_currentCard->getCardId());
        if (context.cardService().getCardById(ui->W_currentCard->getCardId()).value().designId.has_value()) {
            const QPixmap design1(QString::fromStdString(
                context.cardDesignService().getCardDesignById(
                    context.cardService().getCardById(ui->W_currentCard->getCardId()).value().designId.value()).
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

void MainWindow::on_B_cancelPin_clicked() {
    ui->L_accessDenied->hide();
    ui->LE_pin->clear();
    animateTransition(ui->pinScreen, ui->dashboardScreen);
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
        && newPin.size() == 4 && newPinConfirm.size() == 4) {
        const std::string &oldPinStr = oldPin.toStdString();
        const std::string &newPinStr = newPin.toStdString();
        const int id = ui->W_currentCard->getCardId();
        const bool success = context.cardService().changeCardPin(id, oldPinStr, newPinStr);
        if (success) {
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
        int cardId = ui->W_currentCardwd->getCardId();
        ui->W_currentCardwd->setCardId(cardId);
        ui->W_currentCard->setCardId(cardId);
        ui->W_currentCard_2->setCardId(cardId);
        ui->W_currentCard_3->setCardId(cardId);
        ui->W_currentCardOnScreen->setCardId(cardId);
        ui->W_currentCardd->setCardId(cardId);
        ui->W_currentCard_sp->setCardId(cardId);

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
    const int currentAmount = ui->L_amountDeposit->text().toInt() + 20;
    ui->L_amountDeposit->clear();
    ui->L_amountDeposit->setText(QString::fromStdString(std::to_string(currentAmount)));
    ui->B_cancelDeposit->setDisabled(true);
}

void MainWindow::on_B_D50_clicked() const {
    const int currentAmount = ui->L_amountDeposit->text().toInt() + 50;
    ui->L_amountDeposit->clear();
    ui->L_amountDeposit->setText(QString::fromStdString(std::to_string(currentAmount)));
    ui->B_cancelDeposit->setDisabled(true);
}

void MainWindow::on_B_D100_clicked() const {
    const int currentAmount = ui->L_amountDeposit->text().toInt() + 100;
    ui->L_amountDeposit->setDisabled(true);
    ui->L_amountDeposit->setText(QString::fromStdString(std::to_string(currentAmount)));
    ui->B_cancelDeposit->setDisabled(true);
}

void MainWindow::on_B_D200_clicked() const {
    const int currentAmount = ui->L_amountDeposit->text().toInt() + 200;
    ui->L_amountDeposit->clear();
    ui->L_amountDeposit->setText(QString::fromStdString(std::to_string(currentAmount)));
    ui->B_cancelDeposit->setDisabled(true);
}

void MainWindow::on_B_D500_clicked() const {
    const int currentAmount = ui->L_amountDeposit->text().toInt() + 500;
    ui->L_amountDeposit->clear();
    ui->L_amountDeposit->setText(QString::fromStdString(std::to_string(currentAmount)));
    ui->B_cancelDeposit->setDisabled(true);
}

void MainWindow::on_B_D1000_clicked() const {
    const int currentAmount = ui->L_amountDeposit->text().toInt() + 1000;
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
        int cardId = ui->W_currentCardd->getCardId();
        ui->W_currentCardd->setCardId(cardId);
        ui->W_currentCardwd->setCardId(cardId);
        ui->W_currentCard->setCardId(cardId);
        ui->W_currentCard_2->setCardId(cardId);
        ui->W_currentCard_3->setCardId(cardId);
        ui->W_currentCardOnScreen->setCardId(cardId);
        ui->W_currentCard_sp->setCardId(cardId);

        ui->B_cancelDeposit->setDisabled(false);
        ui->L_amountDeposit->clear();
        ui->L_amountDeposit->setText("0");
    } else {
        qDebug() << "Deposit FAILURE";
    }
}

void MainWindow::on_B_transactionHistory_clicked() {
    setupTransHistoryScreen();
    animateTransition(ui->cardScreen, ui->transHistoryScreen);
}

void MainWindow::on_B_backToCard_clicked() {
    QLayout *layout = ui->transHistoryContainer->layout();
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    animateTransition(ui->transHistoryScreen, ui->cardScreen);
}

void MainWindow::on_B_chooseDesign_clicked() {
    setupDesignsScreen();
    animateTransition(ui->cardScreen, ui->designsScreen);
}

void MainWindow::on_B_cancelDesign_clicked() {
    animateTransition(ui->designsScreen, ui->cardScreen);
}

void MainWindow::on_B_transfer_clicked() {
    setupTransferScreen();
    animateTransition(ui->cardScreen, ui->transferScreen);
}

void MainWindow::on_B_T100_clicked() const {
    ui->LE_enteredTransferAmount->clear();
    ui->LE_enteredTransferAmount->insert("100");
}

void MainWindow::on_B_T200_clicked() const {
    ui->LE_enteredTransferAmount->clear();
    ui->LE_enteredTransferAmount->insert("200");
}

void MainWindow::on_B_T500_clicked() const {
    ui->LE_enteredTransferAmount->clear();
    ui->LE_enteredTransferAmount->insert("500");
}

void MainWindow::on_B_T1000_clicked() const {
    ui->LE_enteredTransferAmount->clear();
    ui->LE_enteredTransferAmount->insert("1000");
}

void MainWindow::on_B_cancelTransfer_clicked() {
    ui->LE_enteredTransferAmount->clear();
    ui->LE_transferDest->clear();
    ui->L_failTransfer->hide();
    animateTransition(ui->transferScreen, ui->cardScreen);
}

void MainWindow::on_B_enterTransfer_clicked() const {
    QString dest = ui->LE_transferDest->text();
    dest.remove(' ');
    optional<Card> card = context.cardService().getCardByNumber(dest.toStdString());

    if (dest.size() < 16 || !card)
    {
        qDebug() << "Transaction FAILURE";
        ui->L_failTransfer->show();
        shakeLabel(ui->L_failTransfer);
        return;
    }

    const int enteredAmount = ui->LE_enteredTransferAmount->text().toInt();
    auto newTransaction = BankTransaction();
    newTransaction.type = "transfer";
    newTransaction.fromCardId = ui->W_currentCard_3->getCardId();
    newTransaction.toCardId = card.value().id;
    newTransaction.amount = enteredAmount;
    newTransaction.currencyCode = "UAH";
    newTransaction.description = "transfer";
    newTransaction.comment = "";
    newTransaction.status = "completed";

    if (context.bankTransactionService().createBankTransaction(newTransaction))
    {
        qDebug() << "Transaction SUCCESS";
        ui->L_failTransfer->hide();
        int cardId = ui->W_currentCard_3->getCardId();
        ui->W_currentCardwd->setCardId(cardId);
        ui->W_currentCard->setCardId(cardId);
        ui->W_currentCard_2->setCardId(cardId);
        ui->W_currentCard_3->setCardId(cardId);
        ui->W_currentCardOnScreen->setCardId(cardId);
        ui->W_currentCardd->setCardId(cardId);
        ui->W_currentCard_sp->setCardId(cardId);
        ui->LE_enteredAmount->clear();
    }
    else
    {
        qDebug() << "Transaction FAILURE";
        ui->L_failTransfer->show();
        shakeLabel(ui->L_failTransfer);
    }
}

void MainWindow::on_B_register_clicked() {
    setupRegisterScreen();
    animateTransition(ui->loginScreen, ui->registerScreen);
}

void MainWindow::on_B_cancelRegister_clicked() {
    ui->L_fillCorrectly->hide();
    ui->LE_phone->clear();
    ui->LE_password->clear();
    animateTransition(ui->registerScreen, ui->loginScreen);
}

void MainWindow::on_B_enterRegister_clicked() {
    const std::string regFirstName = ui->LE_firstName->text().toStdString();
    const std::string regLastName = ui->LE_lastName->text().toStdString();
    const std::string regEmail = ui->LE_email->text().toStdString();
    const std::string regPhone = ui->LE_phoneNumber->text().toStdString();
    const std::string regPassword = ui->LE_password_2->text().toStdString();

    if (regFirstName.empty() || regLastName.empty() || regPhone.empty() || regPassword.empty()) {
        ui->L_fillCorrectly->setText("Fields marked with * are required");
        ui->L_fillCorrectly->show();
        shakeLabel(ui->L_fillCorrectly);
        return;
    }

    if (context.userService().getUserByPhone(regPhone).has_value()) {
        ui->L_fillCorrectly->setText("User with this phone number is already registered");
        ui->L_fillCorrectly->show();
        shakeLabel(ui->L_fillCorrectly);
        return;
    }

    if (regPassword.size()<8) {
        ui->L_fillCorrectly->setText("Your password is too short (minimum 8 symbols required)");
        ui->L_fillCorrectly->show();
        shakeLabel(ui->L_fillCorrectly);
        return;
    }

    auto regUser = User();
    regUser.firstName = regFirstName;
    regUser.lastName = regLastName;
    regUser.phone = regPhone;
    regUser.passwordHash = regPassword;
    regUser.status = "active";
    if (!regEmail.empty()) {
        const std::regex pattern(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
        if (!std::regex_match(regEmail, pattern)) {
            ui->L_fillCorrectly->setText("Please enter a valid email (or erase completely, it is not required)");
            ui->L_fillCorrectly->show();
            shakeLabel(ui->L_fillCorrectly);
            return;
        }
        regUser.email = regEmail;
    }
    context.userService().createUser(regUser);

    ui->LE_firstName->clear();
    ui->LE_lastName->clear();
    ui->LE_email->clear();
    ui->LE_phoneNumber->clear();
    ui->LE_password_2->clear();

    ui->L_fillCorrectly->hide();

    animateTransition(ui->registerScreen, ui->thanksScreen);
}

void MainWindow::on_B_startWork_clicked() {
    ui->L_fillCorrectly->hide();
    ui->LE_phone->clear();
    ui->LE_password->clear();
    animateTransition(ui->thanksScreen, ui->loginScreen);
}

void MainWindow::on_B_regularPayment_clicked() {
    setupScheduledTransferScreen();
    animateTransition(ui->cardScreen, ui->scheduledTransferScreen);
}

void MainWindow::on_B_S100_clicked() const {
    ui->LE_enteredScheduledTransferAmount->clear();
    ui->LE_enteredScheduledTransferAmount->insert("100");
}

void MainWindow::on_B_S200_clicked() const {
    ui->LE_enteredScheduledTransferAmount->clear();
    ui->LE_enteredScheduledTransferAmount->insert("200");
}

void MainWindow::on_B_S500_clicked() const {
    ui->LE_enteredScheduledTransferAmount->clear();
    ui->LE_enteredScheduledTransferAmount->insert("500");
}

void MainWindow::on_B_S1000_clicked() const {
    ui->LE_enteredScheduledTransferAmount->clear();
    ui->LE_enteredScheduledTransferAmount->insert("1000");
}

void MainWindow::on_B_backScheduledTransfer_clicked() {
    ui->LE_enteredScheduledTransferAmount->clear();
    ui->LE_scheduledTransferDest->clear();
    ui->L_failScheduledTransfer->hide();
    animateTransition(ui->scheduledTransferScreen, ui->cardScreen);
}

void MainWindow::on_B_enterScheduledTransfer_clicked() const {
    QString dest = ui->LE_scheduledTransferDest->text();
    dest.remove(' ');
    optional<Card> card = context.cardService().getCardByNumber(dest.toStdString());

    if (dest.size() < 16 || !card)
    {
        qDebug() << "Scheduled payment FAILURE";
        ui->L_failScheduledTransfer->text().clear();
        ui->L_failScheduledTransfer->setText("Enter a valid card number");
        ui->L_failScheduledTransfer->show();
        shakeLabel(ui->L_failScheduledTransfer);
        return;
    }

    if (ui->LE_enteredScheduledTransferAmount->text().isEmpty())
    {
        qDebug() << "Scheduled payment FAILURE";
        ui->L_failScheduledTransfer->text().clear();
        ui->L_failScheduledTransfer->setText("Enter an amount");
        ui->L_failScheduledTransfer->show();
        shakeLabel(ui->L_failScheduledTransfer);
        return;
    }
    ui->L_failScheduledTransfer->text().clear();
    ui->L_failScheduledTransfer->setText("FAIL");

    const int enteredAmount = ui->LE_enteredScheduledTransferAmount->text().toInt();
    auto newTransaction = BankTransaction();
    newTransaction.type = "payment";
    newTransaction.fromCardId = ui->W_currentCard_sp->getCardId();
    newTransaction.toCardId = card.value().id;
    newTransaction.amount = enteredAmount;
    newTransaction.currencyCode = "UAH";
    newTransaction.description = "Sch payment";
    newTransaction.comment = "";
    newTransaction.status = "completed";

    if (context.bankTransactionService().createBankTransaction(newTransaction))
    {
        qDebug() << "Sch Transaction SUCCESS";
        auto newSchTransfer = ScheduledTransfer();
        newSchTransfer.fromCardId = ui->W_currentCard_sp->getCardId();
        newSchTransfer.toCardId = card.value().id;
        newSchTransfer.amount = enteredAmount;
        newSchTransfer.currencyCode = "UAH";
        newSchTransfer.description = "Sch payment";
        newSchTransfer.comment = "";
        newSchTransfer.active = 1;
        if (ui->RB_daily->isChecked()) {
            newSchTransfer.frequency = "daily";
        }
        if (ui->RB_weekly->isChecked()) {
            newSchTransfer.frequency = "weekly";
        }
        if (ui->RB_monthly->isChecked()) {
            newSchTransfer.frequency = "monthly";
        }
        const std::time_t nowTime = std::time(nullptr);
        std::tm now{};
        localtime_s(&now, &nowTime);
        newSchTransfer.nextTun = now;
        if (newSchTransfer.frequency=="daily") {
            std::time_t newT = std::mktime(&newSchTransfer.nextTun.value());
            newT += 1 * 24 * 60 * 60;
            localtime_s(&newSchTransfer.nextTun.value(), &newT);
        }
        if (newSchTransfer.frequency=="weekly") {
            std::time_t newT = std::mktime(&newSchTransfer.nextTun.value());
            newT += 7 * 24 * 60 * 60;
            localtime_s(&newSchTransfer.nextTun.value(), &newT);
        }
        if (newSchTransfer.frequency=="monthly") {
            newSchTransfer.nextTun.value().tm_mon += 1;
            std::mktime(&newSchTransfer.nextTun.value());
        }
        if (newSchTransfer.nextTun.has_value()) {
            std::cout << std::put_time(&newSchTransfer.nextTun.value(), "%Y-%m-%d %H:%M:%S") << std::endl;
        }

        context.scheduledTransferService().createScheduledTransfer(newSchTransfer);


        ui->L_failScheduledTransfer->hide();
        int cardId = ui->W_currentCard_sp->getCardId();
        for (ScheduledTransfer s :context.scheduledTransferService().getAllScheduledTransfersFromCardId(cardId)) {
            cout << s.id << " " << s.amount << " " << s.fromCardId << " " << s.toCardId << " " << s.currencyCode << " " << s.description << " " << s.frequency << " " << s.active << endl;
            if (s.nextTun.has_value()) {
                std::cout << std::put_time(&s.nextTun.value(), "%Y-%m-%d %H:%M:%S") << std::endl;
            }
        }


        ui->W_currentCardwd->setCardId(cardId);
        ui->W_currentCard->setCardId(cardId);
        ui->W_currentCard_2->setCardId(cardId);
        ui->W_currentCard_3->setCardId(cardId);
        ui->W_currentCardOnScreen->setCardId(cardId);
        ui->W_currentCardd->setCardId(cardId);
        ui->W_currentCard_sp->setCardId(cardId);

        ui->LE_enteredScheduledTransferAmount->clear();
    }
    else
    {
        qDebug() << "Sch Transaction FAILURE";
        ui->L_failScheduledTransfer->show();
        shakeLabel(ui->L_failScheduledTransfer);
    }
}

void MainWindow::setupPinScreen() {
    ui->LE_pin->clear();
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

void MainWindow::setupRegisterScreen() const {
    ui->LE_firstName->clear();
    ui->LE_lastName->clear();
    ui->LE_email->clear();
    ui->LE_phoneNumber->clear();
    ui->LE_password_2->clear();

    ui->LE_firstName->setValidator(
        new QRegularExpressionValidator(QRegularExpression("^[A-Za-zА-Яа-яЇїІіЄєҐґ'-]{2,20}$"), ui->LE_firstName));
    ui->LE_lastName->setValidator(
        new QRegularExpressionValidator(QRegularExpression("^[A-Za-zА-Яа-яЇїІіЄєҐґ'-]{2,20}$"), ui->LE_lastName));
    ui->LE_email->setValidator(
        new QRegularExpressionValidator(QRegularExpression("^[\\w.-]+@[\\w.-]+\\.[A-Za-z]{2,}$"), ui->LE_email));
    ui->LE_phoneNumber->setInputMask(R"(\+3\8\0 (99) 999-9999;_)");
    ui->LE_password_2->setValidator(new QRegularExpressionValidator(QRegularExpression("^.{6,}$"), ui->LE_password_2));

    ui->L_fillCorrectly->clear();
    ui->L_fillCorrectly->setText("FILL ALL THE FIELDS CORRECTLY, PLEASE");
    ui->L_fillCorrectly->hide();
}

void MainWindow::setupDesignsScreen() const {
    const Card currentCardD = context.cardService().getCardById(ui->W_currentCardOnScreen->getCardId()).value();
    ui->SA_designs->setWidgetResizable(false);
    auto *buttonGroup = new QButtonGroup(ui->GL_designs);
    buttonGroup->setExclusive(true);

    const auto designs = context.cardDesignService().getAllCardDesigns();
    int currentDesignId = 0;
    if (context.cardService().getCardById(currentCardD.id).has_value()) {
        if (context.cardService().getCardById(currentCardD.id).value().designId.has_value()) {
            currentDesignId = context.cardService().getCardById(currentCardD.id).value().designId.value();
        }
    }
    int totalRows = (designs.size() + 1) / 2;
    int rowHeight = 220;
    ui->scrollAreaWidgetContents->setMinimumHeight(totalRows * rowHeight);
    ui->scrollAreaWidgetContents->setMinimumWidth(2 * 220 + 100);
    int row = 0, col = 0;
    for (const auto &d: designs) {
        auto *btn = new QPushButton();
        btn->setIcon(QIcon(QString::fromStdString(d.imageRef)));
        btn->setIconSize(QSize(180, 116));
        btn->setMinimumSize(QSize(200, 130));
        btn->setCheckable(true);
        btn->setStyleSheet("QPushButton { border: 3px solid transparent; border-radius: 10px; }"
            "QPushButton:checked { border: 3px solid #eae3d8; }");
        auto *nameLabel = new QLabel(QString::fromStdString(d.name));
        auto *authorLabel = new QLabel("");
        if (d.author.has_value()) {
            authorLabel = new QLabel("by " + QString::fromStdString(d.author.value()));
        }
        nameLabel->setAlignment(Qt::AlignCenter);
        authorLabel->setAlignment(Qt::AlignCenter);
        btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        nameLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        authorLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        auto *vbox = new QVBoxLayout();
        vbox->addWidget(btn);
        vbox->addWidget(nameLabel);
        vbox->addWidget(authorLabel);
        auto *cardWidget = new QWidget();
        cardWidget->setFixedSize(210, 200);
        cardWidget->setLayout(vbox);
        ui->GL_designs->addWidget(cardWidget, row, col);
        buttonGroup->addButton(btn, d.id);
        if (d.id == currentDesignId) btn->setChecked(true);
        if (++col == 2) {
            col = 0;
            row++;
        }
        ui->scrollAreaWidgetContents->adjustSize();
    }
    connect(buttonGroup, &QButtonGroup::buttonClicked, this,
            [this, buttonGroup, currentCardD](QAbstractButton *button) {
                int id = buttonGroup->id(button);
                qDebug() << "chosen ID:" << id;
                auto cardOpt = context.cardService().getCardById(currentCardD.id);
                if (cardOpt.has_value()) {
                    auto card = cardOpt.value();
                    card.designId = id;
                    context.cardService().updateCard(card);
                    ui->W_currentCardd->setCardId(card.id);
                    ui->W_currentCardwd->setCardId(card.id);
                    ui->W_currentCard->setCardId(card.id);
                    ui->W_currentCard_2->setCardId(card.id);
                    ui->W_currentCard_sp->setCardId(card.id);
                    ui->W_currentCardOnScreen->setCardId(card.id);
                    if (context.cardService().getCardById(ui->W_currentCard->getCardId()).value().designId.
                        has_value()) {
                        const QPixmap design1(QString::fromStdString(
                            context.cardDesignService().getCardDesignById(
                                context.cardService().getCardById(ui->W_currentCard->getCardId()).value().designId.
                                value()).
                            value().imageRef));
                        ui->W_currentCardOnScreen->setDesignPixmap(design1);
                    } else {
                        ui->W_currentCardOnScreen->setDesignPixmap();
                    }
                }
            });

    connect(ui->B_noDesign, &QPushButton::clicked, this, [this, buttonGroup, currentCardD]() {
        buttonGroup->setExclusive(false);
        for (auto *b: buttonGroup->buttons()) b->setChecked(false);
        buttonGroup->setExclusive(true);
        qDebug() << "no design";
        auto cardOpt = context.cardService().getCardById(currentCardD.id);
        if (cardOpt.has_value()) {
            auto card = cardOpt.value();
            card.designId = std::nullopt;
            context.cardService().updateCard(card);
            ui->W_currentCardd->setCardId(card.id);
            ui->W_currentCardwd->setCardId(card.id);
            ui->W_currentCard->setCardId(card.id);
            ui->W_currentCard_2->setCardId(card.id);
            ui->W_currentCardOnScreen->setCardId(card.id);
            ui->W_currentCard_sp->setCardId(card.id);
            if (context.cardService().getCardById(ui->W_currentCard->getCardId()).value().designId.has_value()) {
                const QPixmap design1(QString::fromStdString(
                    context.cardDesignService().getCardDesignById(
                        context.cardService().getCardById(ui->W_currentCard->getCardId()).value().designId.value()).
                    value().imageRef));
                ui->W_currentCardOnScreen->setDesignPixmap(design1);
            } else {
                ui->W_currentCardOnScreen->setDesignPixmap();
            }
        }
    });
}


void MainWindow::setupWithdrawScreen() {
    ui->W_currentCardwd->setCardId(ui->W_currentCardOnScreen->getCardId());
    if (context.cardService().getCardById(ui->W_currentCardOnScreen->getCardId()).value().designId.has_value()) {
        const QPixmap design1(QString::fromStdString(
            context.cardDesignService().getCardDesignById(
                context.cardService().getCardById(ui->W_currentCardOnScreen->getCardId()).value().designId.value()).
            value().imageRef));
        ui->W_currentCardwd->setDesignPixmap(design1);
    } else {
        ui->W_currentCardwd->setDesignPixmap();
    }
    ui->LE_enteredAmount->clear();
    ui->LE_enteredAmount->setValidator(new QIntValidator(0, 10000, this));
    ui->L_failWithdrawal->hide();
}

void MainWindow::on_B_toCardList_clicked()
{
    animateTransition(ui->cardScreen, ui->dashboardScreen);
}

void MainWindow::on_B_deleteCard_clicked()
{
    int cardId = ui->W_currentCardOnScreen->getCardId();
    Card card = context.cardService().getCardById(cardId).value();
    card.status = Card::Status::deleted;
    context.cardService().updateCard(card);
    animateTransition(ui->cardScreen, ui->dashboardScreen);
}

void MainWindow::setupTransferScreen() {
    int cardId = ui->W_currentCardOnScreen->getCardId();
    Card card = context.cardService().getCardById(cardId).value();
    ui->W_currentCard_3->setCardId(cardId);
    if (card.designId.has_value()) {
        const QPixmap design(QString::fromStdString(
            context.cardDesignService().getCardDesignById(card.designId.value()).
            value().imageRef));
        ui->W_currentCard_3->setDesignPixmap(design);
    }
    else {
        ui->W_currentCard_3->setDesignPixmap();
    }
    ui->LE_enteredTransferAmount->clear();
    ui->LE_enteredTransferAmount->setValidator(new QIntValidator(0, 10000, this));
    ui->L_failTransfer->hide();
    ui->LE_transferDest->setInputMask(R"(9999 9999 9999 9999)");
}

void MainWindow::setupTransHistoryScreen() {
    int cardId = ui->W_currentCard->getCardId();
    QWidget *listWidget = new TransactionListWidget(context, cardId);
    ui->W_currentCard_2->setCardId(cardId);
    if (context.cardService().getCardById(cardId).value().designId.has_value()) {
        const QPixmap design(QString::fromStdString(
            context.cardDesignService()
            .getCardDesignById(context.cardService()
                .getCardById(cardId).value().designId.value()).
            value().imageRef));
        ui->W_currentCard_2->setDesignPixmap(design);
    } else {
        ui->W_currentCard_2->setDesignPixmap();
    }
    listWidget->setMouseTracking(true);
    ui->transHistoryContainer->layout()->addWidget(listWidget);
}

void MainWindow::setupDepositScreen() const {
    ui->W_currentCardd->setCardId(ui->W_currentCardOnScreen->getCardId());
    if (context.cardService().getCardById(ui->W_currentCardOnScreen->getCardId()).value().designId.has_value()) {
        const QPixmap design1(QString::fromStdString(
            context.cardDesignService().getCardDesignById(
                context.cardService().getCardById(ui->W_currentCardOnScreen->getCardId()).value().designId.value()).
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

void MainWindow::setupScheduledTransferScreen() {
    ui->RB_daily->setChecked(true);
    ui->RB_weekly->setChecked(false);
    ui->RB_monthly->setChecked(false);
    int cardId = ui->W_currentCardOnScreen->getCardId();
    Card card = context.cardService().getCardById(cardId).value();
    ui->W_currentCard_sp->setCardId(cardId);
    if (card.designId.has_value()) {
        const QPixmap design(QString::fromStdString(
            context.cardDesignService().getCardDesignById(card.designId.value()).
            value().imageRef));
        ui->W_currentCard_sp->setDesignPixmap(design);
    }
    else {
        ui->W_currentCard_sp->setDesignPixmap();
    }
    ui->LE_enteredScheduledTransferAmount->clear();
    ui->LE_enteredScheduledTransferAmount->setValidator(new QIntValidator(0, 10000, this));
    ui->L_failScheduledTransfer->text().clear();
    ui->L_failScheduledTransfer->setText("FAIL");
    ui->L_failScheduledTransfer->hide();
    ui->LE_scheduledTransferDest->setInputMask(R"(9999 9999 9999 9999)");
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
    ui->L_welcome_2->setObjectName("welcome2Label");
    ui->L_welcomeUser->setObjectName("welcomeUserLabel");
    ui->L_accessDenied->setObjectName("accessDeniedLabel");
    ui->L_cashWithdrawal->setObjectName("cashWithdrawalLabel");
    ui->L_failWithdrawal->setObjectName("failWithdrawalLabel");
    ui->L_failTransfer->setObjectName("failTransferLabel");
    ui->L_cashDepositing->setObjectName("cashDepositingLabel");
    ui->L_fillCorrectly->setObjectName("fillCorrectlyLabel");
    ui->L_join->setObjectName("joinLabel");


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
        QLabel#welcome2Label {
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
        QLabel#joinLabel {
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
        QLabel#fillCorrectlyLabel {
            color: #800000;
            font-size: 15px;
            font-weight: bold;
            qproperty-alignment: 'AlignCenter';
        }
        QLabel#failTransferLabel {
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
    if (context.cardDesignService().getAllCardDesigns().size() > 0) {
        return;
    }
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

    CardDesign newDesign2;
    newDesign2.name = "Blurred wheel";
    newDesign2.imageRef = ":/designs/resources/designs/blurred_wheel.jpg";
    context.cardDesignService().createCardDesign(newDesign2);

    CardDesign newDesign3;
    newDesign2.name = "KMA";
    newDesign2.imageRef = ":/designs/resources/designs/kma.jpg";
    context.cardDesignService().createCardDesign(newDesign2);
    /*
        CardDesign newDesign4;
        newDesign2.name = "Floor pattern";
        newDesign2.imageRef = ":/designs/resources/designs/floor.jpg";
        context.cardDesignService().createCardDesign(newDesign2);*/
}
