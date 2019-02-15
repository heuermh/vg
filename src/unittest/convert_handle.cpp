#include "catch.hpp"
#include "../convert_handle.hpp"
#include "../handle.hpp"
#include "../vg.hpp"
#include "../xg.hpp"
#include "../packed_graph.hpp"
#include "../hash_graph.hpp"

namespace vg {
    namespace unittest {
        
        using namespace std;
        
        TEST_CASE("convert_handle converter works on empty graph", "[handle][vg][xg]") {
            xg::XG xg;
            VG vg;
            convert_handle_graph(&xg, &vg);
            REQUIRE(vg.node_count() == 0);
            REQUIRE(vg.edge_count() == 0);
        }
        TEST_CASE("convert_handleconverter works on graphs with one node", "[handle][vg][xg]") {
            string graph_json = R"(
            {
                "node": [
                         {"id":1, "sequence":"GATT"}
                         ]
            }
            )";
            Graph proto_graph;
            json2pb(proto_graph, graph_json.c_str(), graph_json.size());
            
            xg::XG xg(proto_graph);
            VG vg;
            convert_handle_graph(&xg, &vg);
            
            REQUIRE(xg.node_size() == 1);
            REQUIRE(vg.node_size() == 1);
        }
        TEST_CASE("convert_handle converter works on graphs with one reversing edge", "[handle][vg][xg]") {
            string graph_json = R"(
            {
                "node": [
                         {"id":1, "sequence":"GATT"},
                         {"id":2,"sequence":"ACA"},
                         {"id":3,"sequence":"CGAT"},
                         {"id":4,"sequence":"TCGAA"}
                         ],
                "edge":[
                        {"from":1, "to":2, "from_start":false, "to_end": false},
                        {"from":2, "to":3, "from_start":false, "to_end": false},
                        {"from":3, "to":4, "from_start":false, "to_end": false},
                        {"from":4, "to":3, "from_start":false, "to_end": true}
                        ]
            }
            )";
            Graph proto_graph;
            json2pb(proto_graph, graph_json.c_str(), graph_json.size());
            
            xg::XG xg(proto_graph);
            VG vg;
            convert_handle_graph(&xg, &vg);
            
            REQUIRE(xg.node_size() == 4);
            REQUIRE(vg.node_size() == 4);
            REQUIRE(vg.edge_count() == 4);
            REQUIRE(vg.length() == 16);
            
        }
        TEST_CASE("convert_handle converter works on graphs with reversing edges and loops", "[handle][vg][xg]") {
            string graph_json = R"(
            {
                "node": [
                         {"id":1, "sequence":"GATT"},
                         {"id":2,"sequence":"ACA"},
                         {"id":3,"sequence":"CGAT"},
                         {"id":4,"sequence":"TCGAA"}
                         ],
                "edge":[
                        {"from":1, "to":2, "from_start":false, "to_end": false},
                        {"from":2, "to":3, "from_start":false, "to_end": false},
                        {"from":3, "to":4, "from_start":true, "to_end": true},
                        {"from":4, "to":1, "from_start":true, "to_end": true},
                        {"from":1, "to":1, "from_start":true, "to_end": true},
                        {"from":2, "to":3, "from_start":false, "to_end": true},
                        {"from":4, "to":4, "from_start":true, "to_end": false}
                        ]
            }
            )";
            Graph proto_graph;
            json2pb(proto_graph, graph_json.c_str(), graph_json.size());
            
            xg::XG xg(proto_graph);
            VG vg;
            convert_handle_graph(&xg, &vg);
            
            REQUIRE(xg.node_sequence(1) == "GATT");
            REQUIRE(xg.node_sequence(3) == "CGAT");
            REQUIRE(xg.node_size() == 4);
            REQUIRE(vg.node_size() == 4);
            REQUIRE(vg.edge_count() == 7);
            REQUIRE(vg.length() == 16);
        }
    }
}
