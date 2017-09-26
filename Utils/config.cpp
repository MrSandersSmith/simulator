#include <config.h>

namespace Simulator {

	Config::Config() {

	}

	Config::~Config() {

	}

	Config& Config::set_window_width(unsigned int width) {
		windowWidth = width;
		return *this;
	}

	Config& Config::set_window_height(unsigned int height) {
		windowHeight = height;
		return *this;
	}

	unsigned int Config::get_window_width() {
		return windowWidth;
	}

	unsigned int Config::get_window_height() {
		return windowHeight;
	}
}