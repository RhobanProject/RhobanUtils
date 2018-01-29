#include "rhoban_utils/serialization/json_serializable.h"

#include "rhoban_utils/io_tools.h"
#include "rhoban_utils/util.h"

#include <fstream>

namespace rhoban_utils
{

JsonParsingError::JsonParsingError(const std::string & what_arg)
  : std::runtime_error(what_arg)
{
}

JsonSerializable::JsonSerializable()
{
}

JsonSerializable::~JsonSerializable()
{
}

void JsonSerializable::loadFile()
{
  loadFile(getClassName() + ".json");
}
  
void JsonSerializable::loadFile(const std::string & file_path)
{
  loadFile(getBaseName(file_path), getDirName(file_path));
}

void JsonSerializable::loadFile(const std::string & json_file,
                                const std::string & dir_path)
{
  if (dir_path[dir_path.size()-1] != '/') {
    throw std::logic_error("JsonSerializable::loadFile: dir_path should end by a '/', received : '" + dir_path + "'");
  }
  // Read data
  std::string data = file2string(dir_path + json_file);
  // Create Json reader
  // TODO: investigate all the flags
  auto f=Json::Features::all();
  f.allowComments_=true;
  f.strictRoot_=false;
  f.allowDroppedNullPlaceholders_=true;
  f.allowNumericKeys_=true;
  Json::Reader reader(f);
  // Parse json
  // TODO: treat errors properly
  Json::Value json_content;
  reader.parse(data, json_content);
  try {
    std::cout << "Reading : '" << json_file << "'" << std::endl;
    fromJson(json_content,dir_path);
  } catch (const JsonParsingError & exc) {
    throw JsonParsingError(std::string(exc.what()) + " while reading '" +
                           json_file + "' in '" + dir_path + "'");
  }
}

void JsonSerializable::saveFile() const
{
  saveFile(getClassName() + ".json");
}

void JsonSerializable::saveFile(const std::string &path, bool factory_style) const
{
  // Json Writing
  Json::StyledWriter writer;
  Json::Value content;
  if (factory_style) {
    content = toFactoryJson();
  } else {
    content = toJson();
  }
  // Prepare output stream
  //TODO: error treatment
  std::ofstream output(path);
  output << writer.write(content);
}

Json::Value JsonSerializable::toFactoryJson() const
{
  Json::Value v(Json::ValueType::objectValue);
  v["class name"] = getClassName();
  v["content"] = toJson();
  return v;
}

void JsonSerializable::read(const Json::Value & v, const std::string & key, const std::string & dir_name)
{
  checkMember(v,key);
  try{
    fromJson(v[key],dir_name);
  } catch(const JsonParsingError & exc) {
    throw JsonParsingError(std::string(exc.what()) + " in " + key);
  }
}

void JsonSerializable::tryRead(const Json::Value & v, const std::string & key, const std::string & dir_name)
{
  if (!v.isObject() || !v.isMember(key)){
    return;
  }
  read(v,key,dir_name); 
}

void checkMember(const Json::Value & v, const std::string & key)
{
  if (!v.isObject() || !v.isMember(key)){
    throw JsonParsingError("Could not find member '" + key + "'");
  }
}

template <> bool getJsonVal<bool>(const Json::Value & v)
{
  if (!v.isBool()) {
    throw JsonParsingError("Expecting a bool");
  }
  return v.asBool(); 
}

template <> int getJsonVal<int>(const Json::Value & v)
{
  if (!v.isInt()) {
    throw JsonParsingError("Expecting an int");
  }
  return v.asInt(); 
}

template <> double getJsonVal<double>(const Json::Value & v)
{
  if (!v.isDouble()) {
    throw JsonParsingError("Expecting a double");
  }
  return v.asDouble(); 
}

template <> std::string getJsonVal<std::string>(const Json::Value & v)
{
  if (!v.isString()) {
    throw JsonParsingError("Expecting a string");
  }
  return v.asString(); 
}

Json::Value vector2Json(const Eigen::VectorXd & vec)
{
  Json::Value v;
  v["rows"] = (int)vec.rows();
  for (int row = 0; row < vec.rows(); row++) {
    v["data"].append(vec(row));
  }
  return v;        
}


Json::Value matrix2Json(const Eigen::MatrixXd & m)
{
  Json::Value v;
  v["rows"] = (int)m.rows();
  v["cols"] = (int)m.cols();
  for (int row = 0; row < m.rows(); row++) {
    Json::Value row_data;
    for (int col = 0; col < m.cols(); col++) {
      row_data.append(m(row,col));
    }
    v["values"].append(row_data);
  }
  return v;        
}

template <> Eigen::VectorXd getJsonVal<Eigen::VectorXd>(const Json::Value & v)
{
  if (!v.isObject()) {
    throw JsonParsingError("getJsonVal<Eigen::VectorXd>: Expecting an object");
  }
  size_t rows = read<int>(v,"rows");
  checkMember(v,"values");
  const Json::Value values = v["values"];
  if (!values.isArray()){
    throw JsonParsingError("getJsonVal<Eigen::MatrixXd>: Expecting an array for 'values'");
  }
  if (values.size() != rows) {
    throw JsonParsingError("getJsonVal<Eigen::VectorXd>: Inconsistency in rows numbers");
  }
  Eigen::VectorXd vec(rows);
  for (Json::ArrayIndex row = 0; row < rows; row++) {
    vec(row) = getJsonVal<double>(values[row]);
  }
  return vec;
}

template <> Eigen::MatrixXd getJsonVal<Eigen::MatrixXd>(const Json::Value & v)
{
  if (!v.isObject()) {
    throw JsonParsingError("getJsonVal<Eigen::MatrixXd>: Expecting an object");
  }
  size_t rows = read<int>(v,"rows");
  size_t cols = read<int>(v,"cols");
  checkMember(v,"values");
  const Json::Value values = v["values"];
  if (!values.isArray()){
    throw JsonParsingError("getJsonVal<Eigen::MatrixXd>: Expecting an array for 'values'");
  }
  if (values.size() != rows) {
    throw JsonParsingError("getJsonVal<Eigen::MatrixXd>: Inconsistency in rows numbers");
  }
  Eigen::MatrixXd m(rows,cols);
  for (Json::ArrayIndex row = 0; row < rows; row++) {
    const Json::Value & row_value = values[row];
    if (!row_value.isArray()) {
      throw JsonParsingError("getJsonVal<Eigen::MatrixXd>: Expecting an array for row");
    }
    if (row_value.size() != cols) {
      throw JsonParsingError("getJsonVal<Eigen::MatrixXd>: Inconsistency for cols");
    }
    for (Json::ArrayIndex col = 0; col < cols; col++) {
      m(row,col) = getJsonVal<double>(row_value[col]);
    }
  }
  return m;
}


}
