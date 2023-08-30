// Copyright 2023 TIER IV, Inc. All rights reserved.
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

#ifndef TRAFFIC_SIMULATOR__BEHAVIOR__FOLLOW_MODE_POLYLINE_TRAJECTORY_FOLLOWER_HPP_
#define TRAFFIC_SIMULATOR__BEHAVIOR__FOLLOW_MODE_POLYLINE_TRAJECTORY_FOLLOWER_HPP_

#include <traffic_simulator/behavior/follow_trajectory/polyline_trajectory_follower.hpp>

namespace traffic_simulator
{
namespace follow_trajectory
{
class FollowModePolylineTrajectoryFollower : public PolylineTrajectoryFollower
{
public:
  FollowModePolylineTrajectoryFollower() = default;
  ~FollowModePolylineTrajectoryFollower() = default;

  std::optional<traffic_simulator_msgs::msg::EntityStatus> followTrajectory(
    std::shared_ptr<traffic_simulator_msgs::msg::PolylineTrajectory> & polyline_trajectory)
    override;

  auto setParameters(
    const traffic_simulator_msgs::msg::EntityStatus &,
    const traffic_simulator_msgs::msg::BehaviorParameter &, const double,
    const traffic_simulator_msgs::msg::VehicleParameters & vehicle_parameters) -> void override;
  auto setParameters(
    const traffic_simulator_msgs::msg::EntityStatus &,
    const traffic_simulator_msgs::msg::BehaviorParameter &, const double) -> void override;

private:
  auto getDistanceAndTimeToFrontWaypoint(
    const geometry_msgs::msg::Point & target_position,
    const geometry_msgs::msg::Point & position) const
    -> std::optional<std::tuple<double, double>> override;
  auto getDistanceAndTimeToWaypointWithSpecifiedTime(double distance_to_front_waypoint) const
    -> std::optional<std::tuple<double, double>> override;

  void discardTheFrontWaypointFromTrajectory() override;

  auto getSteering(const geometry_msgs::msg::Point & target_position, double desired_speed) const
    -> double;

  std::optional<geometry_msgs::msg::Point> previous_target{std::nullopt};
};
}  // namespace follow_trajectory
}  // namespace traffic_simulator

#endif
