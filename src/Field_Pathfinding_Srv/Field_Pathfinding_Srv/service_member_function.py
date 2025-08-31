from pathfinding_interfaces.srv import FPathfinding

import rclpy
from rclpy.executors import ExternalShutdownException
from rclpy.node import Node


class FieldPathfindingService(Node):

    def __init__(self):
        super().__init__('field_pathfinding_service')
        self.srv = self.create_service(FPathfinding, 'Field_Pathfinding', self.fpathfinding_callback)

    def fpathfinding_callback(self, request, response):
        #do stuff
        response.fieldID
        response.implementWidth
        response.turningRadius
        response.orientationPreference #WbySW/NbyNW/none

        response.x = [0]
        response.y = [0] 
        return response


def main():
    try:
        with rclpy.init():
            Field_Pathfinding = FieldPathfindingService()
            rclpy.spin(Field_Pathfinding)

    except (KeyboardInterrupt, ExternalShutdownException):
        pass

if __name__ == '__main__':
    main()
