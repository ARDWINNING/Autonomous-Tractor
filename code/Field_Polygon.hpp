#include <vectors>
#include <stdio>


struct point {
  int x;
  int y;
  point* next;
};

#define referance_coord_x 0
#define referance_coord_y 0
#define precision 0.1


class Field{
  public:
      const char* field_name;    
      const int min_sampling_freq;
      Field(std::vector(point), const char*, int);
      static void sample(std::vector(point), float, float);
      static std::pair<float, float>  sample_distance(std::vector(point), int, float);
      static float sample_optimisation(float, int, float);
  private:
      std::vector(point) field;
      
}
