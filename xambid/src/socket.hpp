#pragma once
#include <filesystem>
#include <vector>
#include <xambi/utils.hpp>

class xambi_server
{
public:
	virtual void update_connections() = 0;
	virtual void broadcast(const char *data, size_t length) = 0;
	virtual ~xambi_server() = default;
};

class socket_server_connection : public xambi::no_copy
{
public:
	socket_server_connection(int socket, int client);
	socket_server_connection(const socket_server_connection&) = delete;
	socket_server_connection(socket_server_connection&&) noexcept;
	socket_server_connection &operator=(const socket_server_connection&) = delete;
	socket_server_connection &operator=(socket_server_connection&&) noexcept;
	~socket_server_connection();

	bool send(const char *data, size_t length);
	bool is_buffer_empty() const {return m_buffer.empty();}
	bool is_broken() const {return m_broken;}

private:
	int m_socket;
	int m_client;
	bool m_broken;
	std::vector<char> m_buffer;
};

class socket_server : public xambi_server, public xambi::no_copy, public xambi::no_move
{
public:
	socket_server(int socket_fd, int max_conns);
	~socket_server() override;

	void update_connections() override;
	void broadcast(const char *data, size_t length) override;

protected:
	void accept_connection();
	void listen();

	int m_socket_fd;
	int m_max_conns;
	std::vector<socket_server_connection> m_connections;
};

class unix_socket_server : public socket_server
{
public:
	unix_socket_server(const std::filesystem::path &path, int max_conns = 64);
	~unix_socket_server() override;

private:
	std::filesystem::path m_path;
};

class tcp_server : public socket_server
{
public:
	tcp_server(int port, int max_conns = 64);

private:
	int m_port;
};