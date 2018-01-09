#pragma once

#include <istream>
#include <ostream>

namespace rosban_utils
{

/// This class is used to serialize objects into binary streams
class StreamSerializable
{
public:
  /// Each object has its own class ID, which is used to determine its true type
  /// - ClassID does not need to be unique among the whole set of classes
  /// - ClassID need to be unique among a common hierarchy
  /// - This concept is mostly used to distinguish objects types among a factory
  virtual int getClassID() const = 0;

  /// Write the classID and then write internal content
  /// return total number of bytes written
  int write(std::ostream & out) const;

  /// Only write the data required to set the object internal data
  /// return total number of bytes written
  virtual int writeInternal(std::ostream & out) const = 0;

  /// Read directly data from a binary stream, assuming the true type of the
  /// object has already been established
  /// Return the number of bytes read
  virtual int read(std::istream & in) = 0;

  /// Save the object to the given file and return the number of bytes written
  /// - write_class_id is required if loader is not supposed to know the true type of the object
  virtual int save(const std::string & filename, bool write_class_id = true) const;

  /// Load the content of the object from the given path
  void load(const std::string & path);
};

}
