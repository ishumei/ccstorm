// Copyright (c) 2016 SHUMEI Inc. All rights reserved.
// Authors: Liang Kun <liangkun@ishumei.com>.

#include "storm/drpc-client.h"
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <sstream>
#include <utility>
#include <vector>
#include "boost/shared_ptr.hpp"
#include "thrift/Thrift.h"
#include "thrift/transport/TSocket.h"
#include "thrift/transport/TBufferTransports.h"
#include "thrift/protocol/TBinaryProtocol.h"
#include "DistributedRPC.h"

using std::string;
using std::move;
using std::exception;
using std::runtime_error;
using std::rand;
using std::srand;
using std::time;
using boost::shared_ptr;
using apache::thrift::transport::TFramedTransport;
using apache::thrift::transport::TSocket;
using apache::thrift::protocol::TBinaryProtocol;

namespace {
using std::vector;
using std::stringstream;
using std::getline;

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

}  // anonymous namespace

namespace storm {

DRPCClient::DRPCClient(const string &hosts, int port, int timeout)
        : _port(port), _timeout(timeout) {
    DRPCClient(split(hosts, ','), port, timeout);
}

DRPCClient::DRPCClient(const vector<string> &hosts, int port, int timeout) {
    _hosts = hosts;
    if (_hosts.empty()) {
        throw runtime_error("no drpc hosts specified");
    }
    srand(time(nullptr));
    _current_host = rand() % _hosts.size();
    connect();
}

string DRPCClient::execute(const string &function, const string &args) {
    try {
        if (!_client) {
            connect();
        }
        string response;
        _client->execute(response, function, args);
        return std::move(response);
    } catch (exception &e) {
        _client.reset(nullptr);
        throw;
    }
}

void DRPCClient::connect() {
    _current_host = (_current_host + 1) % _hosts.size();
    shared_ptr<TSocket> socket(new TSocket(_hosts[_current_host], port()));
    socket->setConnTimeout(timeout());
    socket->setSendTimeout(timeout());
    socket->setRecvTimeout(timeout());
    socket->open();
    shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
    shared_ptr<TBinaryProtocol> protocol(new TBinaryProtocol(transport));
    _client.reset(new DistributedRPCClient(protocol));
}

}  // namespace storm
