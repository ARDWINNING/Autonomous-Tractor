#include "headers/json.hpp"
#include <fstream>
#include <vector>
#include <filesystem>

using json = nlohmann::json;

// Needs to read the file read points into point* check first == last, remove last, link ends, then do the same for obstacles. 
// If possible check the rotation, outer anti-clockwise, obstacles clockwise. 
// Then call and make a Field Class. 
// This class will handle all to do with field ingestion so will also interact with the swath generation class along with any subsequent classes to do with the procurement of a full route in the field.
// Also include a state machine to feed to eventual GUI showing ingestion progress, as well as log file generation with useful errors

// Need to figure out source of time as well as handling of multithreading one thread handles the ingest, another handles the the getstate() handled by a screen hypervisor maybe?? as will be used in that. Logs also need creating and ensure state is copy on write if needed

// Error Code Master List (conversion is int return + 256):
//  6: Error Reading File
//  7:

static const char* INGESTSTATE[] {
  "Idle", 
  "Ingesting file", 
  "Converting to Field", 
  "Optimising Field", 
  "Decomposing Field", 
  "Field Successfully Ingested",
  "In Error Code: "
};

class ingestion_handler
{
  public:
    ingestion_handler(const char*, const char*);
    const char* getState() const;
    int ingest_file(const char*, const char*);

  private:
    int state;
};



