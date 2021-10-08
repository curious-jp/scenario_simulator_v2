// Copyright 2015-2020 Tier IV, Inc. All rights reserved.
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

#include <quaternion_operation/quaternion_operation.h>

#include <boost/algorithm/clamp.hpp>
#include <memory>
#include <string>
#include <traffic_simulator/entity/pedestrian_entity.hpp>
#include <vector>

namespace traffic_simulator
{
namespace entity
{
PedestrianEntity::PedestrianEntity(
  const std::string & name, const openscenario_msgs::msg::PedestrianParameters & params,
  const openscenario_msgs::msg::EntityStatus & status)
: EntityBase(params.pedestrian_category, name, status),
  parameters(params),
  loader_(pluginlib::ClassLoader<entity_behavior::BehaviorPluginBase>(
    "behavior_plugin", "behavior_plugin_base"))
{
  entity_type_.type = openscenario_msgs::msg::EntityType::PEDESTRIAN;
  /**
   * @todo pass plugin name via constructor
   */
  std::string plugin_name = "behavior_plugin/behavior_tree_plugin";
  behavior_plugin_ptr_ = loader_.createSharedInstance(plugin_name);
  behavior_plugin_ptr_->setPedestrianParameters(parameters);
}

void PedestrianEntity::requestAssignRoute(
  const std::vector<openscenario_msgs::msg::LaneletPose> & waypoints)
{
  behavior_plugin_ptr_->setRequest("follow_lane");
  if (status_.lanelet_pose_valid) {
    return;
  }
  route_planner_ptr_->getRouteLanelets(status_.lanelet_pose, waypoints);
}

void PedestrianEntity::requestAssignRoute(const std::vector<geometry_msgs::msg::Pose> & waypoints)
{
  std::vector<openscenario_msgs::msg::LaneletPose> route;
  for (const auto & waypoint : waypoints) {
    const auto lanelet_waypoint = hdmap_utils_ptr_->toLaneletPose(waypoint);
    if (lanelet_waypoint) {
      route.emplace_back(lanelet_waypoint.get());
    } else {
      THROW_SEMANTIC_ERROR("Waypoint of pedestrian entity should be on lane.");
    }
  }
  requestAssignRoute(route);
}

void PedestrianEntity::requestWalkStraight() { behavior_plugin_ptr_->setRequest("walk_straight"); }

void PedestrianEntity::requestAcquirePosition(
  const openscenario_msgs::msg::LaneletPose & lanelet_pose)
{
  behavior_plugin_ptr_->setRequest("follow_lane");
  if (status_.lanelet_pose_valid) {
    return;
  }
  route_planner_ptr_->getRouteLanelets(status_.lanelet_pose, lanelet_pose);
}

void PedestrianEntity::requestAcquirePosition(const geometry_msgs::msg::Pose & map_pose)
{
  const auto lanelet_pose = hdmap_utils_ptr_->toLaneletPose(map_pose);
  if (lanelet_pose) {
    requestAcquirePosition(lanelet_pose.get());
  } else {
    THROW_SEMANTIC_ERROR("Goal of the pedestrian entity should be on lane.");
  }
}

void PedestrianEntity::cancelRequest()
{
  behavior_plugin_ptr_->setRequest("none");
  route_planner_ptr_->cancelGoal();
}

void PedestrianEntity::setTargetSpeed(double target_speed, bool continuous)
{
  target_speed_planner_.setTargetSpeed(target_speed, continuous);
}

void PedestrianEntity::onUpdate(double current_time, double step_time)
{
  EntityBase::onUpdate(current_time, step_time);
  if (current_time < 0) {
    updateEntityStatusTimestamp(current_time);
  } else {
    behavior_plugin_ptr_->setOtherEntityStatus(other_status_);
    behavior_plugin_ptr_->setEntityTypeList(entity_type_list_);
    behavior_plugin_ptr_->setEntityStatus(status_);
    target_speed_planner_.update(status_.action_status.twist.linear.x);
    behavior_plugin_ptr_->setTargetSpeed(target_speed_planner_.getTargetSpeed());
    if (status_.lanelet_pose_valid) {
      auto route = route_planner_ptr_->getRouteLanelets(status_.lanelet_pose);
      behavior_plugin_ptr_->setRouteLanelets(route);
    } else {
      std::vector<std::int64_t> empty = {};
      behavior_plugin_ptr_->setRouteLanelets(empty);
    }
    behavior_plugin_ptr_->update(current_time, step_time);
    auto status_updated = behavior_plugin_ptr_->getUpdatedStatus();
    if (status_updated.lanelet_pose_valid) {
      auto following_lanelets =
        hdmap_utils_ptr_->getFollowingLanelets(status_updated.lanelet_pose.lanelet_id);
      auto l = hdmap_utils_ptr_->getLaneletLength(status_updated.lanelet_pose.lanelet_id);
      if (following_lanelets.size() == 1 && l <= status_updated.lanelet_pose.s) {
        stopAtEndOfRoad();
        return;
      }
    }
    linear_jerk_ =
      (status_updated.action_status.accel.linear.x - status_.action_status.accel.linear.x) /
      step_time;
    setStatus(status_updated);
    updateStandStillDuration(step_time);
  }
}
}  // namespace entity
}  // namespace traffic_simulator
