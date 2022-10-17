// Copyright 2015 TIER IV, Inc. All rights reserved.
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

//#define OPENSCENARIO_INTERPRETER_NO_EXTENSION

#include <algorithm>
#include <openscenario_preprocessor/openscenario_preprocessor.hpp>
#include <rclcpp_components/register_node_macro.hpp>

namespace openscenario_preprocessor
{
Preprocessor::Preprocessor(const rclcpp::NodeOptions & options)
: rclcpp::Node("preprocessor", options)
{
  using openscenario_preprocessor_msgs::srv::Load;
  load_server = create_service<Load>(
    "~/load",
    [this](
      const Load::Request::SharedPtr request,
      Load::Response::SharedPtr response) -> void {
      auto lock = std::lock_guard(preprocessed_scenarios_mutex);
      try {
        auto s = ScenarioInfo(*request);
        preprocessScenario(s);
        response->has_succeeded = true;
        response->message = "success";
      } catch (std::exception & e) {
        response->has_succeeded = false;
        response->message = e.what();
        preprocessed_scenarios.clear();
      }
    });

  using openscenario_preprocessor_msgs::srv::Derive;
  derive_server = create_service<Derive>(
    "~/derive",
    [this](
      const Derive::Request::SharedPtr,
      Derive::Response::SharedPtr response) -> void {
      auto lock = std::lock_guard(preprocessed_scenarios_mutex);
      if (preprocessed_scenarios.empty()) {
        response->path = "no output";
      } else {
        *response = preprocessed_scenarios.front().getDeriveResponse();
        preprocessed_scenarios.pop_front();
      }
    });

  using openscenario_preprocessor_msgs::srv::CheckDerivativeRemained;
  check_server = create_service<CheckDerivativeRemained>(
    "~/check",
    [this](
      const CheckDerivativeRemained::Request::SharedPtr,
      CheckDerivativeRemained::Response::SharedPtr response) -> void {
      auto lock = std::lock_guard(preprocessed_scenarios_mutex);
      response->derivative_remained = not preprocessed_scenarios.empty();
    });
}

bool Preprocessor::validateXOSC(const std::string & file_name)
{
  return concealer::dollar("ros2 run openscenario_utility validate-xosc " + file_name)
           .find("All xosc files given are standard compliant.") != std::string::npos;
}

void Preprocessor::preprocessScenario(ScenarioInfo & scenario)
{
  // this function doesn't support ParameterValueDistribution now
  if (validateXOSC(scenario.path)) {
    //  auto script = OpenScenario(scenario.path);
    //  if (hasElement("ParameterValueDistribution", scenario.path)) {
    //    assert( validateXOSC( linked scenario.path );
    //    auto derive_server = createDeriveServer();
    //    parameters = evaluate( parameter_value_distribution( given scenario ) )
    //    for( auto derived_scenario : embedParameter( linked scenario, parameters)
    //      preprocessed_scenarios.emplace_back({derived_scenario, derive_server});
    preprocessed_scenarios.emplace_back(scenario);  // temporary code
  } else {
    throw common::Error("the scenario file is not valid. Please check your scenario");
  }
}
}  // namespace openscenario_preprocessor

RCLCPP_COMPONENTS_REGISTER_NODE(openscenario_preprocessor::Preprocessor)
