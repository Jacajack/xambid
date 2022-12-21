#pragma once
#include <filesystem>
#include <vector>
#include "utils.hpp"

class socket_server
{
public:
	virtual void update_connections() = 0;
	virtual void broadcast(uint8_t *data, size_t length) = 0;
};

class unix_socket_server : public socket_server, public no_copy, public no_move
{
public:
	unix_socket_server(const std::filesystem::path &path, int max_conns = 64);

	void update_connections() override;
	void broadcast(uint8_t *data, size_t length) override;

private:
	std::filesystem::path m_path;
	int m_socket_fd;
	std::vector<int> m_clients;
}
