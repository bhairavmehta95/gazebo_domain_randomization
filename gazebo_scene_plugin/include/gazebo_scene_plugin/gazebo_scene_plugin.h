#ifndef __GAZEBO_ROS_EXTENSION_PLUGIN_HH__
#define __GAZEBO_ROS_EXTENSION_PLUGIN_HH__

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <iostream>

#include <tinyxml.h>

#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/transport/transport.hh>

// ROS
#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <ros/subscribe_options.h>
#include <ros/package.h>
#include <rosgraph_msgs/Clock.h>

// Services
#include "std_srvs/Empty.h"

#include "gazebo_ext_msgs/SetSkyProperties.h"
#include "gazebo_ext_msgs/GetSkyProperties.h"

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>

namespace SkyX
{
  class SkyX;
  namespace VClouds
  {
    class VClouds;
  }
}

namespace gazebo
{

/// \brief A plugin loaded within the gzserver on startup.
class GazeboScenePlugin : public SystemPlugin
{
public:
  /// \brief Constructor
  GazeboScenePlugin();

  /// \brief Destructor
  ~GazeboScenePlugin();

  /// \bried Detect if sig-int shutdown signal is recieved
  void shutdownSignal();

  /// \brief Gazebo-inherited load function
  ///
  /// Called before Gazebo is loaded. Must not block.
  /// Capitalized per Gazebo cpp style guidelines
  /// \param _argc Number of command line arguments.
  /// \param _argv Array of command line arguments.
  void Load(int argc, char** argv);

  /// \brief ros queue thread for this node
  void gazeboQueueThread();

  /// \brief advertise services
  void advertiseServices();

  /// \brief
  bool getSkyProperties(gazebo_ext_msgs::GetSkyProperties::Request &req,gazebo_ext_msgs::GetSkyProperties::Response &res);

  /// \brief
  bool setSkyProperties(gazebo_ext_msgs::SetSkyProperties::Request &req,gazebo_ext_msgs::SetSkyProperties::Response &res);

  /// \brief Connect to Gazebo via its plugin interface, get a pointer to the world, start events
  void loadGazeboScenePlugin(std::string world_name);

  // track if the desconstructor event needs to occur
  bool plugin_loaded_;

  // detect if sigint event occurs
  bool stop_;
  gazebo::event::ConnectionPtr sigint_event_;

  std::string robot_namespace_;

  gazebo::transport::NodePtr gazebonode_;

  boost::shared_ptr<ros::NodeHandle> nh_;
  ros::CallbackQueue gazebo_queue_;
  boost::shared_ptr<boost::thread> gazebo_callback_queue_thread_;

  gazebo::physics::WorldPtr world_;
  gazebo::event::ConnectionPtr load_gazebo_scene_plugin_event_;

  ros::ServiceServer get_sky_properties_service_;
  ros::ServiceServer set_sky_properties_service_;

  // ROS comm
  boost::shared_ptr<ros::AsyncSpinner> async_ros_spin_;

  /// \brief A mutex to lock access to fields that are used in ROS message callbacks
  boost::mutex lock_;

  bool world_created_;

};
}
#endif
