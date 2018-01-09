#pragma once

#include "rosban_utils/xml_tools.h"

namespace rosban_utils
{

class Serializable
{
public:
  virtual std::string class_name() const = 0;
  
  /// loads the object from the default file
  /// the default filename is the name of the class + .xml
  virtual void load_file();

  /// loads the object from a given file
  void load_file(const std::string &filename);

  /// serializes and saves to a file using default filename
  void save_file();

  /// serializes and saves to a file using given filename
  void save_file(const std::string &filename);

  /// deserializes from an xml stream
  void from_xml(const std::string &xml_stream);

  /// deserializes from an xml node
  virtual void from_xml(TiXmlNode *node) = 0;

  /// serializes to an xml stream excluding class name
  std::string to_xml() const;

  // Append xml to the given stream
  virtual void to_xml(std::ostream &out) const = 0;

  /// serializes to an xml stream including class name
  std::string to_xml_stream() const;

  /// Write in the given stream the serializable object inside a node with the given key
  void write(const std::string & key, std::ostream & out) const;

  /// In factoryWrite, the pattern is the following:
  /// <key><class_name>...</class_name></key>
  void factoryWrite(const std::string & key, std::ostream & out) const;

  /// Update the object from the given node
  void read(TiXmlNode *node, const std::string & key);

  /// Try to update the object from the given node
  /// Throw an exception:
  /// - if node is NULL
  /// - if an exception is thrown while reading the object from the xml tree
  /// DO NOT throw an exception (do not modify the object)
  /// - If there is no node with the right key
  void tryRead(TiXmlNode *node, const std::string & key);

  /*! pretty print */
  void pretty_print() const;

protected:
  Serializable();

  virtual ~Serializable();
};

/*! not so pretty print */
std::ostream & operator<< (std::ostream & stream, Serializable & obj);

}
