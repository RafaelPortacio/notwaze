'use strict';

// Add the map
var leaflet_map = L.map('map').setView([51.505, -0.09], 13);
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: 'Map data &copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors, Imagery © <a href="https://www.mapbox.com/">Mapbox</a>',
    maxZoom: 19,
}).addTo(leaflet_map);

// Draw shortest paths, if there are any
var params = (new URL(document.location)).searchParams;

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

async function find_and_draw_path(starting_point_str, destination_str) {
    var [start_point_latitude, start_point_longitude] = await geocode_latlong(starting_point_str);
    var [end_point_latitude, end_point_longitude]     = await geocode_latlong(destination_str);

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
    L.marker([starting_point.latitude, starting_point.longitude]).addTo(leaflet_map);
    L.marker([ending_point.  latitude, ending_point.  longitude]).addTo(leaflet_map);

    // Draw lines for indicating paths
    let extents = data["shortest-paths"].map(function(path) {
        let latlongs = path["path"].map(x => [x.latitude, x.longitude]);
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

if (params.has("starting-point") && params.has("destination")) {
    let map_container = document.getElementById("map-container");
    let loader_div = document.createElement("div");
    loader_div.id = "path-loader";
    loader_div.className = "loader";
    map_container.appendChild(loader_div);

    find_and_draw_path(params.get("starting-point"), params.get("destination"))
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
