from typing import NamedTuple
from typing import List
from math import acos, atan, cos, sin, tan, pi

class point_object(NamedTuple):
    x: float
    y: float

def end_of_field_pos(arrable_multiplier: float, implement_width: float, points: list[point_object]) -> point_object: 
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
    if optimal_height is None:
        optimal_height = float(points[0].y)
    return point_object(x = (points[0].x + 0.5*implement_width), y = optimal_height)

def combine_points(reverse: bool, master_points: list[point_object], insertion_points: list[point_object], insertion_index: int) -> int:
    for point in insertion_points:    
        master_points.insert(insertion_index, point)
        if(not reverse):
            insertion_index += 1
    return insertion_index                            

def clockwise_turn_loop(start_angle: float, end_angle: float, granularity: int, turn_points: list[point_object], turning_radius: float, polarity: tuple[bool, bool, bool]):
    increment = pi/granularity
    turn_angle = start_angle + increment
    pol_x, pol_y, add_turn = polarity
    while turn_angle <= end_angle:
        if(turn_angle < pi/2):
            aug_x = (turning_radius - turning_radius * cos(turn_angle)) * 1 if pol_x else -1
            aug_y = (turning_radius * sin(turn_angle)) * 1 if pol_y else -1
            turn_points.append(point_object(x = (turn_points[0].x + aug_x), y = (turn_points[0].y + aug_y)))
        else:
            aug_x = (turning_radius if add_turn else 0 + (turning_radius * sin(turn_angle - pi/2))) * 1 if pol_x else -1
            aug_y = (turning_radius - turning_radius * cos(turn_angle - pi/2)) * 1 if pol_y else -1
            turn_points.append(point_object(x = (turn_points[0].x + aug_x), y = (turn_points[0].y + aug_y)))
        if((turn_angle != end_angle) and ((turn_angle + increment) > end_angle)):
            turn_angle = end_angle
        else:
            turn_angle += increment
    return turn_points

def anticlockwise_turn_loop(start_angle: float, end_angle: float, granularity: int, turn_points: list[point_object], turning_radius: float, polarity: tuple[bool, bool, bool]):
    increment = pi / granularity
    turn_angle = start_angle - increment
    pol_x, pol_y, add_turn = polarity
    while turn_angle >= end_angle:
        if turn_angle > -pi / 2:
            aug_x = (turning_radius - turning_radius * cos(turn_angle)) * (1 if pol_x else -1)
            aug_y = (turning_radius * sin(turn_angle)) * (1 if pol_y else -1)
            turn_points.append(point_object(x=(turn_points[0].x + aug_x), y=(turn_points[0].y + aug_y)))
        else:
            aug_x = ((turning_radius if add_turn else 0) + (turning_radius * sin(turn_angle + pi / 2))) * (1 if pol_x else -1)
            aug_y = (turning_radius - turning_radius * cos(turn_angle + pi / 2)) * (1 if pol_y else -1)
            turn_points.append(point_object(x=(turn_points[0].x + aug_x), y=(turn_points[0].y + aug_y)))
        if (turn_angle != end_angle) and ((turn_angle - increment) < end_angle):
            turn_angle = end_angle
        else:
            turn_angle -= increment
    return turn_points

