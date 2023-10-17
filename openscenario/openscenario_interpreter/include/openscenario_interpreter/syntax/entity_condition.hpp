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

#ifndef OPENSCENARIO_INTERPRETER__SYNTAX__ENTITY_CONDITION_HPP_
#define OPENSCENARIO_INTERPRETER__SYNTAX__ENTITY_CONDITION_HPP_

#include <openscenario_interpreter/scope.hpp>
#include <openscenario_interpreter/syntax/triggering_entities.hpp>
#include <openscenario_interpreter/syntax/acceleration_condition.hpp>
#include <openscenario_interpreter/syntax/collision_condition.hpp>
#include <openscenario_interpreter/syntax/distance_condition.hpp>
#include <openscenario_interpreter/syntax/reach_position_condition.hpp>
#include <openscenario_interpreter/syntax/relative_distance_condition.hpp>
#include <openscenario_interpreter/syntax/speed_condition.hpp>
#include <openscenario_interpreter/syntax/stand_still_condition.hpp>
#include <openscenario_interpreter/syntax/time_headway_condition.hpp>
#include <pugixml.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
/* ---- EntityCondition --------------------------------------------------------
 *
 *  <xsd:complexType name="EntityCondition">
 *    <xsd:choice>
 *      <xsd:element name="EndOfRoadCondition" type="EndOfRoadCondition"/>
 *      <xsd:element name="CollisionCondition" type="CollisionCondition"/>
 *      <xsd:element name="OffroadCondition" type="OffroadCondition"/>
 *      <xsd:element name="TimeHeadwayCondition" type="TimeHeadwayCondition"/>
 *      <xsd:element name="TimeToCollisionCondition" type="TimeToCollisionCondition"/>
 *      <xsd:element name="AccelerationCondition" type="AccelerationCondition"/>
 *      <xsd:element name="StandStillCondition" type="StandStillCondition"/>
 *      <xsd:element name="SpeedCondition" type="SpeedCondition"/>
 *      <xsd:element name="RelativeSpeedCondition" type="RelativeSpeedCondition"/>
 *      <xsd:element name="TraveledDistanceCondition" type="TraveledDistanceCondition"/>
 *      <xsd:element name="ReachPositionCondition" type="ReachPositionCondition"/>
 *      <xsd:element name="DistanceCondition" type="DistanceCondition"/>
 *      <xsd:element name="RelativeDistanceCondition" type="RelativeDistanceCondition"/>
 *    </xsd:choice>
 *  </xsd:complexType>
 *
 * -------------------------------------------------------------------------- */
struct EntityCondition : public ComplexType
{
  explicit EntityCondition(const pugi::xml_node &, Scope &, const TriggeringEntities &);
};

DEFINE_LAZY_VISITOR(
  const EntityCondition,
  // CASE(EndOfRoadCondition),
  CASE(CollisionCondition),
  // CASE(OffroadCondition),
  CASE(TimeHeadwayCondition),
  // CASE(TimeToCollisionCondition),
  CASE(AccelerationCondition),
  CASE(StandStillCondition),
  CASE(SpeedCondition),
  // CASE(RelativeSpeedCondition),
  // CASE(TraveledDistanceCondition),
  CASE(ReachPositionCondition),
  CASE(DistanceCondition),
  CASE(RelativeDistanceCondition),
);
}  // namespace syntax
}  // namespace openscenario_interpreter

#endif  // OPENSCENARIO_INTERPRETER__SYNTAX__ENTITY_CONDITION_HPP_
