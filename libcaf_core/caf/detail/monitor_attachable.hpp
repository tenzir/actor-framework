// This file is part of CAF, the C++ Actor Framework. See the file LICENSE in
// the main distribution directory for license terms and copyright or visit
// https://github.com/actor-framework/actor-framework/blob/main/LICENSE.

#pragma once

#include "caf/action.hpp"
#include "caf/actor.hpp"
#include "caf/actor_addr.hpp"
#include "caf/actor_cast.hpp"
#include "caf/attachable.hpp"
#include "caf/detail/monitor_action.hpp"
#include "caf/mailbox_element.hpp"

namespace caf::detail {

/// Token used to identify and remove a `monitor_attachable` via `detach()`.
struct monitor_token {
  abstract_monitor_action* key;
  static constexpr size_t token_type = attachable::token::monitor;
};

/// Attachable placed on the monitored actor. Unlike `functor_attachable` it
/// carries a matchable token so it can be removed via `abstract_actor::detach()`
/// when the monitoring actor exits or cancels the monitor.
class monitor_attachable : public attachable {
public:
  static constexpr size_t token_type = attachable::token::monitor;

  monitor_attachable(abstract_monitor_action_ptr on_down, actor_addr self)
    : on_down_(std::move(on_down)), self_(std::move(self)) {}

  void actor_exited(const error& reason, scheduler* sched) override {
    if (on_down_->set_reason(error{reason})) {
      if (auto shdl = actor_cast<actor>(self_))
        shdl->enqueue(
          make_mailbox_element(nullptr, make_message_id(), action{on_down_}),
          sched);
    }
  }

  bool matches(const token& what) override {
    if (what.subtype != token_type)
      return false;
    return static_cast<const monitor_token*>(what.ptr)->key == on_down_.get();
  }

private:
  abstract_monitor_action_ptr on_down_;
  actor_addr self_;
};

} // namespace caf::detail
