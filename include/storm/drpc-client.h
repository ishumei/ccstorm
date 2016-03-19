// Copyright (c) 2016 SHUMEI Inc. All rights reserved.
// Authors: Liang Kun <liangkun@ishumei.com>.
#ifndef STORM_DRPC_CLIENT_H
#define STORM_DRPC_CLIENT_H

#include <string>
#include <vector>

class DistributedRPCClient;

namespace storm {

class DRPCClient {
public:
    DRPCClient(const std::string &hosts, int port, int timeout);
    DRPCClient(const std::vector<std::string> &hosts, int port, int timeout);
    ~DRPCClient();

    std::string execute(const std::string &function, const std::string &args);

    const std::vector<std::string> &hosts() { return _hosts; }
    int port() { return _port; }
    int timeout() { return _timeout; }

private:
    void connect();

    size_t _current_host;
    std::vector<std::string> _hosts;
    int _port;
    int _timeout;
    DistributedRPCClient *_client;
};

}  // namespace storm

#endif  // STORM_DRPC_CLIENT_H
