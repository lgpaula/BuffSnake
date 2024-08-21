#include <boost/asio.hpp>
#include <iostream>
#include <utility>
#include <vector>
#include <memory>

using boost::asio::ip::tcp;

class LanServer {
public:
    LanServer(boost::asio::io_context& io_context, short port)
            : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), client_count_(0) {
        std::cout << "created LanServer" << std::endl;
        startAccept();
    }

    [[nodiscard]] int getClientCount() const { return client_count_; }

private:
    void startAccept() {
        acceptor_.async_accept([this](const boost::system::error_code& error, tcp::socket socket) {
            if (!error) {
                auto new_socket = std::make_shared<tcp::socket>(std::move(socket));

                client_count_++;
                clients_.push_back(new_socket);
                std::cout << "Client " << client_count_ << " connected." << std::endl;

                // Send client ID
                std::string id_message = "ID:" + std::to_string(client_count_) + "\n";
                boost::asio::async_write(*new_socket, boost::asio::buffer(id_message),
                                         [](const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/) {});

                if (clients_.size() == 4) {
                    startGame();
                } else {
                    startAccept();
                }
            }
        });
    }

    void startGame() {
        std::cout << "All clients connected. Game is starting!" << std::endl;
        // Notify all clients that the game is starting
        for (auto& client : clients_) {
            boost::asio::async_write(*client, boost::asio::buffer("Game Start\n"),
                                     [](const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/) {});
        }
    }

    tcp::acceptor acceptor_;
    std::vector<std::shared_ptr<tcp::socket>> clients_{};
    int client_count_{};
};

class LanClient {
public:
    LanClient(std::string host, int port) : host(std::move(host)), port(port) {
        startClient();
    }

private:
    void startClient() {
        try {
            boost::asio::io_context io_context;
            tcp::socket socket(io_context);
            tcp::resolver resolver(io_context);
            auto endpoints = resolver.resolve(host, std::to_string(port));
            boost::asio::connect(socket, endpoints);

            std::cout << "Connected to the server!" << std::endl;

            boost::asio::streambuf buf;
            boost::asio::read_until(socket, buf, "\n");
            std::istream input(&buf);
            std::string message;
            std::getline(input, message);

            if (message.find("ID:") == 0) {
                std::string client_id = message.substr(3);
                std::cout << "Assigned Client ID: " << client_id << std::endl;
            }

            boost::asio::read_until(socket, buf, "\n");
            std::getline(input, message);

            if (message == "Game Start") {
                std::cout << "Received message: " << message << std::endl;
            }
        } catch (std::exception &e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }

    const std::string host;
    int port;
};