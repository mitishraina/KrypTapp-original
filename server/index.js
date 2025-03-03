import { storeIdentity, verifyIdentity } from './p2pVerification.js';
const express = require("express")
const { SerialPort } = require("serialport")
const { ReadlineParser } = require("@serialport/parser-readline")
const cors = require("cors")

const app = express()
app.use(cors())
const port1 = 4000

console.log(server running on port http://localhost:4000)

const port = new SerialPort({
	path: "COM5", //change this to the port your arduino is connected to
	baudRate: 9600,
})

var data1, data2, data3

app.get("/connect", async (req, res) => {
	await connectDevice()
	res.sendStatus(200)
})

app.get("/retrieve", async (req, res) => {
	await reciveData()
	setTimeout(() => {
		res.status(200).json({
			encryptedPrivateKey: data1,
			decryptSalt: data2,
			decryptIV: data3,
		})
	}, 1000)
})
// app.get("/", async (req, res) => {
//   await connectDevice();

//   setTimeout(() => {
//     reciveData();
//   }, 5000);

//   setTimeout(() => {
//     res.send(data1 + "," + data2 + "," + data3);
//   }, 7000);
// });

async function connectDevice() {
	port.write("SecretPassword", function (err) {
		if (err) {
			return console.log("Error on write: ", err.message)
		}
		setTimeout(() => {
			console.log("Connection made")
		}, 1000)
	})

	port.on("error", function (err) {
		console.log("Error: ", err.message)
	})
}

async function reciveData() {
	var data = ""
	var counter = 0
	const parser = port.pipe(new ReadlineParser({ delimiter: "\r\n" }))

	port.write("SEND", function (err) {
		if (err) {
			return console.log("Error on write: ", err.message)
		}
		setTimeout(() => {
			console.log("Initiated data transfer")
		}, 1000)
	})

	port.on("error", function (err) {
		console.log("Error: ", err.message)
	})

	parser.on("data", (x) => {
		data = x
		counter++

		if (counter == 1) {
			data1 = data
		}
		if (counter == 2) {
			data2 = data
		}

		if (counter == 3) {
			data3 = data
			console.log("Data transfer complete")
			port.close()
		}
	})
}
app.post('/register', async (req, res) => {
	const { userId, encryptedIdentity } = req.body;
	await storeIdentity(userId, encryptedIdentity);
	res.send({ message: 'Identity stored successfully' });
  });
  
  // Verify Identity
  app.post('/verify', async (req, res) => {
	const { userId } = req.body;
	const identity = await verifyIdentity(userId);
	if (identity) {
	  res.send({ verified: true, data: identity });
	} else {
	  res.status(404).send({ verified: false, message: 'Identity not found' });
	}
  });
app.listen(port1, () => {
	console.log(Server is running on port ${port1})
})
