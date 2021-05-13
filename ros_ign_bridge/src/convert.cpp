// Copyright 2018 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ros_ign_bridge/convert.hpp"

#include <limits>
#include <string>

namespace ros_ign_bridge
{

// This can be used to replace `::` with `/` to make frame_id compatible with TF
std::string replace_delimiter(
  const std::string & input,
  const std::string & old_delim,
  const std::string new_delim)
{
  std::string output;
  output.reserve(input.size());

  std::size_t last_pos = 0;

  while (last_pos < input.size()) {
    std::size_t pos = input.find(old_delim, last_pos);
    output += input.substr(last_pos, pos - last_pos);
    if (pos != std::string::npos) {
      output += new_delim;
      pos += old_delim.size();
    }
    last_pos = pos;
  }

  return output;
}


std::string frame_id_ign_to_ros(const std::string & frame_id)
{
  return replace_delimiter(frame_id, "::", "/");
}

template<>
void
convert_ros_to_ign(
  const std_msgs::msg::Bool & ros_msg,
  ignition::msgs::Boolean & ign_msg)
{
  ign_msg.set_data(ros_msg.data);
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Boolean & ign_msg,
  std_msgs::msg::Bool & ros_msg)
{
  ros_msg.data = ign_msg.data();
}

template<>
void
convert_ros_to_ign(
  const std_msgs::msg::Empty &,
  ignition::msgs::Empty &)
{
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Empty &,
  std_msgs::msg::Empty &)
{
}

template<>
void
convert_ros_to_ign(
  const std_msgs::msg::UInt32 & ros_msg,
  ignition::msgs::UInt32 & ign_msg)
{
  ign_msg.set_data(ros_msg.data);
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::UInt32 & ign_msg,
  std_msgs::msg::UInt32 & ros_msg)
{
  ros_msg.data = ign_msg.data();
}

template<>
void
convert_ros_to_ign(
  const std_msgs::msg::Float32 & ros_msg,
  ignition::msgs::Float & ign_msg)
{
  ign_msg.set_data(ros_msg.data);
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Float & ign_msg,
  std_msgs::msg::Float32 & ros_msg)
{
  ros_msg.data = ign_msg.data();
}

template<>
void
convert_ros_to_ign(
  const std_msgs::msg::Float64 & ros_msg,
  ignition::msgs::Double & ign_msg)
{
  ign_msg.set_data(ros_msg.data);
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Double & ign_msg,
  std_msgs::msg::Float64 & ros_msg)
{
  ros_msg.data = ign_msg.data();
}

template<>
void
convert_ros_to_ign(
  const std_msgs::msg::Header & ros_msg,
  ignition::msgs::Header & ign_msg)
{
  ign_msg.mutable_stamp()->set_sec(ros_msg.stamp.sec);
  ign_msg.mutable_stamp()->set_nsec(ros_msg.stamp.nanosec);
  auto newPair = ign_msg.add_data();
  newPair->set_key("frame_id");
  newPair->add_value(ros_msg.frame_id);
}

template<>
void
convert_ros_to_ign(
  const std_msgs::msg::Int32 & ros_msg,
  ignition::msgs::Int32 & ign_msg)
{
  ign_msg.set_data(ros_msg.data);
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Int32 & ign_msg,
  std_msgs::msg::Int32 & ros_msg)
{
  ros_msg.data = ign_msg.data();
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Header & ign_msg,
  std_msgs::msg::Header & ros_msg)
{
  std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
  std::chrono::system_clock::duration dtn = tp.time_since_epoch();
  auto nanosecs = dtn.count();
  auto secs = nanosecs * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;
  ros_msg.stamp = rclcpp::Time(secs, nanosecs%secs);
  // ros_msg.stamp = rclcpp::Time(ign_msg.stamp().sec(), ign_msg.stamp().nsec());
  for (auto i = 0; i < ign_msg.data_size(); ++i) {
    auto aPair = ign_msg.data(i);
    if (aPair.key() == "frame_id" && aPair.value_size() > 0) {
      ros_msg.frame_id = frame_id_ign_to_ros(aPair.value(0));
    }
  }
}

template<>
void
convert_ros_to_ign(
  const std_msgs::msg::String & ros_msg,
  ignition::msgs::StringMsg & ign_msg)
{
  ign_msg.set_data(ros_msg.data);
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::StringMsg & ign_msg,
  std_msgs::msg::String & ros_msg)
{
  ros_msg.data = ign_msg.data();
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Clock & ign_msg,
  rosgraph_msgs::msg::Clock & ros_msg)
{
  ros_msg.clock = rclcpp::Time(ign_msg.sim().sec(), ign_msg.sim().nsec());
}

template<>
void
convert_ros_to_ign(
  const rosgraph_msgs::msg::Clock & ros_msg,
  ignition::msgs::Clock & ign_msg)
{
  ign_msg.mutable_sim()->set_sec(ros_msg.clock.sec);
  ign_msg.mutable_sim()->set_nsec(ros_msg.clock.nanosec);
}

template<>
void
convert_ros_to_ign(
  const geometry_msgs::msg::Quaternion & ros_msg,
  ignition::msgs::Quaternion & ign_msg)
{
  ign_msg.set_x(ros_msg.x);
  ign_msg.set_y(ros_msg.y);
  ign_msg.set_z(ros_msg.z);
  ign_msg.set_w(ros_msg.w);
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Quaternion & ign_msg,
  geometry_msgs::msg::Quaternion & ros_msg)
{
  ros_msg.x = ign_msg.x();
  ros_msg.y = ign_msg.y();
  ros_msg.z = ign_msg.z();
  ros_msg.w = ign_msg.w();
}

template<>
void
convert_ros_to_ign(
  const geometry_msgs::msg::Vector3 & ros_msg,
  ignition::msgs::Vector3d & ign_msg)
{
  ign_msg.set_x(ros_msg.x);
  ign_msg.set_y(ros_msg.y);
  ign_msg.set_z(ros_msg.z);
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Vector3d & ign_msg,
  geometry_msgs::msg::Vector3 & ros_msg)
{
  ros_msg.x = ign_msg.x();
  ros_msg.y = ign_msg.y();
  ros_msg.z = ign_msg.z();
}

template<>
void
convert_ros_to_ign(
  const geometry_msgs::msg::Point & ros_msg,
  ignition::msgs::Vector3d & ign_msg)
{
  ign_msg.set_x(ros_msg.x);
  ign_msg.set_y(ros_msg.y);
  ign_msg.set_z(ros_msg.z);
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Vector3d & ign_msg,
  geometry_msgs::msg::Point & ros_msg)
{
  ros_msg.x = ign_msg.x();
  ros_msg.y = ign_msg.y();
  ros_msg.z = ign_msg.z();
}

template<>
void
convert_ros_to_ign(
  const geometry_msgs::msg::Pose & ros_msg,
  ignition::msgs::Pose & ign_msg)
{
  convert_ros_to_ign(ros_msg.position, *ign_msg.mutable_position());
  convert_ros_to_ign(ros_msg.orientation, *ign_msg.mutable_orientation());
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Pose & ign_msg,
  geometry_msgs::msg::Pose & ros_msg)
{
  convert_ign_to_ros(ign_msg.position(), ros_msg.position);
  convert_ign_to_ros(ign_msg.orientation(), ros_msg.orientation);
}

template<>
void
convert_ros_to_ign(
  const geometry_msgs::msg::PoseStamped & ros_msg,
  ignition::msgs::Pose & ign_msg)
{
  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));
  convert_ros_to_ign(ros_msg.pose, ign_msg);
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Pose & ign_msg,
  geometry_msgs::msg::PoseStamped & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);
  convert_ign_to_ros(ign_msg, ros_msg.pose);
}

