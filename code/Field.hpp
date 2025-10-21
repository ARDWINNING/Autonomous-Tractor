#include <vector>
#include <math.h>
#include <algorithm>

struct point {
  float x;
  float y;
  point* next;
  point* prev;
  point(int x_val, int y_val, point* n = nullptr, point* p = nullptr)
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
    explicit Field(point*, std::vector<obstacle*>, const char*);
    // Visvalingam-Whyatt optimisation, method: deletion threshold
    // Could look into a heap implementation
    // Would prefer using a threshold than number of points due to variability in fields
    void VW_sample(point*);
    point* VW_calculator(point*);
    float VW_area(const point*);
    // Boustrophedon Cellular Decomposition
    bool BCD();

    // Helper Functions
    point* infer_point(point*, const float, const char);
    void get_extremes();
  private:
    std::vector<point*> field;
    std::pair<float, float> x_extreme;
    std::pair<float, float> y_extreme;
    std::vector<obstacle*> obstacles;
    std::vector<std::vector<point*>> cells;
};
