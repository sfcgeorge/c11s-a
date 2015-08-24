Pebble.addEventListener('ready', function() {});

Pebble.addEventListener('showConfiguration', function() {
  Pebble.openURL('https://rawgit.com/sfcgeorge/c11s-a/master/config/index.html');
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));

  // Send to watchapp
  Pebble.sendAppMessage(configData, function() {
    console.log('Send successful: ' + JSON.stringify(configData));
  }, function() {
    console.log('Send failed!');
  });
});
