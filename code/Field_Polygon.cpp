#include "Field_Polygon.hpp"

Field::Field(point* start, const char* name)
{
  field_name = name;
  VW_sample(start);
}

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

float Field::VW_area(point* targ)
{
  float prev_x = targ->prev->x;
  float prev_y = targ->prev->y;
  float next_x = targ->next->x;
  float next_y = targ->next->y;
  
  float ret = prev_x * targ->y + targ->x * next_y + next_x * prev_y;
  ret -= prev_x * next_y + targ->x * prev_y + next_x * targ->y;
  return 0.5 * std::abs(ret);
}

