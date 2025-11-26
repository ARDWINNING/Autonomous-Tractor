#include "GeoJSON.hpp"

int ingest_file(const char* filename, const char* fieldname = "")
{
  std::ifstream file(filename);
  if(!file)
  {
    return -1; 
  }
  json data = json::parse(file);
  return true;
}
