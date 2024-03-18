#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <string>
#include <cstdlib>

#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

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

private:
    Ui::MainWindow *ui;
    std::string getResponse(std::string ip);
    std::string getFieldFromJson(std::string json, std::string field);
    void OpenBrowser(double lat, double lon);
    void FormatOutput(std::string field, std::string name);
    std::string getOS();

private slots:
    void Check();
};
#endif // MAINWINDOW_H
