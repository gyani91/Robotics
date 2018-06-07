#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from math import radians
import roslib
roslib.load_manifest('learning_tf')
import math
import tf
import geometry_msgs.msg
import turtlesim.srv
from turtlesim.msg import Pose

PI = 3.1415926535897


#def euclidean_distance(goal_pose):
    #"""Euclidean distance between current pose and the goal."""
    #return sqrt(pow((goal_pose.x - self.pose.x), 2) + pow((goal_pose.y - pose.y), 2))


def move(distance,speed,angle,angle_speed,tag):
    #rospy.init_node('robot_cleaner', anonymous=True)
    velocity_publisher = rospy.Publisher('/turtle2/cmd_vel', Twist, queue_size=10)
    vel_msg = Twist()

    angular_speed = angle_speed*2*PI/360
    relative_angle = angle*2*PI/360

    vel_msg.linear.y = 0
    vel_msg.linear.z = 0
    vel_msg.angular.x = 0
    vel_msg.angular.y = 0
    #vel_msg.angular.z = 0 
    #vel_msg.angular.z =abs(angular_speed)

    if(angle>=0):
        vel_msg.angular.z =abs(angular_speed)
    else:
        vel_msg.angular.z =-abs(angular_speed)

    if(distance>=0):
        vel_msg.linear.x = abs(speed)
    else:
        vel_msg.linear.x = -abs(speed)
    #vel_msg.linear.x = speed

    
    while not rospy.is_shutdown():

    	t0 = rospy.Time.now().to_sec()
    	current_distance = 0
    	current_angle = 0

    	while(current_distance <= abs(distance) and current_angle <= abs(relative_angle)):

            	velocity_publisher.publish(vel_msg)
            	#Takes actual time to velocity calculus
            	t1=rospy.Time.now().to_sec()
            	#Calculates distancePoseStamped

            	current_distance= abs(speed*(t1-t0))

	    	current_angle = abs(angular_speed*(t1-t0))
	    	print(tag)


    	#After the loop, stops the robot
    	vel_msg.linear.x = 0
    	vel_msg.angular.z = 0
    	#Force the robot to stop
    	velocity_publisher.publish(vel_msg)
    	#rospy.spin()
    	break

def check(listener, turtle_vel):
    (trans,rot) = listener.lookupTransform('/turtle2','/turtle1', rospy.Time(0))
    if(math.sqrt(trans[0] ** 2 + trans[1] ** 2)<0.5):
	chase(listener, turtle_vel)

def chase(listener, turtle_vel):
    while not rospy.is_shutdown():
	(trans,rot) = listener.lookupTransform('/turtle2','/turtle1', rospy.Time(0))
	angular = 4 * math.atan2(trans[1], trans[0])
	linear = 0.5 * math.sqrt(trans[0] ** 2 + trans[1] ** 2)
	cmd = geometry_msgs.msg.Twist()
	cmd.linear.x = linear
	cmd.angular.z = angular
	turtle_vel.publish(cmd)

if __name__ == '__main__':

    rospy.init_node('turtle_tf_listener')

    listener = tf.TransformListener()
    turtle_vel = rospy.Publisher('turtle2/cmd_vel', geometry_msgs.msg.Twist, queue_size=1)
    rospy.wait_for_service('spawn')
    spawner = rospy.ServiceProxy('spawn', turtlesim.srv.Spawn)
    #spawner(4, 2, 0, 'turtle1')
    spawner(4, 2, 45*2*PI/360, 'turtle2')


    while not rospy.is_shutdown():
        try:
            #(trans,rot) = listener.lookupTransform('/turtle2','/turtle1', rospy.Time(0))

    	    #goal_pose = Pose()
    	    #goal_pose.x = trans[1]
    	    #goal_pose.y = trans[0]
    	    # For U
	    check(listener, turtle_vel)
    	    move(-4,2,-130,180,1)
	    check(listener, turtle_vel)		
    	    move(-2,1,0,0,2)
	    check(listener, turtle_vel)
    	    move(2,1,0,0,2)
	    check(listener, turtle_vel)
    	    move(1,2,130,180,1)
	    check(listener, turtle_vel)
    	    move(1,2,130,180,1)
	    check(listener, turtle_vel)
    	    move(2,1,0,0,2)
	    check(listener, turtle_vel)
    	    move(-2,1,0,0,2)
	    check(listener, turtle_vel)
    	    # For S
    	    move(-15,2,180,200,1)
	    check(listener, turtle_vel)
	    move(-5,2,80,200,1)
	    check(listener, turtle_vel)
    	    move(-10,2,-90,200,1)
	    check(listener, turtle_vel)
    	    move(-12,2,-80,200,1)
	    check(listener, turtle_vel)		
    	    # For I
    	    move(-2.5,2,0,0,1)
	    check(listener, turtle_vel)
    	    move(1.3,2,0,0,1)
	    check(listener, turtle_vel)
    	    move(0,0,90,100,2)
	    check(listener, turtle_vel)
    	    move(2,2,0,0,1)
	    check(listener, turtle_vel)
    	    move(0,0,90,100,2)
	    check(listener, turtle_vel)
    	    move(1.3,2,0,0,1)
	    check(listener, turtle_vel)
    	    move(-2,2,0,0,1)
	    check(listener, turtle_vel)
	    break
        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
            continue
        except rospy.ROSInterruptException:
	    pass
    while not rospy.is_shutdown():
	check(listener, turtle_vel)
