// Copyright (c) 2015 SHUMEI Inc. All rights reserved.
// Authors: Liang Kun <liangkun@ishumei.com>.

#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include "storm/storm.h"

using namespace std;
using namespace storm;

namespace {

class Adder: public Bolt {
public:
    virtual void Execute(unique_ptr<Tuple> &tuple) {
        Values result;
        result.SetArray();
        stringstream ss;
        ss << tuple->values()[0].GetInt() + tuple->values()[1].GetInt();
        result.PushBack(ToValue(ss.str()), g_allocator);
        result.PushBack(tuple->mutable_values()[2], g_allocator);
        oc().Emit("default", tuple, &result);
        oc().Ack(tuple);
    }

private:
};

}  // anonymous namespace

int main(void) {
    unique_ptr<Adder> adder { new Adder() };
    adder->Run(cin, cout);
    return 0;
}
