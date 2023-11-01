#pragma once

class GUI {
public:
	void render() noexcept;

	bool is_open{ true };
	std::once_flag set_font_scale;
private:
	char str_buffer[256];
};
