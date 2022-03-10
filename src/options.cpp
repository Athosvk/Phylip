#include "options.hpp"

namespace phyl {
	Options::Options() {
		options.clear();
	}

	bool Options::hasOption(const std::string &name) const {
		return options.find(name) != options.end();
	}

	std::string Options::getString(const std::string &name, const std::string &def) const {
		auto val = options.find(name);
		if(val != options.end()){
			return std::get<std::string>(val->second);
		} else {
			return def;
		}
	}

	void Options::setString(const std::string &name, const std::string &def) {
		options[name] = def;
	}

	int Options::getInt(const std::string &name, const int def) const {
		auto val = options.find(name);
		if(val != options.end()){
			return std::get<int>(val->second);
		} else {
			return def;
		}
	}

	void Options::setInt(const std::string &name, const int def) {
		options[name] = def;
	}

	float Options::getFloat(const std::string &name, const float def) const {
		auto val = options.find(name);
		if(val != options.end()){
			return std::get<float>(val->second);
		} else {
			return def;
		}
	}
	void Options::setFloat(const std::string &name, const float def) {
		options[name] = def;
	}

	double Options::getDouble(const std::string &name, const double def) const {
		auto val = options.find(name);
		if(val != options.end()){
			return std::get<double>(val->second);
		} else {
			return def;
		}
	}

	void Options::setDouble(const std::string &name, const double def) {
		options[name] = def;
	}
};
