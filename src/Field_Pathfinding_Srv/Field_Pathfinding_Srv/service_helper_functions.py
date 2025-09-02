from typing import NamedTuple
import json

class point_object(NamedTuple):
    x: float
    y: float

def load_json(field_id):
    with open(str(field_id)&'.json', 'r') as file:
        data = json.load(file)


def end_of_field_pos(arrable_multiplier, implement_width, points): 
    """
     # For the input arr of points you can extrapolate the points to the extremities of the implement width using
     # trigonometry as it is a line from last point to point in screen(case of both off just add 2 at both extremities)
    """
    sum_arrable = 0
    trial_count = 0
    trial_height = points[trial_count].y
    computational_height = None
    computational_width = 0
    error_value = None
    optimal_height = None
    while trial_height != (min(points, key=lambda obj: obj.y)).y:
        for point in points:
            if(computational_height != None):
                computational_height -= trial_height if point.y > trial_height else point.y
                computational_width -= (point.x)
                sum_arrable += abs(computational_height)*abs(computational_width)*0.5 + abs(computational_width)*abs(point.y)
            computational_height = trial_height if point.y > trial_height else point.y
            computational_width = point.x

        current_error_value = (sum_arrable*arrable_multiplier)/(trial_height*implement_width)
        if (error_value == None):
            error_value = current_error_value
            optimal_height = trial_height
        else:
            if(current_error_value > error_value):
                error_value = current_error_value
                optimal_height = trial_height
    return optimal_height