template<>
void
convert_ros_to_ign(
  const geometry_msgs::msg::Transform & ros_msg,
  ignition::msgs::Pose & ign_msg)
{
  convert_ros_to_ign(ros_msg.translation, *ign_msg.mutable_position());
  convert_ros_to_ign(ros_msg.rotation, *ign_msg.mutable_orientation());
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Pose & ign_msg,
  geometry_msgs::msg::Transform & ros_msg)
{
  convert_ign_to_ros(ign_msg.position(), ros_msg.translation);
  convert_ign_to_ros(ign_msg.orientation(), ros_msg.rotation);
}

template<>
void
convert_ros_to_ign(
  const geometry_msgs::msg::TransformStamped & ros_msg,
  ignition::msgs::Pose & ign_msg)
{
  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));
  convert_ros_to_ign(ros_msg.transform, ign_msg);

  auto newPair = ign_msg.mutable_header()->add_data();
  newPair->set_key("child_frame_id");
  newPair->add_value(ros_msg.child_frame_id);
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Pose & ign_msg,
  geometry_msgs::msg::TransformStamped & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);
  convert_ign_to_ros(ign_msg, ros_msg.transform);
  for (auto i = 0; i < ign_msg.header().data_size(); ++i) {
    auto aPair = ign_msg.header().data(i);
    if (aPair.key() == "child_frame_id" && aPair.value_size() > 0) {
      ros_msg.child_frame_id = frame_id_ign_to_ros(aPair.value(0));
      break;
    }
  }
}

template<>
void
convert_ros_to_ign(
  const tf2_msgs::msg::TFMessage & ros_msg,
  ignition::msgs::Pose_V & ign_msg)
{
  ign_msg.clear_pose();
  for (auto const & t : ros_msg.transforms) {
    auto p = ign_msg.add_pose();
    convert_ros_to_ign(t, *p);
  }

  if (!ros_msg.transforms.empty()) {
    convert_ros_to_ign(
      ros_msg.transforms[0].header,
      (*ign_msg.mutable_header()));
  }
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Pose_V & ign_msg,
  tf2_msgs::msg::TFMessage & ros_msg)
{
  ros_msg.transforms.clear();
  for (auto const & p : ign_msg.pose()) {
    geometry_msgs::msg::TransformStamped tf;
    convert_ign_to_ros(p, tf);
    ros_msg.transforms.push_back(tf);
  }
}

template<>
void
convert_ros_to_ign(
  const geometry_msgs::msg::Twist & ros_msg,
  ignition::msgs::Twist & ign_msg)
{
  convert_ros_to_ign(ros_msg.linear, (*ign_msg.mutable_linear()));
  convert_ros_to_ign(ros_msg.angular, (*ign_msg.mutable_angular()));
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Twist & ign_msg,
  geometry_msgs::msg::Twist & ros_msg)
{
  convert_ign_to_ros(ign_msg.linear(), ros_msg.linear);
  convert_ign_to_ros(ign_msg.angular(), ros_msg.angular);
}

template<>
void
convert_ros_to_ign(
  const geometry_msgs::msg::Wrench & ros_msg,
  ignition::msgs::Wrench & ign_msg)
{
  convert_ros_to_ign(ros_msg.force, (*ign_msg.mutable_force()));
  convert_ros_to_ign(ros_msg.torque, (*ign_msg.mutable_torque()));
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Wrench & ign_msg,
  geometry_msgs::msg::Wrench & ros_msg)
{
  convert_ign_to_ros(ign_msg.force(), ros_msg.force);
  convert_ign_to_ros(ign_msg.torque(), ros_msg.torque);
}

