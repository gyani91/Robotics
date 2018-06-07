#!/usr/bin/env python
import rospy
import sys

import numpy as np
from geometry_msgs.msg import Pose, Twist
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Range
from math import cos, sin, asin, tan, atan2
# msgs and srv for working with the set_model_service
from gazebo_msgs.msg import ModelState
from gazebo_msgs.srv import SetModelState
from std_srvs.srv import Empty

from visualization_msgs.msg import Marker
from geometry_msgs.msg import Quaternion, Pose, Point, Vector3
from std_msgs.msg import Header, ColorRGBA

import math

# a handy tool to convert orientations
from tf.transformations import euler_from_quaternion, quaternion_from_euler

class BasicThymio:

    def __init__(self, thymio_name):
        """init"""
        self.thymio_name = thymio_name
        rospy.init_node('basic_thymio_controller', anonymous=True)

        # Publish to the topic '/thymioX/cmd_vel'.
        self.velocity_publisher = rospy.Publisher(self.thymio_name + '/cmd_vel',
                                                  Twist, queue_size=10)

        # A subscriber to the topic '/turtle1/pose'. self.update_pose is called
        # when a message of type Pose is received.
        self.pose_subscriber = rospy.Subscriber(self.thymio_name + '/odom',
                                                Odometry, self.update_state)

        self.prox_center_sub = rospy.Subscriber(self.thymio_name + '/proximity/center',
            Range, self.update_prox_center)
        self.prox_center_left_sub = rospy.Subscriber(self.thymio_name + '/proximity/center_left',
            Range, self.update_prox_center_left)
        self.prox_center_right_sub = rospy.Subscriber(self.thymio_name + '/proximity/center_right',
            Range, self.update_prox_center_right)
        self.prox_left_sub = rospy.Subscriber(self.thymio_name + '/proximity/left',
            Range, self.update_prox_left)
        self.prox_right_sub = rospy.Subscriber(self.thymio_name + '/proximity/right',
            Range, self.update_prox_right)

        self.prox_rear_left_sub = rospy.Subscriber(self.thymio_name + '/proximity/rear_left',
            Range, self.update_prox_rear_left)
        self.prox_rear_right_sub = rospy.Subscriber(self.thymio_name + '/proximity/rear_right',
            Range, self.update_prox_rear_right)

        self.current_pose = Pose()
        self.current_twist = Twist()
        self.current_prox_center = Range()
        self.current_prox_center_left = Range()
        self.current_prox_center_right = Range()
        self.current_prox_left = Range()
        self.current_prox_right = Range()
        # publish at this rate
        self.tick_rate = 50
        self.rate = rospy.Rate(self.tick_rate)

        self.vel_msg = Twist()
        
    def update_prox_center(self, data):
        self.current_prox_center = data
    def update_prox_center_left(self, data):
        self.current_prox_center_left = data
    def update_prox_center_right(self, data):
        self.current_prox_center_right = data
    def update_prox_left(self, data):
        self.current_prox_left = data
    def update_prox_right(self, data):
        self.current_prox_right = data

    def update_prox_rear_left(self, data):
        self.current_prox_rear_left = data
    def update_prox_rear_right(self, data):
        self.current_prox_rear_right = data

    def thymio_state_service_request(self, position, orientation):
        """Request the service (set thymio state values) exposed by
        the simulated thymio. A teleportation tool, by default in gazebo world frame.
        Be aware, this does not mean a reset (e.g. odometry values)."""
        rospy.wait_for_service('/gazebo/set_model_state')
        try:
            model_state = ModelState()
            model_state.model_name = self.thymio_name
            model_state.reference_frame = '' # the frame for the pose information
            model_state.pose.position.x = position[0]
            model_state.pose.position.y = position[1]
            model_state.pose.position.z = position[2]
            qto = quaternion_from_euler(orientation[0], orientation[0], orientation[0], axes='sxyz')
            model_state.pose.orientation.x = qto[0]
            model_state.pose.orientation.y = qto[1]
            model_state.pose.orientation.z = qto[2]
            model_state.pose.orientation.w = qto[3]
            # a Twist can also be set but not recomended to do it in a service
            gms = rospy.ServiceProxy('/gazebo/set_model_state', SetModelState)
            response = gms(model_state)
            return response
        except rospy.ServiceException, e:
            print "Service call failed: %s"%e

    def update_state(self, data):
        """A new Odometry message has arrived. See Odometry msg definition."""
        # Note: Odmetry message also provides covariance
        self.current_pose = data.pose.pose
        self.current_twist = data.twist.twist
        quat = (
            self.current_pose.orientation.x,
            self.current_pose.orientation.y,
            self.current_pose.orientation.z,
            self.current_pose.orientation.w)
        (roll, pitch, yaw) = euler_from_quaternion (quat)
        # rospy.loginfo("State from Odom: (%.5f, %.5f, %.5f) " % (self.current_pose.position.x, self.current_pose.position.y, yaw))

    def turn_left(self, speed):
        self.vel_msg.angular.z = speed
        # print(' turning left')
        self.velocity_publisher.publish(self.vel_msg)

    def turn_right(self, speed):
        self.vel_msg.angular.z = -speed
        self.velocity_publisher.publish(self.vel_msg)

    def make_figure_8(self):
        ang_speed = 1.0
        full_circle_duration = rospy.Duration(2 * math.pi / ang_speed)

        start = rospy.Time()
        while start.secs == 0:
            start = rospy.get_rostime()

        print('start: ' + str(start.nsecs))

        self.vel_msg.linear.x = 0.2
        
        while not rospy.is_shutdown():
            now = rospy.get_rostime()

            if (now - start) < full_circle_duration:
                self.turn_left(ang_speed)
            elif (now - start) < 2 * full_circle_duration:
                self.turn_right(ang_speed)
            else:
                break
            self.rate.sleep()
        

        self.vel_msg.linear.x = 0
        self.vel_msg.angular.z = 0
        self.velocity_publisher.publish(self.vel_msg)
        
        rospy.spin()

    def is_close_to_wall(self):
        return (0 < self.current_prox_left.range < 0.08) or \
            (0 < self.current_prox_center_left.range < 0.08) or \
            (0 < self.current_prox_center.range < 0.08) or \
            (0 < self.current_prox_center_right.range < 0.08) or \
            (0 < self.current_prox_right.range < 0.08)
        
    def drive_to_wall(self):
        self.vel_msg.linear.x = 0.1
        self.velocity_publisher.publish(self.vel_msg)

        close = self.is_close_to_wall()

        while not close:
            self.velocity_publisher.publish(self.vel_msg)
            close = self.is_close_to_wall()

            self.rate.sleep()

        self.vel_msg.linear.x = 0
        self.velocity_publisher.publish(self.vel_msg)

    # TODO: it seems that if this script is started when the thymio
    # is already close to the wall, the thymio sometimes rotated the wrong way
    # Also, we might want to make it proportional, and maybe incorporate
    # the sensor inputs of center_left and center_right as well
    def face_wall(self):
        wall_is_left = self.current_prox_left.range < self.current_prox_right.range
        last_prox_center = self.current_prox_center
        if wall_is_left:
            ang_vel = 0.1
        else:
            ang_vel = -0.1
        
        while abs(self.current_prox_center_left.range - self.current_prox_center_right.range) > 0.005:
            self.vel_msg.angular.z = ang_vel
            self.velocity_publisher.publish(self.vel_msg)
            self.rate.sleep()
        
        self.vel_msg.angular.z = 0.0
        self.velocity_publisher.publish(self.vel_msg)

    # TODO: might want to have a proportional controller
    # Also, need to incorporate rear sensor inputs to get perfect final orientation
    # (now it just turns 180 degrees and ignores rear sensor inputs)
    def turn_around(self):
        print('turning around')
        ang_speed = 1.0
        half_circle_duration = rospy.Duration(math.pi / ang_speed)

        start = rospy.Time()
        while start.secs == 0:
            start = rospy.get_rostime()

        print('start: ' + str(start.nsecs))

        while rospy.get_rostime() - start < half_circle_duration:
            self.vel_msg.angular.z = ang_speed
            self.velocity_publisher.publish(self.vel_msg)
            self.rate.sleep()

        self.vel_msg.angular.z = 0
        self.velocity_publisher.publish(self.vel_msg)

    # FIXME: thymio stops too late, probably because the sensors are not in the
    # same location as the base_link
    def drive_forward(self):
        current_dist = self.current_prox_rear_left.range
        dist_to_go = 2.0 - current_dist
        lin_vel = 0.2
        fwd_duration = rospy.Duration(dist_to_go / lin_vel)

        start = rospy.Time()
        while start.secs == 0:
            start = rospy.get_rostime()

        print('start: ' + str(start.nsecs))

        while rospy.get_rostime() - start < fwd_duration:
            self.vel_msg.linear.x = lin_vel
            self.velocity_publisher.publish(self.vel_msg)
            self.rate.sleep()

        self.vel_msg.linear.x = 0
        self.velocity_publisher.publish(self.vel_msg)


    def task1(self):
        self.make_figure_8()

    def task2(self):
        self.drive_to_wall()
        self.face_wall()

    def task3(self):
        self.drive_to_wall()
        self.face_wall()
        self.turn_around()
        self.drive_forward()

        

def usage():
    return "Wrong number of parameters. basic_move.py [thymio_name]"

if __name__ == '__main__':
    if len(sys.argv) == 2:
        thymio_name = sys.argv[1]
        print "Now working with robot: %s" % thymio_name
    else:
        print usage()
        sys.exit(1)
    thymio = BasicThymio(thymio_name)

    # Teleport the robot to a certain pose. If pose is different to the
    # origin of the world, you must account for a transformation between
    # odom and gazebo world frames.
    # NOTE: The goal of this step is *only* to show the available
    # tools. The launch file process should take care of initializing
    # the simulation and spawning the respective models
    
    #thymio.thymio_state_service_request([0.,0.,0.], [0.,0.,0.])
    #rospy.sleep(1.)

    thymio.task1()
    # thymio.task2()
    # thymio.task3()
