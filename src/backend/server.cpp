#include <iostream>

#include <restinio/all.hpp>
#include <nlohmann/json.hpp>

#include "graph.hpp"
#include "io.hpp"
#include "shortest-path.hpp"

using json = nlohmann::json;


int main() {
    const Graph graph = load_graph_from_json_file("rj_graph_database.json");

    std::cout << "n_nodes = " << graph.n_nodes() << std::endl;
    std::cout << "n_edges = " << graph.n_edges() << std::endl;

    auto router = std::make_unique<restinio::router::express_router_t<>>();
    router->http_get("/sayHello",
                     [](const auto& req, const auto& params) {
                         const auto qp = restinio::parse_query(req->header().query());
                         return req->create_response()
                             .set_body("Hello, " + (std::string)qp["name"] + "!")
                             .done();
                     });
    router->http_get("/shortestPath",
                     [&graph](const auto& req, const auto& params) {
                         const auto qp = restinio::parse_query(req->header().query());

                         Node starting_point = Node {
                             .latitude = std::stod((std::string)qp["startPointLat"]),
                             .longitude = std::stod((std::string)qp["startPointLong"]),
                         };
                         Node ending_point = Node {
                             .latitude = std::stod((std::string)qp["endPointLat"]),
                             .longitude = std::stod((std::string)qp["endPointLong"]),
                         };

                         id_t starting_point_id = graph.lookup_node(starting_point).first;
                         id_t ending_point_id = graph.lookup_node(ending_point).first;

                         std::vector<id_t> path
                             = shortest_path(graph, starting_point_id, ending_point_id,
                                             euclidean_heuristic);

                         std::vector<json> path_latlongs(path.size());
                         std::transform(std::rbegin(path), std::rend(path), std::begin(path_latlongs),
                                        [&graph](id_t node_id) {
                                            const Node& node = graph[node_id];
                                            return json {
                                                {"latitude", node.latitude},
                                                {"longitude", node.longitude}
                                            };
                                        });
                         eta_t eta = 0;
                         for (size_t i = 0; i < path.size()-1; ++i) {
                             eta += graph.get_edge(path[i], path[i+1]).eta;
                         }

                         json out_json {
                             {"shortest-paths", json::array({
                                 json::object({
                                     {"eta", eta},
                                     {"path", path_latlongs},
                                 })
                             })}
                         };

                         return req->create_response()
                             .append_header(restinio::http_field_t::access_control_allow_origin, "*")
                             .set_body(out_json.dump())
                             .done();
                     });
    router->non_matched_request_handler([](auto req) {
        return req->create_response(restinio::status_not_found()).connection_close().done();
    });

    struct my_traits_t : public restinio::default_single_thread_traits_t {
        using request_handler_t = restinio::router::express_router_t<>;
    };

    const std::string address = "localhost";
    const unsigned int port = 8080;

    restinio::run(restinio::on_this_thread<my_traits_t>()
                  .address(address)
                  .port(port)
                  .request_handler(std::move(router)));
}
