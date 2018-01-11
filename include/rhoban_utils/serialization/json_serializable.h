#pragma once

#include <json/json.h>

#include <Eigen/Core>

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

/// Throws a JsonParsingError if v[key] does not exist
void checkMember(const Json::Value & v, const std::string & key);

/// Throws a JsonParsingError if 'v' is not of type 'T'
template <typename T> T getJsonVal(const Json::Value & v);

template <> bool        getJsonVal<bool>       (const Json::Value & v);
template <> int         getJsonVal<int>        (const Json::Value & v);
template <> double      getJsonVal<double>     (const Json::Value & v);
template <> std::string getJsonVal<std::string>(const Json::Value & v);

/// Return an object of type 'T' if v[key] exists and is of type 'T'.
/// Otherwise: throws a JsonParsingError
template <typename T> T read(const Json::Value & v, const std::string & key)
{
  checkMember(v,key);
  try {
    return getJsonVal<T>(v[key]);
  } catch (const JsonParsingError & error) {
    throw JsonParsingError(error.what() + std::string(" at '") + key + "'");   
  }
}
  
/// - if v[key] exists and has type 'T', write the value in ptr
/// - if v[key] does not exist, do not modify ptr
/// - if v[key] exists but has inappropriate type, throws a JsonParsingError
template <typename T>
void tryRead(const Json::Value & v, const std::string & key, T * ptr)
{
  if (v.isObject() && v.isMember(key)) {
    *ptr = read<T>(v, key);
  }
}

template <typename T>
std::vector<T> readVector(const Json::Value & v, const std::string & key)
{
  checkMember(v,key);
  if (!v[key].isArray()) {
    throw JsonParsingError("Value at '" + key + "' is not an array");
  }
  std::vector<T> result;
  for (Json::ArrayIndex idx=0; idx < v[key].size();idx++) {
    result.push_back(getJsonVal<T>(v[key][idx]));
  }
  return result;
}

template <typename T>
void tryReadVector(const Json::Value & v, const std::string & key, std::vector<T> * ptr)
{
  if (v.isObject() && v.isMember(key)) {
    *ptr = readVector<T>(v, key);
  }
}

template <typename T>
static Json::Value vector2Json(const std::vector<T> & values)
{
  Json::Value v;
  for (Json::ArrayIndex idx = 0; idx < values.size(); idx++) {
    v[idx] = values[idx];
  }
  return v;
}

Json::Value vector2Json(const Eigen::VectorXd & v);
Json::Value matrix2Json(const Eigen::MatrixXd & m);

template <> Eigen::VectorXd getJsonVal<Eigen::VectorXd>(const Json::Value & v);
template <> Eigen::MatrixXd getJsonVal<Eigen::MatrixXd>(const Json::Value & v);

template <typename T>
std::vector<T> readVector(const Json::Value & v,
                          const std::string & dir_name,
                          std::function<T(const Json::Value & v,
                                          const std::string & dir_name)> builder)
{
  if (!v.isArray()) {
    throw JsonParsingError("readVector:: Expecting an array");
  }
  std::vector<T> result;
  for (Json::ArrayIndex idx = 0; idx < v.size(); idx++){
    result.push_back(builder(v[idx],dir_name));
  }
  return result;
}

template <typename T>
std::vector<T> readVector(const Json::Value & v,
                          const std::string & vec_name,
                          const std::string & dir_name,
                          std::function<T(const Json::Value & v, const std::string & dir_name)> builder)
{
  checkMember(v,vec_name);
  return readVector(v[vec_name], dir_name, builder);
}

template <typename T>
void tryReadVector(const Json::Value & v,
                   const std::string & vec_name,
                   const std::string & dir_name,
                   std::function<T(const Json::Value & v,
                                   const std::string & dir_name)> builder,
                   std::vector<T> * ptr)
{
  if (v.isObject() && v.isMember(vec_name)) {
    *ptr = readVector(v, vec_name, dir_name, builder);
  }
}


/// Read a map which has the following structure using the given builder
/// {
///   "key" : "value",
///   ...
/// }
template <typename T>
std::map<std::string,T> readMap(const Json::Value & v,
                                const std::string & dir_name,
                                std::function<T(const Json::Value & v, const std::string & dir_name)> builder)
{
  if (!v.isObject()) {
    throw JsonParsingError("readMap:: Expecting an object");
  }
  // Parse entries:
  std::map<std::string, T> result;
  for (Json::ValueConstIterator it = v.begin(); it != v.end(); it++) {
    std::string key = it.name();
    result[key] = builder(v[key], dir_name);
  }
  return result;
}

template <typename T>
std::map<std::string,T> readMap(const Json::Value & v,
                                const std::string & map_name,
                                const std::string & dir_name,
                                std::function<T(const Json::Value & v, const std::string & dir_name)> builder)
{
  checkMember(v,map_name);
  return readMap(v[map_name], dir_name, builder);
}

}
