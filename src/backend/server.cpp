#include <iostream>
#include <functional>
#include <chrono>

#include <restinio/all.hpp>
#include <nlohmann/json.hpp>

#include "graph.hpp"
#include "io.hpp"
#include "shortest-path.hpp"

using json = nlohmann::json;


template <typename... Args>
json get_path_json(Args... args) {
    auto [compute_time, maybe_data] = get_path_data(args...);

    if (!maybe_data)
        return json::object({{"compute-time", compute_time}});
    auto [eta, length, coord_path] = *maybe_data;

    std::vector<json> path_latlongs(coord_path.size());
    std::transform(std::rbegin(coord_path), std::rend(coord_path), std::begin(path_latlongs),
                   [](const std::pair<double, double>& latlong) {
                       return json {
                           {"latitude", latlong.first},
                           {"longitude", latlong.second}
                       };
                   });

    return json::object({
        {"eta", eta},
        {"length", length},
        {"compute-time", compute_time},
        {"path", path_latlongs}
    });
}


int main() {
    const Graph graph = load_graph_from_json_file("rj_graph_database.json");

    std::cout << "Loaded the graph." << std::endl;

    auto router = std::make_unique<restinio::router::express_router_t<>>();
    router->http_get("/shortestPath",
                     [&graph](const auto& req, const auto& params) {
                         const auto qp = restinio::parse_query(req->header().query());

                         std::pair<double, double> starting_point = {std::stod((std::string)qp["startPointLat"]),
                                                                     std::stod((std::string)qp["startPointLong"])};
                         std::pair<double, double> ending_point = {std::stod((std::string)qp["endPointLat"]),
                                                                   std::stod((std::string)qp["endPointLong"])};
                         std::string method_str = (std::string)qp["method"];
                         std::string goal_str = (std::string)qp["goal"];

                         ShortestPathMethod method;
                         if (method_str == "dijkstra")
                             method = ShortestPathMethod::Dijkstra;
                         else if (method_str == "astar-euclidean")
                             method = ShortestPathMethod::AStarEuclidean;
                         else if (method_str == "astar-manhattan")
                             method = ShortestPathMethod::AStarManhattan;
                         else
                             throw std::runtime_error("bad shortest path method");

                         std::string json_str;
                         if (goal_str == "shortest")
                             json_str
                                 = get_path_json(graph, starting_point, ending_point, method, ShortestOrFastest::get_weight_length).dump();
                         else if (goal_str == "fastest")
                             json_str
                                 = get_path_json(graph, starting_point, ending_point, method, ShortestOrFastest::get_weight_eta).dump();
                         else
                             throw std::runtime_error("bad goal");

                         return req->create_response()
                             .append_header(restinio::http_field_t::access_control_allow_origin, "*")
                             .set_body(json_str)
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
