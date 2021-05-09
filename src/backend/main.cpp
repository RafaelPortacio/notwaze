#include <iostream>

#include <restinio/all.hpp>

#include "graph.hpp"
#include "io.hpp"


void serve_rest_api(const std::string& address, const unsigned int& port) {
    auto router = std::make_unique<restinio::router::express_router_t<>>();
    router->http_get("/sayHello",
                     [](const auto& req, const auto& params) {
                         return req->create_response()
                             .set_body("Hello World!")
                             .done();
                     });
    router->non_matched_request_handler([](auto req) {
        return req->create_response(restinio::status_not_found()).connection_close().done();
    });

    struct my_traits_t : public restinio::default_single_thread_traits_t {
        using request_handler_t = restinio::router::express_router_t<>;
    };

    restinio::run(restinio::on_this_thread<my_traits_t>()
                  .address(address)
                  .port(port)
                  .request_handler(std::move(router)));
}


int main() {
    const Graph graph = load_graph_from_json_file("rj_graph_database.json");

    std::cout << "n_nodes = " << graph.n_nodes() << std::endl;
    std::cout << "n_edges = " << graph.n_edges() << std::endl;

    serve_rest_api("localhost", 8080);
}
