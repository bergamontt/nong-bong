#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_B_enter_clicked();
    void on_B_logout_clicked();

private:
    Ui::MainWindow *ui;
    std::map<std::string, std::string> users;
    bool authenticate(const std::string&, const std::string&);
    void animateTransition(QWidget* from, QWidget* to);
    void setStyles() const;

};

#endif // MAINWINDOW_H
