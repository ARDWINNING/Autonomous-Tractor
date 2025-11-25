#include "Field.hpp"

Field::Field(point* start, obstacle* InitObstacle, const char* name)
  : initial_obstacle(InitObstacle), field_name(name) {
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

// If possible add in logic to combine cells that are split by an obstacles boundary not impacting the cell, I.E. obstacle1 is below Obstacle2 and celli is bellow obstacle one, it will be split by obstacle2's boundary. make a clockwise linked list structure with completed cells and then check neighbours 

bool Field::BCD()
{
  // Got to find and update place (curr_boundary)
  int curr_boundary = 1; // Placeholder
  std::vector<std::pair<float, float>> boundaries;
  point* curr = field_start;
  std::vector<std::vector<point*>> cell(boundaries.size());
  obstacle* ob;

  float comparator = curr->next->x < curr->x ? boundaries[curr_boundary].second : boundaries[curr_boundary].first;
  do
  {
    curr = curr->prev;
  } while (curr->next->x < curr->x ? comparator > curr->prev->x : comparator < curr->prev->x);
  
  for(int i = 0; i < boundaries.size(); i++)
  {
    float front = curr->next->x < curr->x ? boundaries[curr_boundary].first : boundaries[curr_boundary].second;
    float back = curr->next->x < curr->x ? boundaries[curr_boundary].second : boundaries[curr_boundary].first;

    cell[i].push_back(add(curr, back));
    while(curr->next->x < curr->x ? curr->next->x > front : curr->next->x < front)
    {
      cell[i].push_back(curr);
    }
    cell[i].push_back(add(curr, front));
    ob = find_obstacle(curr, boundaries[curr_boundary]);
    if(ob)
    {
      std::vector<point*> points;
      if(curr->next->x < curr->x)
      {
        points = obstacle_points(ob, boundaries[curr_boundary], 1);
      }
      else
      {
        points = obstacle_points(ob, boundaries[curr_boundary], 0);
      }

      points[0]->prev = cell[i][cell[i].size()-1];
      cell[i][cell[i].size()-1]->next = points[0];
      for(int j = 0; j < points.size(); j++)
      {
        cell[i].push_back(points[i]);
      }
      cell[i][cell[i].size()-1]->next = cell[i][0];
      cell[i][0]->next = cell[i][cell[i].size()-1];
    }
  }
  return true;
}

obstacle* Field::find_obstacle(point* p, std::pair<float, float> extr)
{
  bool dir = p->next->x < p->x;
  obstacle* out = nullptr;
  point* curr = nullptr;
  point* mid;
  for(obstacle* o = initial_obstacle; o != nullptr; o = o->next)
  {
    if(o->x_extremes.first <= p->x && o->x_extremes.second >= p->x)
    {
      point* mid = o->central_line;
      do{
        mid = mid->next;
      }while(mid->x < (extr.second - extr.first)/2 + extr.first);
      
      if(!curr)
      {
        curr = mid;
        out = o;
      }
      else if(dir && mid->y > curr->y) 
      {
        curr = mid;
        out = o;
      }
      else if(mid->y < curr->y)
      {
        curr = mid;
        out = o;
      }
    }
  }
  return out;
}

std::vector<point*> Field::obstacle_points(obstacle* o, std::pair<float, float> extr, int pos)
{
  //check that the bounds are included as points if not infer them with add()
  std::vector<point*> inter;
  std::vector<point*> out;
  point* curr;
  for(int i = 0; i < o->perimeter.size(); i++)
  {
    if(extr.first <= o->perimeter[i]->x && o->perimeter[i]->x <= extr.second)
    {
      inter.push_back(o->perimeter[i]);
    }
  }
  if(pos == 0) // decsencding
  {
    std::sort(inter.begin(), inter.end(), [](const point* a, const point* b) 
              {return a->x > b->x;}); 
    if(inter[0]->y < inter[1]->y && inter[1] != inter[0]->next)
    {
      curr = inter[0];
    } else {
      curr = inter[1];
    }
    do {
      out.push_back(curr);
      curr = curr->next;
    } while (curr->x >= extr.first);

    if(out[0]->x != extr.second)
    {
      out.emplace(out.begin(), infer_point(out[0]->prev, extr.second, 'x'));
    }
    if(out[out.size()-1]->x != extr.first)
    {
      out.emplace(out.begin(), infer_point(out[out.size()-1], extr.first, 'x'));
    }
  }
  else if(pos == 1) // ascending
  {
    std::sort(inter.begin(), inter.end(), [](const point* a, const point* b) 
              {return a->x < b->x;}); 
    if(inter[0]->y > inter[1]->y && inter[1] != inter[0]->prev)
    {
      curr = inter[0];
    } else {
      curr = inter[1]; // this is simplistic in both most liekly need a loop in the future
    }
    do {
      out.push_back(curr);
      curr = curr->prev;
    } while (curr->x <= extr.second);

    if(out[0]->x != extr.first)
    {
      out.emplace(out.begin(), infer_point(out[0]->prev, extr.first, 'x'));
    }
    if(out[out.size()-1]->x != extr.second)
    {
      out.emplace(out.begin(), infer_point(out[out.size()-1], extr.second, 'x'));
    }
  }
  return out;
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






