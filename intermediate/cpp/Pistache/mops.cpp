
#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"
#ifdef __linux__
#include <vector>
#include <signal.h>
#include <unistd.h>
#endif

#include "HelloServiceApiImpl.h"

#define PISTACHE_SERVER_THREADS     2
#define PISTACHE_SERVER_MAX_REQUEST_SIZE 32768
#define PISTACHE_SERVER_MAX_RESPONSE_SIZE 32768

static Pistache::Http::Endpoint *httpEndpoint;
#ifdef __linux__
static void sigHandler [[noreturn]] (int sig){
    switch(sig){
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
        case SIGHUP:
        default:
            httpEndpoint->shutdown();
            break;
    }
    exit(0);
}

static void setUpUnixSignals(std::vector<int> quitSignals) {
    sigset_t blocking_mask;
    sigemptyset(&blocking_mask);
    for (auto sig : quitSignals)
        sigaddset(&blocking_mask, sig);

    struct sigaction sa;
    sa.sa_handler = sigHandler;
    sa.sa_mask    = blocking_mask;
    sa.sa_flags   = 0;

    for (auto sig : quitSignals)
        sigaction(sig, &sa, nullptr);
}
#endif

using namespace org::openapitools::server::api;

int main() {
#ifdef __linux__
    std::vector<int> sigs{SIGQUIT, SIGINT, SIGTERM, SIGHUP};
    setUpUnixSignals(sigs);
#endif
    Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(8080));

    httpEndpoint = new Pistache::Http::Endpoint((addr));
    auto router = std::make_shared<Pistache::Rest::Router>();

    auto opts = Pistache::Http::Endpoint::options()
        .threads(PISTACHE_SERVER_THREADS);
    opts.flags(Pistache::Tcp::Options::ReuseAddr);
    opts.maxRequestSize(PISTACHE_SERVER_MAX_REQUEST_SIZE);
    opts.maxResponseSize(PISTACHE_SERVER_MAX_RESPONSE_SIZE);
    httpEndpoint->init(opts);

    
    HelloServiceApiImpl HelloServiceApiserver(router);
    HelloServiceApiserver.init();

    httpEndpoint->setHandler(router->handler());
    httpEndpoint->serve();

    httpEndpoint->shutdown();

}
