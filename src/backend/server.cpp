#include <iostream>
#include <functional>
#include <chrono>

#include <restinio/all.hpp>
#include <nlohmann/json.hpp>

#include "graph.hpp"
#include "io.hpp"
#include "shortest-path.hpp"

using json = nlohmann::json;


template <typename GetWeight>
json get_path_json(const Graph& graph,
                   const std::pair<double, double>& starting_point, const std::pair<double, double>& ending_point,
                   ShortestPathMethod method,
                   GetWeight get_weight) {
    auto [start_edge, end_edge, starting_point_id, ending_point_id, start_proj,
          end_proj, start_proj_fraction, end_proj_fraction]
        = graph.coords_to_ids(starting_point, ending_point);

    std::optional<std::vector<node_id>> maybe_path;
    auto start_time = std::chrono::high_resolution_clock::now();
    switch (method) {
        case ShortestPathMethod::Dijkstra:
            maybe_path = shortest_path_dijkstra(graph, starting_point_id, ending_point_id, get_weight);
            break;
        case ShortestPathMethod::AStarEuclidean:
            maybe_path = shortest_path_astar(graph, starting_point_id, ending_point_id, euclidean_heuristic, get_weight);
            break;
        case ShortestPathMethod::AStarManhattan:
            maybe_path = shortest_path_astar(graph, starting_point_id, ending_point_id, manhattan_heuristic, get_weight);
            break;
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    unsigned long int compute_time
        = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    if (!maybe_path)
        return json::object({{"compute-time", compute_time}});
    std::vector<node_id> path = *maybe_path;

    std::vector<std::pair<double, double>> coord_path = {};

    for (auto it = path.begin(); it != path.end(); it++)
        coord_path.push_back({graph.get_node(*it).latitude, graph.get_node(*it).longitude});
    if (end_proj.first == 2)
        coord_path.insert(coord_path.begin(),
                          {(end_proj.second).latitude, (end_proj.second).longitude});
    if (start_proj.first == 2)
        coord_path.push_back({(start_proj.second).latitude, (start_proj.second).longitude});

    std::vector<json> path_latlongs(coord_path.size());
    std::transform(std::rbegin(coord_path), std::rend(coord_path), std::begin(path_latlongs),
                   [](const std::pair<double, double>& latlong) {
                       return json {
                           {"latitude", latlong.first},
                           {"longitude", latlong.second}
                       };
                   });
    weight_t eta = 0;
    weight_t length = 0;

    for (size_t i = 0; i < path.size()-1; ++i) {
        const Edge& edge = graph.get_edge(path[i+1], path[i]);
        eta += edge.eta;
        length += edge.length;
    }

    {
        const Edge& edge = graph.get_edge(start_edge.first, start_edge.second);
        eta += edge.eta*start_proj_fraction;
        length += edge.length*start_proj_fraction;
        const Edge& edge2 = graph.get_edge(end_edge.first, end_edge.second);
        eta += edge2.eta*end_proj_fraction;
        length += edge2.length*end_proj_fraction;
    }

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
                                 = get_path_json(graph, starting_point, ending_point, method, get_weight_length).dump();
                         else if (goal_str == "fastest")
                             json_str
                                 = get_path_json(graph, starting_point, ending_point, method, get_weight_eta).dump();
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
