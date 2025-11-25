#include <vector>
#include <math.h>
#include <algorithm>

#define referance_coord_x 0
#define referance_coord_y 0
#define deletion_threshold 1 // Placeholder

struct point {
  float x;
  float y;
  point* next;
  point* prev;
  point(int x_val, int y_val, point* n = nullptr, point* p = nullptr)
    : x(x_val), y(y_val), next(n), prev(p) {}
  point(point* p)
    : x(p->x), y(p->y), next(p->next), prev(p->prev) {}
};

struct obstacle {
  std::vector<point*> perimeter;
  std::pair<float, float> x_extremes;
  std::pair<float, float> y_of_x_extremes;
  obstacle* next;
  obstacle* prev;
  point* central_line;
};

struct cell {
  point* start;
  cell* next;
  cell* prev;
};

class Field{
  public:
    const char* field_name;    
    explicit Field(point*, obstacle*, const char*);
    ~Field();
    // Visvalingam-Whyatt optimisation, method: deletion threshold
    // Could look into a heap implementation
    // Would prefer using a threshold than number of points due to variability in fields
    void VW_sample(point*);
    point* VW_calculator(point*);
    float VW_area(const point*);

    // Boustrophedon Cellular Decomposition
    bool BCD();

    // Helper Functions
  std::vector<point*> obstacle_points(obstacle*, std::pair<float, float>, int);
    obstacle* find_obstacle(point*, std::pair<float, float>);
    point* infer_point(point*, const float, const char);
    void get_extremes();
    point* add(point*, int);

  private:
    std::vector<point*> field;
    std::pair<float, float> x_extreme;
    std::pair<float, float> y_extreme;
    obstacle* initial_obstacle;
    std::vector<std::vector<point*>> cells;
    point* field_start;
};
