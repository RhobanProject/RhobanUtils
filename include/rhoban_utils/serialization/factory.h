#pragma once

#include "rhoban_utils/io_tools.h"
#include "rhoban_utils/util.h"
#include "rhoban_utils/serialization/json_serializable.h"

#include <json/json.h>

#include <fstream>
#include <functional>
#include <memory>
#include <string>
#include <map>

namespace rhoban_utils
{

// TODO:
// - Add write options eventually (issues with guarantees of const, maybe only for shared_ptr)

/// This class implements a factory pattern. It can be used for producing
/// various objects inheriting from class T. More specific initialization can be
/// achieved by using data from a Json::Value
template <class T>
class Factory
{
public:
  Factory() {}
  
  /// Builder which takes no argument
  typedef std::function<std::unique_ptr<T>()> Builder;
  /// JsonBuilder can use json data
  typedef std::function<std::unique_ptr<T>(const Json::Value & json_val,
                                           const std::string & dir)> JsonBuilder;
  /// Builder using an input stream to customize the created object @see StreamSerializable
  typedef std::function<std::unique_ptr<T>(std::istream & in,
                                           int * nb_bytes_read)> StreamBuilder;

  JsonBuilder getBuilder(const std::string &class_name) const
    {
      try
      {
        return json_builders.at(class_name);
      }
      catch (const std::out_of_range &exc)
      {
        std::ostringstream oss;
        oss << "Factory: type '" << class_name << "' is not registered" << std::endl;
        listBuilders(oss);
        throw std::out_of_range(oss.str()); 
      }
    }

  Builder getBuilder(int id) const
    {
      try
      {
        return builders_by_id.at(id);
      }
      catch (const std::out_of_range &exc)
      {
        std::ostringstream oss;
        oss << "Factory: id '" << id << "' is not registered";
        throw std::out_of_range(oss.str()); 
      }
    }

  StreamBuilder getStreamBuilder(int id) const
    {
      try
      {
        return stream_builders_by_id.at(id);
      }
      catch (const std::out_of_range &exc)
      {
        std::ostringstream oss;
        oss << "Factory: id '" << id << "' is not registered";
        throw std::out_of_range(oss.str()); 
      }
    }
  
  std::unique_ptr<T> build(const std::string &class_name) const
    {
      Json::Value v(Json::ValueType::objectValue);
      v["class name"] = class_name;
      v["content"] = Json::Value();
      return build(v);
    }

  /// Reducing ambiguity
  std::unique_ptr<T> build(const char * class_name) const
    {
      return build(std::string(class_name));
    }

  /// When building from a node, the expected format of the json is the following:
  /// "class name" : <object type> ,
  /// "content:" : ...
  std::unique_ptr<T> build(const Json::Value & v,
                           const std::string & dir_path = "./") const
    {
      // Checking node validity
      if (v.isNull()) {
        throw JsonParsingError("Factory::build: Trying to build from a NULL value");
      }
      if (! v.isObject()) {
        throw JsonParsingError("Factory::build: Trying to build from a non-object value");
      }
      // Getting class name
      std::string class_name;
      if (v.isMember("class name")) {
        const Json::Value & class_value = v["class name"];
        if (class_value.isString()) {
          class_name = class_value.asString();
        } else {
          throw JsonParsingError("Factory::build: class name does not contain a string");
        }
      } else {
        throw JsonParsingError("Factory::build: no class name found");
      }
      // Getting content
      if (!v.isMember("content")) {
        throw JsonParsingError("Factory::build: no 'content' found");
      }
      return getBuilder(class_name)(v["content"],dir_path);
    }

  std::unique_ptr<T> build(int id) const
    {
      return getBuilder(id)();
    }

  /// path: path to xml_file
  std::unique_ptr<T> buildFromJsonFile(const std::string &path) const
    {
      // Read data
      std::string data = file2string(path);
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
      /// Build:
      std::string dir_path = getDirName(path);
      return build(json_content, dir_path);
    }

  /// Throw a JsonParsingError, if value is not an object containing the given key,
  /// otherwise, build the object
  /// @see JsonSerializable::toFactoryJson
  std::unique_ptr<T> read(const Json::Value & value,
                          const std::string & key, const std::string & dir_path)
    {
      if (!value.isObject()) {
        throw JsonParsingError("Non object node when trying to read from a factory");
      }
      if (!value.isMember(key)) {
        throw JsonParsingError("Could not find member '" + key + "'");
      }
      return build(value[key], dir_path);
    }

  /// Fill 'ptr' with a generated object with the given value as argument
  /// If value is null or key is not found:
  /// - no error is thrown and 'ptr' content is not modified
  /// If key is found but content cannot be parsed:
  /// - an error is still raised and the 'ptr' is not overwritten
  /// If key is found and content is parsed properly:
  /// - 'ptr' is overwritten (and thus, old value is deleted according to unique_ptr)
  void tryRead(const Json::Value & value,
               const std::string & key, const std::string & dir_path,
               std::unique_ptr<T> * ptr)
    {
      if (!value.isObject()){
        return;
      }
      if (!value.isMember(key)){
        return;
      }
      *ptr = build(value[key], dir_path);
    }

