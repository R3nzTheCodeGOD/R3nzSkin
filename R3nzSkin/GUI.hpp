#pragma once

class GUI {
public:
	void render() noexcept;

	bool is_open{ true };
	char nickBuffer[16];
private:
	char str_buffer[256];
};
