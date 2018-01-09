#pragma once

#include <json/json.h>

namespace rhoban_utils
{

class JsonSerializable
{
public:
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
  void saveFile();

  /// Serializes and saves to a file using given filename
  void saveFile(const std::string &filename);

  /// Deserializes from a json content found in 'dir_name'
  virtual void fromJson(const Json::Value & json_value,
			const std::string & dir_name) = 0;

  // Append xml to the given stream
  virtual Json::Value toJson() const = 0;

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

protected:
  JsonSerializable();

  virtual ~JsonSerializable();
};

}
