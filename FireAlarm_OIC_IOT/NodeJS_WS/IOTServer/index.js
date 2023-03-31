var request = require('request');
const express = require("express");
var http = require('http');
var path = require("path");
const bodyParser = require("body-parser")
var Firestatus;
// New app using express module


const app = express();
app.use(bodyParser.urlencoded({
    extended:true
}));

app.use(express.json());
app.use(express.urlencoded());
const server = http.createServer(app);
const WebSocket = require('ws');
const s = new WebSocket.Server({ server });

app.use(express.json());
app.use(express.urlencoded());
require('dns').lookup(require('os').hostname(), function (err, add, fam) {
    console.log('addr: '+add);
  })

app.post("/IOTServer", function(req, res) {
    Firestatus = (req.body.FireStatus);
    console.log(req.body);
    console.log(Firestatus);
    var options = {
        'method': 'POST',
        'url': 'https://oic-dev-orasenatdpltsecitom03-ia.integration.ocp.oraclecloud.com/ic/api/integration/v1/flows/rest/IOTFIREALARM/1.0/IOT',
        'headers': {
          'Authorization': 'Basic YXBpdXNlcjpFQlNpbnRlZ3JhdGVAMjAyNA==',
          'Content-Type': 'application/json'
        },
        body: JSON.stringify({
          "FireStatus": Firestatus
        })
      
      };
      request(options, function (error, response) {
        if (error) throw new Error(error);
        console.log(response.body);
      });
    
    res.json("Success");

});
  
app.listen(3000, function(){
  console.log("server is running on port 3000");
})

