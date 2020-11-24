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

#ifndef AWAPI_ACCESSOR__ACCESSOR_HPP_
#define AWAPI_ACCESSOR__ACCESSOR_HPP_

#include <autoware_api_msgs/msg/awapi_autoware_status.hpp>
#include <autoware_api_msgs/msg/awapi_vehicle_status.hpp>
#include <autoware_perception_msgs/msg/traffic_light_state_array.hpp>
#include <autoware_planning_msgs/msg/route.hpp>
#include <awapi_accessor/utility/visibility.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/float32.hpp>
#include <std_msgs/msg/string.hpp>

#include <utility>

namespace autoware_api
{

class Accessor
{
#define DEFINE_SUBSCRIPTION(TYPE) \
protected: \
  TYPE current_value_of_ ## TYPE; \
 \
private: \
  rclcpp::Subscription<TYPE>::SharedPtr subscription_of_ ## TYPE; \
 \
public: \
  const auto & get ## TYPE() const noexcept \
  { \
    return current_value_of_ ## TYPE; \
  } \
  static_assert(true, "")


#define DEFINE_PUBLICATION(TYPE) \
private: \
  rclcpp::Publisher<TYPE>::SharedPtr publisher_of_ ## TYPE; \
 \
public: \
  template \
  < \
    typename ... Ts \
  > \
  decltype(auto) set ## TYPE(Ts && ... xs) const \
  { \
    return (*publisher_of_ ## TYPE).publish(std::forward<decltype(xs)>(xs)...); \
  } \
  static_assert(true, "")

  /** ---- AutowareEngage ------------------------------------------------------
   *
   *  Topic: /awapi/autoware/put/engage
   *
   * ------------------------------------------------------------------------ */
  using AutowareEngage = std_msgs::msg::Bool;

  DEFINE_PUBLICATION(AutowareEngage);

  /** ---- AutowareRoute -------------------------------------------------------
   *
   *  Topic: /awapi/autoware/put/route
   *
   * ------------------------------------------------------------------------ */
  using AutowareRoute = autoware_planning_msgs::msg::Route;

  DEFINE_PUBLICATION(AutowareRoute);

  /** ---- LaneChangeApproval --------------------------------------------------
   *
   *  Topic: /awapi/lane_change/put/approval
   *
   * ------------------------------------------------------------------------ */
  using LaneChangeApproval = std_msgs::msg::Bool;

  DEFINE_PUBLICATION(LaneChangeApproval);

  /** ---- LaneChangeForce -----------------------------------------------------
   *
   *  Topic: /awapi/lane_change/put/force
   *
   * ------------------------------------------------------------------------ */
  using LaneChangeForce = std_msgs::msg::Bool;

  DEFINE_PUBLICATION(LaneChangeForce);

  /** ---- TrafficLightStateArray ----------------------------------------------
   *
   *  Overwrite the recognition result of traffic light.
   *
   *  Topic: /awapi/traffic_light/put/traffic_light
   *
   * ------------------------------------------------------------------------ */
  using TrafficLightStateArray = autoware_perception_msgs::msg::TrafficLightStateArray;

  DEFINE_PUBLICATION(TrafficLightStateArray);

  /** ---- VehicleVelocity -----------------------------------------------------
   *
   *  Set upper bound of velocity.
   *
   *  Topic: /awapi/vehicle/put/velocity
   *
   * ------------------------------------------------------------------------ */
  using VehicleVelocity = std_msgs::msg::Float32;

  DEFINE_PUBLICATION(VehicleVelocity);

  /** ---- AutowareStatus ------------------------------------------------------
   *
   *  Topic: /awapi/autoware/get/status
   *
   * ------------------------------------------------------------------------ */
  using AutowareStatus = autoware_api_msgs::msg::AwapiAutowareStatus;

  DEFINE_SUBSCRIPTION(AutowareStatus);

  /** ---- TrafficLightStatus --------------------------------------------------
   *
   *  Topic: /awapi/traffic_light/get/status
   *
   * ------------------------------------------------------------------------ */
  using TrafficLightStatus = autoware_perception_msgs::msg::TrafficLightStateArray;

  DEFINE_SUBSCRIPTION(TrafficLightStatus);

  /** ---- VehicleStatus -------------------------------------------------------
   *
   *  Topic: /awapi/vehicle/get/status
   *
   * ------------------------------------------------------------------------ */
  using VehicleStatus = autoware_api_msgs::msg::AwapiVehicleStatus;

  DEFINE_SUBSCRIPTION(VehicleStatus);

  /** ---- DummyData -----------------------------------------------------------
   *
   *  Topic: ~/dummy
   *
   * ------------------------------------------------------------------------ */
  using DebugString = std_msgs::msg::String;

  DEFINE_PUBLICATION(DebugString);
  DEFINE_SUBSCRIPTION(DebugString);

#undef DEFINE_SUBSCRIPTION
#undef DEFINE_PUBLICATION

public:
#define MAKE_SUBSCRIPTION(TYPE, TOPIC) \
  subscription_of_ ## TYPE( \
    (*node).template create_subscription<TYPE>( \
      TOPIC, 1, \
      [this](const TYPE::SharedPtr message) \
      { \
        current_value_of_ ## TYPE = *message; \
      }))

#define MAKE_PUBLICATION(TYPE, TOPIC) \
  publisher_of_ ## TYPE( \
    (*node).template create_publisher<TYPE>(TOPIC, 10))

  template
  <
    typename Node
  >
  AWAPI_ACCESSOR_PUBLIC
  explicit Accessor(Node * const node)
  : MAKE_PUBLICATION(AutowareEngage, "/awapi/autoware/put/engage"),
    MAKE_PUBLICATION(AutowareRoute, "/awapi/autoware/put/route"),
    MAKE_PUBLICATION(LaneChangeApproval, "/awapi/lane_change/put/approval"),
    MAKE_PUBLICATION(LaneChangeForce, "/awapi/lane_change/put/force"),
    MAKE_PUBLICATION(TrafficLightStateArray, "/awapi/traffic_light/put/traffic_light"),
    MAKE_PUBLICATION(VehicleVelocity, "/awapi/vehicle/put/velocity"),
    MAKE_SUBSCRIPTION(AutowareStatus, "/awapi/autoware/get/status"),
    MAKE_SUBSCRIPTION(TrafficLightStatus, "/awapi/traffic_light/get/status"),
    MAKE_SUBSCRIPTION(VehicleStatus, "/awapi/vehicle/get/status"),
    // Debug
    MAKE_PUBLICATION(DebugString, "debug/string"),
    MAKE_SUBSCRIPTION(DebugString, "debug/string")
  {}

#undef MAKE_SUBSCRIPTION
#undef MAKE_PUBLICATION
};

}  // namespace autoware_api

#endif  // AWAPI_ACCESSOR__ACCESSOR_HPP_