template<>
void
convert_ros_to_ign(
  const ros_ign_interfaces::msg::JointWrench & ros_msg,
  ignition::msgs::JointWrench & ign_msg)
{
  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));
  ign_msg.set_body_1_name(ros_msg.body_1_name.data);
  ign_msg.set_body_2_name(ros_msg.body_2_name.data);
  ign_msg.set_body_1_id(ros_msg.body_1_id.data);
  ign_msg.set_body_2_id(ros_msg.body_2_id.data);
  convert_ros_to_ign(ros_msg.body_1_wrench, (*ign_msg.mutable_body_1_wrench()));
  convert_ros_to_ign(ros_msg.body_2_wrench, (*ign_msg.mutable_body_2_wrench()));
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::JointWrench & ign_msg,
  ros_ign_interfaces::msg::JointWrench & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);
  ros_msg.body_1_name.data = ign_msg.body_1_name();
  ros_msg.body_2_name.data = ign_msg.body_2_name();
  ros_msg.body_1_id.data = ign_msg.body_1_id();
  ros_msg.body_2_id.data = ign_msg.body_2_id();
  convert_ign_to_ros(ign_msg.body_1_wrench(), ros_msg.body_1_wrench);
  convert_ign_to_ros(ign_msg.body_2_wrench(), ros_msg.body_2_wrench);
}

template<>
void
convert_ros_to_ign(
  const ros_ign_interfaces::msg::Entity & ros_msg,
  ignition::msgs::Entity & ign_msg)
{
  ign_msg.set_id(ros_msg.id);
  ign_msg.set_name(ros_msg.name);
  switch (ros_msg.type) {
    case ros_ign_interfaces::msg::Entity::NONE:
      ign_msg.set_type(ignition::msgs::Entity::NONE);
      break;
    case ros_ign_interfaces::msg::Entity::LIGHT:
      ign_msg.set_type(ignition::msgs::Entity::LIGHT);
      break;
    case ros_ign_interfaces::msg::Entity::MODEL:
      ign_msg.set_type(ignition::msgs::Entity::MODEL);
      break;
    case ros_ign_interfaces::msg::Entity::LINK:
      ign_msg.set_type(ignition::msgs::Entity::LINK);
      break;
    case ros_ign_interfaces::msg::Entity::VISUAL:
      ign_msg.set_type(ignition::msgs::Entity::VISUAL);
      break;
    case ros_ign_interfaces::msg::Entity::COLLISION:
      ign_msg.set_type(ignition::msgs::Entity::COLLISION);
      break;
    case ros_ign_interfaces::msg::Entity::SENSOR:
      ign_msg.set_type(ignition::msgs::Entity::SENSOR);
      break;
    case ros_ign_interfaces::msg::Entity::JOINT:
      ign_msg.set_type(ignition::msgs::Entity::JOINT);
      break;
    default:
      std::cerr << "Unsupported entity type [" << ros_msg.type << "]\n";
  }
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Entity & ign_msg,
  ros_ign_interfaces::msg::Entity & ros_msg)
{
  ros_msg.id = ign_msg.id();
  ros_msg.name = ign_msg.name();
  if (ign_msg.type() == ignition::msgs::Entity::Type::Entity_Type_NONE) {
    ros_msg.type = ros_ign_interfaces::msg::Entity::NONE;
  } else if (ign_msg.type() == ignition::msgs::Entity::LIGHT) {
    ros_msg.type = ros_ign_interfaces::msg::Entity::LIGHT;
  } else if (ign_msg.type() == ignition::msgs::Entity::MODEL) {
    ros_msg.type = ros_ign_interfaces::msg::Entity::MODEL;
  } else if (ign_msg.type() == ignition::msgs::Entity::LINK) {
    ros_msg.type = ros_ign_interfaces::msg::Entity::LINK;
  } else if (ign_msg.type() == ignition::msgs::Entity::VISUAL) {
    ros_msg.type = ros_ign_interfaces::msg::Entity::VISUAL;
  } else if (ign_msg.type() == ignition::msgs::Entity::COLLISION) {
    ros_msg.type = ros_ign_interfaces::msg::Entity::COLLISION;
  } else if (ign_msg.type() == ignition::msgs::Entity::SENSOR) {
    ros_msg.type = ros_ign_interfaces::msg::Entity::SENSOR;
  } else if (ign_msg.type() == ignition::msgs::Entity::JOINT) {
    ros_msg.type = ros_ign_interfaces::msg::Entity::JOINT;
  } else {
    std::cerr << "Unsupported Entity [" <<
      ign_msg.type() << "]" << std::endl;
  }
}

template<>
void
convert_ros_to_ign(
  const ros_ign_interfaces::msg::Contact & ros_msg,
  ignition::msgs::Contact & ign_msg)
{
  convert_ros_to_ign(ros_msg.collision1, (*ign_msg.mutable_collision1()));
  convert_ros_to_ign(ros_msg.collision1, (*ign_msg.mutable_collision2()));
  ign_msg.clear_position();
  for (auto const & ros_position : ros_msg.positions) {
    auto ign_position = ign_msg.add_position();
    convert_ros_to_ign(ros_position, *ign_position);
  }
  ign_msg.clear_normal();
  for (const auto & ros_normal : ros_msg.normals) {
    auto ign_normal = ign_msg.add_normal();
    convert_ros_to_ign(ros_normal, *ign_normal);
  }
  for (const auto & ros_depth : ros_msg.depths) {
    ign_msg.add_depth(ros_depth);
  }
  ign_msg.clear_wrench();
  for (const auto & ros_wrench : ros_msg.wrenches) {
    auto ign_wrench = ign_msg.add_wrench();
    convert_ros_to_ign(ros_wrench, *ign_wrench);
  }
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Contact & ign_msg,
  ros_ign_interfaces::msg::Contact & ros_msg)
{
  convert_ign_to_ros(ign_msg.collision1(), ros_msg.collision1);
  convert_ign_to_ros(ign_msg.collision2(), ros_msg.collision2);
  for (auto i = 0; i < ign_msg.position_size(); ++i) {
    geometry_msgs::msg::Vector3 ros_position;
    convert_ign_to_ros(ign_msg.position(i), ros_position);
    ros_msg.positions.push_back(ros_position);
  }
  for (auto i = 0; i < ign_msg.normal_size(); ++i) {
    geometry_msgs::msg::Vector3 ros_normal;
    convert_ign_to_ros(ign_msg.normal(i), ros_normal);
    ros_msg.normals.push_back(ros_normal);
  }
  for (auto i = 0; i < ign_msg.depth_size(); ++i) {
    ros_msg.depths.push_back(ign_msg.depth(i));
  }
  for (auto i = 0; i < ign_msg.wrench_size(); ++i) {
    ros_ign_interfaces::msg::JointWrench ros_joint_wrench;
    convert_ign_to_ros(ign_msg.wrench(i), ros_joint_wrench);
    ros_msg.wrenches.push_back(ros_joint_wrench);
  }
}

