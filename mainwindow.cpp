#include "mainwindow.h"
#include "ui_mainwindow.h"

const static std::string MAIN_API = "ip-api.com";
const static std::string API_ARGUMENTS = "/json/";
namespace http = boost::beast::http;


// определить на какой ОС запускается прога
// открытие браузера с геолокацией
// функция форматирования вывода

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->send, SIGNAL(clicked()), this, SLOT(Check()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Check() {
    ui->info->clear();
    bool isAddress = false;
    std::string ip_input = ui->ip_input->text().toStdString();
    std::string res_str = getResponse(ip_input);
    std::string reserve;
    QString formatted;
    double lat, lon;

    if (ui->continent_check->isChecked())
        ui->info->setText(ui->info->text() + QString::fromStdString(getFieldFromJson(res_str, "continent")));
    if (ui->country_check->isChecked())
        ui->info->setText(ui->info->text() + QString::fromStdString(getFieldFromJson(res_str, "country")));
    if (ui->region_check->isChecked())
        ui->info->setText(ui->info->text() + QString::fromStdString(getFieldFromJson(res_str, "regionName")));
    if (ui->city_check->isChecked())
        ui->info->setText(ui->info->text() + QString::fromStdString(getFieldFromJson(res_str, "city")));
    if (ui->lat_check->isChecked()) {
        reserve = getFieldFromJson(res_str, "lat");
        formatted = FormatOutput();
        ui->info->setText(ui->info->text() + QString::fromStdString(reserve));
        lat = std::stod(getFieldFromJson(res_str, "lat"));
    }
    if (ui->lon_check->isChecked()) {
        reserve = getFieldFromJson(res_str, "lon");
        ui->info->setText(ui->info->text() + QString::fromStdString(reserve));
        lon = std::stod(getFieldFromJson(res_str, "lon"));
    }
    if (ui->timezone_check->isChecked())
        ui->info->setText(ui->info->text() + QString::fromStdString(getFieldFromJson(res_str, "timezone")));
    if (ui->name_check->isChecked())
        ui->info->setText(ui->info->text() + QString::fromStdString(getFieldFromJson(res_str, "asname")));
    if (isAddress)
        OpenBrowser(lat, lon);
}

std::string MainWindow::getResponse(std::string ip) {
    boost::asio::io_context io;
    boost::asio::ip::tcp::resolver resolver(io);
    boost::asio::ip::tcp::socket socket(io);

    boost::asio::connect(socket, resolver.resolve(MAIN_API, "80"));
    std::string argument = API_ARGUMENTS + ip;

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

std::string MainWindow::getFieldFromJson(std::string json, std::string field) {
    std::stringstream jsonEncoded(json);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(jsonEncoded, root);

    boost::optional<boost::property_tree::ptree&> cont_node = root.get_child_optional(field);
    if (cont_node)
        return field + ": " + root.get<std::string>(field) + '\n';
    else return field + ": null\n";
}

void MainWindow::OpenBrowser(double lat, double lon) {
    std::string url = "https://geotree.ru/coordinates?lat=" + std::to_string(lat) + "&lon=" + std::to_string(lon) + "&z=15&c=";
    std::string command = "open " + url; // для macOS

    system(command.c_str());
}

QString MainWindow::FormatOutput() {

}
