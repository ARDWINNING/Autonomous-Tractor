#include "Field.hpp"

Field::Field(point* start, std::vector<obstacle*> Obstacles, const char* name)
  : obstacles(Obstacles), field_name(name) {
  VW_sample(start);
}

// Visvalingam-Whyatt (VW) Optimisation 

void Field::VW_sample(point* initial)
{
  point* current = VW_calculator(initial);
  point* place = current;
  current = current->next;
  
  while(current != place)
  {
    current = VW_calculator(current);
    current = current->next;
  }

  place = current;
  field.push_back(current);
  current = current->next;
  
  while(current != place)
  {
    field.push_back(current);
    current = current->next;
  }
  return;
}

point* Field::VW_calculator(point* curr)
{
  if(VW_area(curr) < deletion_threshold)
  {
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    (void)VW_calculator(curr->prev);
    point* deepest = VW_calculator(curr->next);
    delete curr;
    curr = nullptr;
    return deepest;
  }
  return curr;
}

float Field::VW_area(const point* targ)
{
  float prev_x = targ->prev->x;
  float prev_y = targ->prev->y;
  float next_x = targ->next->x;
  float next_y = targ->next->y;
  
  float ret = prev_x * targ->y + targ->x * next_y + next_x * prev_y;
  ret -= prev_x * next_y + targ->x * prev_y + next_x * targ->y;
  return 0.5 * std::abs(ret);
}

// Boustrophedon Cellular Decomposition (BCD)

bool Field::BCD()
{
  std::vector<float> boundaries;
  point* start = field[0];
  int count = 0;
  
  for(int i = 0; i < obstacles.size(); i++)
  {
    boundaries.push_back(obstacles[i]->x_extremes.first);
    boundaries.push_back(obstacles[i]->x_extremes.second);
  }
  std::sort(boundaries.begin(), boundaries.end());
  int place = boundaries.size();
  if(int j = 0; j < boundaries; j++)
  {
    if(start->x < boundaries[j])
    {
      place = j;
      exit;
    }
  }
  
  
  
  return true;
}

// Helper Functions

point* Field::infer_point(point* before, const float actual, const char axis)
{
  float x;
  float y;
  point* ret;

  if(axis == 'x')
  {
    float x = actual; 
    if(before->x < before->prev->x) // geoJSON is anticlockwise
    {
      float grad = (before->prev->y - before->y) / (before->prev->x - before->x);
      float y = before->y + grad * (actual - before->x);
      ret = new point(x, y, before, before->prev);
      before->prev->next = ret;
      before->prev = ret;
    }
    else
    {
      float grad = (before->next->y - before->y) / (before->next->x - before->x);
      float y = before->y + grad * (actual - before->x);
      ret = new point(x, y, before->next, before);
      before->next->prev = ret;
      before->next = ret;
    }
  }
  else
  {
    float y = actual;
    if(before->y < before->prev->y)
    {
      float grad = (before->prev->x - before->x) / (before->prev->y - before->y);
      float x = before->x + grad * (actual - before->y);
      ret = new point(x, y, before, before->prev);
      before->prev->next = ret;
      before->prev = ret;
    }
    else
    {
      float grad = (before->next->x - before->x) / (before->next->y - before->y);
      float x = before->x + grad * (actual - before->y);
      ret = new point(x, y, before->next, before);
      before->next->prev = ret;
      before->next = ret;
    }
  }
  return ret;
}

void Field::get_extremes()
{
  float x_max = field[0]->x;
  float x_min = field[0]->x;
  float y_max = field[0]->y;
  float y_min = field[0]->y;

  for(int i = 1; i < field.size(); i++)
  {
    if(field[i]->x > x_max)
    {
      x_max = field[i]->x;
    }
    if(field[i]->x < x_min)
    {
      x_min = field[i]->x;
    }
    if(field[i]->y > y_max)
    {
      y_max = field[i]->y;
    }
    if(field[i]->y < y_min)
    {
      y_min = field[i]->y;
    }
  }
  x_extreme = std::pair(x_min, x_max);
  y_extreme = std::pair(y_min, y_max);
}






