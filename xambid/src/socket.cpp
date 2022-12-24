#include "socket.hpp"
#include <asm-generic/errno.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iterator>
#include <utility>
#include <algorithm>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

socket_server_connection::socket_server_connection(int socket, int client) :
	m_socket(socket),
	m_client(client),
	m_broken(false)
{
}

socket_server_connection::socket_server_connection(socket_server_connection &&src) noexcept :
	m_socket(src.m_socket),
	m_client(std::exchange(src.m_client, 0)),
	m_broken(src.m_broken),
	m_buffer(std::move(src.m_buffer))
{
}

socket_server_connection &socket_server_connection::operator=(socket_server_connection &&rhs) noexcept
{
	if (this == &rhs) return *this;
	m_socket = rhs.m_socket;
	m_client = std::exchange(rhs.m_client, 0);
	m_broken = rhs.m_broken;
	m_buffer = std::move(rhs.m_buffer);
	return *this;
}

socket_server_connection::~socket_server_connection()
{
	std::cerr << "closing conn " << m_client << std::endl;
	if (!m_client) return;

	int status;
	status = close(m_client);
	assert(status != -1 && "close() failed on connection");
}

bool socket_server_connection::send(const char *data, size_t length)
{
	if (is_broken())
		return false;

	// Buffer new data
	auto prev_buffer_size = m_buffer.size();
	m_buffer.resize(m_buffer.size() + length);
	std::copy(
		data,
		data + length,
		m_buffer.data() + prev_buffer_size
	);

	// Attempt to send the entire buffer
	auto sent_size = ::send(
		m_client,
		m_buffer.data(),
		m_buffer.size(),
		MSG_DONTWAIT | MSG_NOSIGNAL
	);

	// No data sent
	if (sent_size == -1)
	{
		perror("no chujowo");

		if (errno == EPIPE || errno == ECONNRESET)
		{
			m_buffer.clear();
			m_broken = true;
			return false;
		}
		else if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// Silently ignore
		}
		else
		{
			assert(false && "fatal send() error");
			return false;
		}

		sent_size = 0;
	}
	
	assert(sent_size <= m_buffer.size());
	m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sent_size);
	
	if (m_buffer.size() < m_buffer.capacity() / 10)
		m_buffer.shrink_to_fit();

	return true;
}

socket_server::socket_server(int socket_fd, int max_conns) :
	m_socket_fd(socket_fd),
	m_max_conns(max_conns)
{
	assert(m_socket_fd != -1);

	int status;
	int value = 1;
	status = setsockopt(
		m_socket_fd,
		SOL_SOCKET,
		SO_REUSEADDR,
		&value,
		sizeof(int)
	);
	assert(status == 0 && "setsockopt() failed");
}

socket_server::~socket_server()
{
	int status;
	status = close(m_socket_fd);
	assert(status != -1 && "close() failed on socket");
}

void socket_server::listen()
{
	int status = ::listen(m_socket_fd, m_max_conns);
	assert(status == 0 && "listen() failed");
}

unix_socket_server::unix_socket_server(const std::filesystem::path &path, int max_conns) :
	socket_server(::socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0), max_conns)
{
	struct sockaddr_un unconf{};
	unconf.sun_family = AF_UNIX;
	std::strncpy(unconf.sun_path, path.u8string().c_str(), sizeof(unconf.sun_path));

	int status = bind(
		m_socket_fd,
		reinterpret_cast<struct sockaddr*>(&unconf),
		sizeof(unconf)
	);
	assert(status == 0 && "bind() failed");

	socket_server::listen();
}

unix_socket_server::~unix_socket_server()
{
	int status = unlink(m_path.u8string().c_str());
	assert(status != -1 && "unlink() failed on socket");
}

void socket_server::update_connections()
{
	accept_connection();

	m_connections.erase(
		std::remove_if(
			m_connections.begin(),
			m_connections.end(),
			[](const socket_server_connection &conn) {
				return conn.is_broken();
			}
		),
		m_connections.end()
	);
}

void socket_server::broadcast(const char *data, size_t length)
{
	// Note: send is only called if the buffer is empty
	for (auto &conn : m_connections)
		if (conn.is_buffer_empty() && !conn.is_broken())
			conn.send(data, length);
}

void socket_server::accept_connection()
{
	struct sockaddr_un unconf{};
	socklen_t unconf_size = sizeof(unconf);

	int client = accept(
		m_socket_fd,
		reinterpret_cast<struct sockaddr*>(&unconf),
		&unconf_size
	);

	if (client == -1)
	{
		assert(errno == EAGAIN || errno != EWOULDBLOCK);
		return;
	}

	m_connections.emplace_back(m_socket_fd, client);
	std::cerr << "new connection" << std::endl;
}

tcp_server::tcp_server(int port, int max_conns) :
	socket_server(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0), max_conns),
	m_port(port)
{
	struct sockaddr_in inconf{};
	inconf.sin_port = htons(m_port);
	inconf.sin_addr.s_addr = INADDR_ANY;
	inconf.sin_family = AF_INET;

	int status = bind(
		m_socket_fd,
		reinterpret_cast<struct sockaddr*>(&inconf),
		sizeof(inconf)
	);

	assert(status != -1 && "TCP bind() failed!");

	socket_server::listen();
}