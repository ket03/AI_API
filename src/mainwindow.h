#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDesktopServices>
#include <QMainWindow>

#include <string>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  std::string getResponse(std::string ip) noexcept;
  std::string getFieldFromJson(std::string json, std::string field) noexcept;
  void OpenBrowser(double lat, double lon) noexcept;
  void FormatOutput(std::string field, std::string name) noexcept;

private slots:
  void Check() noexcept;
};
#endif // MAINWINDOW_H
