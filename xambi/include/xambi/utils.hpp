#pragma once

struct no_copy
{
	no_copy() = default;
	no_copy(const no_copy &) = delete;
	no_copy(no_copy &&) noexcept = default;
	no_copy &operator=(const no_copy &) = delete;
	no_copy &operator=(no_copy &&) = default;
	~no_copy() = default;
};

struct no_move
{
	no_move() = default;
	no_move(const no_copy &) = delete;
	no_move(no_copy &&) noexcept = delete;
	no_move &operator=(const no_move &) = default;
	no_move &operator=(no_move &&) = delete;
	~no_move() = default;
};