template<>
void
convert_ros_to_ign(
  const ros_ign_interfaces::msg::Contacts & ros_msg,
  ignition::msgs::Contacts & ign_msg)
{
  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));
  ign_msg.clear_contact();
  for (const auto & ros_contact : ros_msg.contacts) {
    auto ign_contact = ign_msg.add_contact();
    convert_ros_to_ign(ros_contact, *ign_contact);
  }
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Contacts & ign_msg,
  ros_ign_interfaces::msg::Contacts & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);
  for (auto i = 0; i < ign_msg.contact_size(); ++i) {
    ros_ign_interfaces::msg::Contact ros_contact;
    convert_ign_to_ros(ign_msg.contact(i), ros_contact);
    ros_msg.contacts.push_back(ros_contact);
  }
}

/**
template<>
void
convert_ros_to_ign(
  const mav_msgs::msg::Actuators & ros_msg,
  ignition::msgs::Actuators & ign_msg)
{
  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));

  for (auto i = 0u; i < ros_msg.angles.size(); ++i)
    ign_msg.add_position(ros_msg.angles[i]);
  for (auto i = 0u; i < ros_msg.angular_velocities.size(); ++i)
    ign_msg.add_velocity(ros_msg.angular_velocities[i]);
  for (auto i = 0u; i < ros_msg.normalized.size(); ++i)
    ign_msg.add_normalized(ros_msg.normalized[i]);
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Actuators & ign_msg,
  mav_msgs::msg::Actuators & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);

  for (auto i = 0; i < ign_msg.position_size(); ++i)
    ros_msg.angles.push_back(ign_msg.position(i));
  for (auto i = 0; i < ign_msg.velocity_size(); ++i)
    ros_msg.angular_velocities.push_back(ign_msg.velocity(i));
  for (auto i = 0; i < ign_msg.normalized_size(); ++i)
    ros_msg.normalized.push_back(ign_msg.normalized(i));
}
*/

template<>
void
convert_ros_to_ign(
  const nav_msgs::msg::Odometry & ros_msg,
  ignition::msgs::Odometry & ign_msg)
{
  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));
  convert_ros_to_ign(ros_msg.pose.pose, (*ign_msg.mutable_pose()));
  convert_ros_to_ign(ros_msg.twist.twist, (*ign_msg.mutable_twist()));

  auto childFrame = ign_msg.mutable_header()->add_data();
  childFrame->set_key("child_frame_id");
  childFrame->add_value(ros_msg.child_frame_id);
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Odometry & ign_msg,
  nav_msgs::msg::Odometry & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);
  convert_ign_to_ros(ign_msg.pose(), ros_msg.pose.pose);
  convert_ign_to_ros(ign_msg.twist(), ros_msg.twist.twist);

  for (auto i = 0; i < ign_msg.header().data_size(); ++i) {
    auto aPair = ign_msg.header().data(i);
    if (aPair.key() == "child_frame_id" && aPair.value_size() > 0) {
      ros_msg.child_frame_id = frame_id_ign_to_ros(aPair.value(0));
      break;
    }
  }
}

template<>
void
convert_ros_to_ign(
  const sensor_msgs::msg::FluidPressure & ros_msg,
  ignition::msgs::FluidPressure & ign_msg)
{
  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));
  ign_msg.set_pressure(ros_msg.fluid_pressure);
  ign_msg.set_variance(ros_msg.variance);
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::FluidPressure & ign_msg,
  sensor_msgs::msg::FluidPressure & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);
  ros_msg.fluid_pressure = ign_msg.pressure();
  ros_msg.variance = ign_msg.variance();
}

