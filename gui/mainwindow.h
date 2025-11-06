#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    Ui::MainWindow *ui;
    std::map<std::string, std::string> users;
    IContext& context;
    bool authenticate(const std::string& phone, const std::string& password) const;
    void animateTransition(QWidget* from, QWidget* to, int initY = 0, std::function<void()> onFinished = nullptr);
    void setStyles() const;

};

#endif // MAINWINDOW_H
