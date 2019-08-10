/******************************************************************************
 *                       ____    _    _____                                   *
 *                      / ___|  / \  |  ___|    C++                           *
 *                     | |     / _ \ | |_       Actor                         *
 *                     | |___ / ___ \|  _|      Framework                     *
 *                      \____/_/   \_|_|                                      *
 *                                                                            *
 * Copyright 2011-2018 Dominik Charousset                                     *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License or  *
 * (at your option) under the terms and conditions of the Boost Software      *
 * License 1.0. See accompanying files LICENSE and LICENSE_ALTERNATIVE.       *
 *                                                                            *
 * If you did not receive a copy of the license files, see                    *
 * http://opensource.org/licenses/BSD-3-Clause and                            *
 * http://www.boost.org/LICENSE_1_0.txt.                                      *
 ******************************************************************************/

#pragma once

#include "caf/actor_system.hpp"
#include "caf/actor_system_config.hpp"
#include "caf/duration.hpp"
#include "caf/timestamp.hpp"

enum class test_enum {
  a,
  b,
  c,
};

struct test_data {
  test_data(int32_t i32, int64_t i64, float f32, double f64, caf::duration dur,
            caf::timestamp ts, test_enum te, const std::string& str)
    : i32_(i32),
      i64_(i64),
      f32_(f32),
      f64_(f64),
      dur_(dur),
      ts_(ts),
      te_(te),
      str_(str) {
  }

  test_data()
    : test_data(-345, -1234567890123456789ll, 3.45, 54.3,
                caf::duration(caf::time_unit::seconds, 123),
                caf::timestamp{
                  caf::timestamp::duration{1478715821 * 1000000000ll}},
                test_enum::b, "Lorem ipsum dolor sit amet.") {
  }

  int32_t i32_;
  int64_t i64_;
  float f32_;
  double f64_;
  caf::duration dur_;
  caf::timestamp ts_;
  test_enum te_;
  std::string str_;

  friend bool operator==(const test_data& data, const test_data& other) {
    return (data.f64_ == other.f64_ && data.i32_ == other.i32_
            && data.i64_ == other.i64_ && data.str_ == other.str_
            && data.te_ == other.te_ && data.ts_ == other.ts_);
  }
};

template <class Inspector>
typename Inspector::result_type inspect(Inspector& f, test_data& x) {
  return f(caf::meta::type_name("test_data"), x.i32_, x.i64_, x.f32_, x.f64_,
           x.dur_, x.ts_, x.te_, x.str_);
}

struct serialization_fixture {
  caf::actor_system_config cfg;
  caf::actor_system sys{cfg};
  test_data source;
  test_data sink{0,
                 0,
                 0,
                 0,
                 caf::duration(caf::time_unit::seconds, 0),
                 caf::timestamp{caf::timestamp::duration{0}},
                 test_enum::a,
                 ""};
};