template<>
void
convert_ros_to_ign(
  const sensor_msgs::msg::Image & ros_msg,
  ignition::msgs::Image & ign_msg)
{
  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));

  ign_msg.set_width(ros_msg.width);
  ign_msg.set_height(ros_msg.height);

  unsigned int num_channels;
  unsigned int octets_per_channel;

  if (ros_msg.encoding == "mono8") {
    ign_msg.set_pixel_format_type(ignition::msgs::PixelFormatType::L_INT8);
    num_channels = 1;
    octets_per_channel = 1u;
  } else if (ros_msg.encoding == "mono16") {
    ign_msg.set_pixel_format_type(ignition::msgs::PixelFormatType::L_INT16);
    num_channels = 1;
    octets_per_channel = 2u;
  } else if (ros_msg.encoding == "rgb8") {
    ign_msg.set_pixel_format_type(ignition::msgs::PixelFormatType::RGB_INT8);
    num_channels = 3;
    octets_per_channel = 1u;
  } else if (ros_msg.encoding == "rgba8") {
    ign_msg.set_pixel_format_type(ignition::msgs::PixelFormatType::RGBA_INT8);
    num_channels = 4;
    octets_per_channel = 1u;
  } else if (ros_msg.encoding == "bgra8") {
    ign_msg.set_pixel_format_type(ignition::msgs::PixelFormatType::BGRA_INT8);
    num_channels = 4;
    octets_per_channel = 1u;
  } else if (ros_msg.encoding == "rgb16") {
    ign_msg.set_pixel_format_type(ignition::msgs::PixelFormatType::RGB_INT16);
    num_channels = 3;
    octets_per_channel = 2u;
  } else if (ros_msg.encoding == "bgr8") {
    ign_msg.set_pixel_format_type(ignition::msgs::PixelFormatType::BGR_INT8);
    num_channels = 3;
    octets_per_channel = 1u;
  } else if (ros_msg.encoding == "bgr16") {
    ign_msg.set_pixel_format_type(ignition::msgs::PixelFormatType::BGR_INT16);
    num_channels = 3;
    octets_per_channel = 2u;
  } else if (ros_msg.encoding == "32FC1") {
    ign_msg.set_pixel_format_type(ignition::msgs::PixelFormatType::R_FLOAT32);
    num_channels = 1;
    octets_per_channel = 4u;
  } else {
    ign_msg.set_pixel_format_type(ignition::msgs::PixelFormatType::UNKNOWN_PIXEL_FORMAT);
    std::cerr << "Unsupported pixel format [" << ros_msg.encoding << "]" << std::endl;
    return;
  }

  ign_msg.set_step(ign_msg.width() * num_channels * octets_per_channel);

  ign_msg.set_data(&(ros_msg.data[0]), ign_msg.step() * ign_msg.height());
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Image & ign_msg,
  sensor_msgs::msg::Image & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);

  ros_msg.height = ign_msg.height();
  ros_msg.width = ign_msg.width();

  unsigned int num_channels;
  unsigned int octets_per_channel;

  if (ign_msg.pixel_format_type() == ignition::msgs::PixelFormatType::L_INT8) {
    ros_msg.encoding = "mono8";
    num_channels = 1;
    octets_per_channel = 1u;
  } else if (ign_msg.pixel_format_type() == ignition::msgs::PixelFormatType::L_INT16) {
    ros_msg.encoding = "mono16";
    num_channels = 1;
    octets_per_channel = 2u;
  } else if (ign_msg.pixel_format_type() == ignition::msgs::PixelFormatType::RGB_INT8) {
    ros_msg.encoding = "rgb8";
    num_channels = 3;
    octets_per_channel = 1u;
  } else if (ign_msg.pixel_format_type() == ignition::msgs::PixelFormatType::RGBA_INT8) {
    ros_msg.encoding = "rgba8";
    num_channels = 4;
    octets_per_channel = 1u;
  } else if (ign_msg.pixel_format_type() == ignition::msgs::PixelFormatType::BGRA_INT8) {
    ros_msg.encoding = "bgra8";
    num_channels = 4;
    octets_per_channel = 1u;
  } else if (ign_msg.pixel_format_type() == ignition::msgs::PixelFormatType::RGB_INT16) {
    ros_msg.encoding = "rgb16";
    num_channels = 3;
    octets_per_channel = 2u;
  } else if (ign_msg.pixel_format_type() == ignition::msgs::PixelFormatType::BGR_INT8) {
    ros_msg.encoding = "bgr8";
    num_channels = 3;
    octets_per_channel = 1u;
  } else if (ign_msg.pixel_format_type() == ignition::msgs::PixelFormatType::BGR_INT16) {
    ros_msg.encoding = "bgr16";
    num_channels = 3;
    octets_per_channel = 2u;
  } else if (ign_msg.pixel_format_type() == ignition::msgs::PixelFormatType::R_FLOAT32) {
    ros_msg.encoding = "32FC1";
    num_channels = 1;
    octets_per_channel = 4u;
  } else {
    std::cerr << "Unsupported pixel format [" << ign_msg.pixel_format_type() << "]" << std::endl;
    return;
  }

  ros_msg.is_bigendian = false;
  ros_msg.step = ros_msg.width * num_channels * octets_per_channel;

  auto count = ros_msg.step * ros_msg.height;
  ros_msg.data.resize(ros_msg.step * ros_msg.height);
  std::copy(
    ign_msg.data().begin(),
    ign_msg.data().begin() + count,
    ros_msg.data.begin());
}

template<>
void
convert_ros_to_ign(
  const sensor_msgs::msg::CameraInfo & ros_msg,
  ignition::msgs::CameraInfo & ign_msg)
{
  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));

  ign_msg.set_width(ros_msg.width);
  ign_msg.set_height(ros_msg.height);

  auto distortion = ign_msg.mutable_distortion();
  if (ros_msg.distortion_model == "plumb_bob") {
    distortion->set_model(ignition::msgs::CameraInfo::Distortion::PLUMB_BOB);
  } else if (ros_msg.distortion_model == "rational_polynomial") {
    distortion->set_model(ignition::msgs::CameraInfo::Distortion::RATIONAL_POLYNOMIAL);
  } else if (ros_msg.distortion_model == "equidistant") {
    distortion->set_model(ignition::msgs::CameraInfo::Distortion::EQUIDISTANT);
  } else {
    std::cerr << "Unsupported distortion model [" << ros_msg.distortion_model << "]" << std::endl;
  }

  for (double i : ros_msg.d) {
    distortion->add_k(i);
  }

  auto intrinsics = ign_msg.mutable_intrinsics();
  for (double i : ros_msg.k) {
    intrinsics->add_k(i);
  }

  auto projection = ign_msg.mutable_projection();
  for (double i : ros_msg.p) {
    projection->add_p(i);
  }

  for (auto i = 0u; i < ros_msg.r.size(); ++i) {
    ign_msg.add_rectification_matrix(ros_msg.r[i]);
  }
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::CameraInfo & ign_msg,
  sensor_msgs::msg::CameraInfo & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);

  ros_msg.height = ign_msg.height();
  ros_msg.width = ign_msg.width();

  if (ign_msg.has_distortion()) {
    const auto & distortion = ign_msg.distortion();
    if (distortion.model() == ignition::msgs::CameraInfo::Distortion::PLUMB_BOB) {
      ros_msg.distortion_model = "plumb_bob";
    } else if (distortion.model() == ignition::msgs::CameraInfo::Distortion::RATIONAL_POLYNOMIAL) {
      ros_msg.distortion_model = "rational_polynomial";
    } else if (distortion.model() == ignition::msgs::CameraInfo::Distortion::EQUIDISTANT) {
      ros_msg.distortion_model = "equidistant";
    } else {
      std::cerr << "Unsupported distortion model [" << distortion.model() << "]" << std::endl;
    }

    ros_msg.d.resize(distortion.k_size());
    for (auto i = 0; i < distortion.k_size(); ++i) {
      ros_msg.d[i] = distortion.k(i);
    }
  }

  if (ign_msg.has_intrinsics()) {
    const auto & intrinsics = ign_msg.intrinsics();

    for (auto i = 0; i < intrinsics.k_size(); ++i) {
      ros_msg.k[i] = intrinsics.k(i);
    }
  }

  if (ign_msg.has_projection()) {
    const auto & projection = ign_msg.projection();

    for (auto i = 0; i < projection.p_size(); ++i) {
      ros_msg.p[i] = projection.p(i);
    }
  }

  for (auto i = 0; i < ign_msg.rectification_matrix_size(); ++i) {
    ros_msg.r[i] = ign_msg.rectification_matrix(i);
  }
}

