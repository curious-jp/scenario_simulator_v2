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

#include <ament_index_cpp/get_package_share_directory.hpp>
#include <cpp_mock_scenarios/catalogs.hpp>
#include <cpp_mock_scenarios/cpp_scenario_node.hpp>
#include <openscenario_msgs/msg/driver_model.hpp>
#include <rclcpp/rclcpp.hpp>
#include <traffic_simulator/api/api.hpp>

// headers in STL
#include <memory>
#include <string>
#include <vector>

class MergeLeftScenario : public cpp_mock_scenarios::CppScenarioNode
{
public:
  explicit MergeLeftScenario(const rclcpp::NodeOptions & option)
  : cpp_mock_scenarios::CppScenarioNode(
      "merge_left", ament_index_cpp::get_package_share_directory("kashiwanoha_map") + "/map",
      "lanelet2_map.osm", __FILE__, false, option)
  {
    start();
  }

private:
  bool requested = false;
  void onUpdate() override
  {
    if (api_.isInLanelet("ego", 34510, 0.1)) {
      stop(cpp_mock_scenarios::Result::SUCCESS);
    }
    // LCOV_EXCL_START
    if (api_.getCurrentTime() >= 10.0) {
      stop(cpp_mock_scenarios::Result::FAILURE);
    }
    if (api_.checkCollision("ego", "npc")) {
      stop(cpp_mock_scenarios::Result::FAILURE);
    }
    // LCOV_EXCL_STOP
  }
  void onInitialize() override
  {
    api_.spawn(
      false, "ego", getVehicleParameters(),
      traffic_simulator::helper::constructLaneletPose(34462, 10, 0, 0, 0, 0),
      traffic_simulator::helper::constructActionStatus(5));
    api_.setTargetSpeed("ego", 5, true);
    api_.requestLaneChange("ego", 34513);
    api_.spawn(
      false, "npc", getVehicleParameters(),
      traffic_simulator::helper::constructLaneletPose(34513, 0, 0, 0, 0, 0),
      traffic_simulator::helper::constructActionStatus(10));
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::NodeOptions options;
  auto component = std::make_shared<MergeLeftScenario>(options);
  rclcpp::spin(component);
  rclcpp::shutdown();
  return 0;
}
