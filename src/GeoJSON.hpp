#include "headers/json.hpp"
#include <fstream>
#include <vector>
using json = nlohmann::json;

// Needs to read the file read points into point* check first == last, remove last, link ends, then do the same for obstacles. 
// If possible check the rotation, outer anti-clockwise, obstacles clockwise. 
// Then call and make a Field Class. 
// This class will handle all to do with field ingestion so will also interact with the swath generation class along with any subsequent classes to do with the procurement of a full route in the field.
// Also include a state machine to feed to eventual GUI showing ingestion progress, as well as log file generation with useful errors

bool ingest_file(char*, char*);


