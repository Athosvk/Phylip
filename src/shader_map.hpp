#pragma once

#include "shader.hpp"

#include <algorithm>
#include <unordered_map>
#include <memory>

namespace phyl{
typedef std::shared_ptr<Shader> ShaderPtr;

enum Shaders {
	SHADER_BASE,
	SHADER_EDGE,
};

class ShaderMap {
	public:
		static ShaderMap *Instance(){
			if(instance == nullptr){
				instance = new ShaderMap();
			}
			return instance;
		}

		inline ShaderPtr getShader(Shaders shad) {
			auto shader = map.find(shad);
			if(shader == map.end()) {
				ShaderPtr shader = std::make_shared<Shader>();
				map.insert(std::make_pair(shad, shader));
				return shader;
			}
			else 
				return shader->second;
		}

		inline void unloadShader(Shaders s) {
			auto shader = map.find(s);
			if(shader != map.end()) {
				map.erase(shader);
			}
		}

		inline void destroy(){
			map.clear();
			delete instance;
		}

	private:
		ShaderMap() {};
		~ShaderMap() {};

		std::unordered_map<Shaders, ShaderPtr> map;
		static ShaderMap* instance;
};

typedef ShaderMap _ShaderMap;
}
