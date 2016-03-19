// Copyright (c) 2015 SHUMEI Inc. All rights reserved.
// Authors: Liang Kun <liangkun@ishumei.com>.

#ifndef STORM_ICOMPONENT_H
#define STORM_ICOMPONENT_H

#include <memory>
#include <iostream>
#include "storm/exception.h"
#include "storm/topology-context.h"
#include "storm/internal/protocol.h"

namespace storm {

// Base class of storm components(Bolt or Spout).
class Component {
public:
    Component(): _is(nullptr), _os(nullptr), _enable_debug(false) {}  // default constructor
    virtual ~Component() {}

    virtual void Run(std::istream &is, std::ostream &os) {
        _is = &is;
        _os = &os;
        _tc.reset(internal::protocol::InitialHandshake(is, os));
        auto debug_it = tc().config().FindMember("topology.debug");
        if (debug_it != tc().config().MemberEnd() && debug_it->value.IsBool()) {
            _enable_debug = debug_it->value.GetBool();
        }
    }

    std::istream &is() { return *_is; }
    std::ostream &os() { return *_os; }
    const TopologyContext &tc() { return *_tc; }
    bool enable_debug() const { return _enable_debug; }

protected:
    void Log(const std::string &msg) { internal::protocol::EmitLog(msg, *_os); }

private:
    // Component does not own _is and _os
    std::istream *_is;
    std::ostream *_os;
    std::unique_ptr<TopologyContext> _tc;
    bool _enable_debug;
};

}

#endif // STORM_ICOMPONENT_H
