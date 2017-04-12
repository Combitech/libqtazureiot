'use strict';

var Client = require('azure-iothub').Client;
var Message = require('azure-iot-common').Message;
var Properties = require('azure-iot-common').Properties;

var connectionString = process.env.CONNECTION_STRING;
var targetDevice = 'myDeviceId';

var serviceClient = Client.fromConnectionString(connectionString);

function printResultFor(op) {
  return function printResult(err, res) {
    if (err) console.log(op + ' error: ' + err.toString());
    if (res) console.log(op + ' status: ' + res.constructor.name);
  };
}

function receiveFeedback(err, receiver){
  receiver.on('message', function (msg) {
    console.log('Feedback message:')
    console.log(msg.getData().toString('utf-8'));
  });
}

serviceClient.open(function (err) {
  if (err) {
    console.error('Could not connect: ' + err.message);
  } else {
    console.log('Service client connected');
    serviceClient.getFeedbackReceiver(receiveFeedback);
    var message = new Message('Cloud to device message.');
    message.ack = 'full';
    message.messageId = "MyMessageID";
    message.properties.add('myproperty1', 'myvalue1');
    message.properties.add('myproperty2', 'myvalue2');
    console.log('Sending message: ' + message.getData());
    serviceClient.send(targetDevice, message, printResultFor('send'));
  }
});
