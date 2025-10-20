#include <vector>
#include <math.h>

struct point {
  int x;
  int y;
  point* next;
  point* prev;
};

#define referance_coord_x 0
#define referance_coord_y 0
#define deletion_threshold 1

class Field{
  public:
    const char* field_name;    
    explicit Field(point*, const char*);
    void VW_sample(point*);
    point* VW_calculator(point*);
    float VW_area(point*);
  private:
    std::vector<point*> field; //ensure that list is unique so end points to first as next
};
