#pragma once
#include <filesystem>
#include <vector>
#include <xambi/utils.hpp>

class socket_server
{
public:
	virtual void update_connections() = 0;
	virtual void broadcast(const char *data, size_t length) = 0;
	virtual ~socket_server() = default;
};

class unix_socket_server_connection : public no_copy
{
public:
	unix_socket_server_connection(int socket, int client);
	unix_socket_server_connection(const unix_socket_server_connection&) = delete;
	unix_socket_server_connection(unix_socket_server_connection&&) noexcept;
	unix_socket_server_connection &operator=(const unix_socket_server_connection&) = delete;
	unix_socket_server_connection &operator=(unix_socket_server_connection&&) noexcept;
	~unix_socket_server_connection();

	bool send(const char *data, size_t length);
	bool is_buffer_empty() const {return m_buffer.empty();}
	bool is_broken() const {return m_broken;}

private:
	int m_socket;
	int m_client;
	bool m_broken;
	std::vector<char> m_buffer;
};

class unix_socket_server : public socket_server, public no_copy, public no_move
{
public:
	unix_socket_server(const std::filesystem::path &path, int max_conns = 64);
	~unix_socket_server() override;

	void update_connections() override;
	void broadcast(const char *data, size_t length) override;

private:
	void accept_connection();

	std::filesystem::path m_path;
	int m_socket_fd;
	std::vector<unix_socket_server_connection> m_connections;
};