  std::vector<std::unique_ptr<T>> readVector(const Json::Value & value,
                                             const std::string & key,
                                             const std::string & dir_path)
    {
      // Checks: object, member then array
      if (!value.isObject()) {
        throw JsonParsingError("Non object node when trying to read from a factory");
      }
      if (!value.isMember(key)) {
        throw JsonParsingError("Could not find member '" + key + "'");
      }
      const Json::Value key_value = value[key];
      if (!key_value.isArray()) {
        throw JsonParsingError("Member '" + key + "' does not contain an array");
      }
      // Building internal objects
      std::vector<std::unique_ptr<T>> result;
      for (Json::ArrayIndex i = 0; i < key_value.size(); i++) {
        result.push_back(build(key_value[i],dir_path));
      }
      return result;
    }

  /// Try to read a vector with the given key in a json Value
  /// throw exceptions if:
  /// - value is not an object
  /// - an exception is thrown while building one of the element
  /// Do not throw exception if key does not exist (in this case res is not modified)
  void tryReadVector(const Json::Value & value,
                     std::string & key, const std::string & dir_path,
                     std::vector<std::unique_ptr<T>> & res)
    {
      // Checks: object, member then array
      if (!value.isObject()) {
        throw JsonParsingError("Non object node when trying to read from a factory");
      }
      if (!value.isMember(key)) {
        return;
      }
      res = readVector(value, key, dir_path);
    }

  /// Return the number of bytes read
  int read(std::istream & in, std::unique_ptr<T> & ptr)
    {
      int bytes_read = 0;
      int id;
      bytes_read += rhoban_utils::read<int>(in, &id);
      int builder_bytes_read = 0;
      ptr = getStreamBuilder(id)(in, &builder_bytes_read);
      bytes_read += builder_bytes_read;
      return bytes_read;
    }

  /// Return the number of bytes read
  int loadFromFile(const std::string & filename, std::unique_ptr<T> & ptr)
    {
      std::ifstream in(filename, std::ios::binary);
      if (!in) {
        std::ostringstream oss;
        oss << "Failed to open '" << filename << "' for binary reading";
        throw std::runtime_error(oss.str());
      }
      int bytes_read = read(in, ptr);
      in.close();
      return bytes_read;
    }

  /// Convert an empty builder to a Json builder
  static JsonBuilder toJsonBuilder(Builder builder, bool parse_json)
    {
      return [builder, parse_json]
        (const Json::Value & v, const std::string & dir) -> std::unique_ptr<T>
      {
        std::unique_ptr<T> object(builder());
        if (v.isObject() && parse_json) object->fromJson(v,dir);
        return std::move(object);
      };
    }

  /// Convert an empty builder to a stream builder
  static StreamBuilder toStreamBuilder(Builder builder)
    {
      return [builder](std::istream & in,
                       int * nb_bytes_read) -> std::unique_ptr<T>
      {
        std::unique_ptr<T> object(builder());
        int tmp = object->read(in);
        if (nb_bytes_read != nullptr) *nb_bytes_read = tmp;
        return std::move(object);
      };
    }

  /// Send an error if a builder for the given class_name is already registered
  /// - Automatically transform the builder in JsonBuilder
  void registerBuilder(const std::string &class_name, Builder builder, bool parse_json = true)
    {
      registerBuilder(class_name, toJsonBuilder(builder, parse_json));
    }

  /// Send an error if a builder for the given class_name is already registered
  void registerBuilder(const std::string &class_name, JsonBuilder builder)
    {
      if (json_builders.count(class_name) != 0)
        throw std::runtime_error("Factory: registering a class named '" + class_name
                                 + "' while it already exists");
      json_builders[class_name] = builder;
    }

  /// Send an error if a builder for the given id is already registered
  /// if autocreate_streambuilder is true, then it also create and register a streambuilder
  void registerBuilder(int id, Builder builder, bool autocreate_streambuilder = true)
    {
      if (autocreate_streambuilder) {
        registerBuilder(id, toStreamBuilder(builder));
      }
      if (builders_by_id.count(id) != 0) {
        std::ostringstream oss;
        oss << "Factory: registering a class with id '" << id
            << "' while it is already used";
        listBuilders(oss);
        throw std::runtime_error(oss.str());
      }
      builders_by_id[id] = builder;
    }

  /// Send an error if a builder for the given id is already registered
  void registerBuilder(int id, StreamBuilder builder)
    {
      if (stream_builders_by_id.count(id) != 0) {
        std::ostringstream oss;
        oss << "Factory: registering a class with id '" << id
            << "' while it is already used";
        throw std::runtime_error(oss.str());
      }
      stream_builders_by_id[id] = builder;
    }

  /// List all the known builders to the stream
  void listBuilders(std::ostream & out) const
    {
      out << "Json Builders: " << std::endl;
      for (const auto & entry : json_builders) {
        out << "\t" << entry.first << std::endl;
      }
      out << "Default Builders: " << std::endl;
      for (const auto & entry : builders_by_id) {
        out << "\t" << entry.first << std::endl;
      }
      out << "Stream Builders: " << std::endl;
      for (const auto & entry : stream_builders_by_id) {
        out << "\t" << entry.first << std::endl;
      }
    }

private:
  /// Contains a mapping from class names to json builders
  std::map<std::string, JsonBuilder>  json_builders;
  /// Contains a mapping from id to builders
  std::map<int, Builder>  builders_by_id;
  /// Contains a mapping from id to stream builders
  std::map<int, StreamBuilder> stream_builders_by_id;
};

}
