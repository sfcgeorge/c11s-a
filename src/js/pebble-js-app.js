Pebble.addEventListener('ready', function() {
  console.log('Pebble JS ready');
  getWeather();
});

Pebble.addEventListener('showConfiguration', function() {
  Pebble.openURL('https://rawgit.com/sfcgeorge/c11s-a/master/config/index.html');
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));

  // Send to watchapp
  Pebble.sendAppMessage(configData, function() {
    console.log('Config send successful: ' + JSON.stringify(configData));
  }, function() {
    console.log('Config send failed!');
  });
});

Pebble.addEventListener('appmessage',
  function() {
    console.log('AppMessage received!');
    getWeather();
  }
);

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    { timeout: 15000, maximumAge: 60000 }
  );
}

function locationSuccess(pos) {
  // Construct URL
  var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' +
      pos.coords.latitude + '&lon=' + pos.coords.longitude;

  xhrRequest(url, 'GET', function(responseText) {
    var json = JSON.parse(responseText);

    // Temperature in Kelvin requires adjustment
    var temperature = Math.round(json.main.temp - 273.15);

    // Conditions
    var conditions = json.weather[0].main;

    var weatherData = {
      "WEATHER_TEMPERATURE": Math.round(json.main.temp - 273.15), // Kelvin to C
      "WEATHER_HIGH": Math.round(json.main.temp_max - 273.15),
      "WEATHER_LOW": Math.round(json.main.temp_min - 273.15),
      "WEATHER_HUMIDITY": Math.round(json.main.humidity),
      "WEATHER_WIND_SPEED": Math.round(json.wind.speed)
    };

    Pebble.sendAppMessage(weatherData, function() {
      console.log('Weather send successful: ' + JSON.stringify(weatherData));
    }, function() {
      console.log('Error sending weather info to Pebble!');
    });
  });
}

function locationError() {
  console.log('Error requesting location!');
}

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};
