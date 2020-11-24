// Copyright 2015-2020 TierIV.inc. All rights reserved.
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

#ifndef SIMULATION_API__MATH__COLLISION_HPP_
#define SIMULATION_API__MATH__COLLISION_HPP_

#include <openscenario_msgs/msg/bounding_box.hpp>
#include <geometry_msgs/msg/pose.hpp>

#include <vector>

namespace simulation_api
{
namespace math
{
bool checkCollision2D(
  geometry_msgs::msg::Pose pose0, openscenario_msgs::msg::BoundingBox bbox0,
  geometry_msgs::msg::Pose pose1, openscenario_msgs::msg::BoundingBox bbox1);
std::vector<geometry_msgs::msg::Point> transformPoints(
  geometry_msgs::msg::Pose pose,
  std::vector<geometry_msgs::msg::Point> points);
std::vector<geometry_msgs::msg::Point> getPointsFromBbox(
  openscenario_msgs::msg::BoundingBox bbox);
}  // namespace math
}  // namespace simulation_api

#endif  // SIMULATION_API__MATH__COLLISION_HPP_
