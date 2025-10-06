#include "includes/Server.hpp"

volatile sig_atomic_t g_shutdown = 0;

void signalC(int signal) {
    (void)signal;
    if (DEBUG_MODE)
        std::cout << std::endl << RED << "Ctrl+C signal recieved" << RESET << std::endl;
    std::cout << std::endl << RED << "Exit" << RESET << std::endl;
    g_shutdown = 1;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << RED << "Usage : ./ircserv <port> <password>" << RESET << std::endl;
        return 1;
    }

	std::string port = argv[1];
	std::string password = argv[2];

    for (size_t i = 0; i < password.size(); ++i) {
        if (isForbidden(password[i])) {
            std::cerr << RED << "Forbidden character in password" << RESET << std::endl;
            return 1;
        }
    }

	// Ctrl^C
    signal(SIGINT, signalC);

    try {
        Server server(port, password);

        // On utilise un vecteur pour poll
        std::vector<struct pollfd> fds;

        std::cout << asciiArt() << std::endl;

        while (!g_shutdown) {
            // On reconstruit le vecteur a chaque tour de boucle
            fds.clear();

            // Ajouter le socket d'ecoute du serveur
            struct pollfd server_pollfd;
            server_pollfd.fd = server.getSockFd();
            server_pollfd.events = POLLIN; // On veut savoir quand un client arrive
            server_pollfd.revents = 0;
            fds.push_back(server_pollfd);

            // Ajouter l'entree standard (clavier du terminal)
            struct pollfd stdin_pollfd;
            stdin_pollfd.fd = STDIN_FILENO;
            stdin_pollfd.events = POLLIN;
            stdin_pollfd.revents = 0;
            fds.push_back(stdin_pollfd);

            // Ajouter TOUS les sockets des clients connectes
            const std::vector<Client*>& clients = server.getClients();
            for (size_t i = 0; i < clients.size(); ++i) {
                struct pollfd client_pollfd;
                client_pollfd.fd = clients[i]->getFd();
                client_pollfd.events = POLLIN; // On veut savoir quand un client parle
                client_pollfd.revents = 0;
                fds.push_back(client_pollfd);
            }

            // On appelle poll() pour attendre un evenement
            int ret = poll(fds.data(), fds.size(), 100); // Timeout de 100ms

            if (ret < 0) {
                if (errno == EINTR)
					continue; // Interrompu par un signal, on continue
                std::cerr << RED << "Error with poll(): " << strerror(errno) << RESET << std::endl;
                break;
            }

            if (ret == 0)
				continue;

            // Evenement sur le socket du serveur = un nouveau client veut se connecter
            if (fds[0].revents & POLLIN)
                server.handleNewClient();

            // Ctrl^D
            if (fds[1].revents & POLLIN) {
                char c;
                if (read(STDIN_FILENO, &c, 1) <= 0) {
                    std::cout << RED << "Exit" << RESET << std::endl;
                    g_shutdown = 1;
                }
            }

            // Evenement sur un des clients = message recu ou deconnexion
            // On boucle a partir de fds[2] (0: serveur, 1: clavier, >=2: clients)
            for (size_t i = 2; i < fds.size(); ++i) {
                if (fds[i].revents & POLLIN)
                    server.handleClientMessage(fds[i].fd);
            }
        }
        server.closeServer();

	} catch (Server::ConnectionFailure &e) {
		std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
	} catch (Server::WrongPort &e) {
		std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
	} catch (Channel::ChannelError &e) {
		std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
	} catch (const std::exception& e) {
        std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
    }
    return 0;
}
