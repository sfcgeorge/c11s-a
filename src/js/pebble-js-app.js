var maxAttempts = 3;
var locationAttempts = 0;
var sendAttempts = 0;

Pebble.addEventListener('ready', function() {
  console.log('Pebble JS ready');
  if (localStorage.getItem('DISABLE_WEATHER') * 1 != 1) getWeather();
});

Pebble.addEventListener('showConfiguration', function() {
  Pebble.openURL('https://rawgit.com/sfcgeorge/c11s-a/master/config/index.html');
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));
  localStorage.setItem('DISABLE_WEATHER', configData.DISABLE_WEATHER);
  configData.MAIN_COLOR = parseInt(configData.MAIN_COLOR);
  configData.SECONDS_COLOR = parseInt(configData.SECONDS_COLOR);

  // Send to watchapp
  Pebble.sendAppMessage(configData, function() {
    console.log('Config send successful: ' + JSON.stringify(configData));
  }, function() {
    console.log('Config send failed!');
  });
});

Pebble.addEventListener('appmessage', function() {
  console.log('AppMessage received!');
  getWeather();
});

function getWeather() {
  locationAttempts += 1;
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    function() {
      console.log('Error requesting location! ' + locationAttempts);
      if (locationAttempts < maxAttempts) getWeather();
    },
    { timeout: 15000, maximumAge: 60000 }
  );
  locationAttempts = 0;
}

function locationSuccess(pos) {
  // Construct URL
  var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' +
      pos.coords.latitude + '&lon=' + pos.coords.longitude;
  console.log('Weather URL: ' + url);

  xhrRequest(url, 'GET', function(responseText) {
    var json = JSON.parse(responseText);

    // var conditions = json.weather[0].main;
    sendWeather({
      "WEATHER_TEMPERATURE": Math.round(json.main.temp - 273.15), // Kelvin to C
      "WEATHER_HIGH": Math.round(json.main.temp_max - 273.15),
      "WEATHER_LOW": Math.round(json.main.temp_min - 273.15),
      "WEATHER_HUMIDITY": Math.round(json.main.humidity),
      "WEATHER_WIND_SPEED": Math.round(json.wind.speed)
    });
  });
}

function sendWeather(weatherData) {
  sendAttempts += 1;
  Pebble.sendAppMessage(weatherData, function() {
    console.log('Weather send successful: ' + JSON.stringify(weatherData));
  }, function() {
    console.log('Error sending weather info to Pebble! ' + sendAttempts);
    if (sendAttempts < maxAttempts) sendWeather();
  });
  sendAttempts = 0;
}

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};
