#include "mainwindow.h"
#include "ui_mainwindow.h"

const std::string MAIN_API = "ip-api.com";
const std::string API_ARGUMENTS = "/json/";
namespace http = boost::beast::http;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(ui->send, SIGNAL(clicked()), this, SLOT(Check()));
  connect(ui->ip_input, SIGNAL(returnPressed()), this, SLOT(Check()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::Check() noexcept {
  ui->info->clear();
  const std::string ip_input = ui->ip_input->text().toStdString();
  const std::string res_str = getResponse(ip_input);
  std::string reserve = "";
  double lat = 0, lon = 0;

  if (ui->continent_check->isChecked())
    FormatOutput(getFieldFromJson(res_str, "continent"), "continent");
  if (ui->country_check->isChecked())
    FormatOutput(getFieldFromJson(res_str, "country"), "country");
  if (ui->region_check->isChecked())
    FormatOutput(getFieldFromJson(res_str, "region"), "region");
  if (ui->city_check->isChecked())
    FormatOutput(getFieldFromJson(res_str, "city"), "city");
  if (ui->lat_check->isChecked()) {
    reserve = getFieldFromJson(res_str, "lat");
    if (reserve != "") {
      lat = std::stod(reserve);
    }
    FormatOutput(reserve, "lat");
  }
  if (ui->lon_check->isChecked()) {
    reserve = getFieldFromJson(res_str, "lon");
    if (reserve != "") {
      lon = std::stod(reserve);
    }
    FormatOutput(reserve, "lon");
  }
  if (ui->timezone_check->isChecked())
    FormatOutput(getFieldFromJson(res_str, "timezone"), "timezone");
  if (ui->name_check->isChecked())
    FormatOutput(getFieldFromJson(res_str, "asname"), "asname");

  if (lat != 0.0 && lon != 0.0)
    OpenBrowser(lat, lon);
}

std::string MainWindow::getResponse(std::string ip) noexcept {
  boost::asio::io_context io;
  boost::asio::ip::tcp::resolver resolver(io);
  boost::asio::ip::tcp::socket socket(io);

  boost::asio::connect(socket, resolver.resolve(MAIN_API, "80"));
  const std::string argument = API_ARGUMENTS + ip;

  http::request<http::string_body> req(http::verb::get, argument, 11);

  req.set(http::field::host, MAIN_API);
  req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  http::write(socket, req);

  std::string response;
  {
    boost::beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(socket, buffer, res);
    response = boost::beast::buffers_to_string(res.body().data());
  }
  socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);

  return response;
}

std::string MainWindow::getFieldFromJson(std::string json,
                                         std::string field) noexcept {
  std::stringstream jsonEncoded(json);
  boost::property_tree::ptree root;
  boost::property_tree::read_json(jsonEncoded, root);
  boost::optional<boost::property_tree::ptree &> cont_node =
      root.get_child_optional(field);

  if (cont_node)
    return root.get<std::string>(field);
  else
    return "";
}

void MainWindow::OpenBrowser(double lat, double lon) noexcept {
  const QString qurl = QString::fromStdString(
      "https://geotree.ru/coordinates?lat=" + std::to_string(lat) +
      "&lon=" + std::to_string(lon) + "&z=15&c=100,17.2");
  const QUrl url(qurl);
  QDesktopServices::openUrl(url);
}

void MainWindow::FormatOutput(std::string field, std::string name) noexcept {
  if (field == "")
    ui->info->setText(ui->info->text() + QString::fromStdString(name) +
                      ": null" + '\n');
  else
    ui->info->setText(ui->info->text() + QString::fromStdString(name) + ": " +
                      QString::fromStdString(field) + '\n');
}
