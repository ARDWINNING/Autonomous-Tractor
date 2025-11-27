#include "FieldIngest.hpp"

ingestion_handler::ingestion_handler(const char* filepath, const char* fieldname = "")
{
  
  if(ingest_file(filepath, fieldname) != 0)
  {
    // Return to initial screen and display the error code below the filename input.
  }
}

int ingestion_handler::ingest_file(const char* filepath, const char* fieldname = "")
{
  std::filesystem::path p(filepath);
  const char* home = std::getenv("HOME");
  if(!home || p.extension() != ".json")
  {
    return -250;
  }

  std::filesystem::path base(home);
  base = base / "Fields";
  std::filesystem::path full = std::filesystem::canonical(base / p);
  if (full.string().rfind(base.string(), 0) != 0) {
    return -250;
  }

  std::ifstream file(full);
  if(!file)
  {
    return -250; 
  }
  json data = json::parse(file);
  return true;
}

const char* ingestion_handler::getState() const
{
  if(state > 5 || state < 0)
  {
    return INGESTSTATE[6] + (state + 256); 
  }
  return INGESTSTATE[state];
  //rip logs on change
}
