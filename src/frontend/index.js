'use strict';

// Add the map
var leaflet_map = L.map('map').setView([-22.941901818500057, -43.18015336990357], 12);
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: 'Map data &copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors, Imagery © <a href="https://www.mapbox.com/">Mapbox</a>',
    maxZoom: 19,
}).addTo(leaflet_map);

// Draw shortest paths, if there are any
var url = new URL(document.location);
var params = url.searchParams;

class GeocodingError extends Error {
    constructor(place_str) {
        super("Geocoding failed for '" + place_str + "'");
        this.name = "GeocodingError";
    }
}

class BackendUnavailableError extends Error {
    constructor() {
        super("Could not connect to the backend.");
        this.name = "BackendUnavailableError";
    }
}

async function geocode_latlong(place_str) {
    var url = "https://nominatim.openstreetmap.org/search?"
        + "street=" + encodeURI(place_str)
        + "&city=Rio+de+Janeiro"
        + "&country=Brasil"
        + "&format=json";
    var response = await fetch(url);
    var response = await response.json();
    console.log(response);

    if (response.length == 0) {
        await Promise.reject(new GeocodingError(place_str));
    }

    var place = response[0];

    return [place.lat, place.lon];
}

async function find_and_draw_path(starting_point, destination) {
    if (typeof starting_point === "string")
        var [start_point_latitude, start_point_longitude] = await geocode_latlong(starting_point);
    else
        var [start_point_latitude, start_point_longitude] = starting_point;
    if (typeof destination === "string")
        var [end_point_latitude, end_point_longitude] = await geocode_latlong(destination);
    else
        var [end_point_latitude, end_point_longitude] = destination;

    var url = "http://localhost:8080/shortestPath?"
        + "startPointLat=" + encodeURI(start_point_latitude)
        + "&startPointLong=" + encodeURI(start_point_longitude)
        + "&endPointLat=" + encodeURI(end_point_latitude)
        + "&endPointLong=" + encodeURI(end_point_longitude);
    try {
        var data = await fetch(url);
    } catch (err) {
        await Promise.reject(new BackendUnavailableError());
    }
    var data = await data.json();

    var starting_point = data["shortest-paths"][0]["path"][0];
    var ending_point =   data["shortest-paths"][0]["path"][data["shortest-paths"][0]["path"].length-1];

    // Add markers
    let marker_opts = {draggable: true, autoPan: true}
    let marker_start = L.marker([starting_point.latitude, starting_point.longitude], marker_opts).addTo(leaflet_map);
    let marker_end = L.marker([ending_point.  latitude, ending_point.  longitude], marker_opts).addTo(leaflet_map);
    function dragEnd(ev) {
        let start = marker_start.getLatLng();
        let end   = marker_end.getLatLng();
        window.location.assign(window.location.origin + "/?"
                               + "start-lat=" + start.lat
                               + "&start-long=" + start.lng
                               + "&end-lat=" + end.lat
                               + "&end-long=" + end.lng);
    }
    marker_start.on("dragend", dragEnd);
    marker_end.on("dragend", dragEnd);

    // Draw lines for indicating paths
    let extents = data["shortest-paths"].map(function(path) {
        let latlongs = path["path"].map(x => [x.latitude, x.longitude]);
        latlongs.unshift([starting_point.latitude, starting_point.longitude]);
        latlongs.push   ([  ending_point.latitude,   ending_point.longitude]);
        let polyline = L.polyline(latlongs, {
            color: "blue",
            weight: 7,
            opacity: 0.5,
        }).addTo(leaflet_map).bindPopup("ETA: " + humanizeDuration(1000 * Math.round(path["eta"])), {
            autoPan: false,
        });
        polyline.on("mouseover", function(ev) {
            polyline.openPopup();
            polyline.getPopup().setLatLng(ev.latlng);
        });
        polyline.on("mouseout",  function(ev) {
            polyline.closePopup();
        });
        return polyline.getBounds();
    });
    leaflet_map.fitBounds(extents.reduce((acc, x) => acc.extend(x)));
}

document.getElementById("starting-point-input").value = params.get("starting-point", "");
document.getElementById("destination-input").value = params.get("destination", "");

let has_text_inputs = params.has("starting-point") && params.has("destination");
let has_coord_inputs = params.has("start-lat") && params.has("start-long") && params.has("end-lat") && params.has("end-long");

if (has_text_inputs || has_coord_inputs) {
    let map_container = document.getElementById("map-container");
    let loader_div = document.createElement("div");
    loader_div.id = "path-loader";
    loader_div.className = "loader";
    map_container.appendChild(loader_div);

    let starting_point, destination;
    if (has_text_inputs) {
        starting_point = params.get("starting-point");
        destination = params.get("destination");
    } else {
        starting_point = [params.get("start-lat"), params.get("start-long")];
        destination = [params.get("end-lat"), params.get("end-long")];
    }

    find_and_draw_path(starting_point, destination)
        .then(_ => document.getElementById("path-loader").remove())
        .catch(err => {
            if (err instanceof GeocodingError) {
                alert(err.message);
            } else if (err instanceof BackendUnavailableError) {
                alert(err.message);
            } else {
                throw err;
            }
        });
}