def turn_height_smoothing(granularity: int, theta: float, turning_disparity: float, turning_radius: float, points: list[point_object], insertion_index: int = 1) -> int:
    swarth_height_offset = points[insertion_index].y-points[insertion_index-1].y
    if(turning_disparity >= 0):
        unrecoverable_height_offset = abs(swarth_height_offset) - turning_disparity*tan(theta)
        if((swarth_height_offset >= 0) and (unrecoverable_height_offset !=0)):
            points.insert(insertion_index, point_object(x = (points[insertion_index-1].x), y = (points[insertion_index-1].y + unrecoverable_height_offset)))
            insertion_index += 1
        elif(unrecoverable_height_offset !=0):
            points.insert(insertion_index, point_object(x = (points[insertion_index].x), y = (points[insertion_index].y + unrecoverable_height_offset)))
        return insertion_index
    standard_turn_width = 2*turning_radius*(1-cos(pi/4))
    if((turning_disparity + standard_turn_width) < 0):
        phi = acos(1+ turning_disparity/(2*turning_radius))
        temp_list: list[point_object] = []
        if(swarth_height_offset >= 0):
            temp_list.append(points[insertion_index-1])
            anticlockwise_turn_loop(0, phi, granularity, temp_list, turning_radius, (False, True, False))
            temp_list.remove(temp_list[0])
            insertion_index = combine_points(False, points, temp_list, insertion_index)
            temp_list = [points[insertion_index-1]]
            clockwise_turn_loop(pi-phi, pi, granularity, temp_list, turning_radius, (False, True, False))
            temp_list.remove(temp_list[0])
            insertion_index = combine_points(False, points, temp_list, insertion_index)
        else:
            temp_list.append(points[insertion_index])
            clockwise_turn_loop(0, phi, granularity, temp_list, turning_radius, (True, True, False))
            temp_list.remove(temp_list[0])
            insertion_index = combine_points(True, points, temp_list, insertion_index)
            temp_list = [points[insertion_index]]
            anticlockwise_turn_loop(pi-phi, pi, granularity, temp_list, turning_radius, (True, True, False))
            temp_list.remove(temp_list[0])
            insertion_index = combine_points(True, points, temp_list, insertion_index)
    else:
        temp_list: list[point_object] = []
        length = (turning_disparity + standard_turn_width)/cos(pi/4)
        aug_y = length * sin(pi/4)
        if(swarth_height_offset >= 0):
            temp_list.append(points[insertion_index-1])
            anticlockwise_turn_loop(0, pi/4, granularity, temp_list, turning_radius, (False, True, False))
            temp_list.remove(temp_list[0])
            insertion_index = combine_points(False, points, temp_list, insertion_index)
            aug_x = -length * cos(pi/4)
            points.insert(insertion_index, point_object(x = (points[insertion_index-1].x + aug_x), y = (points[insertion_index-1].y + aug_y)))
            insertion_index += 1
            temp_list = [points[insertion_index-1]]
            clockwise_turn_loop(pi-pi/4, pi, granularity, temp_list, turning_radius, (False, True, False))
            temp_list.remove(temp_list[0])
            insertion_index = combine_points(False, points, temp_list, insertion_index)
        else:
            temp_list.append(points[insertion_index])
            clockwise_turn_loop(0, pi/4, granularity, temp_list, turning_radius, (True, True, False))
            temp_list.remove(temp_list[0])
            insertion_index = combine_points(True, points, temp_list, insertion_index)
            aug_x = length * cos(pi/4)
            points.insert(insertion_index, point_object(x = (points[insertion_index].x + aug_x), y = (points[insertion_index].y + aug_y)))
            temp_list = [points[insertion_index]]
            anticlockwise_turn_loop(pi-pi/4, pi, granularity, temp_list, turning_radius, (True, True, False))
            temp_list.remove(temp_list[0])
            insertion_index = combine_points(True, points, temp_list, insertion_index)

    insertion_index = turn_height_smoothing(granularity, atan(abs(points[insertion_index].y - points[insertion_index-1].y)/abs(points[insertion_index].x - points[insertion_index-1].x)), 0, turning_radius, points, insertion_index)
    return insertion_index

def bounded_turn(granularity: int,  insertion_index: int, theta: float, turning_radius: float, points: list[point_object]) -> int:
    if(points[insertion_index].y < points[insertion_index-1].y):
        phi = theta
        slope_down = False
    else:
        phi = pi/2 + theta
        slope_down = True
    theta = atan((abs(points[insertion_index].y - points[insertion_index-1].y))/(turning_disparity))
    turning_disparity = (points[insertion_index].x-points[insertion_index - 1].x) - 2 * turning_radius
    temp_list = [points[insertion_index - 1]]
    clockwise_turn_loop(0, phi, granularity, temp_list, turning_radius, (True, True, True))
    temp_list.remove(temp_list[0])
    insertion_index = combine_points(False, points, temp_list, insertion_index)
    if(turning_disparity):
        aug_x = turning_disparity
        aug_y = turning_disparity * tan(theta) * -1 if slope_down else 1
        points.insert(insertion_index, point_object(x = (points[insertion_index - 1] + aug_x), y = (points[insertion_index - 1] + aug_y)))
    temp_list = [points[insertion_index-1]]
    clockwise_turn_loop(phi, pi, granularity, temp_list, turning_radius, (True, False, False))
    temp_list.remove(temp_list[0])
    insertion_index = combine_points(False, points, temp_list, insertion_index) 
    return insertion_index 

def end_of_field_turn(granularity: int, implement_width: float, turning_radius: float, points: list[point_object]): # For under field where smaller y is away from field wrap the whole call in a transformer that takes points and rectifies the numbers and undoes at the end by making all input y/x negative?
    """
     # The points input is a list consisting of the output of e_o_f_pos for the end of swarth a and beginning of swarth b and theta 2 is the ang
     # This will then determine the correct turn and produce a list of points to follow for the turn 
    """
    turning_disparity = implement_width - 2 * turning_radius
    theta = atan((abs(points[1].y - points[0].y))/(implement_width)) 
    running_index = turn_height_smoothing(granularity, theta, turning_disparity, turning_radius, points) # Look at code to ensure all good with the augmented passing structure
    running_index = bounded_turn(granularity, running_index, theta, turning_radius, points)
    return
