#include "socket.hpp"

unix_socket_server::unix_socket_server(const std::filesystem::path &path, int max_conns) :
	m_path(path)
{
	m_socket_fd = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0);
	assert(m_socket_fd != -1);

	int status;
	status = setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
	assert(status == 0 && "setsockopt() failed");

	struct sockaddr_un unconf{};
	unconf.sun_family = AF_UNIX;
	std::strncpy(unconf.sun_path, path.u8string().c_str(), sizeof(unconf.sun_path));

	status = bind(
		m_socket_fd,
		reinterpret_cast<struct sockaddr*>(&unconf)
		sizeof(unconf)
	);
	assert(status == 0 && "bind() failed");

	status = listen(m_socket_fd, max_conns);
	assert(status == 0 && "listen() failed");
}

