#pragma once

#include <string>
#include <unordered_map>
#include <variant>

namespace phyl{

	using OptionsType = std::variant<int, float, double, std::string, bool>;

	class Options{
		public:
			Options();
			Options(const std::string &fp);
			
			void loadOptions(const std::string &fp);

			bool hasOption(const std::string &name) const;

			std::string getString(const std::string &name, const std::string &def) const;
			void setString(const std::string &name, const std::string &def);

			int getInt(const std::string &name, const int def) const;
			void setInt(const std::string &name, const int def);

			bool getBool(const std::string &name, const bool def) const;
			void setBool(const std::string &name, const bool def);

			float getFloat(const std::string &name, const float def) const;
			void setFloat(const std::string &name, const float def);

			double getDouble(const std::string &name, const double def) const;
			void setDouble(const std::string &name, const double def);

		private:
			std::unordered_map<std::string, OptionsType> options;
	};
}