template<>
void
convert_ros_to_ign(
  const sensor_msgs::msg::Imu & ros_msg,
  ignition::msgs::IMU & ign_msg)
{
  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));

  // ToDo: Verify that this is the expected value (probably not).
  ign_msg.set_entity_name(ros_msg.header.frame_id);

  convert_ros_to_ign(ros_msg.orientation, (*ign_msg.mutable_orientation()));
  convert_ros_to_ign(ros_msg.angular_velocity, (*ign_msg.mutable_angular_velocity()));
  convert_ros_to_ign(ros_msg.linear_acceleration, (*ign_msg.mutable_linear_acceleration()));
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::IMU & ign_msg,
  sensor_msgs::msg::Imu & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);
  convert_ign_to_ros(ign_msg.orientation(), ros_msg.orientation);
  convert_ign_to_ros(ign_msg.angular_velocity(), ros_msg.angular_velocity);
  convert_ign_to_ros(ign_msg.linear_acceleration(), ros_msg.linear_acceleration);

  // Covariances not supported in Ignition::msgs::IMU
}

template<>
void
convert_ros_to_ign(
  const sensor_msgs::msg::JointState & ros_msg,
  ignition::msgs::Model & ign_msg)
{
  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));

  for (auto i = 0u; i < ros_msg.position.size(); ++i) {
    auto newJoint = ign_msg.add_joint();
    newJoint->set_name(ros_msg.name[i]);
    newJoint->mutable_axis1()->set_position(ros_msg.position[i]);
    newJoint->mutable_axis1()->set_velocity(ros_msg.velocity[i]);
    newJoint->mutable_axis1()->set_force(ros_msg.effort[i]);
  }
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Model & ign_msg,
  sensor_msgs::msg::JointState & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);

  for (auto i = 0; i < ign_msg.joint_size(); ++i) {
    ros_msg.name.push_back(ign_msg.joint(i).name());
    ros_msg.position.push_back(ign_msg.joint(i).axis1().position());
    ros_msg.velocity.push_back(ign_msg.joint(i).axis1().velocity());
    ros_msg.effort.push_back(ign_msg.joint(i).axis1().force());
  }
}

template<>
void
convert_ros_to_ign(
  const sensor_msgs::msg::LaserScan & ros_msg,
  ignition::msgs::LaserScan & ign_msg)
{
  const unsigned int num_readings =
    (ros_msg.angle_max - ros_msg.angle_min) / ros_msg.angle_increment;

  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));
  ign_msg.set_frame(ros_msg.header.frame_id);
  ign_msg.set_angle_min(ros_msg.angle_min);
  ign_msg.set_angle_max(ros_msg.angle_max);
  ign_msg.set_angle_step(ros_msg.angle_increment);
  ign_msg.set_range_min(ros_msg.range_min);
  ign_msg.set_range_max(ros_msg.range_max);
  ign_msg.set_count(num_readings);

  // Not supported in sensor_msgs::msg::LaserScan.
  ign_msg.set_vertical_angle_min(0.0);
  ign_msg.set_vertical_angle_max(0.0);
  ign_msg.set_vertical_angle_step(0.0);
  ign_msg.set_vertical_count(0u);

  for (auto i = 0u; i < ign_msg.count(); ++i) {
    ign_msg.add_ranges(ros_msg.ranges[i]);
    ign_msg.add_intensities(ros_msg.intensities[i]);
  }
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::LaserScan & ign_msg,
  sensor_msgs::msg::LaserScan & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);
  ros_msg.header.frame_id = frame_id_ign_to_ros(ign_msg.frame());

  ros_msg.angle_min = ign_msg.angle_min();
  ros_msg.angle_max = ign_msg.angle_max();
  ros_msg.angle_increment = ign_msg.angle_step();

  // Not supported in ignition::msgs::LaserScan.
  ros_msg.time_increment = 0.0;
  ros_msg.scan_time = 0.0;

  ros_msg.range_min = ign_msg.range_min();
  ros_msg.range_max = ign_msg.range_max();

  auto count = ign_msg.count();
  auto vertical_count = ign_msg.vertical_count();

  // If there are multiple vertical beams, use the one in the middle.
  size_t start = (vertical_count / 2) * count;

  // Copy ranges into ROS message.
  ros_msg.ranges.resize(count);
  std::copy(
    ign_msg.ranges().begin() + start,
    ign_msg.ranges().begin() + start + count,
    ros_msg.ranges.begin());

  // Copy intensities into ROS message.
  ros_msg.intensities.resize(count);
  std::copy(
    ign_msg.intensities().begin() + start,
    ign_msg.intensities().begin() + start + count,
    ros_msg.intensities.begin());
}

