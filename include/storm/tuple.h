// Copyright (c) 2015 SHUMEI Inc. All rights reserved.
// Authors: Liang Kun <liangkun@ishumei.com>.

#ifndef STORM_TUPLE_H
#define STORM_TUPLE_H

#include <string>
#include <sstream>
#include <utility>
#include "storm/values.h"

namespace storm {

class Tuple {
public:
    Tuple(std::string &&id,
          std::string &&component,
          std::string &&stream,
          int task,
          Values &&values
    ): _id(std::move(id)),
       _component(std::move(component)),
       _stream(std::move(stream)),
       _task(task),
       _values(std::move(values)) {}

    Tuple(Values &&values): _id("0"), _component(""), _stream("default"), _task(0) {
        _values = values;
    }

    const std::string &id() const { return _id; }
    const std::string &component() const { return _component; }
    const std::string &stream() const { return _stream; }
    int task() const { return _task; }
    const Values &values() const { return _values; }
    Values &mutable_values() { return _values; }
    
    const std::string debug_string() const {
        std::stringstream ss;
        ss << "id = " << id()
           << ", component = " << component()
           << ", stream = " << stream()
           << ", value = " << storm::json::ValueToString(values());
        return ss.str();
    }

private:
    std::string _id;
    std::string _component;
    std::string _stream;
    int _task;
    Values _values;
};

}  // namespace storm

#endif  // STORM_TUPLE_H
