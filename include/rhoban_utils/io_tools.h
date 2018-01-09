#pragma once

#include <iostream>
#include <string>

namespace rhoban_utils
{

/// Return the content of the whole file as a string
std::string file2string(const std::string &path);

/// WARNING:
/// Those versions are implemented with the quick and dirty approach.
/// - endianness is ignored
/// - errors when writing are not handled

/// Return the number of bytes written
template <typename T>
int write(std::ostream & out, const T & val)
{
  out.write(reinterpret_cast<const char*>(&val), sizeof(T));
  return sizeof(T);
}

/// Return the number of bytes written
template <typename T>
int writeArray(std::ostream & out, int nb_values, const T * val)
{
  out.write(reinterpret_cast<const char*>(val), nb_values * sizeof(T));
  return nb_values * sizeof(T);
}

/// Return the number of bytes written
int writeInt(std::ostream & out, int val);
/// Return the number of bytes written
int writeDouble(std::ostream & out, double val);
/// Return the number of bytes written
int writeIntArray(std::ostream & out, const int * values, int nb_values);
/// Return the number of bytes written
int writeDoubleArray(std::ostream & out, const double * values, int nb_values);


/// Return the number of bytes read
int readInt(std::istream & in, int & val);
/// Return the number of bytes read
int readDouble(std::istream & in, double & val);
/// Return the number of bytes read
int readIntArray(std::istream & in, int * values, int nb_values);
/// Return the number of bytes read
int readDoubleArray(std::istream & in, double * values, int nb_values);

/// Return the number of bytes read
template <typename T>
int read(std::istream & in, T * ptr)
{
  in.read(reinterpret_cast<char*>(ptr), sizeof(T));
  return sizeof(T);
}

/// Return the number of bytes read
template <typename T>
int readArray(std::istream & in, int nb_values, T * ptr)
{
  in.read(reinterpret_cast<char*>(ptr), nb_values * sizeof(T));
  return nb_values * sizeof(T);
}

/// Throw a runtime_error if something goes wrong
template <typename T>
T read(std::istream & in)
{
  T obj;
  read<T>(in, &obj); 
  return obj;
}

}
