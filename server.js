const express = require('express') //crearea rutelor
const request = require('request')
const cors = require('cors');
const fs = require('fs');
const moment = require('moment');
const app = express();
const readLastLines = require('read-last-lines');
const port = 3000

const accountSid = 'AC41ff1162b92aa2df55390698eb8517e9';
const authToken = 'e399e420c6e5b745f9e24ff76e5b4f53';
const client = require('twilio')(accountSid, authToken);


var ip = ['192.168.1.178'];
var data = [];
var send_sms = 1;

console.log("pornire server");
app.use(cors());

app.get('/', (req, resp) => { //se primeste req
	//console.log('here');
	request.get('http://'+ip[req.query.patient] + '?type='+req.query.type+'&speed='+req.query.speed, function(err, response){ //reuest.query.ceva=valoarea param"ceva"; se face req si se asteapta raspuns
		console.log(err, response.body);
		if(err) return console.log("Nu s-a putut face request la acel ip: " + ip[req.query.patient] + err);

		if(req.query.type == 5){
			let data = response.body.split("*");
			console.log(data);
			if(data[0]){
				fs.appendFile("./myBPM.txt", data[0] + " " + moment().format('DD-MM-YYYY H:m:ss') + "\r\n", function(err) {
					if(err) {
						return console.log(err);
					}
					console.log("myBPM file was saved!");
				});
			}
			if(data[1]){
				fs.appendFile("./temp.txt", data[1] + " "  + moment().format('DD-MM-YYYY H:m:ss') + "\r\n", function(err) {
					if(err) {
						return console.log(err);
					}
					console.log("Temp file was saved!");
				});
			}
			if(data[2]){
				fs.appendFile("./umd.txt", data[2] + " "  + moment().format('DD-MM-YYYY H:m:ss') + "\r\n", function(err) {
					if(err) {
						return console.log(err);
					}
					console.log("umd file was saved!");
				});
			}
			if(data[3]){
				fs.appendFile("./tempBody.txt", data[3] + " "  + moment().format('DD-MM-YYYY H:m:ss') + "\r\n", function(err) {
					if(err) {
						return console.log(err);
					}
					console.log("tempBody file was saved!");
				});
			}
			if((Number(data[0]) < 60 || Number(data[0]) > 100 ) && Number(sw) === 1){
				if(send_sms){
					send_sms = 0;
					if(tel !== 'undefined' && typeof tel !== 'undefined'){
						client.messages
						  .create({
							 body: 'Atentie! Inima unui pacient nu este in parametrii normali.',
							 from: '+12014823137',
							 to: '+4' + tel
						   })
						  .then(message => {
								console.log(message.sid)

								setTimeout(() => {
									send_sms = 1;
								}, 60 * 2 * 1000);
						  })
						  .catch(err => console.log(err));

					}
				}
			}
		}

		resp.end(response.body);	//se trimite raspunsul catre int. web
	})


	var dictionary = {
		'1': " - A mers inainte \r\n",
		'2': " - A mers inapoi \r\n",
		'3': " - A mers in stanga \r\n",
		'4': " - A mers in dreapta \r\n",
		'5': " - Cauta date \r\n"
	}

	console.log(req.query.type, typeof req.query.type);

	fs.appendFile("./history.txt", moment().format('DD-MM-YYYY H:m:ss') + dictionary[req.query.type] , function(err) {
		if(err) {
			return console.log(err);
		}
		console.log("The file was saved!");
	});
})


app.get('/history', (request, response) => {

	if(request.query.graph == 0){
		readLastLines.read('temp.txt', 10).then((lines) => response.send(lines)); //trimite unde se cere
	}
	if(request.query.graph == 1){
		readLastLines.read('umd.txt', 10).then((lines) => response.send(lines)); //trimite unde se cere
	}
	if(request.query.graph == 2){
		readLastLines.read('myBPM.txt', 10).then((lines) =>  response.send(lines)); //trimite unde se cere
	}
	if(request.query.graph == 3){
		readLastLines.read('tempBody.txt', 10).then((lines) =>  response.send(lines)); //trimite unde se cere
	}
	if(request.query.graph == 4){
		fs.readFile("./history.txt", (error, data) => {
			if(error) {
				return console.log(error);
			}
			response.send(data.toString());
		});
	}

})
var tel;
var sw=0;
readLastLines.read('telefon.txt', 2).then((lines) => tel = lines);
app.get('/data', (request, response) => {

	if(request.query.tel && request.query.sw){
		tel = request.query.tel;
		sw = request.query.sw;
		console.log(tel + " " + sw);
		if(tel && tel !== 'undefined'){
			fs.appendFile("./telefon.txt", tel + "\r\n", function(err) {
				if(err) {
					return console.log(err);
				}
			});
		}
		response.end();
	}else{
		console.log(sw);
		response.send(tel + " " + sw);
	}

})

app.listen(port, (err) => {
  if (err) {
    return console.log('something bad happened', err)
  }
  console.log(`server is listening on ${port}`)
})
