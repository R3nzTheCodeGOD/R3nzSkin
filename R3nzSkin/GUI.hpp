#pragma once

class GUI {
public:
	void render() noexcept;

	bool is_open{ true };
private:
	char str_buffer[256];
};
