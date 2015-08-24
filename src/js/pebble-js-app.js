Pebble.addEventListener('ready', function() {});

Pebble.addEventListener('showConfiguration', function() {
  Pebble.openURL('https://rawgit.com/sfcgeorge/c11s-a/master/config/index.html');
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));

  // var dict = {};
  // if(configData['high_contrast'] === true) {
    // dict['KEY_HIGH_CONTRAST'] = configData['high_contrast'];
  // } else {
    // dict['KEY_COLOR_RED'] = parseInt(backgroundColor.substring(2, 4), 16);
    // dict['KEY_COLOR_GREEN'] = parseInt(backgroundColor.substring(4, 6), 16);
    // dict['KEY_COLOR_BLUE'] = parseInt(backgroundColor.substring(6), 16);
  // }

  // Send to watchapp
  Pebble.sendAppMessage(configData, function() {
    console.log('Send successful: ' + JSON.stringify(configData));
  }, function() {
    console.log('Send failed!');
  });
});
