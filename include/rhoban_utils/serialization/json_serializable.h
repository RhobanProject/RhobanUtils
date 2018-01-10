#pragma once

#include <json/json.h>

#include <exception>

namespace rhoban_utils
{

class JsonParsingError : public std::runtime_error
{
public:
  JsonParsingError(const std::string & what_arg);
};

class JsonSerializable
{
public:
  JsonSerializable();
  virtual ~JsonSerializable();

  virtual std::string getClassName() const = 0;
  
  /// loads the object from the default file
  /// the default filename is the name of the class + .json
  virtual void loadFile();

  /// Load the file at the given path
  void loadFile(const std::string & file_path);

  /// Separation between json_file and directory is used to ensure
  /// possibility to use relative_path (from dir_path)
  /// dir_path should end by a "/"
  void loadFile(const std::string & json_file,
                const std::string & dir_path);

  /// Serializes and saves to a file using default filename
  void saveFile() const;

  /// Serializes and saves to a file using given filename
  /// if factory_style is true, uses toFactoryJson (allows choosing type during loading)
  void saveFile(const std::string &filename, bool factory_style = false) const;

  /// Deserializes from a json content found in 'dir_name'
  virtual void fromJson(const Json::Value & json_value,
                        const std::string & dir_name) = 0;

  /// Represent current object as a Json::Value
  virtual Json::Value toJson() const = 0;

  /// Represent current object as a Json::Value containing information on class for factories
  Json::Value toFactoryJson() const;

  
  /// Read the content of the object if v[key] exists
  /// Otherwise: throws a JsonParsingError
  void read(const Json::Value & v, const std::string & key, const std::string & dir_name = "./");
  /// Read the content of the object if v[key] exists
  /// Do not throw an exception if v[key] is not defined
  /// Still throws an exception if v[key] exists with an invalid content
  void tryRead(const Json::Value & v, const std::string & key, const std::string & dir_name = "./");

  // TODO: check if this part is still necessary
//  /// Write in the given stream the serializable object inside a node with the given key
//  void write(const std::string & key, std::ostream & out) const;
//
//  /// In factoryWrite, the pattern is the following:
//  /// <key><class_name>...</class_name></key>
//  void factoryWrite(const std::string & key, std::ostream & out) const;
//
//  /// Update the object from the given node
//  void read(TiXmlNode *node, const std::string & key);
//
//  /// Try to update the object from the given node
//  /// Throw an exception:
//  /// - if node is NULL
//  /// - if an exception is thrown while reading the object from the xml tree
//  /// DO NOT throw an exception (do not modify the object)
//  /// - If there is no node with the right key
//  void tryRead(TiXmlNode *node, const std::string & key);
//
//  /*! pretty print */
//  void pretty_print() const;

};

/// Return an object of type 'T' if v[key] exists and is of type 'T'.
/// Otherwise: throws a JsonParsingError
template <typename T> T read(const Json::Value & v, const std::string & key);

template <> bool        read<bool>       (const Json::Value & v, const std::string & key);
template <> int         read<int>        (const Json::Value & v, const std::string & key);
template <> double      read<double>     (const Json::Value & v, const std::string & key);
template <> std::string read<std::string>(const Json::Value & v, const std::string & key);
  
/// - if v[key] exists and has type 'T', write the value in ptr
/// - if v[key] does not exist, do not modify ptr
/// - if v[key] exists but has inappropriate type, throws a JsonParsingError
template <typename T>
static void tryRead(const Json::Value & v, const std::string & key, T * ptr)
{
  if (v.isObject() && v.isMember(key)) {
    *ptr = read<T>(v, key);
  }
}


}
