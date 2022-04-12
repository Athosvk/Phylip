#include "options.hpp"
#include "yaml-cpp/yaml.h"

#include <iostream>

namespace phyl {

	Options::Options(const std::string &fp) {
		loadOptions(fp);
	}

	void Options::loadOptions(const std::string &fp) {
		YAML::Node config = YAML::LoadFile(fp);
		for (YAML::const_iterator it = config.begin(); it != config.end(); ++it){
			std::string name = it->first.as<std::string>();
			std::string type = it->second["type"].as<std::string>();
			if(type == "int") {
				this->setInt(name, it->second["value"].as<int>());
			} else if(type == "float") {
				this->setFloat(name, it->second["value"].as<float>());
			} else if(type == "double") {
				this->setDouble(name, it->second["value"].as<double>());
			} else if(type == "string") {
				this->setString(name, it->second["value"].as<std::string>());
			} else if(type == "bool") {
				this->setBool(name, it->second["value"].as<bool>());
			}
		}
	}

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

	bool Options::getBool(const std::string &name, const bool def) const {
		auto val = options.find(name);
		if(val != options.end()){
			return std::get<bool>(val->second);
		} else {
			return def;
		}
	}
	void Options::setBool(const std::string &name, const bool def) {
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
