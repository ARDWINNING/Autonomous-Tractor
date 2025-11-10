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

bool Field::BCD_x()
{
  std::vector<float> boundaries;
  point* curr = field[0];
  int count = 0;
  bool done = false;
  std::vector<point*> fill = {};  
  cells.push_back(fill);
  for(int i = 0; i < obstacles.size(); i++)
  {
    cells.push_back(fill);
    cells.push_back(fill);
    boundaries.push_back(obstacles[i]->x_extremes.first);
    boundaries.push_back(obstacles[i]->x_extremes.second);
  }

  std::sort(boundaries.begin(), boundaries.end());
  int place = boundaries.size();
  for(int j = 0; j < boundaries.size(); j++)
  {
    if(curr->x < boundaries[j])
    {
      place = j;
      break;
    }
  }
  point* end = curr->prev;

  while(curr != end)
  {
    if(curr->x < boundaries[place])
    {
      if(place == 0)
      {
        add(curr, place);
      }
      else if(curr->x > boundaries[place-1])
      {
        add(curr, place);
      }
      else
      {
        cells[place].emplace(cells[place].begin(), infer_point(curr->prev, boundaries[place-1], 'x'));
        place--;
      }
    }
    else
    {
      cells[place+1].emplace(cells[place+1].begin(), infer_point(curr->prev, boundaries[place], 'x'));
      place++;
    }
    if(curr != end->next && !done)
    {
      end = end->next;
      done = true;
    }
  }
  return true;
}

bool Field::BCD_y()
{
  for(int i = 1; i < cells.size() - 2; i++) // First and last have no obstacles * look for cutoff sections 
  {
    // Loop through  
  }
  return true;
}

point* Field::add(point* p, int level)
{
  cells[level].emplace(cells[level].begin(), p);
  if(cells[level].size() > 1)
  {
    if(p->prev != cells[level][1])
    {
      p->prev = cells[level][1];
    }
  }
  return p->next;
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






