/*
 * scout_robot.cpp
 *
 * Created on: Jul 08, 2021 11:13
 * Description:
 *
 * Copyright (c) 2021 Weston Robot Pte. Ltd.
 */

#include "ugv_sdk/mobile_base/scout_robot.hpp"
#include "ugv_sdk/details/robot_base/scout_base.hpp"

namespace westonrobot {
ScoutRobot::ScoutRobot(ProtocolType protocol) {
  if (protocol == ProtocolType::AGX_V1) {
  } else if (protocol == ProtocolType::AGX_V2) {
    robot_ = new ScoutBaseV2();
  }
}

ScoutRobot::~ScoutRobot() {
  if (robot_) delete robot_;
}

void ScoutRobot::EnableCommandedMode() { robot_->EnableCommandedMode(); }

void ScoutRobot::Connect(std::string can_name) { robot_->Connect(can_name); }

void ScoutRobot::Connect(std::string uart_name, uint32_t baudrate) {
  robot_->Connect(uart_name, baudrate);
}

void ScoutRobot::ResetRobotState() { robot_->ResetRobotState(); }

void ScoutRobot::SetMotionCommand(double linear_vel, double angular_vel) {
  auto scout = dynamic_cast<ScoutInterface*>(robot_);
  scout->SetMotionCommand(linear_vel, angular_vel);
}

void ScoutRobot::DisableLightControl() { robot_->DisableLightControl(); }

void ScoutRobot::SetLightCommand(LightMode f_mode, uint8_t f_value,
                                 LightMode r_mode, uint8_t r_value) {
  auto scout = dynamic_cast<ScoutInterface*>(robot_);
  scout->SetLightCommand(f_mode, f_value, r_mode, r_value);
}

ScoutState ScoutRobot::GetRobotState() {
  auto scout = dynamic_cast<ScoutInterface*>(robot_);
  return scout->GetRobotState();
}
}  // namespace westonrobot