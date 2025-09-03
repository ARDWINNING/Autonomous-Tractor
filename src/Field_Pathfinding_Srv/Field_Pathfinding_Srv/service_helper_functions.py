from typing import NamedTuple
import json
from math import atan, cos, sin, tan, pi

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
    return point_object(x = (points[0].x + 0.5*implement_width), y = optimal_height)

def end_of_field_turn(implement_width, turning_radius, points):
    """
     # The points input is a list consisting of the output of e_o_f_pos for the end of swarth a and beginning of swarth b and theta 2 is the ang
     # This will then determine the correct turn and produce a list of points to follow for the turn 
    """
    downward = False
    swarth_height_offset = points[1].y-points[0].y
    if(swarth_height_offset < 0):
        downward = True
    turning_disparity = implement_width - 2 * turning_radius
    running_index = 1
    granularity = pi/16
    if(turning_disparity >= 0):
        theta = atan((abs(swarth_height_offset))/(implement_width)) 
        if(downward):
            initial_run = swarth_height_offset + (turning_disparity*tan(theta))
            points.insert(running_index, point_object(x = (points[0].x + implement_width), y = (points[0].y + initial_run)))
            phi = pi/2 + theta
        else:
            initial_run = swarth_height_offset - (turning_disparity*tan(theta))
            points.insert(running_index, point_object(x = points[0].x, y = (points[0].y + initial_run)))
            running_index += 1
            phi = pi/2 - theta 
        for turn_angle in range(0, phi, granularity):
            if(turn_angle < pi/2):
                aug_x = turning_radius - turning_radius * cos(turn_angle)
                aug_y = turning_radius * sin(turn_angle)
            else:  
                aug_x = turning_radius + turning_radius * cos(turn_angle - pi/2)
                aug_y = turning_radius * sin(turn_angle - pi/2)
            points.insert(running_index, point_object(x = (points[0].x + aug_x), y = (points[0].y + aug_y)))
            running_index += 1
        if(turning_disparity != 0):
            aug_x = 0.5 * turning_disparity * cos(theta)
            aug_y = 0.5 * turning_disparity * sin(theta) * (-1 if downward else 1)
            points.insert(running_index, point_object(x = (points[0].x + aug_x), y = (points[0].y + aug_y)))
        for turn_angle in range(phi, pi, granularity):
            if(turn_angle < pi/2):
                aug_x = turning_radius - turning_radius * cos(turn_angle)
                aug_y = turning_radius * sin(turn_angle)
            else:
                aug_x = turning_radius + turning_radius * cos(turn_angle - pi/2)
                aug_y = turning_radius * sin(turn_angle - pi/2)
            points.insert(running_index, point_object(x = (points[0].x + aug_x), y = (points[0].y + aug_y)))
            running_index += 1
    else:
        # Todo: write the bowed out turn
        pass
    return points
