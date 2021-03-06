/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <units/units.h>
#include <wpi/MathExtras.h>

namespace frc {
/**
 * A helper class that computes feedforward outputs for a simple elevator
 * (modeled as a motor acting against the force of gravity).
 */
template <class Distance>
class ElevatorFeedforward {
  using Velocity =
      units::compound_unit<Distance, units::inverse<units::seconds>>;
  using Acceleration =
      units::compound_unit<Velocity, units::inverse<units::seconds>>;
  using kv_unit = units::compound_unit<units::volts, units::inverse<Velocity>>;
  using ka_unit =
      units::compound_unit<units::volts, units::inverse<Acceleration>>;

 public:
  ElevatorFeedforward() = default;

  /**
   * Creates a new ElevatorFeedforward with the specified gains.
   *
   * @param kS The static gain, in volts.
   * @param kG The gravity gain, in volts.
   * @param kV The velocity gain, in volt seconds per distance.
   * @param kA The acceleration gain, in volt seconds^2 per distance.
   */
  ElevatorFeedforward(units::volt_t kS, units::volt_t kG,
                      units::unit_t<kv_unit> kV,
                      units::unit_t<ka_unit> kA = units::unit_t<ka_unit>(0))
      : m_kS(kS), m_kG(kG), m_kV(kV), m_kA(kA) {}

  /**
   * Calculates the feedforward from the gains and setpoints.
   *
   * @param velocity     The velocity setpoint, in distance per second.
   * @param acceleration The acceleration setpoint, in distance per second^2.
   * @return The computed feedforward, in volts.
   */
  units::volt_t Calculate(units::unit_t<Velocity> velocity,
                          units::unit_t<Acceleration> acceleration =
                              units::unit_t<Acceleration>(0)) {
    return m_kS * wpi::sgn(velocity) + m_kG + m_kV * velocity +
           m_kA * acceleration;
  }

 private:
  units::volt_t m_kS{0};
  units::volt_t m_kG{0};
  units::unit_t<kv_unit> m_kV{0};
  units::unit_t<ka_unit> m_kA{0};
};
}  // namespace frc
