#include "rhoban_utils/serialization/json_serializable.h"

using namespace rhoban_utils;

class Children : public JsonSerializable {
public:
  Children() : int_value(0), double_value(1.0){}

  std::string getClassName() const { return "Children";};

  void fromJson(const Json::Value & json_value,
		const std::string & dir_name) override
  {
    (void) dir_name;
    int_value = json_value["int_value"].asInt();
    double_value = json_value["double_value"].asDouble();
  }
  
  Json::Value toJson() const override
  {
    Json::Value v(Json::ValueType::objectValue);
    v["int_value"] = int_value;
    v["double_value"] = double_value;
    return v;
  }
  
  int int_value;
  double double_value;
};

class Mother : public JsonSerializable {
public:
  Mother() {}

  std::string getClassName() const { return "Mother";};

  void fromJson(const Json::Value & json_value,
		const std::string & dir_name) override
  {
    s_value = json_value["s_value"].asString();
    c.fromJson(json_value["children"],dir_name);
  }

  Json::Value toJson() const override
  {
    Json::Value v(Json::ValueType::objectValue);
    v["s_value"] = s_value;
    v["children"] = c.toJson();
    return v;
  }

  Children c;
  std::string s_value;
};

int main(int argc, char ** argv)
{
  Mother m;
  m.c.int_value = 3;
  m.c.double_value = 2.5;
  m.s_value = "toto";
  m.saveFile("m.json");

  Mother m2;
  m2.loadFile("m.json");
  m2.saveFile("m2.json");
}
