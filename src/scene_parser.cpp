#include "scene_parser.hpp"

namespace phyl {
	SceneParser::SceneParser(const std::string &fp){
		m_sceneNode = YAML::LoadFile(fp);
	}
	void SceneParser::GetCloth(std::shared_ptr<ClothMesh> &cloth){
		YAML::Node clothNode = m_sceneNode["cloth"];
		double mass = clothNode["mass"] ? clothNode["mass"].as<double>():5;
		double width = clothNode["width"] ? clothNode["width"].as<double>() : 50;
		double height = clothNode["height"] ? clothNode["height"].as<double>() : 50;
		double lod = clothNode["lod"] ? clothNode["lod"].as<double>() : 10;
		bool hasFixed = clothNode["fixed"] ? clothNode["fixed"].as<bool>() : false;
		MTransform t;
		if(clothNode["transform"]){
			if(clothNode["transform"]["translation"]){
				YAML::Node translationNode = clothNode["transform"]["translation"];
				float x = translationNode["x"] ? translationNode["x"].as<float>() : 0;
				float y = translationNode["y"] ? translationNode["y"].as<float>() : 0;
				float z = translationNode["z"] ? translationNode["z"].as<float>() : 0;
				t.translate({x,y,z});
			}
			if(clothNode["transform"]["rotation"]){
				YAML::Node rotationNode = clothNode["transform"]["rotation"];
				float x = rotationNode["x"] ? rotationNode["x"].as<float>() : 0;
				t.rotate({1.0f, 0.0f, 0.0f}, x);
				float y = rotationNode["y"] ? rotationNode["y"].as<float>() : 0;
				t.rotate({0.0f, 1.0f, 0.0f}, y);
				float z = rotationNode["z"] ? rotationNode["z"].as<float>() : 0;
				t.rotate({0.0f, 0.0f, 1.0f}, z);
			}
			if(clothNode["transform"]["scale"]){
				YAML::Node scaleNode = clothNode["transform"]["scale"];
				float x = scaleNode["x"] ? scaleNode["x"].as<float>() : 0;
				float y = scaleNode["y"] ? scaleNode["y"].as<float>() : 0;
				float z = scaleNode["z"] ? scaleNode["z"].as<float>() : 0;
				t.scale({x,y,z});
			}
		}
		cloth = std::make_shared<ClothMesh>(width, height, mass, lod, hasFixed);
		cloth->transformPoints(t);
	}
	void SceneParser::GetPrimitives(std::vector<SpherePrimitive> &primitives) {
		YAML::Node spheresArray = m_sceneNode["spheres"];
		for(int i = 0; i < spheresArray.size(); ++i){
			YAML::Node s = spheresArray[i];
			Vector3 center{0.0f,0.0f,0.0f};
			if(s["center"]){
				YAML::Node c = s["center"];
				center.x = c["x"] ? c["x"].as<float>() : 0;
				center.y = c["y"] ? c["y"].as<float>() : 0;
				center.z = c["z"] ? c["z"].as<float>() : 0;
			}
			double rad = s["radius"] ? s["radius"].as<double>() : 1.0f;
			Color c = s["color"] ? GetColor(s["color"].as<int>()) : LIME;
			primitives.push_back(SpherePrimitive(center, rad, c));
		}
	}

	bool SceneParser::HasMovingSpheres() const
	{
		return m_sceneNode["spheres_moving"] ? m_sceneNode["spheres_moving"].as<bool>() : false;
	}

	Color SceneParser::GetColor(int i) {
		switch(i){
			case 0:
				return LIGHTGRAY;
			case 1:
				return RED;
			case 2:
				return GOLD;
			case 3:
				return VIOLET;
			case 4:
				return BEIGE;
			case 5:
				return SKYBLUE;
			case 6:
				return PINK;
			case 7:
				return DARKGREEN;
			case 8:
				return MAROON;
			case 9:
			default:
				return LIME;
		}
	}
}
