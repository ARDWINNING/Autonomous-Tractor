#include <vector>
#include <math.h>

struct point {
  float x;
  float y;
  point* next;
  point* prev;
  point(int x_val = 0, int y_val = 0, point* n = nullptr, point* p = nullptr)
        : x(x_val), y(y_val), next(n), prev(p) {}
};

struct obstacle {
  std::vector<point*> perimeter;
  std::pair<float, float> x_extremes;
  std::pair<float, float> y_extremes;
};

#define referance_coord_x 0
#define referance_coord_y 0
#define deletion_threshold 1 // Placeholder

class Field{
  public:
    const char* field_name;    
    explicit Field(point*, const char*);
    // Visvalingam-Whyatt optimisation, method: deletion threshold
    // Could look into a heap implementation
    // Would prefer using a threshold than number of points due to variability in fields
    void VW_sample(point*);
    point* VW_calculator(point*);
    float VW_area(const point*);
    // Boustrophedon Cellular Decomposition
    std::vector<point*> BCD;

    // Helper Functions
    point* infer_point(point*, const float, const char);
  private:
    std::vector<point*> field; 
    std::vector<obstacle*> obstacles;
};
