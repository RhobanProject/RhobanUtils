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
  fromJson(json_content,dir_path);
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
  fromJson(v[key],dir_name);
}

void JsonSerializable::tryRead(const Json::Value & v, const std::string & key, const std::string & dir_name)
{
  if (!v.isObject() || !v.isMember(key)){
    return;
  }
  read(v,key,dir_name); 
}


//void JsonSerializable::write(const std::string & key, std::ostream & out) const
//{
//  out << "<" << key << ">" << to_xml() << "</" << key << ">";
//}
//
//void JsonSerializable::factoryWrite(const std::string & key, std::ostream & out) const
//{
//  out << "<" << key << ">";
//  write(class_name(), out);
//  out << "</" << key << ">";
//}
//
//void JsonSerializable::read(TiXmlNode *node, const std::string & key)
//{
//  if(!node) throw XMLParsingError("Null node when trying to read an object");
//  TiXmlNode* child = node->FirstChild(key);
//  if (!child) throw XMLParsingError("No node named '" + key + "' in node '"
//                                    + node->Value() + "'");
//  from_xml(child);
//}
//
//void JsonSerializable::tryRead(TiXmlNode *node, const std::string &key)
//{
//  if(!node) throw XMLParsingError("Null node when trying to read an object");
//
//  TiXmlNode* child = node->FirstChild(key);
//  if (!child) return;
//
//  from_xml(child);
//}
//    
//
//void JsonSerializable::pretty_print() const
//{
//  TiXmlDocument * doc = xml_tools::string_to_doc(to_xml_stream());
//  if(!doc)
//  {
//    std::cout << " Failed to turn object to xml doc" << std::endl;
//    return;
//  }
//
//  std::cout << std::endl;
//  doc->Print();
//  std::cout << std::endl;
//  delete doc;
//}

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
