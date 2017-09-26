#ifndef CONFIG_H_
#define CONFIG_H_

namespace Simulator {

	class Config {
	public:
		Config();
		~Config();
		Config& set_window_width(unsigned int width);
		Config& set_window_height(unsigned int height);
		unsigned int get_window_width();
		unsigned int get_window_height();

	private:
		unsigned int windowWidth;
		unsigned int windowHeight;
	};
}

#endif