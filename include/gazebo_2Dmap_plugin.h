/*
 * Copyright 2015 Fadri Furrer, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Michael Burri, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Mina Kamel, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Janosch Nikolic, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Markus Achtelik, ASL, ETH Zurich, Switzerland
 * Copyright 2016 Geoffrey Hunter <gbmhunter@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GAZEBO_2DMAP_PLUGIN_H
#define GAZEBO_2DMAP_PLUGIN_H

#include <iostream>
#include <math.h>

#include <common.h>
#include <gazebo/common/common.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <ros/ros.h>
#include <sdf/sdf.hh>
#include <std_srvs/Empty.h>
#include <nav_msgs/OccupancyGrid.h>
#include <thread>

namespace gazebo {

/// \brief    Octomap plugin for Gazebo.
/// \details  This plugin is dependent on ROS, and is not built if NO_ROS=TRUE is provided to
///           CMakeLists.txt. The PX4/Firmware build does not build this file.
class OccupancyMapFromWorld : public WorldPlugin {
 public:
  OccupancyMapFromWorld()
      : WorldPlugin(), name_("gazebo_2Dmap_plugin")
  {
    ROS_INFO_NAMED(name_, "occupancy map plugin started");
  }
  virtual ~OccupancyMapFromWorld();



 protected:

  /// \brief Load the plugin.
  /// \param[in] _parent Pointer to the world that loaded this plugin.
  /// \param[in] _sdf SDF element that describes the plugin.
  void Load(physics::WorldPtr _parent, sdf::ElementPtr _sdf);

  bool worldCellIntersection(const math::Vector3& cell_center, const double cell_length,
                             gazebo::physics::RayShapePtr ray);

//  void FloodFill(const math::Vector3& seed_point,
//                 const math::Vector3& bounding_box_origin,
//                 const math::Vector3& bounding_box_lengths,
//                 const double leaf_size);
  
  /*! \brief
  */
  void CreateOccupancyMap();

  void OccupancyGridToRviz();

  static void cell2world(unsigned int cell_x, unsigned int cell_y,
                         double map_size_x, double map_size_y, double map_resolution,
                         double& world_x, double &world_y);

  static void world2cell(double world_x, double world_y,
                         double map_size_x, double map_size_y, double map_resolution,
                         unsigned int& cell_x, unsigned int& cell_y);

  static bool cell2index(int cell_x, int cell_y,
                         unsigned int cell_size_x, unsigned int cell_size_y,
                         unsigned int& map_index);

  static bool index2cell(int index, unsigned int cell_size_x, unsigned int cell_size_y,
                         unsigned int& cell_x, unsigned int& cell_y);

 private:
  bool ServiceCallback(std_srvs::Empty::Request& req,
                       std_srvs::Empty::Response& res);

  physics::WorldPtr world_;
  ros::NodeHandle nh_;
  ros::ServiceServer map_service_;
  ros::Publisher map_pub_;
  nav_msgs::OccupancyGrid* occupancy_map_;
  ignition::math::Vector3d map_origin_;
  std::thread occ_grid_rviz_pub_th_;
  double occupancy_map_update_rate_;
  std::string name_;
  std::string full_file_path_;
  double map_resolution_;
  double slice_height_;
  double map_size_x_;
  double map_size_y_;
};

} // namespace gazebo

#endif  // GAZEBO_2DMAP_PLUGIN_H
