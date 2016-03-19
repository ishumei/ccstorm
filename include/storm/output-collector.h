// Copyright (c) 2015 SHUMEI Inc. All rights reserved.
// Authors: Liang Kun <liangkun@ishumei.com>.

#ifndef STORM_OUTPUT_COLLECTOR_H
#define STORM_OUTPUT_COLLECTOR_H

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include "storm/values.h"
#include "storm/internal/protocol.h"

namespace storm {

class OutputCollector {
public:
    OutputCollector(std::ostream &os, bool enable_debug = false):
            _os(os), _enable_debug(enable_debug) {}

    void Ack(const std::unique_ptr<Tuple> &input) {
        internal::protocol::EmitAck(input->id(), _os);
    }

    void Fail(const std::unique_ptr<Tuple> &input) {
        internal::protocol::EmitFail(input->id(), _os);
    }

    void Emit(const std::string &stream, const std::unique_ptr<Tuple> &anchor, Values *output) {
        internal::protocol::EmitTuple(stream, anchor.get(), output, _os);
        if (_enable_debug) {
            internal::protocol::EmitLog(
                    std::string("Emitting: ") + storm::json::ValueToString(*output), _os);
        }
    }

    void Emit(const std::string &stream, const std::vector<const Tuple*> &anchors, Values *output) {
        internal::protocol::EmitTuple(stream, anchors, output, _os);
        if (_enable_debug) {
            internal::protocol::EmitLog(
                    std::string("Emitting: ") + storm::json::ValueToString(*output), _os);
        }
    }

private:
    std::ostream &_os;
    bool _enable_debug;
};

}  // namespace storm

#endif  // STORM_OUTPUT_COLLECTOR_H
