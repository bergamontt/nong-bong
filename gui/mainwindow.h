#pragma once
#include <QLabel>
#include <QMainWindow>
#include <context/AppContext.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(IContext& context, QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_B_enter_clicked();
    void on_B_logout_clicked();
    void on_B_enterPin_clicked();
    void on_B_cancelPin_clicked();

    void on_B_changePin_clicked();
    void on_B_enterNewPin_clicked();
    void on_B_cancelChange_clicked();

    void on_B_transactionHistory_clicked();
    void on_B_backToCard_clicked();

    void on_B_withdraw_clicked();
    void on_B_100_clicked() const;
    void on_B_200_clicked() const;
    void on_B_500_clicked() const;
    void on_B_1000_clicked() const;
    void on_B_cancelWithdraw_clicked();
    void on_B_enterWithdraw_clicked() const;

    void on_B_deposit_clicked();
    void on_B_D20_clicked() const;
    void on_B_D50_clicked() const;
    void on_B_D100_clicked() const;
    void on_B_D200_clicked() const;
    void on_B_D500_clicked() const;
    void on_B_D1000_clicked() const;
    void on_B_cancelDeposit_clicked();
    void on_B_enterDeposit_clicked() const;

    void on_B_transfer_clicked();
    void on_B_T100_clicked() const;
    void on_B_T200_clicked() const;
    void on_B_T500_clicked() const;
    void on_B_T1000_clicked() const;
    void on_B_cancelTransfer_clicked();
    void on_B_enterTransfer_clicked() const;

    void on_B_toCardList_clicked();

    void on_B_chooseDesign_clicked();
    void on_B_cancelDesign_clicked();

    void on_B_register_clicked();
    void on_B_cancelRegister_clicked();
    void on_B_enterRegister_clicked();

    void on_B_startWork_clicked();

private:
    Ui::MainWindow *ui;
    std::map<std::string, std::string> users;
    IContext& context;
    bool authenticate(const std::string& phone, const std::string& password) const;
    void animateTransition(QWidget* from, QWidget* to, int initY = 0, std::function<void()> onFinished = nullptr);

    static void shakeLabel(QLabel* label);
    void initDesigns();

    void setStyles() const;
    void setupPinScreen();
    void setupPinChangeScreen();
    void setupWithdrawScreen();
    void setupTransHistoryScreen();
    void setupDepositScreen() const;
    void setupDesignsScreen() const;
    void setupTransferScreen();
    void setupRegisterScreen() const;

};
