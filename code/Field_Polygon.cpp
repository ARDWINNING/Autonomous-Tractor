#include "Field_Polygon.hpp"

explicit Field::Field(std::vector(point) field, const char* name, int min_sample)
{
  field_name = name;
  min_sampling_freq = min_sample;
}

void Field::sample(std::vector(point) polygon, float x_sample, float y_sample)
{
  point* last = nullptr;
  point* first = polygon[0]; 
}

std::pair<int, int> Field::sample_distance(const std::vector(point) polygon, int min_distance, float precision)
{
  float x_max, x_min = polygon[0].x;
  
  float y_max, y_min = polygon[0].y;
  
  for(int i = 0; i < polygon.size(); i++)
  {
    x_val = polygon[i].x;
    y_val = polygon[i].y;

    x_max = (x_val > x_max) ? x_val : x_max;
    x_min = (x_val < x_min) ? x_val : x_min;

    y_max = (y_val > y_max) ? y_val : y_max;
    y_min = (y_val < y_min) ? y_val : y_min;
  }
  
  float x_dist = x_max - x_min;
  float y_dist = y_max - y_min;

  std::pair<float, float> ret(0,0);
  ret.first = sample_optimisation(x_dist, min_distance, precision);
  ret.second = sample_optimisation(y_dist, min_distance, precision);
 
  return ret;
}

float Field::sample_optimisation(float distance, int minimum, float precision){
  float best_sample = distance % minimum_dist;
  float opt_distance = minimum;
  float curr_sample;

  for(int j = (1/precision * minimum) - 1; j > 0; j--)
  {
    curr_sample = distance % j*precision;
    if(best_sample > curr_sample)
    {
      best_sample = curr_sample;
      opt_distance = j*precision;
    }
  }

  return opt_distance;
}