template<>
void
convert_ros_to_ign(
  const sensor_msgs::msg::MagneticField & ros_msg,
  ignition::msgs::Magnetometer & ign_msg)
{
  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));
  convert_ros_to_ign(ros_msg.magnetic_field, (*ign_msg.mutable_field_tesla()));
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::Magnetometer & ign_msg,
  sensor_msgs::msg::MagneticField & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);
  convert_ign_to_ros(ign_msg.field_tesla(), ros_msg.magnetic_field);

  // magnetic_field_covariance is not supported in Ignition::Msgs::Magnetometer.
}

template<>
void
convert_ros_to_ign(
  const sensor_msgs::msg::PointCloud2 & ros_msg,
  ignition::msgs::PointCloudPacked & ign_msg)
{
  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));

  ign_msg.set_height(ros_msg.height);
  ign_msg.set_width(ros_msg.width);
  ign_msg.set_is_bigendian(ros_msg.is_bigendian);
  ign_msg.set_point_step(ros_msg.point_step);
  ign_msg.set_row_step(ros_msg.row_step);
  ign_msg.set_is_dense(ros_msg.is_dense);
  ign_msg.mutable_data()->resize(ros_msg.data.size());
  memcpy(ign_msg.mutable_data()->data(), ros_msg.data.data(), ros_msg.data.size());

  for (const auto & field : ros_msg.fields) {
    ignition::msgs::PointCloudPacked::Field * pf = ign_msg.add_field();
    pf->set_name(field.name);
    pf->set_count(field.count);
    pf->set_offset(field.offset);
    switch (field.datatype) {
      default:
      case sensor_msgs::msg::PointField::INT8:
        pf->set_datatype(ignition::msgs::PointCloudPacked::Field::INT8);
        break;
      case sensor_msgs::msg::PointField::UINT8:
        pf->set_datatype(ignition::msgs::PointCloudPacked::Field::UINT8);
        break;
      case sensor_msgs::msg::PointField::INT16:
        pf->set_datatype(ignition::msgs::PointCloudPacked::Field::INT16);
        break;
      case sensor_msgs::msg::PointField::UINT16:
        pf->set_datatype(ignition::msgs::PointCloudPacked::Field::UINT16);
        break;
      case sensor_msgs::msg::PointField::INT32:
        pf->set_datatype(ignition::msgs::PointCloudPacked::Field::INT32);
        break;
      case sensor_msgs::msg::PointField::UINT32:
        pf->set_datatype(ignition::msgs::PointCloudPacked::Field::UINT32);
        break;
      case sensor_msgs::msg::PointField::FLOAT32:
        pf->set_datatype(ignition::msgs::PointCloudPacked::Field::FLOAT32);
        break;
      case sensor_msgs::msg::PointField::FLOAT64:
        pf->set_datatype(ignition::msgs::PointCloudPacked::Field::FLOAT64);
        break;
    }
  }
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::PointCloudPacked & ign_msg,
  sensor_msgs::msg::PointCloud2 & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);

  ros_msg.height = ign_msg.height();
  ros_msg.width = ign_msg.width();
  ros_msg.is_bigendian = ign_msg.is_bigendian();
  ros_msg.point_step = ign_msg.point_step();
  ros_msg.row_step = ign_msg.row_step();
  ros_msg.is_dense = ign_msg.is_dense();
  ros_msg.data.resize(ign_msg.data().size());
  memcpy(ros_msg.data.data(), ign_msg.data().c_str(), ign_msg.data().size());

  for (int i = 0; i < ign_msg.field_size(); ++i) {
    sensor_msgs::msg::PointField pf;
    pf.name = ign_msg.field(i).name();
    pf.count = ign_msg.field(i).count();
    pf.offset = ign_msg.field(i).offset();
    switch (ign_msg.field(i).datatype()) {
      default:
      case ignition::msgs::PointCloudPacked::Field::INT8:
        pf.datatype = sensor_msgs::msg::PointField::INT8;
        break;
      case ignition::msgs::PointCloudPacked::Field::UINT8:
        pf.datatype = sensor_msgs::msg::PointField::UINT8;
        break;
      case ignition::msgs::PointCloudPacked::Field::INT16:
        pf.datatype = sensor_msgs::msg::PointField::INT16;
        break;
      case ignition::msgs::PointCloudPacked::Field::UINT16:
        pf.datatype = sensor_msgs::msg::PointField::UINT16;
        break;
      case ignition::msgs::PointCloudPacked::Field::INT32:
        pf.datatype = sensor_msgs::msg::PointField::INT32;
        break;
      case ignition::msgs::PointCloudPacked::Field::UINT32:
        pf.datatype = sensor_msgs::msg::PointField::UINT32;
        break;
      case ignition::msgs::PointCloudPacked::Field::FLOAT32:
        pf.datatype = sensor_msgs::msg::PointField::FLOAT32;
        break;
      case ignition::msgs::PointCloudPacked::Field::FLOAT64:
        pf.datatype = sensor_msgs::msg::PointField::FLOAT64;
        break;
    }
    ros_msg.fields.push_back(pf);
  }
}

