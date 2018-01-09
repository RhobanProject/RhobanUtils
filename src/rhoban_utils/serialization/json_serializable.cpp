#include "rhoban_utils/serialization/json_serializable.h"

#include "rhoban_utils/io_tools.h"
#include "rhoban_utils/util.h"

#include <fstream>

namespace rhoban_utils
{

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

void JsonSerializable::saveFile()
{
  saveFile(getClassName() + ".json");
}

void JsonSerializable::saveFile(const std::string &path)
{
  // Json Writing
  Json::StyledWriter writer;
  std::string content = writer.write(toJson());
  // Prepare output stream
  //TODO: error treatment
  std::ofstream output(path);
  output << content;
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

}
