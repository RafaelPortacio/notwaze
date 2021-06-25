#include <iostream>
#include <functional>

#include <restinio/all.hpp>
#include <nlohmann/json.hpp>

#include "graph.hpp"
#include "io.hpp"
#include "shortest-path.hpp"

using json = nlohmann::json;


int main() {
    const Graph graph = load_graph_from_json_file("rj_graph_database.json");
    
    std::cout << "Loaded the graph." << std::endl;
    
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
                         std::string method = (std::string)qp["method"];

                         
                         std::pair<node_id,node_id> start_edge = graph.lookup_nodes(starting_point);
                         std::pair<node_id,node_id> end_edge = graph.lookup_nodes(ending_point);
                                                 
                         Node start_node_1 = graph.get_node(start_edge.first);
                         Node start_node_2 = graph.get_node(start_edge.second);
                         Node end_node_1 = graph.get_node(end_edge.first);
                         Node end_node_2 = graph.get_node(end_edge.second);
                         
                         std::pair<int, Node> start_proj = projection(start_node_1, start_node_2, starting_point);
                         std::pair<int, Node> end_proj = projection(end_node_1, end_node_2, ending_point);
                         
                         node_id starting_point_id = start_edge.second;
                         node_id ending_point_id = end_edge.first;
                         
                         
                         if(start_proj.first == 0) {
                             starting_point_id = start_edge.first;
                         }
                         
                         if(end_proj.first == 1) {
                             ending_point_id = end_edge.second;
                         }
                         
                        
                         
                         

                         std::function<weight_t(const Node&, const Node&)> heuristic;

                         std::optional<std::vector<node_id>> maybe_path;
                         if (method == "dijkstra") {
                             maybe_path = shortest_path_dijkstra(graph, starting_point_id, ending_point_id, get_weight_length);
                         } else if (method == "astar-euclidean") {
                             maybe_path = shortest_path_astar(graph, starting_point_id, ending_point_id, euclidean_heuristic, get_weight_length);
                         } else if (method == "astar-manhattan") {
                             maybe_path = shortest_path_astar(graph, starting_point_id, ending_point_id, manhattan_heuristic, get_weight_length);
                         } else {
                             throw std::runtime_error("Bad method");
                         }

                         json out_json;
                         if (maybe_path) {
                             
                             std::vector<node_id> path = *maybe_path;
                             std::vector<std::pair<double, double>> coord_path = {};
                             
                             
                             for(auto it = path.begin(); it != path.end(); it++) {
                                coord_path.push_back({graph.get_node(*it).latitude, graph.get_node(*it).longitude});
                             }
                             if(end_proj.first == 2){
                                coord_path.push_back({(end_proj.second).latitude,
                                                      (end_proj.second).longitude});
                             }
                             if(start_proj.first == 2){
                                coord_path.insert(coord_path.begin(),
                                                  {(start_proj.second).latitude,
                                                   (start_proj.second).longitude});
                             }

                             std::vector<json> path_latlongs(coord_path.size());
                             std::transform(std::rbegin(coord_path), std::rend(coord_path), std::begin(path_latlongs),
                                            [](const std::pair<double, double>& latlong) {
                                                return json {
                                                    {"latitude", latlong.first},
                                                    {"longitude", latlong.second}
                                                };
                                            });
                             weight_t eta = 0;
                             for (size_t i = 0; i < path.size()-1; ++i) {
                                 eta += graph.get_edge(path[i], path[i+1]).eta;
                             }

                             json&& json {
                                 {"shortest-paths", json::array({
                                     json::object({
                                         {"eta", eta},
                                         {"path", path_latlongs},
                                     })
                                 })}
                             };
                             out_json = json;
                         } else {
                             json&& json {
                                 {"shortest-paths", json::array({})}
                             };
                             out_json = json;
                         }

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