template<>
void
convert_ros_to_ign(
  const sensor_msgs::msg::BatteryState & ros_msg,
  ignition::msgs::BatteryState & ign_msg)
{
  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));

  ign_msg.set_voltage(ros_msg.voltage);
  ign_msg.set_current(ros_msg.current);
  ign_msg.set_charge(ros_msg.charge);
  ign_msg.set_capacity(ros_msg.capacity);
  ign_msg.set_percentage(ros_msg.percentage);

  switch (ros_msg.power_supply_status) {
    case sensor_msgs::msg::BatteryState::POWER_SUPPLY_STATUS_UNKNOWN:
      ign_msg.set_power_supply_status(ignition::msgs::BatteryState::UNKNOWN);
      break;
    case sensor_msgs::msg::BatteryState::POWER_SUPPLY_STATUS_CHARGING:
      ign_msg.set_power_supply_status(ignition::msgs::BatteryState::CHARGING);
      break;
    case sensor_msgs::msg::BatteryState::POWER_SUPPLY_STATUS_DISCHARGING:
      ign_msg.set_power_supply_status(ignition::msgs::BatteryState::DISCHARGING);
      break;
    case sensor_msgs::msg::BatteryState::POWER_SUPPLY_STATUS_NOT_CHARGING:
      ign_msg.set_power_supply_status(ignition::msgs::BatteryState::NOT_CHARGING);
      break;
    case sensor_msgs::msg::BatteryState::POWER_SUPPLY_STATUS_FULL:
      ign_msg.set_power_supply_status(ignition::msgs::BatteryState::FULL);
      break;
    default:
      std::cerr << "Unsupported power supply status [" << ros_msg.power_supply_status << "]\n";
  }
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::BatteryState & ign_msg,
  sensor_msgs::msg::BatteryState & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);

  ros_msg.voltage = ign_msg.voltage();
  ros_msg.current = ign_msg.current();
  ros_msg.charge = ign_msg.charge();
  ros_msg.capacity = ign_msg.capacity();
  ros_msg.design_capacity = std::numeric_limits<double>::quiet_NaN();
  ros_msg.percentage = ign_msg.percentage();

  if (ign_msg.power_supply_status() == ignition::msgs::BatteryState::UNKNOWN) {
    ros_msg.power_supply_status = sensor_msgs::msg::BatteryState::POWER_SUPPLY_STATUS_UNKNOWN;
  } else if (ign_msg.power_supply_status() == ignition::msgs::BatteryState::CHARGING) {
    ros_msg.power_supply_status = sensor_msgs::msg::BatteryState::POWER_SUPPLY_STATUS_CHARGING;
  } else if (ign_msg.power_supply_status() == ignition::msgs::BatteryState::DISCHARGING) {
    ros_msg.power_supply_status = sensor_msgs::msg::BatteryState::POWER_SUPPLY_STATUS_DISCHARGING;
  } else if (ign_msg.power_supply_status() == ignition::msgs::BatteryState::NOT_CHARGING) {
    ros_msg.power_supply_status = sensor_msgs::msg::BatteryState::POWER_SUPPLY_STATUS_NOT_CHARGING;
  } else if (ign_msg.power_supply_status() == ignition::msgs::BatteryState::FULL) {
    ros_msg.power_supply_status = sensor_msgs::msg::BatteryState::POWER_SUPPLY_STATUS_FULL;
  } else {
    std::cerr << "Unsupported power supply status [" <<
      ign_msg.power_supply_status() << "]" << std::endl;
  }

  ros_msg.power_supply_health = sensor_msgs::msg::BatteryState::POWER_SUPPLY_HEALTH_UNKNOWN;
  ros_msg.power_supply_technology =
    sensor_msgs::msg::BatteryState::POWER_SUPPLY_TECHNOLOGY_UNKNOWN;
  ros_msg.present = true;
}

template<>
void
convert_ros_to_ign(
  const trajectory_msgs::msg::JointTrajectoryPoint & ros_msg,
  ignition::msgs::JointTrajectoryPoint & ign_msg)
{
  for (const auto & ros_position : ros_msg.positions) {
    ign_msg.add_positions(ros_position);
  }
  for (const auto & ros_velocity : ros_msg.velocities) {
    ign_msg.add_velocities(ros_velocity);
  }
  for (const auto & ros_acceleration : ros_msg.accelerations) {
    ign_msg.add_accelerations(ros_acceleration);
  }
  for (const auto & ros_effort : ros_msg.effort) {
    ign_msg.add_effort(ros_effort);
  }

  ignition::msgs::Duration * ign_duration = ign_msg.mutable_time_from_start();
  ign_duration->set_sec(ros_msg.time_from_start.sec);
  ign_duration->set_nsec(ros_msg.time_from_start.nanosec);
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::JointTrajectoryPoint & ign_msg,
  trajectory_msgs::msg::JointTrajectoryPoint & ros_msg)
{
  for (auto i = 0; i < ign_msg.positions_size(); ++i) {
    ros_msg.positions.push_back(ign_msg.positions(i));
  }
  for (auto i = 0; i < ign_msg.velocities_size(); ++i) {
    ros_msg.velocities.push_back(ign_msg.velocities(i));
  }
  for (auto i = 0; i < ign_msg.accelerations_size(); ++i) {
    ros_msg.accelerations.push_back(ign_msg.accelerations(i));
  }
  for (auto i = 0; i < ign_msg.effort_size(); ++i) {
    ros_msg.effort.push_back(ign_msg.effort(i));
  }

  ros_msg.time_from_start = rclcpp::Duration(
    ign_msg.time_from_start().sec(),
    ign_msg.time_from_start().nsec());
}

template<>
void
convert_ros_to_ign(
  const trajectory_msgs::msg::JointTrajectory & ros_msg,
  ignition::msgs::JointTrajectory & ign_msg)
{
  convert_ros_to_ign(ros_msg.header, (*ign_msg.mutable_header()));

  for (const auto & ros_joint_name : ros_msg.joint_names) {
    ign_msg.add_joint_names(ros_joint_name);
  }

  for (const auto & ros_point : ros_msg.points) {
    ignition::msgs::JointTrajectoryPoint * ign_point = ign_msg.add_points();
    convert_ros_to_ign(ros_point, (*ign_point));
  }
}

template<>
void
convert_ign_to_ros(
  const ignition::msgs::JointTrajectory & ign_msg,
  trajectory_msgs::msg::JointTrajectory & ros_msg)
{
  convert_ign_to_ros(ign_msg.header(), ros_msg.header);

  for (auto i = 0; i < ign_msg.joint_names_size(); ++i) {
    ros_msg.joint_names.push_back(ign_msg.joint_names(i));
  }

  for (auto i = 0; i < ign_msg.points_size(); ++i) {
    trajectory_msgs::msg::JointTrajectoryPoint ros_point;
    convert_ign_to_ros(ign_msg.points(i), ros_point);
    ros_msg.points.push_back(ros_point);
  }
}

}  // namespace ros_ign_bridge
